/*
 * Menu.c
 *
 *  Created on: 2023��3��18��
 *      Author: ORRN
 */
//��������˵�����д��˼·��ͬ��

/*
  ���˵�����˼·:���ȶ���һ���ṹ�����(�������Ϸ����·���ȷ������ʾ����(����ʾ������һ������ָ��void (*current_operation)();)
  Ȼ���øýṹ�����Ͷ���һ�����飬������Ϊ�ṹ�����飬ÿ��Ԫ�ض��ǽṹ�壬���Է��ʽṹ���Ա��(ֱ�������ǽ������ֵ������Ա)��ʵ���Ϸ��·���ȷ��
  ������������ʵ����Դ�ڽṹ���Ա(����ָ��)����ָ�����ḳֵ��һ��ָ�����������һϵ�б任������ʵ�ֺ�������
*/

//5.14����˵�̫���˰ɡ�����
//�һ���û���� Ҫ������ʼ���뿪ʼ Ȼ��ͨ��ʱ�������ԭ��һ����� 
//����˵��Ӧ�ÿ�������ÿһ����������Ŀǰ����declaration�����⣩

//������menu2 ��������ĺ�����ôд
//�ҵ������ڷ������棺
//����㷨����д����ʲô����������� ���Ǹ����Ե���������˵����

#include "zf_common_headfile.h"
#include "zf_device_gnss.h" // �滻ͷ�ļ�

extern gnss_info_struct gnss; // ʹ��gnss����

int GO_FLAG = 0;       // ������־λ
int STOP_MENU_FALG = 1;// �˵���־λ
int func_index = 0;    // ��ʼ��ʾ��ӭ����
int last_index = 127;  // last��ʼΪ��Чֵ

int SPEED = 0;            // SPEED����
int *SPEED_Pointer;       // SPEED����ָ��
int Stop_Point = 0;       // Stop_Point����
int *Stop_Point_Pointer;  // Stop_Point����ָ��
int Point_distance = 0;   // Point_distance����
int *Point_distance_Pointer;// Point_distance����ָ��

void (*current_operation_index)(void); // ��ʾ��������ָ��(��ǰ��������)

//ֻ�и����ÿ�������ʵ�ֵ��Ҳ����޸İ�
//������ ����˼���㷨��{��־��up��down��enter}
//���ڸ��ؼ����ǣ����˲˵�Ȼ���أ�һ���Ժ�������˵���ǵ�����
/*
��ǰ���� func_index Ϊ 1����Ӧ table_dispaly[1]��
��ǰ����ָ�� current_operation_index ָ�� fun_a1
*/
/*
a2-GPS_SET
b2-��������
c2-�������ñ���������������ԡ��������
����˵ʵ������д�е��ѸĶ����ر���������ͬһ��һ����嶫���������޶�Ϊ��������Ȼ�һ��ô��
�������ȷʵ���������鷳һ��
��������GPT��

һЩ����ע�ͣ�Com_display; // ��ʾ��ͨ����ʾ��
*/
//�������ȷʵ���������鷳һ��

key_table table_dispaly[30] = // �ṹ������
{
    // ��0��
    {0, 0, 0, 1, (*fun_0)}, // AIIT_meun

    // ��1��
    {1, 4, 2, 5, (*fun_a1)}, // GPS
    {2, 1, 3, 9, (*fun_b1)}, // PID
    {3, 2, 4, 13, (*fun_c1)}, // SPEED
    {4, 3, 1, 0, (*fun_d1)}, // ESC

    // ��2��
    {5, 8, 6, 17, (*fun_a21)},
    {6, 5, 7, 18, (*fun_a22)},
    {7, 6, 8, 19, (*fun_a23)},
    {8, 7, 5, 1, (*fun_a24)},

    {9, 12, 10, 20, (*fun_b21)},
    {10, 9, 11, 21, (*fun_b22)},
    {11, 10, 12, 22, (*fun_b23)},
    {12, 11, 9, 2, (*fun_b24)},

    {13, 16, 14, 23, (*fun_c21)},
    {14, 13, 15, 24, (*fun_c22)},
    {15, 14, 16, 25, (*fun_c23)},
    {16, 15, 13, 3, (*fun_c24)},

    // ��3��
    //���£���������������˵���
    //Ŷ���ԣ���24ָ���Ƕ����˵�����λ
    {17, 17, 17, 5, (*fun_a31)},
    {18, 18, 18, 6, (*fun_a32)},
    {19, 19, 19, 7, (*fun_a33)},

    {20, 20, 20, 9, (*fun_b31)},
    {21, 21, 21, 10, (*fun_b32)},
    {22, 22, 22, 11, (*fun_b33)},

    {23, 23, 23, 13, (*fun_c31)},
    {24, 24, 24, 14, (*fun_c32)},
    {25, 25, 25, 15, (*fun_c33)},
};

