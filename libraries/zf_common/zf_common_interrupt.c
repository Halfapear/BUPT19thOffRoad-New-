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
* �ļ�����          zf_common_interrupt
* ��˾����          �ɶ���ɿƼ����޹�˾
* �汾��Ϣ          �鿴 libraries/doc �ļ����� version �ļ� �汾˵��
* ��������          IAR 9.40.1
* ����ƽ̨          CYT4BB
* ��������          https://seekfree.taobao.com/
*
* �޸ļ�¼
* ����              ����                ��ע
* 2024-1-4       pudding            first version
* 2024-1-29      pudding            �����ж����ȼ����ú��� ��װ�жϳ�ʼ������
********************************************************************************************************************/

#include "cmsis_compiler.h"
#include "zf_common_interrupt.h"
#include "zf_driver_pwm.h"

static vuint32 interrupt_global_flag = 0;
//-------------------------------------------------------------------------------------------------------------------
// �������     ȫ���ж�ʹ��
// ����˵��     void
// ���ز���     void
// ʹ��ʾ��     interrupt_global_enable(1);
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
void interrupt_global_enable (uint32 primask)
{
    if(primask == 0)
    {
        __enable_irq();
    }
    interrupt_global_flag ++;
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ȫ���ж�����
// ����˵��     void
// ���ز���     void
// ʹ��ʾ��     uint32 interrupt_num = interrupt_global_disable();
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
uint32 interrupt_global_disable (void)
{
    __disable_irq();
    if(interrupt_global_flag > 0) interrupt_global_flag --;
    return interrupt_global_flag; 
}


//-------------------------------------------------------------------------------------------------------------------
// �������     �����ж�����
// ����˵��     void
// ���ز���     void
// ʹ��ʾ��     assert_interrupt_config();
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
void assert_interrupt_config (void)
{
    interrupt_global_disable();                 // ȫ���ж�ʧ��
    pwm_all_channel_close();                    // �ر�PWM����ͨ�����
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ָ���ж��������ȼ�
// ����˵��     irqn            ָ���жϺ� �ɲ鿴 isr.c ��Ӧ�жϷ������ı�ע
// ����˵��     priority        �ж����ȼ� 0-7 Խ��Խ��
// ���ز���     void
// ʹ��ʾ��     interrupt_set_priority(CPUIntIdx0_IRQn, 0);
// ��ע��Ϣ     
//-------------------------------------------------------------------------------------------------------------------
void interrupt_set_priority (IRQn_Type irqn, uint8 priority)
{
    NVIC_SetPriority(irqn, priority);
}

//-------------------------------------------------------------------------------------------------------------------
// �������     �ж����ʼ��
// ����˵��     isr_config          �ж����ò����ṹ��
// ����˵��     user_isr_func       �жϻص�����
// ����˵��     priority            �ж����ȼ�
// ���ز���     void        
// ʹ��ʾ��     interrupt_init(&irq_cfg, pit_isr_func[pit_index], 3);
// ��ע��Ϣ     
//-------------------------------------------------------------------------------------------------------------------
void interrupt_init (cy_stc_sysint_irq_t *isr_config, cy_systemIntr_Handler user_isr_func, uint8 priority)
{
    Cy_SysInt_InitIRQ(isr_config);
    Cy_SysInt_SetSystemIrqVector(isr_config->sysIntSrc, user_isr_func);
    interrupt_set_priority(isr_config->intIdx, priority);
    NVIC_ClearPendingIRQ(isr_config->intIdx);
    NVIC_EnableIRQ(isr_config->intIdx);
}

