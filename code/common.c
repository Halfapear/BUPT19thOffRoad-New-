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
    gpio_init(LED1, GPO, GPIO_LOW, GPO_PUSH_PULL);          // 初始化 LED1 输出 默认高电平 推挽输出模式
    Servo_Init();               //舵机初始化
    motorAndencouder_init();    //电机编码器初始化
    imu963ra_init();            //陀螺仪初始化
    mic_init();                 //adc初始化
    system_delay_ms(50);        //系统延时
}