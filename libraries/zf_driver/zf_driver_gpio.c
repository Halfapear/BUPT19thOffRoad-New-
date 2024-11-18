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
* �ļ�����          zf_driver_gpio
* ��˾����          �ɶ���ɿƼ����޹�˾
* �汾��Ϣ          �鿴 libraries/doc �ļ����� version �ļ� �汾˵��
* ��������          IAR 9.40.1
* ����ƽ̨          CYT4BB
* ��������          https://seekfree.taobao.com/
*
* �޸ļ�¼
* ����              ����                ��ע
* 2024-1-4       pudding            first version
********************************************************************************************************************/

#include "gpio/cy_gpio.h"
#include "zf_driver_gpio.h"




//-------------------------------------------------------------------------------------------------------------------
// �������     gpio �������
// ����˵��     pin         ѡ������� (��ѡ��Χ�� zf_driver_gpio.h �� gpio_pin_enum ö��ֵȷ��)
// ����˵��     dat         0���͵�ƽ 1���ߵ�ƽ
// ���ز���     void
// ʹ��ʾ��     gpio_set_level(P00_0, 1);// P00_0 ����ߵ�ƽ
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
void gpio_set_level (gpio_pin_enum pin, uint8 dat)
{
    if(dat)
    {
        Cy_GPIO_Set(get_port(pin), (pin % 8));
    }
    else
    {
        Cy_GPIO_Clr(get_port(pin), (pin % 8));
    }
}
//-------------------------------------------------------------------------------------------------------------------
// �������     gpio ��ƽ��ȡ
// ����˵��     pin         ѡ������� (��ѡ��Χ�� zf_driver_gpio.h �� gpio_pin_enum ö��ֵȷ��)
// ���ز���     uint8       ���ŵ�ǰ��ƽ
// ʹ��ʾ��     uint8 status = gpio_get_level(P00_0);// ��ȡP00_0���ŵ�ƽ
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
uint8 gpio_get_level (gpio_pin_enum pin)
{
    return (uint8)Cy_GPIO_Read(get_port(pin), (pin % 8));
}

//-------------------------------------------------------------------------------------------------------------------
// �������     gpio ��ת��ƽ
// ����˵��     pin         ѡ������� (��ѡ��Χ�� zf_driver_gpio.h �� gpio_pin_enum ö��ֵȷ��)
// ���ز���     void
// ʹ��ʾ��     gpio_toggle_level(P00_0);// P00_0���ŵ�ƽ��ת
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
void gpio_toggle_level (gpio_pin_enum pin)
{
    Cy_GPIO_Inv(get_port(pin), (pin % 8));
}

//-------------------------------------------------------------------------------------------------------------------
// �������     gpio ��������
// ����˵��     pin         ѡ������� (��ѡ��Χ�� zf_driver_gpio.h �� gpio_pin_enum ö��ֵȷ��)
// ����˵��     dir         ���ŵķ���   �����GPO   ���룺GPI
// ����˵��     mode        ���ŵ�ģʽ (��ѡ��Χ�� zf_driver_gpio.h �� gpio_mode_enum ö��ֵȷ��)
// ���ز���     void
// ʹ��ʾ��     gpio_set_dir(P00_0, GPO, GPO_PUSH_PULL);// ����P00_0Ϊ�������ģʽ
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
void gpio_set_dir (gpio_pin_enum pin, gpio_dir_enum dir, gpio_mode_enum pinmode)
{
    uint32 port_mode;
    if(dir == GPI)
    {
        switch(pinmode)
        {
            case GPI_FLOATING_IN:port_mode = CY_GPIO_DM_PULLUP_DOWN;		break; // ��������
            case GPI_PULL_DOWN  :port_mode = CY_GPIO_DM_PULLDOWN;       	break; // ��������
            default:             port_mode = CY_GPIO_DM_PULLUP;       		break; // Ĭ��Ϊ��������
        }
    }
    else
    {
        switch(pinmode)
        {
            case GPO_OPEN_DTAIN :port_mode = CY_GPIO_DM_OD_DRIVESLOW; 		break;// ��©���
            default:             port_mode = CY_GPIO_DM_STRONG ;   			break;// Ĭ��Ϊ�������
        }
		port_mode &= (0x07UL);
    }

    Cy_GPIO_SetDrivemode(get_port(pin), (pin % 8), port_mode);
}

//-------------------------------------------------------------------------------------------------------------------
// �������     gpio ��ʼ��
// ����˵��     pin         ѡ������� (��ѡ��Χ�� zf_driver_gpio.h �� gpio_pin_enum ö��ֵȷ��)
// ����˵��     mode        ���ŵķ��� [GPI/GPIO]
// ����˵��     dat         ���ų�ʼ��ʱ���õĵ�ƽ״̬�����ʱ��Ч 0���͵�ƽ 1���ߵ�ƽ ��������Ϊ���ģʽʱ��Ч
// ����˵��     mode        ���ŵ�ģʽ (��ѡ��Χ�� zf_driver_gpio.h �� gpio_mode_enum ö��ֵȷ��)
// ���ز���     void
// ʹ��ʾ��     gpio_init(P19_0, GPO, 0, GPO_PUSH_PULL); // P19_0��ʼ��ΪGPIO���ܡ����ģʽ������͵�ƽ���������
// ��ע��Ϣ     
//-------------------------------------------------------------------------------------------------------------------
void gpio_init (gpio_pin_enum pin, gpio_dir_enum dir, uint8 dat, gpio_mode_enum pinmode)
{
    cy_stc_gpio_pin_config_t gpio_pin_config = {0};
    
    if(dir == GPI)
    {
        switch(pinmode)
        {
            case GPI_FLOATING_IN:gpio_pin_config.driveMode = CY_GPIO_DM_PULLUP_DOWN;	break; // ��������
            case GPI_PULL_DOWN  :gpio_pin_config.driveMode = CY_GPIO_DM_HIGHZ;       break; // ��������
            default:             gpio_pin_config.driveMode = CY_GPIO_DM_HIGHZ;       	break; // Ĭ��Ϊ��������
        }
    }
    else
    {
        switch(pinmode)
        {
            case GPO_OPEN_DTAIN :gpio_pin_config.driveMode = CY_GPIO_DM_PULLUP_DOWN; 		break;// ��©���
            default:             gpio_pin_config.driveMode = CY_GPIO_DM_STRONG ;   		break;// Ĭ��Ϊ�������
        }
    }
    if(pin != NC)
    {
        Cy_GPIO_Pin_Init(get_port(pin), (pin % 8), &gpio_pin_config);
    }
    gpio_set_level(pin, dat);
}



