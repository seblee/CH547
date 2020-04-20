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
#pragma NOAREGS
sbit LED2 = P2 ^ 2;
sbit LED3 = P2 ^ 3;
sbit LED4 = P2 ^ 4;
sbit LED5 = P2 ^ 5;

sfr chip_0 = 0xA1;
sfr chip_1 = 0xA2;
sfr chip_2 = 0xA3;
sfr chip_3 = 0xA4;
void main()
{
    CfgFsys();  // CH547时钟选择配置
    mDelaymS(20);
    mInitSTDIO();  //串口0初始化
    printf("CH547 demo start ...\n");
    printf("CHIP ID:%04x,%ld\n", CHIP_ID, CHIP_ID);
    printf("chip_x %04x %04x %04x %04x \n", chip_0, chip_1, chip_2, chip_3);
    CH547WDTModeSelect(1);
#ifdef T0_INT
    timer0Init();
#endif
    TouchKey_Init();
    while (1)
    {
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
        if (flag250ms)
        {
            LED2      = ~LED2;
            LED4      = ~LED4;
            flag250ms = 0;
            CH547WDTFeed(0);
        }
        if (flag500ms)
        {
            LED3      = ~LED3;
            LED5      = ~LED5;
            flag500ms = 0;
        }
    }
}