void Menu() // �˵�����
{
    key_scan();

    if (key1_flag) {
        key1_flag = 0;
        func_index = table_dispaly[func_index].up;    // ���Ϸ�
    }
    if (key2_flag) {
        key2_flag = 0;
        func_index = table_dispaly[func_index].down;    // ���·�
    }
    if (key3_flag) {
        key3_flag = 0;
        func_index = table_dispaly[func_index].enter;    // ȷ��
    }

    if (func_index != last_index) {
        current_operation_index = table_dispaly[func_index].current_operation;

        ips_clear();
        (*current_operation_index)(); // ִ�е�ǰ��������
        last_index = func_index;
    }
}

int Menu_key_Operation(int *param_t) // �������ڽ����������//ָ�������Ϊ�βΣ��Ǻ����ڵ��β�ҲҪΪָ����ʽ
{
  //key1_flag�ȴ��ݵ�key_val
  //*param_t��ָ�룬����ָʲô������
    if (key1_flag) { key1_flag = 0; key_val = 1; }
    if (key2_flag) { key2_flag = 0; key_val = 2; }
    if (key3_flag) { key3_flag = 0; key_val = 3; }
    if (key4_flag) { key4_flag = 0; key_val = 4; }

    switch (key_val) {
        case 1: *param_t += 10, key_val = 0; break;
        case 2: *param_t -= 10, key_val = 0; break;
        case 3: *param_t += 1, key_val = 0; break;
        case 4: *param_t -= 1, key_val = 0; break;
        default: break;
    }

    ips_show_int(120, 16 * 3, *param_t, 3);

    return *param_t;
}

//������������˵�������
void Menu_flash_clear(int PAGE_INDEX) // ���FLASH
{
    flash_erase_page(FLASH_SECTION_INDEX, PAGE_INDEX);
    ips_show_string(0, 16 * 0, "Clear FLASH OK!");

    if (PAGE_INDEX == 10) {
        ips_show_string(0, 16 * 1, "SPEED_PAGE_INDEX(10)-->");
    } else if (PAGE_INDEX == 8) {
        ips_show_string(0, 16 * 1, "Stop_Point_PAGE_INDEX(8)-->");
    } else if (PAGE_INDEX == 7) {
        ips_show_string(0, 16 * 1, "Point_Dis_PAGE_INDEX(7)-->");
    } else {
    }

    printf("\r\n�Ѳ���ָ��ҳFLASH:%d", PAGE_INDEX);
}

void FLAG_param_t_init() // FLAG������ʼ��
{
    if (flash_check(FLASH_SECTION_INDEX, FLAG_PAGE_INDEX)) { // �ж�Flash�Ƿ������� : �����ݷ���1�������ݷ���0

        flash_buffer_clear(); // ��ջ�����

        flash_read_page_to_buffer(FLASH_SECTION_INDEX, FLAG_PAGE_INDEX); // �����ݴ�FLASHָ������ҳ����뵽������
        GO_FLAG = flash_union_buffer[0].int32_type; // ��ʲô��������д�뻺��������ʲô���Ͷ�ȡ����Ȼ��ӡ��������������
        STOP_MENU_FALG = flash_union_buffer[1].int32_type;
        flash_erase_page(FLASH_SECTION_INDEX, FLAG_PAGE_INDEX); // ÿ���޸�ֵ����λ�󣬽�FLASH�е�ֵ����������Ȼ�����FLASH�е�ֵ(�Զ�����)

        printf("\r\n�Ѿ����¹���(GO_FLAG):%d", GO_FLAG);
        printf("\r\n�Ѿ����¹���(STOP_MENU_FALG):%d", STOP_MENU_FALG);
    } else {
        printf("\r\nFLAGΪ�����趨ֵ(ԭʼֵ)");
    }
}

