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

#endif  //__BLE_H_
