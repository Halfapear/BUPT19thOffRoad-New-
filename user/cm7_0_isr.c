/*********************************************************************************************************************
* CYT4BB Opensourec Library 即（ CYT4BB 开源库）是一个基于官方 SDK 接口的第三方开源库
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
* 文件名称          cm7_0_isr
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          IAR 9.40.1
* 适用平台          CYT4BB
* 店铺链接          https://seekfree.taobao.com/
*
* 修改记录
* 日期              作者                备注
* 2024-1-9      pudding            first version
* 2024-5-14     pudding            新增12个pit周期中断 增加部分注释说明
********************************************************************************************************************/

#include "zf_common_headfile.h"


// **************************** PIT中断函数 ****************************
void pit0_ch0_isr()                     // 定时器通道 0 周期中断服务函数      
{
    pit_isr_flag_clear(PIT_CH0);//5ms
    //BLDC_Cloop_ctrl(SPEED_Value);
    
    if(gyro_Offset_flag == 1 && GL_IMU_Flag == 1)
    {
        imu963ra_get_gyro();                                                        // 获取 IMU660RA 的角速度测量数值
        IMU_YAW_integral();           //积分出角度值
    }
    
    
#if IMU_Restrain_FLAG
    Control_FLAG = 0; //隔绝GPS对舵机的影响
    Steer_set(SERVO_MOTOR_MID + PidLocCtrl(&PID_IMU, 0 - Daty_Z)); //舵机
#endif

    
}

void pit0_ch1_isr()                     // 定时器通道 1 周期中断服务函数      
{
    pit_isr_flag_clear(PIT_CH1);//10us
    
    //LORA_work();                                    //LORA
   ten_us++;
    if(ten_us==10)//100us采集一次硅麦数据
    {
        mic_data_get();
        Get_xf6_adc();
        ten_us=0;
    }
    //x6f_scan();                                     //Control扫描接收机各个通道
    
}

void pit0_ch2_isr()                     // 定时器通道 2 周期中断服务函数      
{
    pit_isr_flag_clear(PIT_CH2);
    my_tms++;
    if(gnss_flag)
    {
        gnss_flag = 0;
        gnss_data_parse();          //开始解析数据
    }
    if(my_tms>=10)
    {
        my_tms=0;
        my_s++;
        if(MIC_rcyflag)
        {
            start_mict++;
        }
        else
        {
            start_mict=0;
        }
        if(restart_mic>0)
            restart_mic--;
    }
    
}

void pit0_ch10_isr()                    // 定时器通道 10 周期中断服务函数      
{
    pit_isr_flag_clear(PIT_CH10);
    
    //舵机控制
    key_scan();
    
    if(Control_FLAG==1)
    {
        //第一个点的pid用的IMUPID-因为第一个还没获取到方位角？
        /*if(next_point<1)
        {
         Steer_set(SERVO_MOTOR_MID+PidLocCtrl(&PID_IMU,0-Error));//舵机
        }
        else if((next_point>=1&&next_point<=7)||(next_point>7&&next_point<=10)||next_point>10)
        {
        Steer_set(PidLocCtrl(&PID_GPS,SERVO_MOTOR_MID+Error));//SD9
    //  Steer_set(PidLocCtrl(&PID_GPS,SERVO_MOTOR_MID-Error));//SD5
        }*/
//            if(next_point<=1||next_point==4)
//            {
//             Steer_set(SERVO_MOTOR_MID+PidLocCtrl(&PID_IMU,0-Error));//舵机
//            }
//            else if((next_point>1&&next_point<4)||next_point>4)
//            {
//            Steer_set(PidLocCtrl(&PID_GPS,SERVO_MOTOR_MID+Error));//SD9
//        //  Steer_set(PidLocCtrl(&PID_GPS,SERVO_MOTOR_MID-Error));//SD5
//            }
      if(Start_GPSangle_Flag==0)
        {
         Steer_set(SERVO_MOTOR_MID+PidLocCtrl(&PID_IMU,0-Error));//舵机
        }
      else if(MIC_rcyflag){
          Steer_set(PidLocCtrl(&PID_IMU,Error));
      }
      else
      {
          Steer_set(PidLocCtrl(&PID_GPS,SERVO_MOTOR_MID+Error));//看完error的值再决定怎么改
      }
      

    }
    
    HALL_gather();//霍尔编码器定时读取
    if(!xf6_stopflag){
        BLDC_Cloop_ctrl(SPEED_Value);
    }
    else
    {
        BLDC_Cloop_ctrl(0);
        printf("\n\r停车");
    }
     //BLDC_Cloop_ctrl(SPEED);    
    
    
    
}

