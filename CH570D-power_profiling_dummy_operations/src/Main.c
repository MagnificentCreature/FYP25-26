// /********************************** (C) COPYRIGHT *******************************
//  * File Name          : Main.c
//  * Author             : WCH
//  * Version            : V1.3 (Corrected)
//  * Date               : 2024/01/01
//  * Description        : Automated power profiling using the RTC as a wake-up source.
//  * (Modified to use correct LSI configuration register)
//  *********************************************************************************
//  * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
//  * Attention: This software (modified or not) and binary are used for
//  * microcontroller manufactured by Nanjing Qinheng Microelectronics.
//  *******************************************************************************/

// #include "CH57x_common.h"

// /**
//  * @fn      DebugInit
//  * @brief   Initializes the UART for printing debug messages.
//  */
// void DebugInit(void)
// {
//     GPIOA_SetBits(bTXD_0);
//     GPIOA_ModeCfg(bRXD_0, GPIO_ModeIN_PU);
//     GPIOA_ModeCfg(bTXD_0, GPIO_ModeOut_PP_5mA);
//     UART_Remap(ENABLE, UART_TX_REMAP_PA3, UART_RX_REMAP_PA2);
//     UART_DefInit();
// }

// /**
//  * @fn      RTC_Init_Wakeup
//  * @brief   Configures the RTC to trigger an interrupt after a set number of seconds
//  * using direct register access based on the CH570/CH572 datasheet.
//  * @param   seconds - The number of seconds to wait before waking up.
//  */
// void RTC_Init_Wakeup(uint32_t seconds)
// {
//     uint32_t current_count;
//     uint32_t target_count;

//     // 1. Enable the 32kHz Internal Low-Speed Oscillator (LSI) as the RTC clock source.
//     // The LSI is required for the RTC to run in low-power modes.
//     // Using R8_LSI_CONFIG as per datasheet table 6-1.
//     R8_LSI_CONFIG |= RB_CLK_LSI_PON;
//     DelayMs(10); // Wait for the LSI to stabilize

//     // 2. Get the current 32-bit RTC counter value.
//     // This is done by reading two 16-bit registers and combining them.
//     current_count = R16_RTC_CNT_DIV1;
//     current_count = (current_count << 16) | R16_RTC_CNT_LSI;

//     // 3. Calculate the target trigger time.
//     // We assume a nominal LSI frequency of 32000 Hz. This may need calibration
//     // for high-precision timing, but is sufficient for this demonstration.
//     target_count = current_count + (seconds * 32000);

//     // 4. Set the trigger/alarm value in the RTC trigger register.
//     R32_RTC_TRIG = target_count;

//     // 5. Enable the RTC trigger mode. This enables the one-shot alarm.
//     R8_RTC_MODE_CTRL = RB_RTC_TRIG_EN;

//     // 6. Enable the RTC interrupt in the Programmable Fast Interrupt Controller (PFIC).
//     PFIC_EnableIRQ(RTC_IRQn);
// }

// /**
//  * @fn      main
//  * @brief   Main program entry point.
//  */
// int main()
// {
//     // System clock and GPIO initialization
//     HSECFG_Capacitance(HSECap_18p);
//     SetSysClock(CLK_SOURCE_HSE_PLL_100MHz);
//     GPIOA_ModeCfg(GPIO_Pin_All, GPIO_ModeIN_PU);

//     // Configure serial port for debugging
//     DebugInit();
//     PRINT("Start Automated Power Profiling @ChipID=%02x\n\n", R8_CHIP_ID);

//     // Configure RTC as the primary wake-up source
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
//     HSECFG_Current(HSE_RCur_100); // Restore HSE current to nominal after Halt wake-up
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

//     while (1); // End of program
// }

// /**
//  * @fn      RTC_IRQHandler
//  * @brief   RTC Interrupt Service Routine. This function is called on RTC wake-up.
//  */
// __INTERRUPT
// __HIGH_CODE
// void RTC_IRQHandler(void)
// {
//     // Check if the interrupt was caused by the trigger event
//     if (RTC_GetITFlag(RB_RTC_TRIG_FLAG))
//     {
//         RTC_ClearITFlag(RB_RTC_TRIG_FLAG); // IMPORTANT: Clear the flag to allow future wake-ups
//     }
// }

