#ifndef __DEBUG_TOOL_H__
#define __DEBUG_TOOL_H__
#include "MCF52259.h"
#include "gpio.h"
#define XLevelL		0x00
#define XLevelH		0x10
#define XLevel		((XLevelH&0x0F)*16+XLevelL)
#define Max_Column	128
#define Max_Row		  64
#define	Brightness	0xCF 
#define X_WIDTH 128
#define Y_WIDTH 64

extern int jishu;//图像采集次数
extern int8 row;
extern uint8 line;
extern unsigned char image[ROW][LINE];


//基本函数 
/*********************************************************** 
函数名称：LCD_set_XY
函数功能：设置LCD坐标函数
入口参数：X       ：0－83
          Y       ：0－5
出口参数：无 
***********************************************************/
void LCD_set_XY(unsigned char X, unsigned char Y);//LCD定位
/**********************************************************
函数名称：LCD_clear
函数功能：LCD清屏函数
入口参数：num清除某一行
出口参数：无  
***********************************************************/
void LCD_clear(void);//LCD清屏
void LCD_clear_line(int8 num);//清除某一行 
 /**********************************************************
函数名称：show_num
函数功能：LCD参数显示函数 
入口参数：坐标 x,y   数字 num
出口参数：无  
***********************************************************/
void show_num(int x,int y,int num );//显示数据
/**********************************************************
函数名称：LCD_show_all
函数功能：LCD图像及中线显示函数 
入口参数：image 图像数组   length数组长度  center中线数组 D_start展示部分图像起始行数
出口参数：无  
编写方法：遍历整个数组；
***********************************************************/
void LCD_picture(uint8*I,uint8 length,int8 D_start);//显示图像
void LCD_show_all(uint8*I,uint8 length,int16 *center,int8 D_start);
//显示图像以及中线数据
/*********************************************************** 
函数名称：uart0_putdata
函数功能：虚拟示波器智能车助手----------v1.0
入口参数：数据个数num   数据数组data
出口参数：无  
***********************************************************/
void uart0_putdata(int num,int *data);//发送数据
void LCD_write_char(unsigned char c);
/*********************************************************** 
函数名称：uart0_putpicture
函数功能：发送图像函数串口照相机
入口参数：无
出口参数：无  
***********************************************************/
void uart0_putpicture();//发送图像
/*********************************************************** 
函数名称：uart0_putpicture0
函数功能：发送图像数据智能车助手----------v1.0
入口参数：
出口参数：无  
***********************************************************/
void uart0_putpicture0();
/**********************************************************
函数名称：LCD_show_center
函数功能：LCD中线显示函数 
入口参数：center中线数组
出口参数：无  
编写方法：遍历整个数组；
***********************************************************/
void LCD_show_center(int16 *center,int start,int16 *center_l,int16 *center_r);

//OLD初始化
//初始化
void LCD_Init(void);
//清平函数
void LCD_CLS(void);
//打印字符(自动选择字符或字符串)
void LCD_Print(uint8 x, uint8 y, uint8 ch[]);
//SET POSTION
void LCD_Set_Pos(uint8 x, uint8 y);
//show_num
void OLED_Print_num_int(int8 x, int8 y, int num);
void OLED_Print_num_float(int8 x, int8 y, float num);
//write_str
void OLED_Printf_Str(uint8 x,uint8 y,uint8 ch[]);
//write_hanzi
void OLED_P14x16Str(uint8 x,uint8 y,uint8 ch[]);
//write_dian
void OLED_PutPixel(uint8 x,uint8 y);
//draw_picture
void Draw_BMP(uint8 x0,uint8 y0,uint8 x1,uint8 y1,uint8 bmp[]); 
//write_picture
void OLED_picture(uint8*I,uint8 length,int8 D_start);
void OLED_show_all(uint8*I,uint8 length,int16 *center,int8 D_start);
//全平填充
void LCD_Fill(uint8 dat);
//延时
void LCD_DLY_ms(uint16 ms);

void LCD_WrDat(unsigned char data);



#endif