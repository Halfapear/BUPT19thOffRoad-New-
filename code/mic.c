#include "mic.h"

int16 mic_raw_data[4][MIC_RAW_DATA_LEN];        // ����ɼ���Ҫ�õ���ѭ������
int16 mic_raw_data_count;                       // ����ѭ���������λ
int16 mic_raw_data_count_save;                  // ����ѭ���������λ-����ֵ(����ȡ������� FFT_SIZE ������)


int16 mic_raw_data_1[MIC_RAW_DATA_LEN];
int16 mic_raw_data_2[MIC_RAW_DATA_LEN];
int16 mic_raw_data_3[MIC_RAW_DATA_LEN];
int16 mic_raw_data_4[MIC_RAW_DATA_LEN];


float sin_signal[FFT_SIZE+10];

//float fft_signal[4][2*FFT_SIZE];                // ����ȡ�����ݵı���λ��
float fft_signal_1[2*FFT_SIZE];  
float fft_signal_2[2*FFT_SIZE];  
float fft_signal_3[2*FFT_SIZE];  
float fft_signal_4[2*FFT_SIZE];  


int16 adc_offset=2048;                          //adc��̬���
//float����Ϊ���Ǽ���֮����ܻ����С��

//adc��ʼ��
void mic_init()
{
    adc_init(MIC_PIN_1, ADC_12BIT);
    adc_init(MIC_PIN_2, ADC_12BIT); 
    adc_init(MIC_PIN_3, ADC_12BIT);
    adc_init(MIC_PIN_4, ADC_12BIT); 
}

