/********************************
文件名称：Camera_identify摄像头识别程序
作者：无协摄像头一队
版本：V1.0
说明：智能车识别部分
修改记录：
2015.5.23整理完成
********************************/
#include"system.h"

uint8 IsLineL[ROW], IsLineR[ROW];
int16 FlagCenter[ROW],FlagR[ROW],FlagL[ROW];
uint8 WTB[LINE];
uint8 BTW[LINE];
uint8 IsWhiteLine[LINE];
uint8 WhiteLine_num,White_num,Most_White_Position,Left_Hole_Position,Left_Hole_Width,Right_Hole_Position,Right_Hole_Width;
uint8 m_start=0,m_end=0,SingleDirection=0,PreMode=0;
uint8 BTWmostLeft,BTWLeft,BTWCenter,BTWRight,BTWmostRight,
	 WTBmostLeft,WTBLeft,WTBCenter,WTBRight,WTBmostRight;
uint8 FlagSingle=0,FlagRightAngle=0;

//赛道宽度数组
int Track_Width[ROW]=
{
/*	43,45,46,47,48,50,51,52,53,53,//0-9
	55,57,58,59,60,62,64,65,67,68,//10-19
	70,72,74,77,78,81,83,85,87,88,//20-29
	92,94,97,100,102,105,108,111,114,117,//30-39
	120,120,120,120,120//120,124,128,132,136
*/
	130,130,130,130,130,130,130,130,130,130,
	130,130,130,130,130,130,130,130,130,130,
	130,130,130,130,130,130,130,130,130,130,
	125,125,125,125,125,125,125,124,120,120,
	120,120,120,120,120,120,120,120,120,120,
	120,120,120,120,120,120,120,120,120,120
};


int abs(int i)
{
	if(i>0) return i;
	else return -i;
}

