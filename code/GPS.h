/*
 * GPS.h
 *
 *  Created on: 2023��3��18��
 *      Author: ORRN
 */

#include "A_Master_Ctrl.h" //�Ƚ�����,���Դ���������뿪����־λ

#ifndef CODE_GPS_H_
#define CODE_GPS_H_

#include "zf_device_gnss.h" // ���gnssͷ�ļ�

// ȫ�ֱ���
#if Float_Record_FLAG

extern float Target_point[2][150]; // ���ڴ���ɼ���Ŀ�����Ϣ�����ں�����λ�ü���(��һ����γ�ȣ��ڶ����Ǿ���)
extern float Work_target_array[2][150]; // ���ڽ���flash�ж�ȡ��Ŀ�����������ݸ��ƹ�������������鱻��ֵʱ��������һ�к�Flash���޹�ϵ

#endif

#if Double_Record_FLAG

extern double Target_point[2][150]; // ���ڴ���ɼ���Ŀ�����Ϣ�����ں�����λ�ü���(��һ����γ�ȣ��ڶ����Ǿ���)
extern double Work_target_array[2][150]; // ���ڽ���flash�ж�ȡ��Ŀ�����������ݸ��ƹ�������������鱻��ֵʱ��������һ�к�Flash���޹�ϵ

#endif

#if Int_Record_FLAG

extern uint32 Target_point[2][150]; // ���ڴ���ɼ���Ŀ�����Ϣ�����ں�����λ�ü���(��һ����γ�ȣ��ڶ����Ǿ���)
extern uint32 Work_target_array[2][150]; // ���ڽ���flash�ж�ȡ��Ŀ�����������ݸ��ƹ�������������鱻��ֵʱ��������һ�к�Flash���޹�ϵ

#endif

#if Array_Record_FLAG

extern double GPS_GET_LAT[];
extern double GPS_GET_LOT[];

#endif

// �궨��
#define RP_MAX 20 // ���õĲɼ�������(�Լ�����)
#define Point_NUM 10 // ���Ȼ�γ�Ȳɼ�����
#define FLASH_SECTION_INDEX 0 // �洢�����õ�����
#define FLASH_PAGE_INDEX 11 // �洢�����õ�ҳ�� ������һ��ҳ��
#define GPS_PAGE_INDEX 11 // GPS�洢ҳ��

// ��������
void GPS_Record_flash(void); // ���ɼ��ĵ�λ��¼����������������GPS_FLASH
void GPS_Flash_use(void); // ��GPS_FLASH���������¶��ػ���������ֵ������
void GPS_Work_array(void); // ���ڸ���FLASH,ʵ�ʲμӼ��������
void GPS_SHOW(void); // GPS��Ϣ��ʾ
void GPS_param_t_init(void); // GPS������ʼ��

#endif /* CODE_GPS_H_ */
