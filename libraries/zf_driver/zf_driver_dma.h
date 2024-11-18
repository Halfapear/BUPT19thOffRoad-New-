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
* �ļ�����          zf_driver_dma
* ��˾����          �ɶ���ɿƼ����޹�˾
* �汾��Ϣ          �鿴 libraries/doc �ļ����� version �ļ� �汾˵��
* ��������          MDK 5.38a
* ����ƽ̨          CYT4BB7CEE
* ��������          https://seekfree.taobao.com/
*
* �޸ļ�¼
* ����              ����                ��ע
* 2023-11-30       pudding            first version
********************************************************************************************************************/

#ifndef _zf_driver_dma_h_
#define _zf_driver_dma_h_

#include "zf_common_typedef.h"
#include "zf_driver_exti.h"

//====================================================DMA ��������====================================================
void  dma_init          (uint8 *source_addr, uint8 *destination_addr, gpio_pin_enum exti_pin, exti_trigger_enum trigger, uint16 dma_count);
void  dma_config        (uint8 *source_addr, uint8 *destination_addr);
void  dma_disable       (void);
void  dma_enable        (void);
//====================================================DMA ��������====================================================

#endif
