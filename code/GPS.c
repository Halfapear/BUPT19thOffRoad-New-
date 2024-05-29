#include "zf_common_headfile.h"
#include "zf_device_gnss.h" // �滻ͷ�ļ�

extern gnss_info_struct gnss; // ʹ��gnss����

int Number = 0; // ���ʱ���

#define FLASH_SECTION_INDEX 0
#define GPS_PAGE_INDEX 10
#define DATA_LENGTH 128
#define RP_MAX 150

#if Int_Record_FLAG

uint32 Target_point[2][150]; // ���ڴ���ɼ���Ŀ�����Ϣ�����ں�����λ�ü���(��һ����γ�ȣ��ڶ����Ǿ���)
uint32 Work_target_array[2][150]; // ���ڽ���flash�ж�ȡ��Ŀ�����������ݸ��ƹ�������������鱻��ֵʱ��������һ�к�Flash���޹�ϵ
uint32 lat; // γ��
uint32 lot; // ����

void GPS_Record_flash() { // ���ɼ��ĵ�λ��¼����������������GPS_FLASH
    static int NUM = 0; // �ɼ���Ĵ���

    lat = gnss.latitude * 1000000; // γ��*100��(GPS�ش�����ֻ�к�6λ��Ч)
    lot = gnss.longitude * 1000000; // ����*100��(GPS�ش�����ֻ�к�6λ��Ч)

    flash_union_buffer[Number].uint32_type = lat; // ��γ������ǿ��ת���󴢴���FLASH ���������ݻ����� (������Ϊż��ʱ��γ������)
    ips200_show_string(0, 16 * 0, "R:");
    ips200_show_uint(50, 16 * 0, flash_union_buffer[Number].uint32_type, 10);
    ips200_show_uint(180, 16 * 0, Number, 3);
    printf("\r\n������γ������:%d", (int)flash_union_buffer[Number].uint32_type);

    Number++; // �����±�+1,�л����澭��

    flash_union_buffer[Number].uint32_type = lot; // ����������ǿ��ת���󴢴���FLASH ���������ݻ����� (������Ϊ����ʱ�Ǿ�������)
    ips200_show_uint(50, 16 * 1, flash_union_buffer[Number].uint32_type, 10);
    ips200_show_uint(180, 16 * 1, Number, 3);
    printf("\r\n��������������:%d", (int)flash_union_buffer[Number].uint32_type);

    Number++; // �����±�+1,�л�����γ��

    if (flash_check(FLASH_SECTION_INDEX, GPS_PAGE_INDEX)) { // �ж�Flash�Ƿ������� : �����ݷ���1�������ݷ���0
        flash_erase_page(FLASH_SECTION_INDEX, GPS_PAGE_INDEX); // ����Flash����
    }

    flash_write_page(FLASH_SECTION_INDEX, GPS_PAGE_INDEX, (uint32 *)flash_union_buffer, Number); // ��������������д�뵽ָ��Flash ������ҳ��

    NUM++;

    if (NUM > RP_MAX) { // ����ɼ���������ڹ涨���������ֵ������������1
        NUM = 1;
    }

    ips200_show_uint(200, 0, NUM, 3); // ��ʾ�Ѳɼ��ĵ���
}

void GPS_Flash_use() { // ��GPS_FLASH���������¶��ػ���������ֵ������
    flash_buffer_clear(); // ��ջ�����

    if (flash_check(FLASH_SECTION_INDEX, GPS_PAGE_INDEX)) { // �ж�Flash�Ƿ������� : �����ݷ���1�������ݷ���0
        flash_read_page(FLASH_SECTION_INDEX, GPS_PAGE_INDEX, (uint32 *)flash_union_buffer, DATA_LENGTH); // �����ݴ�FLASHָ������ҳ����뵽������

        int TG = 0; // �����л���ά�����±�

        for (int data = 0; data < DATA_LENGTH; data++) {
            Target_point[0][TG] = flash_union_buffer[data].uint32_type; // γ��
            data++;
            Target_point[1][TG] = flash_union_buffer[data].uint32_type; // ����
            TG++; // �±�+1
        }
        printf("\r\n�ɹ���GPS_FLASHȡ������\n");
    }
}
#endif

// GPS_Work_array ��������Ҫ�����ǽ�Ŀ������ (Target_point) �е����ݸ��Ƶ�����Ŀ������ (Work_target_array) �У��Ա��ں����ļ�����ʹ�á���һ����������Ϊ��ԭʼ������ȡ�����и��룬�Ӷ������Flash��ֱ�Ӳ�����������ݷ��ʵ�Ч�ʺͰ�ȫ��

