#ifndef __LED_H_
#define __LED_H_
/**
 ****************************************************************************
 * @Warning Without permission from the author,Not for commercial use
 * @File
 * @Author  xiaowine@cee0.com
 * @date
 * @version V1.0
 *************************************************
 * @brief   -->>
 ****************************************************************************
 * @attention
 * Powered By Xiaowine
 * <h2><center>&copy;  Copyright(C) cee0.com 2015-2019</center></h2>
 * All rights reserved
 *
 **/
#include ".\Public\CH547.H"
#include ".\Public\DEBUG.H"
#include ".\GPIO\GPIO.H"
#include "user_type.h"

typedef enum
{
    LEDON   = 0,
    LED_ON  = 0,
    LED_OFF = 1,
    LEDOFF  = !LEDON,
} ledState_t;
enum
{
    LEDNUM0  = 0,
    LEDNUM1  = 1,
    LEDNUM2  = 2,
    LEDNUM3  = 3,
    LEDNUM4  = 4,
    LEDNUM5  = 5,
    LEDNUM6  = 6,
    LEDNUM7  = 7,
    LEDNUM8  = 8,
    LEDNUM9  = 9,
    LEDNUM10 = 10,
    LEDNUM11 = 11,
    LEDNUM12 = 12,
    LEDNUM13 = 13,
};

sbit LED0  = P3 ^ 3;
sbit LED1  = P3 ^ 4;
sbit LED2  = P3 ^ 5;
sbit LED3  = P3 ^ 6;
sbit LED4  = P4 ^ 3;
sbit LED5  = P4 ^ 2;
sbit LED6  = P4 ^ 6;
sbit LED7  = P4 ^ 1;
sbit LED8  = P4 ^ 0;
sbit LED9  = P2 ^ 0;
sbit LED10 = P2 ^ 1;
sbit LED11 = P1 ^ 5;
sbit LED12 = P1 ^ 4;
sbit LED13 = P1 ^ 3;

#define LEDNUM 14

extern volatile _USR_FLAGA_type ledState[7];
#define led0State ledState[0].s4bits.s0
#define led1State ledState[0].s4bits.s1
#define led2State ledState[1].s4bits.s0
#define led3State ledState[1].s4bits.s1
#define led4State ledState[2].s4bits.s0
#define led5State ledState[2].s4bits.s1
#define led6State ledState[3].s4bits.s0
#define led7State ledState[3].s4bits.s1
#define led8State ledState[4].s4bits.s0
#define led9State ledState[4].s4bits.s1
#define led10State ledState[5].s4bits.s0
#define led11State ledState[5].s4bits.s1
#define led12State ledState[6].s4bits.s0
#define led13State ledState[6].s4bits.s1

void LED_Port_Init(void);
void ledDisplay(void);
void ledGo(void);
void ledSetState(UINT8 num, ledState_t state);

#endif
