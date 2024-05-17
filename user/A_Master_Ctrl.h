/*
 * A_Master_Ctrl.h
 *
 *  Created on: 2023年4月11日
 *      Author: ORRN
 */

#ifndef CODE_A_MASTER_CTRL_H_
#define CODE_A_MASTER_CTRL_H_

#define ZHX_MATH_FLAG      (0)
#define GL_MATH_FLAG       (1)

#define Main_FLAG          (0)          //主程序开启标志位

#define Float_FLAG         (0)          //单精度模式开启标志位(GPS数学库条件编译为单精度)
#define Double_FLAG        (0)          //双精度模式开启标志位(GPS数学库条件编译为双精度)
#define Int_FLAG_D         (0)          //整数模式开启标志位(GPS数学库条件编译为整数,内部计算函数采用double)
#define GL_FLAG            (1)

#define Float_Record_FLAG  (0)          //单精度浮点存储开启标志位//可用
#define Double_Record_FLAG (0)          //双精度浮点存储开启标志位//可用
#define Int_Record_FLAG    (1)          //整数存储开启标志位     //可用
#define Array_Record_FLAG  (1)          //数组存储标志位        //试用

#define ISR_Start_FLAG     (1)          //中断开启标志位
#define IMU_Restrain_FLAG  (0)          //直线抑制测试
#define IMU_STATR_FLAG     (1)          //IMU开启标志位(IMU初始化时间太长了)
#define FLASH_Init_FLAG    (0)          //FLASH数据初始化标志位

#define IMU_Type_FLAG      (1)          //IMU类型标志位

#define USART_Send_STOP_FLAG (0)        //开启华大北斗连接标志位


//条件编译
//为1,可以编译||为0,禁止编译

//开启标志位-说明：
//1.判断ADS是单精度模式还是双精度模式(单精度模式下：float和double都按照float定义)
//2.判断采用何种数学库:Float_FLAG-GPS采用单精度数学库(ADS自带库)||Double_FLAG-GPS采用双精度数学库(自定义库)||Int_FLAG_D-GPS采用整数形参并采用默认数学库
//3.判断才采用何种FLASH存储方式:Float_Record_FLAG-单精度||Double_Record_FLAG-双精度||Int_Record_FLAG-整数存储

//固定模式：
//Float_FLAG  (1)---------Float_Record_FLAG  (1)//单精度
//Double_FLAG (1)---------Double_Record_FLAG (1)//双精度
//Int_FLAG_D  (1)---------Int_Record_FLAG    (1)//整数

/*
舵机和电机在一个定时器--7ms

占空比         霍尔
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
