
//5110��������
#define SCLK_1                MCF_GPIO_PORTTE|=MCF_GPIO_PORTTE_PORTTE4 //A4 
#define SCLK_0                MCF_GPIO_PORTTE&=~MCF_GPIO_PORTTE_PORTTE4//A4
#define SDIN_1                MCF_GPIO_PORTTE|=MCF_GPIO_PORTTE_PORTTE3//A3
#define SDIN_0                MCF_GPIO_PORTTE&=~MCF_GPIO_PORTTE_PORTTE3//A3
#define LCD_DC_1              MCF_GPIO_PORTTE|=MCF_GPIO_PORTTE_PORTTE2//A2
#define LCD_DC_0              MCF_GPIO_PORTTE&=~MCF_GPIO_PORTTE_PORTTE2//A2
#define LCD_CE_1             MCF_GPIO_PORTTE|=MCF_GPIO_PORTTE_PORTTE0//A0
#define LCD_CE_0             MCF_GPIO_PORTTE&=~MCF_GPIO_PORTTE_PORTTE0//A0 
#define LCD_RST_1              MCF_GPIO_PORTTE|=MCF_GPIO_PORTTE_PORTTE1//A1
#define LCD_RST_0             MCF_GPIO_PORTTE&=~MCF_GPIO_PORTTE_PORTTE1//A1

//OLD����
#define SCLK_1                MCF_GPIO_PORTTE|=MCF_GPIO_PORTTE_PORTTE4 //A4 
#define SCLK_0                MCF_GPIO_PORTTE&=~MCF_GPIO_PORTTE_PORTTE4//A4
#define SET_LCD_SCL                MCF_GPIO_PORTTE|=MCF_GPIO_PORTTE_PORTTE0//A3
#define CLR_LCD_SCL                MCF_GPIO_PORTTE&=~MCF_GPIO_PORTTE_PORTTE0//A3
#define SET_LCD_SDA              MCF_GPIO_PORTTE|=MCF_GPIO_PORTTE_PORTTE1//A2
#define CLR_LCD_SDA              MCF_GPIO_PORTTE&=~MCF_GPIO_PORTTE_PORTTE1//A2
#define SET_LCD_DC             MCF_GPIO_PORTTE|=MCF_GPIO_PORTTE_PORTTE3//A0
#define CLR_LCD_DC             MCF_GPIO_PORTTE&=~MCF_GPIO_PORTTE_PORTTE3//A0 
#define SET_LCD_RST              MCF_GPIO_PORTTE|=MCF_GPIO_PORTTE_PORTTE2//A1
#define CLR_LCD_RST             MCF_GPIO_PORTTE&=~MCF_GPIO_PORTTE_PORTTE2//A1

   


//���뿪��
#define SW1                !(MCF_GPIO_SETTF&MCF_GPIO_SETTF_SETTF6)//A10
#define SW4                !(MCF_GPIO_SETTF&MCF_GPIO_SETTF_SETTF3)//A11
#define SW3                !(MCF_GPIO_SETTF&MCF_GPIO_SETTF_SETTF4)//A12
#define SW2                !(MCF_GPIO_SETTF&MCF_GPIO_SETTF_SETTF5)//A13
//Light
#define Light1_ON    MCF_GPIO_PORTTH|=MCF_GPIO_PORTTH_PORTTH4 //D4 
#define Light1_OFF   MCF_GPIO_PORTTH&=~MCF_GPIO_PORTTH_PORTTH4//D4
#define Light2_ON    MCF_GPIO_PORTTH|=MCF_GPIO_PORTTH_PORTTH5 //D5 
#define Light2_OFF   MCF_GPIO_PORTTH&=~MCF_GPIO_PORTTH_PORTTH5//D5
#define Light3_ON    MCF_GPIO_PORTTH|=MCF_GPIO_PORTTH_PORTTH2 //D6
#define Light3_OFF   MCF_GPIO_PORTTH&=~MCF_GPIO_PORTTH_PORTTH2//D6
#define Light4_ON    MCF_GPIO_PORTTH|=MCF_GPIO_PORTTH_PORTTH3 //D7 
#define Light4_OFF   MCF_GPIO_PORTTH&=~MCF_GPIO_PORTTH_PORTTH3//D7

//BEEP
#define Beep_OFF MCF_GPIO_PORTTE |= MCF_GPIO_PORTTE_PORTTE5 //A5
#define Beep_ON MCF_GPIO_PORTTE &=~ MCF_GPIO_PORTTE_PORTTE5 //A5
//�ж�
#define EnableInterrupts asm { move.w SR,D0; andi.l #0xF8FF,D0; move.w D0,SR;  }
//����ͷ��������
#define ROW_START 25//30//��Ч�п�ʼ  
#define ROW_STOP 250//230//��Ч�н��� 
#define LINE 160//136//Ҫ�ɼ�������
#define ROW  60//Ҫ�ɼ�������
#define ROW_INTER 3//��λ������
#define TrackWidth 120//��28�п��//140//�������
#define Mid_Steer 1400//�����ֵ
////�ж�
#define DisableInterrupts  asm(move.w  #0x2700,sr) 
#define EnableInterrupt_Vertical (MCF_EPORT_EPIER |= MCF_EPORT_EPIER_EPIE1)//�����ж�
#define EnableInterrupt_Horizontal (MCF_EPORT_EPIER |= MCF_EPORT_EPIER_EPIE3)//�����ж�
#define DisableInterrupt_Vertical (MCF_EPORT_EPIER &= ~MCF_EPORT_EPIER_EPIE1)//�س��ж�
#define DisableInterrupt_Horizontal (MCF_EPORT_EPIER &= ~MCF_EPORT_EPIER_EPIE3)//�����ж�
#define ClearVertical (MCF_EPORT_EPFR |= MCF_EPORT_EPFR_EPF1)//���жϱ�־λ����
#define ClearHorizontal (MCF_EPORT_EPFR |= MCF_EPORT_EPFR_EPF3)//���жϱ�־λ����
////ͼ��ɼ���־
#define COLLECTING 0//ͼ��ɼ���
#define COLLECTED 1//ͼ��ɼ����
#define delay asm{ nop }//nop��ʱ
#define PORT MCF_GPIO_SETTI
//�������궨��
#define L_GPT_COUNT_OPEN	MCF_GPIO_PORTTF|=MCF_GPIO_PORTTF_PORTTF7//A15
#define L_GPT_COUNT_OFF		MCF_GPIO_PORTTF&=~MCF_GPIO_PORTTF_PORTTF7//A15
#define R_GPT_COUNT_OPEN	MCF_GPIO_PORTTG|=MCF_GPIO_PORTTG_PORTTG1//A17
#define R_GPT_COUNT_OFF		MCF_GPIO_PORTTG&=~MCF_GPIO_PORTTG_PORTTG1//A17

//�ⲿ�жϱ�־λ
#define Clear_IRQ5 (MCF_EPORT_EPFR |= MCF_EPORT_EPFR_EPF5)
#define Clear_IRQ7 (MCF_EPORT_EPFR |= MCF_EPORT_EPFR_EPF7)
//��ʱ������
#define EnablePIT(number) (MCF_PIT_PCSR(number)|=0x0001)
#define DisablePIT(number)  (MCF_PIT_PCSR(number)&=~(0x0001))

//����״̬
#define Go 1
#define Wait 0
#define Stop 2
#define Loading 3
