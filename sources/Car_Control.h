#ifndef __CAR_CONTROL_H__
#define __CAR_CONTROL_H__

#include <stdio.h>
////��������
extern uint8 line;//��
extern int8  row;//��

////ͼ������ 
extern uint8 image[ROW][LINE];
extern uint8 SetPoint_L;//���ֵ��趨�ٶȵı���������ֵ
extern uint8 SetPoint_R;//���ֵ��趨�ٶȵı���������ֵ
extern int16 DUOJI_SetPoint;
extern int16	PULSE_right;//�ɼ��������ҵ��������
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
�������ƣ�takeTurn
�������ܣ�������Ǻ����������ܳ�����
��ڲ�����PD�����Ķ��ֵ
���ڲ��������ֵ  
***********************************************************/
void turn();
void T_change();
#endif