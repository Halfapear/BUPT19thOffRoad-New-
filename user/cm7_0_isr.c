/*********************************************************************************************************************
* CYT4BB Opensource Library ���� CYT4BB ��Դ�⣩��һ�����ڹٷ� SDK �ӿڵĵ�������Դ��
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
* �ļ�����          isr_arm_7_0
* ��˾����          �ɶ���ɿƼ����޹�˾
* �汾��Ϣ          �鿴 libraries/doc �ļ����� version �ļ� �汾˵��
* ��������          IAR 9.40.1
* ����ƽ̨          CYT4BB
* ��������          https://seekfree.taobao.com/
*
* �޸ļ�¼
* ����              ����                ��ע
* 2024-1-9          pudding             first version
********************************************************************************************************************/

#include "zf_common_headfile.h"

// **************************** PIT�жϺ��� ****************************
void pit0_ch0_isr()
{
    pit_isr_flag_clear(PIT_CH0);

    if(gyro_Offset_flag == 1 && GL_IMU_Flag == 1)
    {
        imu660ra_get_gyro();                                                        // ��ȡ IMU660RA �Ľ��ٶȲ�����ֵ
        IMU_YAW_integral();           //���ֳ��Ƕ�ֵ
    }

#if IMU_Restrain_FLAG
    Control_FLAG = 0; //����GPS�Զ����Ӱ��
    Steer_set(SERVO_MOTOR_MID + PidLocCtrl(&PID_IMU, 0 - Daty_Z)); //���
#endif
}

void pit0_ch1_isr()
{
    pit_isr_flag_clear(PIT_CH1);

    LORA_work();                                    //LORA
    x6f_scan();                                     //Controlɨ����ջ�����ͨ��
}

void pit0_ch2_isr()
{
    pit_isr_flag_clear(PIT_CH2);

    if(gnss_flag)
    {
        gnss_flag = 0;
//        gps_data_parse();           //��ʼ��������
    }
}

// ����ô�ǵ�û��ch3����
/*
void pit0_ch3_isr()
{
    pit_isr_flag_clear(PIT_CH3);

    key_scan();

    //ת���п�ģʽ��Control_FLAG==0������
    if(Control_FLAG == 1)
    {
        if(next_point < 1)
        {
            Steer_set(SERVO_MOTOR_MID + PidLocCtrl(&PID_IMU, 0 - Error)); //���
        }
        else if((next_point >= 1 && next_point <= 7) || (next_point > 7 && next_point <= 10) || next_point > 10)
        {
            Steer_set(PidLocCtrl(&PID_GPS, SERVO_MOTOR_MID + Error)); //SD9
        }
    }
    HALL_gather(); //������������ʱ��ȡ
    BLDC_Cloop_ctrl(SPEED_Value);
}
*/
// **************************** PIT�жϺ��� ****************************

// **************************** �ⲿ�жϺ��� ****************************
void gpio_0_exti_isr()
{
    if(exti_flag_get(P01_0))
    {
        exti_flag_clear(P01_0);
        wireless_module_uart_handler();                 // ����ģ��ͳһ�ص�����
    }
    if(exti_flag_get(P01_1))
    {
        exti_flag_clear(P01_1);
    }
}

void gpio_1_exti_isr()
{
    if(exti_flag_get(P02_0))
    {
        exti_flag_clear(P02_0);
    }
    if(exti_flag_get(P02_4))
    {
        exti_flag_clear(P02_4);
    }
}

void gpio_2_exti_isr()
{
    if(exti_flag_get(P03_0))
    {
        exti_flag_clear(P03_0);
        camera_vsync_handler(); // ����ͷ�����ɼ�ͳһ�ص�����
    }
    if(exti_flag_get(P03_1))
    {
        exti_flag_clear(P03_1);
    }
}

void gpio_3_exti_isr()
{
    if(exti_flag_get(P04_0))
    {
        exti_flag_clear(P04_0);
    }
    if(exti_flag_get(P04_1))
    {
        exti_flag_clear(P04_1);
    }
}
// **************************** �ⲿ�жϺ��� ****************************

