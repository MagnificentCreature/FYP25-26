// #include "CH57x_common.h"

// /*********************************************************************
//  * @fn      Moisture_Sensor_Read_4bit
//  *
//  * @brief   Read moisture sensor using internal 4-bit ADC
//  *
//  * @return  Moisture value (0-15 for 4-bit)
//  */
// uint8_t Moisture_Sensor_Read_4bit()
// {
//     uint8_t result = 0;
    
//     // 4-bit binary search using different reference voltage levels
//     for(int bit_pos = 3; bit_pos >= 0; bit_pos--) {
//         uint8_t current_test = result | (1 << bit_pos);
        
//         // Convert test value to reference voltage level (0-15 maps to 50-800mV)
//         CMPNrefLevelTypeDef ref_level;
//         if(current_test <= 15) {
//             ref_level = (CMPNrefLevelTypeDef)current_test; // Direct mapping
//         } else {
//             ref_level = cmp_nref_level_800;
//         }
        
//         // Configure comparator with current reference level
//         CMP_Init(cmp_sw_3, ref_level); // cmp_sw_3: CMP_P1(PA7) vs CMP_VERF (internal reference)
//         CMP_Enable();
//         DelayUs(10); // Short delay for comparator to stabilize
        
//         uint8_t cmp_out = CMP_ReadAPROut();
        
//         if(cmp_out == 0) {
//             // Sensor voltage > reference voltage - keep this bit set
//             result = current_test;
//         }
//         // else: Sensor voltage < reference voltage - don't set this bit
        
//         CMP_Disable();
//     }
    
//     return result;
// }

// void Test_Sensor_Range()
// {
//     PRINT("Testing sensor output range:\n");
//     PRINT("Testing all reference levels to find sensor voltage:\n");
    
//     for(uint8_t ref_level = 0; ref_level < 16; ref_level++) {
//         CMP_Init(cmp_sw_3, (CMPNrefLevelTypeDef)ref_level);
//         CMP_Enable();
//         DelayUs(200); // Longer stabilization
        
//         uint8_t cmp_out = CMP_ReadAPROut();
        
//         // Interpretation:
//         // cmp_out = 0: Sensor voltage > reference voltage
//         // cmp_out = 1: Sensor voltage < reference voltage
        
//         uint32_t ref_mv = 50 + (ref_level * 50);
//         PRINT("Ref: %4dmV, CMP_OUT: %d -> ", ref_mv, cmp_out);
        
//         if(cmp_out == 0) {
//             PRINT("Sensor > %dmV\n", ref_mv);
//         } else {
//             PRINT("Sensor < %dmV\n", ref_mv);
//         }
        
//         CMP_Disable();
//         DelayUs(50);
//     }
// }

// /*********************************************************************
//  * @fn      main
//  *
//  * @brief   Main function for moisture sensing with 4-bit ADC
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
    
//     PRINT("Moisture Sensor Demo - 4-bit ADC Start\n");
//     PRINT("Voltage Range: 50-800mV, 16 levels (~53mV per step)\n");

//     Test_Sensor_Range();
    
//     while(1)
//     {
//         uint8_t moisture_raw = Moisture_Sensor_Read_4bit();
        
//         // Convert 4-bit value to approximate voltage
//         // Reference levels: 50, 100, 150, 200, 250, 300, 350, 400, 
//         //                  450, 500, 550, 600, 650, 700, 750, 800 mV
//         uint32_t moisture_mv = 50 + (moisture_raw * 50); // Approximate mV
        
//         PRINT("Moisture Raw: %2d, Voltage: %4dmV\n", moisture_raw, moisture_mv);
        
//         // Simple moisture level detection (adjust thresholds based on your sensor)
//         if(moisture_raw < 5) {        // < ~300mV
//             PRINT("Status: Dry Soil\n");
//         } else if(moisture_raw < 10) { // < ~550mV  
//             PRINT("Status: Moist Soil\n");
//         } else {                       // >= ~550mV
//             PRINT("Status: Wet Soil\n");
//         }
        
//         DelayMs(500);  // Read every 500ms
//     }
// }