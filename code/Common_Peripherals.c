
//5.14 ��Common Peripherals�� ��˼�ǡ���������Χ�豸��


#include "zf_common_headfile.h"

//===================================================������===================================================
void Buzzer_init()//��������ʼ��
{
    gpio_init(BUZZER_PIN, GPO, 0, GPO_PUSH_PULL);
}

void Buzzer_check(int time1,int time2)//���������Լ캯��
{
    gpio_set_level(BUZZER_PIN,1);
    system_delay_ms(time2);
    gpio_set_level(BUZZER_PIN,0);
}



//===================================================������LED===================================================
// **************************** �������� ****************************
uint8 key1_state = 1;                                                               // ��������״̬
uint8 key2_state = 1;                                                               // ��������״̬
uint8 key3_state = 1;                                                               // ��������״̬
uint8 key4_state = 1;                                                               // ��������״̬

uint8 switch1_state = 0;                                                            // ���뿪�ض���״̬
uint8 switch2_state = 0;                                                            // ���뿪�ض���״̬

uint8 key1_state_last = 0;                                                          // ��һ�ΰ�������״̬
uint8 key2_state_last = 0;                                                          // ��һ�ΰ�������״̬
uint8 key3_state_last = 0;                                                          // ��һ�ΰ�������״̬
uint8 key4_state_last = 0;                                                          // ��һ�ΰ�������״̬

uint8 switch1_state_last = 0;                                                       // ��һ�β��뿪�ض���״̬
uint8 switch2_state_last = 0;                                                       // ��һ�β��뿪�ض���״̬

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

void Key_init()//������LED��ʼ��
{
//5.23 ��Ѱ˼û��4��led�ɣ���ȫ��1��
       gpio_init(LED1, GPO, GPIO_HIGH, GPO_PUSH_PULL);         // ��ʼ�� LED1 ��� Ĭ�ϸߵ�ƽ �������ģʽ
       gpio_init(LED1, GPO, GPIO_HIGH, GPO_PUSH_PULL);         // ��ʼ�� LED2 ��� Ĭ�ϸߵ�ƽ �������ģʽ
       gpio_init(LED1, GPO, GPIO_HIGH, GPO_PUSH_PULL);         // ��ʼ�� LED3 ��� Ĭ�ϸߵ�ƽ �������ģʽ
       gpio_init(LED1, GPO, GPIO_HIGH, GPO_PUSH_PULL);         // ��ʼ�� LED4 ��� Ĭ�ϸߵ�ƽ �������ģʽ

       gpio_init(KEY_1, GPI, GPIO_LOW, GPI_PULL_UP);           // ��ʼ�� KEY1 ���� Ĭ�ϸߵ�ƽ ��������
       gpio_init(KEY_2, GPI, GPIO_HIGH, GPI_PULL_UP);           // ��ʼ�� KEY2 ���� Ĭ�ϸߵ�ƽ ��������
       gpio_init(KEY_3, GPI, GPIO_HIGH, GPI_PULL_UP);           // ��ʼ�� KEY3 ���� Ĭ�ϸߵ�ƽ ��������
       gpio_init(KEY_4, GPI, GPIO_HIGH, GPI_PULL_UP);           // ��ʼ�� KEY4 ���� Ĭ�ϸߵ�ƽ ��������

       gpio_init(SWITCH1, GPI, GPIO_HIGH, GPI_FLOATING_IN);    // ��ʼ�� SWITCH1 ���� Ĭ�ϸߵ�ƽ ��������
       gpio_init(SWITCH2, GPI, GPIO_HIGH, GPI_FLOATING_IN);    // ��ʼ�� SWITCH2 ���� Ĭ�ϸߵ�ƽ ��������

}

