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
* �ļ�����          zf_driver_ipc
* ��˾����          �ɶ���ɿƼ����޹�˾
* �汾��Ϣ          �鿴 libraries/doc �ļ����� version �ļ� �汾˵��
* ��������          IAR 9.40.1
* ����ƽ̨          CYT4BB
* ��������          https://seekfree.taobao.com/
*
* �޸ļ�¼
* ����              ����                ��ע
* 2024-1-11       pudding            first version
********************************************************************************************************************/

#include "ipc/cy_ipc_pipe.h"
#include "ipc/cy_ipc_config.h"
#include "zf_driver_delay.h"
#include "zf_driver_ipc.h"

vuint8 ipc_port_save; 
vuint8 ipc_released; 

static void ipc_default_callback (uint32 ipc_data);

ipc_callback_function user_callback = ipc_default_callback;
//-------------------------------------------------------------------------------------------------------------------
// �������     Ĭ�ϻص�����
// ����˵��     void
// ���ز���     void
// ʹ��ʾ��     
// ��ע��Ϣ     ������������� ��ֹ��û�г�ʼ���豸��ʱ���ܷ�
//-------------------------------------------------------------------------------------------------------------------
static void ipc_default_callback (uint32 ipc_data)
{
    (void)ipc_data;
}

//-------------------------------------------------------------------------------------------------------------------
// �������     IPC���ݷ�����ɻص�����
// ����˵��     void
// ���ز���     void
// ʹ��ʾ��     
// ��ע��Ϣ     �Է��ɹ��������ݺ󽫵��ô˺�����λ�ͷű�־λ
//-------------------------------------------------------------------------------------------------------------------
void ipc_release_callback(void)
{
    ipc_released = 1;
}

//-------------------------------------------------------------------------------------------------------------------
// �������     IPC���ݽ��ջص�����
// ����˵��     ipc_msg           ����ָ��
// ���ز���     void
// ʹ��ʾ��     
// ��ע��Ϣ     ���ڴ˺��������������ݺ�����û��Ļص�����������
//-------------------------------------------------------------------------------------------------------------------
void ipc_received_callback(uint32_t *ipc_msg)
{
    communicate_data_t * data = (communicate_data_t *)ipc_msg;
    user_callback(data->data);
}


//-------------------------------------------------------------------------------------------------------------------
// �������     IPC���ݷ��ͺ���
// ����˵��     send_data           ��Ҫ���͸�����һ�����ĵ�uint32��������
// ���ز���     uint8               0���Է��ɹ���������      1���Է�����Ӧ
// ʹ��ʾ��     ipc_send_data(0x00000001);
// ��ע��Ϣ     
//-------------------------------------------------------------------------------------------------------------------
uint8 ipc_send_data(uint32 send_data)
{
    communicate_data_t pipedata;
    uint32 wait_time_us = 5000;
    uint8  return_data  = 1;
    pipedata.clientId   = 0x01;
    pipedata.data       = send_data;
    ipc_released        = 0;

    if (Cy_IPC_Pipe_SendMessage (ipc_port_save == 0 ? 1 : 0, &pipedata, ipc_release_callback) == CY_IPC_PIPE_SUCCESS)
    {
        while(wait_time_us)
        {
            if(ipc_released == 1)
            {
                return_data = 0;
                break;
            }
            else
            {
                system_delay_us(10);
                wait_time_us -= 10;
            }
        }
    }
    return return_data;
}



//-------------------------------------------------------------------------------------------------------------------
// �������     IPC���ļ�ͨѶ����
// ����˵��     ipc_port            IPC�˿� ����IPCֻ�������������� �����Ҫ���ֶ˿� �������7_0��ʼ�� IPC_PORT_1 ������7_1��ʼ�� IPC_PORT_2 
// ����˵��     ipc_callback        ipcͨѶ�ص��������������һ�����ķ������� �����ô˻ص��������û����ڻص������д�������
// ���ز���     void
// ʹ��ʾ��     ipc_communicate_init(IPC_PORT_1, my_ipc_callback);
// ��ע��Ϣ     
//-------------------------------------------------------------------------------------------------------------------
void ipc_communicate_init(ipc_port_enum ipc_port, ipc_callback_function ipc_callback)
{
    cy_stc_ipc_pipe_config_t pipeConfig = 
    {
        .epIndexForThisCpu = (uint8)ipc_port,
        .epConfigData      = CY_IPC_PIPE_ENDPOINTS_DEFAULT_CONFIG,
    };
    
    Cy_IPC_Pipe_Init(&pipeConfig);
    Cy_IPC_Pipe_RegisterCallback(ipc_received_callback, 0x01u);
    NVIC_ClearPendingIRQ(pipeConfig.epConfigData[(uint8)ipc_port].ipcCpuIntIdx);
    NVIC_EnableIRQ(pipeConfig.epConfigData[(uint8)ipc_port].ipcCpuIntIdx);
    
    user_callback = ipc_callback;
    ipc_port_save = (uint8)ipc_port;
}

