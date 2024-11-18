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
* �ļ�����          zf_driver_uart
* ��˾����          �ɶ���ɿƼ����޹�˾
* �汾��Ϣ          �鿴 libraries/doc �ļ����� version �ļ� �汾˵��
* ��������          IAR 9.40.1
* ����ƽ̨          CYT4BB
* ��������          https://seekfree.taobao.com/
*
* �޸ļ�¼
* ����              ����                ��ע
* 2024-1-5       pudding            first version
* 2024-3-2       pudding            �޸�������ڲ����ʲ������²������쳣������
********************************************************************************************************************/

#include "system/system_cyt4bb.h"
#include "sysclk/cy_sysclk.h"
#include "gpio/cy_gpio.h"
#include "zf_common_interrupt.h"
#include "zf_common_debug.h"
#include "zf_driver_gpio.h"
#include "zf_driver_uart.h"

#define UART_FREQ       CY_INITIAL_TARGET_PERI_FREQ                             // ����ģ��ʱ�� Ĭ��80M
#define UART_USE_ISR    CPUIntIdx3_IRQn



void uart0_isr (void);
void uart1_isr (void);
void uart2_isr (void);
void uart3_isr (void);
void uart4_isr (void);

typedef struct
{ 
    volatile stc_GPIO_PRT_t     *rx_port;
    uint8                       rx_pin;
    en_hsiom_sel_t              rx_hsiom;
    volatile stc_GPIO_PRT_t     *tx_port;
    uint8                       tx_pin;
    en_hsiom_sel_t              tx_hsiom;
    en_clk_dst_t                uart_pclk;
    cy_en_intr_t                uart_irqn;
}uart_config_struct;

