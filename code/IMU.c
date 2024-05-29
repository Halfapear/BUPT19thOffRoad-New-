/*
 * IMU.c
 *
 *  Created on: 2023年3月18日
 *      Author: ORRN
 */
//5.14 在IMU中我们可以先试着单独运行起来，然后和GPS结合（要结合吗，我还不太清楚）
//IMU的单纯用库测量简直不用看，就三个函数

#include "zf_common_headfile.h"


//gyro_Offset_Init()--1.当陀螺仪静止的时候，会产生零飘，那么我们将静止时刻的零飘值进行均值滤波，采集一千次累加之后求其平均值，得到零飘的平均值，在后续的处理中减去零飘平均值即可
//IMU_GetValues()-----2，函数中对加速度计进行一阶低通滤波，但实际没有用到加速度计/陀螺仪减去零飘平均值并转换为弧度制
//IMU_YAW_integral()--3.对转换为弧度制后的角速度求积分，然后利用GPS函数里的弧度制转角度制函数并累积，放在定时器里，2ms中断一次，IMU刷新频率200HZ,5ms一次

gyro_param_t Gyro_Offset;//陀螺仪零飘结构体
IMU_param_t  IMU_Data;   //减去零飘数据结构体

float gyro_Offset_flag;
float Daty_X=0;//积分后的角度
float Daty_Y=0;
float Daty_Z=0;
float IMU_Daty_Z=0;
float GPS_Daty_Z=0;
float GPS_IMU_Daty_Z=0;//经过互补滤波计算出的航向角
float X;
float T_M=0;
float T_N=0;
float C_Direction_2=0;
int    direction_count;//GPS累加循环数
int    Flag=0;
int    GL_IMU_Flag=0;
double gps_direction_average;//gps计算后的平均值
double gps_direction_sum;//gps偏航角累加值
/**
 * @brief 陀螺仪零漂初始化
 * 通过采集一定数据求均值计算陀螺仪零点偏移值。
 * 后续 陀螺仪读取的数据 - 零飘值，即可去除零点偏移量。
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
        system_delay_ms(5);   // 最大 1Khz
    }

    Gyro_Offset.Xdata /= 1000;
    Gyro_Offset.Ydata /= 1000;
    Gyro_Offset.Zdata /= 1000;

    return gyro_Offset_flag=1;
}

/**
 * @brief 将采集的数值转化为实际物理值, 并对陀螺仪进行去零漂处理
 * 加速度计初始化配置 -> 测量范围: ±8g        对应灵敏度: 4096 LSB/g
 * 陀螺仪初始化配置   -> 测量范围: ±2000 dps  对应灵敏度: 16.4 LSB/dps   (degree per second)
 * @tips: gyro = (gyro_val / 16.4) °/s = ((gyro_val / 16.4) * PI / 180) rad/s
 */
void IMU_GetValues()//将采集的数值转化为实际物理值, 并对陀螺仪进行去零漂处理
{
//2000dps:IMU660--16.4
//2000dps:IMU963--14.3

    //! 陀螺仪角速度必须转换为弧度制角速度: deg/s -> rad/s
    IMU_Data.gyro_x = ((float) imu660ra_gyro_x - Gyro_Offset.Xdata) * PI / 180 / 16.4f;
    IMU_Data.gyro_y = ((float) imu660ra_gyro_y - Gyro_Offset.Ydata) * PI / 180 / 16.4f;
    IMU_Data.gyro_z = ((float) imu660ra_gyro_z - Gyro_Offset.Zdata) * PI / 180 / 16.4f;

//        IMU_Data.gyro_x = ((float) imu963ra_gyro_x - Gyro_Offset.Xdata) * PI / 180 / 14.3f;
//        IMU_Data.gyro_y = ((float) imu963ra_gyro_y - Gyro_Offset.Ydata) * PI / 180 / 14.3f;
//        IMU_Data.gyro_z = ((float) imu963ra_gyro_z - Gyro_Offset.Zdata) * PI / 180 / 14.3f;

}


void IMU_YAW_integral()//对角速度进行积分
{


        IMU_GetValues();

    //    Daty_X+=RAD_TO_ANGLE(IMU_Data.gyro_x*0.005);
    //    Daty_Y+=RAD_TO_ANGLE(IMU_Data.gyro_y*0.005);
    //    if(IMU_Data.gyro_z<0.0045&&IMU_Data.gyro_z>-0.0045)
        if(IMU_Data.gyro_z<0.015&&IMU_Data.gyro_z>-0.015)//滤波
        {
            Daty_Z-=0;

        }
        else
        {
             IMU_Handle_180();//规划为0-180和0-(-180)   Daty_Z
             IMU_Handle_360();//规划为0-360和0-(-360)   T_M
             IMU_Handle_0();  //规划为0-正无穷和0-负无穷  T_N
         }



}


void IMU_init()//IMU初始化
{

    imu660ra_init();   //IMU660惯导初始化
//    imu963ra_init();   //IMU660惯导初始化
    pit_ms_init(PIT_CH2, 5);                              // (IMU)初始化 CCU60_CH0 为周期中断 5ms 周期,IMU660频率为200HZ
    IMU_gyro_Offset_Init();// 陀螺仪零漂初始化

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

void GPS_direction_average()//计算GPS偏航角平均值
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
void GPS_IMU_Complementary_filter()//将GPS反馈的direction(航向角)和IMU反馈的YAW(航向角)进行互补滤波
{
    if(gnss.direction>180)    //获取到GPS方位角信息
    {
        GPS_Daty_Z=gnss.direction-360;
    }
    else
    {
        GPS_Daty_Z=gnss.direction;
    }


    if(encoder>100)
    {
         GPS_IMU_Daty_Z=0.8*IMU_Daty_Z+0.2*GPS_Daty_Z;//互补滤波
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
        if(IMU_Daty_Z>360||IMU_Daty_Z<-360)//限制IMU的数值在0-360之间
        {
            IMU_Daty_Z=0;
        }
        Daty_Z=IMU_Daty_Z;

    }

}

void IMU_Handle_180()
{
    Daty_Z-=RAD_TO_ANGLE(IMU_Data.gyro_z*0.005);//(积分过程)本来是逆时针为正,现在改为顺时针为正

    if((Daty_Z>0&&Daty_Z<=180)  ||   (Daty_Z<0&&Daty_Z>=(-180)))//顺时针
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
    T_M-=RAD_TO_ANGLE(IMU_Data.gyro_z*0.005);//(积分过程)本来是逆时针为正,现在改为顺时针为正

    if(T_M>360||T_M<-360)//限制IMU的数值在0-360之间
    {
        T_M=0;
    }
}

void IMU_Handle_0()
{
    T_N-=RAD_TO_ANGLE(IMU_Data.gyro_z*0.005);//(积分过程)本来是逆时针为正,现在改为顺时针为正

}


