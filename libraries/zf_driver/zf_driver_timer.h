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

#ifndef _zf_driver_timer_h_
#define _zf_driver_timer_h_

#include "zf_common_typedef.h"

typedef enum
{
    TC_TIME2_CH0,
    TC_TIME2_CH1,
    TC_TIME2_CH2,
}timer_index_enum;

typedef enum
{
    TIMER_US,                                                                   // ΢���ʱ ��� 4294967296
    TIMER_MS,                                                                   // �����ʱ ��� 4294967296
}timer_mode_enum;


//====================================================timer ��������====================================================
void        timer_start                 (timer_index_enum index);						// ��ʱ������
void        timer_stop                  (timer_index_enum index);						// ��ʱ��ֹͣ
uint32      timer_get                   (timer_index_enum index);						// ��ȡ����ֵ
void        timer_clear                 (timer_index_enum index);						// �������ֵ
void        timer_init                  (timer_index_enum index, timer_mode_enum mode);	// ��ʱ����ʼ��
//====================================================timer ��������====================================================
#endif
