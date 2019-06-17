/********************************
文件名称：Speed_control速度控制函数
作者：无协摄像头一队
版本：V1.0
说明：智能车速度控制函数
修改记录：
2015.5.23整理完成
********************************/

#include"Speed_control.h"
static double  PD_P_1 =2.1;//2.7;//2.3;//4.6; //比例常数 Proportional Const
static double  PD_D_1 = 1.10; //微分常数 Derivative Const
static double  PD_P_2 = 4.7; //比例常数 Proportional Const
static double  PD_D_2 = 1.15; //微分常数 Derivative Const

//电机L_PID
static double  PID_L_P = 4.6;
static double  PID_L_I = 2.37;
static double  PID_L_D = 0.38;


static int flag_t = 0;
int	  Count_xuhao=0;//编码器左右循环切换标志位
int16 PULSE_right=0;//采集回来的右电机脉冲数
int16 PULSE_left=0;//采集回来的左电机脉冲数
/*********************************************************** 
函数名称：PIDInit
函数功能：PID初始化
入口参数：无
出口参数：无
***********************************************************/
void PIDInit(void)
{	
    sptr->LastError = 0; //左轮的Error[-1]
    sptr0->LastError = 0; //右轮的Error[-1]
    sptr->PrevError = 0; //左轮的Error[-2]
    sptr0->PrevError = 0; //右轮的Error[-2]
    sptr->return_value=500;
    sptr0->return_value=500;
    
    sptr1->LastError = 0; 
    sptr1->return_value=0;

}
/*********************************************************** 
函数名称：PWM_left/PWM_right
函数功能：电机调速
入口参数：x
出口参数：无
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
函数名称：PWM_duoji
函数功能：舵机函数
入口参数：x1500摆正2000左转
出口参数：无
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
函数名称：GPT_Pulse_Counting
函数功能：GPT脉冲采集
入口参数：无
出口参数：无 
***********************************************************/
void GPT_Pulse_Counting()
{
	int16 cnt1,cnt2;//记录脉冲数
	switch(Count_xuhao)
	{
		case 0://左编码器
			PULSE_left = MCF_GPT_GPTPACNT;//读取脉冲数
			R_GPT_COUNT_OPEN;//编码器开关
			L_GPT_COUNT_OFF;
			MCF_GPT_GPTPACNT=0;	//清零
			break;
		case 1://右编码器
			PULSE_right=MCF_GPT_GPTPACNT;//读取脉冲数
			L_GPT_COUNT_OPEN;
			R_GPT_COUNT_OFF;
			MCF_GPT_GPTPACNT=0;	
			break;		
	}
	Count_xuhao++;//切换序号
	if(Count_xuhao>=2)
		Count_xuhao=0;
}
//增量式PID控制设计
/*********************************************************** 
函数名称：IncPID
函数功能：电机PID控制/增量式
入口参数：编码器反馈速度值
出口参数：电机PWM的增量
***********************************************************/
int IncPIDR(int NextPoint)
{
	int iError,dError,ddError,Value;
	if(flag_t == 2 || flag_t == -2)
	{
		flag_t = 0;
	}
	//1偏差大时满占空比
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
	//2计算误差
		//当前误差
	iError = SetPoint_R - NextPoint;//   I调节
		//与上次误差的误差
	dError = iError - sptr0->LastError;   //   P调节
		//与上上次的误差
	ddError = iError + sptr0->PrevError - 2 * sptr0->LastError;
    //3增量计算
    sptr0->return_value +=  PID_L_P * dError + PID_L_I * iError + PID_L_D * ddError;
    	
     //4防止溢出
    if(sptr0->return_value<500)
    {
    	sptr0->return_value=500;
    }
    else if(sptr0->return_value>960)
    {
       	sptr0->return_value=950;
    }
    //5存储误差
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
	//1偏差大时满占空比
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
	//2计算误差
	iError = SetPoint_L - NextPoint;//   I调节
	//与上次误差的误差
 	dError = iError - sptr->LastError;   //   P调节
	//与上上次的误差
	ddError = iError + sptr->PrevError - 2 * sptr->LastError;
    //3增量计算
    sptr->return_value +=  PID_L_P * dError + PID_L_I * iError + PID_L_D * ddError;
     //4防止溢出
    if(sptr->return_value<500)
    {
    	sptr->return_value=500;
    }
    else if(sptr->return_value>960)
    {
    	sptr->return_value=950;
    }
    //5储存误差
    sptr->PrevError = sptr->LastError;
 	sptr->LastError = iError;
    return(sptr->return_value);
}
/*********************************************************** 
函数名称：LocPD
函数功能：舵机PD控制/位置式
入口参数:Setpoint 赛道中线在车视野内的设定坐标
	 	 NextPoint 赛道中线坐标值
出口参数：舵机PWM控制量
***********************************************************/
uint16 LocPD(int Setpoint,int NextPoint)
{	
	register int iError1,dError1,return_value;
    DUOJI_SetPoint=Setpoint;
    //当前误差
    iError1 = DUOJI_SetPoint - NextPoint;
    //偏差的微分
    dError1 = iError1 - sptr1->LastError; 
    //存储误差，用于下次计算
    sptr1->LastError = iError1;
    //返回控制值
	return_value=Mid_Steer+PD_P_1 * iError1 + PD_D_1 * dError1;
    if(return_value<(Mid_Steer - 145))   
    	return_value = Mid_Steer - 145;
    if(return_value>(Mid_Steer + 145))
    	return_value = Mid_Steer + 145;    
    return return_value ;
}







