#ifndef MOTOR_H_
#define MOTOR_H_

#define MAX_DUTY        (3000)
#define DIR_CH1         (P10_3)
#define PWM_CH1         (TCPWM_CH30_P10_2)

#define ENCODER1_TIM        (TC_CH58_ENCODER)                                  // ��������ʱ��
#define ENCODER1_PLUS       (TC_CH58_ENCODER_CH1_P17_3)                        // �����������˿�
#define ENCODER1_DIR        (TC_CH58_ENCODER_CH2_P17_4)                        // �����������ֵ�˿�

void motorAndencouder_init(void);//��ˢ����ͱ�������ʼ��
void PWM_OUT(int16 duty);//��ˢ���pwm�������
int16 GetSpeed(void);//��ȡ��������ֵ��ֱ����
#endif