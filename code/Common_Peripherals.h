#ifndef CODE_COMMON_PERIPHERALS_H_
#define CODE_COMMON_PERIPHERALS_H_

// 注释掉在 ZF 库文件中已经定义的宏
#define BUZZER_PIN  (P19_4)
#define SERVO_MOTOR_MID  (80)

#define LED1                    (P19_0)
/*
#define LED2                    (P20_8)
#define LED3                    (P21_5)
#define LED4                    (P21_4)
*/

// 注释掉在 ZF 库文件中已经定义的宏
#define KEY1                    (P22_0)
#define KEY2                    (P22_1)
#define KEY3                    (P22_2)
#define KEY4                    (P22_3)
#define SWITCH2                 (P21_5)
#define SWITCH1                 (P21_6)
// 说实话 我不知道这个switch指的是什么


#define UART_INDEX              (DEBUG_UART_INDEX)
#define UART_BAUDRATE           (DEBUG_UART_BAUDRATE)
#define UART_TX_PIN             (DEBUG_UART_TX_PIN)
#define UART_RX_PIN             (DEBUG_UART_RX_PIN)

//#define PWM_L1                  (ATOM1_CH5_P02_5)
//#define PWM_L2                  (ATOM1_CH4_P02_4)

#define SPEED_PAGE_INDEX        (10)

//#define SERVO_MOTOR_PWM         (ATOM1_CH1_P33_9)
#define SERVO_MOTOR_FREQ        (50)
#define SERVO_MOTOR_LMAX        (55)
#define SERVO_MOTOR_RMAX        (105)
#define SERVO_MOTOR_DUTY(x)     ((float)PWM_DUTY_MAX/(1000.0/(float)SERVO_MOTOR_FREQ)*(0.5+(float)(x)/90.0))

//new double
#define PWM_L1              (TCPWM_CH31_P10_3)
#define PWM_L2              (TCPWM_CH30_P10_2)
//new bl
#define MAX_DUTY            (30 )                                              // 最大占空比输出限制
#define DIR_CH1             (P10_3)                                            // 电机方向输出端口
#define PWM_CH1             (TCPWM_CH30_P10_2)                                 // PWM输出端口
//PWM_CH1

#define DIR_CH2             (P05_3)                                            // 电机方向输出端口
#define PWM_CH2             (TCPWM_CH11_P05_2)                                 // PWM输出端口
//PWM_CH1

#define ENCODER1_TIM        (TC_CH58_ENCODER)                                  // 编码器定时器
#define ENCODER1_PLUS       (TC_CH58_ENCODER_CH1_P17_3)                        // 编码器计数端口
#define ENCODER1_DIR        (TC_CH58_ENCODER_CH2_P17_4)                        // 编码器方向采值端口

#define ENCODER2_TIM        (TC_CH20_ENCODER)                                  // 编码器定时器
#define ENCODER2_PLUS       (TC_CH20_ENCODER_CH1_P08_1)                        // 编码器计数端口
#define ENCODER2_DIR        (TC_CH20_ENCODER_CH2_P08_2)                        // 编码器方向采值端口

//new servo
#define SERVO_MOTOR_PWM            (TCPWM_CH13_P00_3)                          // 定义主板上舵机对应引脚
//SERVO_MOTOR_PWM1
#define SERVO_MOTOR_PWM2            (TCPWM_CH12_P01_0)                          // 定义主板上舵机对应引脚
#define SERVO_MOTOR_PWM3            (TCPWM_CH11_P01_1)                          // 定义主板上舵机对应引脚
#define SERVO_MOTOR_FREQ            (50 )                                       // 定义主板上舵机频率  请务必注意范围 50-300

#define SERVO_MOTOR_L_MAX           (50 )                                       // 定义主板上舵机活动范围 角度
#define SERVO_MOTOR_R_MAX           (150)                                       // 定义主板上舵机活动范围 角度


#define X6F_CH1                 (P14_5)
#define X6F_CH2                 (P14_4)
#define X6F_CH3                 (P12_5)
#define X6F_CH4                 (P10_4)
#define X6F_CH5                 (P06_1)
#define X6F_CH6                 (P06_0)


extern uint8 key1_flag;
extern uint8 key2_flag;
extern uint8 key3_flag;
extern uint8 key4_flag;
extern uint8 key_val;
extern const unsigned char gImage_MOSS[60008];
extern float Steer_Value;
extern int16 x6f_out[6];
extern int Ctrl_GO_FLAG;
extern int16 encoder;
extern int16 stand;

void Buzzer_init();
void Buzzer_check(int time1, int time2);
void Key_init();
void key_scan(void);
void Image_get(void);
void LORA_Send_TXT(void);
void LORA_init();
void LORA_work(void);
void uart_rx_interrupt_handler(void);
void HIP4082_init();
void HIP4082_Motor_ctrl(int32 Motor_SPEED);
void Motor_text(void);
void SPEED_param_t_init();
void BLDC_init();
void BLDC_ctrl(int16 Motor_SPEED);
void BLDC_Cloop_ctrl(int16 SPEED);
void HALL_init(void);
void HALL_gather(void);
void Steer_init();
void Steer_set(int angle);
void Steer_text(void);
void Control_init();
void x6f_scan(void);
void Control_TEXT();
void Control_Program(void);
void WX_CTRL(void);
void YX_CTRL(void);
void ALL_Init(void);

#endif /* CODE_COMMON_PERIPHERALS_H_ */
