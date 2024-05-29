#include "zf_common_headfile.h"
#include "zf_device_gnss.h" // 替换头文件

extern gnss_info_struct gnss; // 使用gnss变量

int Number = 0; // 介质变量

#define FLASH_SECTION_INDEX 0
#define GPS_PAGE_INDEX 10
#define DATA_LENGTH 128
#define RP_MAX 150

#if Int_Record_FLAG

uint32 Target_point[2][150]; // 用于储存采集的目标点信息，用于后续的位置计算(第一层是纬度，第二层是经度)
uint32 Work_target_array[2][150]; // 用于将从flash中读取的目标点数组的数据复制过来，当这个数组被赋值时，后续的一切和Flash再无关系
uint32 lat; // 纬度
uint32 lot; // 经度

void GPS_Record_flash() { // 将采集的点位记录到缓冲区并储存至GPS_FLASH
    static int NUM = 0; // 采集点的次数

    lat = gnss.latitude * 1000000; // 纬度*100万(GPS回传数据只有后6位有效)
    lot = gnss.longitude * 1000000; // 经度*100万(GPS回传数据只有后6位有效)

    flash_union_buffer[Number].uint32_type = lat; // 将纬度数据强制转换后储存在FLASH 操作的数据缓冲区 (数组标号为偶数时是纬度数据)
    ips200_show_string(0, 16 * 0, "R:");
    ips200_show_uint(50, 16 * 0, flash_union_buffer[Number].uint32_type, 10);
    ips200_show_uint(180, 16 * 0, Number, 3);
    printf("\r\n缓冲区纬度数据:%d", (int)flash_union_buffer[Number].uint32_type);

    Number++; // 数组下标+1,切换储存经度

    flash_union_buffer[Number].uint32_type = lot; // 将经度数据强制转换后储存在FLASH 操作的数据缓冲区 (数组标号为奇数时是经度数据)
    ips200_show_uint(50, 16 * 1, flash_union_buffer[Number].uint32_type, 10);
    ips200_show_uint(180, 16 * 1, Number, 3);
    printf("\r\n缓冲区经度数据:%d", (int)flash_union_buffer[Number].uint32_type);

    Number++; // 数组下标+1,切换储存纬度

    if (flash_check(FLASH_SECTION_INDEX, GPS_PAGE_INDEX)) { // 判断Flash是否有数据 : 有数据返回1，无数据返回0
        flash_erase_page(FLASH_SECTION_INDEX, GPS_PAGE_INDEX); // 擦除Flash数据
    }

    flash_write_page(FLASH_SECTION_INDEX, GPS_PAGE_INDEX, (uint32 *)flash_union_buffer, Number); // 将缓冲区的数据写入到指定Flash 扇区的页码

    NUM++;

    if (NUM > RP_MAX) { // 如果采集点次数大于规定次数的最大值，则让他等于1
        NUM = 1;
    }

    ips200_show_uint(200, 0, NUM, 3); // 显示已采集的点数
}

void GPS_Flash_use() { // 将GPS_FLASH的数据重新读回缓冲区并赋值给数组
    flash_buffer_clear(); // 清空缓冲区

    if (flash_check(FLASH_SECTION_INDEX, GPS_PAGE_INDEX)) { // 判断Flash是否有数据 : 有数据返回1，无数据返回0
        flash_read_page(FLASH_SECTION_INDEX, GPS_PAGE_INDEX, (uint32 *)flash_union_buffer, DATA_LENGTH); // 将数据从FLASH指定扇区页码放入到缓冲区

        int TG = 0; // 用以切换二维数组下标

        for (int data = 0; data < DATA_LENGTH; data++) {
            Target_point[0][TG] = flash_union_buffer[data].uint32_type; // 纬度
            data++;
            Target_point[1][TG] = flash_union_buffer[data].uint32_type; // 经度
            TG++; // 下标+1
        }
        printf("\r\n成功从GPS_FLASH取回数据\n");
    }
}
#endif

// GPS_Work_array 函数的主要作用是将目标数组 (Target_point) 中的数据复制到工作目标数组 (Work_target_array) 中，以便在后续的计算中使用。这一步骤可以理解为将原始数据提取并进行隔离，从而避免对Flash的直接操作，提高数据访问的效率和安全性

void GPS_Work_array() { // 用于隔离FLASH,实际参加计算的数组
    for (int NUM = 0; NUM < (ARRAY_Element_Calculation() / 2); NUM++) {
        Work_target_array[0][NUM] = Target_point[0][NUM];
        Work_target_array[1][NUM] = Target_point[1][NUM];

        printf("\r\n组数-%d,工作数组纬度数据-%d,工作数组经度数据-%d", NUM + 1, (int)Work_target_array[0][NUM], (int)Work_target_array[1][NUM]);
        system_delay_ms(50);
    }
}

