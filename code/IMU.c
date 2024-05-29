/*
 * IMU.c
 *
 *  Created on: 2023��3��18��
 *      Author: ORRN
 */
//5.14 ��IMU�����ǿ��������ŵ�������������Ȼ���GPS��ϣ�Ҫ������һ���̫�����
//IMU�ĵ����ÿ������ֱ���ÿ�������������

#include "zf_common_headfile.h"


//gyro_Offset_Init()--1.�������Ǿ�ֹ��ʱ�򣬻������Ʈ����ô���ǽ���ֹʱ�̵���Ʈֵ���о�ֵ�˲����ɼ�һǧ���ۼ�֮������ƽ��ֵ���õ���Ʈ��ƽ��ֵ���ں����Ĵ����м�ȥ��Ʈƽ��ֵ����
//IMU_GetValues()-----2�������жԼ��ٶȼƽ���һ�׵�ͨ�˲�����ʵ��û���õ����ٶȼ�/�����Ǽ�ȥ��Ʈƽ��ֵ��ת��Ϊ������
//IMU_YAW_integral()--3.��ת��Ϊ�����ƺ�Ľ��ٶ�����֣�Ȼ������GPS������Ļ�����ת�Ƕ��ƺ������ۻ������ڶ�ʱ���2ms�ж�һ�Σ�IMUˢ��Ƶ��200HZ,5msһ��

gyro_param_t Gyro_Offset;//��������Ʈ�ṹ��
IMU_param_t  IMU_Data;   //��ȥ��Ʈ���ݽṹ��

float gyro_Offset_flag;
float Daty_X=0;//���ֺ�ĽǶ�
float Daty_Y=0;
float Daty_Z=0;
float IMU_Daty_Z=0;
float GPS_Daty_Z=0;
float GPS_IMU_Daty_Z=0;//���������˲�������ĺ����
float X;
float T_M=0;
float T_N=0;
float C_Direction_2=0;
int    direction_count;//GPS�ۼ�ѭ����
int    Flag=0;
int    GL_IMU_Flag=0;
double gps_direction_average;//gps������ƽ��ֵ
double gps_direction_sum;//gpsƫ�����ۼ�ֵ
/**
 * @brief ��������Ư��ʼ��
 * ͨ���ɼ�һ���������ֵ�������������ƫ��ֵ��
 * ���� �����Ƕ�ȡ������ - ��Ʈֵ������ȥ�����ƫ������
 */
float IMU_gyro_Offset_Init()
{
    Gyro_Offset.Xdata = 0;
    Gyro_Offset.Ydata = 0;
    Gyro_Offset.Zdata = 0;
    for (uint16_t i = 0; i < 1000; i++)
    {
//        Gyro_Offset.Xdata += imu660ra_gyro_x;
//        Gyro_Offset.Ydata += imu660ra_gyro_y;
//        Gyro_Offset.Zdata += imu660ra_gyro_z;

        Gyro_Offset.Xdata += imu963ra_gyro_x;
        Gyro_Offset.Ydata += imu963ra_gyro_y;
        Gyro_Offset.Zdata += imu963ra_gyro_z;
        system_delay_ms(5);   // ��� 1Khz
    }

    Gyro_Offset.Xdata /= 1000;
    Gyro_Offset.Ydata /= 1000;
    Gyro_Offset.Zdata /= 1000;

    return gyro_Offset_flag=1;
}

/**
 * @brief ���ɼ�����ֵת��Ϊʵ������ֵ, ���������ǽ���ȥ��Ư����
 * ���ٶȼƳ�ʼ������ -> ������Χ: ��8g        ��Ӧ������: 4096 LSB/g
 * �����ǳ�ʼ������   -> ������Χ: ��2000 dps  ��Ӧ������: 16.4 LSB/dps   (degree per second)
 * @tips: gyro = (gyro_val / 16.4) ��/s = ((gyro_val / 16.4) * PI / 180) rad/s
 */
void IMU_GetValues()//���ɼ�����ֵת��Ϊʵ������ֵ, ���������ǽ���ȥ��Ư����
{
//2000dps:IMU660--16.4
//2000dps:IMU963--14.3

    //! �����ǽ��ٶȱ���ת��Ϊ�����ƽ��ٶ�: deg/s -> rad/s
    IMU_Data.gyro_x = ((float) imu660ra_gyro_x - Gyro_Offset.Xdata) * PI / 180 / 16.4f;
    IMU_Data.gyro_y = ((float) imu660ra_gyro_y - Gyro_Offset.Ydata) * PI / 180 / 16.4f;
    IMU_Data.gyro_z = ((float) imu660ra_gyro_z - Gyro_Offset.Zdata) * PI / 180 / 16.4f;

//        IMU_Data.gyro_x = ((float) imu963ra_gyro_x - Gyro_Offset.Xdata) * PI / 180 / 14.3f;
//        IMU_Data.gyro_y = ((float) imu963ra_gyro_y - Gyro_Offset.Ydata) * PI / 180 / 14.3f;
//        IMU_Data.gyro_z = ((float) imu963ra_gyro_z - Gyro_Offset.Zdata) * PI / 180 / 14.3f;

}


