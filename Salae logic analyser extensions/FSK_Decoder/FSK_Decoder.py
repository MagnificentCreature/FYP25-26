# High Level Analyzer
# For more information and documentation, please go to https://support.saleae.com/extensions/high-level-analyzer-extensions

from saleae.analyzers import HighLevelAnalyzer, AnalyzerFrame, StringSetting, NumberSetting, ChoicesSetting
from saleae.data import SaleaeTimeDelta

# Global variables to remember the state across multiple cycles
last_bit = None
bit_start_time = None
current_bit_start = None
zero_count = 0
one_count = 0
# High level analyzers must subclass the HighLevelAnalyzer class.
class Hla(HighLevelAnalyzer):
    # List of settings that a user can set for this High Level Analyzer.
    center_frequency_khz = NumberSetting(min_value=0.001, max_value=1000000.0)
    bitrate_bps = NumberSetting(min_value=1.0, max_value=100000000.0)
    tolerance_percent = NumberSetting(min_value=0.0, max_value=100.0)

    # An optional list of types this analyzer produces, providing a way to customize the way frames are displayed in Logic 2.
    result_types = {
        'fsk_data': {
            'format': '{{data.bit}}'
        }
    }

    def __init__(self):
        self.current_bit_start = None
        self.zero_count = 0
        self.one_count = 0

        # --- 2. TRANSLATE INPUTS INTO TIME DELTAS ---
        
        # Calculate half-cycle threshold: T = 1 / (2 * Frequency)
        freq_hz = self.center_frequency_khz * 1_000
        self.threshold = SaleaeTimeDelta(1.0 / (2.0 * freq_hz))

        # Calculate bit width: T = 1 / Bitrate
        # We multiply by 0.95 (95%) to catch the exact physical edge just before 
        # the theoretical boundary, exactly like the hardcoded version did!
        period_sec = 1.0 / self.bitrate_bps
        self.target_width = SaleaeTimeDelta(period_sec * (1 - self.tolerance_percent/100))

    def decode(self, frame):
        global current_bit_start, zero_count, one_count
        
        if current_bit_start is None:
            current_bit_start = frame.start_time

        # We are now measuring HALF-cycles.
        # Threshold between 200ns (2.5MHz) and 250ns (2.0MHz) is 225ns.
        # threshold = SaleaeTimeDelta(0.000000225) 
        
        # We target slightly less than 1us (950ns) so we catch the exact edge 
        # that completes the 1us block, ignoring minor hardware jitter.
        # target_width = SaleaeTimeDelta(0.00000095) 

        # Tally the current half-cycle
        if (frame.end_time - frame.start_time) < self.threshold:
            one_count += 1
        else:
            zero_count += 1

        # Check if this edge brings our accumulated time to ~1 microsecond
        if (frame.end_time - current_bit_start) >= self.target_width:
            
            # Determine bit based on the majority of the half-cycles
            bit_val = '1' if one_count > zero_count else '0'
            
            # Create a bubble that snaps EXACTLY to the physical edge of the frame
            res = AnalyzerFrame('fsk_data', current_bit_start, frame.end_time, {
                'bit': bit_val
            })
            
            # Reset the start time to this exact physical edge
            current_bit_start = frame.end_time
            zero_count = 0
            one_count = 0
            
            return res
        
        return None
