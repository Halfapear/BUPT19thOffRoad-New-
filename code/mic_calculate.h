#ifndef MIC_CALCULATE_H_
#define MIC_CALCULATE_H_
#include "zf_common_headfile.h"
#define FFT_SIZE          2048 
#define FS              10000//采样率

extern float count_signal_0[FFT_SIZE*2];//互相关运算数组
extern float count_signal_1[FFT_SIZE*2];//互相关运算数组
extern float Chrip_signal[FFT_SIZE*2];//标准Chrip信号
extern float one_signal[4][2*FFT_SIZE];//归一化数组
extern float fft_outputSignal[4][FFT_SIZE];//频域
extern float out_signal[2][FFT_SIZE];
extern float rcy;//相关度
extern int MIC_rcyflag;
extern float RcyThredNum;

void Get_mic_Distance();//一组互相关，返回距离差值
float Get_mic_Angle(void);      //互相关获得角度
void generateChirp(float signal[], int length, float f0, float f1, int fs);//获得Chrip信号
void normalizeSignal(float signal[], int length);//归一化处理
void ChripPipei();//判断当前信号与标准Chrip信号的相关度--全局变量接受



#endif