void key_scan()//����ɨ��
    {


        //ʹ�ô˷����ŵ����ڣ�����Ҫʹ��while(1) �ȴ������⴦������Դ�˷�

        //���水��״̬
        key1_state_last = key1_state;
        key2_state_last = key2_state;
        key3_state_last = key3_state;
        key4_state_last = key4_state;

        //��ȡ��ǰ����״̬
        key1_state = gpio_get_level(KEY1);
        key2_state = gpio_get_level(KEY2);
        key3_state = gpio_get_level(KEY_3);
        key4_state = gpio_get_level(KEY4);


        //��⵽��������֮��  ���ſ���λ��־λ
        if(key1_state && !key1_state_last)   {key1_flag = 1;}
        if(key2_state && !key2_state_last)   {key2_flag = 1;}
        if(key3_state && !key3_state_last)   {key3_flag = 1;}
        if(key4_state && !key4_state_last)   {key4_flag = 1;}

        //��־λ��λ֮�󣬿���ʹ�ñ�־λִ���Լ���Ҫ�����¼�

//        system_delay_ms(10);//��ʱ����������Ӧ�ñ�֤����ʱ�䲻С��10ms

    }

/*
//===================================================����ͷ===================================================
void Image_get()//�����õ�ͼ��
{
    if(mt9v03x_finish_flag)
           {
               ips200_displayimage03x((const uint8 *)mt9v03x_image, MT9V03X_W, MT9V03X_H);                       // ��ʾԭʼͼ��
//               ips200_show_gray_image(0, 0, (const uint8 *)mt9v03x_image, MT9V03X_W, MT9V03X_H, 240, 180, 64);     // ��ʾ��ֵ��ͼ��
//        ips200_show_gray_image(0, 0, (const uint8 *)mt9v03x_image, MT9V03X_W, MT9V03X_H, MT9V03X_W, MT9V03X_H, 64);     // ��ʾ��ֵ��ͼ��
               mt9v03x_finish_flag = 0;
           }

}
*/

//===================================================LORA===================================================
//Can this work? Without zf-library?
//����Ǵ����̳����ġ������� ���кܴ�ͬ
//����Ҫ�����������еĺ����𣿣�
//�Ҿ�����ʱ����Ӧ�ÿ���ԭ�������ˣ���Ȼ���ƺ����Խ�
//����Ҫ���ϸ�µý��� ÿһ�У� ���ԭ��ʹ���
//UART_TX_PIN ������pin ������������Ҫ�ġ���go declaration���ɡ�������һ·������CYT4BB��library��
//����������Ϊʲô�������¶�����һ�ݣ�#define LORA_UART_RX_PIN           (UART1_TX_P04_1)
/*
����������
uart_init
uart_rx_interrupt(UART_INDEX, 1);
*/
/*
����FIFO
�����÷�
�������� �Լ�fifo_init
uart_query_byte��ѯUART�Ƿ��н��յ��������ݡ���������ݣ��������get_data��
fifo_write_bufferд��FIFO������
�ͻ�����ʹ�÷��� �Ѿ��ں���LORA_work()�Ⱥ��������ˣ�����ͦ�õ�
�����ʹ�û���ô�� ��ԭ����ʲô���� ����ʹ�ñ����
��Ŀǰ��ȫ�޷�������ݶ�ʧ�����
Ҳ�����û��λ����� ������У�����Ҳ��ѧ�����ˣ�
*/
//���������� ��LORA��η�����ν����һ���û���ر���ȷ�ĸ���
//�����̶Աȣ���������ֵ���ʼ��־���if(temp_data == 0x66)���⵽���ڸ�ʲô Ϊʲô����Щ�жϣ��ڵڶ��ݴ���������û���������ݣ�
//������������ֱ�ӽ���Э��
//���ݴ����е� LoRa ͨ��ʵ���϶����Ա�������͸������ľ���ʵ�֡����ַ�ʽ�£����ݴ�һ�� LoRa ģ�鷢�͵���һ�� LoRa ģ�飬������Ҫ�Ե�ַ���ŵ��������⴦��
//
//
//


uint8 uart_get_data[64];                                                        // ���ڽ������ݻ�����
uint8 fifo_get_data[64];                                                        // fifo �������������
uint8  get_data = 0;                                                            // �������ݱ���
uint32 fifo_data_count = 0;                                                     // fifo ���ݸ���
fifo_struct uart_data_fifo;


