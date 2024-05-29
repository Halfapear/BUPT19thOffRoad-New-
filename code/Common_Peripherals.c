
//5.14 “Common Peripherals” 意思是“常见的外围设备”


#include "zf_common_headfile.h"

//===================================================蜂鸣器===================================================
void Buzzer_init()//蜂鸣器初始化
{
    gpio_init(BUZZER_PIN, GPO, 0, GPO_PUSH_PULL);
}

void Buzzer_check(int time1,int time2)//蜂鸣器的自检函数
{
    gpio_set_level(BUZZER_PIN,1);
    system_delay_ms(time2);
    gpio_set_level(BUZZER_PIN,0);
}



//===================================================按键与LED===================================================
// **************************** 变量定义 ****************************
uint8 key1_state = 1;                                                               // 按键动作状态
uint8 key2_state = 1;                                                               // 按键动作状态
uint8 key3_state = 1;                                                               // 按键动作状态
uint8 key4_state = 1;                                                               // 按键动作状态

uint8 switch1_state = 0;                                                            // 拨码开关动作状态
uint8 switch2_state = 0;                                                            // 拨码开关动作状态

uint8 key1_state_last = 0;                                                          // 上一次按键动作状态
uint8 key2_state_last = 0;                                                          // 上一次按键动作状态
uint8 key3_state_last = 0;                                                          // 上一次按键动作状态
uint8 key4_state_last = 0;                                                          // 上一次按键动作状态

uint8 switch1_state_last = 0;                                                       // 上一次拨码开关动作状态
uint8 switch2_state_last = 0;                                                       // 上一次拨码开关动作状态

uint8 key1_flag;
uint8 key2_flag;
uint8 key3_flag;
uint8 key4_flag;

uint8 key_val;


uint8 switch1_flag;
uint8 switch2_flag;

uint16 key1_count=-1;
uint16 key2_count=-1;
uint16 key3_count=-1;
uint16 key4_count=-1;

uint16 switch1_count=-1;
uint16 switch2_count=-1;

void Key_init()//按键与LED初始化
{
//5.23 我寻思没有4个led吧，那全改1了
       gpio_init(LED1, GPO, GPIO_HIGH, GPO_PUSH_PULL);         // 初始化 LED1 输出 默认高电平 推挽输出模式
       gpio_init(LED1, GPO, GPIO_HIGH, GPO_PUSH_PULL);         // 初始化 LED2 输出 默认高电平 推挽输出模式
       gpio_init(LED1, GPO, GPIO_HIGH, GPO_PUSH_PULL);         // 初始化 LED3 输出 默认高电平 推挽输出模式
       gpio_init(LED1, GPO, GPIO_HIGH, GPO_PUSH_PULL);         // 初始化 LED4 输出 默认高电平 推挽输出模式

       gpio_init(KEY_1, GPI, GPIO_LOW, GPI_PULL_UP);           // 初始化 KEY1 输入 默认高电平 上拉输入
       gpio_init(KEY_2, GPI, GPIO_HIGH, GPI_PULL_UP);           // 初始化 KEY2 输入 默认高电平 上拉输入
       gpio_init(KEY_3, GPI, GPIO_HIGH, GPI_PULL_UP);           // 初始化 KEY3 输入 默认高电平 上拉输入
       gpio_init(KEY_4, GPI, GPIO_HIGH, GPI_PULL_UP);           // 初始化 KEY4 输入 默认高电平 上拉输入

       gpio_init(SWITCH1, GPI, GPIO_HIGH, GPI_FLOATING_IN);    // 初始化 SWITCH1 输入 默认高电平 浮空输入
       gpio_init(SWITCH2, GPI, GPIO_HIGH, GPI_FLOATING_IN);    // 初始化 SWITCH2 输入 默认高电平 浮空输入

}

void key_scan()//按键扫描
    {


        //使用此方法优点在于，不需要使用while(1) 等待，避免处理器资源浪费

        //保存按键状态
        key1_state_last = key1_state;
        key2_state_last = key2_state;
        key3_state_last = key3_state;
        key4_state_last = key4_state;

        //读取当前按键状态
        key1_state = gpio_get_level(KEY1);
        key2_state = gpio_get_level(KEY2);
        key3_state = gpio_get_level(KEY_3);
        key4_state = gpio_get_level(KEY4);


        //检测到按键按下之后  并放开置位标志位
        if(key1_state && !key1_state_last)   {key1_flag = 1;}
        if(key2_state && !key2_state_last)   {key2_flag = 1;}
        if(key3_state && !key3_state_last)   {key3_flag = 1;}
        if(key4_state && !key4_state_last)   {key4_flag = 1;}

        //标志位置位之后，可以使用标志位执行自己想要做的事件

//        system_delay_ms(10);//延时，按键程序应该保证调用时间不小于10ms

    }

