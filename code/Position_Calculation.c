/*
 * Position_Calculation.c
 *
 *  Created on: 2023年3月18日
 *      Author: ORRN
 */
// 5.14 和zf讨论之后我产生了更多的疑问：GPS到底是如何指导小车的，是那个点还是路线
//首先我估计要摸清，去年的要求是什么，为什么会有这个代码 不然你得反推
//会有一个个状态（不过今年的状态重复性比较高）晚点再细考虑
//首要是 先不管路径规划；大体的状态就两种，在距离下一个点某个距离以外GPS+IMU导航，本质是根据角度调整，距离是判断那和视觉的那些没什么两样了；第二种状态是某个距离以内靠硅麦定位吧
//再往下细分就是根据不同距离调速度；转弯转的顺畅一点
//差50米还是可以粗犷一些的吗，晚点不把舵机烧了就行
//让gpt稍微分析了一下（主要是我不想看），这文件和控制那些都要大改的，但首先让我们把共通的弄清楚
//补充 确实 刚下另一个点时还需要转弯 这时候速度过快不知道会发生什么 
//这样一想，其实本质上还是pid，而且输入的也是角度。。（不过说实话，这pid肯定比气垫船的舒服）不是我想象中的像导航地图那样的啊；相比之下，激光雷达那边才是真正的路径规划

//5.15首先是#if 选标志位
//然后是next_point
//接下来是一个个函数Switching_point_D()等
//并不是特别特别多，但可能复杂程度比你想象中大
//右四个type单层反馈双层反馈

//5.16azimuth和distance 有函数直接提供
//他首先根据point设置速度 和 Navigation_mode
//单层是纯GPS 双层加了IMU
//并没有细说绕锥桶、识别什么的，有绕锥桶
//？但好像没有他们如何绕那个路障，他们是怎么采集的 这个很重要
//
//
//

#include "zf_common_headfile.h"
#include "zf_device_gnss.h" // 替换头文件

extern gnss_info_struct gnss; // 使用gnss变量

int Control_FLAG = 0;         // 该标志位意义是，当它为1时是无控模式(靠GPS+IMU导航)/当手柄上的按键按下时，该标志位被清0,无法执行无控模式，转入有控模式
int S_Point = 0;              // 停止点位参数
int P_Distance = 0;           // 点位距离参数
int next_point = 0;           // 下一个目标点
int Point_interval = 0;       // 点位区间数
int ZT_FLAG = 0;              // 绕锥桶一周标志位
int Distance_FLAG = 0;        // 距离切换标志位
int Angle_FLAG = 0;           // 角度切换标志位
int IMU_suppression_FLAG = 1; // IMU抑制开启标志位
int16 SPEED_Value = 0;        // 目标速度

double distance = 0;    // 两点的距离
double azimuth = 0;     // 方位角(得出的函数返回值为double)
double last_azimuth = 0;// 上一次的方位角
double Error = 0;       // 方位角和航向角的误差(得出的函数返回值为double)

float PD_YAW = 0;

