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
* �ļ�����          zf_driver_flash
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

#ifndef _zf_driver_flash_h_
#define _zf_driver_flash_h_

#include "flash/cy_mw_flash.h"
#include "flash/cy_flash.h"
#include "zf_common_typedef.h"

#define FLASH_BASE_ADDR             (CY_WFLASH_LG_SBM_TOP)			                // flash ��ʼ��ַ
#define FLASH_SIZE                  (CY_WFLASH_LG_SBM_SIZE)            				// һ���� 192KB
#define FLASH_PAGE_SIZE             (CY_WORK_LES_SIZE_IN_BYTE)					// ÿҳ��СΪ 2kb 
#define FLASH_PAGE_NUM              (CY_WFLASH_LG_SBM_SIZE / FLASH_PAGE_SIZE)   	        // 192KB ��Ϊ�� 96 ҳ

#define FLASH_DATA_SIZE             (4)							        // ���ݴ�СΪ 4 byte
#define FLASH_PAGE_LENGTH           (FLASH_PAGE_SIZE / 4)         				// ÿҳ���Դ� 512 �� uint32 ���͵�����


typedef union                                                                   		// �̶������ݻ��嵥Ԫ��ʽ
{
    float   float_type;                                                         		// float  ����
    uint32  uint32_type;                                                        		// uint32 ����
    int32   int32_type;                                                         		// int32  ����
    uint16  uint16_type;                                                        		// uint16 ����
    int16   int16_type;                                                         		// int16  ����
    uint8   uint8_type;                                                         		// uint8  ����
    int8    int8_type;                                                          		// int8   ����
}flash_data_union;                                                              		// �����������ݹ���ͬһ�� 32bit ��ַ

extern flash_data_union flash_union_buffer[FLASH_PAGE_LENGTH];

//====================================================FLASH ��������====================================================
uint8   flash_check                     (uint32 sector_num, uint32 page_num);                                    // У��FLASHҳ�Ƿ�������
void    flash_erase_page                (uint32 sector_num, uint32 page_num);                                   // ����ҳ
void    flash_read_page                 (uint32 sector_num, uint32 page_num, uint32 *buf, uint32 len);          // ��ȡһҳ
void    flash_write_page                (uint32 sector_num, uint32 page_num, const uint32 *buf, uint32 len);   // ���һҳ
void    flash_read_page_to_buffer       (uint32 sector_num, uint32 page_num, uint32 len);                       // ��ָ�� FLASH ��ָ��ҳ���ȡ���ݵ�������
uint8   flash_write_page_from_buffer    (uint32 sector_num, uint32 page_num, uint32 len);                       // ��ָ�� FLASH ��������ָ��ҳ��д�뻺����������
void    flash_buffer_clear              (void);                                                                // ������ݻ�����
void 	flash_init 						(void);						// flash��ʼ��
//====================================================FALSH ��������====================================================


#endif
