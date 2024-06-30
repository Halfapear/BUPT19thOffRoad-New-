#ifndef MIC_H_
#define MIC_H_
#include "zf_common_headfile.h"

#define MIC_PIN_1        (ADC1_CH08_P12_4)//ǰ
#define MIC_PIN_2        (ADC1_CH25_P14_5)//��
#define MIC_PIN_3        (ADC1_CH09_P12_5)//��
#define MIC_PIN_4        (ADC1_CH24_P14_4)//��

#define FFT_SIZE          2048 
#define MIC_RAW_DATA_LEN  2500                  // ���ڲɼ������ѭ�����鳤��(��Ҫ���� FFT_SIZE ��ֹ�����쳣)

extern int16 mic_raw_data[4][MIC_RAW_DATA_LEN];        // ����ɼ���Ҫ�õ���ѭ������
extern int16 mic_raw_data_count;                       // ����ѭ���������λ
extern int16 mic_raw_data_count_save;                  // ����ѭ���������λ-����ֵ(����ȡ������� FFT_SIZE ������)
//extern float fft_signal[4][2*FFT_SIZE];                // ����ȡ�����ݵı���λ��

extern float fft_signal_1[2*FFT_SIZE];  
extern float fft_signal_2[2*FFT_SIZE];  
extern float fft_signal_3[2*FFT_SIZE];  
extern float fft_signal_4[2*FFT_SIZE];  
void mic_init();                //��˷�adc�ӿڳ�ʼ��
void mic_data_get (void);       //��˷�ԭʼ���ݻ�ȡ     -��pit0
void mic_data_copy(void);       //��ԭʼ���ݸ�ֵ���������ڼ���   -��ѭ��

#endif //MIC_H_