/********************************
文件名称：System系统函数
作者：无协摄像头一队
版本：V1.0
说明：智能车系统底层驱动函数
修改记录：
2015.5.23整理完成
********************************/

#include"System.h"

/*********************************************************** 
函数名称：PWM_init
函数功能：PWM初始化函数
入口参数：无
出口参数：无  
***********************************************************/
static void PWM_init(void)
{
	//function choose  开启pwm功能
	MCF_GPIO_PTCPAR=MCF_GPIO_PTCPAR_DTIN0_PWM0
					|MCF_GPIO_PTCPAR_DTIN1_PWM2
					|MCF_GPIO_PTCPAR_DTIN2_PWM4;

	MCF_GPIO_PTAPAR|=MCF_GPIO_PTAPAR_ICOC0_PWM1
					|MCF_GPIO_PTAPAR_ICOC1_PWM3
					|MCF_GPIO_PTAPAR_ICOC2_PWM5;

    //pwm Polarity Register 
	MCF_PWM_PWMPOL=MCF_PWM_PWMPOL_PPOL1
					|MCF_PWM_PWMPOL_PPOL3
					|MCF_PWM_PWMPOL_PPOL5;
	
	//pwm Clock select register:1-SA or SB  0-A or B
	MCF_PWM_PWMCLK=//MCF_PWM_PWMCLK_PCLK0
				//	|MCF_PWM_PWMCLK_PCLK1
				//	MCF_PWM_PWMCLK_PCLK2
				//	|MCF_PWM_PWMCLK_PCLK3;
					MCF_PWM_PWMCLK_PCLK4
					|MCF_PWM_PWMCLK_PCLK5;
      
    //pwm prescale clock select register bit[6:4]PCKB  bit[2:0]PCKA////10M
	MCF_PWM_PWMPRCLK|=MCF_PWM_PWMPRCLK_PCKA(2);
	MCF_PWM_PWMPRCLK|=MCF_PWM_PWMPRCLK_PCKB(2);

	//pwm center align enable register:1-center-aligned 0-left-aligned
	MCF_PWM_PWMCAE=0x00;	
	
	//pwm scale A&B register Clock SA=Clock A/2*PWMSCLA
	MCF_PWM_PWMSCLA=5;//250;
//	MCF_PWM_PWMSCLB=5;
	
	//pwm channel counter register
	MCF_PWM_PWMCNT0=0;
	MCF_PWM_PWMCNT1=0;
	MCF_PWM_PWMCNT2=0;
	MCF_PWM_PWMCNT3=0;
	MCF_PWM_PWMCNT4=0; 
	MCF_PWM_PWMCNT5=0;
	
	//pwm control register   级联选择			

	MCF_PWM_PWMCTL= MCF_PWM_PWMCTL_CON01 //10khz
				    |MCF_PWM_PWMCTL_CON23//10khz
					|MCF_PWM_PWMCTL_CON45;//					

	MCF_PWM_PWMPER(1)=1000&0x00ff;
	MCF_PWM_PWMPER(0)=1000>>8;
	MCF_PWM_PWMDTY(0)=500>>8;		//
	MCF_PWM_PWMDTY(1)=500&0x00ff;		//
	MCF_PWM_PWMPER(3)=1000&0x00ff;
	MCF_PWM_PWMPER(2)=1000>>8;
	MCF_PWM_PWMDTY(2)=500>>8;		//(40ms)
	MCF_PWM_PWMDTY(3)=500&0x00ff;		//78左右为中点
	
	MCF_PWM_PWMPER(5)=20000&0x00ff;
	MCF_PWM_PWMPER(4)=20000>>8;
	MCF_PWM_PWMDTY(4)=Mid_Steer>>8;		//
	MCF_PWM_PWMDTY(5)=Mid_Steer&0x00ff;		//78左右为中点
	//pwm start
	MCF_PWM_PWMSDN=MCF_PWM_PWMSDN_IF;
	MCF_PWM_PWME=MCF_PWM_PWME_PWME1
				|MCF_PWM_PWME_PWME3
				|MCF_PWM_PWME_PWME5;
}
/*********************************************************** 
函数名称：uart0_init
函数功能：UART初始化函数
入口参数：baudrate波特率
出口参数：无  
***********************************************************/
static void uart0_init(uint32 baudrate)
{	
	uint32 div,SYS_CLOCK;
	SYS_CLOCK=80000000;//80M
//fuction choose UART function	
	MCF_GPIO_PUAPAR=MCF_GPIO_PUAPAR_UTXD0_UTXD0
				   |MCF_GPIO_PUAPAR_URXD0_URXD0;	
//Reset Transmitter Receiver Mode Register
	MCF_UART0_UCR|=(0
				  |MCF_UART_UCR_RESET_TX
	              |MCF_UART_UCR_RESET_RX                                     
	              |MCF_UART_UCR_RESET_MR);             

//(Mode set Register1)No parity,8bit data
	MCF_UART0_UMR1=(0
		          |MCF_UART_UMR_PM_NONE
	              |MCF_UART_UMR_BC_8);
		
//(Mode set Register2)1bit stop
	MCF_UART0_UMR2|=MCF_UART_UMR_CM_NORMAL
				  |MCF_UART_UMR_SB_STOP_BITS_1;
		                
// Set Rx and Tx buad by SYSTERM CLOCK
	MCF_UART0_UCSR=(0
				  |MCF_UART_UCSR_RCS_SYS_CLK
		  		  |MCF_UART_UCSR_TCS_SYS_CLK);
	
//Set UART interrupts	
	MCF_UART0_UIMR=0;     // Unable interrupt
	//MCF_UART_UISR_FFULL_RXRDY;  //FIFO full or receive state
	//|MCF_UART_UISR_TXRDY;   //ready finish state
	//|MCF_UART_UISR_DB    //Break 
	//|MCF_UART_UISR_COS  //Change-of-State
	
	MCF_INTC0_IMRL&=~MCF_INTC_IMRL_MASKALL;
	MCF_INTC0_IMRL&=~MCF_INTC_IMRL_INT_MASK13;//UART0
	MCF_INTC0_ICR13=MCF_INTC_ICR_IP(6)+MCF_INTC_ICR_IL(2);//UART0	
		
//set buad rate
	div=(SYS_CLOCK/32/baudrate);
	MCF_UART0_UBG1=(unsigned char)(div>>8);
	MCF_UART0_UBG2=(unsigned char)(div&0x00ff);
	
//Enable Tx/Rx
	MCF_UART0_UCR=(0
				 |MCF_UART_UCR_TX_ENABLED
				 |MCF_UART_UCR_RX_ENABLED);
}
static void Delay_ms(uint16 ms)      //延时一毫秒
{
   int ii,jj;
   if (ms<1) ms=1;
   for(ii=0;ii<ms;ii++)     
   for(jj=0;jj<6676;jj++)
   {
   	
   }
    //80MHz--1ms  
}  
/*********************************************************** 
函数名称：LCD_init
函数功能：5110初始化
入口参数：无
出口参数：无  
***********************************************************/
static void LCD_write_byte(unsigned char Data, unsigned char Command)
{
    unsigned char i;
	LCD_CE_0; 	//CE=0	     //5110片选有效，允许输入数据
	
	if(Command==1)                     //DC=Command
	   	LCD_DC_1;
    else
    	LCD_DC_0;    
    
	for(i=0;i<8;i++)				     //传送8bit数据
	{ 
			if(Data&0x80)
			 	SDIN_1;
			else
			    SDIN_0;   
			
			Data = (Data <<1);
			SCLK_0; 
     
			SCLK_1;
	}
    LCD_DC_1;           //	DC=1;
    LCD_CE_1; 			//CE=1;  
    SDIN_1;			// Din=1;				  
}
void LCD_init(void)
{
   	LCD_RST_0;    // RST = 0;     // 产生一个让LCD复位的低电平脉冲
    Delay_ms(100);						
	LCD_RST_1;       //  RST = 1;
  	LCD_CE_0;  			// CE = 1;     // 关闭LCD
    Delay_ms(100);
  	LCD_CE_1;         // CE = 0;     // 使能LCD
    Delay_ms(100);

    LCD_write_byte(0x21, 0);	// 使用扩展命令设置LCD模式,水平寻址
    LCD_write_byte(0xbe, 0);	// 设置液晶偏置电压
    LCD_write_byte(0x06, 0);	// 温度校正
    LCD_write_byte(0x13, 0);	// 1:48
    LCD_write_byte(0x20, 0);
    	// 使用基本命令，V=0，水平寻址
    LCD_set_XY(0,0);
    LCD_clear();	           // 清屏
    LCD_write_byte(0x0c, 0);	// 设定显示模式，正常显示
   
    LCD_CE_0;  	// CE = 0;      // 使能LCD
    LCD_clear();
} 

