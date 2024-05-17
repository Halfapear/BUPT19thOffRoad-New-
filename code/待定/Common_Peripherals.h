/*
 * Common_Peripherals.h
 *
 *  Created on: 2023��3��18��
 *      Author: ORRN
 */

#ifndef CODE_COMMON_PERIPHERALS_H_
#define CODE_COMMON_PERIPHERALS_H_

//�궨��
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

#define UART_INDEX              (DEBUG_UART_INDEX   )                           // Ĭ�� UART_0
#define UART_BAUDRATE           (DEBUG_UART_BAUDRATE)                           // Ĭ�� 115200
#define UART_TX_PIN             (DEBUG_UART_TX_PIN  )                           // Ĭ�� UART0_TX_P14_0
#define UART_RX_PIN             (DEBUG_UART_RX_PIN  )                           // Ĭ�� UART0_RX_P14_1

#define PWM_L1              (ATOM1_CH5_P02_5)
#define PWM_L2              (ATOM1_CH4_P02_4)


//#define PWM_CH1             ATOM1_CH4_P02_4      //PWM����˿�
//#define DIR_CH1             P02_5               //�����������˿�

#define PWM_CH1             ATOM1_CH5_P02_5      //PWM����˿�
#define DIR_CH1             P02_4               //�����������˿�

//#define PWM_CH1             ATOM1_CH5_P02_5      //PWM����˿�
//#define DIR_CH1             P02_6               //�����������˿�


#define ENCODER1_TIM        TIM2_ENCOEDER            //��������ʱ��
#define ENCODER1_PLUS       TIM2_ENCOEDER_CH1_P33_7   //�����������˿�
#define ENCODER1_DIR        TIM2_ENCOEDER_CH2_P33_6  //�����������ֵ�˿�




#define SPEED_PAGE_INDEX    (10)                                                //SPEED�洢ҳ��

#define SERVO_MOTOR_PWM             (ATOM1_CH1_P33_9)                           // ���������϶����Ӧ����
#define SERVO_MOTOR_FREQ            (50)                                       // ���������϶��Ƶ��  �����ע�ⷶΧ
#define SERVO_MOTOR_MID             (80)                                        //��ֵ75//����ͨ���޸���ֵ�ķ�ʽ�ֲ���е���79
#define SERVO_MOTOR_LMAX            (55)                                        //�����60
#define SERVO_MOTOR_RMAX            (105)                                       //�Ҵ���100
//#define SERVO_MOTOR_MID             (75)                                        //��ֵ75//����ͨ���޸���ֵ�ķ�ʽ�ֲ���е���
//#define SERVO_MOTOR_LMAX            (95)                                        //�����60
//#define SERVO_MOTOR_RMAX            (55)                                       //�Ҵ���100
#define SERVO_MOTOR_DUTY(x)         ((float)PWM_DUTY_MAX/(1000.0/(float)SERVO_MOTOR_FREQ)*(0.5+(float)(x)/90.0))// ------------------ ���ռ�ձȼ��㷽ʽ ------------------

#define X6F_CH1                     (P10_3)      //�����ͨ��
#define X6F_CH2                     (P02_7)      //����ͨ��
#define X6F_CH3                     (P10_1)      //ָʾ��ťͨ��
#define X6F_CH4                     (P02_6)      //������λͨ��
#define X6F_CH5                     (P21_5)      //��ťͨ��1
#define X6F_CH6                     (P21_3)      //��ťͨ��2

//IPS114�궨��
#define ips_show_string                  ips114_show_string
#define ips_show_int                     ips114_show_int
#define ips_show_uint                    ips114_show_uint
#define ips_show_float                   ips114_show_float
#define ips_show_chinese                 ips114_show_chinese
#define ips_clear                        ips114_clear
#define ips_init                         ips114_init
#define ips_show_rgb565_image            ips114_show_rgb565_image

//IPS200�궨��
//#define ips_show_string                  ips200_show_string
//#define ips_show_int                     ips200_show_int
//#define ips_show_uint                    ips200_show_uint
//#define ips_show_float                   ips200_show_float
//#define ips_show_chinese                 ips200_show_chinese
//#define ips_clear                        ips200_clear
//#define ips_init                         ips200_init
//#define ips_show_rgb565_image            ips200_show_rgb565_image
/////////////////////////////////////////////

//ȫ�ֱ�������
extern uint8 key1_flag;
extern uint8 key2_flag;
extern uint8 key3_flag;
extern uint8 key4_flag;
extern uint8 key_val;
extern  const unsigned char gImage_MOSS[60008];   //MOSS��ֽ
extern float Steer_Value;
extern int16 x6f_out[6];
extern int   Ctrl_GO_FLAG;
extern int16 encoder;        //������ֵ
extern int16 stand;


//��������
void Buzzer_init();//��������ʼ��
void Buzzer_check(int time1,int time2);//������LED��ʼ��

void Key_init();//������LED��ʼ��
void key_scan(void);//����ɨ��

void Image_get(void);//�����õ�ͼ��

void LORA_Send_TXT(void);
void LORA_init();//LORA��ʼ��
void LORA_work(void);//LORA��������
void uart_rx_interrupt_handler (void);//LORA�����ж�

void HIP4082_init();//HIP4082��ʼ��
void HIP4082_Motor_ctrl(int32 Motor_SPEED);//HIP4082��·����
void Motor_text(void);//�������
void SPEED_param_t_init();//SPEED������ʼ��
void BLDC_init();//��ˢ�����ʼ��
void BLDC_ctrl(int16 Motor_SPEED);//BLDC����
void BLDC_Cloop_ctrl(int16 SPEED);//BLDC�ջ�����

void HALL_init(void);//������������ʼ��
void HALL_gather(void);//������������ȡֵ

void Steer_init();//�����ʼ��
void Steer_set(int angle);//�������
void Steer_text(void);//�������

void Control_init();//ң�������ų�ʼ��
void x6f_scan(void);//ң����ͨ��ɨ��
void Control_TEXT();//ң��������
void Control_Program(void);//���Ƴ���
void WX_CTRL(void);
void YX_CTRL(void);

void ALL_Init(void);//�ܳ�ʼ��
#endif /* CODE_COMMON_PERIPHERALS_H_ */
