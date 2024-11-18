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
* �ļ�����          zf_device_uart_receiver
* ��˾����          �ɶ���ɿƼ����޹�˾
* �汾��Ϣ          �鿴 libraries/doc �ļ����� version �ļ� �汾˵��
* ��������          IAR 9.40.1
* ����ƽ̨          CYT4BB
* ��������          https://seekfree.taobao.com/
*
* �޸ļ�¼
* ����              ����                ��ע
* 2024-01-24       pudding           first version
********************************************************************************************************************/
/*********************************************************************************************************************
* ���߶��壺
*                   ------------------------------------
*                   ģ��ܽ�            ��Ƭ���ܽ�
*                   TXD               �鿴 zf_device_uart_receiver.h �� UART_RECEVIER_RX_PIN �궨��
*                   GND               ��Դ��
*                   5V                5V��Դ
*                   ------------------------------------
********************************************************************************************************************/
#ifndef _zf_device_uart_receiver_h
#define _zf_device_uart_receiver_h

#include "zf_common_typedef.h"

#define UART_RECEVIER_UART_INDEX            UART_4              // ���崮�ڽ��ջ�ʹ�õĴ���
#define UART_RECEVIER_TX_PIN                UART4_TX_P14_1      // ң�������ջ�û��������ţ������ڴ��ڳ�ʼ��ʱռλʹ��
#define UART_RECEVIER_RX_PIN                UART4_RX_P14_0      // ���ڽ��ջ���TX���� ���ӵ�Ƭ����RX����
#define UART_RECEVIER_COUNTER               (TC_TIME2_CH2)      // ����ң�������ܼ��������ͨ��
#define SBUS_UART_BAUDRATE                  (100000)            // ָ�� SBUS ������ʹ�õĵĴ��ڲ����� (�û������޸�)
#define UART_RECEVIER_CHANNEL_NUM           ( 6 )               // ����ң����ͨ������ (�û������޸�)

#define REV_DATA_LEN                        ( 25   )            // SBUS֡��
#define FRAME_STAR                          ( 0X0F )            // ֡ͷ��Ϣ
#define FRAME_END                           ( 0X00 )            // ֡β��Ϣ
#define SBUS_NORMAL_STATE                   ( 0X03 )            // ����״̬
#define SBUS_ABNORMAL_STATE                 ( 0X0F )            // ʧ��״̬

typedef struct
{
    uint16 channel[UART_RECEVIER_CHANNEL_NUM];                  // CH1-CH6ͨ������
    uint8  state;                                               // ң����״̬(1��ʾ�����������ʾʧ��)
    uint8  finsh_flag;                                          // 1����ʾ�ɹ����յ�һ֡ң��������
}uart_receiver_struct;

extern uart_receiver_struct uart_receiver;                      // ���ڽ��ջ�ͨ��������״̬

void uart_receiver_init(void);

#endif
