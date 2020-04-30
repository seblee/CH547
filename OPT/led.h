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

extern volatile _USR_FLAGA_type ledState[4];
#define led1State ledState[0].s4bits.s0
#define led2State ledState[0].s4bits.s1
#define led3State ledState[1].s4bits.s0
#define led4State ledState[1].s4bits.s1
#define led5State ledState[2].s4bits.s0
#define led6State ledState[2].s4bits.s1
#define led7State ledState[3].s4bits.s0
#define led8State ledState[3].s4bits.s1
#define led9State ledState[4].s4bits.s0

void LED_Port_Init(void);
void ledDisplay(void);

#endif
