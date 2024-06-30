#ifndef MIC_CALCULATE_H_
#define MIC_CALCULATE_H_
#include "zf_common_headfile.h"
#define FFT_SIZE          2048 
#define FS              10000//������

extern float count_signal_0[FFT_SIZE*2];//�������������
extern float count_signal_1[FFT_SIZE*2];//�������������
extern float Chrip_signal[FFT_SIZE*2];//��׼Chrip�ź�
extern float one_signal[4][2*FFT_SIZE];//��һ������
extern float fft_outputSignal[4][FFT_SIZE];//Ƶ��
extern float out_signal[2][FFT_SIZE];
extern float rcy;//��ض�
extern int MIC_rcyflag;
extern float RcyThredNum;

void Get_mic_Distance();//һ�黥��أ����ؾ����ֵ
float Get_mic_Angle(void);      //����ػ�ýǶ�
void generateChirp(float signal[], int length, float f0, float f1, int fs);//���Chrip�ź�
void normalizeSignal(float signal[], int length);//��һ������
void ChripPipei();//�жϵ�ǰ�ź����׼Chrip�źŵ���ض�--ȫ�ֱ�������



#endif