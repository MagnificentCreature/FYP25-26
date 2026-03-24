from saleae.analyzers import HighLevelAnalyzer, AnalyzerFrame, NumberSetting, ChoicesSetting
from saleae.data import SaleaeTimeDelta

class Hla(HighLevelAnalyzer):

    center_frequency_khz = NumberSetting(min_value=0.001, max_value=1000000.0)
    bitrate_bps = NumberSetting(min_value=1.0, max_value=100000000.0)
    bit_order = ChoicesSetting(choices=('MSB First', 'LSB First'))
    tolerance_percent = NumberSetting(min_value=0.0, max_value=100.0)


    # Format the bubble to show the hex value
    result_types = {
        'fsk_byte': {
            'format': '{{data.hex_value}}'
        }
    }

    def __init__(self):
        # Keep tracking variables safely isolated inside this specific analyzer
        self.current_bit_start = None
        self.zero_count = 0
        self.one_count = 0
        
        self.current_byte_start = None
        self.bit_buffer = ""

        # Calculate half-cycle threshold: T = 1 / (2 * Frequency)
        freq_hz = self.center_frequency_khz * 1_000
        self.threshold = SaleaeTimeDelta(1.0 / (2.0 * freq_hz))

        # Calculate bit width: T = 1 / Bitrate
        # We multiply by 0.95 (95%) to catch the exact physical edge just before 
        # the theoretical boundary, exactly like the hardcoded version did!
        period_sec = 1.0 / self.bitrate_bps
        self.target_width = SaleaeTimeDelta(period_sec * (1 - self.tolerance_percent/100))

    def decode(self, frame):
        # If this is the very first pulse, synchronize the clock
        if self.current_bit_start is None:
            self.current_bit_start = frame.start_time

        # threshold = SaleaeTimeDelta(0.000000225) 
        # target_width = SaleaeTimeDelta(0.00000095) 

        # Tally the half-cycles from Simple Parallel
        if (frame.end_time - frame.start_time) < self.threshold:
            self.one_count += 1
        else:
            self.zero_count += 1

        # When we hit the 1us boundary, finalize the bit internally
        if (frame.end_time - self.current_bit_start) >= self.target_width:
            bit_val = '1' if self.one_count > self.zero_count else '0'
            
            # If this is the first bit of a new byte, record the start time
            if len(self.bit_buffer) == 0:
                self.current_byte_start = self.current_bit_start
                
            self.bit_buffer += bit_val
            res = None
            byte_str = " "            
            # Once we have collected 8 bits, output the byte frame!
            if len(self.bit_buffer) == 8:
                # Convert to Hex (Assumes MSB first. If LSB first, use self.bit_buffer[::-1])
                if len(self.bit_buffer) == 8:
                    # Apply the user's dropdown choice
                    if self.bit_order == 'LSB First':
                        byte_str = self.bit_buffer[::-1] # Reverses the string
                    else:
                        byte_str = self.bit_buffer     # Keeps it MSB First

                byte_int = int(byte_str, 2)
                hex_str = f"0x{byte_int:02X}"
                
                # Create a frame that spans the entire 8us duration
                res = AnalyzerFrame('fsk_byte', self.current_byte_start, frame.end_time, {
                    'hex_value': hex_str
                })
                
                # Empty the buffer for the next byte
                self.bit_buffer = ""
            
            # Reset bit clock for the next bit
            self.current_bit_start = frame.end_time
            self.zero_count = 0
            self.one_count = 0
            
            return res 
        
        return None
