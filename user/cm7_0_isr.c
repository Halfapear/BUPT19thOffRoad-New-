/*********************************************************************************************************************
* CYT4BB Opensource Library 即（ CYT4BB 开源库）是一个基于官方 SDK 接口的第三方开源库
* Copyright (c) 2022 SEEKFREE 逐飞科技
*
* 本文件是 CYT4BB 开源库的一部分
*
* CYT4BB 开源库 是免费软件
* 您可以根据自由软件基金会发布的 GPL（GNU General Public License，即 GNU通用公共许可证）的条款
* 即 GPL 的第3版（即 GPL3.0）或（您选择的）任何后来的版本，重新发布和/或修改它
*
* 本开源库的发布是希望它能发挥作用，但并未对其作任何的保证
* 甚至没有隐含的适销性或适合特定用途的保证
* 更多细节请参见 GPL
*
* 您应该在收到本开源库的同时收到一份 GPL 的副本
* 如果没有，请参阅<https://www.gnu.org/licenses/>
*
* 额外注明：
* 本开源库使用 GPL3.0 开源许可证协议 以上许可申明为译文版本
* 许可申明英文版在 libraries/doc 文件夹下的 GPL3_permission_statement.txt 文件中
* 许可证副本在 libraries 文件夹下 即该文件夹下的 LICENSE 文件
* 欢迎各位使用并传播本程序 但修改内容时必须保留逐飞科技的版权声明（即本声明）
*
* 文件名称          isr_arm_7_0
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          IAR 9.40.1
* 适用平台          CYT4BB
* 店铺链接          https://seekfree.taobao.com/
*
* 修改记录
* 日期              作者                备注
* 2024-1-9          pudding             first version
********************************************************************************************************************/

#include "zf_common_headfile.h"

// **************************** PIT中断函数 ****************************
void pit0_ch0_isr()
{
    pit_isr_flag_clear(PIT_CH0);

    if(gyro_Offset_flag == 1 && GL_IMU_Flag == 1)
    {
        imu660ra_get_gyro();                                                        // 获取 IMU660RA 的角速度测量数值
        IMU_YAW_integral();           //积分出角度值
    }

#if IMU_Restrain_FLAG
    Control_FLAG = 0; //隔绝GPS对舵机的影响
    Steer_set(SERVO_MOTOR_MID + PidLocCtrl(&PID_IMU, 0 - Daty_Z)); //舵机
#endif
}

void pit0_ch1_isr()
{
    pit_isr_flag_clear(PIT_CH1);

    LORA_work();                                    //LORA
    x6f_scan();                                     //Control扫描接收机各个通道
}

void pit0_ch2_isr()
{
    pit_isr_flag_clear(PIT_CH2);

    if(gnss_flag)
    {
        gnss_flag = 0;
//        gps_data_parse();           //开始解析数据
    }
}

// 我怎么记得没有ch3。。
/*
void pit0_ch3_isr()
{
    pit_isr_flag_clear(PIT_CH3);

    key_scan();

    //转入有控模式后Control_FLAG==0，隔绝
    if(Control_FLAG == 1)
    {
        if(next_point < 1)
        {
            Steer_set(SERVO_MOTOR_MID + PidLocCtrl(&PID_IMU, 0 - Error)); //舵机
        }
        else if((next_point >= 1 && next_point <= 7) || (next_point > 7 && next_point <= 10) || next_point > 10)
        {
            Steer_set(PidLocCtrl(&PID_GPS, SERVO_MOTOR_MID + Error)); //SD9
        }
    }
    HALL_gather(); //霍尔编码器定时读取
    BLDC_Cloop_ctrl(SPEED_Value);
}
*/
// **************************** PIT中断函数 ****************************