void Stop_point_param_t_init() // Stop_point������ʼ��
{
    if (flash_check(FLASH_SECTION_INDEX, Stop_Point_PAGE_INDEX)) { // �ж�Flash�Ƿ������� : �����ݷ���1�������ݷ���0

        flash_buffer_clear(); // ��ջ�����

        flash_read_page_to_buffer(FLASH_SECTION_INDEX, Stop_Point_PAGE_INDEX); // �����ݴ�FLASHָ������ҳ����뵽������
        S_Point = flash_union_buffer[0].int32_type; // ��ʲô��������д�뻺��������ʲô���Ͷ�ȡ����Ȼ��ӡ��������������
        printf("\r\n�Ѿ����¹���(Stop_Point):%d", S_Point);
    } else {
        printf("\r\nStop_PointΪ�����趨ֵ(ԭʼֵ)");
    }
}

void Point_distance_param_t_init() // Point_distance������ʼ��
{
    if (flash_check(FLASH_SECTION_INDEX, Point_distance_PAGE_INDEX)) { // �ж�Flash�Ƿ������� : �����ݷ���1�������ݷ���0

        flash_buffer_clear(); // ��ջ�����

        flash_read_page_to_buffer(FLASH_SECTION_INDEX, Point_distance_PAGE_INDEX); // �����ݴ�FLASHָ������ҳ����뵽������
        P_Distance = flash_union_buffer[0].int32_type; // ��ʲô��������д�뻺��������ʲô���Ͷ�ȡ����Ȼ��ӡ��������������
        printf("\r\n�Ѿ����¹���(Point_Distance):%d", P_Distance);
    } else {
        printf("\r\nPoint_DistanceΪ�����趨ֵ(ԭʼֵ)");
    }
}

/*********��0��***********/
void fun_0()
{
  //ϲ���� ��MOSSŶ
    ips_show_rgb565_image(0, 0, (const uint16 *)gImage_MOSS, 240, 125, 240, 125, 0); // isp114
}

/*********��1��***********/
void fun_a1()
{
    ips_show_string(0, 16 * 1, "->");
    ips_show_string(20, 16 * 1, "GPS_SET");
    ips_show_string(20, 16 * 3, "Parameter_SET");
    ips_show_string(20, 16 * 5, "Other_SET");
    ips_show_string(20, 16 * 7, "ESC");
}

void fun_b1()
{
    ips_show_string(0, 16 * 3, "->");
    ips_show_string(20, 16 * 1, "GPS_SET");
    ips_show_string(20, 16 * 3, "Parameter_SET");
    ips_show_string(20, 16 * 5, "Other_SET");
    ips_show_string(20, 16 * 7, "ESC");
}

void fun_c1()
{
    ips_show_string(0, 16 * 5, "->");
    ips_show_string(20, 16 * 1, "GPS_SET");
    ips_show_string(20, 16 * 3, "Parameter_SET");
    ips_show_string(20, 16 * 5, "Other_SET");
    ips_show_string(20, 16 * 7, "ESC");
}

void fun_d1()
{
    ips_show_string(0, 16 * 7, "->");
    ips_show_string(20, 16 * 1, "GPS_SET");
    ips_show_string(20, 16 * 3, "Parameter_SET");
    ips_show_string(20, 16 * 5, "Other_SET");
    ips_show_string(20, 16 * 7, "ESC");
}

/*********��2��***********/
void fun_a21() // GPS_SET
{
    ips_show_string(0, 16 * 1, "->");
    ips_show_string(20, 16 * 1, "GPS storage point");
    ips_show_string(20, 16 * 3, "Com_display");
    ips_show_string(20, 16 * 5, "CLEAR_GPS_FLASH");
    ips_show_string(20, 16 * 7, "ESC");
}

void fun_a22()
{
    ips_show_string(0, 16 * 3, "->");
    ips_show_string(20, 16 * 1, "GPS storage point");
    ips_show_string(20, 16 * 3, "Com_display");
    ips_show_string(20, 16 * 5, "CLEAR_GPS_FLASH");
    ips_show_string(20, 16 * 7, "ESC");
}

