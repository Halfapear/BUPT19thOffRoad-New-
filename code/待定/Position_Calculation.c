/*
 * Position_Calculation.c
 *
 *  Created on: 2023年3月18日
 *      Author: ORRN
 */

#include "zf_common_headfile.h"



int Control_FLAG=1;         //该标志位意义是，当它为1时是无控模式(靠GPS+IMU导航)/当手柄上的按键按下时，该标志位被清0,无法执行无控模式，转入有控模式
int S_Point=0;              //停止点位参数
int P_Distance=0;           //点位距离参数
int next_point=0;           //下一个目标点
int Point_interval=0;       //点位区间数
int ZT_FLAG=0;              //绕锥桶一周标志位
int Distance_FLAG=0;        //距离切换标志位
int Angle_FLAG=0;           //角度切换标志位
int IMU_suppression_FLAG=1; //IMU抑制开启标志位
int16 SPEED_Value=0;        //目标速度




double distance=0;    //两点的距离
double azimuth=0;     //方位角(得出的函数返回值为double)
double last_azimuth=0;//上一次的方位角
double Error=0;       //方位角和航向角的误差(得出的函数返回值为double)

float  PD_YAW=0;

void Follow_Track()//核心循迹程序
{

#if (Float_Record_FLAG||Double_Record_FLAG)//单精度和双精度存储都用这个

    distance=get_two_points_distance (gps_tau1201.latitude, gps_tau1201.longitude,Work_target_array[0][next_point], Work_target_array[1][next_point]);//距离
    last_azimuth=get_two_points_azimuth (gps_tau1201.latitude, gps_tau1201.longitude,Work_target_array[0][next_point-1], Work_target_array[1][next_point-1]);//方位角 (两点连线与正北的夹角)

    azimuth =get_two_points_azimuth (gps_tau1201.latitude, gps_tau1201.longitude,Work_target_array[0][next_point], Work_target_array[1][next_point]);//方位角 (两点连线与正北的夹角)

#endif

//#if (Int_Record_FLAG)//整数
//
//    distance=get_two_points_distance (gps_tau1201.latitude*1000000, gps_tau1201.longitude*1000000,Work_target_array[0][next_point], Work_target_array[1][next_point]);//距离
//    azimuth =get_two_points_azimuth (gps_tau1201.latitude*1000000, gps_tau1201.longitude*1000000,Work_target_array[0][next_point], Work_target_array[1][next_point]);//方位角 (两点连线与正北的夹角)
////    distance=get_two_points_distance (gps_tau1201.latitude, gps_tau1201.longitude,Work_target_array[0][next_point]/1000000, Work_target_array[1][next_point]/1000000);//距离
////    azimuth =get_two_points_azimuth (gps_tau1201.latitude, gps_tau1201.longitude,Work_target_array[0][next_point]/1000000, Work_target_array[1][next_point]/1000000);//方位角 (两点连线与正北的夹角)
////    if(azimuth>357||azimuth<3)//以后可能有大用，先留着
////    {
////        azimuth=0;
////    }
//
//#endif

#if (Array_Record_FLAG)

    distance=get_two_points_distance (gps_tau1201.latitude, gps_tau1201.longitude,GPS_GET_LAT[next_point], GPS_GET_LOT[next_point]);//距离
    azimuth =get_two_points_azimuth (gps_tau1201.latitude, gps_tau1201.longitude,GPS_GET_LAT[next_point],  GPS_GET_LOT[next_point]);//方位角 (两点连线与正北的夹角)


#endif

    //数据融合的处理
    //把IMU规划为顺时针0-(180),逆时针0-(-180)
    //把GPS规划为正半轴0-(180),负半轴0-(-180)
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
                Switching_point_D();                             //距离判据切点


                if(next_point==1)
                {
                    Retardation(50,40,-400);//反推制动
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
//                    Retardation(60,40,-300);//反推制动
                    SPEED_Value=100;

                }
                else if(next_point==8)
                {
                    gpio_set_level(BUZZER_PIN,1);
                    Task_RZT (67,70,0,'N');                                         //打角 速度
                    SPEED_Value=130;
                }
                else if(next_point==9)
                {
//                    Retardation(80,50,-100);//反推制动
                    SPEED_Value=60;
                }
                else if(next_point==10)//尽头/////////////////////////////////////////
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
//                    Retardation(60,50,-500);//反推制动;
                }
                else if(next_point==13)
                {
//                    STOP();
                    SPEED_Value=100;
                }
                else if(next_point==14)  //坡道后
                {
                    SPEED_Value=100;//250
//                    STOP();
                }
                else if(next_point==15)//停车
                {
                    STOP();
                }
                else                                                              //默认
                {
                    SPEED_Value=100;//250
                }


}
//////////////////////////////////////////////////////////////////////////子任务代码////////////////////////////////////////////////////////////////////
void Task_RZT(int Steer,int16 Motor,int Angle,char Type)//任务:绕锥桶一圈
{

    if(Angle==0&&Type=='N')
    {
        if(ZT_FLAG==0)
        {

                   do{
                          Control_FLAG=0;     //给0隔绝GPS影响
                          Steer_set(Steer);   //绕桶半径大小是由打死角度决定的
                          SPEED_Value=Motor;//闭环
                                if(T_M<(-358))
                                {
                                Control_FLAG=1;//重新赋值，打开GPS影响
                                ZT_FLAG=1;
                                }
                              gpio_set_level(BUZZER_PIN,1);
                   }while(ZT_FLAG==0); //do_while结构:先执行一次，然后判断条件是否为真，为真则继续循环，为否则跳出循环
                              gpio_set_level(BUZZER_PIN,0);
        }
    }
    else if(Angle==0&&Type=='S')
       {
           if(ZT_FLAG==0)
                  {

                             do{
                                    Control_FLAG=0;     //给0隔绝GPS影响
                                    Steer_set(Steer);   //绕桶半径大小是由打死角度决定的
                                    SPEED_Value=Motor;//闭环

                                        if(T_N>(358))
                                        {
                                            Control_FLAG=1;//重新赋值，打开GPS影响
                                            ZT_FLAG=1;
                                        }
                                        gpio_set_level(BUZZER_PIN,1);
                             }while(ZT_FLAG==0); //do_while结构:先执行一次，然后判断条件是否为真，为真则继续循环，为否则跳出循环
                                        gpio_set_level(BUZZER_PIN,0);
                  }
       }
    else if(Angle==180&&Type=='N')
    {
        if(ZT_FLAG==0)
               {

                          do{
                                 Control_FLAG=0;     //给0隔绝GPS影响
                                 Steer_set(Steer);   //绕桶半径大小是由打死角度决定的
                                 SPEED_Value=Motor;//闭环

                                     if(T_N<(-540))
                                     {
                                         Control_FLAG=1;//重新赋值，打开GPS影响
                                         ZT_FLAG=1;
                                     }
                                     gpio_set_level(BUZZER_PIN,1);
                          }while(ZT_FLAG==0); //do_while结构:先执行一次，然后判断条件是否为真，为真则继续循环，为否则跳出循环
                                     gpio_set_level(BUZZER_PIN,0);
               }
    }
}



