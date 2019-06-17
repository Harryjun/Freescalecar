/********************************
�ļ����ƣ�Interrupt�жϺ���
���ߣ���Э����ͷһ��
�汾��V1.0
˵�������ܳ��жϺ���
�޸ļ�¼��
2015.5.23�������
********************************/

#include"Interrupt.h"
int jishu=0;
int Condition_Scan = 1;
int Pulse_Count = 0;//�����������
unsigned char image[ROW][LINE];//ͼ������
int16 rowCounter=0;//��ʵ�м���
uint8 line=0;
int8  row=0;
 //�вɼ�����
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
�������ƣ�PIT1_start
�������ܣ�PIT1����
��ڲ�������
���ڲ�������  
***********************************************************/
static void PIT1_start()
{

	MCF_PIT_PCSR(1)|=MCF_PIT_PCSR_EN;
}
/*********************************************************** 
�������ƣ�PIT1_stop
�������ܣ�PIT1ֹͣ
��ڲ�������
���ڲ�������  
***********************************************************/
static void PIT1_stop(void)
{

	MCF_PIT_PCSR(1)&=~MCF_PIT_PCSR_EN;
}
/*********************************************************** 
�������ƣ�DTIM_Init
�������ܣ�DMA��ʱ����ʼ��
��ڲ�������
���ڲ������� 
***********************************************************/
static void DTIM_Init()//Ƶ��80M
{
	MCF_INTC0_IMRL&=~MCF_INTC_IMRL_MASKALL;
         MCF_INTC0_IMRL|=MCF_INTC_IMRL_INT_MASK19;//DTIM0
         /*�������ȼ�*/
         MCF_INTC0_ICR19=MCF_INTC_ICR_IP(6)+MCF_INTC_ICR_IL(2);

  //MCF_DTIM0_DTMR&=~MCF_DTIM_DTMR_RST;
 MCF_DTIM0_DTMR=0;    //���dtmr�Ĵ���
 MCF_DTIM0_DTER|=MCF_DTIM_DTER_REF;//DMA��־λ����
 MCF_DTIM0_DTMR=MCF_DTIM_DTMR_CE_NONE 
                |MCF_DTIM_DTMR_PS(0)//ϵͳʱ�ӵ�1��Ƶ;
                |MCF_DTIM_DTMR_OM 
                |MCF_DTIM_DTMR_CLK_DIV1 //ʹ��ϵͳʱ��sysclk;
                |MCF_DTIM_DTMR_ORRI//���������ʹ��
                |MCF_DTIM_DTMR_FRR;//ÿ�μ�����������
MCF_DTIM0_DTXMR|=MCF_DTIM_DTXMR_DMAEN;   //DMAѯ��ʹ�ܣ��ж���Ч
}
/*********************************************************** 
�������ƣ�DTIM_count
�������ܣ�DMA��ʱ��ʱ
��ڲ�����count
���ڲ������� 
***********************************************************/
static void DTIM_count(unsigned int count)
{
 MCF_DTIM0_DTRR=count; 
 MCF_DTIM0_DTMR|=MCF_DTIM_DTMR_RST;
}
/*********************************************************** 
�������ƣ�dma_extreq
�������ܣ�DMA����
��ڲ�����Ŀ���ַTRS_BUF   �����ֽ�num
���ڲ������� 
***********************************************************/
static void dma_extreq(uint8*TRS_BUF,uint8 num)
{
//DMA_init(TRS_BUF,num);
	MCF_GPIO_PTIPAR = 0x00;   // ����PTIΪ��ͨIO    
    MCF_GPIO_DDRTI = 0x00;    // ����PTIΪ���� 
	MCF_DMA0_DSR |= MCF_DMA_DSR_DONE;//״̬�Ĵ���//��������״̬λ
	MCF_GPIO_PTCPAR |= MCF_GPIO_PTCPAR_DTIN0_DTOUT0  ;  //����Ƚ�ʱʹ��DTin0
	MCF_DMA0_DSR |= MCF_DMA_DSR_DONE;//����жϱ�־λ
	MCF_SCM_MPR = MCF_SCM_MPR_MPR(0x05);//����DMA���û�����Ȩģʽ����Ϊ���豸
	MCF_SCM_DMAREQC = MCF_SCM_DMAREQC_DMAC0(0x4);//DMA timer0 ��Ӧ DMAͨ��0��
	MCF_DMA_SAR(0)=0x40100034;//MCF_GPIO_SETTIԭ��ַ
	MCF_DMA_DAR(0)=(uint32)(TRS_BUF);//Ŀ�ĵ�ַ
	MCF_DMA_BCR(0)=num;//�ֽڼ�����
	MCF_DMA_DCR(0)=//MCF_DMA_DCR_INT
               MCF_DMA_DCR_DSIZE(1)
              |MCF_DMA_DCR_SSIZE(1)
              |MCF_DMA_DCR_DINC
              |MCF_DMA_DCR_D_REQ
              |MCF_DMA_DCR_CS;
	
//interrupt enable	
	MCF_INTC0_IMRL&=~MCF_INTC_IMRL_MASKALL;
	MCF_INTC0_IMRL&=~MCF_INTC_IMRL_INT_MASK9;//channel 0//�ж�Դ��9
	MCF_INTC0_ICR09=MCF_INTC_ICR_IP(6)+MCF_INTC_ICR_IL(6);

	MCF_DMA_DCR(0)|= MCF_DMA_DCR_EEXT;
}
/*********************************************************** 
�������ƣ�EPORT_irq7_handler
�������ܣ��ⲿ�жϴ�������
��ڲ�������
���ڲ�������
˵�������ܿ����Լ����  
***********************************************************/
__declspec(interrupt:0) void EPORT_irq7_handler(void)
{
	Condition_car = Go;
	Clear_IRQ7;
}
/*********************************************************** 
�������ƣ�EPORT_irq5_handler
�������ܣ��ⲿ�жϼ���������
��ڲ�������
���ڲ�������
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
�������ƣ�PIT0_handler
�������ܣ���������ʱ��
��ڲ�������
���ڲ�������
˵����5ms����һ��+��ϵ������ģ���ʱ
***********************************************************/
__declspec(interrupt:0) void PIT0_handler(void)
{
	if(Condition_car == Wait)
	{
		Condition_car = Go;
		MCF_PIT_PCSR(0) |= MCF_PIT_PCSR_PIF;//���жϱ�־λ	
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
		MCF_PIT_PCSR(0) |= MCF_PIT_PCSR_PIF;//���жϱ�־λ
		if(Count_xuhao == 0)
			PWM_left(IncPIDL(PULSE_left));
		else 
			PWM_right(IncPIDR(PULSE_right));
	}
	
}
/*********************************************************** 
�������ƣ�EPORT_irq1_handler
�������ܣ�����ͷ���ж�
��ڲ�������
���ڲ�������
***********************************************************/
__declspec(interrupt:0)void EPORT_irq1_handler(void)
{	
	ClearVertical;//������жϱ�־
	PIT1_start();
	line = 0;//���м���ȫ����ʼ��
	row = 0;
	rowCounter = 0; 
}
/*********************************************************** 
�������ƣ�PIT1_handler
�������ܣ�����ͷ������Ӱ�ź���ʱ
��ڲ�������
���ڲ�������
***********************************************************/
__declspec(interrupt:0) void PIT1_handler(void)
{

	MCF_PIT_PCSR(1) |= MCF_PIT_PCSR_PIF;//���жϱ�־λ
	PIT1_stop();
	EnableInterrupt_Horizontal;//�������ж�
	
}
/*********************************************************** 
�������ƣ�PIT1_handler
�������ܣ�����ͷ���ж�
��ڲ�������
���ڲ�������
***********************************************************/
__declspec(interrupt:0)void EPORT_irq3_handler(void)
{	
	ClearHorizontal;//������жϱ�־
	rowCounter++;
//	if ((!(rowCounter % ROW_INTER)) && (rowCounter >= ROW_START)&&(rowCounter<=ROW_STOP))//���в�	
	if ((rowCounter==get_hang[row]) && (rowCounter >= ROW_START)&&(rowCounter<=ROW_STOP))//�Ⱦ��
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
			DTIM_Init();               //DMA��־λ����
			DTIM_count(24);   //��ʱ
			row++;
			if (row == ROW)//�ɼ���ʱ���жϹر�
			{
				jishu++;
				DisableInterrupt_Horizontal;	
				DisableInterrupt_Vertical;
				imageReady = COLLECTED;//ͼ��ɼ����
			}
		}		
		
}

