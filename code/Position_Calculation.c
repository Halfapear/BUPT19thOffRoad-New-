/*
 * Position_Calculation.c
 *
 *  Created on: 2023��3��18��
 *      Author: ORRN
 */
// 5.14 ��zf����֮���Ҳ����˸�������ʣ�GPS���������ָ��С���ģ����Ǹ��㻹��·��
//�����ҹ���Ҫ���壬ȥ���Ҫ����ʲô��Ϊʲô����������� ��Ȼ��÷���
//����һ����״̬�����������״̬�ظ��ԱȽϸߣ������ϸ����
//��Ҫ�� �Ȳ���·���滮�������״̬�����֣��ھ�����һ����ĳ����������GPS+IMU�����������Ǹ��ݽǶȵ������������ж��Ǻ��Ӿ�����Щûʲô�����ˣ��ڶ���״̬��ĳ���������ڿ�����λ��
//������ϸ�־��Ǹ��ݲ�ͬ������ٶȣ�ת��ת��˳��һ��
//��50�׻��ǿ��Դ���һЩ������㲻�Ѷ�����˾���
//��gpt��΢������һ�£���Ҫ���Ҳ��뿴�������ļ��Ϳ�����Щ��Ҫ��ĵģ������������ǰѹ�ͨ��Ū���
//���� ȷʵ ������һ����ʱ����Ҫת�� ��ʱ���ٶȹ��첻֪���ᷢ��ʲô 
//����һ�룬��ʵ�����ϻ���pid�����������Ҳ�ǽǶȡ���������˵ʵ������pid�϶������洬������������������е��񵼺���ͼ�����İ������֮�£������״��Ǳ߲���������·���滮

//5.15������#if ѡ��־λ
//Ȼ����next_point
//��������һ��������Switching_point_D()��
//�������ر��ر�࣬�����ܸ��ӳ̶ȱ��������д�
//���ĸ�type���㷴��˫�㷴��

//5.16azimuth��distance �к���ֱ���ṩ
//�����ȸ���point�����ٶ� �� Navigation_mode
//�����Ǵ�GPS ˫�����IMU
//��û��ϸ˵��׶Ͱ��ʶ��ʲô�ģ�����׶Ͱ
//��������û������������Ǹ�·�ϣ���������ô�ɼ��� �������Ҫ
//
//
//

#include "zf_common_headfile.h"
#include "zf_device_gnss.h" // �滻ͷ�ļ�

extern gnss_info_struct gnss; // ʹ��gnss����

int Control_FLAG = 1;         // �ñ�־λ�����ǣ�����Ϊ1ʱ���޿�ģʽ(��GPS+IMU����)/���ֱ��ϵİ�������ʱ���ñ�־λ����0,�޷�ִ���޿�ģʽ��ת���п�ģʽ
int S_Point = 0;              // ֹͣ��λ����
int P_Distance = 0;           // ��λ�������

//next_point����loraģ���ȡ�����ݽ��и�ֵ��lora����0-8,0Ϊ��ʼ�㣬1-8Ϊ�ű�,��ʱ���ֻ��9����
int next_point = 0;           // ��һ��Ŀ���
 

int Point_interval = 0;       // ��λ������
int ZT_FLAG = 0;              // ��׶Ͱһ�ܱ�־λ
int Distance_FLAG = 0;        // �����л���־λ
int Angle_FLAG = 0;           // �Ƕ��л���־λ
int IMU_suppression_FLAG = 1; // IMU���ƿ�����־λ
int16 SPEED_Value = 0;        // Ŀ���ٶ�

double distance = 0;    // ����ľ���
double azimuth = 0;     // ��λ��(�ó��ĺ�������ֵΪdouble)
double last_azimuth = 0;// ��һ�εķ�λ��
double Error = 0;       // ��λ�Ǻͺ���ǵ����(�ó��ĺ�������ֵΪdouble)

double start_distance=0;

int Start_GPSangle_Flag=0;//�ܶ�ֱ���Ի�ȡGPS����ǣ�Ϊ�˺�����IMUͬ��
int ST_Point_RecordFlag=0;//��ʼ��λ��¼��־λ




float PD_YAW = 0;

