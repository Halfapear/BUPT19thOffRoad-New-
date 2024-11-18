/*********************************************************************************************************************
* CYT4BB Opensourec Library ���� CYT4BB ��Դ�⣩��һ�����ڹٷ� SDK �ӿڵĵ�������Դ��
* Copyright (c) 2022 SEEKFREE ��ɿƼ�
*
* ���ļ��� CYT4BB ��Դ���һ����
*
* CYT4BB ��Դ�� ��������
* �����Ը��������������ᷢ���� GPL��GNU General Public License���� GNUͨ�ù������֤��������
* �� GPL �ĵ�3�棨�� GPL3.0������ѡ��ģ��κκ����İ汾�����·�����/���޸���
*
* ����Դ��ķ�����ϣ�����ܷ������ã�����δ�������κεı�֤
* ����û�������������Ի��ʺ��ض���;�ı�֤
* ����ϸ����μ� GPL
*
* ��Ӧ�����յ�����Դ���ͬʱ�յ�һ�� GPL �ĸ���
* ���û�У������<https://www.gnu.org/licenses/>
*
* ����ע����
* ����Դ��ʹ�� GPL3.0 ��Դ���֤Э�� �����������Ϊ���İ汾
* �������Ӣ�İ��� libraries/doc �ļ����µ� GPL3_permission_statement.txt �ļ���
* ���֤������ libraries �ļ����� �����ļ����µ� LICENSE �ļ�
* ��ӭ��λʹ�ò����������� ���޸�����ʱ���뱣����ɿƼ��İ�Ȩ����������������
*
* �ļ�����          main_cm7_0
* ��˾����          �ɶ���ɿƼ����޹�˾
* �汾��Ϣ          �鿴 libraries/doc �ļ����� version �ļ� �汾˵��
* ��������          IAR 9.40.1
* ����ƽ̨          CYT4BB
* ��������          https://seekfree.taobao.com/
*
* �޸ļ�¼
* ����              ����                ��ע
* 2024-1-4       pudding            first version
********************************************************************************************************************/

#include "zf_common_headfile.h"

// ���µĹ��̻��߹����ƶ���λ�����ִ�����²���
// ��һ�� �ر��������д򿪵��ļ�
// �ڶ��� project->clean  �ȴ��·�����������

// �������ǿ�Դ��չ��� ��������ֲ���߲��Ը���������
// �������ǿ�Դ��չ��� ��������ֲ���߲��Ը���������
// �������ǿ�Դ��չ��� ��������ֲ���߲��Ը���������

// **************************** �������� ****************************

int main(void)
{
    clock_init(SYSTEM_CLOCK_250M); 	// ʱ�����ü�ϵͳ��ʼ��<��ر���>
    debug_info_init();                  // ���Դ�����Ϣ��ʼ��
    
    // �˴���д�û����� ���������ʼ�������

    //��ʼ������
    
    ALL_Init();
    mic_init();
    adc_init(XF6_ADC,ADC_12BIT);
    

//CCU60_CH1 ���������úͳ�ʼ����ʱ���жϵı�ʶ����������˵����������һ�ֶ�ʱ��ģ���ͨ�������ڶ�ʱ�Ͳ����жϡ�CCU��Capture/Compare Unit����һ����Ƕ��ʽϵͳ�г�����ģ�飬ͨ�����ڲ��������¼������ɱȽ��¼�������������ƣ�PWM��     
     
//CCU60_CH1 CCU61_CH0 CCU61_CH1�Ļ���
     //����һ�������ǵ��ж���IMU��ʼ����ʱ�̿���
    
    pit_us_init(PIT_CH1, 10);                               // LORA�ж�+ADC�ɼ�
    pit_ms_init(PIT_CH2, 100);                            // GPS���ݽ����ж�
    pit_ms_init(PIT_CH10, 7);                              // ���/����������/���
    
//     Ctrl_GO_FLAG=0; //�����־λ
    Buzzer_check(0,100);                                  // �Լ����
    // ��ʼ������
    //float test_angle;
    //generateChirp(Chrip_signal,FFT_SIZE,250,2000,FS);//���ɱ�׼Chrip�ź�
    seekfree_assistant_interface_init(SEEKFREE_ASSISTANT_DEBUG_UART);           // ��ʼ������������ ѡ��debug���������Ϣ
    
    seekfree_assistant_oscilloscope_data.channel_num  = 4;                      // ����ͨ������Ϊ1��
    
    // �˴���д�û����� ���������ʼ�������
    while(true)
    {
        // �˴���д��Ҫѭ��ִ�еĴ���
        //Menu();       
      //BLDC_ctrl(3000);
        
        if(MIC_Start_FLAG){
              mic_data_copy();//��һ���������ȡ��
         }
        
            //test_data_copy();
              //printf("\r\ntest");
              //test_angle= Get_mic_Angle();
             // printf("\r\n�Ƕ�test_angle:%f",test_angle )
            /* for(int i = 0; i < FFT_SIZE; i++) 
              {
                  seekfree_assistant_oscilloscope_data.data[0] = fft_signal_1[i*2];//fft_outputSignal[0][i];     // ��ȡ��FFT�����ķ�����Ϣ
                  seekfree_assistant_oscilloscope_data.data[1] = fft_signal_2[i*2];//fft_signal[1][i * 2];      // ��ȡ��FFT�����ķ�����Ϣ
                  seekfree_assistant_oscilloscope_data.data[2] = fft_signal_3[i*2];
                  seekfree_assistant_oscilloscope_data.data[3] = fft_signal_4[i*2];
                 seekfree_assistant_oscilloscope_send(&seekfree_assistant_oscilloscope_data);     // ���������Ϣ��ʾ����                                     
              }*/
      //  } 

         // }        
        //printf("/r/nDaty_Z%dT_M,%d,%d,Error%d,azimuth%d",(int)Daty_Z,(int)T_M,(int)T_N,Error,azimuth);
      
        #if  XF6_Control_Flag//ң��ͣ��
            xf6_data_copy();
            if(calcute_xf6_high()>105)
                xf6_stopflag=1;
           /* else
                xf6_stopflag=0;*/
            
            //ips200_show_uint(16,16*5,num,5);
        #endif
            
        #if Main_FLAG
    
                         //if(x6f_out[0]<135) {Ctrl_GO_FLAG=1;}//���Ʒ�����־λ

                         if(GO_FLAG==1)
                          {
                                printf("\r\n��ʼ����������");
                                
                              Follow_Track();//����ѭ������                     
                          }

                          //Follow_Track();
                        /*for(int i = 0; i < FFT_SIZE; i++) 
                          {
                              seekfree_assistant_oscilloscope_data.data[0] =  out_signal_0[i];//fft_outputSignal[0][i];     // ��ȡ��FFT�����ķ�����Ϣ
                              seekfree_assistant_oscilloscope_data.data[1] = out_signal_1[i];//fft_signal[1][i * 2];      // ��ȡ��FFT�����ķ�����Ϣ
                             seekfree_assistant_oscilloscope_data.data[2] = fft_outputSignal[0][i];
                             seekfree_assistant_oscilloscope_data.data[3] = fft_outputSignal[1][i];
                              seekfree_assistant_oscilloscope_send(&seekfree_assistant_oscilloscope_data);     // ���������Ϣ��ʾ����                                     
                          }*/ 
                          
                         if(STOP_MENU_FLAG){Menu();}//�˵�-------//�ȵ�ͱ�־�ı�
                         
        #endif                      
                         //GL_CRC();
                        
                         
                       
        


      
      
      
        // �˴���д��Ҫѭ��ִ�еĴ���
    }
}

// **************************** �������� ****************************
