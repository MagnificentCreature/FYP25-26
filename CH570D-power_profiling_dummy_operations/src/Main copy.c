// #include "CH57x_common.h"

// // --- Configuration ---
// #define LED_PIN             GPIO_Pin_8          // Using GPIOA Pin 8 for LED
// #define ACTIVE_DURATION_S   5                   // Duration of Active Mode in seconds
// #define SLEEP_DURATION_S    5                   // Duration of Light Sleep Mode in seconds
// #define DEEP_SLEEP_DURATION_S 5                 // Duration of Deep Sleep Mode in seconds


// /*********************************************************************
//  * @fn      LED_Init
//  * @brief   Initializes the GPIO Pin for the LED.
//  * @return  none
//  */
// void LED_Init(void)
// {
//     // Configure GPIOA Pin 8 as a 5mA push-pull output
//     GPIOA_SetBits(LED_PIN); // Set pin high before configuring to avoid brief flash
//     GPIOA_ModeCfg(LED_PIN, GPIO_ModeOut_PP_5mA);
// }

// /*********************************************************************
//  * @fn      main
//  * @brief   Main program loop.
//  * @return  none
//  */
// __HIGH_CODE
// int main()
// {
//     // Use a more power-efficient clock source for this demo
//     SetSysClock(CLK_SOURCE_HSE_32MHz);

//     // Initialize the LED pin
//     LED_Init();

//     // Check if the wakeup was from a deep sleep (standby) event
//     if (PWR_GetITFlag(RB_PWR_FLAG_WAKE))
//     {
//         // If yes, clear the flag and keep LED off to show we just woke up.
//         PWR_ClearITFlag(RB_PWR_FLAG_WAKE);
//         // A brief delay to stabilize after wakeup
//         DelayMs(10);
//     }

//     /* ******************************************************************
//      * STATE 1: ACTIVE MODE
//      * ******************************************************************/
//     // The CPU is fully running. This is the highest power consumption state.
//     // We will blink the LED rapidly to show activity.
//     uint32_t active_end_time = R32_TIM_CNT_US + (uint32_t)ACTIVE_DURATION_S * 1000000;
//     while(R32_TIM_CNT_US < active_end_time)
//     {
//         // NOTE: ResetBits pulls the pin LOW. SetBits pulls it HIGH.
//         // On many dev boards, pulling the pin LOW turns the LED ON.
//         GPIOA_ResetBits(LED_PIN); // LED ON
//         DelayMs(100);
//         GPIOA_SetBits(LED_PIN);   // LED OFF
//         DelayMs(100);
//     }
//     GPIOA_SetBits(LED_PIN); // Ensure LED is off before next state


//     /* ******************************************************************
//      * STATE 2: LIGHT SLEEP MODE
//      * ******************************************************************/
//     // The CPU is stopped, but RAM and peripherals are powered.
//     // Wakes up from interrupts. We will use the SysTick timer to wake up.
//     // The current should drop significantly here.
//     SysTick_Config(FREQ_SYS / 1000 * (SLEEP_DURATION_S * 1000)); // Configure SysTick to fire once after the duration
//     LowPower_Sleep(RB_PWR_RAM_OFF); // Enter sleep mode
//     // --- MCU will wake up here when SysTick interrupt fires ---
//     SysTick->CTLR = 0; // Disable SysTick after wakeup


//     /* ******************************************************************
//      * STATE 3: DEEP SLEEP (STANDBY) MODE
//      * ******************************************************************/
//     // Most of the MCU is powered down. Only low-power peripherals like RTC can run.
//     // This should be the lowest power consumption state.
//     // Waking up from this mode causes a full chip reset.
//     RTC_InitTime(2025, 10, 9, 22, 10, 0); // Initialize RTC with a date/time
//     RTC_SetTign((DEEP_SLEEP_DURATION_S)); // Set a wakeup alarm for N seconds from now
//     LowPower_Standby(); // Enter deep sleep mode
//     // --- The code will NOT continue from here. It will RESET upon wakeup. ---


//     // This part of the loop is effectively unreachable because LowPower_Standby() resets the chip.
//     while (1);
// }


// /*********************************************************************
//  * @fn      SysTick_Handler
//  * @brief   This function handles SysTick interrupt.
//  * @return  none
//  */
// __INTERRUPT
// __HIGH_CODE
// void SysTick_Handler(void)
// {
//     // The interrupt itself is the wakeup signal. No action needed here.
//     // We just need to clear the interrupt flag.
//     SysTick->CTLR;
// }