void LORA_Send_TXT()
{
    uart_write_byte(UART_INDEX, '\n');                                          // �������
    uart_write_byte(UART_INDEX, '\n');                                          // �������
    uart_write_string(UART_INDEX, "////////////////////////////");                                // ���������Ϣ
    uart_write_string(UART_INDEX, "Hello!������CYT4BB7(���ն�)");                                // ���������Ϣ
    uart_write_byte(UART_INDEX, '\n');                                          // �������
    uart_write_string(UART_INDEX, "////////////////////////////");                                // ���������Ϣ
    uart_write_string(UART_INDEX, "2024.5.15 jzb�����ʺ�");                                // ���������Ϣ
    uart_write_byte(UART_INDEX, '\n');                                          // �������
    uart_write_string(UART_INDEX, "////////////////////////////");                                // ���������Ϣ
    uart_write_string(UART_INDEX, "һ��˳���Ļ�,���ǵĵ�·�����ٽ���");                                // ���������Ϣ
    uart_write_byte(UART_INDEX, '\r');                                          // ����س�
    uart_write_byte(UART_INDEX, '\n');                                          // �������
    uart_write_string(UART_INDEX, "��ʱ�����������²�����");                                // ���������Ϣ
    uart_write_byte(UART_INDEX, '\r');                                          // ����س�
    uart_write_byte(UART_INDEX, '\n');                                          // �������

}


void LORA_init()//LORA��ʼ��
{
        fifo_init(&uart_data_fifo, FIFO_DATA_8BIT, uart_get_data, 64);              // ��ʼ�� fifo ���ػ�����

        uart_init(UART_INDEX, UART_BAUDRATE, UART_TX_PIN, UART_RX_PIN);             // ��ʼ������
        uart_rx_interrupt(UART_INDEX, 1);                                           // ���� UART_INDEX �Ľ����ж�

        LORA_Send_TXT();//��Ҫ���͵���Ϣ

}

void LORA_work()//LORA��������
{
    fifo_data_count = fifo_used(&uart_data_fifo);                           // �鿴 fifo �Ƿ�������
    if(fifo_data_count != 0)                                                // ��ȡ��������
    {
        fifo_read_buffer(&uart_data_fifo, fifo_get_data, &fifo_data_count, FIFO_READ_AND_CLEAN);    // �� fifo �����ݶ�������� fifo ���صĻ���

        uart_write_string(UART_INDEX, "\r\nCYT4BB7(���ն�) get data:");                // ���������Ϣ
        uart_write_buffer(UART_INDEX, fifo_get_data, fifo_data_count);      // ����ȡ�������ݷ��ͳ�ȥ

        if(fifo_data_count==1)
        {


                        while(1)
                        {
                         HIP4082_Motor_ctrl(0);
                        }
            Buzzer_check(200,200);//�Լ����

        }

    }
}

void uart_rx_interrupt_handler (void)//LORA�����ж�
{
//    get_data = uart_read_byte(UART_INDEX);                                      // �������� while �ȴ�ʽ ���������ж�ʹ��
    uart_query_byte(UART_INDEX, &get_data);                                     // �������� ��ѯʽ �����ݻ᷵�� TRUE û�����ݻ᷵�� FALSE
    fifo_write_buffer(&uart_data_fifo, &get_data, 1);                           // ������д�� fifo ��
}

//===================================================���===================================================
//HIP4082 ���� Intersil����Ϊ Renesas Electronics ���¹�˾��������һ�ָ�ѹ������ MOSFET ��������������Ҫ����������N����MOSFET������ȫ�ţ�H-bridge������ţ�half-bridge����·�С�����оƬ������DC-DCת�����������ơ���Դ���������Ҫ��Ч�������л���Ӧ��


int16 Target_speed=0;
int16 Current_speed=0;
int16 Gap=0;
int32 OUT=0;

