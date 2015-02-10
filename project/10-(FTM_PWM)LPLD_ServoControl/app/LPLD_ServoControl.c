/**
 * --------------����"��������K60�ײ��V3"�Ĺ��̣�LPLD_ServoControl��-----------------
 * @file LPLD_ServoControl.c
 * @version 0.2
 * @date 2013-10-30
 * @brief ����FTMģ���PWM������ܣ�ʵ�ֶ���Ŀ��ơ�
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
 */
#include "common.h"

/****************************************
 ˵����
   CPU_MK60DZ10 & CPU_MK60D10
   *��PTC1����������������ź���
   CPU_MK60F12 & CPU_MK60F15
   *��PTD0����������������ź���
    �۲������˶�״̬
   *��������ź�����Χ0.5ms~2.5ms
    ��(-90��~+90��)
 ****************************************/

//��������
void pwm_init(void);
void delay(uint16);
uint32 angle_to_period(int8);
//��������
FTM_InitTypeDef ftm_init_struct;

void main (void)
{

  //��ʼ��PWM
  pwm_init();
  
  delay(1000);
  //��ʼ����ʱ��ı�Ƕ�Ϊ45��
#if defined(CPU_MK60DZ10) || defined(CPU_MK60D10)
  LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch0, angle_to_period(45));
#elif defined(CPU_MK60F12) || defined(CPU_MK60F15)
  LPLD_FTM_PWM_ChangeDuty(FTM2, FTM_Ch0, angle_to_period(45));
#endif 
  while(1)
  {
  } 
}

/*
 * ��ʼ��FTM��PWM�������
 *
 */
void pwm_init(void)
{
#if defined(CPU_MK60DZ10) || defined(CPU_MK60D10)
  ftm_init_struct.FTM_Ftmx = FTM0;	//ʹ��FTM0ͨ��
  ftm_init_struct.FTM_Mode = FTM_MODE_PWM;	//ʹ��PWMģʽ
  ftm_init_struct.FTM_PwmFreq = 50;	//PWMƵ��50Hz
  
  LPLD_FTM_Init(ftm_init_struct);
  
  LPLD_FTM_PWM_Enable(FTM0, //ʹ��FTM0
                      FTM_Ch0, //ʹ��Ch0ͨ��
                      angle_to_period(0), //��ʼ���Ƕ�0��
                      PTC1, //ʹ��Ch0ͨ����PTC1����
                      ALIGN_LEFT        //���������
                      );  
#elif defined(CPU_MK60F12) || defined(CPU_MK60F15)  
  ftm_init_struct.FTM_Ftmx = FTM2;	//ʹ��FTM0ͨ��
  ftm_init_struct.FTM_Mode = FTM_MODE_PWM;	//ʹ��PWMģʽ
  ftm_init_struct.FTM_PwmFreq = 50;	//PWMƵ��50Hz
  
  LPLD_FTM_Init(ftm_init_struct);
  
  LPLD_FTM_PWM_Enable(FTM2, //ʹ��FTM0
                      FTM_Ch0, //ʹ��Ch0ͨ��
                      angle_to_period(0), //��ʼ���Ƕ�0��
                      PTD0, //ʹ��Ch0ͨ����PTD0����
                      ALIGN_LEFT        //���������
                      );
#endif
}

/*
 * ������ĽǶ�ֵת��ΪPWM��Ƶ��
 * deg--�Ƕ�ֵ(-90��~+90��)
 *   |__-90~+90
 *
 */
uint32 angle_to_period(int8 deg)
{
  uint32 pulse_width = 500+2000*(90+deg)/180;
  return (uint32)(10000*pulse_width/20000);
}

/*
 * ��ʱ����
 */
void delay(uint16 n)
{
  uint16 i;
  while(n--)
  {
    for(i=0; i<5000; i++)
    {
      asm("nop");
    }
  }
}