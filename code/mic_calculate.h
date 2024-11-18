#ifndef MIC_CALCULATE_H_
#define MIC_CALCULATE_H_
#include "zf_common_headfile.h"
#define FFT_SIZE          2048 
#define FS              10000//采样率
#define FILTER_SIZE 5  // 定义滤波器大小

extern float count_signal_0[FFT_SIZE*2];//互相关运算数组
extern float count_signal_1[FFT_SIZE*2];//互相关运算数组
extern float Chrip_signal[FFT_SIZE*2];//标准Chrip信号
extern float one_signal[4][2*FFT_SIZE];//归一化数组
extern float fft_outputSignal[4][FFT_SIZE];//频域
extern float out_signal_0[FFT_SIZE];//可视化输出数组
extern float out_signal_1[FFT_SIZE];
extern float rcy;//相关度
extern int MIC_rcyflag;//相关标志位
extern float RcyThredNum;

extern float angle_filter_buffer[FILTER_SIZE] ;  // 角度值缓存数组
extern int filter_index;  // 滤波数组索引

void Get_mic_Distance();//一组互相关，距离差值
float Get_mic_Angle(void);      //互相关获得角度
void generateChirp(float signal[], int length, float f0, float f1, int fs);//获得Chrip信号
void normalizeSignal(float signal[], int length);//归一化处理
void ChripPipei();//判断当前信号与标准Chrip信号的相关度--全局变量接受
double calculate_angle_with_y_axis(double x, double y);//计算角度
float Moving_Average_Filter(float new_angle);//滤波函数，内部调用
float Get_mic_Angle_Filtered(void);//滤波角度输出




#endif