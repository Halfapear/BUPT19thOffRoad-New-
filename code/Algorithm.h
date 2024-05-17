/*
 * Algorithm.h
 *
 *  Created on: 2023��3��19��
 *      Author: ORRN
 */

#ifndef CODE_ALGORITHM_H_
#define CODE_ALGORITHM_H_

//�궨��
//===================================================����һά����Ԫ�ظ���===================================================
#define   ARRAY_LENGTH(x) (sizeof(x) / sizeof(x[0]))
//===================================================����һά����Ԫ�ظ���===================================================



//===================================================LQ_PID===================================================
typedef struct
{
    float                kp;         //P
    float                ki;         //I
    float                kd;         //D
    float                imax;       //�����޷�

    float                out_p;  //KP���
    float                out_i;  //KI���
    float                out_d;  //KD���
    float                out;    //pid���

    float                integrator; //< ����ֵ
    float                last_error; //< �ϴ����
    float                last_derivative;//< �ϴ���������ϴ����֮��
    unsigned long        last_t;     //< �ϴ�ʱ��
}pid_param_t;


void PidInit(pid_param_t * pid);

float constrain_float(float amt, float low, float high);

float PidLocCtrl(pid_param_t * pid, float error);

float PidIncCtrl(pid_param_t * pid, float error);


int ARRAY_Element_Calculation(void);

//ȫ�ֱ�������
extern pid_param_t PID_Init;
extern pid_param_t PID_IMU;
extern pid_param_t PID_GPS;
extern pid_param_t PID_MOTOR;

 //===================================================LQ_PID===================================================


//===================================================GPS��IMU�����˲�===================================================
void GPS_IMU_COM_filtering();
//===================================================GPS��IMU�����˲�===================================================

//===================================================����ƽ���˲�===================================================
#define MOVE_AVERAGE_SIZE   32  // ���建������С


typedef struct
{
    uint8 index;                            // �±�
    uint8 buffer_size;                      // buffer��С
    int32 data_buffer[MOVE_AVERAGE_SIZE];   // ������
    int32 data_sum;                         // ���ݺ�
    int32 data_average;                     // ����ƽ��ֵ
}move_filter_struct;

extern move_filter_struct speed_filter;


void move_filter_init(move_filter_struct *move_average);
void move_filter_calc(move_filter_struct *move_average, int32 new_data);
void myprintf(char *format, ...) ;

//===================================================��̬PID===================================================




//===================================================��̬PID===================================================



//===================================================����ƽ���˲�===================================================


#endif /* CODE_ALGORITHM_H_ */