void Follow_Track() // 核心循迹程序
{
  //纯gps模式
#if GPS_Mode
#if (Float_Record_FLAG || Double_Record_FLAG || Int_Record_FLAG) // 单精度和双精度存储都用这个
    distance = get_two_points_distance(gnss.latitude, gnss.longitude, Work_target_array[0][next_point], Work_target_array[1][next_point]); // 距离
    last_azimuth = get_two_points_azimuth(gnss.latitude, gnss.longitude, Work_target_array[0][next_point - 1], Work_target_array[1][next_point - 1]); // 方位角 (两点连线与正北的夹角)
    azimuth = get_two_points_azimuth(gnss.latitude, gnss.longitude, Work_target_array[0][next_point], Work_target_array[1][next_point]); // 方位角 (两点连线与正北的夹角)
#endif

    //没看懂为什么需要下面这个 
#if (Array_Record_FLAG)
    distance = get_two_points_distance(gnss.latitude, gnss.longitude, GPS_GET_LAT[next_point], GPS_GET_LOT[next_point]); // 距离
    azimuth = get_two_points_azimuth(gnss.latitude, gnss.longitude, GPS_GET_LAT[next_point], GPS_GET_LOT[next_point]); // 方位角 (两点连线与正北的夹角)
#endif

    // 数据融合的处理
    // 把IMU规划为顺时针0-(180),逆时针0-(-180)
    // 把GPS规划为正半轴0-(180),负半轴0-(-180)
    // IMU_Navigation_0()------0
    // IMU_Navigation_180_N()------1
    // IMU_Navigation_180_S()------2
    // Single_layer_feedback()------3
    // Double_layer_feedback_0()------4
    //上面这几个函数是最终的角度结算函数，上面算完就给舵机赋值了
       
// 6.28 暂时注释排查警钟长鸣问题

    if (next_point < 1 || next_point == 7) {
        Navigation_mode(0);
    }
    //这里可以看出大部分点是3模式，信标应该也只用3就够了吧?
    if ((next_point >= 1 && next_point < 7) || (next_point > 7 && next_point < 10) || next_point > 10) {
        Navigation_mode(3);
    } else if (next_point == 10) {
        Navigation_mode(2);
    }

    // STEER_Value=PidLocCtrl(&PID_GPS,SERVO_MOTOR_MID+Error);//SD9
    // STEER_Value=PidLocCtrl(&PID_GPS,SERVO_MOTOR_MID-Error);//SD5

    Switching_point_D(); // 距离判据切点----这里要融合信标

/*   
    //去年的任务是绕几个锥桶过几个元素，这下面的代码时采集好点位后的不同元素速度控制
    //信标的元素只有一个这部分应该是不需要的
    if (next_point == 1) {
        Retardation(50, 40, -400); // 反推制动
    } else if (next_point == 2) {
        SPEED_Value = 40;
    } else if (next_point == 3) {
        SPEED_Value = 40;
    } else if (next_point == 4) {
        SPEED_Value = 40;
    } else if (next_point == 5) {
        SPEED_Value = 40;
    } else if (next_point == 6) {
        SPEED_Value = 100;
    } else if (next_point == 7) {
        SPEED_Value = 100;
    } else if (next_point == 8) {
        gpio_set_level(BUZZER_PIN, 1);
//        Task_RZT(67, 70, 0, 'N'); // 打角 速度
        SPEED_Value = 130;
    } else if (next_point == 9) {
        SPEED_Value = 60;
    } else if (next_point == 10) { // 尽头
        SPEED_Value = 50;
    } else if (next_point == 11) {
        SPEED_Value = 150;
    } else if (next_point == 12) {
        SPEED_Value = 100;
    } else if (next_point == 13) {
        SPEED_Value = 100;
    } else if (next_point == 14) { // 坡道后
        SPEED_Value = 100;
    } else if (next_point == 15) { // 停车
        STOP();
    } else { // 默认
        SPEED_Value = 100;
    }
   
*/
#endif
    
    
    
//匹配滤波器
//要预先生成一端标准的Chirp信号？

#if MIC_Mode
        
    //ChripPipei();
    
    if(MIC_rcyflag)//波形与预期足够相似
    {
        Error=Get_mic_Angle();
        printf("\r\nError:%lf\r\n",Error);
    }
    else
    {
        Error=0;
        SPEED_Value=0;
    }
    
    
    
    
    
    
#endif
    
   
    
#if GPS_MIC_Mode
    
    
    
#endif

}

