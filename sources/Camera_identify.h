#ifndef __CAMERA_IDENTIFY_H__
#define __CAMERA_IDENTIFY_H__
#include "MCF52259.h"
#include"gpio.h"


/*********************************************************** 
函数名称：SeekCenters
函数功能：提取中线
入口参数：无
出口参数：无  
***********************************************************/
void SeekCenters(void);
/*****************************************
函数名称：VerticalFetch
函数功能：纵向扫线,扫取某列的白变黑，黑变白点
入口参数列数
出口参数：无  
*****************************************/
void VerticalFetch(uint8 rank);
/*******************************************
函数名称：VerticalScan
函数功能：纵向扫线
入口参数无
出口参数：无  
*******************************************/
void VerticalScan(void);
/****************************************
函数名称：ScanHole
函数功能：从近到远找到左右边的第一个缺口，记录缺口的大小，和开始坐标
入口参数无
出口参数：无  
*****************************************/
void ScanHole(void);
/***************************************
函数名称：RepairRightAngleFlag
函数功能：出现直角标记线时，修正补线
入口参数无
出口参数：无  
***************************************/
void RepairRightAngleFlag(void);
/***************************************
函数名称：RepairPassingHole
函数功能：有贯通线和缺口时，修正补线
入口参数无
出口参数：无  
****************************************/
void RepairPassingHole(void);
/****************************************
函数名称：RepairSingle
函数功能：单线后中线修补
入口参数无
出口参数：无  
*****************************************/
void RepairSingle(void);
/****************************************
函数名称：Identify_Picture
函数功能：图像分析
入口参数无
出口参数：无  
*****************************************/
void Identify_Picture(void);


int abs(int i);
#endif