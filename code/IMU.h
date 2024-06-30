/*
 * IMU.h
 *
 *  Created on: 2023��3��18��
 *      Author: ORRN
 */

#ifndef CODE_IMU_H_
#define CODE_IMU_H_


//�ṹ������
typedef struct{
    float Xdata;   //��Ʈ����X
    float Ydata;   //��Ʈ����Y
    float Zdata;   //��Ʈ����Z
}gyro_param_t ;

typedef struct{
    float acc_x;   //x����ٶ�
    float acc_y;   //y����ٶ�
    float acc_z;   //z����ٶ�

    float gyro_x;  //x����ٶ�
    float gyro_y;  //y����ٶ�
    float gyro_z;  //z����ٶ�
}IMU_param_t ;

//ȫ�ֱ�������
extern float gyro_Offset_flag;
extern float Daty_Z;
extern float T_M;
extern float T_N;
extern int   GL_IMU_Flag;

//��������

float IMU_gyro_Offset_Init();//��������Ư��ʼ��
void  GPS_IMU_Complementary_filter(void);//��GPS������direction(�����)��IMU������YAW(�����)���л����˲�
void  IMU_GetValues();//���ɼ�����ֵת��Ϊʵ������ֵ, ���������ǽ���ȥ��Ư����
void  IMU_YAW_integral(void);//�Խ��ٶȽ��л���
void  IMU_init(void);//IMU��ʼ��
void  IMU_SHOW(void);//IMU������ʾ
//void  IMU_TEXT();//IMU���Ʋ���
void  IMU_Handle_180(void);
void  IMU_Handle_360(void);
void  IMU_Handle_0  (void);

#endif /* CODE_IMU_H_ */
