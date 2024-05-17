/*
 * Algorithm.h
 *
 *  Created on: 2023年3月19日
 *      Author: ORRN
 */

#ifndef CODE_ALGORITHM_H_
#define CODE_ALGORITHM_H_

//宏定义
//===================================================计算一维数组元素个数===================================================
#define   ARRAY_LENGTH(x) (sizeof(x) / sizeof(x[0]))
//===================================================计算一维数组元素个数===================================================



//===================================================LQ_PID===================================================
typedef struct
{
    float                kp;         //P
    float                ki;         //I
    float                kd;         //D
    float                imax;       //积分限幅

    float                out_p;  //KP输出
    float                out_i;  //KI输出
    float                out_d;  //KD输出
    float                out;    //pid输出

    float                integrator; //< 积分值
    float                last_error; //< 上次误差
    float                last_derivative;//< 上次误差与上上次误差之差
    unsigned long        last_t;     //< 上次时间
}pid_param_t;


void PidInit(pid_param_t * pid);

float constrain_float(float amt, float low, float high);

float PidLocCtrl(pid_param_t * pid, float error);

float PidIncCtrl(pid_param_t * pid, float error);


int ARRAY_Element_Calculation(void);

//全局变量声明
extern pid_param_t PID_Init;
extern pid_param_t PID_IMU;
extern pid_param_t PID_GPS;
extern pid_param_t PID_MOTOR;

 //===================================================LQ_PID===================================================


//===================================================GPS与IMU互补滤波===================================================
void GPS_IMU_COM_filtering();
//===================================================GPS与IMU互补滤波===================================================

//===================================================滑动平均滤波===================================================
#define MOVE_AVERAGE_SIZE   32  // 定义缓冲区大小


typedef struct
{
    uint8 index;                            // 下标
    uint8 buffer_size;                      // buffer大小
    int32 data_buffer[MOVE_AVERAGE_SIZE];   // 缓冲区
    int32 data_sum;                         // 数据和
    int32 data_average;                     // 数据平均值
}move_filter_struct;

extern move_filter_struct speed_filter;


void move_filter_init(move_filter_struct *move_average);
void move_filter_calc(move_filter_struct *move_average, int32 new_data);
void myprintf(char *format, ...) ;

//===================================================动态PID===================================================




//===================================================动态PID===================================================



//===================================================滑动平均滤波===================================================


#endif /* CODE_ALGORITHM_H_ */
