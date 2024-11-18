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

int main(void)
{
    clock_init(SYSTEM_CLOCK_250M); 	// 时钟配置及系统初始化<务必保留>
    debug_info_init();                  // 调试串口信息初始化
    
    // 此处编写用户代码 例如外设初始化代码等

    //初始化代码
    
    ALL_Init();
    mic_init();
    adc_init(XF6_ADC,ADC_12BIT);
    

//CCU60_CH1 是用于配置和初始化定时器中断的标识符。具体来说，它代表了一种定时器模块的通道，用于定时和产生中断。CCU（Capture/Compare Unit）是一种在嵌入式系统中常见的模块，通常用于捕获输入事件、生成比较事件或用于脉宽调制（PWM）     
     
//CCU60_CH1 CCU61_CH0 CCU61_CH1的换成
     //还有一个陀螺仪的中断在IMU初始化的时刻开启
    
    pit_us_init(PIT_CH1, 10);                               // LORA中断+ADC采集
    pit_ms_init(PIT_CH2, 100);                            // GPS数据解析中断
    pit_ms_init(PIT_CH10, 7);                              // 舵机/霍尔编码器/电机
    
//     Ctrl_GO_FLAG=0; //清除标志位
    Buzzer_check(0,100);                                  // 自检完成
    // 初始化代码
    //float test_angle;
    //generateChirp(Chrip_signal,FFT_SIZE,250,2000,FS);//生成标准Chrip信号
    seekfree_assistant_interface_init(SEEKFREE_ASSISTANT_DEBUG_UART);           // 初始化逐飞助手组件 选择debug串口输出信息
    
    seekfree_assistant_oscilloscope_data.channel_num  = 4;                      // 配置通道长度为1组
    
    // 此处编写用户代码 例如外设初始化代码等
    while(true)
    {
        // 此处编写需要循环执行的代码
        //Menu();       
      //BLDC_ctrl(3000);
        
        if(MIC_Start_FLAG){
              mic_data_copy();//把一组硅麦数据取出
         }
        
            //test_data_copy();
              //printf("\r\ntest");
              //test_angle= Get_mic_Angle();
             // printf("\r\n角度test_angle:%f",test_angle )
            /* for(int i = 0; i < FFT_SIZE; i++) 
              {
                  seekfree_assistant_oscilloscope_data.data[0] = fft_signal_1[i*2];//fft_outputSignal[0][i];     // 获取逆FFT运算后的幅度信息
                  seekfree_assistant_oscilloscope_data.data[1] = fft_signal_2[i*2];//fft_signal[1][i * 2];      // 获取逆FFT运算后的幅度信息
                  seekfree_assistant_oscilloscope_data.data[2] = fft_signal_3[i*2];
                  seekfree_assistant_oscilloscope_data.data[3] = fft_signal_4[i*2];
                 seekfree_assistant_oscilloscope_send(&seekfree_assistant_oscilloscope_data);     // 输出幅度信息到示波器                                     
              }*/
      //  } 

         // }        
        //printf("/r/nDaty_Z%dT_M,%d,%d,Error%d,azimuth%d",(int)Daty_Z,(int)T_M,(int)T_N,Error,azimuth);
      
        #if  XF6_Control_Flag//遥控停车
            xf6_data_copy();
            if(calcute_xf6_high()>105)
                xf6_stopflag=1;
           /* else
                xf6_stopflag=0;*/
            
            //ips200_show_uint(16,16*5,num,5);
        #endif
            
        #if Main_FLAG
    
                         //if(x6f_out[0]<135) {Ctrl_GO_FLAG=1;}//控制发车标志位

                         if(GO_FLAG==1)
                          {
                                printf("\r\n开始发车！！！");
                                
                              Follow_Track();//核心循迹程序                     
                          }

                          //Follow_Track();
                        /*for(int i = 0; i < FFT_SIZE; i++) 
                          {
                              seekfree_assistant_oscilloscope_data.data[0] =  out_signal_0[i];//fft_outputSignal[0][i];     // 获取逆FFT运算后的幅度信息
                              seekfree_assistant_oscilloscope_data.data[1] = out_signal_1[i];//fft_signal[1][i * 2];      // 获取逆FFT运算后的幅度信息
                             seekfree_assistant_oscilloscope_data.data[2] = fft_outputSignal[0][i];
                             seekfree_assistant_oscilloscope_data.data[3] = fft_outputSignal[1][i];
                              seekfree_assistant_oscilloscope_send(&seekfree_assistant_oscilloscope_data);     // 输出幅度信息到示波器                                     
                          }*/ 
                          
                         if(STOP_MENU_FLAG){Menu();}//菜单-------//踩点和标志改变
                         
        #endif                      
                         //GL_CRC();
                        
                         
                       
        


      
      
      
        // 此处编写需要循环执行的代码
    }
}

// **************************** 代码区域 ****************************
