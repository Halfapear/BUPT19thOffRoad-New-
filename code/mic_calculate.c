#include "mic_calculate.h"
#include "mic.h"
#include "arm_math.h"

float count_signal_0[FFT_SIZE*2];//�������������
float count_signal_1[FFT_SIZE*2];//�������������
float out_signal[2][FFT_SIZE];
float fft_outputSignal[4][FFT_SIZE];
float Chrip_signal[FFT_SIZE*2];//��׼Chrip�ź�
float one_signal[4][2*FFT_SIZE]; //��һ��֮����ź�����--
float rcy;//��ض�
int MIC_rcyflag=1;
//extern float RcyThredNum;
float RcyThredNum=3.14;
float d1=0;
float d2=0;

void Get_mic_Distance()
{
    int16 i=0;
    float pmax=0; //���ֵ
    int16 qmax=0; //����±�ֵ
    int16 num=30;//���ֵ��������
    
    //����fft����
    arm_cfft_instance_f32 arm_cfft_instance_f32_len_2048;
    arm_cfft_init_f32(&arm_cfft_instance_f32_len_2048, FFT_SIZE);
    
    arm_cfft_instance_f32 arm_cfft_instance_f32_len_1000;
    arm_cfft_init_f32(&arm_cfft_instance_f32_len_1000, FFT_SIZE);
    
    //fft
    arm_cfft_f32(&arm_cfft_instance_f32_len_2048, fft_signal_1, 0, 1);
    arm_cfft_f32(&arm_cfft_instance_f32_len_1000, fft_signal_2, 0, 1);
    arm_cfft_f32(&arm_cfft_instance_f32_len_2048, fft_signal_3, 0, 1);
    arm_cfft_f32(&arm_cfft_instance_f32_len_1000, fft_signal_4, 0, 1);
    
    
    arm_cmplx_mag_f32(fft_signal_1, fft_outputSignal[0], FFT_SIZE);               // ��FFT���ת��Ϊ������(ת����������Ϊ�˹۲첨��)
    arm_cmplx_mag_f32(fft_signal_3, fft_outputSignal[1], FFT_SIZE);       
    //�ź�2������
    for(i = 0; i < FFT_SIZE; i++)
    {
        fft_signal_3[i*2+1]=-fft_signal_3[i*2+1];
        fft_signal_4[i*2+1]=-fft_signal_4[i*2+1];
        
    }
    
    //�����˷�
    arm_cmplx_mult_cmplx_f32(fft_signal_1, fft_signal_3, count_signal_0, FFT_SIZE);
    arm_cmplx_mult_cmplx_f32(fft_signal_2, fft_signal_4, count_signal_1, FFT_SIZE);
    
    //��fft����
    arm_cfft_f32(&arm_cfft_instance_f32_len_2048, count_signal_0, 1, 1);
    arm_cfft_f32(&arm_cfft_instance_f32_len_1000, count_signal_1, 1, 1);
    
   
    //���һ���-ֻ��ʵ��
    for(i = 0; i<FFT_SIZE; i++)
    {
        float temp=count_signal_0[i*2];
        count_signal_0[i*2]=count_signal_0[i*2+2048];
        count_signal_0[i*2+2048]=temp;
        
        float temp1=count_signal_1[i*2];
        count_signal_1[i*2]=count_signal_1[i*2+2048];
        count_signal_1[i*2+2048]=temp1;
        
    }
    //arm_cmplx_mag_f32(count_signal_0, out_signal[0] , FFT_SIZE);
    //arm_cmplx_mag_f32(count_signal_1, out_signal[1] , FFT_SIZE);
    
    
    
    
    
    
    pmax=count_signal_1[FFT_SIZE-(num*2)];
    qmax=FFT_SIZE/2-num;
    
    for(i = (FFT_SIZE/2)-num; i<(FFT_SIZE/2)+num; i++)
    {
        if(count_signal_1[i*2]>pmax)
        {
            pmax=count_signal_1[i*2];
            qmax=i;
        }
    }
    printf("\r\nqmax2:%d",qmax);
    d2=(FFT_SIZE/2-qmax)*(3.40f);
    
    pmax=count_signal_0[FFT_SIZE-(num*2)];
    qmax=FFT_SIZE/2-num;
    
    for(i = (FFT_SIZE/2)-num; i<(FFT_SIZE/2)+num; i++)
    {
        if(count_signal_0[i*2]>pmax)
        {
            pmax=count_signal_0[i*2];
            qmax=i;
        }
    }
    d1=(FFT_SIZE/2-qmax)*(3.40f);
    printf("\r\nqmax1:%d",qmax);
    
    
    //10khz����Ƶ���µ�ʱ�Ӳ1��ʱ�Ӳ������0.0001��������(340m/s)��Ҳ����3.4cm
    //������������˷�֮��ľ������ݼ���Ч
    //��������1����������2
   // return (FFT_SIZE/2-qmax)*(3.40f);
    


}

float Get_mic_Angle(void)      //����ػ�ýǶ�
{
    //float d1=0,d2=0;//d1ǰ�����d2��������
    float angle;
    
    Get_mic_Distance();
   
    //����ŵ��Դ���
    printf("\r\n�����1:%f\r\n�����2:%f",d1,d2 );
    
    //����Ƕȵľ���ֵ���ٽ������޺��������ж�
    angle=atan2f(fabs(d2),fabs(d1));
    if(d1>0&&d2>0)
    {
        angle=angle;
    }
    else if(d1>0&&d2<0)
    {
        angle=-angle;
    }
    else if(d1<0&&d2<0)
    {
        angle=angle-180.0;
    }
    else
    {
        angle=180.0-angle;
    }
    
    //�Ƕ�ʹ��-180~180ϵͳ
    
    return angle;
    
    
}

//�õ�һ��-1��1��chrip�ź�
void generateChirp(float signal[], int length, float f0, float f1, int fs) 
{
    float t; // ʱ�����
    for (int i = 0; i < length; i++) {
        t = (float)i / fs; // ���㵱ǰʱ��
        // ���Ա仯��Ƶ��
        float f = f0 + (f1 - f0) * t * fs / length;
        // ���㵱ǰ���������λ�������ź�
        signal[i*2] = sinf(2 * PI * f * t);
        signal[i*2+1]=0;
    }
}


void ChripPipei()
{
    //��һ������--��ʱ����һ������Ȼ�������׼ȷ�ʸߵ�
    //normalizeSignal(one_signal[0],FFT_SIZE);
     
    //���������
    //rcy=Get_mic_Distance(one_signal[0],Chrip_signal,count_signal[0],fft_outputSignal[0],fft_outputSignal[1]);
    
    if(fabsf(rcy)<RcyThredNum)
    {
        MIC_rcyflag=1;
        SPEED_Value=40;
    }
    else
    {
        MIC_rcyflag=0;
        SPEED_Value=0;
    }
    
    
    
}


void normalizeSignal(float signal[], int length) {
    float mean = 0.0;
    float std = 0.0;
    float sum = 0.0;

    // �����ֵ
    for (int i = 0; i < length; i++) {
        sum += signal[i*2];
    }
    mean = sum / length;

    // �����׼��
    sum = 0.0;
    for (int i = 0; i < length; i++) {
        sum += (signal[i*2] - mean) * (signal[i*2] - mean);
    }
    std = sqrt(sum / length);
    
    // ��һ���ź�
    for (int i = 0; i < length; i++) {
        signal[i*2] = (signal[i*2] - mean) / std;
    }
}




