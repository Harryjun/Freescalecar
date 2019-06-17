/********************************
�ļ����ƣ�Systemϵͳ����
���ߣ���Э����ͷһ��
�汾��V1.0
˵�������ܳ�ϵͳ�ײ���������
�޸ļ�¼��
2015.5.23�������
********************************/

#include"System.h"

/*********************************************************** 
�������ƣ�PWM_init
�������ܣ�PWM��ʼ������
��ڲ�������
���ڲ�������  
***********************************************************/
static void PWM_init(void)
{
	//function choose  ����pwm����
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
	
	//pwm control register   ����ѡ��			

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
	MCF_PWM_PWMDTY(3)=500&0x00ff;		//78����Ϊ�е�
	
	MCF_PWM_PWMPER(5)=20000&0x00ff;
	MCF_PWM_PWMPER(4)=20000>>8;
	MCF_PWM_PWMDTY(4)=Mid_Steer>>8;		//
	MCF_PWM_PWMDTY(5)=Mid_Steer&0x00ff;		//78����Ϊ�е�
	//pwm start
	MCF_PWM_PWMSDN=MCF_PWM_PWMSDN_IF;
	MCF_PWM_PWME=MCF_PWM_PWME_PWME1
				|MCF_PWM_PWME_PWME3
				|MCF_PWM_PWME_PWME5;
}
/*********************************************************** 
�������ƣ�uart0_init
�������ܣ�UART��ʼ������
��ڲ�����baudrate������
���ڲ�������  
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
static void Delay_ms(uint16 ms)      //��ʱһ����
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
�������ƣ�LCD_init
�������ܣ�5110��ʼ��
��ڲ�������
���ڲ�������  
***********************************************************/
static void LCD_write_byte(unsigned char Data, unsigned char Command)
{
    unsigned char i;
	LCD_CE_0; 	//CE=0	     //5110Ƭѡ��Ч��������������
	
	if(Command==1)                     //DC=Command
	   	LCD_DC_1;
    else
    	LCD_DC_0;    
    
	for(i=0;i<8;i++)				     //����8bit����
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
   	LCD_RST_0;    // RST = 0;     // ����һ����LCD��λ�ĵ͵�ƽ����
    Delay_ms(100);						
	LCD_RST_1;       //  RST = 1;
  	LCD_CE_0;  			// CE = 1;     // �ر�LCD
    Delay_ms(100);
  	LCD_CE_1;         // CE = 0;     // ʹ��LCD
    Delay_ms(100);

    LCD_write_byte(0x21, 0);	// ʹ����չ��������LCDģʽ,ˮƽѰַ
    LCD_write_byte(0xbe, 0);	// ����Һ��ƫ�õ�ѹ
    LCD_write_byte(0x06, 0);	// �¶�У��
    LCD_write_byte(0x13, 0);	// 1:48
    LCD_write_byte(0x20, 0);
    	// ʹ�û������V=0��ˮƽѰַ
    LCD_set_XY(0,0);
    LCD_clear();	           // ����
    LCD_write_byte(0x0c, 0);	// �趨��ʾģʽ��������ʾ
   
    LCD_CE_0;  	// CE = 0;      // ʹ��LCD
    LCD_clear();
} 

