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
* 文件名称          main_cm7_0
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          IAR 9.40.1
* 适用平台          CYT4BB
* 店铺链接          https://seekfree.taobao.com/
*
* 修改记录
* 日期              作者                备注
* 2024-1-4       pudding            first version
********************************************************************************************************************/

#include "zf_common_headfile.h"

// 打开新的工程或者工程移动了位置务必执行以下操作
// 第一步 关闭上面所有打开的文件
// 第二步 project->clean  等待下方进度条走完

// 本例程是开源库空工程 可用作移植或者测试各类内外设
// 本例程是开源库空工程 可用作移植或者测试各类内外设
// 本例程是开源库空工程 可用作移植或者测试各类内外设

// **************************** 代码区域 ****************************
/*
int8 duty = 0;
bool dir = true;
*/
       
{
    clock_init(SYSTEM_CLOCK_250M); 	// 时钟配置及系统初始化<务必保留>
    debug_info_init();                  // 调试串口信息初始化
    
    // 此处编写用户代码 例如外设初始化代码等
    
    //初始化代码
    BLDC_init();
    //ALL_Init();
    mic_init();
#if ISR_Start_FLAG

//CCU60_CH1 是用于配置和初始化定时器中断的标识符。具体来说，它代表了一种定时器模块的通道，用于定时和产生中断。CCU（Capture/Compare Unit）是一种在嵌入式系统中常见的模块，通常用于捕获输入事件、生成比较事件或用于脉宽调制（PWM）     
     
//CCU60_CH1 CCU61_CH0 CCU61_CH1的换成
     //还有一个陀螺仪的中断在IMU初始化的时刻开启
     pit_us_init(PIT_CH1, 10);                               // LORA中断+ADC采集
     pit_ms_init(PIT_CH2, 100);                            // GPS数据解析中断/霍尔编码器中断
     pit_ms_init(PIT_CH10, 7);                              // 舵机
     

#endif

//     Ctrl_GO_FLAG=0; //清除标志位
     Buzzer_check(0,100);                                  // 自检完成
    // 初始化代码
    //float test_angle;
     /* 
    generateChirp(Chrip_signal,FFT_SIZE,250,2000,FS);//生成标准Chrip信号
    seekfree_assistant_interface_init(SEEKFREE_ASSISTANT_DEBUG_UART);           // 初始化逐飞助手组件 选择debug串口输出信息
    
    seekfree_assistant_oscilloscope_data.channel_num  = 4;                      // 配置通道长度为1组
    */
    
    // 此处编写用户代码 例如外设初始化代码等
    while(true)
    {
      
      printf("enter");
      /* 
        if(duty >= 0)                                                           // 正转
        {
            gpio_set_level(DIR_CH2, GPIO_HIGH);                                  // DIR输出高电平
            pwm_set_duty(PWM_CH2, duty);                  // 计算占空比
        }
        else                                                                    // 反转
        {
            gpio_set_level(DIR_CH2, GPIO_LOW);                                   // DIR输出低电平
            pwm_set_duty(PWM_CH2, (-duty));               // 计算占空比
        }
        if(dir)                                                                 // 根据方向判断计数方向 本例程仅作参考
        {
            duty ++;                                                            // 正向计数
            if(duty >= MAX_DUTY)                                                // 达到最大值
                dir = false;                                                    // 变更计数方向
        }
        else
        {
            duty --;                                                            // 反向计数
            if(duty <= -MAX_DUTY)                                               // 达到最小值
                dir = true;                                                     // 变更计数方向
        }
        system_delay_ms(500);
      */
    
      BLDC_Cloop_ctrl(5000);
      //BLDC_ctrl(10000);
        // 此处编写需要循环执行的代码
       /* 
          if(MIC_Start_FLAG){
              mic_data_copy();//把一组硅麦数据取出
              //printf("\r\ntest");
              //test_angle= Get_mic_Angle();
             // printf("\r\n角度test_angle:%f",test_angle )
              for(int i = 0; i < FFT_SIZE; i++) 
              {
                  seekfree_assistant_oscilloscope_data.data[0] = fft_signal[0][i*2];//fft_outputSignal[0][i];     // 获取逆FFT运算后的幅度信息
                  seekfree_assistant_oscilloscope_data.data[1] = fft_signal[1][i*2];//fft_signal[1][i * 2];      // 获取逆FFT运算后的幅度信息
                  seekfree_assistant_oscilloscope_data.data[2] = fft_signal[2][i*2];
                  seekfree_assistant_oscilloscope_data.data[3] = fft_signal[3][i*2];
                  seekfree_assistant_oscilloscope_send(&seekfree_assistant_oscilloscope_data);     // 输出幅度信息到示波器                                     
              } 
          }
          */
        // Menu();
        #if Main_FLAG
      /* 
                         if(x6f_out[0]<135) {Ctrl_GO_FLAG=1;}//控制发车标志位

                         if(GO_FLAG==1)
                           {

                             if(Ctrl_GO_FLAG==1&&Control_FLAG==1) {Follow_Track();}//核心循迹程序

                          
                             if(x6f_out[2]>165&&x6f_out[2]<175)//有控模式+
                             {
                                  Control_FLAG=0;
                                  Control_Program();//控制程序
                             }
                           }
      */
                          Follow_Track();
                          //fft_outputSignal[0][0]=-100000000;
                        /*for(int i = 0; i < FFT_SIZE; i++) 
                          {
                              seekfree_assistant_oscilloscope_data.data[0] =  out_signal[0][i];//fft_outputSignal[0][i];     // 获取逆FFT运算后的幅度信息
                              seekfree_assistant_oscilloscope_data.data[1] = out_signal[1][i];//fft_signal[1][i * 2];      // 获取逆FFT运算后的幅度信息
                             seekfree_assistant_oscilloscope_data.data[2] = fft_outputSignal[0][i];
                             seekfree_assistant_oscilloscope_data.data[3] = fft_outputSignal[1][i];
                              seekfree_assistant_oscilloscope_send(&seekfree_assistant_oscilloscope_data);     // 输出幅度信息到示波器                                     
                          } */
                          
                         if(STOP_MENU_FLAG){Menu();}//菜单
        #endif

                         GL_CRC();

                         //if(GL_IMU_Flag==1) {Follow_Track();}//核心循迹程序
                         
                         /*if(x6f_out[2]>165&&x6f_out[2]<175)//有控模式
                         {
                              Control_FLAG=0;
                              GL_IMU_Flag=0;
                              Control_Program();//控制程序
                         }*/

        


      
      
      
        // 此处编写需要循环执行的代码
    }
}

// **************************** 代码区域 ****************************