void Follow_Track() // ����ѭ������
{
  //��gpsģʽ
#if GPS_Mode
#if (Float_Record_FLAG || Double_Record_FLAG) // �����Ⱥ�˫���ȴ洢�������
    distance = get_two_points_distance(gnss.latitude, gnss.longitude, Work_target_array[0][next_point], Work_target_array[1][next_point]); // ����
    last_azimuth = get_two_points_azimuth(gnss.latitude, gnss.longitude, Work_target_array[0][next_point - 1], Work_target_array[1][next_point - 1]); // ��λ�� (���������������ļн�)
    azimuth = get_two_points_azimuth(gnss.latitude, gnss.longitude, Work_target_array[0][next_point], Work_target_array[1][next_point]); // ��λ�� (���������������ļн�)
#endif

#if (Array_Record_FLAG)
    distance = get_two_points_distance(gnss.latitude, gnss.longitude, GPS_GET_LAT[next_point], GPS_GET_LOT[next_point]); // ����
    azimuth = get_two_points_azimuth(gnss.latitude, gnss.longitude, GPS_GET_LAT[next_point], GPS_GET_LOT[next_point]); // ��λ�� (���������������ļн�)
#endif

    // �����ںϵĴ���
    // ��IMU�滮Ϊ˳ʱ��0-(180),��ʱ��0-(-180)
    // ��GPS�滮Ϊ������0-(180),������0-(-180)
    // IMU_Navigation_0()------0
    // IMU_Navigation_180_N()------1
    // IMU_Navigation_180_S()------2
    // Single_layer_feedback()------3
    // Double_layer_feedback_0()------4
    //�����⼸�����������յĽǶȽ��㺯������������͸������ֵ��
    
//��д���
    
        printf("DS:%f,YAW:%f,AZI:%f��point:%d,gnss:%f",distance,Daty_Z,azimuth,next_point,gnss.direction);
     // �����о��е�----����Ҫ�ں��ű�
    if(Start_GPSangle_Flag==0)
    {
      if(ST_Point_RecordFlag==0){
        Start_Point[0]=gnss.latitude;//���������Լ�����flash�ڲ�������ݽ��л�������
        Start_Point[1]=gnss.longitude;
        ST_Point_RecordFlag=1;//ֻ��һ�Σ�ע�⵽��ʼ����ת��FOLLOW_TRACK
      }
      start_distance=get_two_points_distance(gnss.latitude, gnss.longitude, Start_Point[0], Start_Point[1]); // ����ʼ�����
      //ֱ����Work_target_array������ĵ�һ��ֵ��Ϊ��ʼ��      
      printf("start_distance:%f",start_distance);
      if(fabs(start_distance)>3.0)
      {
          GPS_direction_average();//����3m,��ȡGPS�����
          gps_direction_average= get_two_points_azimuth(Start_Point[0], Start_Point[1], gnss.latitude, gnss.longitude);
          if(gps_direction_average>180 && gps_direction_average<=360)
          {
              gps_direction_average-=360;
          }
          else if(gps_direction_average<(-180) && gps_direction_average>=(-360))
          {
              gps_direction_average+=360;
          }
          Daty_Z=gps_direction_average; 
          
          printf("\r\nIMU��ʼֵ��GPSͬ��!!!,ͬ���Ƕȣ�%f",gps_direction_average);
          Start_GPSangle_Flag=1;
          next_point ++;
      }
      SPEED_Value=1300;//�𲽽׶�
      
    }
    else
    {
        //����Ѱ��-gps
        SPEED_Value=2000;
        Navigation_mode(3);
        //Ҫ���һ����������
        printf("\n Ѱ����");
        Switching_point_D();
      
    
    }
        
    
/*
    if (next_point < 1 || next_point == 7) {
        Navigation_mode(0);
    }
    //������Կ����󲿷ֵ���3ģʽ���ű�Ӧ��Ҳֻ��3�͹��˰�?
    if ((next_point >= 1 && next_point < 7) || (next_point > 7 && next_point < 10) || next_point > 10) {
        Navigation_mode(3);
    } else if (next_point == 10) {
        Navigation_mode(2);
    }

    // STEER_Value=PidLocCtrl(&PID_GPS,SERVO_MOTOR_MID+Error);//SD9
    // STEER_Value=PidLocCtrl(&PID_GPS,SERVO_MOTOR_MID-Error);//SD5

    Switching_point_D(); // �����о��е�----����Ҫ�ں��ű�

    
    //ȥ����������Ƽ���׶Ͱ������Ԫ�أ�������Ĵ���ʱ�ɼ��õ�λ��Ĳ�ͬԪ���ٶȿ���
    //���ű��Ԫ��ֻ��һ���ⲿ��Ӧ���ǲ���Ҫ��
    if (next_point == 1) {
        Retardation(50, 40, -400); // �����ƶ�
    } else if (next_point == 2) {
        SPEED_Value = 40;
    } else if (next_point == 3) {
        SPEED_Value = 40;
    } else if (next_point == 4) {
        SPEED_Value = 40;
    } else if (next_point == 5) {
        SPEED_Value = 40;
    } else if (next_point == 6) {
        SPEED_Value = 100;
    } else if (next_point == 7) {
        SPEED_Value = 100;
    } else if (next_point == 8) {
        gpio_set_level(BUZZER_PIN, 1);
//        Task_RZT(67, 70, 0, 'N'); // ��� �ٶ�
        SPEED_Value = 130;
    } else if (next_point == 9) {
        SPEED_Value = 60;
    } else if (next_point == 10) { // ��ͷ
        SPEED_Value = 50;
    } else if (next_point == 11) {
        SPEED_Value = 150;
    } else if (next_point == 12) {
        SPEED_Value = 100;
    } else if (next_point == 13) {
        SPEED_Value = 100;
    } else if (next_point == 14) { // �µ���
        SPEED_Value = 100;
    } else if (next_point == 15) { // ͣ��
        STOP();
    } else { // Ĭ��
        SPEED_Value = 100;
    }

*/
    
#endif
    
    
    
//ƥ���˲���
//ҪԤ������һ�˱�׼��Chirp�źţ�

#if MIC_Mode
        
    //ChripPipei();
        SPEED_Value=1100;
        MIC_rcyflag=1;
        Start_GPSangle_Flag=1;
        Error=Get_mic_Angle_Filtered();
        printf("\r\nError:%lf\r\n",Error);
    
   
    
    
    
    
    
    
#endif
    
   
    
#if GPS_MIC_Mode
        
    //next_point=lora_receive_num;//��lora�յ��ı��ֱ�Ӹ�ֵ��next_point-0Ϊ�����㣬1-8Ϊ�ű�
    next_point=1;
    if(start_mict>10)
        restart_mic=5;//����ʱ��
    if(restart_mic>0){
        next_point=0;        
    }
    
    ips200_show_int(0,16*8,next_point,2);
            ips200_show_float(0, 16 * 9, Work_target_array[0][next_point],3,6); 
            ips200_show_float(0, 16 * 10, Work_target_array[1][next_point] ,3,6);
    
#if (Float_Record_FLAG || Double_Record_FLAG) // �����Ⱥ�˫���ȴ洢�������
    //start_distance=get_two_points_distance(gnss.latitude, gnss.longitude, Work_target_array[0][0], Work_target_array[1][0]); // ����ʼ�����
    distance = get_two_points_distance(gnss.latitude, gnss.longitude, Work_target_array[0][next_point], Work_target_array[1][next_point]); // ����
    last_azimuth = get_two_points_azimuth(gnss.latitude, gnss.longitude, Work_target_array[0][next_point - 1], Work_target_array[1][next_point - 1]); // ��λ�� (���������������ļн�)
    azimuth = get_two_points_azimuth(gnss.latitude, gnss.longitude, Work_target_array[0][next_point], Work_target_array[1][next_point]); // ��λ�� (���������������ļн�)
#endif
    printf("\r\nDS:%f,YAW:%f,AZI:%f��point:%d",distance,Daty_Z,azimuth,next_point);
     // �����о��е�----����Ҫ�ں��ű�
    if(Start_GPSangle_Flag==0)
    {
      if(ST_Point_RecordFlag==0){
        Start_Point[0]=gnss.latitude;//���������Լ�����flash�ڲ�������ݽ��л�������
        Start_Point[1]=gnss.longitude;
        ST_Point_RecordFlag=1;//ֻ��һ�Σ�ע�⵽��ʼ����ת��FOLLOW_TRACK
      }
      Error=Daty_Z;
      start_distance=get_two_points_distance(gnss.latitude, gnss.longitude, Start_Point[0], Start_Point[1]); // ����ʼ�����
      //ֱ����Work_target_array������ĵ�һ��ֵ��Ϊ��ʼ��      
      printf("start_distance:%f",start_distance);
      if(fabs(start_distance)>3.0)
      {
          GPS_direction_average();//����3m,��ȡGPS�����
          gps_direction_average= get_two_points_azimuth(Start_Point[0], Start_Point[1], gnss.latitude, gnss.longitude);
          if(gps_direction_average>180 && gps_direction_average<=360)
          {
              gps_direction_average-=360;
          }
          else if(gps_direction_average<(-180) && gps_direction_average>=(-360))
          {
              gps_direction_average+=360;
          }
          Daty_Z=gps_direction_average; 
          
          printf("\r\nIMU��ʼֵ��GPSͬ��!!!,ͬ���Ƕȣ�%f",gps_direction_average);
          Start_GPSangle_Flag=1;
          //next_point ++;
      }
      SPEED_Value=3000;//�𲽽׶�
      
    }
    else
    {
        //����Ѱ��-gps-mic
       // SPEED_Value=30;
        
       // Navigation_mode(3);
        //Ҫ���һ����������
        printf("\r\n Ѱ����");
        //Switching_point_D();
        //�е��о���lora����lora��õ�ֱֵ�Ӹ�ֵ��next_point
        if(next_point!=0){
            if(distance>5)
            {
                SPEED_Value=SPEED; //B31�˵����Զ�GPSѰ���ٶȽ��е���       
                Navigation_mode(3);
                MIC_rcyflag=0;
                
            }
            else
            {
                SPEED_Value=1700; 
                //start_mict=my_s;
                Error=Get_mic_Angle_Filtered();
                MIC_rcyflag=1;
            }
        }  
        else{
            SPEED_Value=3000; 
            MIC_rcyflag=0;
            Navigation_mode(3);
            if(distance<2)
            {
                SPEED_Value=0;   
            }          
        }
    }

    
#endif

}