/*
//===================================================摄像头===================================================
void Image_get()//总钻风得到图像
{
    if(mt9v03x_finish_flag)
           {
               ips200_displayimage03x((const uint8 *)mt9v03x_image, MT9V03X_W, MT9V03X_H);                       // 显示原始图像
//               ips200_show_gray_image(0, 0, (const uint8 *)mt9v03x_image, MT9V03X_W, MT9V03X_H, 240, 180, 64);     // 显示二值化图像
//        ips200_show_gray_image(0, 0, (const uint8 *)mt9v03x_image, MT9V03X_W, MT9V03X_H, MT9V03X_W, MT9V03X_H, 64);     // 显示二值化图像
               mt9v03x_finish_flag = 0;
           }

}
*/

//===================================================LORA===================================================
//Can this work? Without zf-library?
//这就是从例程抄来的——不是 还有很大不同
//我需要看懂里面所有的函数吗？：
//我觉得这时候我应该看懂原理再来了，不然反推很伤脑筋
//我需要你更细致得讲解 每一行！ 结合原理和代码
//UART_TX_PIN 像这种pin 晚点毫无疑问需要改——go declaration即可——不过一路导到了CYT4BB的library了
//不过问题是为什么例程重新定义了一份：#define LORA_UART_RX_PIN           (UART1_TX_P04_1)
/*
基本流程是
uart_init
uart_rx_interrupt(UART_INDEX, 1);
*/
/*
关于FIFO
先是用法
声明数组 以及fifo_init
uart_query_byte查询UART是否有接收到的新数据。如果有数据，将其存入get_data中
fifo_write_buffer写入FIFO缓冲区
和基本的使用方法 已经融合在LORA_work()等函数里面了，用用挺好的
如果不使用会怎么样 和原版有什么区别 还能使用别的吗
我目前完全无法理解数据丢失的情况
也许还能用环形缓冲区 软件队列（但那也得学着用了）
*/
//不过到现在 用LORA如何发送如何接受我还是没有特别明确的感受
//和例程对比，例程有奇怪的起始标志检测if(temp_data == 0x66)，这到底在干什么 为什么有这些判断？在第二份代码里面有没有类似内容？
//——好像是在直接解析协议
//两份代码中的 LoRa 通信实际上都可以被看作是透明传输的具体实现。这种方式下，数据从一个 LoRa 模块发送到另一个 LoRa 模块，而不需要对地址和信道进行特殊处理
//
//
//


uint8 uart_get_data[64];                                                        // 串口接收数据缓冲区
uint8 fifo_get_data[64];                                                        // fifo 输出读出缓冲区
uint8  get_data = 0;                                                            // 接收数据变量
uint32 fifo_data_count = 0;                                                     // fifo 数据个数
fifo_struct uart_data_fifo;


void LORA_Send_TXT()
{
    uart_write_byte(UART_INDEX, '\n');                                          // 输出换行
    uart_write_byte(UART_INDEX, '\n');                                          // 输出换行
    uart_write_string(UART_INDEX, "////////////////////////////");                                // 输出测试信息
    uart_write_string(UART_INDEX, "Hello!这里是CYT4BB7(接收端)");                                // 输出测试信息
    uart_write_byte(UART_INDEX, '\n');                                          // 输出换行
    uart_write_string(UART_INDEX, "////////////////////////////");                                // 输出测试信息
    uart_write_string(UART_INDEX, "2024.5.15 jzb向你问好");                                // 输出测试信息
    uart_write_byte(UART_INDEX, '\n');                                          // 输出换行
    uart_write_string(UART_INDEX, "////////////////////////////");                                // 输出测试信息
    uart_write_string(UART_INDEX, "一切顺利的话,我们的道路将不再交汇");                                // 输出测试信息
    uart_write_byte(UART_INDEX, '\r');                                          // 输出回车
    uart_write_byte(UART_INDEX, '\n');                                          // 输出换行
    uart_write_string(UART_INDEX, "及时当勉励，岁月不待人");                                // 输出测试信息
    uart_write_byte(UART_INDEX, '\r');                                          // 输出回车
    uart_write_byte(UART_INDEX, '\n');                                          // 输出换行

}


