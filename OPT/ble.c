/**
 ****************************************************************************
 * @Warning Without permission from the author,Not for commercial use
 * @File    ble.c
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
#include "ble.h"

#define RXMAX 40

static UINT8 rxCount       = 0;
static UINT8 rxStep        = 0;
static UINT8 txBuff[30]    = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0x0a};
static UINT8 rxBuff[RXMAX] = {0};
static UINT8 txCount       = 0;

static UINT16 delayCount    = 0;
static ble_cmd_t bleCmdType = BLE_PPP;

static char bleAddr[30] = {0};
static char bleName[30] = {0};
bit timeToTransFlag     = 0;
bit bleInitFlag         = 0;
bit commandModeFlag     = 0;
bit commandOKFlag       = 0;
bit uartGetFlag         = 0;
bit uartRecOKFlag       = 0;
bit regDataOkFlag       = 0;
bit txCompleteFlag      = 1;

sbit BLE_ON  = P2 ^ 5;
sbit UART_SW = P2 ^ 3;
sbit PWR_ON  = P2 ^ 4;

extern UINT8C protocolHeader[2];

const cmd_map_st AT_command[17] = {
    {"+++", "AT+", "Mode=AT Mode"},                             // BLE_PPP
    {"AT+setBR 9600\r\n", "AT+", ""},                           // BLE_SETBR
    {"AT+getName\r\n", "AT+", ""},                              // BLE_GETNAME
    {"AT+setName=", "AT+", "restart effect!"},                  // BLE_SETNAME
    {"AT+getAddr\r\n", "AT+", ""},                              // BLE_GETADDR
    {"AT+setAddr=112233445566\r\n", "AT+", "restart effect!"},  // BLE_SETADDR
    {"AT+getStatus\r\n", "AT+", ""},                            // BLE_GETSTATUS
    {"AT+setAdvInt 80 100\r\n", "AT+", "restart effect!"},      // BLE_SETADVINT
    {"AT+setConnInt 36 24 23 00\r\n", "AT+", ""},               // BLE_SETCONNINT
    {"AT+disConnect\r\n", "AT+", ""},                           // BLE_DISCONNECT
    {"AT+reStart\r\n", "AT+", "restart now"},                   // BLE_RESTART
    {"AT+getPara\r\n", "AT+", ""},                              // BLE_GETPARA
    {"AT+exit\r\n", "AT+", "Mode=Normal"},                      // BLE_EXIT
    {"AT+shutDown\r\n", "AT+", "power off now!"},               // BLE_SHUTDOWN
    {"AT+reStore\r\n", "AT+", "restart now"},                   // BLE_RESTORE
    {"AT+getInfo\r\n", "AT+", ""},                              // BLE_GETINFO
    {"AT+help\r\n", "AT+", ""},                                 // BLE_HELP
};

void bleRxProcess(void);
void blePushCmdSendBuff(ble_cmd_t cmd, const char *dat);
void bleInitialization();
UINT8 getCheckSum(UINT8 *dat);

void bleInit(void)
{
    GPIO_Init(PORT2, PIN5, MODE0);         // BLE_ON
    GPIO_Init(PORT2, PIN3 | PIN4, MODE1);  // UART_SW  PWR_ON
    UART_SW = 1;
    PWR_ON  = 1;

    IP_EX |= bIP_UART1;
    CH549UART1Init();
}

void bleUart1DataIn(UINT8 dat)
{
    if (rxCount < RXMAX)
    {
        rxBuff[rxCount] = dat;
        rxCount++;
        uartGetFlag = 1;
    }
}
void bleSend(void)
{
    static UINT8 index = 0;
    if (SIF1 & bU1TI)
    {
        SIF1           = bU1TI;  //清除接收完中断
        txCompleteFlag = 1;
    }

    if (txCompleteFlag && (index < txCount))
    {
        SBUF1 = txBuff[index++];
        if (index == txCount)
        {
            index   = 0;
            txCount = 0;
        }
        txCompleteFlag = 0;
    }
}

void ble(void)
{
    bleSend();
    bleRxProcess();
    if ((txCount == 0) && (commandModeFlag == 0) && (regDataOkFlag == 1) && (BLE_ON == 0))
    {
        blePushCmdSendBuff(BLE_TRANSMISSION, txBuff);
        regDataOkFlag = 0;
    }
    if (delayCount)
    {
        if (flag63ms)
        {
            delayCount--;
        }
        return;
    }
    if (flag63ms)
        timeToTransFlag = 1;

    if (!bleInitFlag)
    {
        bleInitialization();
    }
    else
    {
        // bleInitFlag
    }
}

void bleInitialization()
{
    static char step = 0;
    switch (step)
    {
        case 0:
            delayCount = 100;
            step++;
            break;
        case 1:
            if ((!commandModeFlag) && (txCount == 0))
            {
                printf("step:%d", (UINT16)step);
                commandModeFlag = 1;
                commandOKFlag   = 0;
                blePushCmdSendBuff(BLE_PPP, 0);
                step = 2;
            }
            break;
        case 2:
            if ((commandOKFlag) && (txCount == 0))
            {
                printf("step:%d", (UINT16)step);
                commandOKFlag = 0;
                blePushCmdSendBuff(BLE_GETADDR, 0);
                step = 3;
            }
            break;
        case 3:
            if ((commandOKFlag) && (txCount == 0))
            {
                commandOKFlag = 0;
                blePushCmdSendBuff(BLE_GETNAME, 0);
                step = 4;
            }
            break;
        case 4:
            if ((commandOKFlag) && (txCount == 0))
            {
                if (strncmp(bleName, bleAddr, 17))
                {
                    commandOKFlag = 0;
                    blePushCmdSendBuff(BLE_SETNAME, bleAddr);
                    step = 5;
                }
                else
                {
                    step = 7;
                }
            }
            break;
        case 5:
            if ((commandOKFlag) && (txCount == 0))
            {
                commandOKFlag = 0;
                blePushCmdSendBuff(BLE_RESTART, 0);
                step = 6;
            }
            break;
        case 6:
            if ((commandOKFlag) && (txCount == 0))
            {
                step = 0;
            }
            break;
        case 7:
            if ((commandOKFlag) && (txCount == 0))
            {
                commandOKFlag = 0;
                blePushCmdSendBuff(BLE_EXIT, 0);
                step = 8;
            }
            break;
        case 8:
            if (commandOKFlag)
            {
                bleInitFlag = 1;
            }
            break;
        default:
            break;
    }
}

void blePushCmdSendBuff(ble_cmd_t cmd, const char *dat)
{
    bleCmdType = cmd;
    memset(txBuff, 0, sizeof(txBuff));
    if (cmd == BLE_TRANSMISSION)
    {
        txCount = *(dat + 3) + 5;
        memcpy(txBuff, dat, txCount);
    }
    else
    {
        strcpy(txBuff, AT_command[bleCmdType].cmd);
        if (dat)
        {
            strcat(txBuff, dat);
            strcat(txBuff, "\r\n");
        }
        txCount = strlen(txBuff);
    }
}
void bleRxProcess(void)
{
    if (uartGetFlag)
    {
        uartGetFlag = 0;
        if (commandModeFlag)
        {
            if ((strstr(rxBuff, "\r\n")) || (rxCount >= 25))
            {
                uartRecOKFlag = 1;
            }
        }
        else
        {
        again:
            if (rxStep == 0)
            {
                if (rxCount < 2)
                    goto rxContinue;
                if (memcmp(rxBuff, (void *)protocolHeader, 2) == 0)
                    rxStep = 1;
                else
                {
                    memcpy(rxBuff, rxBuff + 1, rxCount - 1);
                    rxCount--;
                    goto again;
                }
            }

            if (rxStep == 1)
            {
                if (rxCount < 4)
                    goto rxContinue;
                if (rxBuff[3] <= 15)
                    rxStep = 2;
                else
                {
                    memcpy(rxBuff, rxBuff + 2, rxCount - 2);
                    rxCount -= 2;
                    rxStep = 0;
                    goto again;
                }
            }

            if (rxStep == 2)
            {
                UINT8 checkSum, checkIndex;
                if (rxCount < rxBuff[3] + 5)
                    goto rxContinue;
                checkSum   = getCheckSum(rxBuff);
                checkIndex = rxBuff[3] + 4;
                if (checkSum == rxBuff[checkIndex])
                {
                    rxStep = 3;
                }
                else
                {
                    memcpy(rxBuff, rxBuff + 4, rxCount - 4);
                    rxCount -= 4;
                    rxStep = 0;
                    goto again;
                }
            }
        }
    }
rxContinue:
    if (uartRecOKFlag)
    {
        char *atBak   = strstr(rxBuff, "AT+ok");
        uartRecOKFlag = 0;
        if (atBak)
        {
            switch (bleCmdType)
            {
                case BLE_PPP:
                    if (strstr(rxBuff, "AT+"))
                    {
                        commandOKFlag = 1;
                    }
                    break;
                // case BLE_SETBR:                    break;
                case BLE_GETNAME:
                    commandOKFlag = 1;
                    strncpy(bleName, atBak + strlen("AT+ok "), 20);
                    break;
                case BLE_SETNAME:
                    commandOKFlag = 1;
                    if (strstr(rxBuff, AT_command[bleCmdType].bkKey))
                    {
                    }
                    break;
                case BLE_GETADDR:
                    commandOKFlag = 1;
                    strncpy(bleAddr, atBak + strlen("AT+ok "), 20);
                    break;
                    // case BLE_GETSTATUS:                    break;
                    // case BLE_SETADVINT:                    break;
                    // case BLE_SETCONNINT:                    break;
                    // case BLE_DISCONNECT:                    break;
                case BLE_RESTART:
                    commandOKFlag = 1;
                    if (strstr(rxBuff, AT_command[bleCmdType].bkKey))
                    {
                        commandModeFlag = 0;
                    }
                    break;
                    // case BLE_GETPARA:                    break;
                case BLE_EXIT:
                    if (strstr(rxBuff, AT_command[bleCmdType].bkKey))
                    {
                        commandModeFlag = 0;
                    }
                    commandOKFlag = 1;
                    break;
                // case BLE_SHUTDOWN:                    break;
                // case BLE_RESTORE:                    break;
                // case BLE_GETINFO:                    break;
                // case BLE_HELP:                    break;
                default:
                    commandOKFlag = 1;
                    break;
            }
        }
        else if (strstr(rxBuff, "AT+err"))
        {
            switch (bleCmdType)
            {
                case BLE_PPP:
                    commandOKFlag = 1;
                    break;

                default:
                    break;
            }
        }
        memset(rxBuff, 0, sizeof(rxBuff));
        rxCount = 0;
    }
}
