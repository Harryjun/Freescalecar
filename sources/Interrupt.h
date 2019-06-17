#ifndef INTERRUPT_H_
#define INTERRUPT_H_
#include <stdio.h>
#include"gpio.h"
#include"Speed_control.h"
//�ⲿ�жϱ�־λ
#define Clear_IRQ5 (MCF_EPORT_EPFR |= MCF_EPORT_EPFR_EPF5)
#define Clear_IRQ7 (MCF_EPORT_EPFR |= MCF_EPORT_EPFR_EPF7)
//��ʱ������
#define EnablePIT(number) (MCF_PIT_PCSR(number)|=0x0001)
#define DisablePIT(number)  (MCF_PIT_PCSR(number)&=~(0x0001))
//��������
extern int Condition_car;
extern int imageReady; 
extern int Count_xuhao;
extern int16 PULSE_right;
extern int16 PULSE_left;

//�ж��б�
__declspec(interrupt:0) void EPORT_irq7_handler(void);
__declspec(interrupt:0) void EPORT_irq5_handler(void);
__declspec(interrupt:0) void EPORT_irq1_handler(void);
__declspec(interrupt:0) void EPORT_irq3_handler(void);
__declspec(interrupt:0) void PIT1_handler(void);
__declspec(interrupt:0) void PIT0_handler(void);
#endif