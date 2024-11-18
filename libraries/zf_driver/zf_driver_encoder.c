/*********************************************************************************************************************
* CYT4BB7CEE Opensourec Library ����CYT4BB7CEE ��Դ�⣩��һ�����ڹٷ� SDK �ӿڵĵ�������Դ��
* Copyright (c) 2023 SEEKFREE ��ɿƼ�
*
* ���ļ��� CYT4BB7CEE ��Դ���һ����
*
* CYT4BB7CEE ��Դ�� ��������
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
* �ļ�����          zf_driver_encoder
* ��˾����          �ɶ���ɿƼ����޹�˾
* �汾��Ϣ          �鿴 libraries/doc �ļ����� version �ļ� �汾˵��
* ��������          MDK 5.38a
* ����ƽ̨          CYT4BB7CEE
* ��������          https://seekfree.taobao.com/
*
* �޸ļ�¼
* ����              ����                ��ע
* 2023-11-30       pudding            first version
* 2023-05-21       pudding            ����֧������������
********************************************************************************************************************/

#include "sysclk/cy_sysclk.h"
#include "tcpwm/cy_tcpwm_quaddec.h"
#include "tcpwm/cy_tcpwm_pwm.h"
#include "gpio/cy_gpio.h"
#include "trigmux/cy_trigmux.h"
#include "zf_driver_gpio.h"
#include "zf_common_debug.h"
#include "zf_driver_encoder.h"


