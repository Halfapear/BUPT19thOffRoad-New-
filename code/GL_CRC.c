/*
 * GL_CRC.c
 *
 *  Created on: 2023年7月19日
 *      Author: ORRN
 */
//5.14  核心功能是通过按键触发采集GPS数据，并实时显示和存储这些数据。GPS_GL_GET函数负责从gnss结构体中读取GPS数据并存储，同时在屏幕上显示这些数据。而GL_CRC函数则处理按键触发的各种操作，包括数据采集、实时显示、数据打印和屏幕清除等
//https://chat.openai.com/share/5f080321-ec01-4a9c-8aeb-02d6788a50c0 
//根据下面代码或上面gpt解释会用这些按键（连接着GPS的）
//解析一律写在最上头 随行的我会另起一行
   
   
   
#include "zf_common_headfile.h"


int       GL_Point=40;
int       GL_NUM=0;
int       i=0;
double    GPS_GET_LAT[40];
double    GPS_GET_LOT[40];

void GPS_GL_GET()
{

            GPS_GET_LAT[i]=gnss.latitude;
            GPS_GET_LOT[i]=gnss.longitude;



            ips200_show_string(0, 16*0,"R:");
            ips200_show_float(50, 16*0,  GPS_GET_LAT[i], 3, 6);
            ips200_show_uint  (180, 16*0, i, 3);
            printf("\r\n缓冲区纬度数据:%f", GPS_GET_LAT[i]);


            ips200_show_float(50, 16*1,  GPS_GET_LOT[i], 3, 6);
            ips200_show_uint (180, 16*1, i, 3);
            printf("\r\n缓冲区经度数据:%f",GPS_GET_LOT[i]);

            i++;
            GL_NUM++;




              if(GL_NUM>40)                                                               //如果采集点次数大于规定次数的最大值，则让他等于1
                  {
                  GL_NUM=1;
                  }
              ips200_show_uint(200, 0, GL_NUM, 3);                                              //显示已采集的点数



    }



void GL_CRC()
{

        if(key1_flag)
         {
                while(key1_flag)//采集点位
              {

                      if(key2_flag)
                      {
                        key2_flag=0;
                        GPS_GL_GET();
                      }

                  ips200_show_string(0, 16*3,"T:");//浮点实时点位
                  ips200_show_float(50, 16*4,  gnss.latitude, 3, 6);
                  ips200_show_float(50, 16*5,  gnss.longitude, 3, 6);

                if(key3_flag)
                  {
                    key3_flag=0;
                    key1_flag=0;
                    ips114_clear();
                  }

              }
         }

        if(key2_flag)
         {
                while(key2_flag)//采集点位
              {

                      if(key3_flag)
                      {
                        key3_flag=0;
                        for(int NUM=0;NUM<20;NUM++)
                         {
                             printf("\r\n组数-%d,工作数组纬度数据-%f,工作数组经度数据-%f",NUM+1,GPS_GET_LAT[NUM],GPS_GET_LOT[NUM]);
                             system_delay_ms(50);
                         }
                      }

                if(key4_flag)
                  {
                    key4_flag=0;
                    key2_flag=0;
                    ips114_clear();
                    system_delay_ms(5000);
                    GL_IMU_Flag=1;
                  }

              }
        }






}


