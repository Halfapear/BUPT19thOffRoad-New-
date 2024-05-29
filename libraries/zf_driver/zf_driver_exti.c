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
* �ļ�����          zf_driver_exti
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

#include "gpio/cy_gpio.h"
#include "zf_common_interrupt.h"
#include "zf_common_debug.h"
#include "zf_driver_exti.h"

#define EXTI_USE_ISR    CPUIntIdx1_IRQn


void gpio_0_exti_isr();
void gpio_1_exti_isr();
void gpio_2_exti_isr();
void gpio_3_exti_isr();
void gpio_4_exti_isr();
void gpio_5_exti_isr();
void gpio_6_exti_isr();
void gpio_7_exti_isr();
void gpio_8_exti_isr();
void gpio_9_exti_isr();
void gpio_10_exti_isr();
void gpio_11_exti_isr();
void gpio_12_exti_isr();
void gpio_13_exti_isr();
void gpio_14_exti_isr();
void gpio_15_exti_isr();
void gpio_16_exti_isr();
void gpio_17_exti_isr();
void gpio_18_exti_isr();
void gpio_19_exti_isr();
void gpio_20_exti_isr();
void gpio_21_exti_isr();
void gpio_22_exti_isr();
void gpio_23_exti_isr();

void (*exti_isr_func[24])() = {gpio_0_exti_isr, gpio_1_exti_isr, gpio_2_exti_isr, gpio_3_exti_isr, gpio_4_exti_isr, gpio_5_exti_isr, gpio_6_exti_isr, gpio_7_exti_isr, 
                                gpio_8_exti_isr, gpio_9_exti_isr, gpio_10_exti_isr, gpio_11_exti_isr, gpio_12_exti_isr, gpio_13_exti_isr, gpio_14_exti_isr, gpio_15_exti_isr, 
                                gpio_16_exti_isr, gpio_17_exti_isr, gpio_18_exti_isr, gpio_19_exti_isr, gpio_20_exti_isr, gpio_21_exti_isr, gpio_22_exti_isr, gpio_23_exti_isr};

//-------------------------------------------------------------------------------------------------------------------
// �������     ��ȡ�����Ƿ��ж�
// ����˵��     exti_pin             ѡ�� EXTI ���� (��ѡ��Χ�� zf_driver_exti.h �� exti_pin_enum ö��ֵȷ��)
// ���ز���     0���ö˿�δ�ж�    �������ö˿ڴ����ж�
// ʹ��ʾ��     exti_flag_get(P10_7);
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
uint8 exti_flag_get(gpio_pin_enum exti_pin)
{
    uint8 return_data = 0;
    
    if(Cy_GPIO_GetInterruptStatusMasked(get_port(exti_pin), (exti_pin % 8)))
    {
        Cy_GPIO_ClearInterrupt(get_port(exti_pin), (exti_pin % 8));
        return_data =1;
    }
  
    return return_data;
}

//-------------------------------------------------------------------------------------------------------------------
// �������     EXTI �ж�ʹ��
// ����˵��     pin             ѡ�� EXTI ���� (��ѡ��Χ�� zf_driver_exti.h �� exti_pin_enum ö��ֵȷ��)
// ���ز���     void
// ʹ��ʾ��     exti_enable(ERU_CH0_REQ4_P10_7);
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
void exti_enable (gpio_pin_enum exti_pin)
{
    Cy_GPIO_SetInterruptMask(get_port(exti_pin), (exti_pin % 8), 1);
}

//-------------------------------------------------------------------------------------------------------------------
// �������     EXTI �ж�ʧ��
// ����˵��     pin             ѡ�� EXTI ���� (��ѡ��Χ�� zf_driver_exti.h �� exti_pin_enum ö��ֵȷ��)
// ���ز���     void
// ʹ��ʾ��     exti_disable(A0);
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
void exti_disable (gpio_pin_enum exti_pin)
{
    Cy_GPIO_SetInterruptMask(get_port(exti_pin), (exti_pin % 8), 0);
}

//-------------------------------------------------------------------------------------------------------------------
//  �������      EXTI ʧ��
//  ���ز���      void
//  ʹ��ʾ��      exti_disable(ERU_CH0_REQ0_P15_4);
//-------------------------------------------------------------------------------------------------------------------
void exti_all_close (void)
{
    for(int i = 0; i < 24; i ++)
    {
        for(int j = 0; j < 8; j ++)
        {
            Cy_GPIO_SetInterruptMask(get_port(i * 8), j, 0);
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  �������      EXTI �жϳ�ʼ��
//  ����˵��      eru_pin         ����eruͨ��������
//  ����˵��      trigger         ���ô�����ʽ
//  ���ز���      void
//  ʹ��ʾ��      exti_init(P15_4, EXTI_TRIGGER_RISING);   // ʹ��P15_4���ţ������ش����ж�
//  ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
void exti_init (gpio_pin_enum exti_pin, exti_trigger_enum trigger)
{
    cy_stc_gpio_pin_config_t    exti_pin_cfg    = {0};
    cy_stc_sysint_irq_t         exti_irq_cfg;
    
    exti_pin_cfg.driveMode = CY_GPIO_DM_HIGHZ;
    exti_pin_cfg.intEdge   = trigger;
    exti_pin_cfg.intMask   = 1ul;
    Cy_GPIO_Pin_Init(get_port(exti_pin), (exti_pin % 8), &exti_pin_cfg);
    
    exti_irq_cfg.sysIntSrc  = (cy_en_intr_t)(ioss_interrupts_gpio_dpslp_0_IRQn + exti_pin / 8);
    exti_irq_cfg.intIdx     = EXTI_USE_ISR;
    exti_irq_cfg.isEnabled  = true;
    interrupt_init(&exti_irq_cfg, exti_isr_func[exti_pin / 8], 3)                ;
}