//////////////////////////////////////////
//图像采集函数部分
//////////////////////////////////////////
/*********************************************************** 
函数名称：SeekCenterSingle_nearest
函数功能：提取最近行中线
入口参数：最近行
出口参数：中线 
***********************************************************/
static void SeekCenterSingle_nearest(uint8 lens)
{
	uint8 temp;
	int16 LeftPoint,RightPoint,CenterPoint;
	int16 Position,PointL=0,PointR=0,LPointL=0,LPointR=0,RPointL=0,RPointR=0;
	IsLineL[lens] = 0;
	IsLineR[lens] = 0;
	LeftPoint=0;
	RightPoint=LINE-1;
	for(temp=5;temp<LINE-16;temp++)
	{
		if(image[lens][temp] < image[lens][temp+1] )
		{
			if(image[lens][temp] < image[lens][temp+2])
				if(image[lens][temp-1] < image[lens][temp+2])
				{
					LeftPoint = temp;
					IsLineL[lens] = 1;
					break;	
				}
		}
	}
	
	for(temp=LINE-6;temp>15;temp--)
	{
		if(image[lens][temp] < image[lens][temp-1])
		{
			if(image[lens][temp] < image[lens][temp-2])
				if(image[lens][temp+1] < image[lens][temp-2])
				{
					RightPoint=temp;
					IsLineR[lens] = 1;
					break;
				}
					
		}
	}
	if(IsLineL[lens] == 1 && IsLineR[lens] == 1)
	{
			
		CenterPoint=(RightPoint + LeftPoint)/2;
		//在两线中间找黑段
		if(CenterPoint<11) Position = 11;
		else  if(CenterPoint>LINE-13) Position = LINE-13;
		else Position = CenterPoint;
		
		for(temp=Position-10;temp<Position+10;temp++)
		{
			if(image[lens][temp] != image[lens][temp+1])
			{
				if(image[lens][temp] != image[lens][temp+2])
					if(image[lens][temp-1] != image[lens][temp+2])
					{
						if(PointL==0) PointL = temp;
						else PointR =temp;
					}
			}
		}
		
		if(PointL != 0 && PointR != 0 )//中间存在白黑白  //一般障碍物只能扫到一条边，即PointL
		{
			if( PointR-PointL<10)//中间不太大，不是障碍物
			{
				FlagSingle = 1;
				FlagL[lens] = PointL;
				FlagR[lens] = PointR;
				FlagCenter[lens] = (PointL+PointR)/2;	
			}
			else//存在障碍物时 //另写//
			{
				FlagSingle = 1;
				FlagL[lens] = PointL;
				FlagR[lens] = PointR;
				FlagCenter[lens] = (PointL+PointR)/2;	
			}
		}
		else//中间不存在白黑白//遍历左右
		{
			//左边向左再找黑变白
			LPointR = LeftPoint;
			
			if(LeftPoint<11) Position = 11;
			else  if(LeftPoint>LINE-13) Position = LINE-13;
			else Position = LeftPoint;
				
			for(temp=Position;temp>Position-10;temp--)
			{	
				if(image[lens][temp] < image[lens][temp-1])
				{
					if(image[lens][temp] < image[lens][temp-2])
						if(image[lens][temp+1] < image[lens][temp-2])
						{
							 LPointL =temp;
						}
				}
			}
			//右边向右再找黑变白
			RPointL = RightPoint;
			
			if(RightPoint<11) Position = 11;
			else  if(RightPoint>LINE-13) Position = LINE-13;
			else Position = RightPoint;
				
			for(temp=Position;temp<Position+10;temp++)
			{	
				if(image[lens][temp] < image[lens][temp+1])
				{
					if(image[lens][temp] < image[lens][temp+2])
						if(image[lens][temp-1] < image[lens][temp+2])
						{
							 RPointR =temp;
						}
				}
			}	
			///////////////////////////////////////
			//左右都是白黑白或者左右都不是。。双线
			if((LPointL != 0 && RPointR != 0) || (LPointL == 0 && RPointR == 0))
			{
				FlagSingle = 0;//双线
				FlagL[lens] = LeftPoint;
				FlagR[lens] = RightPoint;
				FlagCenter[lens] = CenterPoint;//(LeftPoint+RightPoint)/2;
				
			}
			//左边是白黑白，右边不是   左单线
			if(LPointL != 0 && RPointR == 0)
			{
				FlagSingle = 1;//单线，单线为左边
				FlagL[lens] = LPointL;
				FlagR[lens] = LPointR;
				FlagCenter[lens] = (LPointL+LPointR)/2;
			}
			//左边不是白黑白，右边是   右单线
			if(LPointL == 0 && RPointR != 0)
			{
				FlagSingle = 1;//单线，单线为右边
				FlagL[lens] = RPointL;
				FlagR[lens] = RPointR;
				FlagCenter[lens] = (RPointL+RPointR)/2;
			}	
		}
	
	}
	else//左右存在丢线
	{
		if(IsLineL[lens] == 1)//左找到右没找到 
		{
			FlagL[lens] = LeftPoint;
			FlagCenter[lens]=(LeftPoint + RightPoint)/2;
			FlagR[lens]=RightPoint;
		}
		else	if(IsLineR[lens] == 1)//右找到左没找到
		{
			FlagR[lens]=RightPoint;
			FlagCenter[lens]=(LeftPoint + RightPoint)/2;
			FlagL[lens]=LeftPoint;
		}
		else
		{
			FlagL[lens] = LeftPoint;
			FlagR[lens]=RightPoint;
			FlagCenter[lens] =  (LeftPoint+RightPoint)/2;
		}
	}
	if(FlagSingle==1)
	{
		IsLineL[lens]=IsLineR[lens]=3;
	}
	
}