// **************************** 外部中断函数 ****************************
void gpio_0_exti_isr()
{
    if(exti_flag_get(P01_0))
    {
        exti_flag_clear(P01_0);
        wireless_module_uart_handler();                 // 无线模块统一回调函数
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
        camera_vsync_handler(); // 摄像头触发采集统一回调函数
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
// **************************** 外部中断函数 ****************************

// **************************** 串口中断函数 ****************************
void uart0_isr(void)
{
    if(Cy_SCB_GetRxInterruptMask(get_scb_module(UART_0)) & CY_SCB_UART_RX_NOT_EMPTY) // 串口0接收中断
    {
        Cy_SCB_ClearRxInterrupt(get_scb_module(UART_0), CY_SCB_UART_RX_NOT_EMPTY); // 清除接收中断标志位

#if DEBUG_UART_USE_INTERRUPT // 如果开启 debug 串口中断
        debug_interrupr_handler(); // 调用 debug 串口接收处理函数 数据会被 debug 环形缓冲区读取
#endif // 如果修改了 DEBUG_UART_INDEX 那这段代码需要放到对应的串口中断去

//        gps_uart_callback();
    }
    else if(Cy_SCB_GetTxInterruptMask(get_scb_module(UART_0)) & CY_SCB_UART_TX_DONE) // 串口0发送中断
    {
        Cy_SCB_ClearTxInterrupt(get_scb_module(UART_0), CY_SCB_UART_TX_DONE); // 清除接收中断标志位
    }
}

void uart1_isr(void)
{
    if(Cy_SCB_GetRxInterruptMask(get_scb_module(UART_1)) & CY_SCB_UART_RX_NOT_EMPTY) // 串口1接收中断
    {
        Cy_SCB_ClearRxInterrupt(get_scb_module(UART_1), CY_SCB_UART_RX_NOT_EMPTY); // 清除接收中断标志位
        camera_uart_handler(); // 摄像头参数配置统一回调函数
    }
    else if(Cy_SCB_GetTxInterruptMask(get_scb_module(UART_1)) & CY_SCB_UART_TX_DONE) // 串口1发送中断
    {
        Cy_SCB_ClearTxInterrupt(get_scb_module(UART_1), CY_SCB_UART_TX_DONE); // 清除接收中断标志位
    }
}

void uart2_isr(void)
{
    if(Cy_SCB_GetRxInterruptMask(get_scb_module(UART_2)) & CY_SCB_UART_RX_NOT_EMPTY) // 串口2接收中断
    {
        Cy_SCB_ClearRxInterrupt(get_scb_module(UART_2), CY_SCB_UART_RX_NOT_EMPTY); // 清除接收中断标志位
        wireless_module_uart_handler(); // 无线模块统一回调函数
    }
    else if(Cy_SCB_GetTxInterruptMask(get_scb_module(UART_2)) & CY_SCB_UART_TX_DONE) // 串口2发送中断
    {
        Cy_SCB_ClearTxInterrupt(get_scb_module(UART_2), CY_SCB_UART_TX_DONE); // 清除接收中断标志位
    }
}

void uart3_isr(void)
{
    if(Cy_SCB_GetRxInterruptMask(get_scb_module(UART_3)) & CY_SCB_UART_RX_NOT_EMPTY) // 串口3接收中断
    {
        Cy_SCB_ClearRxInterrupt(get_scb_module(UART_3), CY_SCB_UART_RX_NOT_EMPTY); // 清除接收中断标志位
    }
    else if(Cy_SCB_GetTxInterruptMask(get_scb_module(UART_3)) & CY_SCB_UART_TX_DONE) // 串口3发送中断
    {
        Cy_SCB_ClearTxInterrupt(get_scb_module(UART_3), CY_SCB_UART_TX_DONE); // 清除接收中断标志位
    }
}

void uart4_isr(void)
{
    if(Cy_SCB_GetRxInterruptMask(get_scb_module(UART_4)) & CY_SCB_UART_RX_NOT_EMPTY) // 串口4接收中断
    {
        Cy_SCB_ClearRxInterrupt(get_scb_module(UART_4), CY_SCB_UART_RX_NOT_EMPTY); // 清除接收中断标志位
        uart_receiver_handler(); // 串口接收机回调函数
    }
    else if(Cy_SCB_GetTxInterruptMask(get_scb_module(UART_4)) & CY_SCB_UART_TX_DONE) // 串口4发送中断
    {
        Cy_SCB_ClearTxInterrupt(get_scb_module(UART_4), CY_SCB_UART_TX_DONE); // 清除接收中断标志位
    }
}
// **************************** 串口中断函数 ****************************
