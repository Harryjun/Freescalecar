#ifndef SPEED_CONTROL_H_
#define SPEED_CONTROL_H_
#include "MCF52259.h"
#include"gpio.h"
//记录：17/0.54//0.42
//舵机PD    

extern uint16 steer_value;
extern uint8 SetPoint_L;//左轮的设定速度的编码器反馈值
extern uint8 SetPoint_R;//右轮的设定速度的编码器反馈值
extern int16 DUOJI_SetPoint;

typedef struct PID
{
	int SumError;
	int LastError; //Error[-1]
	int PrevError; //Error[-2]
	int return_value;	
}MPID,MPID1;
static  MPID sPID;
static MPID *sptr = &sPID;  //增量式PID参数申明
static MPID1 sPID0;           //位置式PID参数申明
static MPID1 *sptr0 = &sPID0;
static MPID sPD;           //舵机PD参数申明
static MPID *sptr1 = &sPD;


/*********************************************************** 
函数名称：PIDInit
函数功能：PID初始化
入口参数：无
出口参数：无
***********************************************************/
void PIDInit(void);
/*********************************************************** 
函数名称：IncPID
函数功能：电机PID控制/增量式
入口参数：编码器反馈速度值
出口参数：电机PWM的增量
***********************************************************/
int IncPIDL(int NextPoint);//增量式
int IncPIDR(int NextPoint);//增量式
/*********************************************************** 
函数名称：LocPD
函数功能：舵机PD控制/位置式
入口参数:Setpoint 赛道中线在车视野内的设定坐标
	 	 NextPoint 赛道中线坐标值
出口参数：舵机PWM控制量
***********************************************************/
uint16 LocPD(int Setpoint,int NextPoint);
/*********************************************************** 
函数名称：PWM_left/PWM_right
函数功能：电机调速
入口参数：x
出口参数：无
***********************************************************/
void PWM_left(int16 x);//向左转函数
void PWM_right(int16 x);//向右转函数
/*********************************************************** 
函数名称：PWM_duoji
函数功能：舵机函数
入口参数：x1500摆正2000左转
出口参数：无
***********************************************************/
void PWM_duoji(int16 x);//舵机函数
/*********************************************************** 
函数名称：GPT_Pulse_Counting
函数功能：GPT脉冲采集
入口参数：无
出口参数：无 
***********************************************************/
void GPT_Pulse_Counting();



#endif