// **************************** �����жϺ��� ****************************
void uart0_isr(void)
{
    if(Cy_SCB_GetRxInterruptMask(get_scb_module(UART_0)) & CY_SCB_UART_RX_NOT_EMPTY) // ����0�����ж�
    {
        Cy_SCB_ClearRxInterrupt(get_scb_module(UART_0), CY_SCB_UART_RX_NOT_EMPTY); // ��������жϱ�־λ

#if DEBUG_UART_USE_INTERRUPT // ������� debug �����ж�
        debug_interrupr_handler(); // ���� debug ���ڽ��մ����� ���ݻᱻ debug ���λ�������ȡ
#endif // ����޸��� DEBUG_UART_INDEX ����δ�����Ҫ�ŵ���Ӧ�Ĵ����ж�ȥ

//        gps_uart_callback();
    }
    else if(Cy_SCB_GetTxInterruptMask(get_scb_module(UART_0)) & CY_SCB_UART_TX_DONE) // ����0�����ж�
    {
        Cy_SCB_ClearTxInterrupt(get_scb_module(UART_0), CY_SCB_UART_TX_DONE); // ��������жϱ�־λ
    }
}

void uart1_isr(void)
{
    if(Cy_SCB_GetRxInterruptMask(get_scb_module(UART_1)) & CY_SCB_UART_RX_NOT_EMPTY) // ����1�����ж�
    {
        Cy_SCB_ClearRxInterrupt(get_scb_module(UART_1), CY_SCB_UART_RX_NOT_EMPTY); // ��������жϱ�־λ
        camera_uart_handler(); // ����ͷ��������ͳһ�ص�����
    }
    else if(Cy_SCB_GetTxInterruptMask(get_scb_module(UART_1)) & CY_SCB_UART_TX_DONE) // ����1�����ж�
    {
        Cy_SCB_ClearTxInterrupt(get_scb_module(UART_1), CY_SCB_UART_TX_DONE); // ��������жϱ�־λ
    }
}

void uart2_isr(void)
{
    if(Cy_SCB_GetRxInterruptMask(get_scb_module(UART_2)) & CY_SCB_UART_RX_NOT_EMPTY) // ����2�����ж�
    {
        Cy_SCB_ClearRxInterrupt(get_scb_module(UART_2), CY_SCB_UART_RX_NOT_EMPTY); // ��������жϱ�־λ
        wireless_module_uart_handler(); // ����ģ��ͳһ�ص�����
    }
    else if(Cy_SCB_GetTxInterruptMask(get_scb_module(UART_2)) & CY_SCB_UART_TX_DONE) // ����2�����ж�
    {
        Cy_SCB_ClearTxInterrupt(get_scb_module(UART_2), CY_SCB_UART_TX_DONE); // ��������жϱ�־λ
    }
}

void uart3_isr(void)
{
    if(Cy_SCB_GetRxInterruptMask(get_scb_module(UART_3)) & CY_SCB_UART_RX_NOT_EMPTY) // ����3�����ж�
    {
        Cy_SCB_ClearRxInterrupt(get_scb_module(UART_3), CY_SCB_UART_RX_NOT_EMPTY); // ��������жϱ�־λ
    }
    else if(Cy_SCB_GetTxInterruptMask(get_scb_module(UART_3)) & CY_SCB_UART_TX_DONE) // ����3�����ж�
    {
        Cy_SCB_ClearTxInterrupt(get_scb_module(UART_3), CY_SCB_UART_TX_DONE); // ��������жϱ�־λ
    }
}

void uart4_isr(void)
{
    if(Cy_SCB_GetRxInterruptMask(get_scb_module(UART_4)) & CY_SCB_UART_RX_NOT_EMPTY) // ����4�����ж�
    {
        Cy_SCB_ClearRxInterrupt(get_scb_module(UART_4), CY_SCB_UART_RX_NOT_EMPTY); // ��������жϱ�־λ
        uart_receiver_handler(); // ���ڽ��ջ��ص�����
    }
    else if(Cy_SCB_GetTxInterruptMask(get_scb_module(UART_4)) & CY_SCB_UART_TX_DONE) // ����4�����ж�
    {
        Cy_SCB_ClearTxInterrupt(get_scb_module(UART_4), CY_SCB_UART_TX_DONE); // ��������жϱ�־λ
    }
}
// **************************** �����жϺ��� ****************************
