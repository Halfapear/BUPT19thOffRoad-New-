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
* 2024-3-2       pudding            �޸�PITʹ�ܺ���ʧЧ������
* 2024-5-14      pudding            ����12��PIT�����ж�
********************************************************************************************************************/

#include "sysclk/cy_sysclk.h"
#include "tcpwm/cy_tcpwm_pwm.h"
#include "zf_common_interrupt.h"
#include "zf_driver_pit.h"

#define PIT_USE_ISR     CPUIntIdx2_IRQn

void pit0_ch0_isr();
void pit0_ch1_isr();
void pit0_ch2_isr();
void pit0_ch10_isr();
void pit0_ch11_isr();
void pit0_ch12_isr();
void pit0_ch13_isr();
void pit0_ch14_isr();
void pit0_ch15_isr();
void pit0_ch16_isr();
void pit0_ch17_isr();
void pit0_ch18_isr();
void pit0_ch19_isr();
void pit0_ch20_isr();
void pit0_ch21_isr();

void (*pit_isr_func[15])() = {  pit0_ch0_isr,  pit0_ch1_isr,  pit0_ch2_isr,
                                pit0_ch10_isr, pit0_ch11_isr, pit0_ch12_isr, 
                                pit0_ch13_isr, pit0_ch14_isr, pit0_ch15_isr,
                                pit0_ch16_isr, pit0_ch17_isr, pit0_ch18_isr, 
                                pit0_ch19_isr, pit0_ch20_isr, pit0_ch21_isr};


