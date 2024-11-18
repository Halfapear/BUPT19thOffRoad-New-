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
* �ļ�����          zf_driver_delay
* ��˾����          �ɶ���ɿƼ����޹�˾
* �汾��Ϣ          �鿴 libraries/doc �ļ����� version �ļ� �汾˵��
* ��������          IAR 9.40.1
* ����ƽ̨          CYT4BB
* ��������          https://seekfree.taobao.com/
*
* �޸ļ�¼
* ����              ����                ��ע
* 2024-1-8       pudding            first version
********************************************************************************************************************/

#include "systick/cy_systick.h"
#include "sysclk/cy_sysclk.h"
#include "tcpwm/cy_tcpwm_pwm.h"
#include "zf_driver_delay.h"


#if CY_CORE_CM7_0
static en_clk_dst_t delay_clk = PCLK_TCPWM0_CLOCKS518;
static uint8 timer_ch = 6;
#elif CY_CORE_CM7_1
static en_clk_dst_t delay_clk = PCLK_TCPWM0_CLOCKS519;
static uint8 timer_ch = 7;
#endif

//-------------------------------------------------------------------------------------------------------------------
//  �������      system��ʱ����
//  ����˵��      time            ��ʱһ�ֵ�ʱ�䣨��λΪ���룬�����÷�Χ0-20000000��
//  ���ز���      void
//  ʹ��ʾ��      system_delay(cy_delayFreqMhz * 2); // ����ʱ1us  250Mʱ�� ��С��λʱ��Ϊ 4ns
//-------------------------------------------------------------------------------------------------------------------
void system_delay (uint32 time)
{
    Cy_SysLib_DelayCycles(time);
}


//-------------------------------------------------------------------------------------------------------------------
//  �������      system ΢�� ��ʱ����
//  ����˵��      time            ��ʱʱ��  ��λ��us
//  ���ز���      void
//  ʹ��ʾ��      system_delay_us(100);  // ��ʱ100΢��         
//-------------------------------------------------------------------------------------------------------------------
void system_delay_us (uint32 time)
{
#if CY_CORE_CM7_0 || CY_CORE_CM7_1
    time = time * 8;
   
    Cy_Tcpwm_Counter_SetCounter((volatile stc_TCPWM_GRP_CNT_t*) &TCPWM0->GRP[2].CNT[timer_ch], 0);

    while(time > 50000)
    {
        while(Cy_Tcpwm_Counter_GetCounter((volatile stc_TCPWM_GRP_CNT_t*) &TCPWM0->GRP[2].CNT[timer_ch]) <= 50000);
        time -= Cy_Tcpwm_Counter_GetCounter((volatile stc_TCPWM_GRP_CNT_t*) &TCPWM0->GRP[2].CNT[timer_ch]);
        Cy_Tcpwm_Counter_SetCounter((volatile stc_TCPWM_GRP_CNT_t*) &TCPWM0->GRP[2].CNT[timer_ch], 0);    
    }

    while(Cy_Tcpwm_Counter_GetCounter((volatile stc_TCPWM_GRP_CNT_t*) &TCPWM0->GRP[2].CNT[timer_ch]) <= time);
#else
    Cy_SysTick_DelayInUs(time);
#endif  
}

//-------------------------------------------------------------------------------------------------------------------
//  �������      system ���� ��ʱ����
//  ����˵��      time            ��ʱʱ��  ��λ��ms
//  ���ز���      void
//  ʹ��ʾ��      system_delay_ms(100);  // ��ʱ100����
//-------------------------------------------------------------------------------------------------------------------
void system_delay_ms (uint32 time)
{
#if CY_CORE_CM7_0 || CY_CORE_CM7_1
    time = time * 8000;
   
    Cy_Tcpwm_Counter_SetCounter((volatile stc_TCPWM_GRP_CNT_t*) &TCPWM0->GRP[2].CNT[timer_ch], 0);

    while(time > 50000)
    {
        while(Cy_Tcpwm_Counter_GetCounter((volatile stc_TCPWM_GRP_CNT_t*) &TCPWM0->GRP[2].CNT[timer_ch]) <= 50000);
        time -= 50000;
        
        Cy_Tcpwm_Counter_SetCounter((volatile stc_TCPWM_GRP_CNT_t*) &TCPWM0->GRP[2].CNT[timer_ch], 
                                    Cy_Tcpwm_Counter_GetCounter((volatile stc_TCPWM_GRP_CNT_t*) &TCPWM0->GRP[2].CNT[timer_ch]) - 50000);    
    }

    while(Cy_Tcpwm_Counter_GetCounter((volatile stc_TCPWM_GRP_CNT_t*) &TCPWM0->GRP[2].CNT[timer_ch]) <= time);
#else
    Cy_SysTick_DelayInUs(time * 1000);
#endif    
}

//-------------------------------------------------------------------------------------------------------------------
//  �������      ��ʱ��ʼ��
//  ����˵��      void            
//  ���ز���      void
//  ʹ��ʾ��:     �����û�����
//-------------------------------------------------------------------------------------------------------------------
void system_delay_init(void)
{
#if CY_CORE_CM7_0 || CY_CORE_CM7_1
    cy_stc_tcpwm_counter_config_t timer_config  = {0};
    
    Cy_SysClk_PeriphAssignDivider(delay_clk, (cy_en_divider_types_t)CY_SYSCLK_DIV_16_BIT, 0ul);
    Cy_SysClk_PeriphSetDivider(Cy_SysClk_GetClockGroup(delay_clk), (cy_en_divider_types_t)CY_SYSCLK_DIV_16_BIT, 0ul, 9u); // 80Mhzʱ�ӱ�10��ƵΪ8Mhz
    Cy_SysClk_PeriphEnableDivider(Cy_SysClk_GetClockGroup(delay_clk), (cy_en_divider_types_t)CY_SYSCLK_DIV_16_BIT, 0ul);
    
    timer_config.period             = 0xffffffff                         ;        
    timer_config.clockPrescaler     = CY_TCPWM_PRESCALER_DIVBY_1         ;
    timer_config.runMode            = CY_TCPWM_COUNTER_CONTINUOUS        ; 
    timer_config.countDirection     = CY_TCPWM_COUNTER_COUNT_UP          ;
    timer_config.compareOrCapture   = CY_TCPWM_COUNTER_MODE_COMPARE      ;
    timer_config.countInputMode     = CY_TCPWM_INPUT_LEVEL               ;
    timer_config.countInput         = 1uL                                ;
    timer_config.debug_pause        = 1uL                                ;
    
    Cy_Tcpwm_Counter_Init((volatile stc_TCPWM_GRP_CNT_t*) &TCPWM0->GRP[2].CNT[timer_ch], &timer_config);
    Cy_Tcpwm_Counter_Enable((volatile stc_TCPWM_GRP_CNT_t*) &TCPWM0->GRP[2].CNT[timer_ch]);
    Cy_Tcpwm_Counter_SetCounter((volatile stc_TCPWM_GRP_CNT_t*) &TCPWM0->GRP[2].CNT[timer_ch], 0);
    Cy_Tcpwm_TriggerStart((volatile stc_TCPWM_GRP_CNT_t*) &TCPWM0->GRP[2].CNT[timer_ch]);
#endif
}

