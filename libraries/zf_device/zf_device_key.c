/*********************************************************************************************************************
* CYT4BB Opensourec Library ���� CYT4BB ��Դ�⣩��һ�����ڹٷ� SDK �ӿڵĵ�������Դ��
* Copyright (c) 2022 SEEKFREE ��ɿƼ�
*
* ���ļ��� CYT4BB ��Դ���һ����
*
* CYT4BB ��Դ�� ���������
* �����Ը���������������ᷢ���� GPL��GNU General Public License���� GNUͨ�ù�������֤��������
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
* ����Դ��ʹ�� GPL3.0 ��Դ����֤Э�� ������������Ϊ���İ汾
* ��������Ӣ�İ��� libraries/doc �ļ����µ� GPL3_permission_statement.txt �ļ���
* ����֤������ libraries �ļ����� �����ļ����µ� LICENSE �ļ�
* ��ӭ��λʹ�ò����������� ���޸�����ʱ���뱣����ɿƼ��İ�Ȩ����������������
*
* �ļ�����          zf_device_key
* ��˾����          �ɶ���ɿƼ����޹�˾
* �汾��Ϣ          �鿴 libraries/doc �ļ����� version �ļ� �汾˵��
* ��������          IAR 9.40.1
* ����ƽ̨          CYT4BB
* ��������          https://seekfree.taobao.com/
*
* �޸ļ�¼
* ����              ����                ��ע
* 2024-01-12       pudding           first version
********************************************************************************************************************/
/*********************************************************************************************************************
* ���߶��壺
*                   ------------------------------------
*                   ģ��ܽ�            ��Ƭ���ܽ�
*                   // һ�������尴����Ӧ������
*                   KEY1/S1             �鿴 zf_device_key.h �� KEY_LIST[0]
*                   KEY2/S2             �鿴 zf_device_key.h �� KEY_LIST[1]
*                   KEY3/S3             �鿴 zf_device_key.h �� KEY_LIST[2]
*                   KEY4/S4             �鿴 zf_device_key.h �� KEY_LIST[3]
*                   ------------------------------------
********************************************************************************************************************/

#include "zf_common_debug.h"

#include "zf_device_key.h"

static uint32               scanner_period = 0;                                 // ������ɨ������
static uint32               key_press_time[KEY_NUMBER];                         // �����źų���ʱ��
static key_state_enum       key_state[KEY_NUMBER];                              // ����״̬

static const gpio_pin_enum  key_index[KEY_NUMBER] = KEY_LIST;                   // �����б�

//-------------------------------------------------------------------------------------------------------------------
// �������     ����״̬ɨ��
// ����˵��     void
// ���ز���     void
// ʹ��ʾ��     key_scanner();
// ��ע��Ϣ     �������������ѭ������ PIT �ж���
//-------------------------------------------------------------------------------------------------------------------
void key_scanner (void)
{
    uint8 i = 0;
    for(i = 0; KEY_NUMBER > i; i ++)
    {
        if(KEY_RELEASE_LEVEL != gpio_get_level(key_index[i]))                   // ��������
        {
            key_press_time[i] ++;
            if(KEY_LONG_PRESS_PERIOD / scanner_period <= key_press_time[i])
            {
                key_state[i] = KEY_LONG_PRESS;
            }
        }
        else                                                                    // �����ͷ�
        {
            if((KEY_LONG_PRESS != key_state[i]) && (KEY_MAX_SHOCK_PERIOD / scanner_period <= key_press_time[i]))
            {
                key_state[i] = KEY_SHORT_PRESS;
            }
            else
            {
                key_state[i] = KEY_RELEASE;
            }
            key_press_time[i] = 0;
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ��ȡ����״̬
// ����˵��     key_n           ��������
// ���ز���     key_state_enum  ����״̬
// ʹ��ʾ��     key_get_state(KEY_1);
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
key_state_enum key_get_state (key_index_enum key_n)
{
    return key_state[key_n];
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ���ָ������״̬
// ����˵��     key_n           ��������
// ���ز���     void            ��
// ʹ��ʾ��     key_clear_state(KEY_1);
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
void key_clear_state (key_index_enum key_n)
{
    key_state[key_n] = KEY_RELEASE;
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ������а���״̬
// ����˵��     void            ��
// ���ز���     void            ��
// ʹ��ʾ��     key_clear_all_state();
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
void key_clear_all_state (void)
{
    key_state[0] = KEY_RELEASE;
    key_state[1] = KEY_RELEASE;
    key_state[2] = KEY_RELEASE;
    key_state[3] = KEY_RELEASE;
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ������ʼ��
// ����˵��     period          ����ɨ������ �Ժ���Ϊ��λ
// ���ز���     void
// ʹ��ʾ��     key_init(10);
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
void key_init (uint32 period)
{
    zf_assert(0 < period);
    uint8 loop_temp = 0; 
    for(loop_temp = 0; KEY_NUMBER > loop_temp; loop_temp ++)
    {
        gpio_init(key_index[loop_temp], GPI, GPIO_HIGH, GPI_PULL_UP);
        key_state[loop_temp] = KEY_RELEASE;
    }
    scanner_period = period;
}