//-------------------------------------------------------------------------------------------------------------------
//  �������      pit�ر�
//  ����˵��      pit_index        	ѡ��PITģ��
//  ���ز���      void
//  ʹ��ʾ��      pit_isr_flag_clear(PIT_CH0); // �ر� TCPWM2 ͨ��0�ļ�ʱ��
//  ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
void pit_isr_flag_clear (pit_index_enum pit_index)
{
    if(pit_index < PIT_CH10)
    {
        Cy_Tcpwm_Counter_ClearTC_Intr((volatile stc_TCPWM_GRP_CNT_t*) &TCPWM0->GRP[2].CNT[pit_index]);
    }
    else
    {
        Cy_Tcpwm_Counter_ClearTC_Intr((volatile stc_TCPWM_GRP_CNT_t*) &TCPWM0->GRP[1].CNT[(pit_index - 3)]);
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  �������      ʧ��pit�ж�
//  ����˵��      pit_index      	ѡ��PITģ��
//  ���ز���      void
//  ʹ��ʾ��      pit_disable(PIT_CH0); // ��ֹ TCPWM2 ͨ��0���ж�
//  ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
void pit_disable (pit_index_enum pit_index)
{
    if(pit_index < PIT_CH10)
    {
        Cy_Tcpwm_Counter_Disable((volatile stc_TCPWM_GRP_CNT_t*) &TCPWM0->GRP[2].CNT[pit_index]);
    }
    else
    {
        Cy_Tcpwm_Counter_Disable((volatile stc_TCPWM_GRP_CNT_t*) &TCPWM0->GRP[1].CNT[(pit_index - 3)]);
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  �������      ʹ��pit�ж�
//  ����˵��      pit_index       	ѡ��PITģ��
//  ���ز���      void
//  ʹ��ʾ��      pit_enable(PIT_CH0);  // ���� TCPWM2 ͨ��0���ж�
//  ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
void pit_enable (pit_index_enum pit_index)
{
    if(pit_index < PIT_CH10)
    {
        Cy_Tcpwm_Counter_SetCounter((volatile stc_TCPWM_GRP_CNT_t*) &TCPWM0->GRP[2].CNT[pit_index], 0);
        Cy_Tcpwm_Counter_Enable((volatile stc_TCPWM_GRP_CNT_t*) &TCPWM0->GRP[2].CNT[pit_index]);
        Cy_Tcpwm_TriggerStart((volatile stc_TCPWM_GRP_CNT_t*) &TCPWM0->GRP[2].CNT[pit_index]);  
    }
    else
    {
        Cy_Tcpwm_Counter_SetCounter((volatile stc_TCPWM_GRP_CNT_t*) &TCPWM0->GRP[1].CNT[(pit_index - 3)], 0);
        Cy_Tcpwm_Counter_Enable((volatile stc_TCPWM_GRP_CNT_t*) &TCPWM0->GRP[1].CNT[(pit_index - 3)]);
        Cy_Tcpwm_TriggerStart((volatile stc_TCPWM_GRP_CNT_t*) &TCPWM0->GRP[1].CNT[(pit_index - 3)]);  
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  �������      ��ֹ����pit�ж�
//  ���ز���      void
//  ʹ��ʾ��      pit_all_close();
//  ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
void pit_all_close (void)
{
    pit_disable(PIT_CH0);
    pit_disable(PIT_CH1);
    pit_disable(PIT_CH2);
    
    pit_disable(PIT_CH10);
    pit_disable(PIT_CH11);
    pit_disable(PIT_CH12);
    pit_disable(PIT_CH13);
    pit_disable(PIT_CH14);
    pit_disable(PIT_CH15);
    pit_disable(PIT_CH16);
    pit_disable(PIT_CH17);
    pit_disable(PIT_CH18);
    pit_disable(PIT_CH19);
    pit_disable(PIT_CH20);
    pit_disable(PIT_CH21);
}

//-------------------------------------------------------------------------------------------------------------------
//  �������      pit��ʼ��
//  ����˵��      pit_index       	ѡ��PITģ��
//  ����˵��      time            	����ʱ��
//  ���ز���      void
//  ʹ��ʾ��      pit_init(PIT_CH0, 5000);      // ���������ж�5000us
//  ��ע��Ϣ      ��ʹ��.h�ļ��� ��ʱ�䵥λ�ĺ궨�庯�� 
//-------------------------------------------------------------------------------------------------------------------
void pit_init (pit_index_enum pit_index, uint32 time)
{
    cy_stc_tcpwm_counter_config_t pit_config    = {0};
    cy_stc_sysint_irq_t           irq_cfg;
    uint8                         div_num       = 0;
    
    if(pit_index < PIT_CH10)
    {
        Cy_SysClk_PeriphAssignDivider((en_clk_dst_t)((uint32)pit_index + (uint32)PCLK_TCPWM0_CLOCKS512), (cy_en_divider_types_t)CY_SYSCLK_DIV_16_BIT, 0ul);
        Cy_SysClk_PeriphSetDivider(Cy_SysClk_GetClockGroup((en_clk_dst_t)((uint32)pit_index + (uint32)PCLK_TCPWM0_CLOCKS512)), (cy_en_divider_types_t)CY_SYSCLK_DIV_16_BIT, 0ul, 9u); // 80Mhzʱ�ӱ�10��ƵΪ8Mhz
        Cy_SysClk_PeriphEnableDivider(Cy_SysClk_GetClockGroup((en_clk_dst_t)((uint32)pit_index + (uint32)PCLK_TCPWM0_CLOCKS512)), (cy_en_divider_types_t)CY_SYSCLK_DIV_16_BIT, 0ul);
        
        irq_cfg.sysIntSrc  = (cy_en_intr_t)((uint32)tcpwm_0_interrupts_512_IRQn + (uint32)pit_index); 
        irq_cfg.intIdx     = PIT_USE_ISR                                    ;
        irq_cfg.isEnabled  = true                                          ;
        interrupt_init(&irq_cfg, pit_isr_func[pit_index], 3)                ;
        
        pit_config.period             = time * 8                            ;        // pit���ڼ���
        pit_config.clockPrescaler     = CY_TCPWM_PRESCALER_DIVBY_1         ;
        pit_config.runMode            = CY_TCPWM_COUNTER_CONTINUOUS        ; 
        pit_config.countDirection     = CY_TCPWM_COUNTER_COUNT_UP          ;
        pit_config.compareOrCapture   = CY_TCPWM_COUNTER_MODE_COMPARE      ;
        pit_config.countInputMode     = CY_TCPWM_INPUT_LEVEL               ;
        pit_config.countInput         = 1uL                                ;
        pit_config.trigger0EventCfg   = CY_TCPWM_COUNTER_OVERFLOW          ;
        pit_config.trigger1EventCfg   = CY_TCPWM_COUNTER_OVERFLOW          ;
        
        Cy_Tcpwm_Counter_Init((volatile stc_TCPWM_GRP_CNT_t*) &TCPWM0->GRP[2].CNT[pit_index], &pit_config);
        Cy_Tcpwm_Counter_Enable((volatile stc_TCPWM_GRP_CNT_t*) &TCPWM0->GRP[2].CNT[pit_index]);
        Cy_Tcpwm_TriggerStart((volatile stc_TCPWM_GRP_CNT_t*) &TCPWM0->GRP[2].CNT[pit_index]);  
        Cy_Tcpwm_Counter_SetTC_IntrMask((volatile stc_TCPWM_GRP_CNT_t*) &TCPWM0->GRP[2].CNT[pit_index]);
    }
    else
    {
        Cy_SysClk_PeriphAssignDivider((en_clk_dst_t)((uint32)(pit_index - 3) + (uint32)PCLK_TCPWM0_CLOCKS256), (cy_en_divider_types_t)CY_SYSCLK_DIV_16_BIT, 0ul);
        Cy_SysClk_PeriphSetDivider(Cy_SysClk_GetClockGroup((en_clk_dst_t)((uint32)(pit_index - 3) + (uint32)PCLK_TCPWM0_CLOCKS256)), (cy_en_divider_types_t)CY_SYSCLK_DIV_16_BIT, 0ul, 9u); // 80Mhzʱ�ӱ�10��ƵΪ8Mhz
        Cy_SysClk_PeriphEnableDivider(Cy_SysClk_GetClockGroup((en_clk_dst_t)((uint32)(pit_index - 3) + (uint32)PCLK_TCPWM0_CLOCKS256)), (cy_en_divider_types_t)CY_SYSCLK_DIV_16_BIT, 0ul);
        
        irq_cfg.sysIntSrc  = (cy_en_intr_t)((uint32)tcpwm_0_interrupts_256_IRQn + (uint32)(pit_index - 3)); 
        irq_cfg.intIdx     = PIT_USE_ISR                                    ;
        irq_cfg.isEnabled  = true                                          ;
        interrupt_init(&irq_cfg, pit_isr_func[pit_index], 3)                ;
        
        uint32 period_count = time * 8;
        for(int i = 0; i < 8; i ++)
        {
            if(period_count > 0xffff)
            {
                div_num ++;
                period_count /= 2;
            }
        }
        pit_config.period             = period_count                       ;        // pit���ڼ���
        pit_config.clockPrescaler     = (cy_en_tcpwm_clk_prescalers_t)div_num ;
        pit_config.runMode            = CY_TCPWM_COUNTER_CONTINUOUS        ; 
        pit_config.countDirection     = CY_TCPWM_COUNTER_COUNT_UP          ;
        pit_config.compareOrCapture   = CY_TCPWM_COUNTER_MODE_COMPARE      ;
        pit_config.countInputMode     = CY_TCPWM_INPUT_LEVEL               ;
        pit_config.countInput         = 1uL                                ;
        pit_config.trigger0EventCfg   = CY_TCPWM_COUNTER_OVERFLOW          ;
        pit_config.trigger1EventCfg   = CY_TCPWM_COUNTER_OVERFLOW          ;
        
        Cy_Tcpwm_Counter_Init((volatile stc_TCPWM_GRP_CNT_t*) &TCPWM0->GRP[1].CNT[(pit_index - 3)], &pit_config);
        Cy_Tcpwm_Counter_Enable((volatile stc_TCPWM_GRP_CNT_t*) &TCPWM0->GRP[1].CNT[(pit_index - 3)]);
        Cy_Tcpwm_TriggerStart((volatile stc_TCPWM_GRP_CNT_t*) &TCPWM0->GRP[1].CNT[(pit_index - 3)]);  
        Cy_Tcpwm_Counter_SetTC_IntrMask((volatile stc_TCPWM_GRP_CNT_t*) &TCPWM0->GRP[1].CNT[(pit_index - 3)]);
    }
    

}