/*********************************************************** 
函数名称：SeekCenterSingle
函数功能:单线提取
入口参数：len行数  k上一次中线的坐标
出口参数：无  
***********************************************************/
static void SeekCenterSingle(uint8 lens, int16 k)
{
	uint16 temp,PointL=0,PointR=0, CenterPoint;
	
	if(k<25) k = 25;
	else  if(k>LINE-27) k = LINE-27;
	
	for(temp=k-24;temp<k+27;temp++)
	{
		if(image[lens][temp] > image[lens][temp+1])
		{
			if(image[lens][temp] > image[lens][temp+2])
				if(image[lens][temp-1] > image[lens][temp+2])
				{
					 PointL = temp;
					 break;
				}
		}
	}
	for(;temp<k+27;temp++)
	{
		if(image[lens][temp] < image[lens][temp+1])
		{
			if(image[lens][temp] < image[lens][temp+2])
				if(image[lens][temp-1] < image[lens][temp+2])
				{
					 PointR = temp;
					 break;
				}
		}
	}
	if( PointR != 0 && PointR - PointL<20)
	{
		IsLineL[lens] = 3;
		IsLineR[lens] = 3;
		FlagL[lens]=PointL;
		FlagR[lens]=PointR;
		FlagCenter[lens]=(PointL+PointR)/2;
	}
	else
	{
	//	if(PointL != 0)//只采到一边该如何处理？ 
	//	{
	//		FlagL[lens]=PointL;
	//		FlagR[lens]=PointL;
	//		FlagCenter[lens]=PointL;
	//	}
	//	else
		{
			FlagSingle=0;
			SeekCenterSingle_nearest(lens);	
		}
		
	}
}
/*********************************************************** 
函数名称：SeekCenterDouble
函数功能:双线边线提取
入口参数：len行数  
出口参数：无  
***********************************************************/
static void SeekCenterDouble(uint8 lens)
{
	
	uint8 temp;
	uint16 Position,LPointL=0,LPointR=0,RPointL=LINE-1,RPointR=0;
	IsLineL[lens] = 0;
	IsLineR[lens] = 0;	
	//Position确立
	if(image[lens][Most_White_Position]==255)
	{
		Position=Most_White_Position;
	}
	else
	{
		{
		if(FlagCenter[lens+1]<2)Position=2;
		else if(FlagCenter[lens+1]>LINE-3)Position=LINE-3;
		else Position=FlagCenter[lens+1];
		}
	}

	{
		//检测左边界
		for(temp=Position; temp>1; temp-=2)
		{
			if(image[lens][temp] == 254)
			{
				if(image[lens][temp+1] == 254)
				{
					LPointR = temp+1;
					IsLineL[lens] = 1;
					break;
				}
				else if(image[lens][temp-1] == 254)
				{
					LPointR = temp;
					IsLineL[lens] = 1;
					break;
				}
			}
		}
		for(;temp>2&&temp>LPointR-28;temp--)
		{
			if(image[lens][temp] < image[lens][temp-1])
			{
				if(image[lens][temp] < image[lens][temp-2])
					if(image[lens][temp+1] < image[lens][temp-2])
					{
						LPointL = temp;	
						break;	
						
					}
			}
		}
		///检测右边界
		for(temp = Position;temp<LINE-2; temp += 2)
		{
			if(image[lens][temp] == 254)
			{
				if(image[lens][temp-1] == 254 )
				{
					RPointL = temp-1;
					IsLineR[lens] = 1;
					break;
				}
				else if(image[lens][temp+1] == 254)
				{
					RPointL = temp;
					IsLineR[lens] = 1;
					break;
				}
			}
		}
		for(;temp<LINE-2&&temp<RPointL+28;temp++)
		{
			if(image[lens][temp] < image[lens][temp+1])
			{
				if(image[lens][temp] < image[lens][temp+2])
					if(image[lens][temp-1] < image[lens][temp+2])
					{
						RPointR = temp;	
						break;	
						
					}
			}
		}
		if(IsLineL[lens] == 1 && IsLineR[lens] == 1)
		{
			//接着再找是否为一边黑白黑
			if(RPointR==0&&LPointL==0)//左右都不是黑白黑
			{///蓝底双线
				FlagL[lens]=LPointR;
				FlagR[lens]=RPointL;
				FlagCenter[lens]=(LPointR+RPointL)/2;
			}
			else if(RPointR==0)//右边不是黑白黑，左边是
			{
				if(LPointR-LPointL>15)
				{
					FlagR[lens]=RPointL;
					FlagL[lens]=RPointL-Track_Width[lens];
					FlagCenter[lens]=RPointL-Track_Width[lens]/2;
					IsLineL[lens]=0;
				}
				else
				{
					FlagSingle=1;
					FlagL[lens]=LPointL;
					FlagR[lens]=LPointR;
					FlagCenter[lens]=(LPointR+LPointL)/2;		
				}
				
			}
			else 
			{
				FlagSingle=1;
				FlagL[lens]=RPointL;
				FlagR[lens]=RPointR;
				FlagCenter[lens]=(RPointR+RPointL)/2;	
			}
			
		}
		else if(IsLineL[lens]==0 && IsLineR[lens]==1)
		{
			if(RPointR==0)
			{	
				FlagL[lens]=RPointL-Track_Width[lens];
				FlagR[lens]=RPointL;
				FlagCenter[lens] = FlagR[lens] - Track_Width[lens]/2;
			}
			else//单线
			{
			/*	FlagL[lens]=0;
				FlagR[lens]=RPointR;
				FlagCenter[lens]=(RPointL+RPointR)/2;*/
				FlagSingle = 1;
				FlagL[lens]=RPointL;
				FlagR[lens]=RPointR;
				FlagCenter[lens]=(RPointL+RPointR)/2;
			}
	
		}
		else if(IsLineL[lens]==1 && IsLineR[lens]==0)
		{
			if(LPointL==0)
			{
				FlagR[lens]=LPointR+Track_Width[lens];
				FlagL[lens]=LPointR;
				FlagCenter[lens]=FlagL[lens] + Track_Width[lens]/2;//Track_Width[lens]/2;//;		
			}
			else
			{
				FlagSingle = 1;
			
				FlagR[lens]=LPointR;
				FlagL[lens]=LPointL;
				FlagCenter[lens]=(LPointL+LPointR)/2;
			}

		}
		else if(IsLineL[lens]==0 && IsLineR[lens]==0)
		{
		
			FlagL[lens] = FlagL[lens+1];
			FlagR[lens] = FlagR[lens+1];
			FlagCenter[lens] = Position;
		}
		
		if(FlagSingle==1)
		{
			IsLineR[lens]=3;
			IsLineL[lens]=3;
		}
		
	}
}