void fun_a23()
{
    ips_show_string(0, 16 * 5, "->");
    ips_show_string(20, 16 * 1, "GPS storage point");
    ips_show_string(20, 16 * 3, "Com_display");
    ips_show_string(20, 16 * 5, "CLEAR_GPS_FLASH");
    ips_show_string(20, 16 * 7, "ESC");
}

void fun_a24()
{
    ips_show_string(0, 16 * 7, "->");
    ips_show_string(20, 16 * 1, "GPS storage point");
    ips_show_string(20, 16 * 3, "Com_display");
    ips_show_string(20, 16 * 5, "CLEAR_GPS_FLASH");
    ips_show_string(20, 16 * 7, "ESC");
}

void fun_b21() // Parameter_SET
{
    ips_show_string(0, 16 * 1, "->");
    ips_show_string(20, 16 * 1, "Speed_regulate");
    ips_show_string(20, 16 * 3, "Stop_point/Point_distance");
    ips_show_string(20, 16 * 5, "CLEAR_PAT_FLASH");
    ips_show_string(20, 16 * 7, "ESC");
}

void fun_b22()
{
    ips_show_string(0, 16 * 3, "->");
    ips_show_string(20, 16 * 1, "Speed_regulate");
    ips_show_string(20, 16 * 3, "Stop_point/Point_distance");
    ips_show_string(20, 16 * 5, "CLEAR_PAT_FLASH");
    ips_show_string(20, 16 * 7, "ESC");
}

void fun_b23()
{
    ips_show_string(0, 16 * 5, "->");
    ips_show_string(20, 16 * 1, "Speed_regulate");
    ips_show_string(20, 16 * 3, "Stop_point/Point_distance");
    ips_show_string(20, 16 * 5, "CLEAR_PAT_FLASH");
    ips_show_string(20, 16 * 7, "ESC");
}

void fun_b24()
{
    ips_show_string(0, 16 * 7, "->");
    ips_show_string(20, 16 * 1, "Speed_regulate");
    ips_show_string(20, 16 * 3, "Stop_point/Point_distance");
    ips_show_string(20, 16 * 5, "CLEAR_PAT_FLASH");
    ips_show_string(20, 16 * 7, "ESC");
}

void fun_c21() // Other_SET
{
    ips_show_string(0, 16 * 1, "->");
    ips_show_string(20, 16 * 1, "START_CAR");
    ips_show_string(20, 16 * 3, "Steer_TEXT");
    ips_show_string(20, 16 * 5, "Motor_TEXT");
    ips_show_string(20, 16 * 7, "ESC");
}

void fun_c22()
{
    ips_show_string(0, 16 * 3, "->");
    ips_show_string(20, 16 * 1, "START_CAR");
    ips_show_string(20, 16 * 3, "Steer_TEXT");
    ips_show_string(20, 16 * 5, "Motor_TEXT");
    ips_show_string(20, 16 * 7, "ESC");
}

void fun_c23()
{
    ips_show_string(0, 16 * 5, "->");
    ips_show_string(20, 16 * 1, "START_CAR");
    ips_show_string(20, 16 * 3, "Steer_TEXT");
    ips_show_string(20, 16 * 5, "Motor_TEXT");
    ips_show_string(20, 16 * 7, "ESC");
}

void fun_c24()
{
    ips_show_string(0, 16 * 7, "->");
    ips_show_string(20, 16 * 1, "START_CAR");
    ips_show_string(20, 16 * 3, "Steer_TEXT");
    ips_show_string(20, 16 * 5, "Motor_TEXT");
    ips_show_string(20, 16 * 7, "ESC");
}

//////////////////////////////////////////////////////////////////////////////////////////////////-----------GPS_SET
// 5.14 22:35 �����ڻ����Ը�ʲô��

