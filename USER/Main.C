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
#include ".\Timer\Timer.H"
#include ".\TouchKey\TouchKey.H"
#include "beep.h"
#include "user_type.h"
#include "serial.h"
#include "led.h"

#pragma NOAREGS

volatile _TKS_FLAGA_type bitFlag;

/*******************************************************************************
 * Function Name  : LED_Port_Init
 * Description    : LED引脚初始化,推挽输出
 *                  P22~P25
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void getBitFlag(void)
{
    if (flag10)
    {
        flag10   = 0;
        flag10ms = 1;
    }
    if (flag63)
    {
        flag63   = 0;
        flag63ms = 1;
    }
    if (flag250)
    {
        flag250   = 0;
        flag250ms = 1;
    }
    if (flag500)
    {
        flag500   = 0;
        flag500ms = 1;
    }
}
void bitClear(void)
{
    flag10ms  = 0;
    flag63ms  = 0;
    flag250ms = 0;
    flag500ms = 0;
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
    beepInit();
    serialInit();

    while (1)
    {
        if (flag1ms)
        {
            flag1ms = 0;
            getBitFlag();
        }
        if (flag10ms)
        {
            getKeyBitMap();

            // if (KEY1)
            // {
            //     LED8 = ~LED8;
            //     if (LED8)
            //     {
            //         printf("LED8 is OFF\n");
            //     }
            //     else
            //     {
            //         printf("LED8 is ON\n");
            //     }
            // }
            // if (KEY2)
            // {
            //     LED9 = ~LED9;
            //     if (LED9)
            //     {
            //         printf("LED9 is OFF\n");
            //     }
            //     else
            //     {
            //         printf("LED9 is ON\n");
            //     }
            // }
            // if (KEY3)
            // {
            //     LED10 = ~LED10;
            //     if (LED10)
            //     {
            //         printf("LED10 is OFF\n");
            //     }
            //     else
            //     {
            //         printf("LED10 is ON\n");
            //     }
            // }
            // if (KEY4)
            // {
            //     LED11 = ~LED11;
            //     if (LED11)
            //     {
            //         printf("LED11 is OFF\n");
            //     }
            //     else
            //     {
            //         printf("LED11 is ON\n");
            //     }
            // }

            // if (KEY5)
            // {
            //     LED12 = ~LED12;
            //     if (LED12)
            //     {
            //         printf("LED12 is OFF\n");
            //     }
            //     else
            //     {
            //         printf("LED12 is ON\n");
            //     }
            // }
            // if (KEY6)
            // {
            //     LED13 = ~LED13;
            //     if (LED13)
            //     {
            //         printf("LED13 is OFF\n");
            //     }
            //     else
            //     {
            //         printf("LED13 is ON\n");
            //     }
            // }
            // if (KEY7)
            // {
            //     LED7 = ~LED7;
            //     if (LED7)
            //     {
            //         printf("LED7 is OFF\n");
            //     }
            //     else
            //     {
            //         printf("LED7 is ON\n");
            //     }
            // }
        }
        beepShortBee();
        serialOpt();
        touchKeyGet();
        if (flag250ms)
        {
            CH547WDTFeed(0);
        }
        ledDisplay();
        // ledGo();
        if (bitFlag.byte)
        {
            bitClear();
        }
    }
}
