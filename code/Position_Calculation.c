/*
 * Position_Calculation.c
 *
 *  Created on: 2023��3��18��
 *      Author: ORRN
 */

#include "zf_common_headfile.h"



int Control_FLAG=1;         //�ñ�־λ�����ǣ�����Ϊ1ʱ���޿�ģʽ(��GPS+IMU����)/���ֱ��ϵİ�������ʱ���ñ�־λ����0,�޷�ִ���޿�ģʽ��ת���п�ģʽ
int S_Point=0;              //ֹͣ��λ����
int P_Distance=0;           //��λ�������
int next_point=0;           //��һ��Ŀ���
int Point_interval=0;       //��λ������
int ZT_FLAG=0;              //��׶Ͱһ�ܱ�־λ
int Distance_FLAG=0;        //�����л���־λ
int Angle_FLAG=0;           //�Ƕ��л���־λ
int IMU_suppression_FLAG=1; //IMU���ƿ�����־λ
int16 SPEED_Value=0;        //Ŀ���ٶ�




double distance=0;    //����ľ���
double azimuth=0;     //��λ��(�ó��ĺ�������ֵΪdouble)
double last_azimuth=0;//��һ�εķ�λ��
double Error=0;       //��λ�Ǻͺ���ǵ����(�ó��ĺ�������ֵΪdouble)

float  PD_YAW=0;

