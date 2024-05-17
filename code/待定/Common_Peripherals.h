/*
 * Common_Peripherals.h
 *
 *  Created on: 2023年3月18日
 *      Author: ORRN
 */

#ifndef CODE_COMMON_PERIPHERALS_H_
#define CODE_COMMON_PERIPHERALS_H_

//宏定义
#define BUZZER_PIN  (P33_10)

#define LED1                    (P20_9)
#define LED2                    (P20_8)
#define LED3                    (P21_5)
#define LED4                    (P21_4)
#define KEY1                    (P22_0)
#define KEY2                    (P22_1)
#define KEY3                    (P22_2)
#define KEY4                    (P22_3)
#define SWITCH2                 (P33_12)
#define SWITCH1                 (P33_13)

#define UART_INDEX              (DEBUG_UART_INDEX   )                           // 默认 UART_0
#define UART_BAUDRATE           (DEBUG_UART_BAUDRATE)                           // 默认 115200
#define UART_TX_PIN             (DEBUG_UART_TX_PIN  )                           // 默认 UART0_TX_P14_0
#define UART_RX_PIN             (DEBUG_UART_RX_PIN  )                           // 默认 UART0_RX_P14_1

#define PWM_L1              (ATOM1_CH5_P02_5)
#define PWM_L2              (ATOM1_CH4_P02_4)


//#define PWM_CH1             ATOM1_CH4_P02_4      //PWM输出端口
//#define DIR_CH1             P02_5               //电机方向输出端口

#define PWM_CH1             ATOM1_CH5_P02_5      //PWM输出端口
#define DIR_CH1             P02_4               //电机方向输出端口

//#define PWM_CH1             ATOM1_CH5_P02_5      //PWM输出端口
//#define DIR_CH1             P02_6               //电机方向输出端口


#define ENCODER1_TIM        TIM2_ENCOEDER            //编码器定时器
#define ENCODER1_PLUS       TIM2_ENCOEDER_CH1_P33_7   //编码器计数端口
#define ENCODER1_DIR        TIM2_ENCOEDER_CH2_P33_6  //编码器方向采值端口




#define SPEED_PAGE_INDEX    (10)                                                //SPEED存储页码

#define SERVO_MOTOR_PWM             (ATOM1_CH1_P33_9)                           // 定义主板上舵机对应引脚
#define SERVO_MOTOR_FREQ            (50)                                       // 定义主板上舵机频率  请务必注意范围
#define SERVO_MOTOR_MID             (80)                                        //中值75//可以通过修改中值的方式弥补机械误差79
#define SERVO_MOTOR_LMAX            (55)                                        //左打死60
#define SERVO_MOTOR_RMAX            (105)                                       //右打死100
//#define SERVO_MOTOR_MID             (75)                                        //中值75//可以通过修改中值的方式弥补机械误差
//#define SERVO_MOTOR_LMAX            (95)                                        //左打死60
//#define SERVO_MOTOR_RMAX            (55)                                       //右打死100
#define SERVO_MOTOR_DUTY(x)         ((float)PWM_DUTY_MAX/(1000.0/(float)SERVO_MOTOR_FREQ)*(0.5+(float)(x)/90.0))// ------------------ 舵机占空比计算方式 ------------------

#define X6F_CH1                     (P10_3)      //方向舵通道
#define X6F_CH2                     (P02_7)      //油门通道
#define X6F_CH3                     (P10_1)      //指示按钮通道
#define X6F_CH4                     (P02_6)      //拨动挡位通道
#define X6F_CH5                     (P21_5)      //旋钮通道1
#define X6F_CH6                     (P21_3)      //旋钮通道2

//IPS114宏定义
#define ips_show_string                  ips114_show_string
#define ips_show_int                     ips114_show_int
#define ips_show_uint                    ips114_show_uint
#define ips_show_float                   ips114_show_float
#define ips_show_chinese                 ips114_show_chinese
#define ips_clear                        ips114_clear
#define ips_init                         ips114_init
#define ips_show_rgb565_image            ips114_show_rgb565_image

//IPS200宏定义
//#define ips_show_string                  ips200_show_string
//#define ips_show_int                     ips200_show_int
//#define ips_show_uint                    ips200_show_uint
//#define ips_show_float                   ips200_show_float
//#define ips_show_chinese                 ips200_show_chinese
//#define ips_clear                        ips200_clear
//#define ips_init                         ips200_init
//#define ips_show_rgb565_image            ips200_show_rgb565_image
/////////////////////////////////////////////

//全局变量声明
extern uint8 key1_flag;
extern uint8 key2_flag;
extern uint8 key3_flag;
extern uint8 key4_flag;
extern uint8 key_val;
extern  const unsigned char gImage_MOSS[60008];   //MOSS壁纸
extern float Steer_Value;
extern int16 x6f_out[6];
extern int   Ctrl_GO_FLAG;
extern int16 encoder;        //编码器值
extern int16 stand;


//函数声明
void Buzzer_init();//蜂鸣器初始化
void Buzzer_check(int time1,int time2);//按键与LED初始化

void Key_init();//按键与LED初始化
void key_scan(void);//按键扫描

void Image_get(void);//总钻风得到图像

void LORA_Send_TXT(void);
void LORA_init();//LORA初始化
void LORA_work(void);//LORA工作函数
void uart_rx_interrupt_handler (void);//LORA串口中断

void HIP4082_init();//HIP4082初始化
void HIP4082_Motor_ctrl(int32 Motor_SPEED);//HIP4082单路驱动
void Motor_text(void);//电机测试
void SPEED_param_t_init();//SPEED参数初始化
void BLDC_init();//无刷电机初始化
void BLDC_ctrl(int16 Motor_SPEED);//BLDC驱动
void BLDC_Cloop_ctrl(int16 SPEED);//BLDC闭环控制

void HALL_init(void);//霍尔编码器初始化
void HALL_gather(void);//霍尔编码器获取值

void Steer_init();//舵机初始化
void Steer_set(int angle);//舵机驱动
void Steer_text(void);//舵机测试

void Control_init();//遥控器引脚初始化
void x6f_scan(void);//遥控器通道扫描
void Control_TEXT();//遥控器测试
void Control_Program(void);//控制程序
void WX_CTRL(void);
void YX_CTRL(void);

void ALL_Init(void);//总初始化
#endif /* CODE_COMMON_PERIPHERALS_H_ */