#include "CH57x_common.h"
#include "CH57x_sys.h"

/*********************************************************************
 * Global Variables
 *********************************************************************/
volatile uint32_t g_millis = 0; // Global millisecond counter

/*********************************************************************
 * External Function Prototypes
 *********************************************************************/
// This function exists in system_ch57x.c but is not declared in a header.
// We must provide the prototype to call it.
void SystemCoreClockUpdate(void);

/*********************************************************************
 * @fn      SysTick_Handler
 *
 * @brief   SysTick Interrupt Service Routine (ISR).
 * This function is called automatically by the core every 1ms.
 *
 * @return  none
 */
void SysTick_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void SysTick_Handler(void)
{
    g_millis++;                 // Increment our 1ms counter
    SysTick->SR = 0;            // Clear the SysTick interrupt flag
}

/*********************************************************************
 * @fn      SysTick_Recalibrate_1ms
 *
 * @brief   Configures the SysTick timer for a 1ms interrupt.
 * @note    This function MUST be called every time the system clock
 * frequency is changed to recalibrate the timer.
 *
 * @return  none
 */
void SysTick_Recalibrate_1ms(void)
{
    uint32_t ticks;

    // 1. Update the 'SystemCoreClock' global variable
    //    This function reads the clock registers to get the new, current speed.
    SystemCoreClockUpdate();

    // 2. Calculate the number of ticks needed for a 1ms interrupt
    ticks = GetSysClock() / 1000;

    // 3. Call the official SysTick_Config function from core_riscv.h
    //    This will correctly set CMP = ticks - 1 and enable the timer
    //    with all correct control bits (including STRE).
    SysTick_Config(ticks);
}

/*********************************************************************
 * @fn      Delay_ms
 *
 * @brief   A blocking, power-friendly delay using the SysTick timer.
 *
 * @param   ms - Number of milliseconds to delay.
 *
 * @return  none
 */
void Delay_ms(uint32_t ms)
{
    uint32_t start_time = g_millis;
    while((g_millis - start_time) < ms)
    {
        // __WFI() = "Wait For Interrupt"
        // This is the key for power profiling. It puts the CPU to
        // sleep until the next interrupt (our 1ms SysTick) wakes it up.
        // This avoids a "busy-wait" loop and shows real power savings.
        __WFI();
    }
}

/*********************************************************************
 * @fn      LED_Init
 *
 * @brief   Initializes GPIO Pin PA6 for the LED.
 *
 * @return  none
 */
void LED_Init(void)
{
    // Configure PA6 as a 5mA push-pull output
    // Set pin high (LED OFF) initially
    GPIOA_SetBits(GPIO_Pin_6);
    GPIOA_ModeCfg(GPIO_Pin_6, GPIO_ModeOut_PP_5mA);
}

/*********************************************************************
 * @fn      Flash_LED
 *
 * @brief   Flashes the LED on PA6 for 100ms.
 *
 * @return  none
 */
void Flash_LED(void)
{
    GPIOA_ResetBits(GPIO_Pin_6); // Turn LED ON
    Delay_ms(100);               // Keep it on for 100ms
    GPIOA_SetBits(GPIO_Pin_6);   // Turn LED OFF
}

/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main()
{
    // --- 1. Initial Hardware Setup ---
    HSECFG_Capacitance(HSECap_18p); // Tune the external crystal
    LED_Init();                     // Setup PA6 as an output

    // --- 2. Run at 100MHz ---
    SetSysClock(CLK_SOURCE_HSE_PLL_100MHz);
    SysTick_Recalibrate_1ms(); // CRITICAL: Calibrate SysTick to 100MHz
    Delay_ms(5000);            // Wait for 5 seconds
    Flash_LED();               // Flash the LED

    // --- 3. Run at 32MHz ---
    SetSysClock(CLK_SOURCE_HSE_32MHz);
    SysTick_Recalibrate_1ms(); // CRITICAL: RE-Calibrate SysTick to 32MHz
    Delay_ms(5000);            // Wait for 5 seconds (this is now accurate)
    Flash_LED();               // Flash the LED

    // --- 4. Loop Forever ---
    while(1)
    {
        // Program finished, just sleep.
        __WFI();
    }
}