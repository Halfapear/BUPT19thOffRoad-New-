#include "mic.h"

int16 mic_raw_data[4][MIC_RAW_DATA_LEN];        // 定义采集需要用到的循环数组
int16 mic_raw_data_count;                       // 定义循环数组计数位
int16 mic_raw_data_count_save;                  // 定义循环数组计数位-保存值(用于取出最近的 FFT_SIZE 个数据)


int16 mic_raw_data_1[MIC_RAW_DATA_LEN];
int16 mic_raw_data_2[MIC_RAW_DATA_LEN];
int16 mic_raw_data_3[MIC_RAW_DATA_LEN];
int16 mic_raw_data_4[MIC_RAW_DATA_LEN];


float sin_signal[FFT_SIZE+10];

//float fft_signal[4][2*FFT_SIZE];                // 定义取出数据的保存位置
float fft_signal_1[2*FFT_SIZE];  
float fft_signal_2[2*FFT_SIZE];  
float fft_signal_3[2*FFT_SIZE];  
float fft_signal_4[2*FFT_SIZE];  


int16 adc_offset=2048;                          //adc稳态误差
//float是因为三角计算之后可能会产生小数

//adc初始化
void mic_init()
{
    adc_init(MIC_PIN_1, ADC_12BIT);
    adc_init(MIC_PIN_2, ADC_12BIT); 
    adc_init(MIC_PIN_3, ADC_12BIT);
    adc_init(MIC_PIN_4, ADC_12BIT); 
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     采集麦克风数据
// 参数说明     void
// 返回参数     void   
// 使用示例     mic_data_get();
// 备注信息     该函数在中断中调用
//-------------------------------------------------------------------------------------------------------------------
void mic_data_get (void)
{
    //adc——offset稳态误差
    // 因为在做FFT计算的时候我们需要减去直流偏量，因此这里固定减2048，实际应该自己通过测量没有声音时的数值来确定这里应该减去多少
    // 当没有声音的时候，减去偏置后得到的数值应该是在0附近，说明减的偏置就是正确的
    mic_raw_data_1[mic_raw_data_count] = adc_convert(MIC_PIN_1) - adc_offset+120;
    mic_raw_data_2[mic_raw_data_count] = adc_convert(MIC_PIN_2) - adc_offset+150;
    mic_raw_data_3[mic_raw_data_count] = adc_convert(MIC_PIN_3) - adc_offset+129;
    mic_raw_data_4[mic_raw_data_count] = adc_convert(MIC_PIN_4) - adc_offset+131;
    

    // 采集点数加一
    mic_raw_data_count++;

    if(mic_raw_data_count >= 2500)       // 判断当前采集点数是否已经达到数组末尾
    {
        mic_raw_data_count = 0;          // 采集次数清零  实现循环
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     复制硅麦数据(最近的 FFT_SIZE 个)
// 参数说明     void
// 返回参数     void   
// 使用示例     mic_data_copy();
// 备注信息     取出循环数组最近的2048个数据用于互相关计算
//-------------------------------------------------------------------------------------------------------------------
void mic_data_copy(void) 
{
    uint16 mic_raw_data_count_temp = 0;         
    uint16 mic_data_count = 0;
    
    mic_raw_data_count_save = mic_raw_data_count;       // 保存当前时刻的采集位置( mic_raw_data_count 会在中断被持续更新，保存以防止数据异常）
    
    if(mic_raw_data_count_save < FFT_SIZE)              // 若当前采集位置小于需复制的长度(无法一个循环完成复制)，则分为两端分别复制数据
    {
        mic_raw_data_count_temp = MIC_RAW_DATA_LEN - (FFT_SIZE - mic_raw_data_count_save);      
        
        for(int16 i = mic_raw_data_count_temp;i < MIC_RAW_DATA_LEN; i ++)
        {
            fft_signal_1[mic_data_count * 2] = mic_raw_data_1[i];             // 保存用于FFT计算的数据 实部赋值
            fft_signal_2[mic_data_count * 2] = mic_raw_data_2[i];
            fft_signal_3[mic_data_count * 2] = mic_raw_data_3[i];             
            fft_signal_4[mic_data_count * 2] = mic_raw_data_4[i];
            fft_signal_1[mic_data_count * 2 + 1] = 0.0f;                          // 保存用于FFT计算的数据 虚部赋值
            fft_signal_2[mic_data_count * 2 + 1] = 0.0f;
            fft_signal_3[mic_data_count * 2 + 1] = 0.0f;                          
            fft_signal_4[mic_data_count * 2 + 1] = 0.0f;
            /*
            one_signal[0][mic_data_count * 2] = mic_raw_data[0][i];             // 保存一个副本，用于归一化之后进行互相关
            one_signal[1][mic_data_count * 2] = mic_raw_data[1][i];
            one_signal[2][mic_data_count * 2] = mic_raw_data[2][i];             
            one_signal[3][mic_data_count * 2] = mic_raw_data[3][i];
            one_signal[0][mic_data_count * 2 + 1] = 0.0f;                         
            one_signal[1][mic_data_count * 2 + 1] = 0.0f;
            one_signal[2][mic_data_count * 2 + 1] = 0.0f;                          
            one_signal[3][mic_data_count * 2 + 1] = 0.0f;*/
            mic_data_count ++;
        }
        for(int16 i = 0;i < mic_raw_data_count_save; i ++)
        {
            fft_signal_1[mic_data_count * 2] = mic_raw_data_1[i];             // 保存用于FFT计算的数据 实部赋值
            fft_signal_2[mic_data_count * 2] = mic_raw_data_2[i];
            fft_signal_3[mic_data_count * 2] = mic_raw_data_3[i];             
            fft_signal_4[mic_data_count * 2] = mic_raw_data_4[i];
            fft_signal_1[mic_data_count * 2 + 1] = 0.0f;
            fft_signal_2[mic_data_count * 2 + 1] = 0.0f;                          // 保存用于FFT计算的数据 虚部赋值
            fft_signal_3[mic_data_count * 2 + 1] = 0.0f;
            fft_signal_4[mic_data_count * 2 + 1] = 0.0f; 
            /*
            one_signal[0][mic_data_count * 2] = mic_raw_data[0][i];             // 保存一个副本，用于归一化之后进行互相关
            one_signal[1][mic_data_count * 2] = mic_raw_data[1][i];
            one_signal[2][mic_data_count * 2] = mic_raw_data[2][i];             
            one_signal[3][mic_data_count * 2] = mic_raw_data[3][i];
            one_signal[0][mic_data_count * 2 + 1] = 0.0f;                         
            one_signal[1][mic_data_count * 2 + 1] = 0.0f;
            one_signal[2][mic_data_count * 2 + 1] = 0.0f;                          
            one_signal[3][mic_data_count * 2 + 1] = 0.0f;*/
            mic_data_count ++;
        }
    }
    else                                                                        // 若当前采集位置大于需复制的长度，则直接一个循环完成复制
    {
        for(int16 i = 0;i < FFT_SIZE; i ++)
        {
            fft_signal_1[i * 2] = mic_raw_data_1[i];                          // 保存用于FFT计算的数据 实部赋值
            fft_signal_2[i * 2] = mic_raw_data_2[i];
            fft_signal_3[i * 2] = mic_raw_data_3[i];                          // 保存用于FFT计算的数据 实部赋值
            fft_signal_4[i * 2] = mic_raw_data_4[i];
            fft_signal_1[i * 2 + 1] = 0.0f;                                       // 保存用于FFT计算的数据 虚部赋值
            fft_signal_2[i * 2 + 1] = 0.0f;  
            fft_signal_3[i * 2 + 1] = 0.0f;                                       // 保存用于FFT计算的数据 虚部赋值
            fft_signal_4[i * 2 + 1] = 0.0f;  
            /*
            one_signal[0][i * 2] = mic_raw_data[0][i];                          // 保存用于FFT计算的数据 实部赋值
            one_signal[1][i * 2] = mic_raw_data[1][i];
            one_signal[2][i * 2] = mic_raw_data[2][i];                          // 保存用于FFT计算的数据 实部赋值
            one_signal[3][i * 2] = mic_raw_data[3][i];
            one_signal[0][i * 2 + 1] = 0.0f;                                       // 保存用于FFT计算的数据 虚部赋值
            one_signal[1][i * 2 + 1] = 0.0f;  
            one_signal[2][i * 2 + 1] = 0.0f;                                       // 保存用于FFT计算的数据 虚部赋值
            one_signal[3][i * 2 + 1] = 0.0f; 
              */ 
            
        }
    }
}





void test_data_copy(void)
{
    for(int i=0;i<FFT_SIZE+10;i++)
    {
        sin_signal[i]=arm_sin_f32(PI*i/10);
    
    }
    
        for(int16 i = 0;i < FFT_SIZE; i ++)
        {
            fft_signal_1[i * 2] = sin_signal[i];                          // 保存用于FFT计算的数据 实部赋值
            fft_signal_2[i * 2] = sin_signal[i+5];
            fft_signal_3[i * 2] = sin_signal[i+5];                          // 保存用于FFT计算的数据 实部赋值
            fft_signal_4[i * 2] = sin_signal[i];
            fft_signal_1[i * 2 + 1] = 0.0f;                                       // 保存用于FFT计算的数据 虚部赋值
            fft_signal_2[i * 2 + 1] = 0.0f;  
            fft_signal_3[i * 2 + 1] = 0.0f;                                       // 保存用于FFT计算的数据 虚部赋值
            fft_signal_4[i * 2 + 1] = 0.0f;  
            
        }

}








