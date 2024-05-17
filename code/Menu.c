/*
 * Menu.c
 *
 *  Created on: 2023年3月18日
 *      Author: ORRN
 */
//看来这个菜单和我写的思路不同呢

/*
  本菜单代码思路:首先定义一个结构体包含(索引，上翻，下翻，确定，显示内容(该显示内容是一个函数指针void (*current_operation)();)
  然后用该结构体类型定义一个数组，该数组为结构体数组，每个元素都是结构体，可以访问结构体成员，(直观来看是将数组的值赋给成员)，实现上翻下翻和确认
  而操作函数的实现来源于结构体成员(函数指针)，该指针最后会赋值给一个指针变量，经过一系列变换后，最终实现函数操作
*/

//5.14这个菜单太难了吧。。。
//我还是没看懂 要不你从最开始进入开始 然后通过时间操作和原理一起解释 
//按理说我应该看懂里面每一个函数（但目前好像declaration有问题）

//继续看menu2 搞清楚他的函数怎么写
//我的任务在飞书里面：
//这个算法和我写的有什么区别？晚点再想 我那个可以叠三层吗按理说可以

#include "zf_common_headfile.h"
#include "zf_device_gnss.h" // 替换头文件

extern gnss_info_struct gnss; // 使用gnss变量

int GO_FLAG = 0;       // 发车标志位
int STOP_MENU_FALG = 1;// 菜单标志位
int func_index = 0;    // 初始显示欢迎界面
int last_index = 127;  // last初始为无效值

int SPEED = 0;            // SPEED参数
int *SPEED_Pointer;       // SPEED参数指针
int Stop_Point = 0;       // Stop_Point参数
int *Stop_Point_Pointer;  // Stop_Point参数指针
int Point_distance = 0;   // Point_distance参数
int *Point_distance_Pointer;// Point_distance参数指针

void (*current_operation_index)(void); // 显示函数索引指针(当前操作索引)

//只有搞清楚每个是如何实现的我才能修改啊
//看懂了 有意思的算法：{标志，up，down，enter}
//现在更关键的是：出了菜单然后呢，一次性函数还好说，那调参呢
/*
当前索引 func_index 为 1，对应 table_dispaly[1]。
当前函数指针 current_operation_index 指向 fun_a1
*/
/*
a2-GPS_SET
b2-参数设置
c2-其他设置比如启动、舵机测试、电机测试
不过说实话这样写有点难改动，特别是我想在同一类一二层插东西，而且限定为三个，不然我还得大改
添加起来确实比我那种麻烦一点
但可以让GPT干

一些翻译注释：Com_display; // 显示“通信显示”
*/
//添加起来确实比我那种麻烦一点