void pit0_ch11_isr()                    // 定时器通道 11 周期中断服务函数      
{
    pit_isr_flag_clear(PIT_CH11);
    
}

void pit0_ch12_isr()                    // 定时器通道 12 周期中断服务函数      
{
    pit_isr_flag_clear(PIT_CH12);
    
}

void pit0_ch13_isr()                    // 定时器通道 13 周期中断服务函数      
{
    pit_isr_flag_clear(PIT_CH13);
    
}

void pit0_ch14_isr()                    // 定时器通道 14 周期中断服务函数      
{
    pit_isr_flag_clear(PIT_CH14);
    
}

void pit0_ch15_isr()                    // 定时器通道 15 周期中断服务函数      
{
    pit_isr_flag_clear(PIT_CH15);
    
}

void pit0_ch16_isr()                    // 定时器通道 16 周期中断服务函数      
{
    pit_isr_flag_clear(PIT_CH16);
    
}

void pit0_ch17_isr()                    // 定时器通道 17 周期中断服务函数      
{
    pit_isr_flag_clear(PIT_CH17);
    
}

void pit0_ch18_isr()                    // 定时器通道 18 周期中断服务函数      
{
    pit_isr_flag_clear(PIT_CH18);
    
}

void pit0_ch19_isr()                    // 定时器通道 19 周期中断服务函数      
{
    pit_isr_flag_clear(PIT_CH19);
    
}

void pit0_ch20_isr()                    // 定时器通道 20 周期中断服务函数      
{
    pit_isr_flag_clear(PIT_CH20);
    
}

void pit0_ch21_isr()                    // 定时器通道 21 周期中断服务函数      
{
    pit_isr_flag_clear(PIT_CH21);
    
}
// **************************** PIT中断函数 ****************************


// **************************** 外部中断函数 ****************************
void gpio_0_exti_isr()                  // 外部 GPIO_0 中断服务函数     
{
    
  
  
}

void gpio_1_exti_isr()                  // 外部 GPIO_1 中断服务函数     
{
    if(exti_flag_get(P01_0))		// 示例P1_0端口外部中断判断
    {

      
      
            
    }
    if(exti_flag_get(P01_1))
    {

            
            
    }
}

void gpio_2_exti_isr()                  // 外部 GPIO_2 中断服务函数     
{
    if(exti_flag_get(P02_0))
    {
            
            
    }
    if(exti_flag_get(P02_4))
    {
            
            
    }

}

void gpio_3_exti_isr()                  // 外部 GPIO_3 中断服务函数     
{



}

void gpio_4_exti_isr()                  // 外部 GPIO_4 中断服务函数     
{



}

void gpio_5_exti_isr()                  // 外部 GPIO_5 中断服务函数     
{



}

void gpio_6_exti_isr()                  // 外部 GPIO_6 中断服务函数     
{
	


}

void gpio_7_exti_isr()                  // 外部 GPIO_7 中断服务函数     
{



}

void gpio_8_exti_isr()                  // 外部 GPIO_8 中断服务函数     
{



}

void gpio_9_exti_isr()                  // 外部 GPIO_9 中断服务函数     
{



}

void gpio_10_exti_isr()                  // 外部 GPIO_10 中断服务函数     
{



}

void gpio_11_exti_isr()                  // 外部 GPIO_11 中断服务函数     
{



}

void gpio_12_exti_isr()                  // 外部 GPIO_12 中断服务函数     
{



}

void gpio_13_exti_isr()                  // 外部 GPIO_13 中断服务函数     
{



}

void gpio_14_exti_isr()                  // 外部 GPIO_14 中断服务函数     
{



}

void gpio_15_exti_isr()                  // 外部 GPIO_15 中断服务函数     
{



}

void gpio_16_exti_isr()                  // 外部 GPIO_16 中断服务函数     
{



}

void gpio_17_exti_isr()                  // 外部 GPIO_17 中断服务函数     
{



}

void gpio_18_exti_isr()                  // 外部 GPIO_18 中断服务函数     
{



}

void gpio_19_exti_isr()                  // 外部 GPIO_19 中断服务函数     
{



}

void gpio_20_exti_isr()                  // 外部 GPIO_20 中断服务函数     
{



}

void gpio_21_exti_isr()                  // 外部 GPIO_21 中断服务函数     
{



}

void gpio_22_exti_isr()                  // 外部 GPIO_22 中断服务函数     
{



}

void gpio_23_exti_isr()                  // 外部 GPIO_23 中断服务函数     
{



}
// **************************** 外部中断函数 ****************************