void fun_a31() // GPS��λ�ɼ��������ӦFLASH
{
    if (gpio_get_level(SWITCH1)) {
        while (gpio_get_level(SWITCH1)) {
            key_scan();
            if (key4_flag) {
                key4_flag = 0;
                GPS_Record_flash();
            }
            ips_show_string(0, 16 * 3, "T:"); // ����ʵʱ��λ
            ips_show_float(50, 16 * 4, gnss.latitude, 3, 6);
            ips_show_float(50, 16 * 5, gnss.longitude, 3, 6);
        }
    } else {
        ips_show_string(0, 16 * 1, "Function not executing"); // ��ִ�к���δִ��
    }
}

void fun_a32() // �ۺ���Ϣ��ʾ
{
    if (gpio_get_level(SWITCH1)) {
        while (gpio_get_level(SWITCH1)) {
            GPS_SHOW(); // ��Ϣ��ʾ
        }
    }
}

void fun_a33() // ���
{
    flash_buffer_clear(); // ��ջ�����(��֤��һ���ɾ��Ļ�����)------Warning
    if (flash_check(FLASH_SECTION_INDEX, GPS_PAGE_INDEX)) { // �ж�Flash�Ƿ������� : �����ݷ���1�������ݷ���0
        Menu_flash_clear(GPS_PAGE_INDEX); // ���GPS����FLASH
    } else {
        ips_show_string(0, 16 * 0, "FLASH is blank !");
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////-----------Parameter_SET
void fun_b31() // Speed_regulate
{
    if (gpio_get_level(SWITCH1)) { // Speed_regulate����
        while (gpio_get_level(SWITCH1)) {
            SPEED_Pointer = &(SPEED); // ֱ�ӽ�Ҫ�����ı���ȡ��ַ����ֵ��ָ�����

            ips_show_string(0, 16 * 1, "Function executing"); // ��ִ�к���ִ��
            ips_show_string(0, 16 * 2, "Speed_regulate");
            ips_show_string(0, 16 * 3, "Speed");
            key_scan();
            Menu_key_Operation(SPEED_Pointer); // ֱ�Ӳ�����ַ���������β�Ϊ��̬�������Զ��ͷ��ڴ������

            if (gpio_get_level(SWITCH2)) { // �����뿪��Ϊ1������£��Ҳ��뿪��Ϊ1���뱣�����ģʽ
                flash_union_buffer[0].int32_type = Menu_key_Operation(SPEED_Pointer) * 100; // �����ٶ���˵,10��1��̫С�ˣ�����Ŵ�100��
                flash_write_page_from_buffer(FLASH_SECTION_INDEX, SPEED_PAGE_INDEX); // ��������������д�뵽ָ��Flash ������ҳ��
                printf("\r\n����������SPEED:%d", flash_union_buffer[0].int32_type);
                ips_show_string(0, 16 * 4, "Record param_t OK!");
            }
        }
    }
}

void fun_b32() // Stop_point/Point_distance
{
    if (gpio_get_level(SWITCH1) && !gpio_get_level(SWITCH2)) { // Stop_point����
        while (gpio_get_level(SWITCH1)) {
            Stop_Point_Pointer = &(Stop_Point);
            ips_show_string(0, 16 * 1, "Function executing"); // ��ִ�к���ִ��
            ips_show_string(0, 16 * 2, "Stop_Point");
            ips_show_string(0, 16 * 3, "S_Point");

            key_scan();
            Menu_key_Operation(Stop_Point_Pointer); // ֱ�Ӳ�����ַ���������β�Ϊ��̬�������Զ��ͷ��ڴ������
            if (gpio_get_level(SWITCH2)) { // �����뿪��Ϊ1������£��Ҳ��뿪��Ϊ1���뱣�����ģʽ
                flash_buffer_clear(); // ��ջ�����

                flash_union_buffer[0].int32_type = Menu_key_Operation(Stop_Point_Pointer);
                flash_write_page_from_buffer(FLASH_SECTION_INDEX, Stop_Point_PAGE_INDEX); // ��������������д�뵽ָ��Flash ������ҳ��
                printf("\r\n����������Stop_Point:%d", flash_union_buffer[0].int32_type);
                ips_show_string(0, 16 * 4, "Record param_t OK!");
            }
        }
    }

    if (gpio_get_level(SWITCH2) && !gpio_get_level(SWITCH1)) { // Point_distance����
        while (gpio_get_level(SWITCH2)) {
            Point_distance_Pointer = &(Point_distance);
            ips_show_string(0, 16 * 1, "Function executing"); // ��ִ�к���ִ��
            ips_show_string(0, 16 * 2, "Point_distance");
            ips_show_string(0, 16 * 3, "P_distance");

            key_scan();
            Menu_key_Operation(Point_distance_Pointer); // ֱ�Ӳ�����ַ���������β�Ϊ��̬�������Զ��ͷ��ڴ������

            if (gpio_get_level(SWITCH1)) { // ���Ҳ��뿪��Ϊ1������£����뿪��Ϊ1���뱣�����ģʽ
                flash_buffer_clear(); // ��ջ�����

                flash_union_buffer[0].int32_type = Menu_key_Operation(Point_distance_Pointer);
                flash_write_page_from_buffer(FLASH_SECTION_INDEX, Point_distance_PAGE_INDEX); // ��������������д�뵽ָ��Flash ������ҳ��
                printf("\r\n����������Point_distance:%d", flash_union_buffer[0].int32_type);
                ips_show_string(0, 16 * 4, "Record param_t OK!");
            }
        }
    }
}

void fun_b33() // ���
{
    if (gpio_get_level(SWITCH1) && !gpio_get_level(SWITCH2)) {
        flash_buffer_clear(); // ��ջ�����(��֤��һ���ɾ��Ļ�����)------Warning
        Menu_flash_clear(SPEED_PAGE_INDEX); // ���SPEED����FLASH
    }

    if (gpio_get_level(SWITCH1) && gpio_get_level(SWITCH2)) {
        flash_buffer_clear(); // ��ջ�����(��֤��һ���ɾ��Ļ�����)------Warning
        Menu_flash_clear(Stop_Point_PAGE_INDEX); // ���Stop_Point����FLASH
    }

    if (!gpio_get_level(SWITCH1) && gpio_get_level(SWITCH2)) {
        flash_buffer_clear(); // ��ջ�����(��֤��һ���ɾ��Ļ�����)------Warning
        Menu_flash_clear(Point_distance_PAGE_INDEX); // ���Point_distance����FLASH
    }
}

void fun_c31() // START_CAR����
{
    if (gpio_get_level(SWITCH2)) { // START_CAR����
        while (gpio_get_level(SWITCH2)) {
            GO_FLAG = 1; // �򿪷�����־λ
            ips_show_string(0, 16 * 0, "GO_FLAG"); ips_show_int(120, 0, GO_FLAG, 3);
            printf("\r\nGO_FLAG:%d", GO_FLAG);

            STOP_MENU_FALG = 0; // �رղ˵���־λ
            ips_show_string(0, 16 * 1, "STOP_MENU"); ips_show_int(120, 16 * 1, STOP_MENU_FALG, 3);
            printf("\r\nSTOP_MENU_FALG:%d", STOP_MENU_FALG);

            ips_show_string(0, 16 * 3, "GO GO GO !!!");
            ips_show_float(0, 16 * 5, gnss.latitude, 3, 6);
            ips_show_float(0, 16 * 6, gnss.longitude, 3, 6);
            ips_show_string(0, 16 * 7, "satellite:"); ips_show_uint(100, 16 * 7, gnss.satellite_used, 5);

            flash_buffer_clear(); // ��ջ�����

            flash_union_buffer[0].int32_type = GO_FLAG; // ��־λ���뻺����
            flash_union_buffer[1].int32_type = STOP_MENU_FALG; // ��־λ���뻺����
            flash_write_page_from_buffer(FLASH_SECTION_INDEX, FLAG_PAGE_INDEX); // ��������������д�뵽ָ��Flash ������ҳ��
            printf("\r\nGO_FLAG:%d", flash_union_buffer[0].int32_type);
            printf("\r\nSTOP_MENU_FALG:%d", flash_union_buffer[1].int32_type);
        }
    }
}

void fun_c32() // ������Ե���
{
    while (gpio_get_level(SWITCH1)) {
        Control_FLAG = 0; // ����GPS�Զ����Ӱ��
        Steer_text(); // �������
    }
}

void fun_c33() // ������Ե���
{
    while (gpio_get_level(SWITCH1)) {
        Motor_text(); // �������
    }
}