void GPS_Work_array() { // ���ڸ���FLASH,ʵ�ʲμӼ��������
    for (int NUM = 0; NUM < (ARRAY_Element_Calculation() / 2); NUM++) {
        Work_target_array[0][NUM] = Target_point[0][NUM];
        Work_target_array[1][NUM] = Target_point[1][NUM];

        printf("\r\n����-%d,��������γ������-%d,�������龭������-%d", NUM + 1, (int)Work_target_array[0][NUM], (int)Work_target_array[1][NUM]);
        system_delay_ms(50);
    }
}

void GPS_param_t_init() { // GPS������ʼ��
    if (flash_check(FLASH_SECTION_INDEX, GPS_PAGE_INDEX)) { // �ж�Flash�Ƿ������� : �����ݷ���1�������ݷ���0
        GPS_Flash_use(); // ��GPS_FLASH���������¶��ػ���������ֵ������
        GPS_Work_array(); // ��Ŀ����������ݸ���һ�ݵ�����Ŀ������(������֮��GPS�ɵ㳹�׽���,���Ը���FLASH)
    } else {
        printf("\r\nGPSΪ�����趨ֵ(ԭʼֵ)");
    }
}

// �ܻ���һ��show
void GPS_SHOW() { // GPS��Ϣ��ʾ
    // ÿ���ַ���ռ8����,��ռ16������
    ips200_show_string(0, 16 * 0, "state:"); ips200_show_uint(90, 16 * 0, gnss.state, 5);
    ips200_show_string(0, 16 * 1, "direction:"); ips200_show_float(90, 16 * 1, gnss.direction, 4, 6);
    ips200_show_string(0, 16 * 2, "latitude:"); ips200_show_float(90, 16 * 2, gnss.latitude, 4, 6);
    ips200_show_string(0, 16 * 3, "longitude:"); ips200_show_float(90, 16 * 3, gnss.longitude, 4, 6);
    ips200_show_string(0, 16 * 4, "yaw:"); ips200_show_float(90, 16 * 4, Daty_Z, 4, 6);
}

#if Float_Record_FLAG

float Target_point[2][150]; // ���ڴ���ɼ���Ŀ�����Ϣ�����ں�����λ�ü���(��һ����γ�ȣ��ڶ����Ǿ���)
float Work_target_array[2][150]; // ���ڽ���flash�ж�ȡ��Ŀ�����������ݸ��ƹ�������������鱻��ֵʱ��������һ�к�Flash���޹�ϵ
double lat; // γ��
double lot; // ����

void GPS_Record_flash() { // ���ɼ��ĵ�λ��¼����������������GPS_FLASH
    static int NUM = 0; // �ɼ���Ĵ���

    lat = gnss.latitude; // ��ȡγ��
    lot = gnss.longitude; // ��ȡ����

    flash_union_buffer[Number].float_type = lat; // ��γ������ǿ��ת���󴢴���FLASH ���������ݻ����� (������Ϊż��ʱ��γ������)
    ips200_show_string(0, 16 * 0, "R:");
    ips200_show_float(50, 16 * 0, flash_union_buffer[Number].float_type, 3, 6);
    ips200_show_uint(180, 16 * 0, Number, 3);
    printf("\r\n������γ������:%f", flash_union_buffer[Number].float_type);

    Number++; // �����±�+1,�л����澭��

    flash_union_buffer[Number].float_type = lot; // ����������ǿ��ת���󴢴���FLASH ���������ݻ����� (������Ϊ����ʱ�Ǿ�������)
    ips200_show_float(50, 16 * 1, flash_union_buffer[Number].float_type, 3, 6);
    ips200_show_uint(180, 16 * 1, Number, 3);
    printf("\r\n��������������:%f", flash_union_buffer[Number].float_type);

    Number++; // �����±�+1,�л�����γ��

    if (flash_check(FLASH_SECTION_INDEX, GPS_PAGE_INDEX)) { // �ж�Flash�Ƿ������� : �����ݷ���1�������ݷ���0
        flash_erase_page(FLASH_SECTION_INDEX, GPS_PAGE_INDEX); // ����Flash����
    }

    flash_write_page(FLASH_SECTION_INDEX, GPS_PAGE_INDEX, (uint32 *)flash_union_buffer, Number); // ��������������д�뵽ָ��Flash ������ҳ��

    NUM++;

    if (NUM > RP_MAX) { // ����ɼ���������ڹ涨���������ֵ������������1
        NUM = 1;
    }

    ips200_show_uint(200, 0, NUM, 3); // ��ʾ�Ѳɼ��ĵ���
}

void GPS_Flash_use() { // ��GPS_FLASH���������¶��ػ���������ֵ������
    flash_buffer_clear(); // ��ջ�����

    if (flash_check(FLASH_SECTION_INDEX, GPS_PAGE_INDEX)) { // �ж�Flash�Ƿ������� : �����ݷ���1�������ݷ���0
        flash_read_page(FLASH_SECTION_INDEX, GPS_PAGE_INDEX, (uint32 *)flash_union_buffer, DATA_LENGTH); // �����ݴ�FLASHָ������ҳ����뵽������

        int TG = 0; // �����л���ά�����±�

        for (int data = 0; data < DATA_LENGTH; data++) {
            Target_point[0][TG] = flash_union_buffer[data].float_type; // γ������
            data++;
            Target_point[1][TG] = flash_union_buffer[data].float_type; // ��������
            TG++;
        }
        printf("\r\n�ɹ���GPS_FLASHȡ������\n");
    }
}
#endif