/*********************************************************** 
函数名称：EPORT_init
函数功能：中断初始化
入口参数：无
出口参数：无  
***********************************************************/
void EPORT_init(void)
{
	//功能选择
	MCF_GPIO_PNQPAR = MCF_GPIO_PNQPAR_IRQ1_IRQ1
				     |MCF_GPIO_PNQPAR_IRQ3_IRQ3
				     |MCF_GPIO_PNQPAR_IRQ5_IRQ5
				     |MCF_GPIO_PNQPAR_IRQ7_IRQ7;
	//触发方式：下降沿触发
	MCF_EPORT_EPPAR = MCF_EPORT_EPPAR_EPPA1_FALLING
				 	 |MCF_EPORT_EPPAR_EPPA3_FALLING
				 	 |MCF_EPORT_EPPAR_EPPA5_FALLING
				 	 |MCF_EPORT_EPPAR_EPPA7_FALLING;
	//方向寄存器：数据方向为输入			 	
	MCF_EPORT_EPDDR =(uint8)~MCF_EPORT_EPDDR_EPDD1
					 &~MCF_EPORT_EPDDR_EPDD3
					 &~MCF_EPORT_EPDDR_EPDD5
					 &~MCF_EPORT_EPDDR_EPDD7;
	//IRQ中断使能寄存器：中断使能
    MCF_EPORT_EPIER =MCF_EPORT_EPIER_EPIE1
					|MCF_EPORT_EPIER_EPIE3
					|MCF_EPORT_EPIER_EPIE5
					|MCF_EPORT_EPIER_EPIE7;
	//状态(输入模式时反映该引脚状态)			 	
	MCF_EPORT_EPDR = MCF_EPORT_EPDR_EPD1
	  			    |MCF_EPORT_EPDR_EPD3
	  			    |MCF_EPORT_EPDR_EPD5
	  			    |MCF_EPORT_EPDR_EPD7;
	//中断标志位清零  			 
	MCF_EPORT_EPFR = MCF_EPORT_EPFR_EPF1
				    |MCF_EPORT_EPFR_EPF3
				    |MCF_EPORT_EPFR_EPF5
				    |MCF_EPORT_EPFR_EPF7;				   
	//中断使能			   
	MCF_INTC0_IMRL&=~MCF_INTC_IMRL_MASKALL;
	MCF_INTC0_IMRL&=~MCF_INTC_IMRL_INT_MASK1 
				   &~MCF_INTC_IMRL_INT_MASK3
				   &~MCF_INTC_IMRL_INT_MASK5
				  &~MCF_INTC_IMRL_INT_MASK7;
	//配置优先级			   
	MCF_INTC0_ICR01=MCF_INTC_ICR_IP(5)+MCF_INTC_ICR_IL(4);
	MCF_INTC0_ICR03=MCF_INTC_ICR_IP(5)+MCF_INTC_ICR_IL(5);
	MCF_INTC0_ICR05=MCF_INTC_ICR_IP(5)+MCF_INTC_ICR_IL(3);
	MCF_INTC0_ICR05=MCF_INTC_ICR_IP(5)+MCF_INTC_ICR_IL(2);

}
/*********************************************************** 
函数名称：PIT0_init
函数功能：PIT0初始化
入口参数：无
出口参数：无  
***********************************************************/
void PIT0_init(void)
{
	//PIT控制和状态寄存器	
	MCF_PIT_PCSR(0)=MCF_PIT_PCSR_RLD//重载位
				   |MCF_PIT_PCSR_PIF//计数器为0中断标志位
				   |MCF_PIT_PCSR_PIE//中断使能位
				   |MCF_PIT_PCSR_OVW
				   |MCF_PIT_PCSR_PRE(5)//f=f(sys)/2/2^x
				   						//at 80MHz
				   						 //(15-610Hz)(14-1220Hz)(13-4882Hz)
				   						 //(12-9765Hz)(11-19531Hz)(10-39062Hz)(9-78125Hz)
				   |MCF_PIT_PCSR_DBG;
				   //|MCF_PIT_PCSR_DOZE			   
	MCF_PIT_PMR(0)=(uint16)6250;//模数寄存器
								//1ms PER=5 PMR=1250
	MCF_INTC0_IMRL&=~MCF_INTC_IMRL_MASKALL;
	MCF_INTC0_IMRH&=~MCF_INTC_IMRH_INT_MASK55;//PIT0
	MCF_INTC0_ICR55=MCF_INTC_ICR_IP(2)+MCF_INTC_ICR_IL(2);//PIT0
}
/*********************************************************** 
函数名称：PIT1_init
函数功能：PIT1初始化
入口参数：无
出口参数：无  
***********************************************************/
void PIT1_init()
{

		
	MCF_PIT_PCSR(1)=MCF_PIT_PCSR_RLD
				   |MCF_PIT_PCSR_PIF
				   |MCF_PIT_PCSR_PIE
				   |MCF_PIT_PCSR_OVW
				   |MCF_PIT_PCSR_PRE(4);//fPIT=fsys/2/2^PER
				   						 //at 80MHz
												//0- 40000000Hz
												//1- 20000000Hz
												//2- 10000000Hz
												//3- 5000000Hz
												//4- 2500000Hz -0.4us
												//5- 1250000Hz
												//6- 625000Hz
												//7- 312500Hz
												//8- 156250Hz
												//9- 78125Hz   -12.8us
												//10-39062.5Hz
												//11-19531.25Hz
												//12-9765.625Hz
												//13-4882.8125Hz
												//14-2441.40625Hz
												//15-1220.703125Hz
				   //|MCF_PIT_PCSR_DBG; 
				   //|MCF_PIT_PCSR_DOZE
				   
	MCF_PIT_PMR(1)=5000;//延时2毫秒5000
	MCF_INTC0_IMRL&=~MCF_INTC_IMRL_MASKALL;
	MCF_INTC0_IMRH&=~MCF_INTC_IMRH_INT_MASK56;//PIT1	
	MCF_INTC0_ICR56=MCF_INTC_ICR_IP(6)+MCF_INTC_ICR_IL(6);//PIT1
}

