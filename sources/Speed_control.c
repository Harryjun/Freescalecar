/********************************
�ļ����ƣ�Speed_control�ٶȿ��ƺ���
���ߣ���Э����ͷһ��
�汾��V1.0
˵�������ܳ��ٶȿ��ƺ���
�޸ļ�¼��
2015.5.23�������
********************************/

#include"Speed_control.h"
static double  PD_P_1 =2.1;//2.7;//2.3;//4.6; //�������� Proportional Const
static double  PD_D_1 = 1.10; //΢�ֳ��� Derivative Const
static double  PD_P_2 = 4.7; //�������� Proportional Const
static double  PD_D_2 = 1.15; //΢�ֳ��� Derivative Const

//���L_PID
static double  PID_L_P = 4.6;
static double  PID_L_I = 2.37;
static double  PID_L_D = 0.38;


static int flag_t = 0;
int	  Count_xuhao=0;//����������ѭ���л���־λ
int16 PULSE_right=0;//�ɼ��������ҵ��������
int16 PULSE_left=0;//�ɼ�����������������
/*********************************************************** 
�������ƣ�PIDInit
�������ܣ�PID��ʼ��
��ڲ�������
���ڲ�������
***********************************************************/
void PIDInit(void)
{	
    sptr->LastError = 0; //���ֵ�Error[-1]
    sptr0->LastError = 0; //���ֵ�Error[-1]
    sptr->PrevError = 0; //���ֵ�Error[-2]
    sptr0->PrevError = 0; //���ֵ�Error[-2]
    sptr->return_value=500;
    sptr0->return_value=500;
    
    sptr1->LastError = 0; 
    sptr1->return_value=0;

}
/*********************************************************** 
�������ƣ�PWM_left/PWM_right
�������ܣ��������
��ڲ�����x
���ڲ�������
***********************************************************/
void PWM_left(int16 x)
{
	MCF_PWM_PWMDTY(2)=(x>>8);
	MCF_PWM_PWMDTY(3)=(x&0x00ff);
}
void PWM_right(int16 x)
{
	MCF_PWM_PWMDTY(0)=(x>>8);
	MCF_PWM_PWMDTY(1)=(x&0x00ff);
}
/*********************************************************** 
�������ƣ�PWM_duoji
�������ܣ��������
��ڲ�����x1500����2000��ת
���ڲ�������
***********************************************************/
void PWM_duoji(int16 x)
{
	int temp;
	if(x < Mid_Steer - 150)
		x = Mid_Steer - 150;
	if(x > Mid_Steer + 150)
		x = Mid_Steer + 150;
	MCF_PWM_PWMDTY(4)=(x>>8);
	MCF_PWM_PWMDTY(5)=(x&0x00ff);	
}
/*********************************************************** 
�������ƣ�GPT_Pulse_Counting
�������ܣ�GPT����ɼ�
��ڲ�������
���ڲ������� 
***********************************************************/
void GPT_Pulse_Counting()
{
	int16 cnt1,cnt2;//��¼������
	switch(Count_xuhao)
	{
		case 0://�������
			PULSE_left = MCF_GPT_GPTPACNT;//��ȡ������
			R_GPT_COUNT_OPEN;//����������
			L_GPT_COUNT_OFF;
			MCF_GPT_GPTPACNT=0;	//����
			break;
		case 1://�ұ�����
			PULSE_right=MCF_GPT_GPTPACNT;//��ȡ������
			L_GPT_COUNT_OPEN;
			R_GPT_COUNT_OFF;
			MCF_GPT_GPTPACNT=0;	
			break;		
	}
	Count_xuhao++;//�л����
	if(Count_xuhao>=2)
		Count_xuhao=0;
}
//����ʽPID�������
/*********************************************************** 
�������ƣ�IncPID
�������ܣ����PID����/����ʽ
��ڲ����������������ٶ�ֵ
���ڲ��������PWM������
***********************************************************/
int IncPIDR(int NextPoint)
{
	int iError,dError,ddError,Value;
	if(flag_t == 2 || flag_t == -2)
	{
		flag_t = 0;
	}
	//1ƫ���ʱ��ռ�ձ�
	if(SetPoint_R - NextPoint > 17 || flag_t == 1)
	{
		flag_t++;
		sptr0->return_value = 800;
		return(950);
	}	
	else if((SetPoint_R - NextPoint < -16 && SetPoint_R - NextPoint > -30)|| flag_t == -1)
	{
		flag_t--;
		sptr0->return_value = 125;
		return(210);
	}
	//2�������
		//��ǰ���
	iError = SetPoint_R - NextPoint;//   I����
		//���ϴ��������
	dError = iError - sptr0->LastError;   //   P����
		//�����ϴε����
	ddError = iError + sptr0->PrevError - 2 * sptr0->LastError;
    //3��������
    sptr0->return_value +=  PID_L_P * dError + PID_L_I * iError + PID_L_D * ddError;
    	
     //4��ֹ���
    if(sptr0->return_value<500)
    {
    	sptr0->return_value=500;
    }
    else if(sptr0->return_value>960)
    {
       	sptr0->return_value=950;
    }
    //5�洢���
    sptr0->PrevError = sptr0->LastError;
	sptr0->LastError = iError;
	
    return(sptr0->return_value);
}

int IncPIDL(int NextPoint)
{
	int iError,dError,ddError,Value;
	if(flag_t == 2 || flag_t == -2)
	{
		flag_t = 0;
	}
	//1ƫ���ʱ��ռ�ձ�
	if(SetPoint_R - NextPoint > 17 || flag_t == 1)
	{

		flag_t++;
		sptr->return_value = 800;
		return(950);
	}
	else if((SetPoint_R - NextPoint < -16 && SetPoint_R - NextPoint > -30) || flag_t == -1)
	{
		flag_t--;
		sptr->return_value = 125;
		return(210);
	}
	//2�������
	iError = SetPoint_L - NextPoint;//   I����
	//���ϴ��������
 	dError = iError - sptr->LastError;   //   P����
	//�����ϴε����
	ddError = iError + sptr->PrevError - 2 * sptr->LastError;
    //3��������
    sptr->return_value +=  PID_L_P * dError + PID_L_I * iError + PID_L_D * ddError;
     //4��ֹ���
    if(sptr->return_value<500)
    {
    	sptr->return_value=500;
    }
    else if(sptr->return_value>960)
    {
    	sptr->return_value=950;
    }
    //5�������
    sptr->PrevError = sptr->LastError;
 	sptr->LastError = iError;
    return(sptr->return_value);
}
/*********************************************************** 
�������ƣ�LocPD
�������ܣ����PD����/λ��ʽ
��ڲ���:Setpoint ���������ڳ���Ұ�ڵ��趨����
	 	 NextPoint ������������ֵ
���ڲ��������PWM������
***********************************************************/
uint16 LocPD(int Setpoint,int NextPoint)
{	
	register int iError1,dError1,return_value;
    DUOJI_SetPoint=Setpoint;
    //��ǰ���
    iError1 = DUOJI_SetPoint - NextPoint;
    //ƫ���΢��
    dError1 = iError1 - sptr1->LastError; 
    //�洢�������´μ���
    sptr1->LastError = iError1;
    //���ؿ���ֵ
	return_value=Mid_Steer+PD_P_1 * iError1 + PD_D_1 * dError1;
    if(return_value<(Mid_Steer - 145))   
    	return_value = Mid_Steer - 145;
    if(return_value>(Mid_Steer + 145))
    	return_value = Mid_Steer + 145;    
    return return_value ;
}