void Follow_Track()//����ѭ������
{

#if (Float_Record_FLAG||Double_Record_FLAG)//�����Ⱥ�˫���ȴ洢�������

    distance=get_two_points_distance (gps_tau1201.latitude, gps_tau1201.longitude,Work_target_array[0][next_point], Work_target_array[1][next_point]);//����
    last_azimuth=get_two_points_azimuth (gps_tau1201.latitude, gps_tau1201.longitude,Work_target_array[0][next_point-1], Work_target_array[1][next_point-1]);//��λ�� (���������������ļн�)

    azimuth =get_two_points_azimuth (gps_tau1201.latitude, gps_tau1201.longitude,Work_target_array[0][next_point], Work_target_array[1][next_point]);//��λ�� (���������������ļн�)

#endif

//#if (Int_Record_FLAG)//����
//
//    distance=get_two_points_distance (gps_tau1201.latitude*1000000, gps_tau1201.longitude*1000000,Work_target_array[0][next_point], Work_target_array[1][next_point]);//����
//    azimuth =get_two_points_azimuth (gps_tau1201.latitude*1000000, gps_tau1201.longitude*1000000,Work_target_array[0][next_point], Work_target_array[1][next_point]);//��λ�� (���������������ļн�)
////    distance=get_two_points_distance (gps_tau1201.latitude, gps_tau1201.longitude,Work_target_array[0][next_point]/1000000, Work_target_array[1][next_point]/1000000);//����
////    azimuth =get_two_points_azimuth (gps_tau1201.latitude, gps_tau1201.longitude,Work_target_array[0][next_point]/1000000, Work_target_array[1][next_point]/1000000);//��λ�� (���������������ļн�)
////    if(azimuth>357||azimuth<3)//�Ժ�����д��ã�������
////    {
////        azimuth=0;
////    }
//
//#endif

#if (Array_Record_FLAG)

    distance=get_two_points_distance (gps_tau1201.latitude, gps_tau1201.longitude,GPS_GET_LAT[next_point], GPS_GET_LOT[next_point]);//����
    azimuth =get_two_points_azimuth (gps_tau1201.latitude, gps_tau1201.longitude,GPS_GET_LAT[next_point],  GPS_GET_LOT[next_point]);//��λ�� (���������������ļн�)


#endif

    //�����ںϵĴ���
    //��IMU�滮Ϊ˳ʱ��0-(180),��ʱ��0-(-180)
    //��GPS�滮Ϊ������0-(180),������0-(-180)
    //IMU_Navigation_0()------0
    //IMU_Navigation_180_N()------1
    //IMU_Navigation_180_S()------2
    //Single_layer_feedback()------3
    //Double_layer_feedback_0()------4

if(next_point<1||next_point==7)
{
    Navigation_mode(0);
}
if((next_point>=1&&next_point<7)||(next_point>7&&next_point<10)||next_point>10)
{
    Navigation_mode(3);
}
else if(next_point==10)
{
//    gpio_set_level(BUZZER_PIN,1);
    Navigation_mode(2);
}



//    STEER_Value=PidLocCtrl(&PID_GPS,SERVO_MOTOR_MID+Error);//SD9
//    STEER_Value=PidLocCtrl(&PID_GPS,SERVO_MOTOR_MID-Error);//SD5

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                Switching_point_D();                             //�����о��е�


                if(next_point==1)
                {
                    Retardation(50,40,-400);//�����ƶ�
//                    SPEED_Value=40;
                }
                else if(next_point==2)
                {
                    SPEED_Value=40;
                }
                else if(next_point==3)
                {
                      SPEED_Value=40;
                }
                else if(next_point==4)
                {
                    SPEED_Value=40;
                }
                else if(next_point==5)
                {
                    SPEED_Value=40;
                }
                else if(next_point==6)
                {
                    SPEED_Value=100;
                }
                else if(next_point==7)
                {
//                    Retardation(60,40,-300);//�����ƶ�
                    SPEED_Value=100;

                }
                else if(next_point==8)
                {
                    gpio_set_level(BUZZER_PIN,1);
                    Task_RZT (67,70,0,'N');                                         //��� �ٶ�
                    SPEED_Value=130;
                }
                else if(next_point==9)
                {
//                    Retardation(80,50,-100);//�����ƶ�
                    SPEED_Value=60;
                }
                else if(next_point==10)//��ͷ/////////////////////////////////////////
                {
                    SPEED_Value=50;
                }
                else if(next_point==11)
                {
//                    STOP();
                    SPEED_Value=150;
                }
                else if(next_point==12)
                {
                    SPEED_Value=100;
//                    STOP();
//                    Retardation(60,50,-500);//�����ƶ�;
                }
                else if(next_point==13)
                {
//                    STOP();
                    SPEED_Value=100;
                }
                else if(next_point==14)  //�µ���
                {
                    SPEED_Value=100;//250
//                    STOP();
                }
                else if(next_point==15)//ͣ��
                {
                    STOP();
                }
                else                                                              //Ĭ��
                {
                    SPEED_Value=100;//250
                }


}
//////////////////////////////////////////////////////////////////////////���������////////////////////////////////////////////////////////////////////
void Task_RZT(int Steer,int16 Motor,int Angle,char Type)//����:��׶ͰһȦ
{

    if(Angle==0&&Type=='N')
    {
        if(ZT_FLAG==0)
        {

                   do{
                          Control_FLAG=0;     //��0����GPSӰ��
                          Steer_set(Steer);   //��Ͱ�뾶��С���ɴ����ǶȾ�����
                          SPEED_Value=Motor;//�ջ�
                                if(T_M<(-358))
                                {
                                Control_FLAG=1;//���¸�ֵ����GPSӰ��
                                ZT_FLAG=1;
                                }
                              gpio_set_level(BUZZER_PIN,1);
                   }while(ZT_FLAG==0); //do_while�ṹ:��ִ��һ�Σ�Ȼ���ж������Ƿ�Ϊ�棬Ϊ�������ѭ����Ϊ��������ѭ��
                              gpio_set_level(BUZZER_PIN,0);
        }
    }
    else if(Angle==0&&Type=='S')
       {
           if(ZT_FLAG==0)
                  {

                             do{
                                    Control_FLAG=0;     //��0����GPSӰ��
                                    Steer_set(Steer);   //��Ͱ�뾶��С���ɴ����ǶȾ�����
                                    SPEED_Value=Motor;//�ջ�

                                        if(T_N>(358))
                                        {
                                            Control_FLAG=1;//���¸�ֵ����GPSӰ��
                                            ZT_FLAG=1;
                                        }
                                        gpio_set_level(BUZZER_PIN,1);
                             }while(ZT_FLAG==0); //do_while�ṹ:��ִ��һ�Σ�Ȼ���ж������Ƿ�Ϊ�棬Ϊ�������ѭ����Ϊ��������ѭ��
                                        gpio_set_level(BUZZER_PIN,0);
                  }
       }
    else if(Angle==180&&Type=='N')
    {
        if(ZT_FLAG==0)
               {

                          do{
                                 Control_FLAG=0;     //��0����GPSӰ��
                                 Steer_set(Steer);   //��Ͱ�뾶��С���ɴ����ǶȾ�����
                                 SPEED_Value=Motor;//�ջ�

                                     if(T_N<(-540))
                                     {
                                         Control_FLAG=1;//���¸�ֵ����GPSӰ��
                                         ZT_FLAG=1;
                                     }
                                     gpio_set_level(BUZZER_PIN,1);
                          }while(ZT_FLAG==0); //do_while�ṹ:��ִ��һ�Σ�Ȼ���ж������Ƿ�Ϊ�棬Ϊ�������ѭ����Ϊ��������ѭ��
                                     gpio_set_level(BUZZER_PIN,0);
               }
    }
}