void HIP4082_init()//HIP4082��ʼ��
{
       pwm_init(PWM_L1, 17000, 0);                                                 // PWM ͨ�� L1 ��ʼ��Ƶ�� 17KHz ռ�ձȳ�ʼΪ 0
       pwm_init(PWM_L2, 17000, 0);                                                 // PWM ͨ�� L2 ��ʼ��Ƶ�� 17KHz ռ�ձȳ�ʼΪ 0



}

void HIP4082_Motor_ctrl(int32 Motor_SPEED)//HIP4082��·����
{

        if(Motor_SPEED>=0)
            {
                pwm_set_duty(PWM_L1, Motor_SPEED );//2.6
                pwm_set_duty(PWM_L2, 0);           // ͬһʱ�� һ�����ֻ�����һ�� PWM ��һͨ�����ֵ͵�ƽ
            }
        else
            {
                pwm_set_duty(PWM_L1, 0);
                pwm_set_duty(PWM_L2,(-Motor_SPEED));                    // ͬһʱ�� һ�����ֻ�����һ�� PWM ��һͨ�����ֵ͵�ƽ
            }

}

void Motor_text()//�������
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

void SPEED_param_t_init()//SPEED������ʼ��
{
    if(flash_check(FLASH_SECTION_INDEX, SPEED_PAGE_INDEX)) //�ж�Flash�Ƿ������� : �����ݷ���1�������ݷ���0
    {
//        flash_buffer_clear();//��ջ�����

        flash_read_page_to_buffer(FLASH_SECTION_INDEX, SPEED_PAGE_INDEX,10);       //�����ݴ�FLASHָ������ҳ����뵽������
        SPEED=flash_union_buffer[0].int32_type;         //��ʲô��������д�뻺��������ʲô���Ͷ�ȡ����Ȼ��ӡ��������������
        printf("\r\n�Ѿ����¹���(SPEED):%d",  SPEED);

    }
    else{printf("\r\nSPEEDΪ�����趨ֵ(ԭʼֵ)");}

}

void BLDC_init()//��ˢ�����ʼ��
{

    pwm_init(PWM_CH1, 1000, 0);                 //PWM���ų�ʼ��
    gpio_init(DIR_CH1, GPO, 1, GPO_PUSH_PULL);  //��������
    PidInit(&PID_Init);
}

void BLDC_ctrl(int16 Motor_SPEED)//BLDC����
{

    if(Motor_SPEED>=0)//��ת
    {
        pwm_set_duty(PWM_CH1, (Motor_SPEED));
        gpio_set_level(DIR_CH1,1);
    }
    else             //��ת
    {
        pwm_set_duty(PWM_CH2, -Motor_SPEED);
        gpio_set_level(DIR_CH1,0);
    }

 //    ips200_show_uint(100,  16*1,Motor_SPEED, 5);

}

void BLDC_Cloop_ctrl(int16 SPEED) //BLDC�ջ�����
{

      Target_speed=SPEED;      //Ŀ���ٶ�
      Current_speed= encoder;   //��ǰ�ٶ�
      Gap=Target_speed-Current_speed;       //�ٶȲ��


      OUT=PidIncCtrl(&PID_MOTOR,(float)Gap);
    if(OUT> 10000) {OUT=10000;}
    if(OUT<-10000) {OUT=-10000;}

    BLDC_ctrl((int16)OUT);
//    system_delay_ms(3);
//    printf("%d,%d,%d,%d\n",Target_speed,Current_speed,Gap,OUT);
//      printf("%d\n",Current_speed);



//    BLDC_ctrl(20);

}

//===================================================����������===================================================
int16 encoder=0;  //ת��
int16 stand=0;
/*

void HALL_init()//������������ʼ��
{
    encoder_dir_init(ENCODER1_TIM,ENCODER1_PLUS,ENCODER1_DIR);
}

void HALL_gather()//������������ȡֵ
{

    encoder= (encoder_get_count(ENCODER1_TIM));
    encoder_clear_count(ENCODER1_TIM);                                // �ɼ���Ӧ����������
//    printf("encoder counter: %d\n", encoder);                // ��������ɼ�������
//    system_delay_ms(100);
}

*/