//// **************************** DMA中断函数 ****************************
//void dma_event_callback(void* callback_arg, cyhal_dma_event_t event)
//{
//    CY_UNUSED_PARAMETER(event);
//	
//
//	
//	
//}
// **************************** DMA中断函数 ****************************

// **************************** 串口中断函数 ****************************
// 串口0默认作为调试串口
void uart0_isr (void)
{
    if(Cy_SCB_GetRxInterruptMask(get_scb_module(UART_0)) & CY_SCB_UART_RX_NOT_EMPTY)            // 串口0接收中断
    {
        Cy_SCB_ClearRxInterrupt(get_scb_module(UART_0), CY_SCB_UART_RX_NOT_EMPTY);              // 清除接收中断标志位
        
#if DEBUG_UART_USE_INTERRUPT                        				                // 如果开启 debug 串口中断
        debug_interrupr_handler();                  				                // 调用 debug 串口接收处理函数 数据会被 debug 环形缓冲区读取
#endif                                              				                // 如果修改了 DEBUG_UART_INDEX 那这段代码需要放到对应的串口中断去
      
        
        
    }
    else if(Cy_SCB_GetTxInterruptMask(get_scb_module(UART_0)) & CY_SCB_UART_TX_DONE)            // 串口0发送中断
    {           
        Cy_SCB_ClearTxInterrupt(get_scb_module(UART_0), CY_SCB_UART_TX_DONE);                   // 清除接收中断标志位
        
        
        
    }
}

void uart1_isr (void)
{
    if(Cy_SCB_GetRxInterruptMask(get_scb_module(UART_1)) & CY_SCB_UART_RX_NOT_EMPTY)            // 串口1接收中断
    {
        Cy_SCB_ClearRxInterrupt(get_scb_module(UART_1), CY_SCB_UART_RX_NOT_EMPTY);              // 清除接收中断标志位

        wireless_module_uart_handler();
        
        
    }
    else if(Cy_SCB_GetTxInterruptMask(get_scb_module(UART_1)) & CY_SCB_UART_TX_DONE)            // 串口1发送中断
    {
        Cy_SCB_ClearTxInterrupt(get_scb_module(UART_1), CY_SCB_UART_TX_DONE);                   // 清除接收中断标志位
        
        
        
    }
}

void uart2_isr (void)
{
    if(Cy_SCB_GetRxInterruptMask(get_scb_module(UART_2)) & CY_SCB_UART_RX_NOT_EMPTY)            // 串口2接收中断
    {
        Cy_SCB_ClearRxInterrupt(get_scb_module(UART_2), CY_SCB_UART_RX_NOT_EMPTY);              // 清除接收中断标志位

        gnss_uart_callback();
        
        
    }
    else if(Cy_SCB_GetTxInterruptMask(get_scb_module(UART_2)) & CY_SCB_UART_TX_DONE)            // 串口2发送中断
    {
        Cy_SCB_ClearTxInterrupt(get_scb_module(UART_2), CY_SCB_UART_TX_DONE);                   // 清除接收中断标志位
        
        
        
    }
}

void uart3_isr (void)
{
    if(Cy_SCB_GetRxInterruptMask(get_scb_module(UART_3)) & CY_SCB_UART_RX_NOT_EMPTY)            // 串口3接收中断
    {
        Cy_SCB_ClearRxInterrupt(get_scb_module(UART_3), CY_SCB_UART_RX_NOT_EMPTY);              // 清除接收中断标志位

        
        
        
    }
    else if(Cy_SCB_GetTxInterruptMask(get_scb_module(UART_3)) & CY_SCB_UART_TX_DONE)            // 串口3发送中断
    {
        Cy_SCB_ClearTxInterrupt(get_scb_module(UART_3), CY_SCB_UART_TX_DONE);                   // 清除接收中断标志位
        
        
        
    }
}

void uart4_isr (void)
{
    
    if(Cy_SCB_GetRxInterruptMask(get_scb_module(UART_4)) & CY_SCB_UART_RX_NOT_EMPTY)            // 串口4接收中断
    {
        Cy_SCB_ClearRxInterrupt(get_scb_module(UART_4), CY_SCB_UART_RX_NOT_EMPTY);              // 清除接收中断标志位

        
        uart_receiver_handler();                                                                // 串口接收机回调函数
        
        
    }
    else if(Cy_SCB_GetTxInterruptMask(get_scb_module(UART_4)) & CY_SCB_UART_TX_DONE)            // 串口4发送中断
    {
        Cy_SCB_ClearTxInterrupt(get_scb_module(UART_4), CY_SCB_UART_TX_DONE);                   // 清除接收中断标志位
        
        
        
    }
}
// **************************** 串口中断函数 ****************************