/*********************************************************** 
�������ƣ�EPORT_init
�������ܣ��жϳ�ʼ��
��ڲ�������
���ڲ�������  
***********************************************************/
void EPORT_init(void)
{
	//����ѡ��
	MCF_GPIO_PNQPAR = MCF_GPIO_PNQPAR_IRQ1_IRQ1
				     |MCF_GPIO_PNQPAR_IRQ3_IRQ3
				     |MCF_GPIO_PNQPAR_IRQ5_IRQ5
				     |MCF_GPIO_PNQPAR_IRQ7_IRQ7;
	//������ʽ���½��ش���
	MCF_EPORT_EPPAR = MCF_EPORT_EPPAR_EPPA1_FALLING
				 	 |MCF_EPORT_EPPAR_EPPA3_FALLING
				 	 |MCF_EPORT_EPPAR_EPPA5_FALLING
				 	 |MCF_EPORT_EPPAR_EPPA7_FALLING;
	//����Ĵ��������ݷ���Ϊ����			 	
	MCF_EPORT_EPDDR =(uint8)~MCF_EPORT_EPDDR_EPDD1
					 &~MCF_EPORT_EPDDR_EPDD3
					 &~MCF_EPORT_EPDDR_EPDD5
					 &~MCF_EPORT_EPDDR_EPDD7;
	//IRQ�ж�ʹ�ܼĴ������ж�ʹ��
    MCF_EPORT_EPIER =MCF_EPORT_EPIER_EPIE1
					|MCF_EPORT_EPIER_EPIE3
					|MCF_EPORT_EPIER_EPIE5
					|MCF_EPORT_EPIER_EPIE7;
	//״̬(����ģʽʱ��ӳ������״̬)			 	
	MCF_EPORT_EPDR = MCF_EPORT_EPDR_EPD1
	  			    |MCF_EPORT_EPDR_EPD3
	  			    |MCF_EPORT_EPDR_EPD5
	  			    |MCF_EPORT_EPDR_EPD7;
	//�жϱ�־λ����  			 
	MCF_EPORT_EPFR = MCF_EPORT_EPFR_EPF1
				    |MCF_EPORT_EPFR_EPF3
				    |MCF_EPORT_EPFR_EPF5
				    |MCF_EPORT_EPFR_EPF7;				   
	//�ж�ʹ��			   
	MCF_INTC0_IMRL&=~MCF_INTC_IMRL_MASKALL;
	MCF_INTC0_IMRL&=~MCF_INTC_IMRL_INT_MASK1 
				   &~MCF_INTC_IMRL_INT_MASK3
				   &~MCF_INTC_IMRL_INT_MASK5
				  &~MCF_INTC_IMRL_INT_MASK7;
	//�������ȼ�			   
	MCF_INTC0_ICR01=MCF_INTC_ICR_IP(5)+MCF_INTC_ICR_IL(4);
	MCF_INTC0_ICR03=MCF_INTC_ICR_IP(5)+MCF_INTC_ICR_IL(5);
	MCF_INTC0_ICR05=MCF_INTC_ICR_IP(5)+MCF_INTC_ICR_IL(3);
	MCF_INTC0_ICR05=MCF_INTC_ICR_IP(5)+MCF_INTC_ICR_IL(2);

}
/*********************************************************** 
�������ƣ�PIT0_init
�������ܣ�PIT0��ʼ��
��ڲ�������
���ڲ�������  
***********************************************************/
void PIT0_init(void)
{
	//PIT���ƺ�״̬�Ĵ���	
	MCF_PIT_PCSR(0)=MCF_PIT_PCSR_RLD//����λ
				   |MCF_PIT_PCSR_PIF//������Ϊ0�жϱ�־λ
				   |MCF_PIT_PCSR_PIE//�ж�ʹ��λ
				   |MCF_PIT_PCSR_OVW
				   |MCF_PIT_PCSR_PRE(5)//f=f(sys)/2/2^x
				   						//at 80MHz
				   						 //(15-610Hz)(14-1220Hz)(13-4882Hz)
				   						 //(12-9765Hz)(11-19531Hz)(10-39062Hz)(9-78125Hz)
				   |MCF_PIT_PCSR_DBG;
				   //|MCF_PIT_PCSR_DOZE			   
	MCF_PIT_PMR(0)=(uint16)6250;//ģ���Ĵ���
								//1ms PER=5 PMR=1250
	MCF_INTC0_IMRL&=~MCF_INTC_IMRL_MASKALL;
	MCF_INTC0_IMRH&=~MCF_INTC_IMRH_INT_MASK55;//PIT0
	MCF_INTC0_ICR55=MCF_INTC_ICR_IP(2)+MCF_INTC_ICR_IL(2);//PIT0
}
/*********************************************************** 
�������ƣ�PIT1_init
�������ܣ�PIT1��ʼ��
��ڲ�������
���ڲ�������  
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
				   
	MCF_PIT_PMR(1)=5000;//��ʱ2����5000
	MCF_INTC0_IMRL&=~MCF_INTC_IMRL_MASKALL;
	MCF_INTC0_IMRH&=~MCF_INTC_IMRH_INT_MASK56;//PIT1	
	MCF_INTC0_ICR56=MCF_INTC_ICR_IP(6)+MCF_INTC_ICR_IL(6);//PIT1
}

/*********************************************************** 
�������ƣ�GPT_init
�������ܣ�GPTģ���ʼ��
��ڲ�����x    ��Ԥ��Ƶ����
		x=3  T=5ms
���ڲ������� 
***********************************************************/
void GPT_init()//5�����һ�������������ֵ
{
	MCF_GPIO_DDRTC|=0X00;
//	MCF_GPT_GPTSCR1 = 0x00;//gptϵͳ����1
	//1//��GPT3�������ó�GPTģ�������
	MCF_GPT_GPTDDR &=~0x8;//��GPT3�������ó�ΪGPTģ�������
	MCF_GPIO_PTAPAR |= MCF_GPIO_PTAPAR_ICOC3_ICOC3;	
	// 2��ֹGPTģ��
	MCF_GPT_GPTSCR1 &= ~MCF_GPT_GPTSCR1_GPTEN;//GPTʹ��
//					|MCF_GPT_GPTSCR1_TFFCA;//��������λ
	//3//����GPTʱ��
	MCF_GPT_GPTSCR2_PR(3);//���ü�����ʱ�ӷ�Ƶf(gpt)=f(sys)/2/2^x
	//4//���������ۼӹ����½����ۼ�//�ر������ۼ��ж�//
	MCF_GPT_GPTPACTL=0x40;
//	MCF_GPT_GPTIOS=0x00;//����Ϊ���벶׽
//	MCF_GPT_GPTCTL2|=0x80;//�½��ز�׽
	//control register
	
	//5//��ձ�־λ
	MCF_GPT_GPTPAFLG|=MCF_GPT_GPTPAFLG_PAIF;
	
	//6//�������������
	MCF_GPT_GPTPACNT=0X0000;
	//7//��GPT������
	MCF_GPT_GPTCNT=0X0000;
	//////
	MCF_GPT_GPTSCR1 |= MCF_GPT_GPTSCR1_GPTEN;//GPTʹ��
}

