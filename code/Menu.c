#include "zf_common_headfile.h"
#include "zf_device_gnss.h" // 替换头文件

extern gnss_info_struct gnss; // 使用gnss变量

int GO_FLAG = 0;       // 发车标志位
int STOP_MENU_FLAG = 1;// 菜单标志位

int func_index = 0;    // 初始显示欢迎界面
int last_index = 127;  // last初始为无效值

int SPEED = 5000;            // SPEED参数
int *SPEED_Pointer;       // SPEED参数指针
int Stop_Point = 0;       // Stop_Point参数
int *Stop_Point_Pointer;  // Stop_Point参数指针
int Point_distance = 0;   // Point_distance参数
int *Point_distance_Pointer;// Point_distance参数指针

void (*current_operation_index)(void); // 显示函数索引指针(当前操作索引)

/*
// 定义菜单结构体
typedef struct {
    int index;
    int up;
    int down;
    int enter;
    void (*current_operation)(void);
} key_table;
*/

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

        ips200_clear();
        (*current_operation_index)(); // 执行当前操作函数
        last_index = func_index;
    }
    
    
//menutest    
        

        // 此处编写需要循环执行的代码
    
}

    
    
int Menu_key_Operation(int *param_t) // 按键调节界面操作函数//指针变量作为形参，那函数内的形参也要为指针形式
{
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

    ips200_show_int(120, 16 * 3, *param_t, 6);

    return *param_t;
}

//下面就是三级菜单函数了
void Menu_flash_clear(int PAGE_INDEX) // 清除FLASH
{
    flash_erase_page(FLASH_SECTION_INDEX, PAGE_INDEX);
    ips200_show_string(0, 16 * 0, "Clear FLASH OK!");

    if (PAGE_INDEX == 10) {
        ips200_show_string(0, 16 * 1, "SPEED_PAGE_INDEX(10)-->");
    } else if (PAGE_INDEX == 8) {
        ips200_show_string(0, 16 * 1, "Stop_Point_PAGE_INDEX(8)-->");
    } else if (PAGE_INDEX == 7) {
        ips200_show_string(0, 16 * 1, "Point_Dis_PAGE_INDEX(7)-->");
    } else {
    }
    

   // printf("\r\n已擦除指定页FLASH:%d", PAGE_INDEX);
}

void FLAG_param_t_init() // FLAG参数初始化
{
    if (flash_check(FLASH_SECTION_INDEX, FLAG_PAGE_INDEX)) { // 判断Flash是否有数据 : 有数据返回1，无数据返回0

        flash_buffer_clear(); // 清空缓冲区

        flash_read_page(FLASH_SECTION_INDEX, FLAG_PAGE_INDEX, (uint32 *)flash_union_buffer, 2); // 将数据从FLASH指定扇区页码放入到缓冲区
        GO_FLAG = flash_union_buffer[0].int32_type; // 用什么数据类型写入缓冲区就用什么类型读取，不然打印出来的码是乱码
        STOP_MENU_FLAG = flash_union_buffer[1].int32_type;
        flash_erase_page(FLASH_SECTION_INDEX, FLAG_PAGE_INDEX); // 每次修改值并复位后，将FLASH中的值赋给变量，然后擦除FLASH中的值(自动擦除)

       // printf("\r\n已经更新过的(GO_FLAG):%d", GO_FLAG);
        //printf("\r\n已经更新过的(STOP_MENU_FLAG):%d", STOP_MENU_FLAG);
    } else {
       // printf("\r\nFLAG为程序设定值(原始值)");
    }
}

void Stop_point_param_t_init() // Stop_point参数初始化
{
    if (flash_check(FLASH_SECTION_INDEX, Stop_Point_PAGE_INDEX)) { // 判断Flash是否有数据 : 有数据返回1，无数据返回0

        flash_buffer_clear(); // 清空缓冲区

        flash_read_page(FLASH_SECTION_INDEX, Stop_Point_PAGE_INDEX, (uint32 *)flash_union_buffer, 1); // 将数据从FLASH指定扇区页码放入到缓冲区
        Stop_Point = flash_union_buffer[0].int32_type; // 用什么数据类型写入缓冲区就用什么类型读取，不然打印出来的码是乱码
       // printf("\r\n已经更新过的(Stop_Point):%d", Stop_Point);
    } else {
       // printf("\r\nStop_Point为程序设定值(原始值)");
    }
}

