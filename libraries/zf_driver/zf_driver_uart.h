/*********************************************************************************************************************
* CYT4BB Opensourec Library ���� CYT4BB ��Դ�⣩��һ�����ڹٷ� SDK �ӿڵĵ�������Դ��
* Copyright (c) 2022 SEEKFREE ��ɿƼ�
*
* ���ļ��� CYT4BB ��Դ���һ����
*
* CYT4BB ��Դ�� ��������
* �����Ը��������������ᷢ���� GPL��GNU General Public License���� GNUͨ�ù������֤��������
* �� GPL �ĵ�3�棨�� GPL3.0������ѡ��ģ��κκ����İ汾�����·�����/���޸���
*
* ����Դ��ķ�����ϣ�����ܷ������ã�����δ�������κεı�֤
* ����û�������������Ի��ʺ��ض���;�ı�֤
* ����ϸ����μ� GPL
*
* ��Ӧ�����յ�����Դ���ͬʱ�յ�һ�� GPL �ĸ���
* ���û�У������<https://www.gnu.org/licenses/>
*
* ����ע����
* ����Դ��ʹ�� GPL3.0 ��Դ���֤Э�� �����������Ϊ���İ汾
* �������Ӣ�İ��� libraries/doc �ļ����µ� GPL3_permission_statement.txt �ļ���
* ���֤������ libraries �ļ����� �����ļ����µ� LICENSE �ļ�
* ��ӭ��λʹ�ò����������� ���޸�����ʱ���뱣����ɿƼ��İ�Ȩ����������������
*
* �ļ�����          zf_driver_uart
* ��˾����          �ɶ���ɿƼ����޹�˾
* �汾��Ϣ          �鿴 libraries/doc �ļ����� version �ļ� �汾˵��
* ��������          IAR 9.40.1
* ����ƽ̨          CYT4BB
* ��������          https://seekfree.taobao.com/
*
* �޸ļ�¼
* ����              ����                ��ע
* 2024-1-5       pudding            first version
********************************************************************************************************************/

#ifndef _zf_driver_uart_h_
#define _zf_driver_uart_h_

#include "scb/cy_scb_uart.h"
#include "zf_common_typedef.h"

typedef enum                   // ö�ٴ��ڷ������� ��ö�ٶ��岻�����û��޸�
{
    UART0_TX_P00_1,             // ����0 ��������
 
    UART1_TX_P04_1,             // ����1 ��������

    UART2_TX_P10_1,             // ����2 �������ſ�ѡ��Χ

    UART3_TX_P17_2,             // ����3 �������ſ�ѡ��Χ

    UART4_TX_P14_1,             // ����4 �������ſ�ѡ��Χ
}uart_tx_pin_enum;


typedef enum                   // ö�ٴ��ڽ������� ��ö�ٶ��岻�����û��޸�
{

    UART0_RX_P00_0,             // ����0 ��������
 
    UART1_RX_P04_0,             // ����1 ��������
  
    UART2_RX_P10_0,             // ����2 �������ſ�ѡ��Χ

    UART3_RX_P17_1,             // ����3 �������ſ�ѡ��Χ
	
    UART4_RX_P14_0,             // ����4 �������ſ�ѡ��Χ
}uart_rx_pin_enum;


typedef enum                   // ö�ٴ��ں� ��ö�ٶ��岻�����û��޸�
{
    UART_0,
    UART_1,
    UART_2,
    UART_3,
    UART_4,
}uart_index_enum;

volatile stc_SCB_t* get_scb_module(uart_index_enum uart_n);

//====================================================���� ��������====================================================
void    uart_write_byte                     (uart_index_enum uartn, const uint8 dat);
void    uart_write_buffer                   (uart_index_enum uartn, const uint8 *buff, uint32 len);
void    uart_write_string                   (uart_index_enum uartn, const char *str);

uint8   uart_read_byte                      (uart_index_enum uartn);
uint8   uart_query_byte                     (uart_index_enum uartn, uint8 *dat);

void    uart_tx_interrupt                   (uart_index_enum uartn, uint32 status);
void    uart_rx_interrupt                   (uart_index_enum uartn, uint32 status);

void    uart_sbus_init                      (uart_index_enum uartn, uint32 baud, uart_tx_pin_enum tx_pin, uart_rx_pin_enum rx_pin);
void    uart_init                           (uart_index_enum uartn, uint32 baud, uart_tx_pin_enum tx_pin, uart_rx_pin_enum rx_pin);
//====================================================���� ��������====================================================

#endif
