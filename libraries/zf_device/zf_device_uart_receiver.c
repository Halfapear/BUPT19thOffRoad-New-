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

#include "zf_device_type.h"
#include "zf_driver_uart.h"
#include "zf_driver_timer.h"
#include "zf_device_uart_receiver.h"

uart_receiver_struct    uart_receiver ;             // ���ڽ��ջ�ͨ��������״̬

uint8   uart_receiver_data[REV_DATA_LEN]  = {0};    // ������ԭʼ����

//-------------------------------------------------------------------------------------------------------------------
//  �������     ��ȡ���ε���uart_receiver_interval_time����ʱ���� 
//  ����˵��     void
//  ���ز���     uint32            ���شӿ�ʼ�����ڵ�ʱ��(��λ1us)
//  ʹ��ʾ��     uint32 time = uart_receiver_interval_time();
//  ��ע��Ϣ     �ڲ�����
//-------------------------------------------------------------------------------------------------------------------
static uint32 uart_receiver_interval_time (void)
{
    uint32 interval_time;

    interval_time = timer_get(UART_RECEVIER_COUNTER);

    timer_clear(UART_RECEVIER_COUNTER);
    
    return interval_time;
}

//-------------------------------------------------------------------------------------------------------------------
// �������     SBUS���ݽ���
// ����˵��     *remote_data    ����ң�������ݵĵ�ַ
// ����˵��     *bufer          ԭʼ����
// ���ز���     void
// ʹ��ʾ��
// ��ע��Ϣ     ��sbus���ݽ������н���
//-------------------------------------------------------------------------------------------------------------------
static void uart_receiver_analysis (uart_receiver_struct *remote_data,uint8 * buffer)
{
    uint8 num = 0;
    remote_data->channel[num++] = (buffer[1] |buffer[ 2] << 8 ) & 0x07FF;
    remote_data->channel[num++] = (buffer[2] >> 3 | buffer[3] << 5 ) & 0x07FF;
    remote_data->channel[num++] = (buffer[3] >> 6 | buffer[4] << 2 | buffer[5] << 10 ) & 0x07FF;
    remote_data->channel[num++] = (buffer[5] >> 1 | buffer[6] << 7 ) & 0x07FF;
    remote_data->channel[num++] = (buffer[6] >> 4 | buffer[7] << 4 ) & 0x07FF;
    remote_data->channel[num++] = (buffer[7] >> 7 | buffer[8] << 1 | buffer[9] << 9 ) & 0x07FF;
    remote_data->state = (SBUS_NORMAL_STATE == buffer[23]) ? 1 : 0;
    uart_receiver.finsh_flag  = 1;
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ���ڽ��ջ�ģ�� �����жϻص�����
// ����˵��     void
// ���ز���     void
// ʹ��ʾ��     uart_receiver_callback();
// ��ע��Ϣ     �ú����� ISR �ļ� �����жϳ���ͨ��uart_receiver_uart_handler����ָ�뱻����
//-------------------------------------------------------------------------------------------------------------------
void uart_receiver_callback(void)
{
    static vuint8 length = 0;

    if (uart_receiver_interval_time() > 3000)
    {
        length = 0;
    }
    
    if(uart_query_byte(UART_RECEVIER_UART_INDEX, &uart_receiver_data[length]))
    {
        length ++;
        if((REV_DATA_LEN == length) && (FRAME_STAR == uart_receiver_data[0]) && (FRAME_END == uart_receiver_data[24]))
        {
            uart_receiver_analysis(&uart_receiver, uart_receiver_data);
        }
    }

}

//-------------------------------------------------------------------------------------------------------------------
// �������     ���ڽ��ջ���ʼ��
// ����˵��     void
// ���ز���     void
// ʹ��ʾ��     uart_receiver_init();
//-------------------------------------------------------------------------------------------------------------------
void uart_receiver_init(void)
{
    uart_sbus_init(UART_RECEVIER_UART_INDEX, SBUS_UART_BAUDRATE, UART_RECEVIER_TX_PIN, UART_RECEVIER_RX_PIN);

    timer_init(UART_RECEVIER_COUNTER, TIMER_US);
    
    // ���ô����жϻص�����
    set_wireless_type(RECEIVER_UART, uart_receiver_callback);
    
    uart_rx_interrupt(UART_RECEVIER_UART_INDEX, 1);
    
    timer_start(UART_RECEVIER_COUNTER);
}
