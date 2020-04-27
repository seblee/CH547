/********************************** (C) COPYRIGHT *******************************
* File Name          : Main.C
* Author             : WCH
* Version            : V1.0
* Date               : 2018/08/09
* Description        : CH547 GPIO功能演示,普通输入输出测试；中断触发模式
                       注意包含DEBUG.C
*******************************************************************************/
#include ".\Public\CH547.H"
#include ".\Public\DEBUG.H"
#include ".\GPIO\GPIO.H"
#include ".\Timer\Timer.H"
#include ".\TouchKey\TouchKey.H"
#include ".\UART\UART.H"
#include ".\PWM\PWM.H"

#pragma NOAREGS
// sbit LED2 = P2 ^ 2;
sbit LED3 = P2 ^ 3;
sbit LED4 = P2 ^ 4;
sbit LED5 = P2 ^ 5;

bit beepFlag    = 0;
UINT8 beepCount = 1;

/*******************************************************************************
 * Function Name  : LED_Port_Init
 * Description    : LED引脚初始化,推挽输出
 *                  P22~P25
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void LED_Port_Init(void)
{
    P2 |= (0xF << 2);  //默认熄灭
    P2_MOD_OC &= ~(0xF << 2);
    P2_DIR_PU |= (0xF << 2);
}

void main()
{
    CfgFsys();  // CH547时钟选择配置
    mDelaymS(20);
    mInitSTDIO();  //串口0初始化
    printf("Start @ChipID=%02X\n", (UINT16)CHIP_ID);
    LED_Port_Init();
    CH547WDTModeSelect(1);
#ifdef T0_INT
    timer0Init();
#endif
    TouchKey_Init();
    /* 时钟 频率设置 */
    SetPWMClkDiv(4);     // PWM时钟配置,FREQ_SYS/4
    SetPWMCycle64Clk();  // PWM周期 FREQ_SYS/4/64
    SetPWM3Dat(64);
    PWM_SEL_CHANNEL(CH3, Enable);

    LED3 = 1;
    LED4 = 1;
    LED5 = 1;
    while (1)
    {
        // I2C_IO_Check();
        if (flag1ms)
        {
            flag1ms = 0;
        }
        if (flag10ms)
        {
            getKeyBitMap();
            if (KEY1)
            {
                printf("key1 pressed\n");
            }
            if (KEY2)
            {
                printf("key2 pressed\n");
            }
            if (KEY3)
            {
                printf("key3 pressed\n");
            }
            if (KEY4)
            {
                printf("key4 pressed\n");
            }

            if (KEY1Restan)
            {
                printf("key1Restan pressed\n");
            }
            if (KEY2Restan)
            {
                printf("key2Restan pressed\n");
            }
            if (KEY3Restan)
            {
                printf("key3Restan pressed\n");
            }
            if (KEY4Restan)
            {
                printf("key4Restan pressed\n");
            }

            flag10ms = 0;
        }
        if (flag63ms)
        {
            flag63ms = 0;
            if (beepFlag)
            {
                beepFlag = 0;
                beepOFF;
            }
            else if (beepCount)
            {
                beepCount--;
                beepFlag = 1;
                beepON;
            }
        }
        if (flag250ms)
        {
            flag250ms = 0;
            CH547WDTFeed(0);
        }
        if (flag500ms)
        {
            // LED5      = ~LED5;
            flag500ms = 0;
        }
    }
}