static uint8 			encoder_mode[4]; 
en_clk_dst_t   encoder_clk[4] = {PCLK_TCPWM0_CLOCKS263, PCLK_TCPWM0_CLOCKS20, PCLK_TCPWM0_CLOCKS27, PCLK_TCPWM0_CLOCKS58};
volatile stc_TCPWM_GRP_CNT_t *encoder_module[4] = {TCPWM0_GRP1_CNT7, TCPWM0_GRP0_CNT20, TCPWM0_GRP0_CNT27, TCPWM0_GRP0_CNT58};
//-------------------------------------------------------------------------------------------------------------------
// �������     ��ȡͨ��1����ֵ
// ����˵��     ch1_pin       
// ���ز���     gpio_pin_enum
// ʹ��ʾ��     get_ch1_pin(TIM_CH7_ENCODER_CH1_P7_6);
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
static gpio_pin_enum get_ch1_pin (encoder_channel1_enum ch1_pin)
{
    gpio_pin_enum temp_pin;
    switch(ch1_pin)
    {
        case TC_CH07_ENCODER_CH1_P07_6: 	temp_pin = P07_6; 	break;
        case TC_CH20_ENCODER_CH1_P08_1: 	temp_pin = P08_1; 	break;
        case TC_CH27_ENCODER_CH1_P19_2: 	temp_pin = P19_2; 	break;
        case TC_CH58_ENCODER_CH1_P17_3: 	temp_pin = P17_3; 	break;
    }
    return temp_pin;
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ��ȡͨ��1���Ÿ��ù�ϵ
// ����˵��     ch1_pin       
// ���ز���     en_hsiom_sel_t
// ʹ��ʾ��     get_ch1_pin(TIM_CH7_ENCODER_CH1_P7_6);
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
static en_hsiom_sel_t get_ch1_hsiom (encoder_channel1_enum ch1_pin)
{
    en_hsiom_sel_t temp_pin;
    switch(ch1_pin)
    {
        case TC_CH07_ENCODER_CH1_P07_6: 	temp_pin = P7_6_TCPWM0_TR_ONE_CNT_IN789; 	break;
        case TC_CH20_ENCODER_CH1_P08_1: 	temp_pin = P8_1_TCPWM0_TR_ONE_CNT_IN60; 	break;
        case TC_CH27_ENCODER_CH1_P19_2: 	temp_pin = P19_2_TCPWM0_TR_ONE_CNT_IN81; 	break;
        case TC_CH58_ENCODER_CH1_P17_3: 	temp_pin = P17_3_TCPWM0_TR_ONE_CNT_IN174; 	break;
    }
    return temp_pin;
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ��ȡͨ��2����ֵ
// ����˵��     ch2_pin       
// ���ز���     gpio_pin_enum
// ʹ��ʾ��     get_ch2_pin(TIM_CH7_ENCODER_CH1_P7_6);
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
static gpio_pin_enum get_ch2_pin (encoder_channel2_enum ch2_pin)
{
    gpio_pin_enum temp_pin;
    switch(ch2_pin)
    {
        case TC_CH07_ENCODER_CH2_P07_7: 	temp_pin = P07_7; 	break;
        case TC_CH20_ENCODER_CH2_P08_2: 	temp_pin = P08_2; 	break;
        case TC_CH27_ENCODER_CH2_P19_3: 	temp_pin = P19_3; 	break;
        case TC_CH58_ENCODER_CH2_P17_4: 	temp_pin = P17_4; 	break;
    }
    return temp_pin;
}
//-------------------------------------------------------------------------------------------------------------------
// �������     ��ȡͨ��2���Ÿ��ù�ϵ
// ����˵��     ch2_pin       
// ���ز���     en_hsiom_sel_t
// ʹ��ʾ��     get_ch2_pin(TIM_CH7_ENCODER_CH1_P7_6);
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
static en_hsiom_sel_t get_ch2_hsiom (encoder_channel2_enum ch2_pin)
{
    en_hsiom_sel_t temp_pin;
    switch(ch2_pin)
    {
        case TC_CH07_ENCODER_CH2_P07_7: 	temp_pin = P7_7_TCPWM0_TR_ONE_CNT_IN790; 	break;
        case TC_CH20_ENCODER_CH2_P08_2: 	temp_pin = P8_2_TCPWM0_TR_ONE_CNT_IN61; 	break;
        case TC_CH27_ENCODER_CH2_P19_3: 	temp_pin = P19_3_TCPWM0_TR_ONE_CNT_IN82; 	break;
        case TC_CH58_ENCODER_CH2_P17_4: 	temp_pin = P17_4_TCPWM0_TR_ONE_CNT_IN175; 	break;
    }
    return temp_pin;
}

//-------------------------------------------------------------------------------------------------------------------
//  �������      ���������ݲɼ�
//  ����˵��      gptn            ѡ����ʹ�õĶ�ʱ��
//  ���ز���      void
//  ʹ��ʾ��      int16 speed; speed = encoder_get_count(TIM2_ENCODER); // ʹ��T2��ʱ��
//  ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
int16 encoder_get_count (encoder_index_enum encoder_n)
{
    int16 encoder_data = 0;
    if(encoder_mode[encoder_n])
    {
        encoder_data = (int16)(Cy_Tcpwm_Counter_GetCounter(encoder_module[encoder_n]));
        encoder_data = gpio_get_level(get_ch2_pin((encoder_channel2_enum)encoder_n)) == 1? encoder_data : -encoder_data;
    }
    else
    {
        encoder_data = (int16)(0x00008000 - Cy_Tcpwm_QuadDec_GetCounter(encoder_module[encoder_n]));
        Cy_Tcpwm_QuadDec_SetCounter(encoder_module[encoder_n], 0x00008000);
        
    }
    return encoder_data;
}
//-------------------------------------------------------------------------------------------------------------------
//  �������      �������������
//  ����˵��      gptn            ѡ����ʹ�õĶ�ʱ��
//  ���ز���      void
//  ʹ��ʾ��      encoder_clear_count(TIM2_ENCODER);// ʹ��T2��ʱ��
//  ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
void encoder_clear_count (encoder_index_enum encoder_n)
{
    if(encoder_mode[encoder_n])
    {
        Cy_Tcpwm_Counter_SetCounter(encoder_module[encoder_n], 0);
    }
    else
    {
        Cy_Tcpwm_QuadDec_SetCounter(encoder_module[encoder_n], 0);
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  �������      �����������ɼ���ʼ��
//  ����˵��      encoder_n       ѡ����ʹ�õ�GPT12��ʱ��
//  ����˵��      ch1_pin         ENCODER ͨ�� 1
//  ����˵��      ch2_pin         ENCODER ͨ�� 2
//  ���ز���      void
//  ʹ��ʾ��      encoder_quad_init(TC_CH07_ENCODER, TC_CH07_ENCODER_CH1_P07_6, TC_CH07_ENCODER_CH2_P07_7);// ʹ��tcpwmģ���ͨ��7   P7_6 ����ΪAͨ��    P7_7����ΪBͨ��
//  ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
void encoder_quad_init (encoder_index_enum encoder_n, encoder_channel1_enum ch1_pin, encoder_channel2_enum ch2_pin)
{
    cy_stc_gpio_pin_config_t  pwm_pin_config    = {0};
    cy_stc_tcpwm_quaddec_config_t encoder_quad_config;
    
    pwm_pin_config.driveMode                    = CY_GPIO_DM_HIGHZ;
    pwm_pin_config.hsiom                        = get_ch1_hsiom(ch1_pin);
    Cy_GPIO_Pin_Init(get_port(get_ch1_pin(ch1_pin)), (get_ch1_pin(ch1_pin) % 8), &pwm_pin_config);
    
    pwm_pin_config.driveMode                    = CY_GPIO_DM_HIGHZ;
    pwm_pin_config.hsiom                        = get_ch2_hsiom(ch2_pin);
    Cy_GPIO_Pin_Init(get_port(get_ch2_pin(ch2_pin)), (get_ch2_pin(ch2_pin) % 8), &pwm_pin_config);
    
    Cy_SysClk_PeriphAssignDivider(encoder_clk[encoder_n], CY_SYSCLK_DIV_16_BIT, 0ul);
    Cy_SysClk_PeriphSetDivider(Cy_SysClk_GetClockGroup(encoder_clk[encoder_n]), CY_SYSCLK_DIV_16_BIT, 0ul, 9u); // 80Mhzʱ�ӱ�10��ƵΪ8Mhz
    Cy_SysClk_PeriphEnableDivider(Cy_SysClk_GetClockGroup(encoder_clk[encoder_n]), CY_SYSCLK_DIV_16_BIT, 0ul);
   
    
    Cy_Tcpwm_QuadDec_DeInit(encoder_module[encoder_n]);
    
    encoder_quad_config.encoding_mode           = CY_TCPWM_QUADDEC_X1           ;
    encoder_quad_config.range_mode              = CY_TCPWM_QUADDEC_RANGE_0      ;
    encoder_quad_config.interruptSources        = CY_TCPWM_INT_NONE             ;
    encoder_quad_config.indexInputMode          = CY_TCPWM_INPUT_RISING_EDGE    ;
    encoder_quad_config.indexInput              = CY_TCPWM_INPUT0               ;
    encoder_quad_config.stopInputMode           = CY_TCPWM_INPUT_LEVEL          ;
    encoder_quad_config.stopInput               = CY_TCPWM_INPUT0               ;
    encoder_quad_config.phiAInput               = CY_TCPWM_INPUT_TRIG0          ;
    encoder_quad_config.phiBInput               = CY_TCPWM_INPUT_TRIG1          ;  
    
    Cy_Tcpwm_QuadDec_Init(encoder_module[encoder_n], &encoder_quad_config);
    Cy_Tcpwm_QuadDec_Enable(encoder_module[encoder_n]);
    Cy_Tcpwm_TriggerStart(encoder_module[encoder_n]);
    Cy_Tcpwm_TriggerReloadOrIndex(encoder_module[encoder_n]);
    
    encoder_mode[encoder_n]  = 0;
}       

//-------------------------------------------------------------------------------------------------------------------
//  �������      ����������ɼ���ʼ��
//  ����˵��      encoder_n       ѡ����ʹ�õ�GPT12��ʱ��
//  ����˵��      ch1_pin         ���ü�������
//  ����˵��      ch2_pin         ���÷�������
//  ���ز���      void
//  ʹ��ʾ��      encoder_dir_init(TC_CH7_ENCODER, TC_CH7_ENCODER_CH1_P7_6, TC_CH7_ENCODER_CH2_P7_7);// ʹ��TCPWM��ʱ��   P7_6���Ž��м���    ��������ʹ��P7_7����
//  ��ע��Ϣ      
//-------------------------------------------------------------------------------------------------------------------
void encoder_dir_init (encoder_index_enum encoder_n, encoder_channel1_enum count_pin, encoder_channel2_enum dir_pin)
{
    cy_stc_gpio_pin_config_t  pwm_pin_config    = {0};
    cy_stc_tcpwm_counter_config_t encoder_dir_config;
    
    // ���ѣ�ģ��źͶ˿ڶ�����Ӧ��ô�ܳ�ʼ���أ�
    zf_assert((uint8)encoder_n == (uint8)count_pin ? 1 : 0);
    zf_assert((uint8)count_pin == (uint8)dir_pin ? 1 : 0);
    
    Cy_SysClk_PeriphAssignDivider(encoder_clk[encoder_n], CY_SYSCLK_DIV_16_BIT, 0ul);
    Cy_SysClk_PeriphSetDivider(Cy_SysClk_GetClockGroup(encoder_clk[encoder_n]), CY_SYSCLK_DIV_16_BIT, 0ul, 9u); // 80Mhzʱ�ӱ�10��ƵΪ8Mhz
    Cy_SysClk_PeriphEnableDivider(Cy_SysClk_GetClockGroup(encoder_clk[encoder_n]), CY_SYSCLK_DIV_16_BIT, 0ul);
  
    pwm_pin_config.driveMode                    = CY_GPIO_DM_STRONG;
    pwm_pin_config.hsiom                        = get_ch1_hsiom(count_pin);
    Cy_GPIO_Pin_Init(get_port(get_ch1_pin(count_pin)), (get_ch1_pin(count_pin) % 8), &pwm_pin_config);
  
    pwm_pin_config.driveMode                    = CY_GPIO_DM_PULLDOWN;
    pwm_pin_config.hsiom                        = HSIOM_SEL_GPIO;
    Cy_GPIO_Pin_Init(get_port(get_ch2_pin(dir_pin)), (get_ch2_pin(dir_pin) % 8), &pwm_pin_config);
  
    memset(&encoder_dir_config, 0, sizeof(encoder_dir_config));
    
    encoder_dir_config.period             = 0xffff                             ;  
    encoder_dir_config.compare0           = 0xffff                             ;
    encoder_dir_config.clockPrescaler     = CY_TCPWM_PRESCALER_DIVBY_1         ;
    encoder_dir_config.runMode            = CY_TCPWM_COUNTER_CONTINUOUS        ; 
    encoder_dir_config.countDirection     = CY_TCPWM_COUNTER_COUNT_UP          ;
    encoder_dir_config.compareOrCapture   = CY_TCPWM_COUNTER_MODE_COMPARE      ;
    encoder_dir_config.countInputMode     = CY_TCPWM_INPUT_RISING_EDGE         ;
    encoder_dir_config.countInput         = 2uL                                ;
  
    Cy_Tcpwm_Counter_Init(encoder_module[encoder_n], &encoder_dir_config);
    Cy_Tcpwm_Counter_Enable(encoder_module[encoder_n]);
    Cy_Tcpwm_TriggerStart(encoder_module[encoder_n]);
    Cy_Tcpwm_Counter_SetCounter(encoder_module[encoder_n], 0);
    
    encoder_mode[encoder_n]  = 1;
}

