/*
 * Menu.h
 *
 *  Created on: 2023年3月18日
 *      Author: ORRN
 */

#ifndef CODE_MENU_H_
#define CODE_MENU_H_

//全局变量定义
extern int  SPEED;           //SPEED参数
extern int GO_FLAG;
extern int STOP_MENU_FALG;

//宏定义
#define FLAG_PAGE_INDEX   (9)                //FLAG          存储页码
#define Stop_Point_PAGE_INDEX   (8)          //Stop_Point    存储页码
#define Point_distance_PAGE_INDEX (7)        //Point_distance存储页码

//函数声明
void  Menu(void);//菜单函数
void  Menu_flash_clear(int PAGE_INDEX);//清除FLASH
void  FLAG_param_t_init(void);          //FLAG参数初始化
void  Stop_point_param_t_init(void);    //Stop_point参数初始化
void  Point_distance_param_t_init(void);//Point_distance参数初始化
int   Menu_key_Operation(int *param_t );//按键调节界面操作函数


//结构体声明
typedef struct
{
    int current;
    int up;//向上翻索引号
    int down;//向下翻索引号
    int enter;//确认索引号
    void (*current_operation)();//当前页面的索引号要执行的显示函数，这是一个函数指针
} key_table;


extern key_table table[30];

extern void fun_a1();
extern void fun_b1();
extern void fun_c1();
extern void fun_d1();
extern void fun_E1();

extern void fun_a21();
extern void fun_a22();
extern void fun_a23();
extern void fun_a24();

extern void fun_b21();
extern void fun_b22();
extern void fun_b23();
extern void fun_b24();

extern void fun_c21();
extern void fun_c22();
extern void fun_c23();
extern void fun_c24();

extern void fun_a31();
extern void fun_a32();
extern void fun_a33();

extern void fun_b31();
extern void fun_b32();
extern void fun_b33();

extern void fun_c31();
extern void fun_c32();
extern void fun_c33();

extern void fun_0();










#endif /* CODE_MENU_H_ */
