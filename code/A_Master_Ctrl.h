/*
 * A_Master_Ctrl.h
 *
 *  Created on: 2023��4��11��
 *      Author: ORRN
 */

#ifndef CODE_A_MASTER_CTRL_H_
#define CODE_A_MASTER_CTRL_H_

#define ZHX_MATH_FLAG      (0)
#define GL_MATH_FLAG       (1)

#define Main_FLAG          (1)          //����������־λ

#define Float_FLAG         (0)          //������ģʽ������־λ(GPS��ѧ����������Ϊ������)
#define Double_FLAG        (0)          //˫����ģʽ������־λ(GPS��ѧ����������Ϊ˫����)
#define Int_FLAG_D         (0)          //����ģʽ������־λ(GPS��ѧ����������Ϊ����,�ڲ����㺯������double)
#define GL_FLAG            (1)

#define Float_Record_FLAG  (0)          //�����ȸ���洢������־λ//����
#define Double_Record_FLAG (0)          //˫���ȸ���洢������־λ//����
#define Int_Record_FLAG    (1)          //�����洢������־λ     //����
#define Array_Record_FLAG  (1)          //����洢��־λ        //����

#define ISR_Start_FLAG     (1)          //�жϿ�����־λ
#define IMU_Restrain_FLAG  (0)          //ֱ�����Ʋ���
#define IMU_STATR_FLAG     (1)          //IMU������־λ(IMU��ʼ��ʱ��̫����)
#define FLASH_Init_FLAG    (0)          //FLASH���ݳ�ʼ����־λ

#define IMU_Type_FLAG      (1)          //IMU���ͱ�־λ

#define USART_Send_STOP_FLAG (0)        //�������󱱶����ӱ�־λ

#define MIC_Start_FLAG        (1)        //��������--����ɼ��ǲ��صģ������־�Ǹ��Ƴ����������ʱ�õ�


#define MIC_Mode           (0)           //��������־λ����ΪFLOOWTRACK����׼����
#define GPS_Mode           (1)
#define GPS_MIC_Mode       (0)

//��������
//Ϊ1,���Ա���||Ϊ0,��ֹ����

//������־λ-˵����
//1.�ж�ADS�ǵ�����ģʽ����˫����ģʽ(������ģʽ�£�float��double������float����)
//2.�жϲ��ú�����ѧ��:Float_FLAG-GPS���õ�������ѧ��(ADS�Դ���)||Double_FLAG-GPS����˫������ѧ��(�Զ����)||Int_FLAG_D-GPS���������ββ�����Ĭ����ѧ��
//3.�жϲŲ��ú���FLASH�洢��ʽ:Float_Record_FLAG-������||Double_Record_FLAG-˫����||Int_Record_FLAG-�����洢

//�̶�ģʽ��
//Float_FLAG  (1)---------Float_Record_FLAG  (1)//������
//Double_FLAG (1)---------Double_Record_FLAG (1)//˫����
//Int_FLAG_D  (1)---------Int_Record_FLAG    (1)//����

/*
����͵����һ����ʱ��--7ms

ռ�ձ�         ����
0              0
1000           50
2000           100
3000           140
4000           170
5000           190
6000           210
7000           220
8000           230
9000           240
10000          250
 */



#endif /* CODE_A_MASTER_CTRL_H_ */
