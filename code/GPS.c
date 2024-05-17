/*
 * GPS.c
 *
 *  Created on: 2023��3��18��
 *      Author: ORRN
 */

#include "zf_common_headfile.h"



int          Number=0;//���ʱ���


///////////////////////////////////////////////////////////////////////////�����ȸ���洢/////////////////////////////////////////////////////////////////////////////////

#if  Float_Record_FLAG

float   Target_point[2][150];//���ڴ���ɼ���Ŀ�����Ϣ�����ں�����λ�ü���(��һ����γ�ȣ��ڶ����Ǿ���)
float   Work_target_array[2][150];//���ڽ���flash�ж�ȡ��Ŀ�����������ݸ��ƹ�������������鱻��ֵʱ��������һ�к�Flash���޹�ϵ
double  lat; //γ��(Ӣ�����FLASHֻ�ܴ�32λ���ݣ�������double���Ͷ���ı����浽������ֻ��ת��float)
double  lot; //����(Ӣ�����FLASHֻ�ܴ�32λ���ݣ�������double���Ͷ���ı����浽������ֻ��ת��float)

void GPS_Record_flash()    //���ɼ��ĵ�λ��¼����������������GPS_FLASH
{
    static int NUM=0;       //�ɼ���Ĵ���

    {
        lat=gps_tau1201.latitude;   //γ��
        lot=gps_tau1201.longitude;  //����

        flash_union_buffer[Number].float_type=lat;//��ά������ǿ��ת���󴢴���FLASH ���������ݻ�����  (������Ϊż��ʱ��γ������)
        ips_show_string(0, 16*0,"R:");
        ips_show_float(50, 16*0,  flash_union_buffer[Number].float_type, 3, 6);
        ips_show_uint(180, 16*0, Number, 3);
        printf("\r\n������γ������:%f", flash_union_buffer[Number].float_type);

        Number++;//�����±�+1,�л����澭��

        flash_union_buffer[Number].float_type=lot;//����������ǿ��ת���󴢴���FLASH ���������ݻ�����  (������Ϊ����ʱ�Ǿ�������)
        ips_show_float(50, 16*1,  flash_union_buffer[Number].float_type, 3, 6);
        ips_show_uint(180, 16*1, Number, 3);
        printf("\r\n��������������:%f", flash_union_buffer[Number].float_type);

        Number++;//�����±�+1,�л�����γ��
                                                                      //�����û���ֶ����GPS_FLASH����������������Զ����GPS_FLASH������������:ÿ�βɼ���λ֮ǰ�����GPS_FLASH
        if(flash_check(FLASH_SECTION_INDEX, GPS_PAGE_INDEX))                      //�ж�Flash�Ƿ������� : �����ݷ���1�������ݷ���0 (�ú���ÿ������ִ��һ�Σ���ִ�б�����ʱ,GPS_FLASHû�ж��������Բ���Ҳ�޷�)                                                               //
            {
                flash_erase_page(FLASH_SECTION_INDEX, GPS_PAGE_INDEX);                //����Flash����
            }

        flash_write_page_from_buffer(FLASH_SECTION_INDEX, GPS_PAGE_INDEX);        //��������������д�뵽ָ��Flash ������ҳ��
    }

    NUM++;

    if(NUM>RP_MAX)                                                               //����ɼ���������ڹ涨���������ֵ������������1
        {
          NUM=1;
        }

    ips_show_uint(200, 0, NUM, 3);                                              //��ʾ�Ѳɼ��ĵ���
}



void GPS_Flash_use()//��GPS_FLASH���������¶��ػ���������ֵ������
{
    flash_buffer_clear();//��ջ�����

    if(flash_check(FLASH_SECTION_INDEX, GPS_PAGE_INDEX))                      //�ж�Flash�Ƿ������� : �����ݷ���1�������ݷ���0
    {
        flash_read_page_to_buffer(FLASH_SECTION_INDEX, GPS_PAGE_INDEX);       //�����ݴ�FLASHָ������ҳ����뵽������

        int TG=0;//�����л���ά�����±�

        for(int data=0;data<RP_MAX;data++)
                  {

                      Target_point[0][TG]=flash_union_buffer[data].float_type; //γ           //�����������ݾ���ǿ��ת��������ż��˳��ŵ�����������������

      //                printf("\r\n��FLASH��ȡ�Ļ�����γ������:%f", flash_union_buffer[data].float_type);
      //                printf("\r\n��FLASH��ȡ��Ŀ������γ������:%f", Target_point[0][tg]);

                      data++;

                      Target_point[1][TG]=flash_union_buffer[data].float_type; //��
      //                printf("\r\n��FLASH��ȡ�Ļ�������������:%f", flash_union_buffer[data].float_type);
      //                printf("\r\n��FLASH��ȡ��Ŀ�����龭������:%f", Target_point[1][tg]);

                      TG++;//�±�+1

                  }
        printf("\r\n�ɹ���GPS_FLASHȡ������");                                   //�ɼ����
        printf("\r\n");                                                         //����

    }


}
#endif


