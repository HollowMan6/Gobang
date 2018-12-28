#include<conio.h>
#include<stdlib.h>
#include<stdio.h>
#include<windows.h>
#define N 15		        //包含设置坐标函数 

int ChessBoard[N][N] = {0}; //棋盘图 
int Cursor[2] = {8,8};      //当前光标坐标X,Y的默认位置
int ChessStep[255][2] = {0};//记录按照落子次序的棋盘坐标位置，用于悔棋
char name1[10],name2[10];   //记录玩家姓名
int i;                      //计数

void gotoxy(int x,int y)
{
	COORD pos;
	pos.X=x-1;
	pos.Y=y-1;
	//Windows自带的设置坐标位置函数
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),pos); 
}

void welcome()
{
	int i;
	gotoxy(15, 5);
	printf("/**********************************************/");
	gotoxy(15, 20);
	printf("/**********************************************/");
	gotoxy(35, 7);
	printf("五子棋游戏");
	gotoxy(30, 16);
	printf("支持双人游戏！");
	gotoxy(18, 18);
	printf("快来和你的小伙伴一起玩呀!!!      =￣ω￣="); 
	gotoxy(35, 22);
	printf("***Powered By Hollow Man***");
	gotoxy(20, 11);
	printf("·请输入你的姓名（少于10字符）:");
	for (i=0;i<10;i++)
	{
		scanf("%c",&name1[i]);
		if(name1[i]=='\n')
			break;
		else if(i==9)
			break;
	}
	gotoxy(20, 12);
	printf("·请输入你小伙伴的姓名（少于10字符）:");
	for (i=0;i<10;i++)
	{
		scanf("%c",&name2[i]);
		if(name2[i]=='\n')
			break;
		else if(i==9)
			break;
	}
	system("cls");
}

void MoveTo(int x,int y)
{
	x=3*x-2;
	y=2*y-1;
	gotoxy(x,y);
}

//输入参数为光标移动的方向（1表示上，2表示下，3表示左，4表示右）
void Move(int direction)
{
	switch(direction)
	{
		case 1:Cursor[1]-=1;break;
		case 2:Cursor[1]+=1;break;
		case 3:Cursor[0]-=1;break;
		case 4:Cursor[0]+=1;break;
		default:printf("移动出错！           ");
	}
	MoveTo(Cursor[0],Cursor[1]); 
} 

//功能：将按键转化为数值编码：0 1 2 3 4 5 6 7
//参数数值代码含义
//		0表示输入错误，1表示上，2表示下，3表示左，4表示右；
//		5表示落子，6表示悔棋，7表示退出
//返回值含义：1表示方向键，2表示落子，3表示悔棋，4表示退出，0表示错误
int GetKey(int *data)
{
	unsigned char temp;
	while(1)
	{
		temp=getch();
		if(temp==224)
		{
			temp=getch();
			switch(temp)
			{
				case 72:*data=1;break;
				case 80:*data=2;break;
				case 75:*data=3;break;
				case 77:*data=4;break;
				default:continue;
			}
			return 1;
		}
		else
		{
			switch(temp)
			{		
				case 13:*data=5;return 2; //Enter键
				case 32:*data=5;return 2; //空格键
				case 8:*data=6;return 3; //BackSpace键
				case 27:*data=7;return 4; //Esc键
				default:*data=0;return 0;
			}
		}
	}
} 

int InBoundry(int x,int y)				  //位置是否出界 
{
	if (x==-1||x==N||y==-1||y==N) return 0;
	else return 1;
}

int Win(int v)							  //构成连续5子返回1，否则返回0 
{
	int i=1,j=1,x=0,y=0;
	//左对角线上连续5个棋子
	while(ChessBoard[x=Cursor[0]-i-1][y=Cursor[1]-i-1]==v&&InBoundry(x,y)) i++; 
	while(ChessBoard[x=Cursor[0]+j-1][y=Cursor[1]+j-1]==v&&InBoundry(x,y)) j++;
	if(i+j>=6) return 1; 
	
	//右对角线上连续5个棋子
	i=1,j=1,x=0,y=0;
	while(ChessBoard[x=Cursor[0]+i-1][y=Cursor[1]-i-1]==v&&InBoundry(x,y)) i++; 
	while(ChessBoard[x=Cursor[0]-j-1][y=Cursor[1]+j-1]==v&&InBoundry(x,y)) j++;
	if(i+j>=6) return 1; 
	
	//同一列上连续5个棋子
	i=1,j=1,x=0,y=0;
	while(ChessBoard[x=Cursor[0]-i-1][y=Cursor[1]-1]==v&&InBoundry(x,y)) i++; 
	while(ChessBoard[x=Cursor[0]+j-1][y=Cursor[1]-1]==v&&InBoundry(x,y)) j++;
	if(i+j>=6) return 1; 
	
	//同一行上连续5个棋子
	i=1,j=1,x=0,y=0;
	while(ChessBoard[x=Cursor[0]-1][y=Cursor[1]-i-1]==v&&InBoundry(x,y)) i++; 
	while(ChessBoard[x=Cursor[0]-1][y=Cursor[1]+j-1]==v&&InBoundry(x,y)) j++;
	if(i+j>=6) return 1; 
	return 0; 
} 

