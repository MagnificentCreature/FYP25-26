#include "CH57x_common.h"
#include "utility.h"

uint8_t RxBuff[100];

//We plan to have 1MHz bitrate
//We plan to have f0=2MHz, f1=2.5MHz
//That is going to be 20 SPI bits per BLE bit
//Because its so fast, we have to use DMA, so
//11111000001111100000 at 20MHz defines a f0
//11110000111100001111 at 20MHz defines a f1

// #define  g_50ns  (FREQ_SYS/5) //100MHz/5 = 20MHz => 50ns
//This means 5 clock ticks at 100MHz will gives us 5ns

#define TICKS_FOR_20MHz FREQ_SYS/20000000 // 5 if clock is 100,000,000 

#define SPI_BITS_PER_BLE_BIT 20

// Half-periods in terms of 20 MHz SPI ticks
#define F0_HALF_PERIOD_TICKS 5   // For 2.0 MHz (10 ticks total per cycle)
#define F1_HALF_PERIOD_TICKS 4   // For 2.5 MHz (8 ticks total per cycle)

#define HEADER_LEN 10
uint8_t ble_packet[HEADER_LEN] = {
    0xAA, 0xAA, 0xAA, 0xAA,    /* preamble */
    0x93, 0x0B, 0x51, 0xDE,    /* sync / access address */
    0x00,                       /* length  (set by add_header) */
    0x00                        /* seq num (set by add_header) */
};

#define BUFFER_LEN (HEADER_LEN * SPI_BITS_PER_BLE_BIT * 8 / 32) //times 8 for bits per byte, divide by 32 since array is 32 bit int
#define BUFFER_LEN_8 (HEADER_LEN * SPI_BITS_PER_BLE_BIT) //times 8 for bits per byte, divide by 32 since array is 32 bit int
__attribute__((aligned(4))) uint32_t spi_tx_buffer[BUFFER_LEN];
__attribute__((aligned(4))) uint8_t spi_tx_buffer_uint8[BUFFER_LEN_8];

/**
 * @brief Expands a standard BLE packet into a phase-continuous 2FSK 32-bit SPI buffer.
 * @param ble_packet     Pointer to the raw BLE data bytes.
 * @param num_ble_bits   Total number of BLE bits to transmit.
 * @param spi_tx_buffer  Allocated 32-bit output buffer.
 */
void generate_fsk_buffer(const uint8_t* ble_packet, size_t num_ble_bits, uint32_t* spi_tx_buffer) {
    size_t spi_bit_index = 0;
    
    // State machine variables to maintain phase continuity
    uint8_t current_level = 1;          
    uint8_t ticks_in_current_level = 0; 
    for (size_t bit_idx = 0; bit_idx < num_ble_bits; bit_idx++) {
        
        // 1. Extract the current BLE data bit (BLE standard expects LSB first)
        uint8_t byte_idx = bit_idx / 8;
        uint8_t bit_pos = bit_idx % 8;
        uint8_t current_ble_bit = (ble_packet[byte_idx] >> bit_pos) & 0x01;

        // 2. Set the toggle threshold
        uint8_t half_period_ticks = current_ble_bit ? F1_HALF_PERIOD_TICKS : F0_HALF_PERIOD_TICKS;

        // 3. Generate the 20 DMA ticks for this 1-microsecond BLE bit
        for (uint8_t tick = 0; tick < SPI_BITS_PER_BLE_BIT; tick++) {
            
            // --- Pure 32-bit Linear Mapping ---
            size_t word_idx = spi_bit_index / 32;
            uint8_t bit_shift = spi_bit_index % 32; // Goes cleanly from 0 to 31

            // Write the voltage level into the 32-bit buffer
            if (current_level) {
                spi_tx_buffer[word_idx] |= (1UL << bit_shift);
            } else {
                spi_tx_buffer[word_idx] &= ~(1UL << bit_shift);
            }
            
            spi_bit_index++;
            ticks_in_current_level++;

            // 4. Toggle the phase if we've completed a half-cycle
            if (ticks_in_current_level >= half_period_ticks) {
                current_level ^= 1;
                ticks_in_current_level = 0; 
            }
        }
    }
}