void Switching_point_D()//切点(距离)判断
{



                if(next_point==0||next_point<1||next_point==12)
                {
                    if(distance<2)//切点
                    {
                          next_point++;
    //                      Buzzer_check(50,50);                                  // 自检完成
                        printf("\r\n 3-当前点位-%d",next_point);
                        printf("\r\n \\\\\\\\\\\\\\\\\\\\");
                    }
                }
                else if(next_point==8||next_point==9||next_point==10)
                {
                    if(distance<2)//切点
                    {
                          next_point++;
    //                      Buzzer_check(50,50);                                  // 自检完成
                        printf("\r\n 0.75-当前点位-%d",next_point);
                        printf("\r\n \\\\\\\\\\\\\\\\\\\\");
                    }
                }
              else
              {
                   if(distance<0.75)//切点
                    {
                          next_point++;
    //                      Buzzer_check(50,50);                                  // 自检完成
                        printf("\r\n 2-当前点位-%d",next_point);
                        printf("\r\n \\\\\\\\\\\\\\\\\\\\");
                    }
              }



}


void Double_layer_feedback_0()//双层反馈(用于去的时候是0度)
{
    //向北发车
            if(azimuth>180)  //将大于180的方向角变为负角
               {
                   azimuth-=360;
               }
      //向南发车
             azimuth+=180;
             if(azimuth>180)  //将大于180的方向角变为负角
                 {
                     azimuth-=360;
                 }

        if((azimuth - Daty_Z)>180)      //当两角度之差大于180度时，则将差值减去360
        {
             Error=azimuth-PidLocCtrl(&PID_IMU,Daty_Z)-360;//GPS的方向角-IMU经过PD处理的航向角
             printf("IMU(0).azimuth:%f,Daty_Z:%f,Error:%f\n",azimuth,Daty_Z,Error);//如果点位在正北，那么方向角未经过处理的值是0


        }
        else if((azimuth - Daty_Z)<-180)//当两角度之差小于-180度时，则将差值加上360
        {
             Error=azimuth-PidLocCtrl(&PID_IMU,Daty_Z)+360;//GPS的方向角-IMU经过PD处理的航向角
             printf("IMU(0).azimuth:%f,Daty_Z:%f,Error:%f\n",azimuth,Daty_Z,Error);//如果点位在正北，那么方向角未经过处理的值是0

        }
        else
        {
              Error=azimuth-PidLocCtrl(&PID_IMU,Daty_Z);//GPS的方向角-IMU经过PD处理的航向角
              printf("IMU(0).azimuth:%f,Daty_Z:%f,Error:%f\n",azimuth,Daty_Z,Error);//如果点位在正北，那么方向角未经过处理的值是0

        }

}