void Switching_point_D() // �е�(����)�ж�-----�е��־Ҫ�ģ��ű�Ӱ���£�Ӧ�ø�Ϊ�ȹ��ű����е�
{       
       if (distance < 3){//�е�
         next_point++;
         printf("\r\n 3-��ǰ��λ-%d,distance:%d", next_point,distance);
         
       }
}  


void Double_layer_feedback_0() // ˫�㷴��(����ȥ��ʱ����0��)
{
    //��һ����azimuth��ȡ���˳������Ŀ���ķ�λ��
    // �򱱷���
    if (azimuth > 180) { // ������180�ķ���Ǳ�Ϊ����
        azimuth -= 360;
    }
    
    // ���Ϸ���
    
    
    /*azimuth += 180;    if (azimuth > 180) { // ������180�ķ���Ǳ�Ϊ����
        azimuth -= 360;
    }*/

    if ((azimuth - Daty_Z) > 180) { // �����Ƕ�֮�����180��ʱ���򽫲�ֵ��ȥ360
        Error = azimuth - PidLocCtrl(&PID_IMU, Daty_Z) - 360; // GPS�ķ����-IMU����PD����ĺ����
       // printf("IMU(0).azimuth:%f,Daty_Z:%f,Error:%f\n", azimuth, Daty_Z, Error); // �����λ����������ô�����δ���������ֵ��0
    } else if ((azimuth - Daty_Z) < -180) { // �����Ƕ�֮��С��-180��ʱ���򽫲�ֵ����360
        Error = azimuth - PidLocCtrl(&PID_IMU, Daty_Z) + 360; // GPS�ķ����-IMU����PD����ĺ����
       // printf("IMU(0).azimuth:%f,Daty_Z:%f,Error:%f\n", azimuth, Daty_Z, Error); // �����λ����������ô�����δ���������ֵ��0
    } else {
        Error = azimuth - PidLocCtrl(&PID_IMU, Daty_Z); // GPS�ķ����-IMU����PD����ĺ����
       // printf("IMU(0).azimuth:%f,Daty_Z:%f,Error:%f\n", azimuth, Daty_Z, Error); // �����λ����������ô�����δ���������ֵ��0
    }
}