void (*uart_isr_func[5])() = {uart0_isr, uart1_isr, uart2_isr, uart3_isr, uart4_isr};
cy_stc_scb_uart_context_t  uart_context[5] = {0}; 
volatile stc_SCB_t* scb_module[5] = {SCB0, SCB5, SCB4, SCB3, SCB2};
//-------------------------------------------------------------------------------------------------------------------
// �������       ��ȡ����������Ϣ
// ����˵��       uart_n          ����ģ��� ���� zf_driver_uart.h �� uart_index_enum ö���嶨��
// ����˵��       tx_pin          ���ڷ�������
// ����˵��       rx_pin          ���ڽ�������
// ���ز���       void
// ʹ��ʾ��       get_uart_config(&uart_pin_config, tx_pin, rx_pin);                  
// ��ע��Ϣ       �ڲ����� �û��������
//-------------------------------------------------------------------------------------------------------------------
static void get_uart_config(uart_config_struct *config_struct, uart_tx_pin_enum tx_pin, uart_rx_pin_enum rx_pin)
{
    switch(tx_pin)
    {
        case  	UART0_TX_P00_1:
        {
            config_struct->tx_port  = GPIO_PRT0 ;  	
            config_struct->tx_pin   = 1;      
            config_struct->tx_hsiom = P0_1_SCB0_UART_TX;  
            config_struct->uart_pclk= PCLK_SCB0_CLOCK;
            config_struct->uart_irqn= scb_0_interrupt_IRQn;
        }break;
        case	UART1_TX_P04_1:
        {
            config_struct->tx_port  = GPIO_PRT4 ;  	
            config_struct->tx_pin   = 1;      
            config_struct->tx_hsiom = P4_1_SCB5_UART_TX; 
            config_struct->uart_pclk= PCLK_SCB5_CLOCK;
            config_struct->uart_irqn= scb_5_interrupt_IRQn;
        }break;
        case	UART2_TX_P10_1:
        {
            config_struct->tx_port  = GPIO_PRT10;  	
            config_struct->tx_pin   = 1;      
            config_struct->tx_hsiom = P10_1_SCB4_UART_TX; 
            config_struct->uart_pclk= PCLK_SCB4_CLOCK;
            config_struct->uart_irqn= scb_4_interrupt_IRQn;
        }break;
        case  	UART3_TX_P17_2: 
        {
            config_struct->tx_port  = GPIO_PRT17;  	
            config_struct->tx_pin   = 2;      
            config_struct->tx_hsiom = P17_2_SCB3_UART_TX; 
            config_struct->uart_pclk= PCLK_SCB3_CLOCK;
            config_struct->uart_irqn= scb_3_interrupt_IRQn;
        }break;
        case  	UART4_TX_P14_1:
        {
            config_struct->tx_port  = GPIO_PRT14;  	
            config_struct->tx_pin   = 1;      
            config_struct->tx_hsiom = P14_1_SCB2_UART_TX; 
            config_struct->uart_pclk= PCLK_SCB2_CLOCK;
            config_struct->uart_irqn= scb_2_interrupt_IRQn;
        }break;
        default: zf_assert(0);     break;
    }
    switch(rx_pin)
    {
        case  	UART0_RX_P00_0:
        {
            config_struct->rx_port  = GPIO_PRT0 ;  	
            config_struct->rx_pin   = 0;      
            config_struct->rx_hsiom = P0_0_SCB0_UART_RX;  
            config_struct->uart_pclk= PCLK_SCB0_CLOCK;
            config_struct->uart_irqn= scb_0_interrupt_IRQn;
        }break;
        case	UART1_RX_P04_0: 
        {
            config_struct->rx_port  = GPIO_PRT4 ;  	
            config_struct->rx_pin   = 0;      
            config_struct->rx_hsiom = P4_0_SCB5_UART_RX;
            config_struct->uart_pclk= PCLK_SCB5_CLOCK;
            config_struct->uart_irqn= scb_5_interrupt_IRQn;
        }break;
        case	UART2_RX_P10_0: 
        {
            config_struct->rx_port  = GPIO_PRT10;  	
            config_struct->rx_pin   = 0;      
            config_struct->rx_hsiom = P10_0_SCB4_UART_RX; 
            config_struct->uart_pclk= PCLK_SCB4_CLOCK;
            config_struct->uart_irqn= scb_4_interrupt_IRQn;
        }break;
        case  	UART3_RX_P17_1: 
        {
            config_struct->rx_port  = GPIO_PRT17;  	
            config_struct->rx_pin   = 1;      
            config_struct->rx_hsiom = P17_1_SCB3_UART_RX; 
            config_struct->uart_pclk= PCLK_SCB3_CLOCK;
            config_struct->uart_irqn= scb_3_interrupt_IRQn;
        }break;
        case  	UART4_RX_P14_0: 
        {
            config_struct->rx_port  = GPIO_PRT14;  	
            config_struct->rx_pin   = 0;      
            config_struct->rx_hsiom = P14_0_SCB2_UART_RX; 
            config_struct->uart_pclk= PCLK_SCB2_CLOCK;
            config_struct->uart_irqn= scb_2_interrupt_IRQn;
        }break;
        default: zf_assert(0);     break;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// �������       ��ȡ����SCBģ���
// ����˵��       uart_n          ����ģ��� ���� zf_driver_uart.h �� uart_index_enum ö���嶨��
// ���ز���       stc_SCB_t       ģ��ṹ��
// ʹ��ʾ��       get_scb_module(UART_0);                   
// ��ע��Ϣ       �ڲ����� �û��������
//-------------------------------------------------------------------------------------------------------------------
volatile stc_SCB_t* get_scb_module(uart_index_enum uart_n)
{
    volatile stc_SCB_t* temp_module;
    switch(uart_n)
    {
        case UART_0: temp_module = SCB0; break;
        case UART_1: temp_module = SCB5; break;
        case UART_2: temp_module = SCB4; break;
        case UART_3: temp_module = SCB3; break;
        case UART_4: temp_module = SCB2; break;
        default: zf_assert(0);     break;
    }
    return temp_module;
}

//-------------------------------------------------------------------------------------------------------------------
// �������       ���ڷ���д��
// ����˵��       uart_n          ����ģ��� ���� zf_driver_uart.h �� uart_index_enum ö���嶨��
// ����˵��       dat             ��Ҫ���͵��ֽ�
// ���ز���       void
// ʹ��ʾ��       uart_write_byte(UART_0, 0xA5);                    // ������1�ķ��ͻ�����д��0xA5��д�����Ȼ�ᷢ�����ݣ����ǻ����CPU�ڴ��ڵ�ִ��ʱ
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
void uart_write_byte (uart_index_enum uart_n, const uint8 dat)
{
    Cy_SCB_WriteTxFifo(scb_module[uart_n], dat);	
    while(Cy_SCB_IsTxComplete(scb_module[uart_n]) == 0);
}


//-------------------------------------------------------------------------------------------------------------------
// �������       ���ڷ�������
// ����˵��       uart_n          ����ģ��� ���� zf_driver_uart.h �� uart_index_enum ö���嶨��
// ����˵��       *buff           Ҫ���͵������ַ
// ����˵��       len             ���ͳ���
// ���ز���       void
// ʹ��ʾ��       uart_write_buffer(UART_0, &a[0], 5);
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
void uart_write_buffer (uart_index_enum uart_n, const uint8 *buff, uint32 len)
{
    while(len)
    {
        uart_write_byte(uart_n, *buff);
        len--;
        buff++;
    }
}


//-------------------------------------------------------------------------------------------------------------------
// �������       ���ڷ����ַ���
// ����˵��       uart_n          ����ģ��� ���� zf_driver_uart.h �� uart_index_enum ö���嶨��
// ����˵��       *str            Ҫ���͵��ַ�����ַ
// ���ز���       void
// ʹ��ʾ��       uart_write_string(UART_0, "seekfree");
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
void uart_write_string (uart_index_enum uart_n, const char *str)
{
    while(*str)
    {
        uart_write_byte(uart_n, *str++);
    }
}

//-------------------------------------------------------------------------------------------------------------------
// �������       ��ȡ���ڽ��յ����ݣ�whlie�ȴ���
// ����˵��       uart_n          ����ģ��� ���� zf_driver_uart.h �� uart_index_enum ö���嶨��
// ����˵��       *dat            �������ݵĵ�ַ
// ���ز���       uint8           ���յ�����
// ʹ��ʾ��       uint8 dat = uart_read_byte(UART_0);             // ���� UART_1 ����  ������ dat ������
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
uint8 uart_read_byte (uart_index_enum uart_n)
{
    while(Cy_SCB_GetNumInRxFifo(scb_module[uart_n]) == 0);
    return (uint8)Cy_SCB_ReadRxFifo(scb_module[uart_n]);	
}
//-------------------------------------------------------------------------------------------------------------------
// �������       ��ȡ���ڽ��յ����ݣ���ѯ���գ�
// ����˵��       uart_n          ����ģ��� ���� zf_driver_uart.h �� uart_index_enum ö���嶨��
// ����˵��       *dat            �������ݵĵ�ַ
// ���ز���       uint8           1�����ճɹ�   0��δ���յ�����
// ʹ��ʾ��       uint8 dat; uart_query_byte(UART_0, &dat);       // ���� UART_1 ����  ������ dat ������
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
uint8 uart_query_byte (uart_index_enum uart_n, uint8 *dat)
{
    uint8 return_data = 0;
    
    if(Cy_SCB_GetNumInRxFifo(scb_module[uart_n]))
    {
        *dat = (uint8_t)Cy_SCB_ReadRxFifo(scb_module[uart_n]);
        return_data = 1;
    }
    else
    {
        return_data = 0;
    }
    return return_data;
}

//-------------------------------------------------------------------------------------------------------------------
// �������       ���ڷ����ж�����
// ����˵��       uart_n           ����ģ���
// ����˵��       status          1�����ж�   0���ر��ж�
// ���ز���       void
// ʹ��ʾ��       uart_tx_interrupt(UART_0, 1);                   // �򿪴���1�����ж�
// ��ע��Ϣ	      �����жϺͷ����ж�ֻ��ѡ��һ��������ͬʱ���ã�������õ�Ϊ��Ч
//-------------------------------------------------------------------------------------------------------------------
void uart_tx_interrupt (uart_index_enum uart_n, uint32 status)
{
    
    Cy_SCB_SetTxInterruptMask(scb_module[uart_n], Cy_SCB_GetTxInterruptMask(scb_module[uart_n]) & ~CY_SCB_UART_TX_TRIGGER);
    Cy_SCB_SetTxInterruptMask(scb_module[uart_n], Cy_SCB_GetTxInterruptMask(scb_module[uart_n]) & ~CY_SCB_UART_TX_NOT_FULL);
    Cy_SCB_SetTxInterruptMask(scb_module[uart_n], Cy_SCB_GetTxInterruptMask(scb_module[uart_n]) & ~CY_SCB_UART_TX_OVERFLOW);
    Cy_SCB_SetTxInterruptMask(scb_module[uart_n], Cy_SCB_GetTxInterruptMask(scb_module[uart_n]) & ~CY_SCB_UART_TX_UNDERFLOW);
    Cy_SCB_SetTxInterruptMask(scb_module[uart_n], Cy_SCB_GetTxInterruptMask(scb_module[uart_n]) & ~CY_SCB_UART_TX_EMPTY);
    Cy_SCB_SetTxInterruptMask(scb_module[uart_n], Cy_SCB_GetTxInterruptMask(scb_module[uart_n]) & ~CY_SCB_UART_TX_NACK);
    Cy_SCB_SetTxInterruptMask(scb_module[uart_n], Cy_SCB_GetTxInterruptMask(scb_module[uart_n]) & ~CY_SCB_UART_TX_ARB_LOST);
    if(status)
    {
        Cy_SCB_SetTxInterruptMask(scb_module[uart_n], Cy_SCB_GetRxInterruptMask(scb_module[uart_n]) | CY_SCB_UART_TX_DONE);
    }
    else
    {
        Cy_SCB_SetTxInterruptMask(scb_module[uart_n], Cy_SCB_GetRxInterruptMask(scb_module[uart_n]) & ~CY_SCB_UART_TX_DONE);
    }	
}
//-------------------------------------------------------------------------------------------------------------------
// �������       ���ڽ����ж�����
// ����˵��       uart_n           ����ģ���
// ����˵��       status          1�����ж�   0���ر��ж�
// ���ز���       void
// ʹ��ʾ��       uart_rx_interrupt(UART_0, 1);                   // �򿪴���1�����ж�
// ��ע��Ϣ	      �����жϺͷ����ж�ֻ��ѡ��һ��������ͬʱ���ã�������õ�Ϊ��Ч
//-------------------------------------------------------------------------------------------------------------------
void uart_rx_interrupt (uart_index_enum uart_n, uint32 status)
{
    Cy_SCB_SetRxInterruptMask(scb_module[uart_n], Cy_SCB_GetRxInterruptMask(scb_module[uart_n]) & ~CY_SCB_UART_RX_TRIGGER);
    Cy_SCB_SetRxInterruptMask(scb_module[uart_n], Cy_SCB_GetRxInterruptMask(scb_module[uart_n]) & ~CY_SCB_UART_RX_FULL);
    Cy_SCB_SetRxInterruptMask(scb_module[uart_n], Cy_SCB_GetRxInterruptMask(scb_module[uart_n]) & ~CY_SCB_UART_RX_OVERFLOW);
    Cy_SCB_SetRxInterruptMask(scb_module[uart_n], Cy_SCB_GetRxInterruptMask(scb_module[uart_n]) & ~CY_SCB_UART_RX_ERR_FRAME);
    Cy_SCB_SetRxInterruptMask(scb_module[uart_n], Cy_SCB_GetRxInterruptMask(scb_module[uart_n]) & ~CY_SCB_UART_RX_ERR_PARITY);
    Cy_SCB_SetRxInterruptMask(scb_module[uart_n], Cy_SCB_GetRxInterruptMask(scb_module[uart_n]) & ~CY_SCB_UART_RX_BREAK_DETECT);
    if(status)
    {
        Cy_SCB_SetRxInterruptMask(scb_module[uart_n], Cy_SCB_GetRxInterruptMask(scb_module[uart_n]) | CY_SCB_UART_RX_NOT_EMPTY);
    }
    else
    {
        Cy_SCB_SetRxInterruptMask(scb_module[uart_n], Cy_SCB_GetRxInterruptMask(scb_module[uart_n]) & ~CY_SCB_UART_RX_NOT_EMPTY);
    }	
}

//-------------------------------------------------------------------------------------------------------------------
//  �������      SBUS���ڳ�ʼ��
//  ����˵��      uartn           SBUS����ģ���(UART_0,UART_1,UART_2,UART_3)
//  ����˵��      baud            SBUS���ڲ�����
//  ����˵��      tx_pin          SBUS���ڷ�������
//  ����˵��      rx_pin          SBUS���ڽ�������
//  ���ز���      void
//  ʹ��ʾ��      uart_sbus_init(UART_4, 100000, UART4_TX_P14_1, UART4_RX_P14_0);       // ��ʼ������4 ������100000 ��������ʹ�� 14_1 ��������ʹ�� P14_0
//  ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
void uart_sbus_init (uart_index_enum uart_n, uint32 baud, uart_tx_pin_enum tx_pin, uart_rx_pin_enum rx_pin)
{
    // ���ѣ����ںźͶ˿ڶ�����Ӧ��ô�ܳ�ʼ���أ�
	
    zf_assert((uint8)uart_n == (uint8)tx_pin ? 1 : 0);
    zf_assert((uint8)uart_n == (uint8)rx_pin ? 1 : 0);
	
    uart_config_struct          uart_pin_config                 = {0};
    cy_stc_gpio_pin_config_t    gpio_pin_config                 = {0};
    cy_stc_scb_uart_config_t    g_stc_uart_config               = {0};
    uint64_t                    targetFreq                      = 8 * baud;
    uint64_t                    sourceFreq_fp5                  = ((uint64_t)UART_FREQ << 5ull);
    uint32_t                    divSetting_fp5                  = (uint32_t)(sourceFreq_fp5 / targetFreq);
    
    get_uart_config(&uart_pin_config, tx_pin, rx_pin);
    
    gpio_pin_config.driveMode           = CY_GPIO_DM_HIGHZ;
    gpio_pin_config.hsiom               = uart_pin_config.rx_hsiom;
    Cy_GPIO_Pin_Init(uart_pin_config.rx_port, uart_pin_config.rx_pin, &gpio_pin_config);
    
    gpio_pin_config.driveMode           = CY_GPIO_DM_STRONG_IN_OFF;
    gpio_pin_config.hsiom               = uart_pin_config.rx_hsiom;
    Cy_GPIO_Pin_Init(uart_pin_config.tx_port, uart_pin_config.tx_pin, &gpio_pin_config);

    
    g_stc_uart_config.uartMode          = CY_SCB_UART_STANDARD;
    g_stc_uart_config.oversample        = 8;
    g_stc_uart_config.dataWidth         = 9;
    g_stc_uart_config.stopBits          = CY_SCB_UART_STOP_BITS_1;
    g_stc_uart_config.parity            = CY_SCB_UART_PARITY_EVEN;
    g_stc_uart_config.ctsPolarity       = CY_SCB_UART_ACTIVE_LOW;  
    g_stc_uart_config.rtsPolarity       = CY_SCB_UART_ACTIVE_LOW;
   
    Cy_SCB_UART_DeInit(scb_module[uart_n]);
    Cy_SCB_UART_Init(scb_module[uart_n], &g_stc_uart_config, &uart_context[uart_n]);
    Cy_SCB_UART_Enable(scb_module[uart_n]);  
    
    Cy_SysClk_PeriphAssignDivider(uart_pin_config.uart_pclk, CY_SYSCLK_DIV_24_5_BIT, (uint8)uart_n);
    Cy_SysClk_PeriphSetFracDivider(Cy_SysClk_GetClockGroup(uart_pin_config.uart_pclk), CY_SYSCLK_DIV_24_5_BIT, (uint8)uart_n, ((divSetting_fp5 & 0x1FFFFFE0ul) >> 5ul), (divSetting_fp5 & 0x0000001Ful));
    Cy_SysClk_PeriphEnableDivider(Cy_SysClk_GetClockGroup(uart_pin_config.uart_pclk), CY_SYSCLK_DIV_24_5_BIT, (uint8)uart_n);
    
    cy_stc_sysint_irq_t                 stc_sysint_irq_cfg_uart;
    stc_sysint_irq_cfg_uart.sysIntSrc = uart_pin_config.uart_irqn;
    stc_sysint_irq_cfg_uart.intIdx    = UART_USE_ISR;
    stc_sysint_irq_cfg_uart.isEnabled = true;
    Cy_SysInt_InitIRQ(&stc_sysint_irq_cfg_uart);
    Cy_SysInt_SetSystemIrqVector(stc_sysint_irq_cfg_uart.sysIntSrc, (cy_systemIntr_Handler)uart_isr_func[uart_n]);
    NVIC_EnableIRQ(stc_sysint_irq_cfg_uart.intIdx);
    
    uart_rx_interrupt(uart_n, 0);
    uart_tx_interrupt(uart_n, 0);
}

//-------------------------------------------------------------------------------------------------------------------
//  �������      ���ڳ�ʼ��
//  ����˵��      uartn           ����ģ���(UART_0,UART_1,UART_2,UART_3)
//  ����˵��      baud            ���ڲ�����
//  ����˵��      tx_pin          ���ڷ�������
//  ����˵��      rx_pin          ���ڽ�������
//  ���ز���      void
//  ʹ��ʾ��      uart_init(UART_0, 115200, UART0_TX_P00_1, UART0_RX_P00_0);       // ��ʼ������0 ������115200 ��������ʹ�� P0_1 ��������ʹ�� P0_0
//  ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
void uart_init (uart_index_enum uart_n, uint32 baud, uart_tx_pin_enum tx_pin, uart_rx_pin_enum rx_pin)
{
    // ���ѣ����ںźͶ˿ڶ�����Ӧ��ô�ܳ�ʼ���أ�
	
    zf_assert((uint8)uart_n == (uint8)tx_pin ? 1 : 0);
    zf_assert((uint8)uart_n == (uint8)rx_pin ? 1 : 0);
    
    uart_config_struct          uart_pin_config                 = {0};
    cy_stc_gpio_pin_config_t    gpio_pin_config                 = {0};
    cy_stc_scb_uart_config_t    g_stc_uart_config               = {0};
    uint64_t                    targetFreq                      = 8 * baud;
    uint64_t                    sourceFreq_fp5                  = ((uint64_t)UART_FREQ << 5ull);
    uint32_t                    divSetting_fp5                  = (uint32_t)(sourceFreq_fp5 / targetFreq);
    
    get_uart_config(&uart_pin_config, tx_pin, rx_pin);
    
    gpio_pin_config.driveMode           = CY_GPIO_DM_HIGHZ;
    gpio_pin_config.hsiom               = uart_pin_config.rx_hsiom;
    Cy_GPIO_Pin_Init(uart_pin_config.rx_port, uart_pin_config.rx_pin, &gpio_pin_config);
    
    gpio_pin_config.driveMode           = CY_GPIO_DM_STRONG_IN_OFF;
    gpio_pin_config.hsiom               = uart_pin_config.rx_hsiom;
    Cy_GPIO_Pin_Init(uart_pin_config.tx_port, uart_pin_config.tx_pin, &gpio_pin_config);

    
    g_stc_uart_config.uartMode          = CY_SCB_UART_STANDARD;
    g_stc_uart_config.oversample        = 8;
    g_stc_uart_config.dataWidth         = 8;
    g_stc_uart_config.stopBits          = CY_SCB_UART_STOP_BITS_1;
    g_stc_uart_config.parity            = CY_SCB_UART_PARITY_NONE;
    g_stc_uart_config.ctsPolarity       = CY_SCB_UART_ACTIVE_LOW;  
    g_stc_uart_config.rtsPolarity       = CY_SCB_UART_ACTIVE_LOW;
    
    Cy_SCB_UART_DeInit(scb_module[uart_n]);
    Cy_SCB_UART_Init(scb_module[uart_n], &g_stc_uart_config, &uart_context[uart_n]);
    Cy_SCB_UART_Enable(scb_module[uart_n]);  
    
    Cy_SysClk_PeriphAssignDivider(uart_pin_config.uart_pclk, CY_SYSCLK_DIV_24_5_BIT, (uint8)uart_n);
    Cy_SysClk_PeriphSetFracDivider(Cy_SysClk_GetClockGroup(uart_pin_config.uart_pclk), CY_SYSCLK_DIV_24_5_BIT, (uint8)uart_n, ((divSetting_fp5 & 0x1FFFFFE0ul) >> 5ul), (divSetting_fp5 & 0x0000001Ful));
    Cy_SysClk_PeriphEnableDivider(Cy_SysClk_GetClockGroup(uart_pin_config.uart_pclk), CY_SYSCLK_DIV_24_5_BIT, (uint8)uart_n);
    
    cy_stc_sysint_irq_t                 uart_irq_cfg;
    uart_irq_cfg.sysIntSrc = uart_pin_config.uart_irqn;
    uart_irq_cfg.intIdx    = UART_USE_ISR;
    uart_irq_cfg.isEnabled = true;
    interrupt_init(&uart_irq_cfg, uart_isr_func[uart_n], 7);
    
    uart_rx_interrupt(uart_n, 0);
    uart_tx_interrupt(uart_n, 0);
}










