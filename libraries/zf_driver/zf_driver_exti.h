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

#ifndef _zf_driver_exti_h_
#define _zf_driver_exti_h_

#include "zf_driver_gpio.h"
#include "zf_common_typedef.h"


typedef enum  // ö�ٴ�����ʽ
{
    EXTI_TRIGGER_RISING 	=  1 << 0,                                                	// �����ش���ģʽ
    EXTI_TRIGGER_FALLING 	=  1 << 1,                                            		// �½��ش���ģʽ
    EXTI_TRIGGER_BOTH		= (EXTI_TRIGGER_RISING | EXTI_TRIGGER_FALLING),         	// ˫���ش���ģʽ
}exti_trigger_enum;


//====================================================EXIT ��������====================================================
uint8   exti_flag_get          (gpio_pin_enum exti_pin);                                // EXTI �ж�״̬��ȡ
void 	exti_all_close         (void);                                                  // EXTI ʧ��
void 	exti_enable            (gpio_pin_enum exti_pin);                                // EXTI �ж�ʹ��
void 	exti_disable           (gpio_pin_enum exti_pin);                                // EXTI �ж�ʧ��
void 	exti_init              (gpio_pin_enum exti_pin, exti_trigger_enum trigger);     // EXTI �жϳ�ʼ��
//====================================================EXIT ��������====================================================


#endif