void GPS_param_t_init() { // GPS参数初始化
    if (flash_check(FLASH_SECTION_INDEX, GPS_PAGE_INDEX)) { // 判断Flash是否有数据 : 有数据返回1，无数据返回0
        GPS_Flash_use(); // 将GPS_FLASH的数据重新读回缓冲区并赋值给数组
        GPS_Work_array(); // 将目标数组的数据复制一份到工作目标数组(从这里之后GPS采点彻底结束,用以隔离FLASH)
    } else {
        printf("\r\nGPS为程序设定值(原始值)");
    }
}

// 总会有一个show
void GPS_SHOW() { // GPS信息显示
    // 每个字符宽占8个格,长占16个格子
    ips200_show_string(0, 16 * 0, "state:"); ips200_show_uint(90, 16 * 0, gnss.state, 5);
    ips200_show_string(0, 16 * 1, "direction:"); ips200_show_float(90, 16 * 1, gnss.direction, 4, 6);
    ips200_show_string(0, 16 * 2, "latitude:"); ips200_show_float(90, 16 * 2, gnss.latitude, 4, 6);
    ips200_show_string(0, 16 * 3, "longitude:"); ips200_show_float(90, 16 * 3, gnss.longitude, 4, 6);
    ips200_show_string(0, 16 * 4, "yaw:"); ips200_show_float(90, 16 * 4, Daty_Z, 4, 6);
}

#if Float_Record_FLAG

float Target_point[2][150]; // 用于储存采集的目标点信息，用于后续的位置计算(第一层是纬度，第二层是经度)
float Work_target_array[2][150]; // 用于将从flash中读取的目标点数组的数据复制过来，当这个数组被赋值时，后续的一切和Flash再无关系
double lat; // 纬度
double lot; // 经度

void GPS_Record_flash() { // 将采集的点位记录到缓冲区并储存至GPS_FLASH
    static int NUM = 0; // 采集点的次数

    lat = gnss.latitude; // 获取纬度
    lot = gnss.longitude; // 获取经度

    flash_union_buffer[Number].float_type = lat; // 将纬度数据强制转换后储存在FLASH 操作的数据缓冲区 (数组标号为偶数时是纬度数据)
    ips200_show_string(0, 16 * 0, "R:");
    ips200_show_float(50, 16 * 0, flash_union_buffer[Number].float_type, 3, 6);
    ips200_show_uint(180, 16 * 0, Number, 3);
    printf("\r\n缓冲区纬度数据:%f", flash_union_buffer[Number].float_type);

    Number++; // 数组下标+1,切换储存经度

    flash_union_buffer[Number].float_type = lot; // 将经度数据强制转换后储存在FLASH 操作的数据缓冲区 (数组标号为奇数时是经度数据)
    ips200_show_float(50, 16 * 1, flash_union_buffer[Number].float_type, 3, 6);
    ips200_show_uint(180, 16 * 1, Number, 3);
    printf("\r\n缓冲区经度数据:%f", flash_union_buffer[Number].float_type);

    Number++; // 数组下标+1,切换储存纬度

    if (flash_check(FLASH_SECTION_INDEX, GPS_PAGE_INDEX)) { // 判断Flash是否有数据 : 有数据返回1，无数据返回0
        flash_erase_page(FLASH_SECTION_INDEX, GPS_PAGE_INDEX); // 擦除Flash数据
    }

    flash_write_page(FLASH_SECTION_INDEX, GPS_PAGE_INDEX, (uint32 *)flash_union_buffer, Number); // 将缓冲区的数据写入到指定Flash 扇区的页码

    NUM++;

    if (NUM > RP_MAX) { // 如果采集点次数大于规定次数的最大值，则让他等于1
        NUM = 1;
    }

    ips200_show_uint(200, 0, NUM, 3); // 显示已采集的点数
}

void GPS_Flash_use() { // 将GPS_FLASH的数据重新读回缓冲区并赋值给数组
    flash_buffer_clear(); // 清空缓冲区

    if (flash_check(FLASH_SECTION_INDEX, GPS_PAGE_INDEX)) { // 判断Flash是否有数据 : 有数据返回1，无数据返回0
        flash_read_page(FLASH_SECTION_INDEX, GPS_PAGE_INDEX, (uint32 *)flash_union_buffer, DATA_LENGTH); // 将数据从FLASH指定扇区页码放入到缓冲区

        int TG = 0; // 用以切换二维数组下标

        for (int data = 0; data < DATA_LENGTH; data++) {
            Target_point[0][TG] = flash_union_buffer[data].float_type; // 纬度数据
            data++;
            Target_point[1][TG] = flash_union_buffer[data].float_type; // 经度数据
            TG++;
        }
        printf("\r\n成功从GPS_FLASH取回数据\n");
    }
}
#endif

