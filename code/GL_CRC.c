/*
 * GL_CRC.c
 *
 *  Created on: 2023��7��19��
 *      Author: ORRN
 */
//5.14  ���Ĺ�����ͨ�����������ɼ�GPS���ݣ���ʵʱ��ʾ�ʹ洢��Щ���ݡ�GPS_GL_GET���������gnss�ṹ���ж�ȡGPS���ݲ��洢��ͬʱ����Ļ����ʾ��Щ���ݡ���GL_CRC���������������ĸ��ֲ������������ݲɼ���ʵʱ��ʾ�����ݴ�ӡ����Ļ�����
//https://chat.openai.com/share/5f080321-ec01-4a9c-8aeb-02d6788a50c0 
//����������������gpt���ͻ�����Щ������������GPS�ģ�
//����һ��д������ͷ ���е��һ�����һ��
   
   
   
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
            printf("\r\n������γ������:%f", GPS_GET_LAT[i]);


            ips200_show_float(50, 16*1,  GPS_GET_LOT[i], 3, 6);
            ips200_show_uint (180, 16*1, i, 3);
            printf("\r\n��������������:%f",GPS_GET_LOT[i]);

            i++;
            GL_NUM++;




              if(GL_NUM>40)                                                               //����ɼ���������ڹ涨���������ֵ������������1
                  {
                  GL_NUM=1;
                  }
              ips200_show_uint(200, 0, GL_NUM, 3);                                              //��ʾ�Ѳɼ��ĵ���



    }



void GL_CRC()
{

        if(key1_flag)
         {
                while(key1_flag)//�ɼ���λ
              {

                      if(key2_flag)
                      {
                        key2_flag=0;
                        GPS_GL_GET();
                      }

                  ips200_show_string(0, 16*3,"T:");//����ʵʱ��λ
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
                while(key2_flag)//�ɼ���λ
              {

                      if(key3_flag)
                      {
                        key3_flag=0;
                        for(int NUM=0;NUM<20;NUM++)
                         {
                             printf("\r\n����-%d,��������γ������-%f,�������龭������-%f",NUM+1,GPS_GET_LAT[NUM],GPS_GET_LOT[NUM]);
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


