#ifndef __BLE_H_
#define __BLE_H_
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
#include ".\UART\UART.H"
#include ".\GPIO\GPIO.H"
#include "user_type.h"

typedef struct
{
    unsigned char *cmd;
    unsigned char *bkHead;
    unsigned char *bkKey;
} cmd_map_st;
typedef enum
{
    BLE_PPP,
    BLE_SETBR,
    BLE_GETNAME,
    BLE_SETNAME,
    BLE_GETADDR,
    BLE_SETADDR,
    BLE_GETSTATUS,
    BLE_SETADVINT,
    BLE_SETCONNINT,
    BLE_DISCONNECT,
    BLE_RESTART,
    BLE_GETPARA,
    BLE_EXIT,
    BLE_SHUTDOWN,
    BLE_RESTORE,
    BLE_GETINFO,
    BLE_HELP,
    BLE_TRANSMISSION,
} ble_cmd_t;
typedef enum
{
    BLEINIT_STEP0,
    BLEINIT_STEP1,
    BLEINIT_STEP2,
    BLEINIT_STEP3,
    BLEINIT_STEP4,
    BLEINIT_STEP5,
    BLEINIT_STEP6,
    BLEINIT_STEP7,
    BLEINIT_STEP8,
    BLEINIT_STEP9,
    BLEINIT_STEPA,
    BLEINIT_STEPB,
    BLEINIT_STEPC,
    BLEINIT_STEPD,
    BLEINIT_STEPE,
    BLEINIT_STEPF,
}  ;

void ble(void);
void bleInit(void);
void bleUart1DataIn(UINT8 dat);

#endif  //__BLE_H_