///////////////////////////////////////////////////////////////////////////˫���Ⱦ��ȸ���洢/////////////////////////////////////////////////////////////////////////////////

#if Double_Record_FLAG

double    Target_point[2][150];//���ڴ���ɼ���Ŀ�����Ϣ�����ں�����λ�ü���(��һ����γ�ȣ��ڶ����Ǿ���)
double    Work_target_array[2][150];//���ڽ���flash�ж�ȡ��Ŀ�����������ݸ��ƹ�������������鱻��ֵʱ��������һ�к�Flash���޹�ϵ
double   lat; //γ��(Ӣ�����FLASHֻ�ܴ�32λ���ݣ�������double���Ͷ���ı����浽������ֻ��ת��float)
double   lot; //����(Ӣ�����FLASHֻ�ܴ�32λ���ݣ�������double���Ͷ���ı����浽������ֻ��ת��float)

uint32   I_lat;//double����γ��������������
float    F_lat;//double����γ������С������(ǿ��ת��Ϊfloat)(������float���Ͷ�������Ϊ����������λ0ʱ��float���Ϳ��Դ浽��7λ)

uint32   I_lot;//double���;���������������
float    F_lot;//double���;�������С������(ǿ��ת��Ϊfloat)

void GPS_Record_flash()    //���ɼ��ĵ�λ��¼����������������GPS_FLASH
{
    static int NUM=0;       //�ɼ���Ĵ���
    {
        lat=gps_tau1201.latitude;   //γ��(ԭʼ������double���ͱ����洢,14λ)
        lot=gps_tau1201.longitude;  //����(ԭʼ������double���ͱ����洢,13λ)

        I_lat=(uint32)(lat);//����ǿת��latȡ���õ���������
        F_lat=lat-I_lat; //��double��������-�������ֵõ�С�����ֲ�ǿ��ת��Ϊfloat����

        I_lot=(uint32)(lot);//����ǿת��lotȡ���õ���������
        F_lot=lot-I_lot; //��double��������-�������ֵõ�С�����ֲ�ǿ��ת��Ϊfloat����

        ips_show_string(0, 16*0,"R:");

        {
            flash_union_buffer[Number].uint32_type=I_lat;  //������ά������ǿ��ת���󴢴���FLASH ���������ݻ�����
            ips_show_uint (50, 16*0, flash_union_buffer[Number].uint32_type, 5);  //��ʾ��������
            ips_show_uint(150, 16*0, Number, 3);//����������λ��

            Number++;

            flash_union_buffer[Number].float_type=F_lat; //��������ά������ǿ��ת���󴢴���FLASH ���������ݻ�����
            ips_show_float(50, 16*1, flash_union_buffer[Number].float_type, 3, 6);//��ʾС������
            ips_show_uint(150, 16*1, Number, 3);//����������λ��

            ips_show_float(50, 16*2, gps_tau1201.latitude, 3, 6);//��ʾ��������

        }

         Number++;//�����±�+1,�л����澭��

        {
            flash_union_buffer[Number].uint32_type=I_lot;  //��������������ǿ��ת���󴢴���FLASH ���������ݻ�����
            ips_show_uint (50, 16*4, flash_union_buffer[Number].uint32_type, 5);  //��ʾ��������
            ips_show_uint(150, 16*4, Number, 3);//����������λ��

            Number++;

            flash_union_buffer[Number].float_type=F_lot; //����������������ǿ��ת���󴢴���FLASH ���������ݻ�����
            ips_show_float(50, 16*5, flash_union_buffer[Number].float_type, 3, 6);//��ʾС������
            ips_show_uint(150, 16*5, Number, 3);//����������λ��

            ips_show_float(50, 16*6, gps_tau1201.longitude, 3, 6);//��ʾ��������
        }


        Number++;//�����±�+1,�л�����γ��

                //�����û���ֶ����GPS_FLASH����������������Զ����GPS_FLASH������������:ÿ�βɼ���λ֮ǰ�����GPS_FLASH
        if(flash_check(FLASH_SECTION_INDEX, GPS_PAGE_INDEX))                      //�ж�Flash�Ƿ������� : �����ݷ���1�������ݷ���0 (�ú���ÿ������ִ��һ�Σ���ִ�б�����ʱ,GPS_FLASHû�ж��������Բ���Ҳ�޷�)                                                               //
        {
          flash_erase_page(FLASH_SECTION_INDEX, GPS_PAGE_INDEX);                //����Flash����
        }

          flash_write_page_from_buffer(FLASH_SECTION_INDEX, GPS_PAGE_INDEX);        //��������������д�뵽ָ��Flash ������ҳ��

    }

    NUM++;

       if(NUM>RP_MAX)                                                               //����ɼ���������ڹ涨���������ֵ������������1
           {
             NUM=1;
           }

       ips_show_uint(210, 0, NUM, 3);                                              //��ʾ�Ѳɼ��ĵ���


}