/*********************************************************** 
�������ƣ�gpio_init
�������ܣ�IO�ڳ�ʼ��
��ڲ�������
���ڲ�������  
***********************************************************/
static void gpio_init()
{
    //5110�˿�����
    MCF_GPIO_PTEPAR=0X00;
    MCF_GPIO_DDRTE=0XFF;
    
    //����ͷ���ݿ�
    MCF_GPIO_PTIPAR=0x00;//��ͨio��
    MCF_GPIO_DDRTI=0x00;
    
    //���뿪��
    MCF_GPIO_DDRTF &=~MCF_GPIO_DDRTF_DDRTF2;             					            
	MCF_GPIO_DDRTF &=~MCF_GPIO_DDRTF_DDRTF3;              
	MCF_GPIO_DDRTF &=~MCF_GPIO_DDRTF_DDRTF4;
	MCF_GPIO_DDRTF &=~MCF_GPIO_DDRTF_DDRTF5;
    MCF_GPIO_DDRTF |=MCF_GPIO_DDRTF_DDRTF7;//
    MCF_GPIO_DDRTG |=MCF_GPIO_DDRTG_DDRTG1;//A15,A17Ϊ����
    //��־��ģ��
    MCF_GPIO_PTHPAR = 0X00;
    MCF_GPIO_DDRTH = 0XFF;
   	//MCF_GPIO_PORTTH = 0X00;
   	Light1_OFF;
	Light2_OFF;
	Light3_OFF;
	Light4_OFF;
  	//�������ģ��
    MCF_GPIO_PANPAR = 0X00; 
    MCF_GPIO_DDRAN = 0X0F;   
    //beep
    MCF_GPIO_PORTTE |= MCF_GPIO_PORTTE_PORTTE5;
    //��������
    MCF_GPIO_DDRTG &=~MCF_GPIO_DDRTG_DDRTG3;  //��A19��IRQ5��ͨ         					             	
}

/*********************************************************** 
�������ƣ�system_init
�������ܣ�ϵͳ��ʼ������
��ڲ�����baudrate
���ڲ�������  
***********************************************************/
void system_init(uint32 baudrate)//ϵͳ��ʼ������
{
	//�����ʼ��
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