/********************************
�ļ����ƣ�main������
���ߣ���Э����ͷһ��
�汾��V1.0
˵�������ܳ��������ٿ�
�޸ļ�¼��������־�ڴ���д��ÿ���¼��
2015.5.23�������
2015.5.31����sonny_ccd����

��ע��
5110����ģ�飺
1>show_num(x,y,num);
2>LCD_clear_line(num);
3>
���뿪�ع��ܣ�
1>speed2
2>speed1
3>LCD_show_all(&image,160,&center,D_start);
4>
********************************/
#include "support_common.h" /* include peripheral declarations and more */
#if (CONSOLE_IO_SUPPORT || ENABLE_UART_SUPPORT) 
/* Standard IO is only possible if Console or UART support is enabled. */
#include <stdio.h>
#endif
#include "system.h"
//����״̬
int Condition_car = Loading;
int8 Speed_flag = 0;
//���������ʼ��
uint8 SetPoint_L=0;//���ֵ��趨�ٶȵı���������ֵ
uint8 SetPoint_R=0;//���ֵ��趨�ٶȵı���������ֵ
int16 DUOJI_SetPoint=Mid_Steer;
extern int16 Center_T[ROW];
extern int16 Center_R[ROW];
extern int16 Center_L[ROW];
int imageReady = 0;//ͼ��ɼ���ɱ�־λ
void Delay(uint16 ms)
{
   int ii,jj;
   if (ms<1) ms=1;
   for(ii=0;ii<ms;ii++)
     //for(jj=0;jj<50;jj++);     
     for(jj=0;jj<8006;jj++);    
}
//������
int main(void)
{
	int count;
	//ģ���ʼ��
	system_init(38400);
	EnableInterrupts;
	LCD_Fill(0xff); 
    	Delay(2000); 
    	LCD_Fill(0x00);
   	 Delay(1000); 
	LCD_Print(10,0,(uint8 *)"��Э����ͷһ��");
	Delay(1000);
	LCD_Print(10,2,(uint8 *)"2015-7-3");
	Delay(1000);
	OLED_Printf_Str(20,3,(uint8 *)"Waiting");
	Delay(2000);
	while(Condition_car == Loading)//�ϵ�ȴ�����
	{
		OLED_Printf_Str(77,3,(uint8 *)".");
		Delay(500);
		OLED_Printf_Str(83,3,(uint8 *)".");
		Delay(500);
		OLED_Printf_Str(89,3,(uint8 *)".");
		Delay(500);
		OLED_Printf_Str(77,3,(uint8 *)"   ");
		Delay(500);
	}
	EnablePIT(0);  
	while(Condition_car == Wait);//�ȴ���������״̬
	LCD_CLS();
	PWM_right(950);
	PWM_left(950);
	SetPoint_L = 40;
	SetPoint_R = 40;//Ԥ����
	imageReady = COLLECTING;//��ʼ������ͷ
	EnableInterrupt_Vertical;
	ClearVertical;
	//��ʻ״̬
	while(Condition_car == Go)
	{
		if(imageReady==COLLECTED)
		{
		
			if(SW2)
			{
			//	LCD_show_center(&Center_T[0],12,&Center_L[0],&Center_R[0]);
				//Speed_flag = 2;//����ģʽ��	
			}
		
			if(SW4)
			{
				Identify_Picture();
				turn();	
			}
			if(SW3)
			{
			//	SetPoint_L = 20;
			//	SetPoint_R = 20;
		//	OLED_picture(&image[0][0],160,4);
			OLED_show_all(&image[0][0],160,&FlagCenter[0],4);

			}
		
			if(SW1)
			{
			LCD_Print(85,0,(uint8 *)"x:");
			OLED_Print_num_int(98,0,250);
			
			
		//		LCD_set_XY(0,0);
		//		for(count=0;count<ROW;count++)
		//		{
		//			LCD_write_char(IsLineL[count]+48);
		//		}
				//	show_num()
				//Speed_flag = 1;//�ٶ�ģʽ��
			}
			imageReady = COLLECTING;
			EnableInterrupt_Vertical;
			ClearVertical;
		}
	}
}