void Point_distance_param_t_init() // Point_distance参数初始化
{
    if (flash_check(FLASH_SECTION_INDEX, Point_distance_PAGE_INDEX)) { // 判断Flash是否有数据 : 有数据返回1，无数据返回0

        flash_buffer_clear(); // 清空缓冲区

        flash_read_page(FLASH_SECTION_INDEX, Point_distance_PAGE_INDEX, (uint32 *)flash_union_buffer, 1); // 将数据从FLASH指定扇区页码放入到缓冲区
        Point_distance = flash_union_buffer[0].int32_type; // 用什么数据类型写入缓冲区就用什么类型读取，不然打印出来的码是乱码
       // printf("\r\n已经更新过的(Point_Distance):%d", Point_distance);
    } else {
       // printf("\r\nPoint_Distance为程序设定值(原始值)");
    }
}

/*********第0层***********/
void fun_0()
{
    //ips200_show_rgb565_image(0, 0, (const uint16 *)gImage_MOSS, 240, 125, 240, 125, 0); // isp114
    //ips200_show_int(0,0,(int)Daty_Z,3);
    //ips200_show_float(0,0,Gyro_Offset.Zdata,3,5);
    IMU_SHOW();
    
}
/*********第1层***********/
void fun_a1()
{
    ips200_show_string(0, 16 * 1, "->");
    ips200_show_string(20, 16 * 1, "GPS_SET");
    ips200_show_string(20, 16 * 3, "Parameter_SET");
    ips200_show_string(20, 16 * 5, "Other_SET");
    ips200_show_string(20, 16 * 7, "ESC");
}

void fun_b1()
{
    ips200_show_string(0, 16 * 3, "->");
    ips200_show_string(20, 16 * 1, "GPS_SET");
    ips200_show_string(20, 16 * 3, "Parameter_SET");
    ips200_show_string(20, 16 * 5, "Other_SET");
    ips200_show_string(20, 16 * 7, "ESC");
}

void fun_c1()
{
    ips200_show_string(0, 16 * 5, "->");
    ips200_show_string(20, 16 * 1, "GPS_SET");
    ips200_show_string(20, 16 * 3, "Parameter_SET");
    ips200_show_string(20, 16 * 5, "Other_SET");
    ips200_show_string(20, 16 * 7, "ESC");
}

void fun_d1()
{
    ips200_show_string(0, 16 * 7, "->");
    ips200_show_string(20, 16 * 1, "GPS_SET");
    ips200_show_string(20, 16 * 3, "Parameter_SET");
    ips200_show_string(20, 16 * 5, "Other_SET");
    ips200_show_string(20, 16 * 7, "ESC");
}

/*********第2层***********/
void fun_a21() // GPS_SET
{
    ips200_show_string(0, 16 * 1, "->");
    ips200_show_string(20, 16 * 1, "GPS storage point");
    ips200_show_string(20, 16 * 3, "Com_display");
    ips200_show_string(20, 16 * 5, "CLEAR_GPS_FLASH");
    ips200_show_string(20, 16 * 7, "ESC");
}

void fun_a22()
{
    ips200_show_string(0, 16 * 3, "->");
    ips200_show_string(20, 16 * 1, "GPS storage point");
    ips200_show_string(20, 16 * 3, "Com_display");
    ips200_show_string(20, 16 * 5, "CLEAR_GPS_FLASH");
    ips200_show_string(20, 16 * 7, "ESC");
}

void fun_a23()
{
    ips200_show_string(0, 16 * 5, "->");
    ips200_show_string(20, 16 * 1, "GPS storage point");
    ips200_show_string(20, 16 * 3, "Com_display");
    ips200_show_string(20, 16 * 5, "CLEAR_GPS_FLASH");
    ips200_show_string(20, 16 * 7, "ESC");
}

void fun_a24()
{
    ips200_show_string(0, 16 * 7, "->");
    ips200_show_string(20, 16 * 1, "GPS storage point");
    ips200_show_string(20, 16 * 3, "Com_display");
    ips200_show_string(20, 16 * 5, "CLEAR_GPS_FLASH");
    ips200_show_string(20, 16 * 7, "ESC");
}

void fun_b21() // Parameter_SET
{
    ips200_show_string(0, 16 * 1, "->");
    ips200_show_string(20, 16 * 1, "Speed_regulate");
    ips200_show_string(20, 16 * 3, "Stop_point/Point_distance");
    ips200_show_string(20, 16 * 5, "CLEAR_PAT_FLASH");
    ips200_show_string(20, 16 * 7, "ESC");
}

void fun_b22()
{
    ips200_show_string(0, 16 * 3, "->");
    ips200_show_string(20, 16 * 1, "Speed_regulate");
    ips200_show_string(20, 16 * 3, "Stop_point/Point_distance");
    ips200_show_string(20, 16 * 5, "CLEAR_PAT_FLASH");
    ips200_show_string(20, 16 * 7, "ESC");
}