void LORA_init()//LORA初始化
{
        fifo_init(&uart_data_fifo, FIFO_DATA_8BIT, uart_get_data, 64);              // 初始化 fifo 挂载缓冲区

        uart_init(UART_INDEX, UART_BAUDRATE, UART_TX_PIN, UART_RX_PIN);             // 初始化串口
        uart_rx_interrupt(UART_INDEX, 1);                                           // 开启 UART_INDEX 的接收中断

        LORA_Send_TXT();//需要发送的信息

}

void LORA_work()//LORA工作函数
{
    fifo_data_count = fifo_used(&uart_data_fifo);                           // 查看 fifo 是否有数据
    if(fifo_data_count != 0)                                                // 读取到数据了
    {
        fifo_read_buffer(&uart_data_fifo, fifo_get_data, &fifo_data_count, FIFO_READ_AND_CLEAN);    // 将 fifo 中数据读出并清空 fifo 挂载的缓冲

        uart_write_string(UART_INDEX, "\r\nCYT4BB7(接收端) get data:");                // 输出测试信息
        uart_write_buffer(UART_INDEX, fifo_get_data, fifo_data_count);      // 将读取到的数据发送出去

        if(fifo_data_count==1)
        {


                        while(1)
                        {
                         HIP4082_Motor_ctrl(0);
                        }
            Buzzer_check(200,200);//自检完成

        }

    }
}

void uart_rx_interrupt_handler (void)//LORA串口中断
{
//    get_data = uart_read_byte(UART_INDEX);                                      // 接收数据 while 等待式 不建议在中断使用
    uart_query_byte(UART_INDEX, &get_data);                                     // 接收数据 查询式 有数据会返回 TRUE 没有数据会返回 FALSE
    fifo_write_buffer(&uart_data_fifo, &get_data, 1);                           // 将数据写入 fifo 中
}

//===================================================电机===================================================
//HIP4082 是由 Intersil（现为 Renesas Electronics 旗下公司）生产的一种高压、高速 MOSFET 驱动器。它的主要功能是驱动N沟道MOSFET，用于全桥（H-bridge）或半桥（half-bridge）电路中。这种芯片常用于DC-DC转换器、马达控制、电源逆变器等需要高效、高速切换的应用


int16 Target_speed=0;
int16 Current_speed=0;
int16 Gap=0;
int32 OUT=0;

void HIP4082_init()//HIP4082初始化
{
       pwm_init(PWM_L1, 17000, 0);                                                 // PWM 通道 L1 初始化频率 17KHz 占空比初始为 0
       pwm_init(PWM_L2, 17000, 0);                                                 // PWM 通道 L2 初始化频率 17KHz 占空比初始为 0



}

void HIP4082_Motor_ctrl(int32 Motor_SPEED)//HIP4082单路驱动
{

        if(Motor_SPEED>=0)
            {
                pwm_set_duty(PWM_L1, Motor_SPEED );//2.6
                pwm_set_duty(PWM_L2, 0);           // 同一时间 一个电机只能输出一个 PWM 另一通道保持低电平
            }
        else
            {
                pwm_set_duty(PWM_L1, 0);
                pwm_set_duty(PWM_L2,(-Motor_SPEED));                    // 同一时间 一个电机只能输出一个 PWM 另一通道保持低电平
            }

}

void Motor_text()//电机测试
{
   static int16 MOTOR=0;


          key_scan();
          if(key1_flag)
             {
                 key1_flag=0;
                 MOTOR+=100;
             }
          if(key2_flag)
             {
                 key2_flag=0;
                 MOTOR-=100;

             }
          if(key3_flag)
             {
                 key3_flag=0;
                 MOTOR+=1000;
//                 MOTOR=100;
             }
          if(key4_flag)
             {
                 key4_flag=0;
                 MOTOR-=1000;
//                 MOTOR=190;


             }


          ips_show_int(100,  16*3,MOTOR, 5);
//          HIP4082_Motor_ctrl(MOTOR);
          BLDC_ctrl(MOTOR);
//          BLDC_Cloop_ctrl(MOTOR);



}

void SPEED_param_t_init()//SPEED参数初始化
{
    if(flash_check(FLASH_SECTION_INDEX, SPEED_PAGE_INDEX)) //判断Flash是否有数据 : 有数据返回1，无数据返回0
    {
//        flash_buffer_clear();//清空缓冲区

        flash_read_page_to_buffer(FLASH_SECTION_INDEX, SPEED_PAGE_INDEX,10);       //将数据从FLASH指定扇区页码放入到缓冲区
        SPEED=flash_union_buffer[0].int32_type;         //用什么数据类型写入缓冲区就用什么类型读取，不然打印出来的码是乱码
        printf("\r\n已经更新过的(SPEED):%d",  SPEED);

    }
    else{printf("\r\nSPEED为程序设定值(原始值)");}

}