void Switching_point_D()//�е�(����)�ж�
{



                if(next_point==0||next_point<1||next_point==12)
                {
                    if(distance<2)//�е�
                    {
                          next_point++;
    //                      Buzzer_check(50,50);                                  // �Լ����
                        printf("\r\n 3-��ǰ��λ-%d",next_point);
                        printf("\r\n \\\\\\\\\\\\\\\\\\\\");
                    }
                }
                else if(next_point==8||next_point==9||next_point==10)
                {
                    if(distance<2)//�е�
                    {
                          next_point++;
    //                      Buzzer_check(50,50);                                  // �Լ����
                        printf("\r\n 0.75-��ǰ��λ-%d",next_point);
                        printf("\r\n \\\\\\\\\\\\\\\\\\\\");
                    }
                }
              else
              {
                   if(distance<0.75)//�е�
                    {
                          next_point++;
    //                      Buzzer_check(50,50);                                  // �Լ����
                        printf("\r\n 2-��ǰ��λ-%d",next_point);
                        printf("\r\n \\\\\\\\\\\\\\\\\\\\");
                    }
              }



}


void Double_layer_feedback_0()//˫�㷴��(����ȥ��ʱ����0��)
{
    //�򱱷���
            if(azimuth>180)  //������180�ķ���Ǳ�Ϊ����
               {
                   azimuth-=360;
               }
      //���Ϸ���
             azimuth+=180;
             if(azimuth>180)  //������180�ķ���Ǳ�Ϊ����
                 {
                     azimuth-=360;
                 }

        if((azimuth - Daty_Z)>180)      //�����Ƕ�֮�����180��ʱ���򽫲�ֵ��ȥ360
        {
             Error=azimuth-PidLocCtrl(&PID_IMU,Daty_Z)-360;//GPS�ķ����-IMU����PD����ĺ����
             printf("IMU(0).azimuth:%f,Daty_Z:%f,Error:%f\n",azimuth,Daty_Z,Error);//�����λ����������ô�����δ���������ֵ��0


        }
        else if((azimuth - Daty_Z)<-180)//�����Ƕ�֮��С��-180��ʱ���򽫲�ֵ����360
        {
             Error=azimuth-PidLocCtrl(&PID_IMU,Daty_Z)+360;//GPS�ķ����-IMU����PD����ĺ����
             printf("IMU(0).azimuth:%f,Daty_Z:%f,Error:%f\n",azimuth,Daty_Z,Error);//�����λ����������ô�����δ���������ֵ��0

        }
        else
        {
              Error=azimuth-PidLocCtrl(&PID_IMU,Daty_Z);//GPS�ķ����-IMU����PD����ĺ����
              printf("IMU(0).azimuth:%f,Daty_Z:%f,Error:%f\n",azimuth,Daty_Z,Error);//�����λ����������ô�����δ���������ֵ��0

        }

}

