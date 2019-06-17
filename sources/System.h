#ifndef __SYSTEM_H__
#define __SYSTEM_H__
#include "MCF52259.h"
#include "gpio.h" 
#include "Debug_tool.h"
#include "Speed_control.h"
#include "Camera_identify.h"
#include "Car_Control.h"
/*********************************************************** 
函数名称：system_init
函数功能：系统初始化函数
入口参数：baudrate
出口参数：无  
***********************************************************/
void system_init(uint32 baudrate);//系统初始化函数

#endif