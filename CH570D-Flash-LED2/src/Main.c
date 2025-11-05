

/**
 *********************************************************************************
 * @file    main.c
 * @author  WCH
 * @version V1.0
 * @date    (Date of SDK example)
 * @brief   Main program body for blinking LEDO (D2) on pin PA8
 *********************************************************************************
 *
 * This code is specifically for the schematic provided (CH572SCH.pdf).
 * - Target Chip: CH572D
 * - Target LED: D2 (labeled LEDO)
 * - Connection:
 * - LED Anode -> +3V3
 * - LED Cathode -> R2 (1K Resistor) -> Pin 20 (PA8)
 * - Logic:
 * - To turn LED ON: Set PA8 pin LOW (0V).
 * - To turn LED OFF: Set PA8 pin HIGH (3.3V).
 *
 */

// This is the main SDK header
#include "CH57x_common.h"

/*
NOTE THIS REQUIRES A PHYSICAL JUMPER FROM PA8 to LEDO
 */
#include "CH57x_gpio.h"   // Defines GPIOA_ModeCfg, GPIOA_SetBits, etc.
#include "CH57x_sys.h"    // Declares mDelaymS()
#include "CH57x_uart.h" // Declares UART0_DefInit() or similar

// For clarity, let's define our specific LED pin
#define LED_PIN     GPIO_Pin_11

void DebugInit(void)
{
    GPIOA_SetBits(bTXD_0);
    GPIOA_ModeCfg(bTXD_0, GPIO_ModeOut_PP_5mA);
    UART_Remap(ENABLE, UART_TX_REMAP_PA3, UART_RX_REMAP_PA2);
    UART_DefInit();
}

void LED_Init(void)
{
    GPIOA_ModeCfg(LED_PIN, GPIO_ModeOut_PP_5mA);
    GPIOA_SetBits(LED_PIN);
}

/**
 * @brief  Main routine.
 */
int main()
{
    /* System clock initialization */

    SetSysClock(CLK_SOURCE_HSE_PLL_100MHz);
    GPIOA_ModeCfg(GPIO_Pin_All, GPIO_ModeIN_PU);

    DebugInit();
    DelayMs(1000);

    LED_Init();


    /* Infinite loop to blink the LED */
    while(1)
    {
        /*
         * Turn LED ON.
         * We call the macro "GPIOA_ResetBits" directly.
         */
        GPIOA_ResetBits(LED_PIN);

        /* Wait for 500 milliseconds */
        DelayMs(100);

        /*
         * Turn LED OFF.
         * We call the macro "GPIOA_SetBits" directly.
         */
        GPIOA_SetBits(LED_PIN);

        /* Wait for 500 milliseconds */
        DelayMs(1000);
    }
}
