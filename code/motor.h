#ifndef MOTOR_H_
#define MOTOR_H_

#define MAX_DUTY        (3000)
#define DIR_CH1         (P10_3)
#define PWM_CH1         (TCPWM_CH30_P10_2)

#define ENCODER1_TIM        (TC_CH58_ENCODER)                                  // 编码器定时器
#define ENCODER1_PLUS       (TC_CH58_ENCODER_CH1_P17_3)                        // 编码器计数端口
#define ENCODER1_DIR        (TC_CH58_ENCODER_CH2_P17_4)                        // 编码器方向采值端口

void motorAndencouder_init(void);//无刷电机和编码器初始化
void PWM_OUT(int16 duty);//无刷电机pwm输出函数
int16 GetSpeed(void);//获取编码器数值，直接数
#endif