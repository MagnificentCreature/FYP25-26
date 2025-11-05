#include "CH57x_common.h"

/*********************************************************************
 * @fn      ADC_4bit_Sample
 *
 * @brief   CMP模拟4位ADC采样,采样脚(PA7)采样范围（0~825mV）
 *
 * @return  采样值，对应电压的计算公式为：（采样值*25mV）
 */
uint32_t ADC_4bit_Sample()
{
    uint32_t vref=0;
    uint32_t cmp_count=1;
    vref = 8;
    while(1)
    {
        uint8_t cmp_out=0;
        uint8_t cmp_out_1ast=0;
        CMP_Init( cmp_sw_3, vref-1 );// CMP_P0(PA7),CMP_N(CMP_VERF)
        CMP_Enable();
        DelayUs(1);
        cmp_out_1ast = CMP_ReadAPROut();
        while(1)
        {
            cmp_out = CMP_ReadAPROut();
            if(cmp_out == cmp_out_1ast)
            {
                break;
            }
            cmp_out_1ast = cmp_out;
        }

        if(cmp_count==8)
        {
            if (cmp_out)
            {
                if(vref==15)
                {
                    vref = 16;
                }
                else
                {
                    return (vref*2+1);
                }
            }
            else
            {
                return (vref*2-1);
            }
        }
        else
        {
            cmp_count<<=1;
            if (cmp_out)
            {
                vref += 8/cmp_count;
            }
            else
            {
                vref -= 8/cmp_count;
            }
        }
    }
}

uint32_t ADC_Moisture_Sample()
{
    uint32_t vref = 4;  // Start lower
    uint32_t cmp_count = 1;
    
    while(1)
    {
        uint8_t cmp_out = 0;
        uint8_t cmp_out_last = 0;
        
        // Reduce the reference voltage range
        CMP_Init(cmp_sw_3, vref);  
        CMP_Enable();
        DelayUs(10);  // Longer stabilization for higher resistance
        
        cmp_out_last = CMP_ReadAPROut();
        uint8_t stable_count = 0;
        
        while(stable_count < 5)  // More stable reading
        {
            cmp_out = CMP_ReadAPROut();
            if(cmp_out == cmp_out_last) {
                stable_count++;
            } else {
                stable_count = 0;
            }
            cmp_out_last = cmp_out;
            DelayUs(1);
        }

        if(cmp_count >= 16)  // More iterations for finer resolution
        {
            return vref;  // Raw value, scale externally
        }
        
        cmp_count <<= 1;
        if (cmp_out) {
            vref += 16 / cmp_count;  // Smaller steps
        } else {
            vref -= 16 / cmp_count;
        }
        
        if(vref > 31) vref = 31;  // Prevent overflow
    }
}

/*********************************************************************
 * @fn      main
 *
 * @brief   主函数
 *
 * @return  none
 */
int main()
{
    HSECFG_Capacitance(HSECap_18p);
    SetSysClock(CLK_SOURCE_HSE_PLL_100MHz);
    /* 配置串口1：先配置IO口模式，再配置串口 */
    GPIOA_SetBits( bTXD_0 );
    GPIOA_ModeCfg( bTXD_0, GPIO_ModeOut_PP_5mA );          // TXD-配置推挽输出，注意先让IO口输出高电平
    UART_Remap(ENABLE, UART_TX_REMAP_PA3, UART_RX_REMAP_PA2);
    UART_DefInit();
    PRINT( "Start @ChipID=%02X\n", R8_CHIP_ID );
    GPIOA_ModeCfg(GPIO_Pin_7, GPIO_ModeIN_PD);

    while(1) {
        PRINT ("ADC_4bit_Sample %d\n",ADC_Moisture_Sample());

        DelayMs(2000);
    }
    // while(1);
}