//-------------------------------------------------------------------------------------------------------------------
// �������     �ɼ���˷�����
// ����˵��     void
// ���ز���     void   
// ʹ��ʾ��     mic_data_get();
// ��ע��Ϣ     �ú������ж��е���
//-------------------------------------------------------------------------------------------------------------------
void mic_data_get (void)
{
    //adc����offset��̬���
    // ��Ϊ����FFT�����ʱ��������Ҫ��ȥֱ��ƫ�����������̶���2048��ʵ��Ӧ���Լ�ͨ������û������ʱ����ֵ��ȷ������Ӧ�ü�ȥ����
    // ��û��������ʱ�򣬼�ȥƫ�ú�õ�����ֵӦ������0������˵������ƫ�þ�����ȷ��
    mic_raw_data_1[mic_raw_data_count] = adc_convert(MIC_PIN_1) - adc_offset+120;
    mic_raw_data_2[mic_raw_data_count] = adc_convert(MIC_PIN_2) - adc_offset+150;
    mic_raw_data_3[mic_raw_data_count] = adc_convert(MIC_PIN_3) - adc_offset+129;
    mic_raw_data_4[mic_raw_data_count] = adc_convert(MIC_PIN_4) - adc_offset+131;
    

    // �ɼ�������һ
    mic_raw_data_count++;

    if(mic_raw_data_count >= 2500)       // �жϵ�ǰ�ɼ������Ƿ��Ѿ��ﵽ����ĩβ
    {
        mic_raw_data_count = 0;          // �ɼ���������  ʵ��ѭ��
    }
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ���ƹ�������(����� FFT_SIZE ��)
// ����˵��     void
// ���ز���     void   
// ʹ��ʾ��     mic_data_copy();
// ��ע��Ϣ     ȡ��ѭ�����������2048���������ڻ���ؼ���
//-------------------------------------------------------------------------------------------------------------------
void mic_data_copy(void) 
{
    uint16 mic_raw_data_count_temp = 0;         
    uint16 mic_data_count = 0;
    
    mic_raw_data_count_save = mic_raw_data_count;       // ���浱ǰʱ�̵Ĳɼ�λ��( mic_raw_data_count �����жϱ��������£������Է�ֹ�����쳣��
    
    if(mic_raw_data_count_save < FFT_SIZE)              // ����ǰ�ɼ�λ��С���踴�Ƶĳ���(�޷�һ��ѭ����ɸ���)�����Ϊ���˷ֱ�������
    {
        mic_raw_data_count_temp = MIC_RAW_DATA_LEN - (FFT_SIZE - mic_raw_data_count_save);      
        
        for(int16 i = mic_raw_data_count_temp;i < MIC_RAW_DATA_LEN; i ++)
        {
            fft_signal_1[mic_data_count * 2] = mic_raw_data_1[i];             // ��������FFT��������� ʵ����ֵ
            fft_signal_2[mic_data_count * 2] = mic_raw_data_2[i];
            fft_signal_3[mic_data_count * 2] = mic_raw_data_3[i];             
            fft_signal_4[mic_data_count * 2] = mic_raw_data_4[i];
            fft_signal_1[mic_data_count * 2 + 1] = 0.0f;                          // ��������FFT��������� �鲿��ֵ
            fft_signal_2[mic_data_count * 2 + 1] = 0.0f;
            fft_signal_3[mic_data_count * 2 + 1] = 0.0f;                          
            fft_signal_4[mic_data_count * 2 + 1] = 0.0f;
            /*
            one_signal[0][mic_data_count * 2] = mic_raw_data[0][i];             // ����һ�����������ڹ�һ��֮����л����
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
            fft_signal_1[mic_data_count * 2] = mic_raw_data_1[i];             // ��������FFT��������� ʵ����ֵ
            fft_signal_2[mic_data_count * 2] = mic_raw_data_2[i];
            fft_signal_3[mic_data_count * 2] = mic_raw_data_3[i];             
            fft_signal_4[mic_data_count * 2] = mic_raw_data_4[i];
            fft_signal_1[mic_data_count * 2 + 1] = 0.0f;
            fft_signal_2[mic_data_count * 2 + 1] = 0.0f;                          // ��������FFT��������� �鲿��ֵ
            fft_signal_3[mic_data_count * 2 + 1] = 0.0f;
            fft_signal_4[mic_data_count * 2 + 1] = 0.0f; 
            /*
            one_signal[0][mic_data_count * 2] = mic_raw_data[0][i];             // ����һ�����������ڹ�һ��֮����л����
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
    else                                                                        // ����ǰ�ɼ�λ�ô����踴�Ƶĳ��ȣ���ֱ��һ��ѭ����ɸ���
    {
        for(int16 i = 0;i < FFT_SIZE; i ++)
        {
            fft_signal_1[i * 2] = mic_raw_data_1[i];                          // ��������FFT��������� ʵ����ֵ
            fft_signal_2[i * 2] = mic_raw_data_2[i];
            fft_signal_3[i * 2] = mic_raw_data_3[i];                          // ��������FFT��������� ʵ����ֵ
            fft_signal_4[i * 2] = mic_raw_data_4[i];
            fft_signal_1[i * 2 + 1] = 0.0f;                                       // ��������FFT��������� �鲿��ֵ
            fft_signal_2[i * 2 + 1] = 0.0f;  
            fft_signal_3[i * 2 + 1] = 0.0f;                                       // ��������FFT��������� �鲿��ֵ
            fft_signal_4[i * 2 + 1] = 0.0f;  
            /*
            one_signal[0][i * 2] = mic_raw_data[0][i];                          // ��������FFT��������� ʵ����ֵ
            one_signal[1][i * 2] = mic_raw_data[1][i];
            one_signal[2][i * 2] = mic_raw_data[2][i];                          // ��������FFT��������� ʵ����ֵ
            one_signal[3][i * 2] = mic_raw_data[3][i];
            one_signal[0][i * 2 + 1] = 0.0f;                                       // ��������FFT��������� �鲿��ֵ
            one_signal[1][i * 2 + 1] = 0.0f;  
            one_signal[2][i * 2 + 1] = 0.0f;                                       // ��������FFT��������� �鲿��ֵ
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
            fft_signal_1[i * 2] = sin_signal[i];                          // ��������FFT��������� ʵ����ֵ
            fft_signal_2[i * 2] = sin_signal[i+5];
            fft_signal_3[i * 2] = sin_signal[i+5];                          // ��������FFT��������� ʵ����ֵ
            fft_signal_4[i * 2] = sin_signal[i];
            fft_signal_1[i * 2 + 1] = 0.0f;                                       // ��������FFT��������� �鲿��ֵ
            fft_signal_2[i * 2 + 1] = 0.0f;  
            fft_signal_3[i * 2 + 1] = 0.0f;                                       // ��������FFT��������� �鲿��ֵ
            fft_signal_4[i * 2 + 1] = 0.0f;  
            
        }

}








