
//5110基本定义
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

//OLD定义
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

   


//拨码开关
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
//中断
#define EnableInterrupts asm { move.w SR,D0; andi.l #0xF8FF,D0; move.w D0,SR;  }
//摄像头部分声明
#define ROW_START 25//30//有效行开始  
#define ROW_STOP 250//230//有效行结束 
#define LINE 160//136//要采集的列数
#define ROW  60//要采集的行数
#define ROW_INTER 3//单位是行数
#define TrackWidth 120//第28行宽度//140//赛道宽度
#define Mid_Steer 1400//舵机中值
////中断
#define DisableInterrupts  asm(move.w  #0x2700,sr) 
#define EnableInterrupt_Vertical (MCF_EPORT_EPIER |= MCF_EPORT_EPIER_EPIE1)//开场中断
#define EnableInterrupt_Horizontal (MCF_EPORT_EPIER |= MCF_EPORT_EPIER_EPIE3)//开行中断
#define DisableInterrupt_Vertical (MCF_EPORT_EPIER &= ~MCF_EPORT_EPIER_EPIE1)//关场中断
#define DisableInterrupt_Horizontal (MCF_EPORT_EPIER &= ~MCF_EPORT_EPIER_EPIE3)//关行中断
#define ClearVertical (MCF_EPORT_EPFR |= MCF_EPORT_EPFR_EPF1)//场中断标志位清零
#define ClearHorizontal (MCF_EPORT_EPFR |= MCF_EPORT_EPFR_EPF3)//行中断标志位清零
////图象采集标志
#define COLLECTING 0//图像采集中
#define COLLECTED 1//图像采集完成
#define delay asm{ nop }//nop延时
#define PORT MCF_GPIO_SETTI
//编码器宏定义
#define L_GPT_COUNT_OPEN	MCF_GPIO_PORTTF|=MCF_GPIO_PORTTF_PORTTF7//A15
#define L_GPT_COUNT_OFF		MCF_GPIO_PORTTF&=~MCF_GPIO_PORTTF_PORTTF7//A15
#define R_GPT_COUNT_OPEN	MCF_GPIO_PORTTG|=MCF_GPIO_PORTTG_PORTTG1//A17
#define R_GPT_COUNT_OFF		MCF_GPIO_PORTTG&=~MCF_GPIO_PORTTG_PORTTG1//A17

//外部中断标志位
#define Clear_IRQ5 (MCF_EPORT_EPFR |= MCF_EPORT_EPFR_EPF5)
#define Clear_IRQ7 (MCF_EPORT_EPFR |= MCF_EPORT_EPFR_EPF7)
//定时器开关
#define EnablePIT(number) (MCF_PIT_PCSR(number)|=0x0001)
#define DisablePIT(number)  (MCF_PIT_PCSR(number)&=~(0x0001))

//车体状态
#define Go 1
#define Wait 0
#define Stop 2
#define Loading 3
