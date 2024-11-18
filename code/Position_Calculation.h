/*
 * Position_Calculation.h
 *
 *  Created on: 2023��3��18��
 *      Author: ORRN
 */

#ifndef CODE_POSITION_CALCULATION_H_
#define CODE_POSITION_CALCULATION_H_

//ȫ�ֱ�������
extern int S_Point;
extern int P_Distance;
extern int Control_FLAG;         //�ñ�־λ�����ǣ�����Ϊ1ʱ���޿�ģʽ(��GPS+IMU����)/���ֱ��ϵİ�������ʱ���ñ�־λ����0,�޷�ִ���޿�ģʽ��ת���п�ģʽ
extern double Error;
extern int   STEER_Value;
extern int16 SPEED_Value;
extern double azimuth;
extern int next_point;

extern double start_distance;//������ʼ��͵�ǰ��ľ���


extern int Start_GPSangle_Flag;//�ܶ�ֱ���Ի�ȡGPS����ǣ�Ϊ�˺�����IMUͬ��


//��������
void Follow_Track(void);//����ѭ������
void Task_RZT(int Steer,int16 Motor,int Angle,char Type);//����:��׶ͰһȦ
void Task_STOP(int point);           //����:���õ�n����ͣ��
void Task_H_speed(int16 H_speed);    //ֱ��������ʻ
void Task_L_speed(int16 L_speed);    //����·�ε�����ʻ
void Path_judgment(void);            //ͨ����λ���ж��Ǵ����ĸ�Ԫ��
void Interval_judgment(void);        //��λ�����жϣ���������ֵ
void Switching_point_AD(void);       //�е�ǶȾ������ж�
void Switching_point_D(void);        //�е�����ж�
void Double_layer_feedback_0(void);    //˫�㷴��
void Double_layer_feedback_180(void);    //˫�㷴��
void Single_layer_feedback(void);    //���㷴��
void IMU_Navigation_0(void);
void IMU_Navigation_180_N(void);//��ʱ��180��IMU����
void IMU_Navigation_180_S(void);//˳ʱ��180��IMU����
void Retardation(int X,int16 Y,int16 Z);//�����ƶ�
void STOP(void);//ͣ��
void Navigation_mode(int TYPE);//����ģʽѡ��

#endif /* CODE_POSITION_CALCULATION_H_ */