/*********************************************************** 
函数名称：SeekCenters
函数功能：提取中线
入口参数：无
出口参数：无  
***********************************************************/
void SeekCenters(void)
{
	int8 j;
    	for(j=(ROW-1);j>=0;j--)
    	{
        	if(j==(ROW-1))
        	{//寻找最近线	
             		SeekCenterSingle_nearest(j);//
        	}
        	else
        	{//每一次搜线，都是按上一次的中线往两边搜
        		if(FlagSingle==0)
        			SeekCenterDouble(j);
        		else
        	  		SeekCenterSingle(j,FlagCenter[j+1]);
        	}
    	}
}

/*****************************************
函数名称：VerticalFetch
函数功能：纵向扫线,扫取某列的白变黑，黑变白点
入口参数列数
出口参数：无  
*****************************************/
void VerticalFetch(uint8 rank)
{
	short temp,FlagWhite;
	WTB[rank]=0;
	BTW[rank]=0;
	if(image[ROW-1][rank]==255)FlagWhite=1;
	
	for(temp=ROW-2;temp>1&&(WTB[rank]*BTW[rank]==0);temp--)
	{
		if(FlagWhite==1)
		{
			if(image[temp][rank]==254)
			{
				if(image[temp-1][rank]==254)
				{
					FlagWhite=0;
					WTB[rank]=temp;
				}
			}
			
		}
		else
		{
			if(image[temp][rank]==255)
			{
				if(image[temp-1][rank]==255)
				{
					FlagWhite=1;
					BTW[rank]=temp;
				}
			}	
		}
	}
	if(WTB[rank]==BTW[rank])//待定
	{
		if(FlagWhite==1){WTB[rank]=0; BTW[rank]=ROW-1;}
		else{WTB[rank]=ROW-1; BTW[rank]=0;}
	}
} 
/*******************************************
函数名称：VerticalScan
函数功能：纵向扫线
入口参数无
出口参数：无  
*******************************************/
void VerticalScan(void)//静态
{
	int temp,num,mostwhite=ROW-1;
	WhiteLine_num=0;
	White_num=0;
	Most_White_Position = LINE/2;
	num=0;
	for(temp=0;temp<LINE;temp++)
	{
		VerticalFetch(temp);
		if(BTW[temp]==ROW-1&&WTB[temp]==0)
		{
			IsWhiteLine[temp]=1;
			WhiteLine_num++;	
		}
		else	IsWhiteLine[temp]=0;
		
	}
	BTWmostLeft=BTW[30];
	BTWLeft=BTW[60];
	BTWCenter=BTW[80];
	BTWRight=BTW[90];
	BTWmostRight=BTW[130];
	WTBmostLeft=WTB[30];
	WTBLeft=WTB[60];
	WTBCenter=WTB[80];
	WTBRight=WTB[90];
	WTBmostRight=WTB[130];
	if(WhiteLine_num != 0)
	{
		for(temp=0;temp<LINE;temp++)
		{
			if(IsWhiteLine[temp]==1)
			{
				num++;
			}
			else if(num>White_num)
			{
				White_num=num;
				Most_White_Position=temp-num/2;
				num=0;
			}
			else
			{
				num=0;
			}
		}	
	}
	else
	{
		for(temp=0;temp<LINE;temp++)
		{
			if((WTB[temp]>BTW[temp]||(BTW[temp]>ROW-5 && WTB[temp]<5)) && WTB[temp]<mostwhite)
			{
				Most_White_Position = temp;
				mostwhite = WTB[temp];
			}
		}
	}
	
}