void generate_fsk_buffer_uint8(const uint8_t* ble_packet, size_t num_ble_bits, uint8_t* spi_tx_buffer) {
    size_t spi_bit_index = 0;
    
    // State machine variables to maintain phase continuity
    uint8_t current_level = 1;          
    uint8_t ticks_in_current_level = 0; 

    for (size_t bit_idx = 0; bit_idx < num_ble_bits; bit_idx++) {
        
        // 1. Extract the current BLE data bit (LSB first)
        uint8_t byte_idx = bit_idx / 8;
        uint8_t bit_pos = bit_idx % 8;
        uint8_t current_ble_bit = (ble_packet[byte_idx] >> bit_pos) & 0x01;

        // 2. Set the toggle threshold
        uint8_t half_period_ticks = current_ble_bit ? F1_HALF_PERIOD_TICKS : F0_HALF_PERIOD_TICKS;

        // 3. Generate the ticks for this BLE bit
        for (uint8_t tick = 0; tick < SPI_BITS_PER_BLE_BIT; tick++) {
            
            // --- 8-bit Byte Mapping ---
            size_t word_idx = spi_bit_index / 8;     // Now indexing by bytes
            uint8_t bit_shift = spi_bit_index % 8;  // Shift remains within 0-7

            // Write the voltage level into the 8-bit buffer
            if (current_level) {
                spi_tx_buffer[word_idx] |= (uint8_t)(1U << bit_shift);
            } else {
                spi_tx_buffer[word_idx] &= (uint8_t)~(1U << bit_shift);
            }
            
            spi_bit_index++;
            ticks_in_current_level++;

            // 4. Toggle the phase if we've completed a half-cycle
            if (ticks_in_current_level >= half_period_ticks) {
                current_level ^= 1;
                ticks_in_current_level = 0; 
            }
        }
    }
}

void SPI_FSK_Init() {
    // 1. Configure PA4 (MOSI) as an output pin
    GPIOA_SetBits(GPIO_Pin_4);
    GPIOA_ModeCfg(GPIO_Pin_4 | GPIO_Pin_7, GPIO_ModeOut_PP_5mA);
    
    // 2. Set Clock Divider for 20MHz (100MHz / 5)
    R8_SPI_CLOCK_DIV = TICKS_FOR_20MHz; 

    // 3. Reset the SPI module using the correct bit name
    //Master out slave in (so PA4 is out)
    //Enable Serial Clock
    //Enable two wire mode
    R8_SPI_CTRL_MOD = RB_SPI_ALL_CLEAR;
    R8_SPI_CTRL_MOD = RB_SPI_MOSI_OE | RB_SPI_SCK_OE | RB_SPI_2WIRE_MOD; 

    R8_SPI_CTRL_CFG |= RB_SPI_AUTO_IF;
    //Set idle low
    // R8_SPI_CTRL_MOD = ~RB_SPI_MST_SCK_MOD;

    // 4. Configure Bit Order (LSB First for bluetooth)
    R8_SPI_CTRL_CFG |= RB_SPI_BIT_ORDER; 
}

__HIGH_CODE
void start_fsk_spi(uint32_t* buffer, uint16_t num_words) {
    PRINT("LETS GO\n");
    DelayMs(10);
    R8_SPI_CTRL_MOD &= ~RB_SPI_FIFO_DIR;

    // // 1. Set DMA Start and end Address
    R16_SPI_DMA_BEG = (uint32_t)buffer;
    R16_SPI_DMA_END = (uint32_t)(buffer + num_words * 4);

    R16_SPI_TOTAL_CNT = num_words * 4;
    R8_SPI_INT_FLAG = RB_SPI_IF_CNT_END | RB_SPI_IF_DMA_END;
    R8_SPI_CTRL_CFG |= RB_SPI_DMA_ENABLE;
    while(!(R8_SPI_INT_FLAG & RB_SPI_IF_CNT_END));
    R8_SPI_CTRL_CFG &= ~RB_SPI_DMA_ENABLE;
}


__HIGH_CODE
void SPI_MasterDMATrans_FSK_Blocking(uint32_t *pbuf, uint16_t num_words) {
    uint16_t len_bytes = num_words * 4;

    // 1. Set the Total Byte Count (Critical for Master Mode completion)
    R16_SPI_TOTAL_CNT = len_bytes;

    // 2. Load DMA Address Boundaries (16-bit registers)
    R16_SPI_DMA_BEG = (uint16_t)((uint32_t)pbuf & 0xFFFF);
    R16_SPI_DMA_END = (uint16_t)(((uint32_t)pbuf + len_bytes) & 0xFFFF);

    // 3. Clear Interrupt Flags (Writing 1 to clear)
    R8_SPI_INT_FLAG = RB_SPI_IF_CNT_END | RB_SPI_IF_DMA_END;

    // 4. Enable DMA (Bit 0 of R8_SPI_CTRL_CFG)
    R8_SPI_CTRL_CFG |= RB_SPI_DMA_ENABLE;

    // 5. Wait for Hardware Completion
    // RB_SPI_IF_CNT_END fires only when the last bit has left the pin
    while(!(R8_SPI_INT_FLAG & RB_SPI_IF_CNT_END));

    // 6. Stop DMA for next cycle
    R8_SPI_CTRL_CFG &= ~RB_SPI_DMA_ENABLE;
}