/*********************************************************** 
函数名称：GPT_init
函数功能：GPT模块初始化
入口参数：x    ：预分频因子
		x=3  T=5ms
出口参数：无 
***********************************************************/
void GPT_init()//5毫秒读一次脉冲计数器的值
{
	MCF_GPIO_DDRTC|=0X00;
//	MCF_GPT_GPTSCR1 = 0x00;//gpt系统设置1
	//1//将GPT3引脚设置成GPT模块的引脚
	MCF_GPT_GPTDDR &=~0x8;//将GPT3引脚设置成为GPT模块的引脚
	MCF_GPIO_PTAPAR |= MCF_GPIO_PTAPAR_ICOC3_ICOC3;	
	// 2禁止GPT模块
	MCF_GPT_GPTSCR1 &= ~MCF_GPT_GPTSCR1_GPTEN;//GPT使能
//					|MCF_GPT_GPTSCR1_TFFCA;//快速清零位
	//3//设置GPT时钟
	MCF_GPT_GPTSCR2_PR(3);//设置计数器时钟分频f(gpt)=f(sys)/2/2^x
	//4//设置脉冲累加功能下降沿累加//关闭脉冲累加中断//
	MCF_GPT_GPTPACTL=0x40;
//	MCF_GPT_GPTIOS=0x00;//设置为输入捕捉
//	MCF_GPT_GPTCTL2|=0x80;//下降沿捕捉
	//control register
	
	//5//请空标志位
	MCF_GPT_GPTPAFLG|=MCF_GPT_GPTPAFLG_PAIF;
	
	//6//脉冲计数器清零
	MCF_GPT_GPTPACNT=0X0000;
	//7//清GPT计数器
	MCF_GPT_GPTCNT=0X0000;
	//////
	MCF_GPT_GPTSCR1 |= MCF_GPT_GPTSCR1_GPTEN;//GPT使能
}