/***************************************
函数名称：RepairRightAngleFlag
函数功能：出现直角标记线时，修正补线
入口参数无
出口参数：无  
***************************************/
void RepairRightAngleFlag(void)
{
	int temp,num,min,max,middle;
	num=0;
	min=LINE;
	max=0;
	for(temp=10;temp<LINE-10;temp++)
	{
		if(abs(WTB[temp] - WTB[temp+4])<2 && abs(BTW[temp] - BTW[temp+4]<2)
			&&WTB[temp] - BTW[temp]>2 && WTB[temp] - BTW[temp]<6 
			&&WTB[temp] != 0 && BTW[temp] != ROW - 1 
			&&BTW[temp] != 0 && WTB[temp] != ROW - 1 )
		{
			num++;
			if(temp<min)min=temp;
			else if(temp>max)max=temp;
		}
	}
	if(num>19)
	{
		if(FlagRightAngle == 0)
		FlagRightAngle = 1;
		Most_White_Position = (min + max)/2;
		middle = (min + max)/2;
		White_num=88;
		for(temp=BTW[Most_White_Position];temp>=1;temp--)
		{
			if(image[temp][Most_White_Position]==254)
				if(image[temp-1][Most_White_Position]==254)
				White_num=99;
			
		}
		if(WTB[min]>WTB[max])temp = WTB[min];
		else temp = WTB[max];
		for(;temp>=BTW[min]||temp>=BTW[max];temp--)
		{
			FlagCenter[temp]=middle;
			FlagL[temp] = min;
			FlagR[temp] = max;
			IsLineL[temp] = 1;
			IsLineR[temp] = 1;	
		}
	}

}

/***************************************
函数名称：RepairPassingHole
函数功能：有贯通线和缺口时，修正补线
入口参数无
出口参数：无  
****************************************/
void RepairPassingHole(void)
{
	int temp ;
	if(Left_Hole_Width > 0 && Right_Hole_Width > 0 && White_num > 0)
	{
		if(Left_Hole_Position>Right_Hole_Position) temp = Left_Hole_Position;
		else temp = Right_Hole_Position;
		for(;temp>Left_Hole_Position - Left_Hole_Width || temp>Right_Hole_Position - Right_Hole_Width;temp--)
		{
			FlagL[temp] = Most_White_Position - White_num/2;
			FlagR[temp] = Most_White_Position + White_num/2;
			FlagCenter[temp] = Most_White_Position;
		}
		for(;temp>=0;temp--)
		{
			if(FlagSingle==0)
	          		SeekCenterDouble(temp);
	        	else
	        	  	SeekCenterSingle(temp,FlagCenter[temp+1]);
		}
	}
}