void Double_layer_feedback_180()//˫�㷴��(���ڷ��ص�ʱ����180��)
{
    if(azimuth>180)  //������180�ķ���Ǳ�Ϊ����
        {
            azimuth-=360;
        }


        if((azimuth - Daty_Z)>180)      //�����Ƕ�֮�����180��ʱ���򽫲�ֵ��ȥ360
        {
             Error=azimuth-PidLocCtrl(&PID_IMU,Daty_Z)-360;//GPS�ķ����-IMU����PD����ĺ����
             printf("IMU(180).azimuth:%f,Daty_Z:%f,Error:%f\n",azimuth,Daty_Z,Error);//�����λ����������ô�����δ���������ֵ��0


        }
        else if((azimuth - Daty_Z)<-180)//�����Ƕ�֮��С��-180��ʱ���򽫲�ֵ����360
        {
             Error=azimuth-PidLocCtrl(&PID_IMU,Daty_Z)+360;//GPS�ķ����-IMU����PD����ĺ����
             printf("IMU(180).azimuth:%f,Daty_Z:%f,Error:%f\n",azimuth,Daty_Z,Error);//�����λ����������ô�����δ���������ֵ��0

        }
        else
        {
              Error=azimuth-PidLocCtrl(&PID_IMU,Daty_Z);//GPS�ķ����-IMU����PD����ĺ����
              printf("IMU(180).azimuth:%f,Daty_Z:%f,Error:%f\n",azimuth,Daty_Z,Error);//�����λ����������ô�����δ���������ֵ��0

        }

}

void Single_layer_feedback()//���㷴��
{
    //�򱱷���
            if(azimuth>180)  //������180�ķ���Ǳ�Ϊ����
               {
                   azimuth-=360;
               }
      //���Ϸ���
             azimuth+=180;
             if(azimuth>180)  //������180�ķ���Ǳ�Ϊ����
                 {
                     azimuth-=360;
                 }
        if((azimuth - Daty_Z)>180)      //�����Ƕ�֮�����180��ʱ���򽫲�ֵ��ȥ360
        {
            Error = azimuth-Daty_Z-360;
             printf("GPS.azimuth:%f,Daty_Z:%f,Error:%f\n",azimuth,Daty_Z,Error);//�����λ����������ô�����δ���������ֵ��0


        }
        else if((azimuth - Daty_Z)<-180)//�����Ƕ�֮��С��-180��ʱ���򽫲�ֵ����360
        {
            Error = azimuth-Daty_Z+360;
             printf("GPS.azimuth:%f,Daty_Z:%f,Error:%f\n",azimuth,Daty_Z,Error);//�����λ����������ô�����δ���������ֵ��0

        }
        else
        {
            Error = azimuth-Daty_Z;
              printf("GPS.azimuth:%f,Daty_Z:%f,Error:%f\n",azimuth,Daty_Z,Error);//�����λ����������ô�����δ���������ֵ��0
        }

}

void IMU_Navigation_0()//0��IMU����
{
    Error=T_M;
}

void IMU_Navigation_180_N()//180��IMU����
{
    Error=(180+T_M);
}

void IMU_Navigation_180_S()//180��IMU����
{
    Error=(-180+T_M);
}
void Retardation(int X,int16 Y,int16 Z)//�����ƶ�
{
    if(encoder<X)
    {
     SPEED_Value=Y;
    }
    else
    {
        SPEED_Value=(Z);
    }
}

void STOP()//ͣ��
{
    if(encoder<10)
                        {
                            while(1)
                            {
                                SPEED_Value=0;                                  //������ɣ�ͣ��������
                                Steer_set(SERVO_MOTOR_MID);
                                gpio_set_level(BUZZER_PIN,1);
                            }
                        }
                        else
                        {
                            SPEED_Value=(-300);
                        }

}

void Navigation_mode(int TYPE)
{
    if(TYPE==0)
    {
        IMU_Navigation_0();      //IMU--0�ȵ���
    }
    else if(TYPE==1)
    {
        IMU_Navigation_180_N();  //��ʱ��IMU--180�ȵ���(��ת)
    }
    else if(TYPE==2)
    {
        IMU_Navigation_180_S();  //˳ʱ��IMU--180�ȵ���(��ת)
    }
    else if(TYPE==3)
    {
        Single_layer_feedback(); //���㷴��----û��IMU����ת��Ч��
    }
    else if(TYPE==4)
    {
        Double_layer_feedback_0();//˫�㷴��(����ȥ��ʱ����0��)
    }

}