void Double_layer_feedback_180() // ˫�㷴��(���ڷ��ص�ʱ����180��)
{
    if (azimuth > 180) { // ������180�ķ���Ǳ�Ϊ����
        azimuth -= 360;
    }

    if ((azimuth - Daty_Z) > 180) { // �����Ƕ�֮�����180��ʱ���򽫲�ֵ��ȥ360
        Error = azimuth - PidLocCtrl(&PID_IMU, Daty_Z) - 360; // GPS�ķ����-IMU����PD����ĺ����
        //printf("IMU(180).azimuth:%f,Daty_Z:%f,Error:%f\n", azimuth, Daty_Z, Error); // �����λ����������ô�����δ���������ֵ��0
    } else if ((azimuth - Daty_Z) < -180) { // �����Ƕ�֮��С��-180��ʱ���򽫲�ֵ����360
        Error = azimuth - PidLocCtrl(&PID_IMU, Daty_Z) + 360; // GPS�ķ����-IMU����PD����ĺ����
        //printf("IMU(180).azimuth:%f,Daty_Z:%f,Error:%f\n", azimuth, Daty_Z, Error); // �����λ����������ô�����δ���������ֵ��0
    } else {
        Error = azimuth - PidLocCtrl(&PID_IMU, Daty_Z); // GPS�ķ����-IMU����PD����ĺ����
       // printf("IMU(180).azimuth:%f,Daty_Z:%f,Error:%f\n", azimuth, Daty_Z, Error); // �����λ����������ô�����δ���������ֵ��0
    }
    printf("ERROR:%f",Error);
}

