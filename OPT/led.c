/**
 ****************************************************************************
 * @Warning Without permission from the author,Not for commercial use
 * @File    led.c
 * @Author  xiaowine@cee0.com
 * @date
 * @version V1.0
 *************************************************
 * @brief   标注系统信息
 ****************************************************************************
 * @attention
 * Powered By Xiaowine
 * <h2><center>&copy;  Copyright(C) cee0.com 2015-2019</center></h2>
 * All rights reserved
 *
 **/
#include "led.h"

volatile _USR_FLAGA_type ledState[7] = {0};
UINT8 ledFlashFast[LEDNUM]           = {0};

bit flashFlag_0_5HZ;
bit flashFlag_1HZ;
bit flashFlag_2HZ;

static UINT16 ledValue = 0;

void LED_Port_Init(void)
{
    P1 &= ~0x38;  //默认熄灭
    P1_MOD_OC &= ~0x38;
    P1_DIR_PU |= 0x38;

    P2 &= ~0x03;  //默认熄灭
    P2_MOD_OC &= ~0x03;
    P2_DIR_PU |= 0x03;

    P3 &= ~0x78;  //默认熄灭
    P3_MOD_OC &= ~0x78;
    P3_DIR_PU |= 0x78;

    P4 &= ~0x4f;  //默认熄灭
    P4_MOD_OC &= ~0x4f;
    P4_DIR_PU |= 0x4f;
}
void ledGo(void)
{
    UINT8 i                    = 0;
    static UINT16 ledValueTemp = 1;
    if (flag500ms)
    {
        UINT16 ValueTemp = ~ledValueTemp;
        for (i = 0; i < 14; i++)
        {
            if (ValueTemp & (1 << i))
                ledSetState(i, LED_OFF);
            else
            {
                ledSetState(i, LED_ON);
            }
        }

        ledValueTemp <<= 1;
        if (ledValueTemp == 0x4000)
        {
            ledValueTemp = 1;
        }
    }
}