/****************************************
函数名称：RepairSingle
函数功能：单线后中线修补
入口参数无
出口参数：无  
*****************************************/
void RepairSingle(void)
{
	int temp;
	m_start=0;
	m_end=0;
	for(temp=ROW-1;temp>10 && (IsLineL[temp]!=5||IsLineL[temp-7]!=5);temp--)
	{
		if(IsLineL[temp]==3&&IsLineL[temp-1]==3)
		{
			m_start=temp;
			break;
		}
	}
	if(m_start != 0)
	{
		for(;temp>1;temp--)
		{
			if(IsLineL[temp] !=3&&IsLineL[temp-1] !=3 )
			{
				m_end=temp+1;
				break;
			}
		}	
		
	}
	if((m_end > 1 )&&(m_end < ROW-10)&&(IsLineL[m_end-5]+IsLineR[m_end-5]==0 || FlagR[m_end-5]-FlagL[m_end-5]>90)&&(m_start-m_end)>10)
	{
		PreMode = 1;
		if(FlagCenter[m_end]>FlagCenter[m_end+2])
		{
			for(temp=FlagCenter[m_end]-3;temp<FlagCenter[m_end]+10&&temp<LINE-1;temp++)
			{
				if(image[m_end][temp]==254 && image[m_end][temp+1]==254)
				FlagCenter[m_end]=temp;
			}
		}
		else
		{
			for(temp=FlagCenter[m_end]+3;temp>FlagCenter[m_end]-10&&temp>0;temp--)
			{
				if(image[m_end][temp]==254 && image[m_end][temp-1]==254)
				FlagCenter[m_end]=temp;
			}	
		}
		if(m_start>52&&m_end<32)
		{
			if(FlagCenter[m_start]<FlagCenter[m_end])
			{
					SingleDirection = 2;//右
			}
			else
			{
					SingleDirection = 1;//左	
			}
		}
		
		for(temp=m_end-1;temp>=0&&m_end>40;temp--)
		{
			FlagCenter[temp]=2*FlagCenter[temp+1]-FlagCenter[temp+2];
			
			if(FlagCenter[temp]<0)
			{
				FlagCenter[temp]=0;
			}
			if(FlagCenter[temp]>159)
			{
				FlagCenter[temp]=159;	
			}
		}
	}
	else if( m_start-m_end<4 && m_end > 3)
	{
		for(temp=m_start;temp>=m_end;temp--)
		{
			FlagCenter[temp]=Most_White_Position;
			IsLineL[temp]=IsLineR[temp]=1;
		}
		m_start=m_end=0;
	}
		
}


/****************************************
函数名称：ScanHole
函数功能：从近到远找到左右边的第一个缺口，记录缺口的大小，和开始坐标
入口参数无
出口参数：无  
*****************************************/
void ScanHole(void)
{///////////////////两个问题1>弯道缺口畸变会使他很小，可以用丢线来确定缺口起始位置，宽度？		
			////////	 2>弯道入直角
	int temp=0,temp1=0;int rrrr=0,llll=0;
	Left_Hole_Width=Right_Hole_Width=0;
	Left_Hole_Position=Right_Hole_Position=0;
	
	for(temp = ROW-1;temp>=0;temp--)
	{	
		rrrr += IsLineR[temp];
		llll += IsLineL[temp];
		if(temp==ROW-1)
		{
			if(IsLineL[temp]==IsLineR[temp]&&0==IsLineR[temp])
			{
				if(image[temp][FlagCenter[temp]]==254
					&&image[temp][FlagL[temp]]==254
					&&image[temp][FlagR[temp]]==254)
					IsLineL[temp]=5;
			}	
		}
		else
		{
			if(IsLineL[temp]==IsLineR[temp]&&1==IsLineR[temp])
			{
				if(image[temp][FlagCenter[temp]]==254
					&&image[temp][FlagL[temp]]==254
					&&image[temp][FlagR[temp]]==254)
					IsLineL[temp]=5;
			}
		}
	}
	
	for(temp=ROW-1;temp >= 0 &&IsLineL[temp]!=5;temp--)
	{
		if(IsLineL[temp] == 0)
		{
			for(temp1=temp;temp1>=0;temp1--)
			{
				if(IsLineL[temp1] == 1 || temp1 == 0 || IsLineL[temp1]==5 ) 
				{
					if(temp - temp1 > Left_Hole_Width)
					{
						Left_Hole_Width = temp - temp1;
						Left_Hole_Position = temp;
					}
					break;		
				}		
			}		
		}
		 if(IsLineR[temp] == 0)
		{
			for(temp1=temp;temp1>=0;temp1--)
			{
				if(IsLineR[temp1] == 1 || temp1 == 0 || IsLineL[temp1]==5 ) 
				{
					if(temp - temp1 > Right_Hole_Width)
					{
						Right_Hole_Width = temp - temp1 ;
						Right_Hole_Position = temp;
					}
					break;	
				}
			}
		}		
	}
}
/****************************************
函数名称：Identify_Picture
函数功能：图像分析
入口参数无
出口参数：无  
*****************************************/
void Identify_Picture()
{
	VerticalScan();
	SeekCenters();
	ScanHole();
	RepairSingle();
	RepairRightAngleFlag();
//	RepairPassingHole();
	
}