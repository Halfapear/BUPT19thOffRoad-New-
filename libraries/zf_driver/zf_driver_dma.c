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


#include "zf_common_debug.h"
#include "zf_driver_exti.h"
#include "zf_driver_dma.h"

//#define BUFFER_SIZE (32)
//#define PERIPHERAL_FIFO_ADDR (0x4001000)
//
//
//void dma_event_callback(void* callback_arg, cyhal_dma_event_t event);
//cyhal_dma_t     dma_obj;
//cyhal_source_t dma_source;

//-------------------------------------------------------------------------------------------------------------------
// �������     dma �����ֹ
// ����˵��     ch              ѡ�� dma ͨ�� (��� zf_driver_dma.h ��ö�� dma_channel_enum ����)
// ���ز���     void
// ʹ��ʾ��     dma_disable();
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
void dma_disable (void)
{
//	Cy_DMA_Channel_Disable(DW0, dma_obj.resource.channel_num); // ʧ��DMAģ��																
}


//-------------------------------------------------------------------------------------------------------------------
// �������     dma ����ʹ��
// ����˵��     ch              ѡ�� dma ͨ�� (��� zf_driver_dma.h ��ö�� dma_channel_enum ����)
// ���ز���     void
// ʹ��ʾ��     dma_enable();
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
void dma_enable (void)
{
//    Cy_DMA_Channel_Enable(DW1, dma_obj.resource.channel_num); // ʹ��DMAģ��
}

//-------------------------------------------------------------------------------------------------------------------
// �������      dma�����޸�
// ����˵��      source_addr         ����Դ��ַ
// ����˵��      destination_addr    ����Ŀ�ĵ�ַ
// ����˵��      dma_count           ����dma���ƴ���
// ���ز���      uint8
// ʹ��ʾ��      dma_config(MT9V03X_DATA_ADD, mt9v03x_image[0]);
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
void dma_config (uint8 *source_addr, uint8 *destination_addr)
{
//	dma_obj.descriptor.dw.dst = (uint32_t)destination_addr;
//	dma_obj.descriptor.dw.src = (uint32_t)source_addr;
}
//-------------------------------------------------------------------------------------------------------------------
// �������      dma��ʼ��
// ����˵��      dma_ch              ѡ��DMAͨ��
// ����˵��      source_addr         ����Դ��ַ
// ����˵��      destination_addr    ����Ŀ�ĵ�ַ
// ����˵��      exti_pin            ���ô�����eruͨ��
// ����˵��      trigger             ���ô�����ʽ
// ����˵��      dma_count           ����dma���ƴ���
// ���ز���      uint8
// ʹ��ʾ��      dma_init(MT9V03X_DATA_ADD, mt9v03x_image[0], MT9V03X_PCLK_PIN, EXTI_TRIGGER_RISING, MT9V03X_IMAGE_SIZE);
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
void dma_init (uint8 *source_addr, uint8 *destination_addr, gpio_pin_enum exti_pin, exti_trigger_enum trigger, uint16 dma_count)
{
    
  
  
  
}



