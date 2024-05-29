/*********************************************************************************************************************
* CYT4BB Opensourec Library ���� CYT4BB ��Դ�⣩��һ�����ڹٷ� SDK �ӿڵĵ�������Դ��
* Copyright (c) 2022 SEEKFREE ��ɿƼ�
*
* ���ļ��� CYT4BB ��Դ���һ����
*
* CYT4BB ��Դ�� ��������
* �����Ը��������������ᷢ���� GPL��GNU General Public License���� GNUͨ�ù������֤��������
* �� GPL �ĵ�3�棨�� GPL3.0������ѡ��ģ��κκ����İ汾�����·�����/���޸���
*
* ����Դ��ķ�����ϣ�����ܷ������ã�����δ�������κεı�֤
* ����û�������������Ի��ʺ��ض���;�ı�֤
* ����ϸ����μ� GPL
*
* ��Ӧ�����յ�����Դ���ͬʱ�յ�һ�� GPL �ĸ���
* ���û�У������<https://www.gnu.org/licenses/>
*
* ����ע����
* ����Դ��ʹ�� GPL3.0 ��Դ���֤Э�� �����������Ϊ���İ汾
* �������Ӣ�İ��� libraries/doc �ļ����µ� GPL3_permission_statement.txt �ļ���
* ���֤������ libraries �ļ����� �����ļ����µ� LICENSE �ļ�
* ��ӭ��λʹ�ò����������� ���޸�����ʱ���뱣����ɿƼ��İ�Ȩ����������������
*
* �ļ�����          zf_driver_adc
* ��˾����          �ɶ���ɿƼ����޹�˾
* �汾��Ϣ          �鿴 libraries/doc �ļ����� version �ļ� �汾˵��
* ��������          IAR 9.40.1
* ����ƽ̨          CYT4BB
* ��������          https://seekfree.taobao.com/
*
* �޸ļ�¼
* ����              ����                ��ע
* 2024-1-8       pudding            first version
* 2024-1-29      pudding            �Ż���߲ɼ��ٶ� ���βɼ�ʱ��0.525us����
********************************************************************************************************************/

#include "sysclk/cy_sysclk.h"
#include "gpio/cy_gpio.h"
#include "zf_driver_gpio.h"
#include "adc/cy_adc.h"
#include "zf_common_debug.h"
#include "zf_driver_adc.h"

#define ADC_FREQ                                CY_INITIAL_TARGET_PERI_FREQ     // ADCģ��ʱ�� Ĭ��80M
#define ADC_OPERATION_FREQUENCY_MAX_IN_HZ       (13340000ul)                    // ADC����Ƶ�� ���13.34M
#define ANALOG_IN_SAMPLING_TIME_MIN_IN_NS       (600ull)                        // ADC����ʱ�� ���600ns
#define DIV_ROUND_UP(a,b)                       (((a) + (b)/2) / (b))           // ����ϵ������ ����ȡ��

static vuint8 adc_resolution[ADC_CHANNEL_NUM];
static volatile stc_PASS_SAR_t* sar_adc[3] = {PASS0_SAR0, PASS0_SAR1, PASS0_SAR2};