/*
void Task_RZT(int Steer, int16 Motor, int Angle, char Type) // 任务:绕锥桶一圈
{
    if (Angle == 0 && Type == 'N') {
        if (ZT_FLAG == 0) {
            do {
                Control_FLAG = 0;     // 给0隔绝GPS影响
                Steer_set(Steer);   // 绕桶半径大小是由打死角度决定的
                SPEED_Value = Motor; // 闭环
                if (T_M < (-358)) {
                    Control_FLAG = 1; // 重新赋值，打开GPS影响
                    ZT_FLAG = 1;
                }
                gpio_set_level(BUZZER_PIN, 1);
            } while (ZT_FLAG == 0);
            gpio_set_level(BUZZER_PIN, 0);
        }
    } else if (Angle == 0 && Type == 'S') {
        if (ZT_FLAG == 0) {
            do {
                Control_FLAG = 0;     // 给0隔绝GPS影响
                Steer_set(Steer);   // 绕桶半径大小是由打死角度决定的
                SPEED_Value = Motor; // 闭环
                if (T_N > (358)) {
                    Control_FLAG = 1; // 重新赋值，打开GPS影响
                    ZT_FLAG = 1;
                }
                gpio_set_level(BUZZER_PIN, 1);
            } while (ZT_FLAG == 0);
            gpio_set_level(BUZZER_PIN, 0);
        }
    } else if (Angle == 180 && Type == 'N') {
        if (ZT_FLAG == 0) {
            do {
                Control_FLAG = 0;     // 给0隔绝GPS影响
                Steer_set(Steer);   // 绕桶半径大小是由打死角度决定的
                SPEED_Value = Motor; // 闭环
                if (T_N < (-540)) {
                    Control_FLAG = 1; // 重新赋值，打开GPS影响
                    ZT_FLAG = 1;
                }
                gpio_set_level(BUZZER_PIN, 1);
            } while (ZT_FLAG == 0);
            gpio_set_level(BUZZER_PIN, 0);
        }
    }
}
*/

void Switching_point_D() // 切点(距离)判断-----切点标志要改，信标影响下，应该改为踩过信标再切点
{
    if (next_point == 0 || next_point < 1 || next_point == 12) {
        if (distance < 2) { // 切点
            next_point++;
           // printf("\r\n 3-当前点位-%d", next_point);
            //printf("\r\n \\\\\\\\\\\\\\\\\\\\");
        }
    } else if (next_point == 8 || next_point == 9 || next_point == 10) {
        if (distance < 2) { // 切点
            next_point++;
           // printf("\r\n 0.75-当前点位-%d", next_point);
            //printf("\r\n \\\\\\\\\\\\\\\\\\\\");
        }
    } else {
        if (distance < 0.75) { // 切点
            next_point++;
           // printf("\r\n 2-当前点位-%d", next_point);
            //printf("\r\n \\\\\\\\\\\\\\\\\\\\");
        }
    }
}

void Double_layer_feedback_0() // 双层反馈(用于去的时候是0度)
{
    //第一个点azimuth获取到了出发点和目标点的方位角
    // 向北发车
    if (azimuth > 180) { // 将大于180的方向角变为负角
        azimuth -= 360;
    }
    // 向南发车
    azimuth += 180;
    if (azimuth > 180) { // 将大于180的方向角变为负角
        azimuth -= 360;
    }

    if ((azimuth - Daty_Z) > 180) { // 当两角度之差大于180度时，则将差值减去360
        Error = azimuth - PidLocCtrl(&PID_IMU, Daty_Z) - 360; // GPS的方向角-IMU经过PD处理的航向角
       // printf("IMU(0).azimuth:%f,Daty_Z:%f,Error:%f\n", azimuth, Daty_Z, Error); // 如果点位在正北，那么方向角未经过处理的值是0
    } else if ((azimuth - Daty_Z) < -180) { // 当两角度之差小于-180度时，则将差值加上360
        Error = azimuth - PidLocCtrl(&PID_IMU, Daty_Z) + 360; // GPS的方向角-IMU经过PD处理的航向角
       // printf("IMU(0).azimuth:%f,Daty_Z:%f,Error:%f\n", azimuth, Daty_Z, Error); // 如果点位在正北，那么方向角未经过处理的值是0
    } else {
        Error = azimuth - PidLocCtrl(&PID_IMU, Daty_Z); // GPS的方向角-IMU经过PD处理的航向角
       // printf("IMU(0).azimuth:%f,Daty_Z:%f,Error:%f\n", azimuth, Daty_Z, Error); // 如果点位在正北，那么方向角未经过处理的值是0
    }
}