void fun_b23()
{
    ips200_show_string(0, 16 * 5, "->");
    ips200_show_string(20, 16 * 1, "Speed_regulate");
    ips200_show_string(20, 16 * 3, "Stop_point/Point_distance");
    ips200_show_string(20, 16 * 5, "CLEAR_PAT_FLASH");
    ips200_show_string(20, 16 * 7, "ESC");
}

void fun_b24()
{
    ips200_show_string(0, 16 * 7, "->");
    ips200_show_string(20, 16 * 1, "Speed_regulate");
    ips200_show_string(20, 16 * 3, "Stop_point/Point_distance");
    ips200_show_string(20, 16 * 5, "CLEAR_PAT_FLASH");
    ips200_show_string(20, 16 * 7, "ESC");
}

void fun_c21() // Other_SET
{
    ips200_show_string(0, 16 * 1, "->");
    ips200_show_string(20, 16 * 1, "START_CAR");
    ips200_show_string(20, 16 * 3, "Steer_TEXT");
    ips200_show_string(20, 16 * 5, "Motor_TEXT");
    ips200_show_string(20, 16 * 7, "ESC");
}

void fun_c22()
{
    ips200_show_string(0, 16 * 3, "->");
    ips200_show_string(20, 16 * 1, "START_CAR");
    ips200_show_string(20, 16 * 3, "Steer_TEXT");
    ips200_show_string(20, 16 * 5, "Motor_TEXT");
    ips200_show_string(20, 16 * 7, "ESC");
}

void fun_c23()
{
    ips200_show_string(0, 16 * 5, "->");
    ips200_show_string(20, 16 * 1, "START_CAR");
    ips200_show_string(20, 16 * 3, "Steer_TEXT");
    ips200_show_string(20, 16 * 5, "Motor_TEXT");
    ips200_show_string(20, 16 * 7, "ESC");
}

void fun_c24()
{
    ips200_show_string(0, 16 * 7, "->");
    ips200_show_string(20, 16 * 1, "START_CAR");
    ips200_show_string(20, 16 * 3, "Steer_TEXT");
    ips200_show_string(20, 16 * 5, "Motor_TEXT");
    ips200_show_string(20, 16 * 7, "ESC");
}