void BLDC_init()//无刷电机初始化
{

    pwm_init(PWM_CH1, 1000, 0);                 //PWM引脚初始化
    gpio_init(DIR_CH1, GPO, 1, GPO_PUSH_PULL);  //方向引脚
    PidInit(&PID_Init);
}

void BLDC_ctrl(int16 Motor_SPEED)//BLDC驱动
{

    if(Motor_SPEED>=0)//正转
    {
        pwm_set_duty(PWM_CH1, (Motor_SPEED));
        gpio_set_level(DIR_CH1,1);
    }
    else             //反转
    {
        pwm_set_duty(PWM_CH2, -Motor_SPEED);
        gpio_set_level(DIR_CH1,0);
    }

 //    ips200_show_uint(100,  16*1,Motor_SPEED, 5);

}

void BLDC_Cloop_ctrl(int16 SPEED) //BLDC闭环控制
{

      Target_speed=SPEED;      //目标速度
      Current_speed= encoder;   //当前速度
      Gap=Target_speed-Current_speed;       //速度差距


      OUT=PidIncCtrl(&PID_MOTOR,(float)Gap);
    if(OUT> 10000) {OUT=10000;}
    if(OUT<-10000) {OUT=-10000;}

    BLDC_ctrl((int16)OUT);
//    system_delay_ms(3);
//    printf("%d,%d,%d,%d\n",Target_speed,Current_speed,Gap,OUT);
//      printf("%d\n",Current_speed);



//    BLDC_ctrl(20);

}

//===================================================霍尔编码器===================================================
int16 encoder=0;  //转速
int16 stand=0;
/*

void HALL_init()//霍尔编码器初始化
{
    encoder_dir_init(ENCODER1_TIM,ENCODER1_PLUS,ENCODER1_DIR);
}

void HALL_gather()//霍尔编码器获取值
{

    encoder= (encoder_get_count(ENCODER1_TIM));
    encoder_clear_count(ENCODER1_TIM);                                // 采集对应编码器数据
//    printf("encoder counter: %d\n", encoder);                // 串口输出采集的数据
//    system_delay_ms(100);
}

*/


//===================================================舵机===================================================


float Steer_Value=SERVO_MOTOR_MID;

void Steer_init()//舵机初始化
{
    pwm_init(SERVO_MOTOR_PWM, SERVO_MOTOR_FREQ, (uint32)SERVO_MOTOR_DUTY(SERVO_MOTOR_MID));
    PidInit(&PID_Init);
}

void Steer_set(int angle)//舵机驱动
{
    if(angle<SERVO_MOTOR_LMAX){angle=SERVO_MOTOR_LMAX;}
    if(angle>SERVO_MOTOR_RMAX){angle=SERVO_MOTOR_RMAX;}
    pwm_set_duty(SERVO_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY(angle));

}

void Steer_text()//舵机测试
{

   static int32 angle=SERVO_MOTOR_MID;

      key_scan();
      if(key1_flag)
         {
             key1_flag=0;
             angle+=10;
         }
      if(key2_flag)
         {
             key2_flag=0;
             angle-=10;
         }
      if(key3_flag)
         {
             key3_flag=0;
             angle+=1;
//             angle=55;//左打死
         }
      if(key4_flag)
         {
             key4_flag=0;
            angle-=1;
//             angle=85;//右打死
         }
      ips_show_int(100,  16*3,angle, 5);
      Steer_set(angle);




}

//===================================================IPS显示===================================================


//===================================================逐飞遥控===================================================

//各通道引脚
int16 x6f_pin_map[6] = {X6F_CH1, X6F_CH2, X6F_CH3, X6F_CH4, X6F_CH5, X6F_CH6};
//各通道高电平计数变量
int16 x6f_count[6];
//各通道高电平计数输出
int16 x6f_out[6];

int   Ctrl_GO_FLAG=0;//遥控发车标志位

void Control_init()//遥控器引脚初始化
{

    //初始化接收机引脚
       gpio_init(X6F_CH1, GPI, GPIO_LOW, GPI_PULL_UP);
       gpio_init(X6F_CH2, GPI, GPIO_LOW, GPI_PULL_UP);
       gpio_init(X6F_CH3, GPI, GPIO_LOW, GPI_PULL_UP);
       gpio_init(X6F_CH4, GPI, GPIO_LOW, GPI_PULL_UP);
       gpio_init(X6F_CH5, GPI, GPIO_LOW, GPI_PULL_UP);
       gpio_init(X6F_CH6, GPI, GPIO_LOW, GPI_PULL_UP);
}

