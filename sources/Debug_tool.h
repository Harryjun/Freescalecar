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

extern int jishu;//ͼ��ɼ�����
extern int8 row;
extern uint8 line;
extern unsigned char image[ROW][LINE];


//�������� 
/*********************************************************** 
�������ƣ�LCD_set_XY
�������ܣ�����LCD���꺯��
��ڲ�����X       ��0��83
          Y       ��0��5
���ڲ������� 
***********************************************************/
void LCD_set_XY(unsigned char X, unsigned char Y);//LCD��λ
/**********************************************************
�������ƣ�LCD_clear
�������ܣ�LCD��������
��ڲ�����num���ĳһ��
���ڲ�������  
***********************************************************/
void LCD_clear(void);//LCD����
void LCD_clear_line(int8 num);//���ĳһ�� 
 /**********************************************************
�������ƣ�show_num
�������ܣ�LCD������ʾ���� 
��ڲ��������� x,y   ���� num
���ڲ�������  
***********************************************************/
void show_num(int x,int y,int num );//��ʾ����
/**********************************************************
�������ƣ�LCD_show_all
�������ܣ�LCDͼ��������ʾ���� 
��ڲ�����image ͼ������   length���鳤��  center�������� D_startչʾ����ͼ����ʼ����
���ڲ�������  
��д�����������������飻
***********************************************************/
void LCD_picture(uint8*I,uint8 length,int8 D_start);//��ʾͼ��
void LCD_show_all(uint8*I,uint8 length,int16 *center,int8 D_start);
//��ʾͼ���Լ���������
/*********************************************************** 
�������ƣ�uart0_putdata
�������ܣ�����ʾ�������ܳ�����----------v1.0
��ڲ��������ݸ���num   ��������data
���ڲ�������  
***********************************************************/
void uart0_putdata(int num,int *data);//��������
void LCD_write_char(unsigned char c);
/*********************************************************** 
�������ƣ�uart0_putpicture
�������ܣ�����ͼ�������������
��ڲ�������
���ڲ�������  
***********************************************************/
void uart0_putpicture();//����ͼ��
/*********************************************************** 
�������ƣ�uart0_putpicture0
�������ܣ�����ͼ���������ܳ�����----------v1.0
��ڲ�����
���ڲ�������  
***********************************************************/
void uart0_putpicture0();
/**********************************************************
�������ƣ�LCD_show_center
�������ܣ�LCD������ʾ���� 
��ڲ�����center��������
���ڲ�������  
��д�����������������飻
***********************************************************/
void LCD_show_center(int16 *center,int start,int16 *center_l,int16 *center_r);

//OLD��ʼ��
//��ʼ��
void LCD_Init(void);
//��ƽ����
void LCD_CLS(void);
//��ӡ�ַ�(�Զ�ѡ���ַ����ַ���)
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
//ȫƽ���
void LCD_Fill(uint8 dat);
//��ʱ
void LCD_DLY_ms(uint16 ms);

void LCD_WrDat(unsigned char data);



#endif