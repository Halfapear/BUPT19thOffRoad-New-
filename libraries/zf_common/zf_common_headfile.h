
#ifndef _zf_common_headfile_h_
#define _zf_common_headfile_h_



//===================================================C���� ������===================================================
#include "math.h"
#include "stdio.h"
#include "stdint.h"
#include "stdbool.h"
#include "string.h"

#include "stdarg.h"//�Լ���ӵ�
//===================================================C���� ������===================================================

//===================================================оƬ SDK �ײ�===================================================
#include "cy_project.h"
#include "cy_device_headers.h"
#include "arm_math.h"
//===================================================оƬ SDK �ײ�===================================================

//====================================================��Դ�⹫����====================================================
#include "zf_common_typedef.h"
#include "zf_common_clock.h"
#include "zf_common_debug.h"
#include "zf_common_fifo.h"
#include "zf_common_font.h"
#include "zf_common_function.h"
#include "zf_common_interrupt.h"
//====================================================��Դ�⹫����====================================================

//===================================================оƬ����������===================================================
#include "zf_driver_adc.h"
#include "zf_driver_delay.h"
#include "zf_driver_dma.h"
#include "zf_driver_encoder.h"
#include "zf_driver_exti.h"
#include "zf_driver_flash.h"
#include "zf_driver_gpio.h"
#include "zf_driver_pit.h"
#include "zf_driver_pwm.h"
#include "zf_driver_soft_iic.h"
#include "zf_driver_spi.h"
#include "zf_driver_soft_spi.h"
#include "zf_driver_uart.h"
#include "zf_driver_timer.h"
//===================================================оƬ����������===================================================

//===================================================����豸������===================================================
#include "zf_device_absolute_encoder.h"
#include "zf_device_bluetooth_ch9141.h"
#include "zf_device_gnss.h"
//#include "zf_device_gps_tau1201.h"
//#include "zf_device_camera.h"
#include "zf_device_dl1a.h"
#include "zf_device_icm20602.h"
#include "zf_device_imu660ra.h"
#include "zf_device_imu963ra.h"
#include "zf_device_ips114.h"
#include "zf_device_ips200.h"
#include "zf_device_key.h"
#include "zf_device_mpu6050.h"
//#include "zf_device_mt9v03x.h"
#include "zf_device_oled.h"
//#include "zf_device_ov7725.h"
//#include "zf_device_scc8660.h"
#include "zf_device_tft180.h"
#include "zf_device_tsl1401.h"
#include "zf_device_type.h"
#include "zf_device_virtual_oscilloscope.h"
#include "zf_device_wifi_uart.h"
#include "zf_device_wireless_uart.h"
//===================================================����豸������===================================================

//===================================================�Լ����ļ�===================================================

#include "Common_Peripherals.h"
#include "Position_Calculation.h"
#include "Algorithm.h"
#include "GPS.h"
#include "IMU.h"
#include "Menu.h"
#include "Math_self.h"
#include "A_Master_Ctrl.h" //�Ƚ�����,���Դ���������뿪����־λ
#include "GL_CRC.h"
#include "GL_Math.h"



#endif