void x6f_scan(void)//遥控器通道扫描
{
    for(int i = 0; i < 6; i ++)
    {
        if(gpio_get_level(x6f_pin_map[i]))
        {
            x6f_count[i]++;
        }
        else if(x6f_count[i] > 0)
        {
            x6f_out[i] = x6f_count[i];
            x6f_count[i] = 0;
        }
    }
}

void Control_TEXT()//遥控器测试
{
    //打印各通道高电平时长计数值
    printf("CH1 = %d\tCH2 = %d\tCH3 = %d\tCH4= %d\tCH5 = %d\tCH6 = %d\r\n",
            x6f_out[0], x6f_out[1], x6f_out[2], x6f_out[3], x6f_out[4], x6f_out[5]);
    gpio_toggle_level(LED1);
    system_delay_ms(50);
}

void Control_Program()//控制程序
{
    WX_CTRL();
//    YX_CTRL();
}

void WX_CTRL()
{
    gpio_set_level(BUZZER_PIN,0);

       if(x6f_out[0]<145)
              {Steer_set(SERVO_MOTOR_LMAX);}

       else if(x6f_out[0]>155)
              {Steer_set(SERVO_MOTOR_RMAX);}

       else   {Steer_set(SERVO_MOTOR_MID);}


       if(x6f_out[1]>170)
       {
//           {BLDC_ctrl(2000);}
//        BLDC_Cloop_ctrl(30);
        SPEED_Value=50;
       }

       else if (x6f_out[1]<140)
       {
//           {BLDC_ctrl(-2000);}
//           BLDC_Cloop_ctrl(-25);
           SPEED_Value=(-30);
       }
       else
       {
//            {BLDC_ctrl(0);}
//           BLDC_Cloop_ctrl(0);
           SPEED_Value=0;
       }

       while(x6f_out[3]>150)
       {
           BLDC_ctrl(0);
   //        BLDC_Cloop_ctrl(0);
       }
}

void YX_CTRL()
{
    gpio_set_level(BUZZER_PIN,0);

       if(x6f_out[0]<145)
              {Steer_set(SERVO_MOTOR_LMAX);}

       else if(x6f_out[0]>155)
              {Steer_set(SERVO_MOTOR_RMAX);}

       else   {Steer_set(SERVO_MOTOR_MID);}


       if(x6f_out[1]>170)
       {
   //        {BLDC_ctrl(2000);}
   //     BLDC_Cloop_ctrl(30);
//        SPEED_Value=50;
        HIP4082_Motor_ctrl(7000);

       }

       else if (x6f_out[1]<140)
       {
   //        {BLDC_ctrl(-2000);}
   //        BLDC_Cloop_ctrl(-25);
//           SPEED_Value=(-30);
        HIP4082_Motor_ctrl(-2000);
       }
       else
       {
   //         {BLDC_ctrl(0);}
   //        BLDC_Cloop_ctrl(0);
//           SPEED_Value=0;
           HIP4082_Motor_ctrl(0);
       }

       while(x6f_out[3]>150)
       {
//           BLDC_ctrl(0);
   //        BLDC_Cloop_ctrl(0);
           HIP4082_Motor_ctrl(0);
       }
}

//===================================================TOF===================================================
//TOF 是 "Time of Flight" 的缩写，意思是“飞行时间”



//===================================================总初始化===================================================
void ALL_Init()
{
    gnss_init(1);        //GPS初始化
//    ips200_init(IPS200_TYPE_PARALLEL8);     //IPS200显示初始化
    ips200_init(IPS200_TYPE_PARALLEL8);        //IPS114
    Buzzer_init();     //蜂鸣器初始化
    Key_init();        //按键及LED初始化
//    HIP4082_init();      //HIP4062驱动初始化
    BLDC_init();     //无刷电机初始化
    Steer_init();      //Steer舵机初始化
    LORA_init();       //LORA初始化
//  mt9v03x_init();    //摄像头初始化
    Control_init();    //遥控器引脚初始化
//    HALL_init();//霍尔编码器初始化

#if FLASH_Init_FLAG
    SPEED_param_t_init();
    FLAG_param_t_init();
    Stop_point_param_t_init();
    Point_distance_param_t_init();
    GPS_param_t_init();
#endif


#if IMU_STATR_FLAG
    IMU_init();         //IMU初始化
#endif

}












