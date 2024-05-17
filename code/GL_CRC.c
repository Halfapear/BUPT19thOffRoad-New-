/*
 * GL_CRC.c
 *
 *  Created on: 2023年7月19日
 *      Author: ORRN
 */


#include "zf_common_headfile.h"


int       GL_Point=40;
int       GL_NUM=0;
int       i=0;
double    GPS_GET_LAT[40];
double    GPS_GET_LOT[40];

void GPS_GL_GET()
{

            GPS_GET_LAT[i]=gps_tau1201.latitude;
            GPS_GET_LOT[i]=gps_tau1201.longitude;



            ips_show_string(0, 16*0,"R:");
            ips_show_float(50, 16*0,  GPS_GET_LAT[i], 3, 6);
            ips_show_uint  (180, 16*0, i, 3);
            printf("\r\n缓冲区纬度数据:%f", GPS_GET_LAT[i]);


            ips_show_float(50, 16*1,  GPS_GET_LOT[i], 3, 6);
            ips_show_uint (180, 16*1, i, 3);
            printf("\r\n缓冲区经度数据:%f",GPS_GET_LOT[i]);

            i++;
            GL_NUM++;




              if(GL_NUM>40)                                                               //如果采集点次数大于规定次数的最大值，则让他等于1
                  {
                  GL_NUM=1;
                  }
              ips_show_uint(200, 0, GL_NUM, 3);                                              //显示已采集的点数



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

                  ips_show_string(0, 16*3,"T:");//浮点实时点位
                  ips_show_float(50, 16*4,  gps_tau1201.latitude, 3, 6);
                  ips_show_float(50, 16*5,  gps_tau1201.longitude, 3, 6);

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


