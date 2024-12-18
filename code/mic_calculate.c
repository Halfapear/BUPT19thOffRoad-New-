#include "mic_calculate.h"
#include "mic.h"
#include "arm_math.h"

float count_signal_0[FFT_SIZE*2];//互相关运算数组
float count_signal_1[FFT_SIZE*2];//互相关运算数组
float out_signal_0[FFT_SIZE];//可视化输出数组
float out_signal_1[FFT_SIZE];
float fft_outputSignal[4][FFT_SIZE];
float Chrip_signal[FFT_SIZE*2];//标准Chrip信号
float one_signal[4][2*FFT_SIZE]; //归一化之后的信号数组--
float rcy;//相关度
int MIC_rcyflag=0;
//extern float RcyThredNum;

float angle_filter_buffer[FILTER_SIZE] = {0};  // 角度值缓存数组
int filter_index = 0;  // 滤波数组索引

float RcyThredNum=3.14;
float d1=0;
float d2=0;

void Get_mic_Distance()
{
    int16_t i=0;
    float pmax=0; //最大值
    int16_t qmax=0; //最大下标值
    int16_t num=30;//最大值搜索点数
    
    //定义fft对象
    arm_cfft_instance_f32 arm_cfft_instance_f32_len_2048;
    arm_cfft_init_f32(&arm_cfft_instance_f32_len_2048, FFT_SIZE);
    
    arm_cfft_instance_f32 arm_cfft_instance_f32_len_1000;
    arm_cfft_init_f32(&arm_cfft_instance_f32_len_1000, FFT_SIZE);
    
    //fft
    arm_cfft_f32(&arm_cfft_instance_f32_len_2048, fft_signal_1, 0, 1);
    arm_cfft_f32(&arm_cfft_instance_f32_len_1000, fft_signal_2, 0, 1);
    arm_cfft_f32(&arm_cfft_instance_f32_len_2048, fft_signal_3, 0, 1);
    arm_cfft_f32(&arm_cfft_instance_f32_len_1000, fft_signal_4, 0, 1);
    
    
    arm_cmplx_mag_f32(fft_signal_1, fft_outputSignal[0], FFT_SIZE);               // 将FFT结果转换为幅度谱(转换幅度谱是为了观察波形)
    arm_cmplx_mag_f32(fft_signal_3, fft_outputSignal[1], FFT_SIZE);       
    //信号2复共轭
    for(i = 0; i < FFT_SIZE; i++)
    {
        fft_signal_3[i*2+1]=-fft_signal_3[i*2+1];
        fft_signal_4[i*2+1]=-fft_signal_4[i*2+1];
        
    }
    
    //复数乘法
    arm_cmplx_mult_cmplx_f32(fft_signal_1, fft_signal_3, count_signal_0, FFT_SIZE);
    arm_cmplx_mult_cmplx_f32(fft_signal_2, fft_signal_4, count_signal_1, FFT_SIZE);
    
    //逆fft运算
    arm_cfft_f32(&arm_cfft_instance_f32_len_2048, count_signal_0, 1, 1);
    arm_cfft_f32(&arm_cfft_instance_f32_len_1000, count_signal_1, 1, 1);
    
   
    //左右互换-只换实部
    for(i = 0; i<FFT_SIZE/2; i++)
    {
        float temp=count_signal_0[i*2];
        count_signal_0[i*2]=count_signal_0[i*2+2048];
        count_signal_0[i*2+2048]=temp;
        
        float temp1=count_signal_1[i*2];
        count_signal_1[i*2]=count_signal_1[i*2+2048];
        count_signal_1[i*2+2048]=temp1;
        
    }
    arm_cmplx_mag_f32(count_signal_0, out_signal_0 , FFT_SIZE);
    arm_cmplx_mag_f32(count_signal_1, out_signal_1 , FFT_SIZE);
    
    
    
    
    
    
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
    
    
    //10khz采样频率下的时延差，1个时延差则代表0.0001乘以音速(340m/s)，也就是3.4cm
    //如果距离大于麦克风之间的距离数据即无效
    //正数靠近1，负数靠近2
   // return (FFT_SIZE/2-qmax)*(3.40f);
    


}

double calculate_angle_with_y_axis(double x, double y) {
    double angle_with_x_axis = fast_atan2(y, x);
    double angle_with_y_axis = PI / 2 - angle_with_x_axis;
    return angle_with_y_axis*(180/PI);
}

float Get_mic_Angle(void)      //互相关获得角度
{
    //float d1=0,d2=0;//d1前后距离差，d2右左距离差
    float angle=90.0;
    
    Get_mic_Distance();
    
    //这里放调试代码
    printf("\r\n距离差1:%f\r\n距离差2:%f",d1,d2 );
    if(fabsf(d1)>27)
      d1=0;
    if(fabsf(d2)>27)
      d2=0;
    
    
    //先算角度的绝对值，再进行象限和正负的判断
    
    if(fabsf(d2)<3.3&&d1>21.0)//横向值接近0，走直线
    {
       angle=90.0;
    }
    else if(fabsf(d2)<3.3&&d1<-15.0)
    {
        angle=0.0;
    }
    else if(fabsf(d1)<3.3&&d2>12.0)//
    {
        angle=0.0;
    }
    else if(fabsf(d1)<3.3&&d2<-12.0)
    {
        angle=180;
    }
    else if(fabsf(d1)>3.5&&fabsf(d2)>3.5&&fabsf(d1)<27&&fabsf(d2)<27){
        angle=calculate_angle_with_y_axis(d1,d2);
        
    }
    
    
    
    
    //角度使用-180~180系统
    
    return angle;
    
    
}


// 移动平均滤波函数
float Moving_Average_Filter(float new_angle) {
    float sum = 0.0;
    int count;

    // 将新的角度值添加到数组中
    angle_filter_buffer[filter_index++] = new_angle;

    // 如果数组已满，重置索引以循环存储新值
    if (filter_index >= FILTER_SIZE) {
        filter_index = 0;
    }

    // 计算数组中所有值的平均值
    for (count = 0; count < FILTER_SIZE; count++) {
        sum += angle_filter_buffer[count];
    }

    return sum / FILTER_SIZE;  // 返回平均值
}

// 修改后的Get_mic_Angle函数，使用滤波器
float Get_mic_Angle_Filtered(void) {
    float raw_angle = Get_mic_Angle();  // 获取原始角度值
    return Moving_Average_Filter(raw_angle);  // 返回滤波后的角度值
}

//得到一个-1到1的chrip信号
void generateChirp(float signal[], int length, float f0, float f1, int fs) 
{
    float t; // 时间变量
    for (int i = 0; i < length; i++) {
        t = (float)i / fs; // 计算当前时间
        // 线性变化的频率
        float f = f0 + (f1 - f0) * t * fs / length;
        // 计算当前采样点的相位并生成信号
        signal[i*2] = sinf(2 * PI * f * t);
        signal[i*2+1]=0;
    }
}


void ChripPipei()
{
    //归一化处理--暂时先算一个，虽然多算可能准确率高点
    //normalizeSignal(one_signal[0],FFT_SIZE);
     
    //互相关运算
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

    // 计算均值
    for (int i = 0; i < length; i++) {
        sum += signal[i*2];
    }
    mean = sum / length;

    // 计算标准差
    sum = 0.0;
    for (int i = 0; i < length; i++) {
        sum += (signal[i*2] - mean) * (signal[i*2] - mean);
    }
    std = sqrt(sum / length);
    
    // 归一化信号
    for (int i = 0; i < length; i++) {
        signal[i*2] = (signal[i*2] - mean) / std;
    }
}




