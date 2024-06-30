#include "common.h"
#include "steer.h"
#include "motor.h"
#include "mic.h"

#define LED1                    (P19_0)

uint16 CurrentServoDty=0;
uint16 ms=0;
uint16 s=0;
uint16 encoder=0;

void All_init()
{
    gpio_init(LED1, GPO, GPIO_LOW, GPO_PUSH_PULL);          // ��ʼ�� LED1 ��� Ĭ�ϸߵ�ƽ �������ģʽ
    Servo_Init();               //�����ʼ��
    motorAndencouder_init();    //�����������ʼ��
    imu963ra_init();            //�����ǳ�ʼ��
    mic_init();                 //adc��ʼ��
    system_delay_ms(50);        //ϵͳ��ʱ
}