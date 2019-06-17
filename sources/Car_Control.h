#ifndef __CAR_CONTROL_H__
#define __CAR_CONTROL_H__

#include <stdio.h>
////数组行列
extern uint8 line;//列
extern int8  row;//行

////图象数组 
extern uint8 image[ROW][LINE];
extern uint8 SetPoint_L;//左轮的设定速度的编码器反馈值
extern uint8 SetPoint_R;//右轮的设定速度的编码器反馈值
extern int16 DUOJI_SetPoint;
extern int16	PULSE_right;//采集回来的右电机脉冲数
extern int16	PULSE_left;
extern uint8 IsLineL[ROW], IsLineR[ROW];
extern int16 FlagCenter[ROW],FlagR[ROW],FlagL[ROW];
extern uint8 WTB[LINE];
extern uint8 BTW[LINE];
extern uint8 WhiteLine_num,White_num,Most_White_Position,Left_Hole_Position,Left_Hole_Width,Right_Hole_Position,Right_Hole_Width;
extern uint8 m_start,m_end,SingleDirection,PreMode;
extern uint8 BTWmostLeft,BTWLeft,BTWCenter,BTWRight,BTWmostRight,
	 WTBmostLeft,WTBLeft,WTBCenter,WTBRight,WTBmostRight;
extern int abs(int i);
/*********************************************************** 
函数名称：takeTurn
函数功能：最后处理打角函数，避免跑出赛道
入口参数：PD处理后的舵机值
出口参数：打角值  
***********************************************************/
void turn();
void T_change();
#endif