void GameStart()
{
	int i,j;
	for(i=0;i<N;i++)
		for(j=0;j<N;j++)
			ChessBoard[i][j]=0;
			
	system("cls");
	for(i=1;i<30;i++)
	{
		for(j=1;j<=43;j++)
			if(i%2==1) printf("-");
			else if(j%3==1) printf("|");
			else printf(" ");
		printf("\n");
	}
	
	Cursor[0]=8;
	Cursor[1]=8;
	gotoxy(50,2);
	printf("玩家1: %s",name1);
	gotoxy(52,4);
	printf("VS");
	gotoxy(50,6);
	printf("玩家2: %s",name2);
	gotoxy(50,10);
	printf("·使用方向键移动光标 ");
	gotoxy(50,12);
	printf("·使用空格或Enter键落子");
	gotoxy(50,14);
	printf("·按BackSpace键悔棋 ");
	gotoxy(50,16);
	printf("·按Esc键结束游戏     ");
	gotoxy(50,20);
	printf("请玩家1首先下棋...   ");
	gotoxy(9, 30);
	printf("***Powered By Hollow Man***");
	MoveTo(8,8); 
}

int OutBoundry(int data)
{
	int StepXY[2],New[2];
	switch(data)
	{
		case 1:StepXY[0]=0;StepXY[1]=-1;break;
		case 2:StepXY[0]=0;StepXY[1]=1;break;
		case 3:StepXY[0]=-1;StepXY[1]=0;break;
		case 4:StepXY[0]=1;StepXY[1]=0;break;
		case 5:StepXY[0]=0;StepXY[1]=0;break;
	}
	
	New[0]=Cursor[0]+StepXY[0];
	New[1]=Cursor[1]+StepXY[1];
	if(New[0]>N||New[0]<1||New[1]>N||New[1]<1) return 1;
	else return 0;
}

//第一个输入值为按键的玩家序号
//第二个为按下键的对应值
int KeyError(int v,int direction)
{
	if(v==3||v==4) return 0;
	else if(v==0) return 1;
	if(OutBoundry(direction)) return 1;
		return 0;
} 

int luozi(int v)
{
	ChessBoard[Cursor[0]-1][Cursor[1]-1]=v;
	if(v==1) printf("O");
	else if(v==2) printf("X");
	if (Win(v))
	{
		gotoxy(50,10);
		printf("                     ");
		gotoxy(50,12);
		printf("                            ");
		gotoxy(50,14);
		printf("·按空格或Esc键退出程序          ");
		gotoxy(50,16);
		printf("                             ");
		gotoxy(50,20);
		printf("玩家%d赢了!!!        ",v);
		gotoxy(50,24);
		printf("祝贺!!!      o(*￣▽￣*)o  ",v);
		MoveTo(Cursor[0],Cursor[1]);
		getch();
		return 1;
	}
	gotoxy(50,20);
	printf("现在请玩家%d下棋!        ",v%2+1);
	MoveTo(Cursor[0],Cursor[1]);
	return 0;
}

void HuiQi(int step)						//step为当前要悔的是第几步棋 
{
	Cursor[0]=ChessStep[step-1][0];
	Cursor[1]=ChessStep[step-1][1];
	ChessBoard[Cursor[0]-1][Cursor[1]-1]=0;
	
	MoveTo(Cursor[0],Cursor[1]);
	printf("-");
	gotoxy(50,20);
	printf("现在请玩家%d下棋!        ",(step+1)%2+1);
	MoveTo(Cursor[0],Cursor[1]);
}

int Play()
{
	int direction=0;
	int i=0,temp;

	while(1)
	{
		while(temp=GetKey(&direction),KeyError(temp,direction));
		switch(direction)
		{
			case 1:
			case 2:
			case 3:
			case 4:Move(direction);break;
			case 5:
				if(ChessBoard[Cursor[0]-1][Cursor[1]-1]) continue;
					if(luozi(i%2+1)) return 0;
					ChessStep[i][0]=Cursor[0];		//记录落子步骤
					ChessStep[i][1]=Cursor[1];
					i++;
					break;
			case 6:
				if(i==0)
				{
					gotoxy(50,20);
					printf("现在无法悔棋!         ");
					MoveTo(Cursor[0],Cursor[1]);
				}
				else HuiQi(i--);
				break;
			case 7:return 0;
		}
	}
	return 1;
}

int main()
{
	//system("color 0B");
	welcome();
	GameStart();
	Play();
	gotoxy(50,22);
	return 0;
}