void ledDisplay(void)
{
    UINT8 i, State;
    UINT16 ledValueTemp = ledValue;
    bit value;
    if (flag250ms)
    {
        if (flashFlag_2HZ)
        {
            flashFlag_2HZ = 0;
        }
        else
        {
            flashFlag_2HZ = 1;
        }
    }
    if (flag500ms)
    {
        if (flashFlag_1HZ)
        {
            flashFlag_1HZ = 0;
        }
        else
        {
            flashFlag_1HZ = 1;
        }
        if (flashFlag_1HZ)
        {
            if (flashFlag_0_5HZ)
            {
                flashFlag_0_5HZ = 0;
            }
            else
            {
                flashFlag_0_5HZ = 1;
            }
        }
    }

    for (i = 0; i < LEDNUM; i++)
    {
        value = ((ledValueTemp >> i) & 1);
        if (i % 2 == 0)
        {
            State = ledState[i / 2].byte & 0x0f;
        }
        else
        {
            State = (ledState[i / 2].byte >> 4);
        }

        if (ledFlashFast[i] > 0)
        {
            if (State >= STATE_LED_FLASH_1_T)
            {
                ledFlashFast[i] += 2 * (State - 4);
                if (i % 2 == 0)
                {
                    ledState[i / 2].byte &= 0xf0;
                }
                else
                {
                    ledState[i / 2].byte &= 0x0f;
                }
            }
            if (flag63ms)
            {
                ledFlashFast[i]--;
                value = !((ledValueTemp >> i) & 1);
            }
        }
        else
        {
            switch (State)
            {
                case STATE_LED_OFF:
                    value = LED_OFF;
                    break;
                case STATE_LED_ON:
                    value = LED_ON;
                    break;
                case STATE_LED_FLASH_2HZ:
                    if (flag250ms)
                        value = flashFlag_2HZ;
                    break;
                case STATE_LED_FLASH_1HZ:
                    if (flag500ms)
                        value = flashFlag_1HZ;
                    break;
                case STATE_LED_FLASH_0_5HZ:
                    if (flag500ms)
                        value = flashFlag_0_5HZ;
                    break;

                case STATE_LED_FLASH_1_T:  //闪烁一下
                case STATE_LED_FLASH_2_T:  //闪烁两下
                case STATE_LED_FLASH_3_T:  //闪烁三下
                    ledFlashFast[i] += 2 * (State - 4);
                    if (i % 2 == 0)
                    {
                        ledState[i / 2].byte &= 0xf0;
                    }
                    else
                    {
                        ledState[i / 2].byte &= 0x0f;
                    }
                    break;
            }
        }
        if (value)
        {
            ledValueTemp |= 1 << i;
        }
        else
        {
            ledValueTemp &= ~(1 << i);
        }
        if (ledValueTemp ^ ledValue)
        {
            if (ledValueTemp & (1 << i))
                ledSetState(i, LED_OFF);
            else
            {
                ledSetState(i, LED_ON);
            }
        }
        ledValue = ledValueTemp;
    }

    /*******led1****************/
    // if (ledFlashFast[0] > 0)
    // {
    //     if (led1State == STATE_LED_FLASH_2_T)
    //     {
    //         ledFlashFast[0] += 4;
    //     }
    //     if (flag63ms)
    //     {
    //         ledFlashFast[0]--;
    //         LED1 = !LED1;
    //     }
    // }
    // else
    // {
    //     switch (led1State)
    //     {
    //         case STATE_LED_OFF:
    //             LED1 = LED_OFF;
    //             break;
    //         case STATE_LED_ON:
    //             LED1 = LED_ON;
    //             break;
    //         case STATE_LED_FLASH_2HZ:
    //             if (flag250ms)
    //                 LED1 = flashFlag_2HZ;
    //             break;
    //         case STATE_LED_FLASH_1HZ:
    //             if (flag500ms)
    //                 LED1 = flashFlag_1HZ;
    //             break;
    //         case STATE_LED_FLASH_0_5HZ:
    //             if (flag500ms)
    //                 LED1 = flashFlag_0_5HZ;
    //             break;
    //         case STATE_LED_FLASH_2_T:  //闪烁两下
    //             ledFlashFast[0] += 4;
    //             led1State = 0;
    //             break;
    //     }
    // }
    // /*******led2****************/
    // if (ledFlashFast[1] > 0)
    // {
    //     if (led2State == STATE_LED_FLASH_2_T)
    //     {
    //         ledFlashFast[1] += 4;
    //     }
    //     if (flag63ms)
    //     {
    //         ledFlashFast[1]--;
    //         LED2 = !LED2;
    //     }
    // }
    // else
    // {
    //     switch (led2State)
    //     {
    //         case STATE_LED_OFF:
    //             LED2 = LED_OFF;
    //             break;
    //         case STATE_LED_ON:
    //             LED2 = LED_ON;
    //             break;
    //         case STATE_LED_FLASH_2HZ:
    //             if (flag250ms)
    //                 LED2 = flashFlag_2HZ;
    //             break;
    //         case STATE_LED_FLASH_1HZ:
    //             if (flag500ms)
    //                 LED2 = flashFlag_1HZ;
    //             break;
    //         case STATE_LED_FLASH_0_5HZ:
    //             if (flag500ms)
    //                 LED2 = flashFlag_0_5HZ;
    //             break;
    //         case STATE_LED_FLASH_2_T:  //闪烁两下
    //             ledFlashFast[1] += 4;
    //             led2State = 0;
    //             break;
    //     }
    // }
    /*******led3****************/
    // if (ledFlashFast[2] > 0)
    // {
    //     if (led3State == STATE_LED_FLASH_2_T)
    //     {
    //         ledFlashFast[2] += 4;
    //     }
    //     if (flag63ms)
    //     {
    //         ledFlashFast[2]--;
    //         LED3 = !LED3;
    //     }
    // }
    // else
    // {
    //     switch (led3State)
    //     {
    //         case STATE_LED_OFF:
    //             LED3 = LED_OFF;
    //             break;
    //         case STATE_LED_ON:
    //             LED3 = LED_ON;
    //             break;
    //         case STATE_LED_FLASH_2HZ:
    //             if (flag250ms)
    //                 LED3 = flashFlag_2HZ;
    //             break;
    //         case STATE_LED_FLASH_1HZ:
    //             if (flag500ms)
    //                 LED3 = flashFlag_1HZ;
    //             break;
    //         case STATE_LED_FLASH_0_5HZ:
    //             if (flag500ms)
    //                 LED3 = flashFlag_0_5HZ;
    //             break;
    //         case STATE_LED_FLASH_2_T:  //闪烁两下
    //             ledFlashFast[2] += 4;
    //             led3State = 0;
    //             break;
    //     }
    // }
    /*******led4****************/
    // if (ledFlashFast[3] > 0)
    // {
    //     if (led4State == STATE_LED_FLASH_2_T)
    //     {
    //         ledFlashFast[3] += 4;
    //     }
    //     if (flag63ms)
    //     {
    //         ledFlashFast[3]--;
    //         LED4 = !LED4;
    //     }
    // }
    // else
    // {
    //     switch (led4State)
    //     {
    //         case STATE_LED_OFF:
    //             LED4 = LED_OFF;
    //             break;
    //         case STATE_LED_ON:
    //             LED4 = LED_ON;
    //             break;
    //         case STATE_LED_FLASH_2HZ:
    //             if (flag250ms)
    //                 LED4 = flashFlag_2HZ;
    //             break;
    //         case STATE_LED_FLASH_1HZ:
    //             if (flag500ms)
    //                 LED4 = flashFlag_1HZ;
    //             break;
    //         case STATE_LED_FLASH_0_5HZ:
    //             if (flag500ms)
    //                 LED4 = flashFlag_0_5HZ;
    //             break;
    //         case STATE_LED_FLASH_2_T:  //闪烁两下
    //             ledFlashFast[3] += 4;
    //             led4State = 0;
    //             break;
    //     }
    // }

    // /*******led5****************/
    // if (ledFlashFast[4] > 0)
    // {
    //     if (led5State == STATE_LED_FLASH_2_T)
    //     {
    //         ledFlashFast[4] += 4;
    //     }
    //     if (flag63ms)
    //     {
    //         ledFlashFast[4]--;
    //         LED5 = !LED5;
    //     }
    // }
    // else
    // {
    //     switch (led5State)
    //     {
    //         case STATE_LED_OFF:
    //             LED5 = LED_OFF;
    //             break;
    //         case STATE_LED_ON:
    //             LED5 = LED_ON;
    //             break;
    //         case STATE_LED_FLASH_2HZ:
    //             if (flag250ms)
    //                 LED5 = flashFlag_2HZ;
    //             break;
    //         case STATE_LED_FLASH_1HZ:
    //             if (flag500ms)
    //                 LED5 = flashFlag_1HZ;
    //             break;
    //         case STATE_LED_FLASH_0_5HZ:
    //             if (flag500ms)
    //                 LED5 = flashFlag_0_5HZ;
    //             break;
    //         case STATE_LED_FLASH_2_T:  //闪烁两下
    //             ledFlashFast[4] += 4;
    //             led5State = 0;
    //             break;
    //     }
    // }
    // /*******led6****************/
    // if (ledFlashFast[5] > 0)
    // {
    //     if (led6State == STATE_LED_FLASH_2_T)
    //     {
    //         ledFlashFast[5] += 4;
    //     }
    //     if (flag63ms)
    //     {
    //         ledFlashFast[5]--;
    //         LED6 = !LED6;
    //     }
    // }
    // else
    // {
    //     switch (led6State)
    //     {
    //         case STATE_LED_OFF:
    //             LED6 = LED_OFF;
    //             break;
    //         case STATE_LED_ON:
    //             LED6 = LED_ON;
    //             break;
    //         case STATE_LED_FLASH_2HZ:
    //             if (flag250ms)
    //                 LED6 = flashFlag_2HZ;
    //             break;
    //         case STATE_LED_FLASH_1HZ:
    //             if (flag500ms)
    //                 LED6 = flashFlag_1HZ;
    //             break;
    //         case STATE_LED_FLASH_0_5HZ:
    //             if (flag500ms)
    //                 LED6 = flashFlag_0_5HZ;
    //             break;
    //         case STATE_LED_FLASH_2_T:  //闪烁两下
    //             ledFlashFast[5] += 4;
    //             led6State = 0;
    //             break;
    //     }
    // }
    // /*******led7****************/
    // if (ledFlashFast[6] > 0)
    // {
    //     if (led7State == STATE_LED_FLASH_2_T)
    //     {
    //         ledFlashFast[6] += 4;
    //     }
    //     if (flag63ms)
    //     {
    //         ledFlashFast[6]--;
    //         LED7 = !LED7;
    //     }
    // }
    // else
    // {
    //     switch (led7State)
    //     {
    //         case STATE_LED_OFF:
    //             LED7 = LED_OFF;
    //             break;
    //         case STATE_LED_ON:
    //             LED7 = LED_ON;
    //             break;
    //         case STATE_LED_FLASH_2HZ:
    //             if (flag250ms)
    //                 LED7 = flashFlag_2HZ;
    //             break;
    //         case STATE_LED_FLASH_1HZ:
    //             if (flag500ms)
    //                 LED7 = flashFlag_1HZ;
    //             break;
    //         case STATE_LED_FLASH_0_5HZ:
    //             if (flag500ms)
    //                 LED7 = flashFlag_0_5HZ;
    //             break;
    //         case STATE_LED_FLASH_2_T:  //闪烁两下
    //             ledFlashFast[6] += 4;
    //             led7State = 0;
    //             break;
    //     }
    // }
    // /*******led8****************/
    // if (ledFlashFast[7] > 0)
    // {
    //     if (led8State == STATE_LED_FLASH_2_T)
    //     {
    //         ledFlashFast[7] += 4;
    //     }
    //     if (flag63ms)
    //     {
    //         ledFlashFast[7]--;
    //         LED8 = !LED8;
    //     }
    // }
    // else
    // {
    //     switch (led8State)
    //     {
    //         case STATE_LED_OFF:
    //             LED8 = LED_OFF;
    //             break;
    //         case STATE_LED_ON:
    //             LED8 = LED_ON;
    //             break;
    //         case STATE_LED_FLASH_2HZ:
    //             if (flag250ms)
    //                 LED8 = flashFlag_2HZ;
    //             break;
    //         case STATE_LED_FLASH_1HZ:
    //             if (flag500ms)
    //                 LED8 = flashFlag_1HZ;
    //             break;
    //         case STATE_LED_FLASH_0_5HZ:
    //             if (flag500ms)
    //                 LED8 = flashFlag_0_5HZ;
    //             break;
    //         case STATE_LED_FLASH_2_T:  //闪烁两下
    //             ledFlashFast[7] += 4;
    //             led8State = 0;
    //             break;
    //     }
    // }
}

