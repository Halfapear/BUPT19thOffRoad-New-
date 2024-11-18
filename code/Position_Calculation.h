/*
 * Position_Calculation.h
 *
 *  Created on: 2023年3月18日
 *      Author: ORRN
 */

#ifndef CODE_POSITION_CALCULATION_H_
#define CODE_POSITION_CALCULATION_H_

//全局变量声明
extern int S_Point;
extern int P_Distance;
extern int Control_FLAG;         //该标志位意义是，当它为1时是无控模式(靠GPS+IMU导航)/当手柄上的按键按下时，该标志位被清0,无法执行无控模式，转入有控模式
extern double Error;
extern int   STEER_Value;
extern int16 SPEED_Value;
extern double azimuth;
extern int next_point;

extern double start_distance;//计算起始点和当前点的距离


extern int Start_GPSangle_Flag;//跑短直线以获取GPS航向角，为了后续与IMU同步


//函数声明
void Follow_Track(void);//核心循迹程序
void Task_RZT(int Steer,int16 Motor,int Angle,char Type);//任务:绕锥桶一圈
void Task_STOP(int point);           //任务:设置第n个点停车
void Task_H_speed(int16 H_speed);    //直道高速行驶
void Task_L_speed(int16 L_speed);    //特殊路段低速行驶
void Path_judgment(void);            //通过点位来判断是处于哪个元素
void Interval_judgment(void);        //点位区间判断，返回区间值
void Switching_point_AD(void);       //切点角度距离混合判断
void Switching_point_D(void);        //切点距离判断
void Double_layer_feedback_0(void);    //双层反馈
void Double_layer_feedback_180(void);    //双层反馈
void Single_layer_feedback(void);    //单层反馈
void IMU_Navigation_0(void);
void IMU_Navigation_180_N(void);//逆时针180度IMU导航
void IMU_Navigation_180_S(void);//顺时针180度IMU导航
void Retardation(int X,int16 Y,int16 Z);//反推制动
void STOP(void);//停车
void Navigation_mode(int TYPE);//导航模式选择

#endif /* CODE_POSITION_CALCULATION_H_ */
