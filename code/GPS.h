/*
 * GPS.h
 *
 *  Created on: 2023年3月18日
 *      Author: ORRN
 */

#include "A_Master_Ctrl.h" //比较特殊,用以存放条件编译开启标志位

#ifndef CODE_GPS_H_
#define CODE_GPS_H_

#include "zf_device_gnss.h" // 添加gnss头文件

// 全局变量
#if Float_Record_FLAG

extern float Target_point[2][150]; // 用于储存采集的目标点信息，用于后续的位置计算(第一层是纬度，第二层是经度)
extern float Work_target_array[2][150]; // 用于将从flash中读取的目标点数组的数据复制过来，当这个数组被赋值时，后续的一切和Flash再无关系

#endif

#if Double_Record_FLAG

extern double Target_point[2][150]; // 用于储存采集的目标点信息，用于后续的位置计算(第一层是纬度，第二层是经度)
extern double Work_target_array[2][150]; // 用于将从flash中读取的目标点数组的数据复制过来，当这个数组被赋值时，后续的一切和Flash再无关系

#endif

#if Int_Record_FLAG

extern uint32 Target_point[2][150]; // 用于储存采集的目标点信息，用于后续的位置计算(第一层是纬度，第二层是经度)
extern uint32 Work_target_array[2][150]; // 用于将从flash中读取的目标点数组的数据复制过来，当这个数组被赋值时，后续的一切和Flash再无关系

#endif

#if Array_Record_FLAG

extern double GPS_GET_LAT[];
extern double GPS_GET_LOT[];

#endif

// 宏定义
#define RP_MAX 20 // 设置的采集点上限(自己决定)
#define Point_NUM 10 // 精度或纬度采集个数
#define FLASH_SECTION_INDEX 0 // 存储数据用的扇区
#define FLASH_PAGE_INDEX 11 // 存储数据用的页码 倒数第一个页码
#define GPS_PAGE_INDEX 11 // GPS存储页码

// 函数声明
void GPS_Record_flash(void); // 将采集的点位记录到缓冲区并储存至GPS_FLASH
void GPS_Flash_use(void); // 将GPS_FLASH的数据重新读回缓冲区并赋值给数组
void GPS_Work_array(void); // 用于隔离FLASH,实际参加计算的数组
void GPS_SHOW(void); // GPS信息显示
void GPS_param_t_init(void); // GPS参数初始化

#endif /* CODE_GPS_H_ */
