
#include "zf_common_headfile.h"

// **************************** 代码区域 ****************************

int main(void)
{
//把clock_init();换掉 变成    clock_init(SYSTEM_CLOCK_250M); 	// 时钟配置及系统初始化<务必保留>
    clock_init(SYSTEM_CLOCK_250M); 	// 时钟配置及系统初始化<务必保留>
    debug_init();                   // 初始化默认调试串口

    //初始化代码
     ALL_Init();

#if ISR_Start_FLAG

//CCU60_CH1 是用于配置和初始化定时器中断的标识符。具体来说，它代表了一种定时器模块的通道，用于定时和产生中断。CCU（Capture/Compare Unit）是一种在嵌入式系统中常见的模块，通常用于捕获输入事件、生成比较事件或用于脉宽调制（PWM）     
     
//CCU60_CH1 CCU61_CH0 CCU61_CH1的换成
     pit_us_init(PIT_CH0, 10);                               // LORA中断
     pit_ms_init(PIT_CH1, 100);                            // GPS数据解析中断/霍尔编码器中断
     pit_ms_init(PIT_CH2, 7);                              // 舵机

#endif

//     Ctrl_GO_FLAG=0; //清除标志位
     Buzzer_check(0,100);                                  // 自检完成
    // 初始化代码

//这个被大坑了一波     
//    cpu_wait_event_ready();         // 等待所有核心初始化完毕



    while (1)
    {
         Menu();
         /*
        #if Main_FLAG
                         if(x6f_out[0]<135) {Ctrl_GO_FLAG=1;}//控制发车标志位

                         if(GO_FLAG==1)
                           {

                             if(Ctrl_GO_FLAG==1&&Control_FLAG==1) {Follow_Track();}//核心循迹程序
                             if(x6f_out[2]>165&&x6f_out[2]<175)//有控模式
                             {
                                  Control_FLAG=0;
                                  Control_Program();//控制程序
                             }
                           }

                         if(STOP_MENU_FLAG){Menu();}//菜单
        #endif

                         GL_CRC();

                         if(GL_IMU_Flag==1) {Follow_Track();}//核心循迹程序
                         if(x6f_out[2]>165&&x6f_out[2]<175)//有控模式
                         {
                              Control_FLAG=0;
                              GL_IMU_Flag=0;
                              Control_Program();//控制程序
                         }

        */

    }
}

