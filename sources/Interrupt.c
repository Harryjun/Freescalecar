/********************************
文件名称：Interrupt中断函数
作者：无协摄像头一队
版本：V1.0
说明：智能车中断函数
修改记录：
2015.5.23整理完成
********************************/

#include"Interrupt.h"
int jishu=0;
int Condition_Scan = 1;
int Pulse_Count = 0;//灯塔脉冲计数
unsigned char image[ROW][LINE];//图像数组
int16 rowCounter=0;//现实行计数
uint8 line=0;
int8  row=0;
 //行采集数组
uint8 get_hang[ROW]=
{
/*
	34,//long
36,41,//1.5m
44,//1.4m
46,48,//1.3m
50,52,54,//1.2m
56,58,60,//1.1m
62,64,66,//1.0m
68,71,74,//0.9m
77,81,85,//0.8m
89,93,97,//0.7m
100,104,108,//0.6m
114,119,124,//0.5m
129,134,139,145,//0.4m
151,157,163,170,//0.3m
177,184,192,200,//0.2m
215,230,248//0.1m
*/
27,//long
29,//1.7m
31,//1.6m
33,//1.5m
36,//1.4m
38,40,//1.3
42,43,//1.2
45,47,//1.1

48,50,52,//1.0m
54,56,58,//0.9m[13-16]

60,62,64,66,//0.8m[17-20]
68,70,72,74,//0.7m[21-24]

76,78,80,82,84,///////0.6m[25-29]
86,88,90,92,95,///////0.5m[30-34]
98,101,104,107,110,///0.4m[35-49]
/*
113,116,119,122,126,130,//0.3m
133,136,139,142,146,150,//0.2m
155,160,165,170,175,180,//0.1m
186,192,198,204,210,215//0m
*/
114,118,122,126,130,//0.3[40-44]
134,138,142,146,150,//0.2[45-49]
156,162,168,174,180,//0.1[50-54]
187,194,201,208,215///0.0[55-59]
};
/*********************************************************** 
函数名称：PIT1_start
函数功能：PIT1启动
入口参数：无
出口参数：无  
***********************************************************/
static void PIT1_start()
{

	MCF_PIT_PCSR(1)|=MCF_PIT_PCSR_EN;
}
/*********************************************************** 
函数名称：PIT1_stop
函数功能：PIT1停止
入口参数：无
出口参数：无  
***********************************************************/
static void PIT1_stop(void)
{

	MCF_PIT_PCSR(1)&=~MCF_PIT_PCSR_EN;
}
/*********************************************************** 
函数名称：DTIM_Init
函数功能：DMA定时器初始化
入口参数：无
出口参数：无 
***********************************************************/
static void DTIM_Init()//频率80M
{
	MCF_INTC0_IMRL&=~MCF_INTC_IMRL_MASKALL;
         MCF_INTC0_IMRL|=MCF_INTC_IMRL_INT_MASK19;//DTIM0
         /*设置优先级*/
         MCF_INTC0_ICR19=MCF_INTC_ICR_IP(6)+MCF_INTC_ICR_IL(2);

  //MCF_DTIM0_DTMR&=~MCF_DTIM_DTMR_RST;
 MCF_DTIM0_DTMR=0;    //清空dtmr寄存器
 MCF_DTIM0_DTER|=MCF_DTIM_DTER_REF;//DMA标志位清零
 MCF_DTIM0_DTMR=MCF_DTIM_DTMR_CE_NONE 
                |MCF_DTIM_DTMR_PS(0)//系统时钟的1分频;
                |MCF_DTIM_DTMR_OM 
                |MCF_DTIM_DTMR_CLK_DIV1 //使用系统时钟sysclk;
                |MCF_DTIM_DTMR_ORRI//输出触发的使能
                |MCF_DTIM_DTMR_FRR;//每次计数到后清零
MCF_DTIM0_DTXMR|=MCF_DTIM_DTXMR_DMAEN;   //DMA询问使能，中断无效
}
/*********************************************************** 
函数名称：DTIM_count
函数功能：DMA延时计时
入口参数：count
出口参数：无 
***********************************************************/
static void DTIM_count(unsigned int count)
{
 MCF_DTIM0_DTRR=count; 
 MCF_DTIM0_DTMR|=MCF_DTIM_DTMR_RST;
}
/*********************************************************** 
函数名称：dma_extreq
函数功能：DMA配置
入口参数：目标地址TRS_BUF   数据字节num
出口参数：无 
***********************************************************/
static void dma_extreq(uint8*TRS_BUF,uint8 num)
{
//DMA_init(TRS_BUF,num);
	MCF_GPIO_PTIPAR = 0x00;   // 配置PTI为普通IO    
    MCF_GPIO_DDRTI = 0x00;    // 设置PTI为输入 
	MCF_DMA0_DSR |= MCF_DMA_DSR_DONE;//状态寄存器//清零所有状态位
	MCF_GPIO_PTCPAR |= MCF_GPIO_PTCPAR_DTIN0_DTOUT0  ;  //输出比较时使用DTin0
	MCF_DMA0_DSR |= MCF_DMA_DSR_DONE;//清除中断标志位
	MCF_SCM_MPR = MCF_SCM_MPR_MPR(0x05);//允许DMA在用户和特权模式下作为主设备
	MCF_SCM_DMAREQC = MCF_SCM_DMAREQC_DMAC0(0x4);//DMA timer0 对应 DMA通道0；
	MCF_DMA_SAR(0)=0x40100034;//MCF_GPIO_SETTI原地址
	MCF_DMA_DAR(0)=(uint32)(TRS_BUF);//目的地址
	MCF_DMA_BCR(0)=num;//字节计数器
	MCF_DMA_DCR(0)=//MCF_DMA_DCR_INT
               MCF_DMA_DCR_DSIZE(1)
              |MCF_DMA_DCR_SSIZE(1)
              |MCF_DMA_DCR_DINC
              |MCF_DMA_DCR_D_REQ
              |MCF_DMA_DCR_CS;
	
//interrupt enable	
	MCF_INTC0_IMRL&=~MCF_INTC_IMRL_MASKALL;
	MCF_INTC0_IMRL&=~MCF_INTC_IMRL_INT_MASK9;//channel 0//中断源码9
	MCF_INTC0_ICR09=MCF_INTC_ICR_IP(6)+MCF_INTC_ICR_IL(6);

	MCF_DMA_DCR(0)|= MCF_DMA_DCR_EEXT;
}
/*********************************************************** 
函数名称：EPORT_irq7_handler
函数功能：外部中断触发函数
入口参数：无
出口参数：无
说明：功能可以自己添加  
***********************************************************/
__declspec(interrupt:0) void EPORT_irq7_handler(void)
{
	Condition_car = Go;
	Clear_IRQ7;
}
/*********************************************************** 
函数名称：EPORT_irq5_handler
函数功能：外部中断检测灯塔函数
入口参数：无
出口参数：无
***********************************************************/
__declspec(interrupt:0) void EPORT_irq5_handler(void)
{
	if(Condition_car == Loading)
	{
		Condition_car = Wait;
	}
	else if(Condition_car == Wait)
	{
		
		Condition_car = Wait;
		MCF_PIT_PMR(0) = (uint16)6250;	//clear timer
	}
	else if(Condition_car == Go)
	{	
		Pulse_Count++;	
	
	}
	Clear_IRQ5;	
}
/*********************************************************** 
函数名称：PIT0_handler
函数功能：编码器计时器
入口参数：无
出口参数：无
说明：5ms测速一次+配合灯塔检测模块计时
***********************************************************/
__declspec(interrupt:0) void PIT0_handler(void)
{
	if(Condition_car == Wait)
	{
		Condition_car = Go;
		MCF_PIT_PCSR(0) |= MCF_PIT_PCSR_PIF;//清中断标志位	
	}
	
	else
	{
		if(Pulse_Count > 4)
		{
			if(Condition_Scan == 0)
				Condition_car = Stop;
		}
		else
		{
			Pulse_Count = 0;
		}
		GPT_Pulse_Counting();
		MCF_PIT_PCSR(0) |= MCF_PIT_PCSR_PIF;//清中断标志位
		if(Count_xuhao == 0)
			PWM_left(IncPIDL(PULSE_left));
		else 
			PWM_right(IncPIDR(PULSE_right));
	}
	
}
/*********************************************************** 
函数名称：EPORT_irq1_handler
函数功能：摄像头场中断
入口参数：无
出口参数：无
***********************************************************/
__declspec(interrupt:0)void EPORT_irq1_handler(void)
{	
	ClearVertical;//清除场中断标志
	PIT1_start();
	line = 0;//行列计数全部初始化
	row = 0;
	rowCounter = 0; 
}
/*********************************************************** 
函数名称：PIT1_handler
函数功能：摄像头场场消影信号延时
入口参数：无
出口参数：无
***********************************************************/
__declspec(interrupt:0) void PIT1_handler(void)
{

	MCF_PIT_PCSR(1) |= MCF_PIT_PCSR_PIF;//清中断标志位
	PIT1_stop();
	EnableInterrupt_Horizontal;//开启行中断
	
}
/*********************************************************** 
函数名称：PIT1_handler
函数功能：摄像头行中断
入口参数：无
出口参数：无
***********************************************************/
__declspec(interrupt:0)void EPORT_irq3_handler(void)
{	
	ClearHorizontal;//清除行中断标志
	rowCounter++;
//	if ((!(rowCounter % ROW_INTER)) && (rowCounter >= ROW_START)&&(rowCounter<=ROW_STOP))//等行采	
	if ((rowCounter==get_hang[row]) && (rowCounter >= ROW_START)&&(rowCounter<=ROW_STOP))//等距采
//	if ((rowCounter >= 30)&&(rowCounter<80)||((rowCounter >= 190)&&(rowCounter<240)))
		{
			delay delay delay delay delay delay delay delay delay delay delay delay delay delay delay delay delay delay
			delay delay delay delay delay delay delay delay delay delay delay delay delay delay delay delay delay delay
     		delay delay delay delay delay delay delay delay delay delay delay delay delay delay delay delay delay delay
			delay delay delay delay delay delay delay delay delay delay delay delay delay delay delay delay delay delay
			delay delay delay delay delay delay delay delay delay delay delay delay delay delay delay delay delay delay 
			delay delay delay delay delay delay delay delay delay delay delay delay delay delay delay delay delay delay
			delay delay delay delay delay delay delay delay delay delay delay delay delay delay delay delay delay delay 
			delay delay delay delay delay delay delay delay delay delay delay delay delay delay delay delay delay delay 
			delay delay delay delay delay delay
			dma_extreq(image[row],160);//136 
			DTIM_Init();               //DMA标志位清零
			DTIM_count(24);   //延时
			row++;
			if (row == ROW)//采集满时将中断关闭
			{
				jishu++;
				DisableInterrupt_Horizontal;	
				DisableInterrupt_Vertical;
				imageReady = COLLECTED;//图像采集完成
			}
		}		
		
}