void ledSetState(UINT8 num, ledState_t state)
{
    switch (num)
    {
        case LEDNUM0:
            if (state == LEDON)
            {
                LED0 = LED_ON;
            }
            else
            {
                LED0 = LED_OFF;
            }
            break;
        case LEDNUM1:
            if (state == LEDON)
            {
                LED1 = LED_ON;
            }
            else
            {
                LED1 = LED_OFF;
            }
            break;
        case LEDNUM2:
            if (state == LEDON)
            {
                LED2 = LED_ON;
            }
            else
            {
                LED2 = LED_OFF;
            }
            break;
        case LEDNUM3:
            if (state == LEDON)
            {
                LED3 = LED_ON;
            }
            else
            {
                LED3 = LED_OFF;
            }
            break;
        case LEDNUM4:
            if (state == LEDON)
            {
                LED4 = LED_ON;
            }
            else
            {
                LED4 = LED_OFF;
            }
            break;
        case LEDNUM5:
            if (state == LEDON)
            {
                LED5 = LED_ON;
            }
            else
            {
                LED5 = LED_OFF;
            }
            break;
        case LEDNUM6:
            if (state == LEDON)
            {
                LED6 = LED_ON;
            }
            else
            {
                LED6 = LED_OFF;
            }
            break;
        case LEDNUM7:
            if (state == LEDON)
            {
                LED7 = LED_ON;
            }
            else
            {
                LED7 = LED_OFF;
            }
            break;
        case LEDNUM8:
            if (state == LEDON)
            {
                LED8 = LED_ON;
            }
            else
            {
                LED8 = LED_OFF;
            }
            break;
        case LEDNUM9:
            if (state == LEDON)
            {
                LED9 = LED_ON;
            }
            else
            {
                LED9 = LED_OFF;
            }
            break;
        case LEDNUM10:
            if (state == LEDON)
            {
                LED10 = LED_ON;
            }
            else
            {
                LED10 = LED_OFF;
            }
            break;
        case LEDNUM11:
            if (state == LEDON)
            {
                LED11 = LED_ON;
            }
            else
            {
                LED11 = LED_OFF;
            }
            break;
        case LEDNUM12:
            if (state == LEDON)
            {
                LED12 = LED_ON;
            }
            else
            {
                LED12 = LED_OFF;
            }
            break;
        case LEDNUM13:
            if (state == LEDON)
            {
                LED13 = LED_ON;
            }
            else
            {
                LED13 = LED_OFF;
            }
            break;
        default:
            break;
    }
}