/*
 * GL_Math.h
 *
 *  Created on: 2023年7月19日
 *      Author: ORRN
 */
#include "A_Master_Ctrl.h" //比较特殊,用以存放条件编译开启标志位

#ifndef CODE_GL_MATH_H_
#define CODE_GL_MATH_H_

#if GL_MATH_FLAG
/************************常用计算*******************************/

//一阶IIR低通滤波器，hz是截止频率
#define LPF_1_(hz,t,in,out) ((out) += ( 1 / ( 1 + 1 / ( (hz) *6.28f *(t) ) ) ) *( (in) - (out) ))

#define LIMIT( x,min,max ) ( ((x) <= (min)) ? (min) : ( ((x) > (max))? (max) : (x) ) )

#define ABS(x) ( (x)>0?(x):-(x) )

#define POW(a) ((a)*(a))

//求倒数，防止除0错误
#define safe_div(numerator,denominator,safe_value) ( (denominator == 0)? (safe_value) : ((numerator)/(denominator)) )

/************************常用计算END*******************************/
/************************常用变量*******************************/



/************************常用变量END*******************************/
double func_pow(double x,int k);
double func_sqrt(double x);
double fast_atan2(double y, double x);
double func_cos(double x);
double func_sin(double x);
double py_fabs(double x);
double py_asin(double x);
long factorial(int x);
double py_sqrt(double x);

#endif

#endif /* CODE_GL_MATH_H_ */
