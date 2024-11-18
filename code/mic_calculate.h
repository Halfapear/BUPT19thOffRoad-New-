#ifndef MIC_CALCULATE_H_
#define MIC_CALCULATE_H_
#include "zf_common_headfile.h"
#define FFT_SIZE          2048 
#define FS              10000//������
#define FILTER_SIZE 5  // �����˲�����С

extern float count_signal_0[FFT_SIZE*2];//�������������
extern float count_signal_1[FFT_SIZE*2];//�������������
extern float Chrip_signal[FFT_SIZE*2];//��׼Chrip�ź�
extern float one_signal[4][2*FFT_SIZE];//��һ������
extern float fft_outputSignal[4][FFT_SIZE];//Ƶ��
extern float out_signal_0[FFT_SIZE];//���ӻ��������
extern float out_signal_1[FFT_SIZE];
extern float rcy;//��ض�
extern int MIC_rcyflag;//��ر�־λ
extern float RcyThredNum;

extern float angle_filter_buffer[FILTER_SIZE] ;  // �Ƕ�ֵ��������
extern int filter_index;  // �˲���������

void Get_mic_Distance();//һ�黥��أ������ֵ
float Get_mic_Angle(void);      //����ػ�ýǶ�
void generateChirp(float signal[], int length, float f0, float f1, int fs);//���Chrip�ź�
void normalizeSignal(float signal[], int length);//��һ������
void ChripPipei();//�жϵ�ǰ�ź����׼Chrip�źŵ���ض�--ȫ�ֱ�������
double calculate_angle_with_y_axis(double x, double y);//����Ƕ�
float Moving_Average_Filter(float new_angle);//�˲��������ڲ�����
float Get_mic_Angle_Filtered(void);//�˲��Ƕ����




#endif