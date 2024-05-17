/*
 * Menu.h
 *
 *  Created on: 2023��3��18��
 *      Author: ORRN
 */

#ifndef CODE_MENU_H_
#define CODE_MENU_H_

//ȫ�ֱ�������
extern int  SPEED;           //SPEED����
extern int GO_FLAG;
extern int STOP_MENU_FALG;

//�궨��
#define FLAG_PAGE_INDEX   (9)                //FLAG          �洢ҳ��
#define Stop_Point_PAGE_INDEX   (8)          //Stop_Point    �洢ҳ��
#define Point_distance_PAGE_INDEX (7)        //Point_distance�洢ҳ��

//��������
void  Menu(void);//�˵�����
void  Menu_flash_clear(int PAGE_INDEX);//���FLASH
void  FLAG_param_t_init(void);          //FLAG������ʼ��
void  Stop_point_param_t_init(void);    //Stop_point������ʼ��
void  Point_distance_param_t_init(void);//Point_distance������ʼ��
int   Menu_key_Operation(int *param_t );//�������ڽ����������


//�ṹ������
typedef struct
{
    int current;
    int up;//���Ϸ�������
    int down;//���·�������
    int enter;//ȷ��������
    void (*current_operation)();//��ǰҳ���������Ҫִ�е���ʾ����������һ������ָ��
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