key_table table_dispaly[30] = // 结构体数组
{
    // 第0层
    {0, 0, 0, 1, (*fun_0)}, // AIIT_meun

    // 第1层
    {1, 4, 2, 5, (*fun_a1)}, // GPS
    {2, 1, 3, 9, (*fun_b1)}, // PID
    {3, 2, 4, 13, (*fun_c1)}, // SPEED
    {4, 3, 1, 0, (*fun_d1)}, // ESC

    // 第2层
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

    // 第3层
    //可怕，第三层就是三级菜单了
    //哦不对，像24指的是二级菜单第四位
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

void Menu() // 菜单函数
{
    key_scan();

    if (key1_flag) {
        key1_flag = 0;
        func_index = table_dispaly[func_index].up;    // 向上翻
    }
    if (key2_flag) {
        key2_flag = 0;
        func_index = table_dispaly[func_index].down;    // 向下翻
    }
    if (key3_flag) {
        key3_flag = 0;
        func_index = table_dispaly[func_index].enter;    // 确认
    }

    if (func_index != last_index) {
        current_operation_index = table_dispaly[func_index].current_operation;

        ips_clear();
        (*current_operation_index)(); // 执行当前操作函数
        last_index = func_index;
    }
}

int Menu_key_Operation(int *param_t) // 按键调节界面操作函数//指针变量作为形参，那函数内的形参也要为指针形式
{
  //key1_flag等传递到key_val
  //*param_t是指针，具体指什么。。。
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

//下面就是三级菜单函数了
void Menu_flash_clear(int PAGE_INDEX) // 清除FLASH
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

    printf("\r\n已擦除指定页FLASH:%d", PAGE_INDEX);
}

void FLAG_param_t_init() // FLAG参数初始化
{
    if (flash_check(FLASH_SECTION_INDEX, FLAG_PAGE_INDEX)) { // 判断Flash是否有数据 : 有数据返回1，无数据返回0

        flash_buffer_clear(); // 清空缓冲区

        flash_read_page_to_buffer(FLASH_SECTION_INDEX, FLAG_PAGE_INDEX); // 将数据从FLASH指定扇区页码放入到缓冲区
        GO_FLAG = flash_union_buffer[0].int32_type; // 用什么数据类型写入缓冲区就用什么类型读取，不然打印出来的码是乱码
        STOP_MENU_FALG = flash_union_buffer[1].int32_type;
        flash_erase_page(FLASH_SECTION_INDEX, FLAG_PAGE_INDEX); // 每次修改值并复位后，将FLASH中的值赋给变量，然后擦除FLASH中的值(自动擦除)

        printf("\r\n已经更新过的(GO_FLAG):%d", GO_FLAG);
        printf("\r\n已经更新过的(STOP_MENU_FALG):%d", STOP_MENU_FALG);
    } else {
        printf("\r\nFLAG为程序设定值(原始值)");
    }
}

void Stop_point_param_t_init() // Stop_point参数初始化
{
    if (flash_check(FLASH_SECTION_INDEX, Stop_Point_PAGE_INDEX)) { // 判断Flash是否有数据 : 有数据返回1，无数据返回0

        flash_buffer_clear(); // 清空缓冲区

        flash_read_page_to_buffer(FLASH_SECTION_INDEX, Stop_Point_PAGE_INDEX); // 将数据从FLASH指定扇区页码放入到缓冲区
        S_Point = flash_union_buffer[0].int32_type; // 用什么数据类型写入缓冲区就用什么类型读取，不然打印出来的码是乱码
        printf("\r\n已经更新过的(Stop_Point):%d", S_Point);
    } else {
        printf("\r\nStop_Point为程序设定值(原始值)");
    }
}

void Point_distance_param_t_init() // Point_distance参数初始化
{
    if (flash_check(FLASH_SECTION_INDEX, Point_distance_PAGE_INDEX)) { // 判断Flash是否有数据 : 有数据返回1，无数据返回0

        flash_buffer_clear(); // 清空缓冲区

        flash_read_page_to_buffer(FLASH_SECTION_INDEX, Point_distance_PAGE_INDEX); // 将数据从FLASH指定扇区页码放入到缓冲区
        P_Distance = flash_union_buffer[0].int32_type; // 用什么数据类型写入缓冲区就用什么类型读取，不然打印出来的码是乱码
        printf("\r\n已经更新过的(Point_Distance):%d", P_Distance);
    } else {
        printf("\r\nPoint_Distance为程序设定值(原始值)");
    }
}

/*********第0层***********/
void fun_0()
{
  //喜欢吗 是MOSS哦
    ips_show_rgb565_image(0, 0, (const uint16 *)gImage_MOSS, 240, 125, 240, 125, 0); // isp114
}

/*********第1层***********/
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

/*********第2层***********/
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
// 5.14 22:35 我现在还可以干什么，

void fun_a31() // GPS点位采集并存入对应FLASH
{
    if (gpio_get_level(SWITCH1)) {
        while (gpio_get_level(SWITCH1)) {
            key_scan();
            if (key4_flag) {
                key4_flag = 0;
                GPS_Record_flash();
            }
            ips_show_string(0, 16 * 3, "T:"); // 浮点实时点位
            ips_show_float(50, 16 * 4, gnss.latitude, 3, 6);
            ips_show_float(50, 16 * 5, gnss.longitude, 3, 6);
        }
    } else {
        ips_show_string(0, 16 * 1, "Function not executing"); // 待执行函数未执行
    }
}

void fun_a32() // 综合信息显示
{
    if (gpio_get_level(SWITCH1)) {
        while (gpio_get_level(SWITCH1)) {
            GPS_SHOW(); // 信息显示
        }
    }
}

void fun_a33() // 清除
{
    flash_buffer_clear(); // 清空缓冲区(保证有一个干净的缓冲区)------Warning
    if (flash_check(FLASH_SECTION_INDEX, GPS_PAGE_INDEX)) { // 判断Flash是否有数据 : 有数据返回1，无数据返回0
        Menu_flash_clear(GPS_PAGE_INDEX); // 清除GPS参数FLASH
    } else {
        ips_show_string(0, 16 * 0, "FLASH is blank !");
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////-----------Parameter_SET
void fun_b31() // Speed_regulate
{
    if (gpio_get_level(SWITCH1)) { // Speed_regulate调节
        while (gpio_get_level(SWITCH1)) {
            SPEED_Pointer = &(SPEED); // 直接将要操作的变量取地址并赋值给指针变量

            ips_show_string(0, 16 * 1, "Function executing"); // 待执行函数执行
            ips_show_string(0, 16 * 2, "Speed_regulate");
            ips_show_string(0, 16 * 3, "Speed");
            key_scan();
            Menu_key_Operation(SPEED_Pointer); // 直接操作地址，避免了形参为动态变量会自动释放内存的问题

            if (gpio_get_level(SWITCH2)) { // 在左拨码开关为1的情况下，右拨码开关为1进入保存参数模式
                flash_union_buffer[0].int32_type = Menu_key_Operation(SPEED_Pointer) * 100; // 对于速度来说,10和1都太小了，将其放大100倍
                flash_write_page_from_buffer(FLASH_SECTION_INDEX, SPEED_PAGE_INDEX); // 将缓冲区的数据写入到指定Flash 扇区的页码
                printf("\r\n缓冲区数据SPEED:%d", flash_union_buffer[0].int32_type);
                ips_show_string(0, 16 * 4, "Record param_t OK!");
            }
        }
    }
}

void fun_b32() // Stop_point/Point_distance
{
    if (gpio_get_level(SWITCH1) && !gpio_get_level(SWITCH2)) { // Stop_point调节
        while (gpio_get_level(SWITCH1)) {
            Stop_Point_Pointer = &(Stop_Point);
            ips_show_string(0, 16 * 1, "Function executing"); // 待执行函数执行
            ips_show_string(0, 16 * 2, "Stop_Point");
            ips_show_string(0, 16 * 3, "S_Point");

            key_scan();
            Menu_key_Operation(Stop_Point_Pointer); // 直接操作地址，避免了形参为动态变量会自动释放内存的问题
            if (gpio_get_level(SWITCH2)) { // 在左拨码开关为1的情况下，右拨码开关为1进入保存参数模式
                flash_buffer_clear(); // 清空缓冲区

                flash_union_buffer[0].int32_type = Menu_key_Operation(Stop_Point_Pointer);
                flash_write_page_from_buffer(FLASH_SECTION_INDEX, Stop_Point_PAGE_INDEX); // 将缓冲区的数据写入到指定Flash 扇区的页码
                printf("\r\n缓冲区数据Stop_Point:%d", flash_union_buffer[0].int32_type);
                ips_show_string(0, 16 * 4, "Record param_t OK!");
            }
        }
    }

    if (gpio_get_level(SWITCH2) && !gpio_get_level(SWITCH1)) { // Point_distance调节
        while (gpio_get_level(SWITCH2)) {
            Point_distance_Pointer = &(Point_distance);
            ips_show_string(0, 16 * 1, "Function executing"); // 待执行函数执行
            ips_show_string(0, 16 * 2, "Point_distance");
            ips_show_string(0, 16 * 3, "P_distance");

            key_scan();
            Menu_key_Operation(Point_distance_Pointer); // 直接操作地址，避免了形参为动态变量会自动释放内存的问题

            if (gpio_get_level(SWITCH1)) { // 在右拨码开关为1的情况下，左拨码开关为1进入保存参数模式
                flash_buffer_clear(); // 清空缓冲区

                flash_union_buffer[0].int32_type = Menu_key_Operation(Point_distance_Pointer);
                flash_write_page_from_buffer(FLASH_SECTION_INDEX, Point_distance_PAGE_INDEX); // 将缓冲区的数据写入到指定Flash 扇区的页码
                printf("\r\n缓冲区数据Point_distance:%d", flash_union_buffer[0].int32_type);
                ips_show_string(0, 16 * 4, "Record param_t OK!");
            }
        }
    }
}

void fun_b33() // 清除
{
    if (gpio_get_level(SWITCH1) && !gpio_get_level(SWITCH2)) {
        flash_buffer_clear(); // 清空缓冲区(保证有一个干净的缓冲区)------Warning
        Menu_flash_clear(SPEED_PAGE_INDEX); // 清除SPEED参数FLASH
    }

    if (gpio_get_level(SWITCH1) && gpio_get_level(SWITCH2)) {
        flash_buffer_clear(); // 清空缓冲区(保证有一个干净的缓冲区)------Warning
        Menu_flash_clear(Stop_Point_PAGE_INDEX); // 清除Stop_Point参数FLASH
    }

    if (!gpio_get_level(SWITCH1) && gpio_get_level(SWITCH2)) {
        flash_buffer_clear(); // 清空缓冲区(保证有一个干净的缓冲区)------Warning
        Menu_flash_clear(Point_distance_PAGE_INDEX); // 清除Point_distance参数FLASH
    }
}

void fun_c31() // START_CAR调节
{
    if (gpio_get_level(SWITCH2)) { // START_CAR调节
        while (gpio_get_level(SWITCH2)) {
            GO_FLAG = 1; // 打开发车标志位
            ips_show_string(0, 16 * 0, "GO_FLAG"); ips_show_int(120, 0, GO_FLAG, 3);
            printf("\r\nGO_FLAG:%d", GO_FLAG);

            STOP_MENU_FALG = 0; // 关闭菜单标志位
            ips_show_string(0, 16 * 1, "STOP_MENU"); ips_show_int(120, 16 * 1, STOP_MENU_FALG, 3);
            printf("\r\nSTOP_MENU_FALG:%d", STOP_MENU_FALG);

            ips_show_string(0, 16 * 3, "GO GO GO !!!");
            ips_show_float(0, 16 * 5, gnss.latitude, 3, 6);
            ips_show_float(0, 16 * 6, gnss.longitude, 3, 6);
            ips_show_string(0, 16 * 7, "satellite:"); ips_show_uint(100, 16 * 7, gnss.satellite_used, 5);

            flash_buffer_clear(); // 清空缓冲区

            flash_union_buffer[0].int32_type = GO_FLAG; // 标志位存入缓冲区
            flash_union_buffer[1].int32_type = STOP_MENU_FALG; // 标志位存入缓冲区
            flash_write_page_from_buffer(FLASH_SECTION_INDEX, FLAG_PAGE_INDEX); // 将缓冲区的数据写入到指定Flash 扇区的页码
            printf("\r\nGO_FLAG:%d", flash_union_buffer[0].int32_type);
            printf("\r\nSTOP_MENU_FALG:%d", flash_union_buffer[1].int32_type);
        }
    }
}

void fun_c32() // 舵机测试调节
{
    while (gpio_get_level(SWITCH1)) {
        Control_FLAG = 0; // 隔绝GPS对舵机的影响
        Steer_text(); // 舵机测试
    }
}

void fun_c33() // 舵机测试调节
{
    while (gpio_get_level(SWITCH1)) {
        Motor_text(); // 电机测试
    }
}