#if Double_Record_FLAG

double Target_point[2][150]; // ���ڴ���ɼ���Ŀ�����Ϣ�����ں�����λ�ü���(��һ����γ�ȣ��ڶ����Ǿ���)
double Work_target_array[2][150]; // ���ڽ���flash�ж�ȡ��Ŀ�����������ݸ��ƹ�������������鱻��ֵʱ��������һ�к�Flash���޹�ϵ
double lat; // γ��
double lot; // ����

uint32 I_lat; // double����γ��������������
float F_lat; // double����γ������С������(ǿ��ת��Ϊfloat)

uint32 I_lot; // double���;���������������
float F_lot; // double���;�������С������(ǿ��ת��Ϊfloat)

void GPS_Record_flash() { // ���ɼ��ĵ�λ��¼����������������GPS_FLASH
    static int NUM = 0; // �ɼ���Ĵ���

    lat = gnss.latitude; // ��ȡγ��
    lot = gnss.longitude; // ��ȡ����

    I_lat = (uint32)(lat); // ��ȡγ����������
    F_lat = lat - I_lat; // ��ȡγ��С������

    I_lot = (uint32)(lot); // ��ȡ������������
    F_lot = lot - I_lot; // ��ȡ����С������

    ips200_show_string(0, 16 * 0, "R:");

    flash_union_buffer[Number].uint32_type = I_lat; // ������γ�����ݴ��뻺����
    ips200_show_uint(50, 16 * 0, flash_union_buffer[Number].uint32_type, 5);
    ips200_show_uint(150, 16 * 0, Number, 3);

    Number++;

    flash_union_buffer[Number].float_type = F_lat; // ������γ�����ݴ��뻺����
    ips200_show_float(50, 16 * 1, flash_union_buffer[Number].float_type, 3, 6);
    ips200_show_uint(150, 16 * 1, Number, 3);

    ips200_show_float(50, 16 * 2, gnss.latitude, 3, 6); // ��ʾ����γ��

    Number++; // �����±�+1,�л��洢����

    flash_union_buffer[Number].uint32_type = I_lot; // �������������ݴ��뻺����
    ips200_show_uint(50, 16 * 4, flash_union_buffer[Number].uint32_type, 5);
    ips200_show_uint(150, 16 * 4, Number, 3);

    Number++;

    flash_union_buffer[Number].float_type = F_lot; // �����㾭�����ݴ��뻺����
    ips200_show_float(50, 16 * 5, flash_union_buffer[Number].float_type, 3, 6);
    ips200_show_uint(150, 16 * 5, Number, 3);

    ips200_show_float(50, 16 * 6, gnss.longitude, 3, 6); // ��ʾ��������

    Number++; // �����±�+1,�л��洢γ��

    if (flash_check(FLASH_SECTION_INDEX, GPS_PAGE_INDEX)) { // �ж�Flash�Ƿ������� : �����ݷ���1�������ݷ���0
        flash_erase_page(FLASH_SECTION_INDEX, GPS_PAGE_INDEX); // ����Flash����
    }

    flash_write_page(FLASH_SECTION_INDEX, GPS_PAGE_INDEX, (uint32 *)flash_union_buffer, Number); // ������������д��Flash

    NUM++;

    if (NUM > RP_MAX) { // ����ɼ���������ڹ涨���������ֵ������������1
        NUM = 1;
    }

    ips200_show_uint(210, 0, NUM, 3); // ��ʾ�Ѳɼ��ĵ���
}

void GPS_Flash_use() { // ��GPS_FLASH���������¶��ػ���������ֵ������
    flash_buffer_clear(); // ��ջ�����

    if (flash_check(FLASH_SECTION_INDEX, GPS_PAGE_INDEX)) { // �ж�Flash�Ƿ������� : �����ݷ���1�������ݷ���0
        flash_read_page(FLASH_SECTION_INDEX, GPS_PAGE_INDEX, (uint32 *)flash_union_buffer, DATA_LENGTH); // �����ݴ�FLASHָ������ҳ����뵽������

        int TG = 0; // �����л���ά�����±�

        for (int data = 0; data < DATA_LENGTH; data++) {
            Target_point[0][TG] = flash_union_buffer[data].uint32_type + flash_union_buffer[data + 1].float_type; // γ��
            data = data + 2;
            Target_point[1][TG] = flash_union_buffer[data].uint32_type + flash_union_buffer[data + 1].float_type; // ����
            data = data + 1;
            TG++; // �±�+1
        }
    }
}
#endif