#if Double_Record_FLAG

double Target_point[2][150]; // 用于储存采集的目标点信息，用于后续的位置计算(第一层是纬度，第二层是经度)
double Work_target_array[2][150]; // 用于将从flash中读取的目标点数组的数据复制过来，当这个数组被赋值时，后续的一切和Flash再无关系
double lat; // 纬度
double lot; // 经度

uint32 I_lat; // double类型纬度数据整数部分
float F_lat; // double类型纬度数据小数部分(强制转换为float)

uint32 I_lot; // double类型经度数据整数部分
float F_lot; // double类型经度数据小数部分(强制转换为float)

void GPS_Record_flash() { // 将采集的点位记录到缓冲区并储存至GPS_FLASH
    static int NUM = 0; // 采集点的次数

    lat = gnss.latitude; // 获取纬度
    lot = gnss.longitude; // 获取经度

    I_lat = (uint32)(lat); // 获取纬度整数部分
    F_lat = lat - I_lat; // 获取纬度小数部分

    I_lot = (uint32)(lot); // 获取经度整数部分
    F_lot = lot - I_lot; // 获取经度小数部分

    ips200_show_string(0, 16 * 0, "R:");

    flash_union_buffer[Number].uint32_type = I_lat; // 将整数纬度数据存入缓冲区
    ips200_show_uint(50, 16 * 0, flash_union_buffer[Number].uint32_type, 5);
    ips200_show_uint(150, 16 * 0, Number, 3);

    Number++;

    flash_union_buffer[Number].float_type = F_lat; // 将浮点纬度数据存入缓冲区
    ips200_show_float(50, 16 * 1, flash_union_buffer[Number].float_type, 3, 6);
    ips200_show_uint(150, 16 * 1, Number, 3);

    ips200_show_float(50, 16 * 2, gnss.latitude, 3, 6); // 显示完整纬度

    Number++; // 数组下标+1,切换存储经度

    flash_union_buffer[Number].uint32_type = I_lot; // 将整数经度数据存入缓冲区
    ips200_show_uint(50, 16 * 4, flash_union_buffer[Number].uint32_type, 5);
    ips200_show_uint(150, 16 * 4, Number, 3);

    Number++;

    flash_union_buffer[Number].float_type = F_lot; // 将浮点经度数据存入缓冲区
    ips200_show_float(50, 16 * 5, flash_union_buffer[Number].float_type, 3, 6);
    ips200_show_uint(150, 16 * 5, Number, 3);

    ips200_show_float(50, 16 * 6, gnss.longitude, 3, 6); // 显示完整经度

    Number++; // 数组下标+1,切换存储纬度

    if (flash_check(FLASH_SECTION_INDEX, GPS_PAGE_INDEX)) { // 判断Flash是否有数据 : 有数据返回1，无数据返回0
        flash_erase_page(FLASH_SECTION_INDEX, GPS_PAGE_INDEX); // 擦除Flash数据
    }

    flash_write_page(FLASH_SECTION_INDEX, GPS_PAGE_INDEX, (uint32 *)flash_union_buffer, Number); // 将缓冲区数据写入Flash

    NUM++;

    if (NUM > RP_MAX) { // 如果采集点次数大于规定次数的最大值，则让他等于1
        NUM = 1;
    }

    ips200_show_uint(210, 0, NUM, 3); // 显示已采集的点数
}

void GPS_Flash_use() { // 将GPS_FLASH的数据重新读回缓冲区并赋值给数组
    flash_buffer_clear(); // 清空缓冲区

    if (flash_check(FLASH_SECTION_INDEX, GPS_PAGE_INDEX)) { // 判断Flash是否有数据 : 有数据返回1，无数据返回0
        flash_read_page(FLASH_SECTION_INDEX, GPS_PAGE_INDEX, (uint32 *)flash_union_buffer, DATA_LENGTH); // 将数据从FLASH指定扇区页码放入到缓冲区

        int TG = 0; // 用以切换二维数组下标

        for (int data = 0; data < DATA_LENGTH; data++) {
            Target_point[0][TG] = flash_union_buffer[data].uint32_type + flash_union_buffer[data + 1].float_type; // 纬度
            data = data + 2;
            Target_point[1][TG] = flash_union_buffer[data].uint32_type + flash_union_buffer[data + 1].float_type; // 经度
            data = data + 1;
            TG++; // 下标+1
        }
    }
}
#endif
