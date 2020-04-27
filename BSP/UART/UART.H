#ifndef __UART_H__
#define __UART_H__
#include ".\Public\CH547.H"
#include ".\Public\DEBUG.H"
/* UART1��� */
#define   UART1_INTERRUPT                           //�����Ƿ�������1�ж�
#define   UART1_BUAD           115200               //����1����������
/* UART2��� */
#define   UART2_INTERRUPT                           //�����Ƿ�������2�ж�
#define   UART2_BUAD           115200               //����2����������
/* UART3��� */
//#define   UART3_INTERRUPT                           //�����Ƿ�������3�ж�
#define   UART3_BUAD           115200               //����2����������
/* �ⲿ�����Ӻ��� */
extern void  CH549UART1Init();                      //UART1��ʼ������
extern void  CH549UART1Alter();                     //����1����ӳ��
extern UINT8 CH549UART1RcvByte( );                  //UART1����һ���ֽ�
extern void  CH549UART1SendByte(UINT8 SendDat);     //UART1����һ���ֽ�
extern void  CH549UART2Init();                      //UART2��ʼ������
extern UINT8 CH549UART2RcvByte( );                  //UART2����һ���ֽ�
extern void  CH549UART2SendByte(UINT8 SendDat);     //UART2����һ���ֽ�
extern void  CH549UART3Init();                      //UART3��ʼ������
extern UINT8 CH549UART3RcvByte( );                  //UART3����һ���ֽ�
extern void  CH549UART3SendByte(UINT8 SendDat);     //UART3����һ���ֽ�
#endif