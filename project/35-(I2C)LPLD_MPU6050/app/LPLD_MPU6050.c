/**
 * --------------����"��������K60�ײ��V3"�Ĺ��̣�LPLD_MPU6050��-----------------
 * @file LPLD_MPU6050.c
 * @version 0.0
 * @date 2013-9-29
 * @brief ���ڸù��̵�����
 *
 * ��Ȩ����:�����������µ��Ӽ������޹�˾
 * http://www.lpld.cn
 * mail:support@lpld.cn
 * Ӳ��ƽ̨:  LPLD K60 Card / LPLD K60 Nano
 *
 * �����̻���"��������K60�ײ��V3"������
 * ���п�Դ�������"lib"�ļ����£��û����ظ��ĸ�Ŀ¼�´��룬
 * �����û������豣����"project"�ļ����£��Թ����������ļ�������
 * �ײ��ʹ�÷���������ĵ��� 
 *
 * ʮ�ָ�л���ļ����߶Եײ��Ĺ��ף�
 * http://www.lpld.cn
 * mail:support@lpld.cn
 * 
 * �ļ�������LPLD_K60�ײ��Ⱥ
 * ���ߣ�Ͷ�ʴ���
 *
 */
#include "common.h"
#include "DEV_MPU6050.h"

int16 accel_x,accel_y,accel_z,gyro_x,gyro_y,gyro_z;
float32 temperature;

void main()
{
  MPU6050_Init();
  
  while(1)
  {
    accel_x = MPU6050_GetResult(ACCEL_XOUT_H);
    accel_y = MPU6050_GetResult(ACCEL_YOUT_H);
    accel_z = MPU6050_GetResult(ACCEL_ZOUT_H);
    gyro_x  = MPU6050_GetResult(GYRO_XOUT_H);
    gyro_y  = MPU6050_GetResult(GYRO_YOUT_H);
    gyro_z  = MPU6050_GetResult(GYRO_ZOUT_H);
    temperature = MPU6050_GetResult(TEMP_OUT_H)/340+36.53;
  } 
}

