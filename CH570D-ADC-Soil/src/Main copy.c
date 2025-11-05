// #include "CH57x_common.h"

// /*********************************************************************
//  * @fn      Moisture_Sensor_Read
//  *
//  * @brief   Read moisture sensor using 9-bit ADC
//  *
//  * @return  Moisture value (0-511 for 9-bit)
//  */
// uint32_t Moisture_Sensor_Read()
// {
//     uint32_t vref = 256;
//     uint32_t cmp_count = 1;
    
//     /* Configure GPIO */
//     GPIOA_ModeCfg(GPIO_Pin_4, GPIO_ModeOut_PP_5mA);  // PA4 - PWM4
//     PWMX_CLKCfg(1);                                  // cycle = Fsys
//     PWM_16bit_CycleEnable();                         // Enable 16-bit width
//     PWMX_16bit_CycleCfg(CH_PWM4, 512);               // PWM clock cycle for 16-bit data width
    
//     // Configure comparator: CMP_P1(PA7) as sensor input, CMP_N(PA2) as PWM reference
//     CMP_Init(cmp_sw_2, cmp_nref_level_800);
//     CMP_Enable();
    
//     while(1)
//     {
//         uint8_t cmp_out = 0;
//         uint8_t cmp_out_last = 0;
        
//         PWMX_16bit_ACTOUT(CH_PWM4, vref, High_Level, ENABLE);
//         DelayUs(230);  // Wait for PWM to stabilize
        
//         cmp_out_last = CMP_ReadAPROut();
//         while(1)
//         {
//             DelayUs(20);
//             cmp_out = CMP_ReadAPROut();
//             if(cmp_out == cmp_out_last)
//             {
//                 break;
//             }
//             cmp_out_last = cmp_out;
//         }

//         if(cmp_count == 256)  // After 8 comparisons (9-bit resolution)
//         {
//             if (cmp_out)
//             {
//                 return (vref * 2 - 1);  // Final moisture value
//             }
//             else
//             {
//                 return (vref * 2 + 1);  // Final moisture value
//             }
//         }
//         else
//         {
//             cmp_count <<= 1;
//             if (cmp_out)
//             {
//                 vref -= 256 / cmp_count;  // Sensor voltage > reference
//             }
//             else
//             {
//                 vref += 256 / cmp_count;  // Sensor voltage < reference
//             }
//         }
//     }
// }

// /*********************************************************************
//  * @fn      main
//  *
//  * @brief   Main function for moisture sensing
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
    
//     PRINT("Moisture Sensor Demo Start\n");
    
//     while(1)
//     {
//         uint32_t moisture_raw = Moisture_Sensor_Read();
//         uint32_t moisture_mv = moisture_raw * 3300 / 1024;  // Convert to mV (assuming 3.3V VDD)
        
//         PRINT("Moisture Raw: %d, Voltage: %dmV\n", moisture_raw, moisture_mv);
        
//         // Simple moisture level detection
//         if(moisture_mv < 1000) {
//             PRINT("Status: Dry Soil\n");
//         } else if(moisture_mv < 2000) {
//             PRINT("Status: Moist Soil\n");
//         } else {
//             PRINT("Status: Wet Soil\n");
//         }
        
//         DelayMs(2000);  // Read every 2 seconds
//     }
// }