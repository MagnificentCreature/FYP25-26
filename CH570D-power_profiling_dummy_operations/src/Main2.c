// #include "CH57x_common.h"

// // Forward declaration for the RTC interrupt handler
// void RTC_IRQHandler(void);

// void RTC_Init_Wakeup(uint32_t seconds)
// {
//     // Make sure the Low-Frequency Oscillator (LFO) is calibrated and running
//     LFX_SROUGH_CAL_EN( );
//     LFX_CAL_DELAY_Set( 0 );
//     LFX_SROUGH_CAL_EN( );
//     R8_OSC_CAL_CTRL |= RB_OSC_LFO_CAL_EN;
//     DelayMs(1);
//     R8_OSC_CAL_CTRL &= ~RB_OSC_LFO_CAL_EN;
//     // Get the current RTC count
//     uint32_t current_count = RTC_GetCycle32();
//     // Set the trigger point 'seconds' from now
//     RTC_SetTrigCycle32(current_count + (seconds * 32768)); // Assuming 32.768kHz clock source for RTC
//     // Enable the RTC interrupt and set its priority
//     PFIC_EnableIRQ(RTC_IRQn);
// }

// int main()
// {
//     /* --- Your initial System and Debug Clock Setup --- */
//     HSECFG_Capacitance(HSECap_18p);
//     SetSysClock(CLK_SOURCE_HSE_PLL_100MHz);
//     GPIOA_ModeCfg(GPIO_Pin_All, GPIO_ModeIN_PU);
//     DebugInit();
//     PRINT("Start Automated Power Profiling @ChipID=%02x\n", R8_CHIP_ID);

//     /* --- Configure RTC as the Wake-Up Source --- */
//     // This replaces your GPIO configuration
//     PWR_PeriphWakeUpCfg(ENABLE, RB_SLP_RTC_WAKE, Fsys_Delay_4096);
//     DelayMs(200);

//     /* --- Automated Profiling Sequence --- */

//     // 1. Profile IDLE mode
//     PRINT("Entering IDLE mode for 5 seconds...\n");
//     DelayMs(2); // Ensure print buffer is empty before sleep
//     RTC_Init_Wakeup(5);
//     LowPower_Idle();
//     PRINT("Woke from IDLE.\n\n");
//     DelayMs(500);

//     // 2. Profile HALT mode
//     PRINT("Entering HALT mode for 5 seconds...\n");
//     DelayMs(2);
//     RTC_Init_Wakeup(5);
//     LowPower_Halt();
//     HSECFG_Current(HSE_RCur_100); // Restore HSE current after Halt
//     PRINT("Woke from HALT.\n\n");
//     DelayMs(500);

//     // 3. Profile SLEEP mode
//     PRINT("Entering SLEEP mode for 5 seconds...\n");
//     DelayMs(2);
//     RTC_Init_Wakeup(5);
//     LowPower_Sleep(RB_PWR_RAM12K | RB_PWR_EXTEND);
//     PRINT("Woke from SLEEP.\n\n");
//     DelayMs(500);

//     PRINT("Profiling sequence complete.\n");

//     while(1);
// }

// // RTC Interrupt Service Routine
// __INTERRUPT
// __HIGH_CODE
// void RTC_IRQHandler(void)
// {
//     // Check for the trigger event flag
//     if (RTC_GetITFlag(RB_RTC_TRIG_FLAG))
//     {
//         RTC_ClearITFlag(RB_RTC_TRIG_FLAG); // Clear the flag to prevent re-triggering
//     }
// }