void Double_layer_feedback_180()//双层反馈(用于返回的时候是180度)
{
    if(azimuth>180)  //将大于180的方向角变为负角
        {
            azimuth-=360;
        }


        if((azimuth - Daty_Z)>180)      //当两角度之差大于180度时，则将差值减去360
        {
             Error=azimuth-PidLocCtrl(&PID_IMU,Daty_Z)-360;//GPS的方向角-IMU经过PD处理的航向角
             printf("IMU(180).azimuth:%f,Daty_Z:%f,Error:%f\n",azimuth,Daty_Z,Error);//如果点位在正北，那么方向角未经过处理的值是0


        }
        else if((azimuth - Daty_Z)<-180)//当两角度之差小于-180度时，则将差值加上360
        {
             Error=azimuth-PidLocCtrl(&PID_IMU,Daty_Z)+360;//GPS的方向角-IMU经过PD处理的航向角
             printf("IMU(180).azimuth:%f,Daty_Z:%f,Error:%f\n",azimuth,Daty_Z,Error);//如果点位在正北，那么方向角未经过处理的值是0

        }
        else
        {
              Error=azimuth-PidLocCtrl(&PID_IMU,Daty_Z);//GPS的方向角-IMU经过PD处理的航向角
              printf("IMU(180).azimuth:%f,Daty_Z:%f,Error:%f\n",azimuth,Daty_Z,Error);//如果点位在正北，那么方向角未经过处理的值是0

        }

}

void Single_layer_feedback()//单层反馈
{
    //向北发车
            if(azimuth>180)  //将大于180的方向角变为负角
               {
                   azimuth-=360;
               }
      //向南发车
             azimuth+=180;
             if(azimuth>180)  //将大于180的方向角变为负角
                 {
                     azimuth-=360;
                 }
        if((azimuth - Daty_Z)>180)      //当两角度之差大于180度时，则将差值减去360
        {
            Error = azimuth-Daty_Z-360;
             printf("GPS.azimuth:%f,Daty_Z:%f,Error:%f\n",azimuth,Daty_Z,Error);//如果点位在正北，那么方向角未经过处理的值是0


        }
        else if((azimuth - Daty_Z)<-180)//当两角度之差小于-180度时，则将差值加上360
        {
            Error = azimuth-Daty_Z+360;
             printf("GPS.azimuth:%f,Daty_Z:%f,Error:%f\n",azimuth,Daty_Z,Error);//如果点位在正北，那么方向角未经过处理的值是0

        }
        else
        {
            Error = azimuth-Daty_Z;
              printf("GPS.azimuth:%f,Daty_Z:%f,Error:%f\n",azimuth,Daty_Z,Error);//如果点位在正北，那么方向角未经过处理的值是0
        }

}

void IMU_Navigation_0()//0度IMU导航
{
    Error=T_M;
}

void IMU_Navigation_180_N()//180度IMU导航
{
    Error=(180+T_M);
}

void IMU_Navigation_180_S()//180度IMU导航
{
    Error=(-180+T_M);
}
void Retardation(int X,int16 Y,int16 Z)//反推制动
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

void STOP()//停车
{
    if(encoder<10)
                        {
                            while(1)
                            {
                                SPEED_Value=0;                                  //任务完成，停车！！！
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
        IMU_Navigation_0();      //IMU--0度导航
    }
    else if(TYPE==1)
    {
        IMU_Navigation_180_N();  //逆时针IMU--180度导航(左转)
    }
    else if(TYPE==2)
    {
        IMU_Navigation_180_S();  //顺时针IMU--180度导航(右转)
    }
    else if(TYPE==3)
    {
        Single_layer_feedback(); //单层反馈----没有IMU抑制转向效果
    }
    else if(TYPE==4)
    {
        Double_layer_feedback_0();//双层反馈(用于去的时候是0度)
    }

}