/*********************************************************** 
函数名称：gpio_init
函数功能：IO口初始化
入口参数：无
出口参数：无  
***********************************************************/
static void gpio_init()
{
    //5110端口设置
    MCF_GPIO_PTEPAR=0X00;
    MCF_GPIO_DDRTE=0XFF;
    
    //摄像头数据口
    MCF_GPIO_PTIPAR=0x00;//普通io口
    MCF_GPIO_DDRTI=0x00;
    
    //拨码开关
    MCF_GPIO_DDRTF &=~MCF_GPIO_DDRTF_DDRTF2;             					            
	MCF_GPIO_DDRTF &=~MCF_GPIO_DDRTF_DDRTF3;              
	MCF_GPIO_DDRTF &=~MCF_GPIO_DDRTF_DDRTF4;
	MCF_GPIO_DDRTF &=~MCF_GPIO_DDRTF_DDRTF5;
    MCF_GPIO_DDRTF |=MCF_GPIO_DDRTF_DDRTF7;//
    MCF_GPIO_DDRTG |=MCF_GPIO_DDRTG_DDRTG1;//A15,A17为测速
    //标志灯模块
    MCF_GPIO_PTHPAR = 0X00;
    MCF_GPIO_DDRTH = 0XFF;
   	//MCF_GPIO_PORTTH = 0X00;
   	Light1_OFF;
	Light2_OFF;
	Light3_OFF;
	Light4_OFF;
  	//矩阵键盘模块
    MCF_GPIO_PANPAR = 0X00; 
    MCF_GPIO_DDRAN = 0X0F;   
    //beep
    MCF_GPIO_PORTTE |= MCF_GPIO_PORTTE_PORTTE5;
    //灯塔部分
    MCF_GPIO_DDRTG &=~MCF_GPIO_DDRTG_DDRTG3;  //将A19与IRQ5导通         					             	
}

/*********************************************************** 
函数名称：system_init
函数功能：系统初始化函数
入口参数：baudrate
出口参数：无  
***********************************************************/
void system_init(uint32 baudrate)//系统初始化函数
{
	//各项初始化
	PIT0_init();
	PWM_init();  
	gpio_init();
	uart0_init(baudrate);
	LCD_init();
	EPORT_init();
	PIT1_init();
	GPT_init();
	PIDInit();
	LCD_Init();	
}