void DebugInit(void)
{
    GPIOA_SetBits(bTXD_0);
    GPIOA_ModeCfg(bRXD_0, GPIO_ModeIN_PU);      // RXD-配置上拉输入
    GPIOA_ModeCfg(bTXD_0, GPIO_ModeOut_PP_5mA); // TXD-配置推挽输出，注意先让IO口输出高电平
    UART_Remap(ENABLE, UART_TX_REMAP_PA3, UART_RX_REMAP_PA2);
    UART_DefInit();
}

/*********************************************************************
 * @fn      main
 *
 * @brief   主函数
 *
 * @return  none
 */
__HIGH_CODE
int main()
{
    uint8_t len;

    // 关闭两线调试
    R16_PIN_ALTERNATE &= ~RB_PIN_DEBUG_EN;
    HSECFG_Capacitance(HSECap_18p);
    SetSysClock(CLK_SOURCE_HSE_PLL_100MHz);

    /* 配置串口1：先配置IO口模式，再配置串口 */
    DebugInit();
    SPI_FSK_Init();

#if 1 // 查询方式：接收数据后发送出去
    while(1)
    {
        len = UART_RecvString(RxBuff);
        if(len)
        {
            PRINT("SPI DMA\n");
            DelayMs(10);
            generate_fsk_buffer(ble_packet, HEADER_LEN * 8, spi_tx_buffer);
            // generate_fsk_buffer_uint8(ble_packet, HEADER_LEN * 8, spi_tx_buffer_uint8);
            // print_uint8_array_bin(spi_tx_buffer_uint8, sizeof(spi_tx_buffer_uint8) / sizeof(spi_tx_buffer_uint8[0]));
            // print_uint32_array(spi_tx_buffer, sizeof(spi_tx_buffer)/sizeof(spi_tx_buffer[0]));
            // DelayMs(2000);            

            GPIOA_ResetBits(GPIO_Pin_4);
            // __attribute__((aligned(4))) uint32_t test_pattern[] = {0xF0F0F0F0, 0xF0F0F0F0, 0xF83E0F83, 0xE0F83E0F};
            // start_fsk_spi(test_pattern, 4);
            
            start_fsk_spi(spi_tx_buffer, 4 * sizeof(spi_tx_buffer) / sizeof(spi_tx_buffer[0]));
            GPIOA_SetBits(GPIO_Pin_4);
           
            /* 开启计数溢出中断，计满100个周期进入中断 */
            // TMR_ClearITFlag(TMR_IT_DMA_END);
            // TMR_ITCfg(ENABLE, TMR_IT_DMA_END);
            // PFIC_EnableIRQ(TMR_IRQn);

            // DMA 连续发送
            // GPIOA_ResetBits(GPIO_Pin_4);
            // SPI_MasterDMATrans(spi_tx_buffer_uint8, HEADER_LEN * SPI_BITS_PER_BLE_BIT);
            // GPIOA_SetBits(GPIO_Pin_4);
            DelayMs(10);
            PRINT("END ...\n");
        }
    }

#endif

    while(1);
}

/*********************************************************************
 * @fn      UART_IRQHandler
 *
 * @brief   UART中断函数
 *
 * @return  none
 */
__INTERRUPT
__HIGH_CODE
void UART_IRQHandler(void)
{
    volatile uint8_t i;

    switch(UART_GetITFlag())
    {
        case UART_II_LINE_STAT: // 线路状态错误
        {
            UART_GetLinSTA();
            break;
        }

        case UART_II_RECV_RDY: // 数据达到设置触发点
            break;

        case UART_II_RECV_TOUT: // 接收超时，暂时一帧数据接收完成
            i = UART_RecvString(RxBuff);
            UART_SendString(RxBuff, i);
            break;

        case UART_II_THR_EMPTY: // 发送缓存区空，可继续发送
            break;

        default:
            break;
    }
}