//-------------------------------------------------------------------------------------------------------------------
// �������     ADC �˿ںŻ�ȡ
// ����˵��     vadc_chn        ѡ�� ADC ͨ�� (��� zf_driver_adc.h ��ö�� adc_channel_enum ����)
// ���ز���     gpio_pin_enum   ADCͨ����Ӧ��GPIO����
// ʹ��ʾ��     get_adc_pin(ADC0_CH0_P6_0);
// ��ע��Ϣ     �ڲ����� �û��������
//-------------------------------------------------------------------------------------------------------------------
static gpio_pin_enum get_adc_pin(adc_channel_enum vadc_chn)
{
    gpio_pin_enum adc_pin;
    switch(vadc_chn)
    {
        case ADC0_CH00_P06_0	: adc_pin = P06_0; break;
        case ADC0_CH01_P06_1	: adc_pin = P06_1; break;
        case ADC0_CH02_P06_2	: adc_pin = P06_2; break;
        case ADC0_CH03_P06_3	: adc_pin = P06_3; break;
        case ADC0_CH04_P06_4	: adc_pin = P06_4; break;
        case ADC0_CH05_P06_5	: adc_pin = P06_5; break;
        case ADC0_CH06_P06_6	: adc_pin = P06_6; break;
        case ADC0_CH07_P06_7	: adc_pin = P06_7; break;
        case ADC0_CH16_P07_0	: adc_pin = P07_0; break;
        case ADC0_CH17_P07_1	: adc_pin = P07_1; break;
        case ADC0_CH18_P07_2	: adc_pin = P07_2; break;
        case ADC0_CH19_P07_3	: adc_pin = P07_3; break;
        case ADC0_CH20_P07_4	: adc_pin = P07_4; break;
        case ADC0_CH21_P07_5	: adc_pin = P07_5; break;
        case ADC0_CH22_P07_6	: adc_pin = P07_6; break;
        case ADC0_CH23_P07_7	: adc_pin = P07_7; break;
        case ADC0_CH24_P08_1	: adc_pin = P08_1; break;
        case ADC0_CH25_P08_2	: adc_pin = P08_2; break;
        case ADC0_CH26_P08_3	: adc_pin = P08_3; break;
        case ADC0_CH28_P09_0	: adc_pin = P09_0; break;
        case ADC0_CH29_P09_1	: adc_pin = P09_1; break;
        case ADC1_CH00_P10_4	: adc_pin = P10_4; break;
        case ADC1_CH04_P12_0	: adc_pin = P12_0; break;
        case ADC1_CH05_P12_1	: adc_pin = P12_1; break;
        case ADC1_CH06_P12_2	: adc_pin = P12_2; break;
        case ADC1_CH07_P12_3	: adc_pin = P12_3; break;
        case ADC1_CH08_P12_4	: adc_pin = P12_4; break;
        case ADC1_CH09_P12_5	: adc_pin = P12_5; break;
        case ADC1_CH12_P13_0	: adc_pin = P13_0; break;
        case ADC1_CH13_P13_1	: adc_pin = P13_1; break;
        case ADC1_CH14_P13_2	: adc_pin = P13_2; break;
        case ADC1_CH15_P13_3	: adc_pin = P13_3; break;
        case ADC1_CH16_P13_4	: adc_pin = P13_4; break;
        case ADC1_CH17_P13_5	: adc_pin = P13_5; break;
        case ADC1_CH18_P13_6	: adc_pin = P13_6; break;
        case ADC1_CH19_P13_7	: adc_pin = P13_7; break;
        case ADC1_CH20_P14_0	: adc_pin = P14_0; break;
        case ADC1_CH21_P14_1	: adc_pin = P14_1; break;
        case ADC1_CH24_P14_4	: adc_pin = P14_4; break;
        case ADC1_CH25_P14_5	: adc_pin = P14_5; break;
        case ADC1_CH28_P15_0	: adc_pin = P15_0; break;
        case ADC1_CH29_P15_1	: adc_pin = P15_1; break;
        case ADC1_CH30_P15_2	: adc_pin = P15_2; break;
        case ADC2_CH00_P18_0	: adc_pin = P18_0; break;
        case ADC2_CH01_P18_1	: adc_pin = P18_1; break;
        case ADC2_CH02_P18_2	: adc_pin = P18_2; break;
        case ADC2_CH03_P18_3	: adc_pin = P18_3; break;
        case ADC2_CH04_P18_4	: adc_pin = P18_4; break;
        case ADC2_CH05_P18_5	: adc_pin = P18_5; break;
        case ADC2_CH06_P18_6	: adc_pin = P18_6; break;
        case ADC2_CH07_P18_7	: adc_pin = P18_7; break;
        default:break;
    }
    return adc_pin;
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ADC ת��һ��
// ����˵��     ch              ѡ�� ADC ͨ�� (��� zf_driver_adc.h ��ö�� adc_channel_enum ����)
// ���ز���     uint16          ת���� ADC ֵ
// ʹ��ʾ��     adc_convert(ADC0_CH00_P06_0);
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
uint16 adc_convert (adc_channel_enum adc_chn)
{
    uint8 offset_temp;
    uint8 adc_port      = adc_chn / 32;
    uint8 adc_channel   = adc_chn % 32;
    uint16 adc_data;
    
    offset_temp = 4 - (adc_resolution[adc_chn] * 2);
    
    sar_adc[adc_port]->CH[adc_channel].unTR_CMD.stcField.u1START = 1ul;
    
    adc_data = sar_adc[adc_port]->CH[adc_channel].unRESULT.stcField.u16RESULT;
    
    adc_data = adc_data >> offset_temp;
    
    return adc_data;
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ADC ��ֵ�˲�ת��
// ����˵��     ch              ѡ�� ADC ͨ�� (��� zf_driver_adc.h ��ö�� adc_channel_enum ����)
// ����˵��     count           ��ֵ�˲�����
// ���ز���     uint16          ת���� ADC ֵ
// ʹ��ʾ��     adc_mean_filter_convert(ADC0_CH00_P06_0, 5);                        // �ɼ�5�� Ȼ�󷵻�ƽ��ֵ
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
uint16 adc_mean_filter_convert (adc_channel_enum vadc_chn, uint8 count)
{
    uint8 i;
    uint32 sum;

    zf_assert(count);// ���Դ�������Ϊ0

    sum = 0;
    for(i=0; i<count; i++)
    {
        sum += adc_convert(vadc_chn);
    }

    sum = sum/count;

    return (uint16)sum;
}


//-------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------
// �������     ADC ��ʼ��
// ����˵��     ch              ѡ�� ADC ͨ�� (��� zf_driver_adc.h ��ö�� adc_channel_enum ����)
// ����˵��     resolution      ѡ��ѡ��ͨ���ֱ���(���ͬһ�� ADC ģ���ʼ��ʱ�����˲�ͬ�ķֱ��� �����һ����ʼ���ķֱ�����Ч)
// ���ز���     void
// ʹ��ʾ��     adc_init(ADC0_CH00_P06_0, ADC_12BIT);                                // ��ʼ�� A0 Ϊ ADC1 �� channel0 ���빦�� �ֱ���Ϊ8λ
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
void adc_init (adc_channel_enum adc_chn, adc_resolution_enum resolution)
{	
    uint32_t actual_adc_operation_freq          = 0;
    uint32_t div_num                            = 0;
    uint32_t sampling_cycle                     = 0;
    cy_stc_gpio_pin_config_t adc_pin_config     = {0};
    cy_stc_adc_config_t adc_config              = {0};
    adc_resolution[adc_chn]                     = resolution;
    cy_stc_adc_channel_config_t adc_channel_config;
    
    Cy_Adc_Channel_DeInit(&sar_adc[adc_chn / 32]->CH[adc_chn % 32]);
    
    adc_pin_config.driveMode    = CY_GPIO_DM_ANALOG;
    Cy_GPIO_Pin_Init(get_port(get_adc_pin(adc_chn)), (get_adc_pin(adc_chn) % 8), &adc_pin_config);
    
    div_num = DIV_ROUND_UP(ADC_FREQ, ADC_OPERATION_FREQUENCY_MAX_IN_HZ);
    actual_adc_operation_freq = ADC_FREQ / div_num;
    
    Cy_SysClk_PeriphAssignDivider((en_clk_dst_t)((adc_chn / 32) + PCLK_PASS0_CLOCK_SAR0), CY_SYSCLK_DIV_16_BIT, 0u);
    Cy_SysClk_PeriphSetDivider(Cy_SysClk_GetClockGroup((en_clk_dst_t)((adc_chn / 32) + PCLK_PASS0_CLOCK_SAR0)), CY_SYSCLK_DIV_16_BIT, 0u, (div_num - 1));
    Cy_SysClk_PeriphEnableDivider(Cy_SysClk_GetClockGroup((en_clk_dst_t)((adc_chn / 32) + PCLK_PASS0_CLOCK_SAR0)), CY_SYSCLK_DIV_16_BIT, 0u);

    adc_config.msbStretchMode           = CY_ADC_MSB_STRETCH_MODE_1CYCLE;
    adc_config.sarMuxEnable             = true;
    adc_config.adcEnable                = true;
    adc_config.sarIpEnable              = true;
    Cy_Adc_Init(sar_adc[adc_chn / 32], &adc_config);
    
    sampling_cycle = (uint32_t)DIV_ROUND_UP((ANALOG_IN_SAMPLING_TIME_MIN_IN_NS * (uint64_t)actual_adc_operation_freq), 1000000000ul);
    
    memset(&adc_channel_config, 0, sizeof(adc_channel_config));
    adc_channel_config.preenptionType   = CY_ADC_PREEMPTION_FINISH_RESUME;
    adc_channel_config.pinAddress       = (cy_en_adc_pin_address_t)(adc_chn % 32);
    adc_channel_config.extMuxEnable     = true;
    adc_channel_config.sampleTime       = sampling_cycle;
    adc_channel_config.isGroupEnd       = true;
    adc_channel_config.mask.grpDone     = true;
    Cy_Adc_Channel_Init(&sar_adc[adc_chn / 32]->CH[adc_chn % 32], &adc_channel_config);
    Cy_Adc_Channel_Enable(&sar_adc[adc_chn / 32]->CH[adc_chn % 32]);
    Cy_Adc_Channel_SoftwareTrigger(&sar_adc[adc_chn / 32]->CH[adc_chn % 32]);
}