void IMU_YAW_integral()//�Խ��ٶȽ��л���
{


        IMU_GetValues();

    //    Daty_X+=RAD_TO_ANGLE(IMU_Data.gyro_x*0.005);
    //    Daty_Y+=RAD_TO_ANGLE(IMU_Data.gyro_y*0.005);
    //    if(IMU_Data.gyro_z<0.0045&&IMU_Data.gyro_z>-0.0045)
        if(IMU_Data.gyro_z<0.015&&IMU_Data.gyro_z>-0.015)//�˲�
        {
            Daty_Z-=0;

        }
        else
        {
             IMU_Handle_180();//�滮Ϊ0-180��0-(-180)   Daty_Z
             IMU_Handle_360();//�滮Ϊ0-360��0-(-360)   T_M
             IMU_Handle_0();  //�滮Ϊ0-�������0-������  T_N
         }



}


void IMU_init()//IMU��ʼ��
{

    imu660ra_init();   //IMU660�ߵ���ʼ��
//    imu963ra_init();   //IMU660�ߵ���ʼ��
    pit_ms_init(PIT_CH2, 5);                              // (IMU)��ʼ�� CCU60_CH0 Ϊ�����ж� 5ms ����,IMU660Ƶ��Ϊ200HZ
    IMU_gyro_Offset_Init();// ��������Ư��ʼ��

}

void IMU_SHOW()
{
//    static int ZT_FLAG=0;

//    ips200_show_string(0, 16*0, "Pitch:");    ips200_show_float(60,16*0,Daty_Y,3,6);
//    ips200_show_string(0, 16*1, "Roll:");     ips200_show_float(60,16*1,Daty_X,3,6);
    ips200_show_string(0, 16*0, "Yaw:");      ips200_show_float(60,16*0,Daty_Z,3,6);
    ips200_show_string(0, 16*1, "T_M:");      ips200_show_float(60,16*1,T_M,3,6);
    ips200_show_string(0, 16*2, "T_N:");      ips200_show_float(60,16*2,T_N,3,6);

}

void IMU_TEXT()
{
    HIP4082_Motor_ctrl(5000);

}

void GPS_direction_average()//����GPSƫ����ƽ��ֵ
{
    if(1)
    {
        if(direction_count <= 20)
        {
            gps_direction_sum += gnss.direction;
            direction_count++;
        }
        gps_direction_average=(double)(gps_direction_sum/direction_count);
    }
}
void GPS_IMU_Complementary_filter()//��GPS������direction(�����)��IMU������YAW(�����)���л����˲�
{
    if(gnss.direction>180)    //��ȡ��GPS��λ����Ϣ
    {
        GPS_Daty_Z=gnss.direction-360;
    }
    else
    {
        GPS_Daty_Z=gnss.direction;
    }


    if(encoder>100)
    {
         GPS_IMU_Daty_Z=0.8*IMU_Daty_Z+0.2*GPS_Daty_Z;//�����˲�
         Flag=1;
         Daty_Z=GPS_IMU_Daty_Z;
    }
    else
    {
        if(Flag==1)
        {
            IMU_Daty_Z=GPS_IMU_Daty_Z;
            Flag=0;
        }

        IMU_Daty_Z-=RAD_TO_ANGLE(IMU_Data.gyro_z*0.005);
        if(IMU_Daty_Z>360||IMU_Daty_Z<-360)//����IMU����ֵ��0-360֮��
        {
            IMU_Daty_Z=0;
        }
        Daty_Z=IMU_Daty_Z;

    }

}

void IMU_Handle_180()
{
    Daty_Z-=RAD_TO_ANGLE(IMU_Data.gyro_z*0.005);//(���ֹ���)��������ʱ��Ϊ��,���ڸ�Ϊ˳ʱ��Ϊ��

    if((Daty_Z>0&&Daty_Z<=180)  ||   (Daty_Z<0&&Daty_Z>=(-180)))//˳ʱ��
    {
        Daty_Z= +Daty_Z;
    }
    else if(Daty_Z>180 && Daty_Z<=360)
    {
        Daty_Z-=360;
    }
    else if(Daty_Z<(-180) && Daty_Z>=(-360))
    {
        Daty_Z+=360;
    }

}


void IMU_Handle_360()
{
    T_M-=RAD_TO_ANGLE(IMU_Data.gyro_z*0.005);//(���ֹ���)��������ʱ��Ϊ��,���ڸ�Ϊ˳ʱ��Ϊ��

    if(T_M>360||T_M<-360)//����IMU����ֵ��0-360֮��
    {
        T_M=0;
    }
}

void IMU_Handle_0()
{
    T_N-=RAD_TO_ANGLE(IMU_Data.gyro_z*0.005);//(���ֹ���)��������ʱ��Ϊ��,���ڸ�Ϊ˳ʱ��Ϊ��

}


