#ifndef __SYSTEM_H__
#define __SYSTEM_H__
#include "MCF52259.h"
#include "gpio.h" 
#include "Debug_tool.h"
#include "Speed_control.h"
#include "Camera_identify.h"
#include "Car_Control.h"
/*********************************************************** 
�������ƣ�system_init
�������ܣ�ϵͳ��ʼ������
��ڲ�����baudrate
���ڲ�������  
***********************************************************/
void system_init(uint32 baudrate);//ϵͳ��ʼ������

#endif