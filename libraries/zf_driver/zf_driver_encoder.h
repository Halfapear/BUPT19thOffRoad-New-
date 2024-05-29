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
* �ļ�����          zf_driver_encoder
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

#ifndef _zf_driver_encoder_h_
#define _zf_driver_encoder_h_

#include "zf_common_typedef.h"

// ��ö�ٶ��岻�����û��޸�
typedef enum // ö�ٱ���������
{
    TC_CH07_ENCODER_CH1_P07_6,    // tcpwmģ���ͨ��7  �������ſ�ѡ��Χ
    TC_CH20_ENCODER_CH1_P08_1,    // tcpwmģ���ͨ��20 �������ſ�ѡ��Χ
    TC_CH27_ENCODER_CH1_P19_2,    // tcpwmģ���ͨ��27 �������ſ�ѡ��Χ	
    TC_CH58_ENCODER_CH1_P17_3,    // tcpwmģ���ͨ��58 �������ſ�ѡ��Χ
}encoder_channel1_enum;

// ��ö�ٶ��岻�����û��޸�
typedef enum // ö�ٱ���������
{
    TC_CH07_ENCODER_CH2_P07_7,    // tcpwmģ���ͨ��7  �������ſ�ѡ��Χ	 
    TC_CH20_ENCODER_CH2_P08_2,    // tcpwmģ���ͨ��20 �������ſ�ѡ��Χ
    TC_CH27_ENCODER_CH2_P19_3,    // tcpwmģ���ͨ��27 �������ſ�ѡ��Χ	
    TC_CH58_ENCODER_CH2_P17_4,    // tcpwmģ���ͨ��58 �������ſ�ѡ��Χ
}encoder_channel2_enum;

typedef enum  // ö�� ��ʱ�����
{
    TC_CH07_ENCODER,
    TC_CH20_ENCODER,
    TC_CH27_ENCODER,
    TC_CH58_ENCODER,
}encoder_index_enum;

//====================================================������ ��������====================================================
int16 encoder_get_count     (encoder_index_enum encoder_n);
void  encoder_clear_count   (encoder_index_enum encoder_n);

//void  encoder_quad_init     (encoder_index_enum encoder_n, encoder_channel1_enum count_pin, encoder_channel2_enum dir_pin);
void  encoder_dir_init      (encoder_index_enum encoder_n, encoder_channel1_enum ch1_pin, encoder_channel2_enum ch2_pin);
//====================================================������ ��������====================================================

#endif