void Double_layer_feedback_180() // 双层反馈(用于返回的时候是180度)
{
    if (azimuth > 180) { // 将大于180的方向角变为负角
        azimuth -= 360;
    }

    if ((azimuth - Daty_Z) > 180) { // 当两角度之差大于180度时，则将差值减去360
        Error = azimuth - PidLocCtrl(&PID_IMU, Daty_Z) - 360; // GPS的方向角-IMU经过PD处理的航向角
        //printf("IMU(180).azimuth:%f,Daty_Z:%f,Error:%f\n", azimuth, Daty_Z, Error); // 如果点位在正北，那么方向角未经过处理的值是0
    } else if ((azimuth - Daty_Z) < -180) { // 当两角度之差小于-180度时，则将差值加上360
        Error = azimuth - PidLocCtrl(&PID_IMU, Daty_Z) + 360; // GPS的方向角-IMU经过PD处理的航向角
        //printf("IMU(180).azimuth:%f,Daty_Z:%f,Error:%f\n", azimuth, Daty_Z, Error); // 如果点位在正北，那么方向角未经过处理的值是0
    } else {
        Error = azimuth - PidLocCtrl(&PID_IMU, Daty_Z); // GPS的方向角-IMU经过PD处理的航向角
       // printf("IMU(180).azimuth:%f,Daty_Z:%f,Error:%f\n", azimuth, Daty_Z, Error); // 如果点位在正北，那么方向角未经过处理的值是0
    }
}

void Single_layer_feedback() // 单层反馈
{
    // 向北发车
    if (azimuth > 180) { // 将大于180的方向角变为负角
        azimuth -= 360;
    }
    // 向南发车
    azimuth += 180;
    if (azimuth > 180) { // 将大于180的方向角变为负角
        azimuth -= 360;
    }
    if ((azimuth - Daty_Z) > 180) { // 当两角度之差大于180度时，则将差值减去360
        Error = azimuth - Daty_Z - 360;
       // printf("GPS.azimuth:%f,Daty_Z:%f,Error:%f\n", azimuth, Daty_Z, Error); // 如果点位在正北，那么方向角未经过处理的值是0
    } else if ((azimuth - Daty_Z) < -180) { // 当两角度之差小于-180度时，则将差值加上360
        Error = azimuth - Daty_Z + 360;
       // printf("GPS.azimuth:%f,Daty_Z:%f,Error:%f\n", azimuth, Daty_Z, Error); // 如果点位在正北，那么方向角未经过处理的值是0
    } else {
        Error = azimuth - Daty_Z;
       // printf("GPS.azimuth:%f,Daty_Z:%f,Error:%f\n", azimuth, Daty_Z, Error); // 如果点位在正北，那么方向角未经过处理的值是0
    }
}

void IMU_Navigation_0() // 0度IMU导航
{
    Error = T_M;
}

void IMU_Navigation_180_N() // 180度IMU导航
{
    Error = (180 + T_M);
}

void IMU_Navigation_180_S() // 180度IMU导航
{
    Error = (-180 + T_M);
}

void Retardation(int X, int16 Y, int16 Z) // 反推制动
{
    if (encoder < X) {
        SPEED_Value = Y;
    } else {
         SPEED_Value = (Z);
    }
}

void STOP() // 停车
{
    if (encoder < 10) {
        while (1) {
            SPEED_Value = 0; // 任务完成，停车！
            Steer_set(SERVO_MOTOR_MID);
            gpio_set_level(BUZZER_PIN, 1);
        }
    } else {
        SPEED_Value = (-300);
    }
}

void Navigation_mode(int TYPE)
{
    if (TYPE == 0) {
        IMU_Navigation_0(); // IMU--0度导航
    } else if (TYPE == 1) {
        IMU_Navigation_180_N(); // 逆时针IMU--180度导航(左转)
    } else if (TYPE == 2) {
        IMU_Navigation_180_S(); // 顺时针IMU--180度导航(右转)
    } else if (TYPE == 3) {
         Single_layer_feedback(); // 单层反馈----没有IMU抑制转向效果
    } else if (TYPE == 4) {
        Double_layer_feedback_0(); // 双层反馈(用于去的时候是0度)
    }
}
