/**
 ****************************************************************************
 * @Warning Without permission from the author,Not for commercial use
 * @File    beep.c
 * @Author  xiaowine@cee0.com
 * @date
 * @version V1.0
 *************************************************
 * @brief   ��עϵͳ��Ϣ
 ****************************************************************************
 * @attention
 * Powered By Xiaowine
 * <h2><center>&copy;  Copyright(C) cee0.com 2015-2019</center></h2>
 * All rights reserved
 *
 **/
#include "beep.h"

bit beepFlag    = 0;
UINT8 beepCount = 1;

void beepInit(void)
{
    /* ʱ�� Ƶ������ */
    SetPWMClkDiv(4);     // PWMʱ������,FREQ_SYS/4
    SetPWMCycle64Clk();  // PWM���� FREQ_SYS/4/64
    SetPWM3Dat(64);
    PWM_SEL_CHANNEL(CH3, Enable);
}

void beepShortBee(void)
{
    // if (beepFlag)
    // {
    //    beepFlag  = 1;
    //     beepON;
    //     beepCount = 0;
    // }
    // else
    {
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
}