//===================================================���===================================================


float Steer_Value=SERVO_MOTOR_MID;

void Steer_init()//�����ʼ��
{
    pwm_init(SERVO_MOTOR_PWM, SERVO_MOTOR_FREQ, (uint32)SERVO_MOTOR_DUTY(SERVO_MOTOR_MID));
    PidInit(&PID_Init);
}

void Steer_set(int angle)//�������
{
    if(angle<SERVO_MOTOR_LMAX){angle=SERVO_MOTOR_LMAX;}
    if(angle>SERVO_MOTOR_RMAX){angle=SERVO_MOTOR_RMAX;}
    pwm_set_duty(SERVO_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY(angle));

}

void Steer_text()//�������
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
//             angle=55;//�����
         }
      if(key4_flag)
         {
             key4_flag=0;
            angle-=1;
//             angle=85;//�Ҵ���
         }
      ips_show_int(100,  16*3,angle, 5);
      Steer_set(angle);




}

//===================================================IPS��ʾ===================================================


//===================================================���ң��===================================================

//��ͨ������
int16 x6f_pin_map[6] = {X6F_CH1, X6F_CH2, X6F_CH3, X6F_CH4, X6F_CH5, X6F_CH6};
//��ͨ���ߵ�ƽ��������
int16 x6f_count[6];
//��ͨ���ߵ�ƽ�������
int16 x6f_out[6];

int   Ctrl_GO_FLAG=0;//ң�ط�����־λ

void Control_init()//ң�������ų�ʼ��
{

    //��ʼ�����ջ�����
       gpio_init(X6F_CH1, GPI, GPIO_LOW, GPI_PULL_UP);
       gpio_init(X6F_CH2, GPI, GPIO_LOW, GPI_PULL_UP);
       gpio_init(X6F_CH3, GPI, GPIO_LOW, GPI_PULL_UP);
       gpio_init(X6F_CH4, GPI, GPIO_LOW, GPI_PULL_UP);
       gpio_init(X6F_CH5, GPI, GPIO_LOW, GPI_PULL_UP);
       gpio_init(X6F_CH6, GPI, GPIO_LOW, GPI_PULL_UP);
}

void x6f_scan(void)//ң����ͨ��ɨ��
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

void Control_TEXT()//ң��������
{
    //��ӡ��ͨ���ߵ�ƽʱ������ֵ
    printf("CH1 = %d\tCH2 = %d\tCH3 = %d\tCH4= %d\tCH5 = %d\tCH6 = %d\r\n",
            x6f_out[0], x6f_out[1], x6f_out[2], x6f_out[3], x6f_out[4], x6f_out[5]);
    gpio_toggle_level(LED1);
    system_delay_ms(50);
}

void Control_Program()//���Ƴ���
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
//TOF �� "Time of Flight" ����д����˼�ǡ�����ʱ�䡱



//===================================================�ܳ�ʼ��===================================================
void ALL_Init()
{
    gnss_init(1);        //GPS��ʼ��
//    ips200_init(IPS200_TYPE_PARALLEL8);     //IPS200��ʾ��ʼ��
    ips200_init(IPS200_TYPE_PARALLEL8);        //IPS114
    Buzzer_init();     //��������ʼ��
    Key_init();        //������LED��ʼ��
//    HIP4082_init();      //HIP4062������ʼ��
    BLDC_init();     //��ˢ�����ʼ��
    Steer_init();      //Steer�����ʼ��
    LORA_init();       //LORA��ʼ��
//  mt9v03x_init();    //����ͷ��ʼ��
    Control_init();    //ң�������ų�ʼ��
//    HALL_init();//������������ʼ��

#if FLASH_Init_FLAG
    SPEED_param_t_init();
    FLAG_param_t_init();
    Stop_point_param_t_init();
    Point_distance_param_t_init();
    GPS_param_t_init();
#endif


#if IMU_STATR_FLAG
    IMU_init();         //IMU��ʼ��
#endif

}