//////////////////////////////////////////////////////////////////////////////////////////////////-----------GPS_SET
void fun_a31() // GPS点位采集并存入对应FLASH
{
    if (gpio_get_level(SWITCH1)) {
        while (gpio_get_level(SWITCH1)) {
            key_scan();
            if (key4_flag) {
                key4_flag = 0;
                GPS_Record_flash();
                //ips200_show_float(50, 16 * 4, flash_union_buffer[Number].float_type, 3, 6);
            }
            ips200_show_string(0, 16 * 3, "T:"); // 浮点实时点位
            ips200_show_float(50, 16 * 4, gnss.latitude, 3, 6);
            ips200_show_float(50, 16 * 5, gnss.longitude, 3, 6);
        }
    } else {
        ips200_show_string(0, 16 * 1, "Function not executing"); // 待执行函数未执行
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
        ips200_show_string(0, 16 * 0, "FLASH is blank !");
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////-----------Parameter_SET
void fun_b31() // Speed_regulate
{
    if (gpio_get_level(SWITCH1)) { // Speed_regulate调节
        while (gpio_get_level(SWITCH1)) {
            SPEED_Pointer = &(SPEED); // 直接将要操作的变量取地址并赋值给指针变量

            ips200_show_string(0, 16 * 1, "Function executing"); // 待执行函数执行
            ips200_show_string(0, 16 * 2, "Speed_regulate");
            ips200_show_string(0, 16 * 3, "Speed");
            key_scan();
            Menu_key_Operation(SPEED_Pointer); // 直接操作地址，避免了形参为动态变量会自动释放内存的问题

            if (gpio_get_level(SWITCH2)) { // 在左拨码开关为1的情况下，右拨码开关为1进入保存参数模式
                flash_union_buffer[0].int32_type = Menu_key_Operation(SPEED_Pointer) * 100; // 对于速度来说,10和1都太小了，将其放大100倍
                flash_read_page(FLASH_SECTION_INDEX, FLAG_PAGE_INDEX, (uint32 *)flash_union_buffer, 2); // 将缓冲区的数据写入到指定Flash 扇区的页码
              //  printf("\r\n缓冲区数据SPEED:%d", flash_union_buffer[0].int32_type);
                ips200_show_string(0, 16 * 4, "Record param_t OK!");
            }
        }
    }
}

void fun_b32() // Stop_point/Point_distance
{
    if (gpio_get_level(SWITCH1) && !gpio_get_level(SWITCH2)) { // Stop_point调节
        while (gpio_get_level(SWITCH1)) {
            Stop_Point_Pointer = &(Stop_Point);
            ips200_show_string(0, 16 * 1, "Function executing"); // 待执行函数执行
            ips200_show_string(0, 16 * 2, "Stop_Point");
            ips200_show_string(0, 16 * 3, "S_Point");

            key_scan();
            Menu_key_Operation(Stop_Point_Pointer); // 直接操作地址，避免了形参为动态变量会自动释放内存的问题
            if (gpio_get_level(SWITCH2)) { // 在左拨码开关为1的情况下，右拨码开关为1进入保存参数模式
                flash_buffer_clear(); // 清空缓冲区

                flash_union_buffer[0].int32_type = Menu_key_Operation(Stop_Point_Pointer);
                flash_write_page_from_buffer(FLASH_SECTION_INDEX, Stop_Point_PAGE_INDEX,10); // 将缓冲区的数据写入到指定Flash 扇区的页码
               // printf("\r\n缓冲区数据Stop_Point:%d", flash_union_buffer[0].int32_type);
                ips200_show_string(0, 16 * 4, "Record param_t OK!");
            }
        }
    }

    if (gpio_get_level(SWITCH2) && !gpio_get_level(SWITCH1)) { // Point_distance调节
        while (gpio_get_level(SWITCH2)) {
            Point_distance_Pointer = &(Point_distance);
            ips200_show_string(0, 16 * 1, "Function executing"); // 待执行函数执行
            ips200_show_string(0, 16 * 2, "Point_distance");
            ips200_show_string(0, 16 * 3, "P_distance");

            key_scan();
            Menu_key_Operation(Point_distance_Pointer); // 直接操作地址，避免了形参为动态变量会自动释放内存的问题

            if (gpio_get_level(SWITCH1)) { // 在右拨码开关为1的情况下，左拨码开关为1进入保存参数模式
                flash_buffer_clear(); // 清空缓冲区

                flash_union_buffer[0].int32_type = Menu_key_Operation(Point_distance_Pointer);
                flash_write_page_from_buffer(FLASH_SECTION_INDEX, Point_distance_PAGE_INDEX,10); // 将缓冲区的数据写入到指定Flash 扇区的页码
             //   printf("\r\n缓冲区数据Point_distance:%d", flash_union_buffer[0].int32_type);
                ips200_show_string(0, 16 * 4, "Record param_t OK!");
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
            ips200_show_string(0, 16 * 0, "GO_FLAG"); ips200_show_int(120, 0, GO_FLAG, 3);
           // printf("\r\nGO_FLAG:%d", GO_FLAG);

            STOP_MENU_FLAG = 0; // 关闭菜单标志位
            ips200_show_string(0, 16 * 1, "STOP_MENU"); ips200_show_int(120, 16 * 1, STOP_MENU_FLAG, 3);
            //printf("\r\nSTOP_MENU_FALG:%d", STOP_MENU_FLAG);

            ips200_show_string(0, 16 * 3, "GO GO GO !!!");
            ips200_show_float(0, 16 * 5, gnss.latitude, 3, 6);
            ips200_show_float(0, 16 * 6, gnss.longitude, 3, 6);
            ips200_show_string(0, 16 * 7, "satellite:"); ips200_show_uint(100, 16 * 7, gnss.satellite_used, 5);
            ips200_show_int(0,16*8,next_point,2);
            ips200_show_float(0, 16 * 9, Work_target_array[0][next_point],3,6); 
            ips200_show_float(0, 16 * 10, Work_target_array[1][next_point] ,3,6);

            flash_buffer_clear(); // 清空缓冲区

            flash_union_buffer[0].int32_type = GO_FLAG; // 标志位存入缓冲区
            flash_union_buffer[1].int32_type = STOP_MENU_FLAG; // 标志位存入缓冲区
            flash_write_page_from_buffer(FLASH_SECTION_INDEX, FLAG_PAGE_INDEX,10); // 将缓冲区的数据写入到指定Flash 扇区的页码
          //  printf("\r\nGO_FLAG:%d", flash_union_buffer[0].int32_type);
            //printf("\r\nSTOP_MENU_FALG:%d", flash_union_buffer[1].int32_type);
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

//16位BMP 240*80 逐飞科技logo图像取模数据
//Image2LCD取模选项设置
//水平扫描
//16位
//240*80
//不包含图像头数据
//自左至右
//自顶至底
//低位在前
//- MOSS好像出什么bug；MOSS被我注释了——注释不了？


//智能量子计算机MOSS图片用作壁纸
//IFX_ALIGN(4)  
const unsigned char gImage_MOSS[60008] = { 0x00
};