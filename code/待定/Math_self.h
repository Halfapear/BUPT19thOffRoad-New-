/*
 * Math_self.h
 *
 *  Created on: 2023年4月10日
 *      Author: ORRN
 */
#include "A_Master_Ctrl.h" //比较特殊,用以存放条件编译开启标志位
#ifndef MATH_SELF_H_
#define MATH_SELF_H_

#if ZHX_MATH_FLAG
//函数声明
double func_pow(double x,int k);//自定义pow函数(功能为计算x的K次幂，返回幂指数的结果)
double func_sqrt(double x);//自定义sqrt函数(平方根运算)
double func_cos(double x);//自定义cos函数
double func_sin(double x);//自定义sin函数

#endif


#endif /* MATH_SELF_H_ */
