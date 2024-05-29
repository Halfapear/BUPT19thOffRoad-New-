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
* �ļ�����          zf_driver_pit
* ��˾����          �ɶ���ɿƼ����޹�˾
* �汾��Ϣ          �鿴 libraries/doc �ļ����� version �ļ� �汾˵��
* ��������          IAR 9.40.1
* ����ƽ̨          CYT4BB
* ��������          https://seekfree.taobao.com/
*
* �޸ļ�¼
* ����              ����                ��ע
* 2024-1-9       pudding            first version
********************************************************************************************************************/

#include "sysclk/cy_sysclk.h"
#include "tcpwm/cy_tcpwm_pwm.h"
#include "zf_driver_timer.h"


static uint8 driver_timer_mode[3];

//-------------------------------------------------------------------------------------------------------------------
// �������     TIMER ��ʼ��ʱ
// ����˵��     index           TIMER ����ģ���
// ���ز���     void
// ʹ��ʾ��     timer_start(TC_TIME2_CH0);
// ��ע��Ϣ     
//-------------------------------------------------------------------------------------------------------------------
void timer_start (timer_index_enum index)
{
    Cy_Tcpwm_Counter_Enable((volatile stc_TCPWM_GRP_CNT_t*) &TCPWM0->GRP[2].CNT[index + 3]);
    Cy_Tcpwm_TriggerStart((volatile stc_TCPWM_GRP_CNT_t*) &TCPWM0->GRP[2].CNT[index + 3]);
}

//-------------------------------------------------------------------------------------------------------------------
// �������     TIMER ֹͣ��ʱ
// ����˵��     index           TIMER ����ģ���
// ���ز���     void
// ʹ��ʾ��     timer_stop(TC_TIME2_CH0);
// ��ע��Ϣ     
//-------------------------------------------------------------------------------------------------------------------
void timer_stop (timer_index_enum index)
{
    Cy_Tcpwm_Counter_Disable((volatile stc_TCPWM_GRP_CNT_t*) &TCPWM0->GRP[2].CNT[index + 3]);
}

//-------------------------------------------------------------------------------------------------------------------
// �������     TIMER ��ȡ��ʱ������
// ����˵��     index           TIMER ����ģ���
// ���ز���     uint16          ����ֵ
// ʹ��ʾ��     timer_get(TC_TIME2_CH0);
// ��ע��Ϣ     
//-------------------------------------------------------------------------------------------------------------------
uint32 timer_get (timer_index_enum index)
{
    return ((Cy_Tcpwm_Counter_GetCounter((volatile stc_TCPWM_GRP_CNT_t*) &TCPWM0->GRP[2].CNT[index + 3])) / (driver_timer_mode[index] == TIMER_MS ? 8000 : 8));
}

//-------------------------------------------------------------------------------------------------------------------
// �������     TIMER �����ʱ������
// ����˵��     index           TIMER ����ģ���
// ���ز���     void
// ʹ��ʾ��     timer_clear(TC_TIME2_CH0);
// ��ע��Ϣ     
//-------------------------------------------------------------------------------------------------------------------
void timer_clear (timer_index_enum index)
{
    Cy_Tcpwm_Counter_SetCounter((volatile stc_TCPWM_GRP_CNT_t*) &TCPWM0->GRP[2].CNT[index + 3], 0);
}

//-------------------------------------------------------------------------------------------------------------------
// �������     TIMER ��ʱ����ʼ��
// ����˵��     index           TIMER ����ģ���
// ����˵��     mode            ��ʱ��ʽ
// ���ز���     void
// ʹ��ʾ��     timer_init(TC_TIME2_CH0, TIMER_MS);
// ��ע��Ϣ     
//-------------------------------------------------------------------------------------------------------------------
void timer_init (timer_index_enum index, timer_mode_enum mode)
{
    cy_stc_tcpwm_counter_config_t timer_config  = {0};
    driver_timer_mode[index]                    = mode;
    
    Cy_SysClk_PeriphAssignDivider((en_clk_dst_t)((uint32)index + (uint32)PCLK_TCPWM0_CLOCKS515), (cy_en_divider_types_t)CY_SYSCLK_DIV_16_BIT, 0ul);
    Cy_SysClk_PeriphSetDivider(Cy_SysClk_GetClockGroup((en_clk_dst_t)((uint32)index + (uint32)PCLK_TCPWM0_CLOCKS515)), (cy_en_divider_types_t)CY_SYSCLK_DIV_16_BIT, 0ul, 9u); // 80Mhzʱ�ӱ�10��ƵΪ8Mhz
    Cy_SysClk_PeriphEnableDivider(Cy_SysClk_GetClockGroup((en_clk_dst_t)((uint32)index + (uint32)PCLK_TCPWM0_CLOCKS515)), (cy_en_divider_types_t)CY_SYSCLK_DIV_16_BIT, 0ul);
    
    timer_config.period             = 0xffffffff                         ;        
    timer_config.clockPrescaler     = CY_TCPWM_PRESCALER_DIVBY_1         ;
    timer_config.runMode            = CY_TCPWM_COUNTER_CONTINUOUS        ; 
    timer_config.countDirection     = CY_TCPWM_COUNTER_COUNT_UP          ;
    timer_config.compareOrCapture   = CY_TCPWM_COUNTER_MODE_COMPARE      ;
    timer_config.countInputMode     = CY_TCPWM_INPUT_LEVEL               ;
    timer_config.countInput         = 1uL                                ;
    timer_config.debug_pause        = 1uL                                ;
    
    Cy_Tcpwm_Counter_Init((volatile stc_TCPWM_GRP_CNT_t*) &TCPWM0->GRP[2].CNT[index + 3], &timer_config);
    Cy_Tcpwm_Counter_Disable((volatile stc_TCPWM_GRP_CNT_t*) &TCPWM0->GRP[2].CNT[index + 3]);
    Cy_Tcpwm_Counter_SetCounter((volatile stc_TCPWM_GRP_CNT_t*) &TCPWM0->GRP[2].CNT[index + 3], 0);
}




