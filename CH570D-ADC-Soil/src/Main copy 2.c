// #include "CH57x_common.h"

// /*********************************************************************
//  * @fn      CMP_Configure
//  *
//  * @brief   Configure comparator with specific reference level
//  *
//  * @param   ref_level - reference voltage level (0-15 for 50-800mV)
//  */
// void CMP_Configure(uint8_t ref_level)
// {
//     // Disable comparator first
//     R32_CMP_CTRL &= ~RB_CMP_EN;
    
//     // Configure comparator settings:
//     // - Positive input: PA7 (bit3=1)
//     // - Negative input: CMP_VERF (bit2=1) 
//     // - Reference level: ref_level (bits 7:4)
//     // - Enable comparator (bit0=1)
    
//     uint32_t cmp_config = 0;
//     cmp_config |= (1 << 3);                    // PA7 as positive input
//     cmp_config |= (1 << 2);                    // CMP_VERF as negative input  
//     cmp_config |= ((ref_level & 0x0F) << 4);   // Reference level
//     cmp_config |= RB_CMP_EN;                   // Enable comparator
    
//     R32_CMP_CTRL = cmp_config;
// }

// /*********************************************************************
//  * @fn      CMP_ReadOutput
//  *
//  * @brief   Read comparator output
//  *
//  * @return  0 if sensor voltage > reference, 1 if sensor voltage < reference
//  */
// uint8_t CMP_ReadOutput(void)
// {
//     // Wait for comparator to stabilize
//     DelayUs(100);
    
//     // Read real-time comparator output (bit 25)
//     return (R32_CMP_CTRL & RB_APR_OUT_CMP) ? 1 : 0;
// }

// /*********************************************************************
//  * @fn      Moisture_Sensor_Read_4bit
//  *
//  * @brief   Read moisture sensor using 4-bit binary search
//  */
// uint8_t Moisture_Sensor_Read_4bit()
// {
//     uint8_t result = 0;
    
//     for(int bit_pos = 3; bit_pos >= 0; bit_pos--) {
//         uint8_t current_test = result | (1 << bit_pos);
        
//         // Configure comparator with current reference level
//         CMP_Configure(current_test);
        
//         uint8_t cmp_out = CMP_ReadOutput();
        
//         PRINT("Bit %d, Ref: %d (%dmV), CMP_OUT: %d -> ", 
//               bit_pos, current_test, 50 + (current_test * 50), cmp_out);
        
//         if(cmp_out == 0) {
//             // Sensor voltage > reference voltage - keep this bit set
//             result = current_test;
//             PRINT("Sensor > Ref, result=%d\n", result);
//         } else {
//             // Sensor voltage < reference voltage - don't set this bit
//             PRINT("Sensor < Ref, result=%d\n", result);
//         }
        
//         // Disable comparator briefly
//         R32_CMP_CTRL &= ~RB_CMP_EN;
//         DelayUs(10);
//     }
    
//     // Disable comparator when done
//     R32_CMP_CTRL &= ~RB_CMP_EN;
    
//     return result;
// }

// /*********************************************************************
//  * @fn      Test_Sensor_Range_Direct
//  *
//  * @brief   Test all reference levels to find sensor voltage range
//  */
// void Test_Sensor_Range_Direct()
// {
//     PRINT("Testing sensor voltage range with direct register access:\n");
    
//     for(uint8_t ref_level = 0; ref_level < 16; ref_level++) {
//         CMP_Configure(ref_level);
        
//         uint8_t cmp_out = CMP_ReadOutput();
//         uint32_t ref_mv = 50 + (ref_level * 50);
        
//         PRINT("Ref: %4dmV, CMP_OUT: %d -> ", ref_mv, cmp_out);
        
//         if(cmp_out == 0) {
//             PRINT("Sensor voltage > %dmV\n", ref_mv);
//         } else {
//             PRINT("Sensor voltage < %dmV\n", ref_mv);
//         }
        
//         // Disable between tests
//         R32_CMP_CTRL &= ~RB_CMP_EN;
//         DelayUs(50);
//     }
// }

// /*********************************************************************
//  * @fn      Test_Comparator_Register
//  *
//  * @brief   Debug function to show register contents
//  */
// void Test_Comparator_Register(void)
// {
//     PRINT("CMP_CTRL Register analysis:\n");
    
//     // Configure with reference level 0 (50mV)
//     CMP_Configure(0);
//     DelayUs(100);
    
//     uint32_t reg_value = R32_CMP_CTRL;
//     PRINT("Full register: 0x%08lX\n", reg_value);
//     PRINT("RB_CMP_EN: %lu\n", (reg_value & RB_CMP_EN) >> 0);
//     PRINT("RB_CMP_SW: %lu\n", (reg_value & RB_CMP_SW) >> 2);
//     PRINT("RB_CMP_NREF_LEVEL: %lu\n", (reg_value & (0xF << 4)) >> 4);
//     PRINT("RB_APR_OUT_CMP: %lu\n", (reg_value & RB_APR_OUT_CMP) >> 25);
    
//     // Disable
//     R32_CMP_CTRL &= ~RB_CMP_EN;
// }

// /*********************************************************************
//  * @fn      main
//  *
//  * @brief   Main function
//  */
// int main()
// {
//     HSECFG_Capacitance(HSECap_18p);
//     SetSysClock(CLK_SOURCE_HSE_PLL_100MHz);
    
//     /* Configure UART for debugging */
//     GPIOA_SetBits(bTXD_0);
//     GPIOA_ModeCfg(bTXD_0, GPIO_ModeOut_PP_5mA);
//     UART_Remap(ENABLE, UART_TX_REMAP_PA3, UART_RX_REMAP_PA2);
//     UART_DefInit();
    
//     // Configure PA7 as analog input for comparator
//     GPIOA_ModeCfg(GPIO_Pin_7, GPIO_ModeIN_Floating);
    
//     PRINT("Moisture Sensor - Direct Register Control\n");
//     PRINT("=========================================\n");
    
//     // First, test the register configuration
//     Test_Comparator_Register();
//     PRINT("\n");
    
//     // Test sensor range
//     Test_Sensor_Range_Direct();
//     PRINT("\n");
    
//     // Now do continuous reading
//     PRINT("Starting continuous moisture monitoring:\n");
    
//     while(1)
//     {
//         uint8_t moisture_raw = Moisture_Sensor_Read_4bit();
//         uint32_t moisture_mv = 50 + (moisture_raw * 50);
        
//         PRINT("Final -> Raw: %2d, Voltage: %4dmV - ", moisture_raw, moisture_mv);
        
//         // Moisture interpretation
//         if(moisture_raw < 4) {        // < 250mV
//             PRINT("Very Dry\n");
//         } else if(moisture_raw < 8) { // < 450mV
//             PRINT("Dry\n"); 
//         } else if(moisture_raw < 12) { // < 650mV
//             PRINT("Moist\n");
//         } else {                      // >= 650mV
//             PRINT("Wet\n");
//         }
        
//         DelayMs(2000);
//         PRINT("\n");
//     }
// }