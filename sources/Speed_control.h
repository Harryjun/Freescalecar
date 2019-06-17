#ifndef SPEED_CONTROL_H_
#define SPEED_CONTROL_H_
#include "MCF52259.h"
#include"gpio.h"
//��¼��17/0.54//0.42
//���PD    

extern uint16 steer_value;
extern uint8 SetPoint_L;//���ֵ��趨�ٶȵı���������ֵ
extern uint8 SetPoint_R;//���ֵ��趨�ٶȵı���������ֵ
extern int16 DUOJI_SetPoint;

typedef struct PID
{
	int SumError;
	int LastError; //Error[-1]
	int PrevError; //Error[-2]
	int return_value;	
}MPID,MPID1;
static  MPID sPID;
static MPID *sptr = &sPID;  //����ʽPID��������
static MPID1 sPID0;           //λ��ʽPID��������
static MPID1 *sptr0 = &sPID0;
static MPID sPD;           //���PD��������
static MPID *sptr1 = &sPD;


/*********************************************************** 
�������ƣ�PIDInit
�������ܣ�PID��ʼ��
��ڲ�������
���ڲ�������
***********************************************************/
void PIDInit(void);
/*********************************************************** 
�������ƣ�IncPID
�������ܣ����PID����/����ʽ
��ڲ����������������ٶ�ֵ
���ڲ��������PWM������
***********************************************************/
int IncPIDL(int NextPoint);//����ʽ
int IncPIDR(int NextPoint);//����ʽ
/*********************************************************** 
�������ƣ�LocPD
�������ܣ����PD����/λ��ʽ
��ڲ���:Setpoint ���������ڳ���Ұ�ڵ��趨����
	 	 NextPoint ������������ֵ
���ڲ��������PWM������
***********************************************************/
uint16 LocPD(int Setpoint,int NextPoint);
/*********************************************************** 
�������ƣ�PWM_left/PWM_right
�������ܣ��������
��ڲ�����x
���ڲ�������
***********************************************************/
void PWM_left(int16 x);//����ת����
void PWM_right(int16 x);//����ת����
/*********************************************************** 
�������ƣ�PWM_duoji
�������ܣ��������
��ڲ�����x1500����2000��ת
���ڲ�������
***********************************************************/
void PWM_duoji(int16 x);//�������
/*********************************************************** 
�������ƣ�GPT_Pulse_Counting
�������ܣ�GPT����ɼ�
��ڲ�������
���ڲ������� 
***********************************************************/
void GPT_Pulse_Counting();



#endif