void GPS_Flash_use()//��GPS_FLASH���������¶��ػ���������ֵ������
{

    flash_buffer_clear();//��ջ�����

    if(flash_check(FLASH_SECTION_INDEX, GPS_PAGE_INDEX))                      //�ж�Flash�Ƿ������� : �����ݷ���1�������ݷ���0
    {
        flash_read_page_to_buffer(FLASH_SECTION_INDEX, GPS_PAGE_INDEX);       //�����ݴ�FLASHָ������ҳ����뵽������

        int TG=0;//�����л���ά�����±�

        for(int data=0;data<200;data++)
          {

            Target_point[0][TG]=flash_union_buffer[data].uint32_type+flash_union_buffer[data+1].float_type; //γ(�����ƺ�Ҫ��double���͵�����,��Ϊ����������������һ��float���͵�С������)

            data=data+2;

            Target_point[1][TG]=flash_union_buffer[data].uint32_type+flash_union_buffer[data+1].float_type; //��

            data=data+1;
            TG++;//�±�+1

          }


    }

}


#endif


///////////////////////////////////////////////////////////////////////////�����洢/////////////////////////////////////////////////////////////////////////////////
#if  Int_Record_FLAG

uint32 Target_point[2][150];//���ڴ���ɼ���Ŀ�����Ϣ�����ں�����λ�ü���(��һ����γ�ȣ��ڶ����Ǿ���)
uint32 Work_target_array[2][150];//���ڽ���flash�ж�ȡ��Ŀ�����������ݸ��ƹ�������������鱻��ֵʱ��������һ�к�Flash���޹�ϵ
uint32 lat; //γ��(Ӣ�����FLASHֻ�ܴ�32λ���ݣ�������double���Ͷ���ı����浽������ֻ��ת��float)
uint32 lot; //����(Ӣ�����FLASHֻ�ܴ�32λ���ݣ�������double���Ͷ���ı����浽������ֻ��ת��float)



void GPS_Record_flash()    //���ɼ��ĵ�λ��¼����������������GPS_FLASH
{

    static int NUM=0;       //�ɼ���Ĵ���

   {
        lat=gps_tau1201.latitude*1000000;   //γ��*100��(GPS�ش�����ֻ�к�6λ��Ч)
        lot=gps_tau1201.longitude*1000000;  //����*100��(GPS�ش�����ֻ�к�6λ��Ч)

        flash_union_buffer[Number].uint32_type=lat;//��ά������ǿ��ת���󴢴���FLASH ���������ݻ�����  (������Ϊż��ʱ��γ������)
        ips_show_string(0, 16*0,"R:");
        ips_show_uint  (50, 16*0,  flash_union_buffer[Number].uint32_type,10);
        ips_show_uint  (180, 16*0, Number, 3);
        printf("\r\n������γ������:%d",(int)flash_union_buffer[Number].uint32_type);

        Number++;//�����±�+1,�л����澭��

        flash_union_buffer[Number].uint32_type=lot;//����������ǿ��ת���󴢴���FLASH ���������ݻ�����  (������Ϊ����ʱ�Ǿ�������)
        ips_show_uint(50, 16*1,  flash_union_buffer[Number].uint32_type, 10);
        ips_show_uint (180, 16*1, Number, 3);
        printf("\r\n��������������:%d",(int)flash_union_buffer[Number].uint32_type);

        Number++;//�����±�+1,�л�����γ��
                                                                              //�����û���ֶ����GPS_FLASH����������������Զ����GPS_FLASH������������:ÿ�βɼ���λ֮ǰ�����GPS_FLASH
                if(flash_check(FLASH_SECTION_INDEX, GPS_PAGE_INDEX))                      //�ж�Flash�Ƿ������� : �����ݷ���1�������ݷ���0 (�ú���ÿ������ִ��һ�Σ���ִ�б�����ʱ,GPS_FLASHû�ж��������Բ���Ҳ�޷�)                                                               //
                    {
                        flash_erase_page(FLASH_SECTION_INDEX, GPS_PAGE_INDEX);                //����Flash����
                    }

                flash_write_page_from_buffer(FLASH_SECTION_INDEX, GPS_PAGE_INDEX);        //��������������д�뵽ָ��Flash ������ҳ��


    }

   NUM++;


      if(NUM>RP_MAX)                                                               //����ɼ���������ڹ涨���������ֵ������������1
          {
            NUM=1;
          }



      ips_show_uint(200, 0, NUM, 3);                                              //��ʾ�Ѳɼ��ĵ���


}