void Single_layer_feedback() // ���㷴��
{
    // �򱱷���
    if (azimuth > 180) { // ������180�ķ���Ǳ�Ϊ����
        azimuth -= 360;
    }
    // ���Ϸ���
   // azimuth += 180;
   // if (azimuth > 180) { // ������180�ķ���Ǳ�Ϊ����
     //   azimuth -= 360;
    //}
    if ((azimuth - Daty_Z) > 180) { // �����Ƕ�֮�����180��ʱ���򽫲�ֵ��ȥ360
        Error = azimuth - Daty_Z - 360;
       printf("\r\nGPS.azimuth:%f,Daty_Z:%f,Error:%f\n", azimuth, Daty_Z, Error); // �����λ����������ô�����δ���������ֵ��0
    } else if ((azimuth - Daty_Z) < -180) { // �����Ƕ�֮��С��-180��ʱ���򽫲�ֵ����360
        Error = azimuth - Daty_Z + 360;
       printf("\r\nGPS.azimuth:%f,Daty_Z:%f,Error:%f\n", azimuth, Daty_Z, Error); // �����λ����������ô�����δ���������ֵ��0
    } else {
        Error = azimuth - Daty_Z;
       printf("\r\nGPS.azimuth:%f,Daty_Z:%f,Error:%f\n", azimuth, Daty_Z, Error); // �����λ����������ô�����δ���������ֵ��0
    }
}

void IMU_Navigation_0() // 0��IMU����
{
    Error = T_M;
}

void IMU_Navigation_180_N() // 180��IMU����
{
    Error = (180 + T_M);
}

void IMU_Navigation_180_S() // 180��IMU����
{
    Error = (-180 + T_M);
}

void Retardation(int X, int16 Y, int16 Z) // �����ƶ�
{
    if (encoder < X) {
        SPEED_Value = Y;
    } else {
        SPEED_Value = (Z);
    }
}

void STOP() // ͣ��
{
    if (encoder < 10) {
        while (1) {
            SPEED_Value = 0; // ������ɣ�ͣ����
            Steer_set(SERVO_MOTOR_MID);
            gpio_set_level(BUZZER_PIN, 1);
        }
    } else {
        SPEED_Value = (-300);
    }
}

void Navigation_mode(int TYPE)
{
    if (TYPE == 0) {
        IMU_Navigation_0(); // IMU--0�ȵ���
    } else if (TYPE == 1) {
        IMU_Navigation_180_N(); // ��ʱ��IMU--180�ȵ���(��ת)
    } else if (TYPE == 2) {
        IMU_Navigation_180_S(); // ˳ʱ��IMU--180�ȵ���(��ת)
    } else if (TYPE == 3) {
         Single_layer_feedback(); // ���㷴��----û��IMU����ת��Ч��
    } else if (TYPE == 4) {
        Double_layer_feedback_0(); // ˫�㷴��(����ȥ��ʱ����0��)
    }
}
