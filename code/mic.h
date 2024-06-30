#ifndef MIC_H_
#define MIC_H_
#include "zf_common_headfile.h"

#define MIC_PIN_1        (ADC1_CH08_P12_4)//前
#define MIC_PIN_2        (ADC1_CH25_P14_5)//左
#define MIC_PIN_3        (ADC1_CH09_P12_5)//后
#define MIC_PIN_4        (ADC1_CH24_P14_4)//右

#define FFT_SIZE          2048 
#define MIC_RAW_DATA_LEN  2500                  // 用于采集硅麦的循环数组长度(需要大于 FFT_SIZE 防止数据异常)

extern int16 mic_raw_data[4][MIC_RAW_DATA_LEN];        // 定义采集需要用到的循环数组
extern int16 mic_raw_data_count;                       // 定义循环数组计数位
extern int16 mic_raw_data_count_save;                  // 定义循环数组计数位-保存值(用于取出最近的 FFT_SIZE 个数据)
//extern float fft_signal[4][2*FFT_SIZE];                // 定义取出数据的保存位置

extern float fft_signal_1[2*FFT_SIZE];  
extern float fft_signal_2[2*FFT_SIZE];  
extern float fft_signal_3[2*FFT_SIZE];  
extern float fft_signal_4[2*FFT_SIZE];  
void mic_init();                //麦克风adc接口初始化
void mic_data_get (void);       //麦克风原始数据获取     -放pit0
void mic_data_copy(void);       //将原始数据赋值出来，用于计算   -放循环

#endif //MIC_H_