void GPS_Flash_use()//��GPS_FLASH���������¶��ػ���������ֵ������
{
    flash_buffer_clear();//��ջ�����
    if(flash_check(FLASH_SECTION_INDEX, GPS_PAGE_INDEX))                      //�ж�Flash�Ƿ������� : �����ݷ���1�������ݷ���0
    {
        flash_read_page_to_buffer(FLASH_SECTION_INDEX, GPS_PAGE_INDEX);       //�����ݴ�FLASHָ������ҳ����뵽������

        int TG=0;//�����л���ά�����±�

        for(int data=0;data<ARRAY_Element_Calculation();data++)
        {
            Target_point[0][TG]=flash_union_buffer[data].uint32_type; //γ           //�����������ݾ���ǿ��ת��������ż��˳��ŵ�����������������

//          printf("\r\n��FLASH��ȡ�Ļ�����γ������:%d",  (int)flash_union_buffer[data].uint32_type);
//          printf("\r\n��FLASH��ȡ��Ŀ������γ������:%d",(int)Target_point[0][TG]);

            data++;

            Target_point[1][TG]=flash_union_buffer[data].uint32_type; //��

//          printf("\r\n��FLASH��ȡ�Ļ�������������:%d",  (int)flash_union_buffer[data].uint32_type);
//          printf("\r\n��FLASH��ȡ��Ŀ�����龭������:%d",(int)Target_point[1][TG]);

            TG++;//�±�+1
//            system_delay_ms(100);
        }

        printf("\r\n�ɹ���GPS_FLASHȡ������");                                   //�ɼ����
        printf("\r\n");                                                         //����


    }

}

#endif

///////////////////////////////////////////////////////////////////////////����洢/////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////����洢/////////////////////////////////////////////////////////////////////////////////

void GPS_Work_array()//���ڸ���FLASH,ʵ�ʲμӼ��������
{
    for(int NUM=0;NUM<(ARRAY_Element_Calculation()/2);NUM++)
    {

        Work_target_array[0][NUM]=Target_point[0][NUM];
        Work_target_array[1][NUM]=Target_point[1][NUM];

        printf("\r\n����-%d,��������γ������-%d,�������龭������-%d",NUM+1,(int)Work_target_array[0][NUM],(int)Work_target_array[1][NUM]);
        system_delay_ms(50);
    }
}



void GPS_param_t_init()//GPS������ʼ��
{
    if(flash_check(FLASH_SECTION_INDEX, GPS_PAGE_INDEX)) //�ж�Flash�Ƿ������� : �����ݷ���1�������ݷ���0
    {
        GPS_Flash_use();              //��GPS_FLASH���������¶��ػ���������ֵ������
        GPS_Work_array();                //��Ŀ����������ݸ���һ�ݵ�����Ŀ������(������֮��GPS�ɵ㳹�׽���,���Ը���FLASH)
    }
    else{printf("\r\nGPSΪ�����趨ֵ(ԭʼֵ)");}

}

void GPS_SHOW()//GPS��Ϣ��ʾ
{

    //ÿ���ַ���ռ8����,��ռ16������
    ips_show_string(0, 16*0,"state:");        ips_show_uint(90,  16*0, gps_tau1201.state, 5);
    ips_show_string(0, 16*1,"direction:");    ips_show_float(90, 16*1, gps_tau1201.direction,4, 6);
    ips_show_string(0, 16*2,"latitude:");     ips_show_float(90, 16*2, gps_tau1201.latitude,4, 6);
    ips_show_string(0, 16*3,"longitude:");    ips_show_float(90, 16*3, gps_tau1201.longitude,4, 6);
    ips_show_string(0, 16*4,"yaw:");          ips_show_float(90, 16*4, Daty_Z,4, 6);

}
