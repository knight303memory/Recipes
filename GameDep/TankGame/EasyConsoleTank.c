/*目前设定了打死5个坦克就算赢了  另外那个分数存储有BUG 没改好~~*/
#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#define N0 5
#define M 20
#define N 45
#define Up 72 //上，下，左，右
#define Down 80
#define Left 75
#define Right 77
#define Kong 32 //发射子弹
//#define NUM 5
char arr[M][N];
int arr0[M];
int arr1[M];//记录敌方坦克坐标的数组
int arr_x[M];
int arr_y[M];//记录敌方子弹的数组
int score = 0;
int count_tank = 5;
/**************************FILE NEED**************************************/
struct s
{
	char name[20];
	int myscore;
};
typedef struct m
{
	struct s date;
	struct m* next;
}NODE;                                   
/**************************GAME NEED**************************************/
void HideCursor()// 隐藏光标
{
		CONSOLE_CURSOR_INFO cursor_info = {1, 0}; 
		SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&cursor_info);
}

void gotoxy(int x, int y)//移动坐标
{
    COORD coord = {x - 1, y - 1};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

int initation(char arr[][45])//初始化
{	
	
	int sum=0;
	int i,j;
	srand( time(NULL) );
	while (1)
	{	
		i=rand()%45;//横坐标
		if(i>=2&&i<=44)
			break;
	}
	while(1)
	{
		j=rand()%10;//纵坐标
		if(j>=2&&j<=9)
			break;
	}
		if(arr[j-1][i-1]==32)
		{
			arr[j-1][i-1]=30;
			gotoxy(i,j);
			printf("%c\n",arr[j-1][i-1]);
		}
		else 
		sum = 1;
	return sum;


}
void tip()//右侧提示
{	
	int y=3,m=0,i;
	char kongzhi[]="↑ ↓ ← →分别控制移动方向，空格键发射炮弹\n";
	char guize[]="    首先，不要让你的老家被打爆!当你每干掉敌方一辆tank，你将会获得积分（敌方tank越碉，分越多）。总分数超过20分时将获得胜利	!!!";
	gotoxy(50,y);
	printf("	   坦克小战^_^\n");
	gotoxy(50,y+=1);
	printf("游戏规则：");
	gotoxy(50,y+=2);
	for(i=0;guize[i]!='\0';i++)
	{
		printf("%c",guize[i]);
		m++;
		if(m%29==0)
		{
			y++;
			gotoxy(50,y);
			m=0;
		}
	}
	gotoxy(50,y+=2);
	printf("控制说明：\n");
	gotoxy(50,y+=1);
	for(i=0;kongzhi[i]!='\0';i++)
	{
		printf("%c",kongzhi[i]);
		m++;
		if(m%29==0)
		{
			y++;
			gotoxy(50,y);
			m=0;
		}
	}
}	
void draw_my_oldhome(char arr[][45])/****************************画老家**************************************/
{


}
void range(char arr[][45])// (45*20)  范围 
{
	int i,j;
	for(i=0;i<M;i++)
		for(j=0;j<N;j++)
		{
			if(i==0||i==M-1)
				arr[i][j]=22;
			else if(j==0||j==N-1)
				arr[i][j]=15;
			else 
				arr[i][j]=32;
		}
	for(i=0;i<M;i++)
	{	for(j=0;j<N;j++)
			printf("%c",arr[i][j]);
			printf("\n");
	}
}
//**************************************************************正运行
int switch0(int* x,int *y,int direction)
{
		switch(direction)
					{
						case 0:(*y)++;return 4;
						case 1:(*y)--;return 3;
						case 2:(*x)++;return 1;
						case 3:(*x)--;return 2;
					}
		return 0;
}

int switch1(int* x,int *y,int direction)//右 
{
		switch(direction)
					{
						case 1:(*y)--;return 3;
						case 0:(*y)++;return 4;
						case 2:(*x)--;return 2;	
					}
		return 0;
}
int switch2(int* x,int *y,int direction)//左
{
		switch(direction)
					{
						case 0:(*y)--;return 3;
						case 1:(*y)++;return 4;
						case 2:(*x)++;return 1;					
					}
	return 0;
}
int switch3(int* x,int *y,int direction)//上
{
		switch(direction)
					{
						case 0:(*x)++;return 1;
						case 2:(*y)++;return 4;
						case 1:(*x)--;return 2;					
					}
		return 0;
}
int switch4(int* x,int *y,int direction)//底
{
		switch(direction)
					{
						case 2:(*y)--;return 3;
						case 1:(*x)++;return 1;
						case 0:(*x)--;return 2;				
					}
	return 0;
}
int switch1_1(int* x,int *y,int direction)
{
		switch(direction)
					{					
						case 1:(*y)++;return 4;//
						case 0:(*x)--;return 2;//right					
					}
		return 0;
}
int switch1_2(int* x,int *y,int direction)
{
		switch(direction)
					{					
						case 1:(*y)--;return 3;//up
						case 0:(*x)--;return 2;//right					
					}
		return 0;
}
int switch1_3(int* x,int *y,int direction)
{
		switch(direction)
					{					
						case 1:(*y)--;return 3;//up
						case 0:(*x)++;return 1;//right					
					}
		return 0;
}
int switch1_4(int* x,int *y,int direction)
{
		switch(direction)
					{					
						case 1:(*y)++;return 4;//up
						case 0:(*x)++;return 1;//right
					}
		return 0;
}

//********************************************************反角坐标
int fanswitch1_1(int* x,int *y,int direction)// youshang
{
		switch(direction)
					{					
						case 1:(*y)++;return 4;
						case 0:(*x)--;return 2;			
					}
		return 0;
}
int fanswitch1_2(int* x,int *y,int direction)//youxia
{
		switch(direction)
					{					
						case 0:(*y)--;return 3;
						case 1:(*x)--;return 2;				
					}
		return 0;
}
int fanswitch1_3(int* x,int *y,int direction)//zuoxia
{
		switch(direction)
					{					
						case 1:(*y)--;return 4;
						case 0:(*x)++;return 1;				
					}
		return 0;

}
int fanswitch1_4(int* x,int *y,int direction)//左上角
{
		switch(direction)
					{					
						case 0:(*y)++;return 4;
						case 1:(*x)++;return 1;
					}
		return 0;
}
//***********************************************************
//***********************************************************反边界坐标
int fanswitch0(int* x,int *y,int direction)
{
		switch(direction)
					{
						case 0:(*y)--;return 3;
						case 1:(*y)++;return 4;
						case 2:(*x)--;return 2;
						case 3:(*x)++;return 1;
					}
		return 0;
}

int fanswitch1(int* x,int *y,int direction)//右 
{
		switch(direction)
					{
						case 1:(*y)++;return 4;
						case 0:(*y)--;return 3;
						default:(*x)--;return 2;	
					}
		return 0;
}
int fanswitch2(int* x,int *y,int direction)//左
{
		switch(direction)
					{
						case 0:(*y)++;return 4;
						case 1:(*y)--;return 3;
						default:(*x)++;return 1;					
					}
		return 0;
}
int fanswitch3(int* x,int *y,int direction)//上
{
		switch(direction)
					{
						case 1:(*x)++;return 1;
						case 0:(*x)--;return 2;
						default:(*y)++;return 4;					
					}
		return 0;
}
int fanswitch4(int* x,int *y,int direction)//底
{
		switch(direction)
					{
						
						case 1:(*x)++;return 1;
						case 0:(*x)--;return 2;
						default:(*y)--;return 3;				
					}
		return 0;
}
//*************************************************************
int tank_run(char arr[][45])//一辆坦克
{
		int enemy_bullet_run(char arr[][45],int m);
		int dead_flag = 0;
		int n=0;
		int m=0;
		int i;
		int j;
		int z;
		int flag = 0;
		int direction[N0]={10};//存方向的数组
		int tank_direction[N0] ={10};//坦克面对的方向
		srand((unsigned) time(NULL) );	
		
		for(i=1;i<19;i++)		
			for(j=1;j<44;j++)
				if(arr[i][j]==30||arr[i][j]==31||arr[i][j]==16||arr[i][j]==17)		//记录数组坐标	
				{
					arr0[m]=i;//i+1==y
					arr1[m]=j;//j+1==x
					m++;
				}
			
					for(n=0;n<m;n++)//全部擦除
					{
						arr[  arr0[n]  ][   arr1[n]  ]=32;
						gotoxy(arr1[n]+1,arr0[n]+1);
						printf("%c",arr[  arr0[n]  ][   arr1[n]  ]);
						direction[n]=rand()%8;//生成随机数,不一定每次都运动
					}
					
				for(n=0;n<m;n++)//全部移动 arr1[]+1->x  arr0[]+1 ->y  
				{
					if(arr1[n]+1==44||arr1[n]+1==2||arr0[n]+1==2||arr0[n]+1==19)//处理边界
					{
						if(arr0[n]+1==2&&arr1[n]+1==44)//右上角	arr1[]+1->x  arr0[]+1 ->y
						{
							direction[n]=rand()%2;
							tank_direction[n] = switch1_1(&arr1[n],&arr0[n],direction[n]);
						}
						if(arr0[n]+1==19&&arr1[n]+1==44)//	右下角			
						{
							direction[n]=rand()%2;
							tank_direction[n] =	switch1_2(&arr1[n],&arr0[n],direction[n]);
						}
						if(arr0[n]+1==19&&arr1[n]+1==2)//左下角
						{
							direction[n]=rand()%2;
							tank_direction[n] =	switch1_3(&arr1[n],&arr0[n],direction[n]);
						}
						if(arr0[n]+1==2&&arr1[n]+1==2)//左上角
						{
							direction[n]=rand()%2;
							tank_direction[n] = switch1_4(&arr1[n],&arr0[n],direction[n]);
						}

						if(arr1[n]+1==44&&arr0[n]+1!=19&&arr0[n]+1!=2) // 右边界            arr1[]+1->x  arr0[]+1 ->y
						{
							direction[n]=rand()%3;
							tank_direction[n] = switch1(&arr1[n],&arr0[n],direction[n]);
						}

						if(arr1[n]+1==2&&arr0[n]+1!=19&&arr0[n]+1!=2) // 左边界            arr1[]+1->x  arr0[]+1 ->y
						{
							direction[n]=rand()%3;
							tank_direction[n] = switch2(&arr1[n],&arr0[n],direction[n]);
						}

						if(arr0[n]+1==2&&arr1[n]+1!=2&&arr1[n]+1!=44) //   上边界           arr1[]+1->x  arr0[]+1 ->y
						{
							direction[n]=rand()%3;
							tank_direction[n] = switch3(&arr1[n],&arr0[n],direction[n]);
						}

						if(arr0[n]+1==19&&arr1[n]+1!=2&&arr1[n]+1!=44)  // 下边界            arr1[]+1->x  arr0[]+1 ->y
						{
							direction[n]=rand()%3;
							tank_direction[n] = switch4(&arr1[n],&arr0[n],direction[n]);
						}

					}
				else	//	非边界				 
					{
						tank_direction[n] = switch0(&arr1[n],&arr0[n],direction[n]);
					}
				}
				//*********************************************************************新画
				for(n=0;n<m;n++)
				{		
					for(z=0;z<m;z++)
					if(arr1[n]==arr1[z]&&arr0[n]==arr0[z]&&n!=z)        //arr1[]+1->x  arr0[]+1 ->y	避免重叠
					{
								if(arr1[n]+1==44||arr1[n]+1==2||arr0[n]+1==2||arr0[n]+1==19)//处理边界
							{
								if(arr0[n]+1==2&&arr1[n]+1==44)//右上角	arr1[]+1->x  arr0[]+1 ->y
								{
									tank_direction[n] = fanswitch1_1(&arr1[n],&arr0[n],direction[n]);
								}
								if(arr0[n]+1==19&&arr1[n]+1==44)//	右下角			
								{						
									tank_direction[n] = fanswitch1_2(&arr1[n],&arr0[n],direction[n]);
								}
								if(arr0[n]+1==19&&arr1[n]+1==2)//左下角
								{							
									tank_direction[n] = fanswitch1_3(&arr1[n],&arr0[n],direction[n]);
								}
								if(arr0[n]+1==2&&arr1[n]+1==2)//左上角
								{					
									tank_direction[n] = fanswitch1_4(&arr1[n],&arr0[n],direction[n]);
								}

								if(arr1[n]+1==44&&arr0[n]+1!=19&&arr0[n]+1!=2) // 右边界            arr1[]+1->x  arr0[]+1 ->y
								{					
									tank_direction[n] = fanswitch1(&arr1[n],&arr0[n],direction[n]);
								}

								if(arr1[n]+1==2&&arr0[n]+1!=19&&arr0[n]+1!=2) // 左边界            arr1[]+1->x  arr0[]+1 ->y
								{
									tank_direction[n] = fanswitch2(&arr1[n],&arr0[n],direction[n]);
								}

								if(arr0[n]+1==2&&arr1[n]+1!=2&&arr1[n]+1!=44) //   上边界           arr1[]+1->x  arr0[]+1 ->y
								{
									tank_direction[n] = fanswitch3(&arr1[n],&arr0[n],direction[n]);
								}

								if(arr0[n]+1==19&&arr1[n]+1!=2&&arr1[n]+1!=44)  // 下边界            arr1[]+1->x  arr0[]+1 ->y
								{
									tank_direction[n] = fanswitch4(&arr1[n],&arr0[n],direction[n]);
								}
							}	
								else
								{
									tank_direction[n] = fanswitch0(&arr1[n],&arr0[n],direction[n]);
								}
					}
					switch(tank_direction[n])
					{
					case 1:if(arr[  arr0[n]  ][   arr1[n]  ]=='*')	
								{
									gotoxy(arr1[n]+1,arr0[n]+1);
									printf("X");
									return 10000;
								}
							else arr[  arr0[n]  ][   arr1[n]  ]=16;	break;							
					case 2:if(arr[  arr0[n]  ][   arr1[n]  ]=='*')
								{
									gotoxy(arr1[n]+1,arr0[n]+1);
									printf("X");
									return 10000;
								}
							else arr[  arr0[n]  ][   arr1[n]  ]=17;	break;
					case 3:if(arr[  arr0[n]  ][   arr1[n]  ]=='*')
								{
									gotoxy(arr1[n]+1,arr0[n]+1);
									printf("X");
									return 10000;
								}
							else arr[  arr0[n]  ][   arr1[n]  ]=30;	break;
					case 4:if(arr[  arr0[n]  ][   arr1[n]  ]=='*')	
								{
									gotoxy(arr1[n]+1,arr0[n]+1);
									printf("X");
									return 10000;
								}
							else arr[  arr0[n]  ][   arr1[n]  ]=31;	break;
					default:if(arr[  arr0[n]  ][   arr1[n]  ]=='*')	
								{
									gotoxy(arr1[n]+1,arr0[n]+1);
									printf("X");
									return 10000;
								}
							else arr[  arr0[n]  ][   arr1[n]  ]=30;	break;
					
					}				
						gotoxy(arr1[n]+1,arr0[n]+1);
						printf("%c",arr[  arr0[n]  ][   arr1[n]  ]);
				}
				
			dead_flag=	enemy_bullet_run(arr,m);
			if(dead_flag==1)
			{
				return 10000;
			}
			return 0;
}
int  judge_enemy_death(char arr[][45],int x,int y,int *score)
{
	int flag0 = 0;
	if(arr[y][x]!=32)
	{
		if(arr[y][x]==30||arr[y][x]==31||arr[y][x]==16||arr[y][x]==17)
		{
			arr[y][x]=32;
			gotoxy(x+1,y+1);
			printf("х");
			(*score)++;
			count_tank--;
			flag0 = 1;
		}
		if(arr[y][x]==22||arr[y][x]==15)
		{
			flag0 =  2;	
		}
	}
	return flag0;
}
int enemy_bullet_live(char arr[][45],int x,int y)
{
	int flag_a = 0;
	if(arr[y][x]!=32)
	{
		if(arr[y][x]==22||arr[y][x]==15)
		return 	flag_a = 2;
		if(arr[y][x]==24||arr[y][x]==25||arr[y][x]==26||arr[y][x]==27)
		return  flag_a = 3;
		if(arr[y][x]==16||arr[y][x]==17||arr[y][x]==29||arr[y][x]==30)
		return  flag_a = 4;
		if(arr[y][x]==42)
		return  flag_a = 5;
	}
	return flag_a;

}
int enemy_bullet_run(char arr[][45],int m)///////////////////////////////////////////////////////////////////
{
	int dead_flag = 0;
	int flag_a = 0;
	int i;
	int x;
	int y;
	int j;
	int random_accept = 0;
	srand((unsigned) time(NULL) );	
	for(i=0;i<m;i++)		
	{	y=arr0[i];
		x=arr1[i];					
				switch(arr[  arr0[i]  ][  arr1[i]  ])
			{
			
				case 30:random_accept = rand()%7;
						if(random_accept == 1)
						{
							for(j=0;j<3&&flag_a!=2&&flag_a!=3;j++)
							{
								flag_a = enemy_bullet_live(arr,x,y-1);
								if(flag_a==0)
							{
								arr[  y-1  ][  x  ] = 24;
								gotoxy(x+1,y);
								printf("%c",arr[  y-1  ][  x  ]);
							}
							if(flag_a!=2&&flag_a!=3&&flag_a!=4&&flag_a!=5)
							{
								Sleep(100);
								arr[  y-1  ][  x  ] = 32;
								gotoxy(x+1,y);
								printf("%c",arr[  y-1  ][  x  ]);
								y--;
							}
							if(flag_a==3)
							{
								Sleep(100);
								arr[  y-1  ][  x  ] = 32;
								gotoxy(x+1,y);
								printf("%c",arr[  y-1  ][  x  ]);
								break;

							}
							if(flag_a==5)
							{
								return dead_flag=1;
							}
							}
						}break;
			

				case 31:random_accept = rand()%7;
						if(random_accept == 1)
						{
							for(j=0;j<3&&flag_a!=2&&flag_a!=3;j++)
							{
								flag_a = enemy_bullet_live(arr,x,y+1);
								if(flag_a==0)
								{
									arr[  y+1 ][  x  ] = 25;
									gotoxy(x+1,y+2);
									printf("%c",arr[  y+1   ][  x  ]);
								}
								if(flag_a!=2&&flag_a!=3&&flag_a!=4&&flag_a!=5)
								{
									Sleep(100);
									arr[  y+1  ][  x  ] = 32;
									gotoxy(x+1,y+2);
									printf("%c",arr[  y+1   ][  x  ]);
									y++;
								}
								if(flag_a==3)
								{
									Sleep(100);
									arr[  y+1  ][  x  ] = 32;
									gotoxy(x+1,y+2);
									printf("%c",arr[  y+1   ][  x  ]);
									break;
								}
								if(flag_a==5)
								{
									return dead_flag=1;
								}
							}
						}break;
				case 16:random_accept = rand()%7;
						if(random_accept == 1)
						{
							for(j=0;j<3&&flag_a!=2&&flag_a!=3;j++)
							{
								flag_a = enemy_bullet_live(arr,x+1,y);
								if(flag_a==0)
								{
									arr[  y ][  x+1  ] = 26;
									gotoxy(x+2,y+1);
									printf("%c",arr[  y  ][  x+1  ]);
								}
								if(flag_a!=2&&flag_a!=3&&flag_a!=4&&flag_a!=5)
								{
									Sleep(100);
									arr[  y  ][  x+1  ] = 32;
									gotoxy(x+2,y+1);
									printf("%c",arr[  y ][  x+1  ]);
									x++;
								}
								if(flag_a==3)
								{
									Sleep(100);
									arr[  y  ][  x+1  ] = 32;
									gotoxy(x+2,y+1);
									printf("%c",arr[  y ][  x+1  ]);
									break;

								}
								if(flag_a==5)
								{
									return dead_flag=1;
								}
							}
						}break;
				case 17:random_accept = rand()%7;//letf
						if(random_accept == 1)
						{
							for(j=0;j<3&&flag_a!=2&&flag_a!=3;j++)
							{
									flag_a = enemy_bullet_live(arr,x-1,y);
									if(flag_a==0)
									{
										arr[  y ][  x-1  ] = 27;
										gotoxy(x,y+1);
										printf("%c",arr[  y  ][  x-1  ]);
									}
									if(flag_a!=2&&flag_a!=3&&flag_a!=4&&flag_a!=5)
									{
										Sleep(100);
										arr[  y  ][  x-1  ] = 32;
										gotoxy(x,y+1);
										printf("%c",arr[  y ][  x-1 ]);
										x--;
									}
									if(flag_a==3)
									{
										Sleep(100);
										arr[  y  ][  x-1  ] = 32;
										gotoxy(x,y+1);
										printf("%c",arr[  y ][  x-1 ]);
										break;
									}
									if(flag_a==5)
									{
										return dead_flag=1;
									}
							}
						}break;
				default :break;
			}		
	}
	return dead_flag = 0;

}
void bullet_run(char arr[][45],int flag,int x,int y)
{
	int i;
	int flag0  = 0;

		switch(flag)
		{
		case 1:{
					
				for(i=0;i<3&&flag0!=1&&flag0!=2;i++)
				{					
					flag0 = judge_enemy_death(arr,x-1,y,&score);
					
					if(flag0 == 0 )
					{
						arr[y][x-1]=25;
						gotoxy(x,y+1);
						printf("%c",arr[y][x-1]);
					}
					if(flag0 != 2)
					{	
						Sleep(100);
						arr[y][x-1]=32;
						gotoxy(x,y+1);
						printf("%c",arr[y][x-1]);
						y++;
					}	
				
				}
				break;				
			   }//DOWN
		case 2:{
			
				for(i=0;i<3&&flag0!=1&&flag0!=2;i++)
				{
					flag0 = judge_enemy_death(arr,x-1,y-2,&score);
					if(flag0 == 0)
					{
						arr[y-2][x-1]=24;
						gotoxy(x,y-1);
						printf("%c",arr[y-2][x-1]);
					}
					if(flag0 != 2)
					{
						Sleep(100);				
						arr[y-2][x-1]=32;
						gotoxy(x,y-1);
						printf("%c",arr[y-2][x-1]);	
						y--;
					}
					
				}
				break;
			   }	// UP		
		case 3:
				for(i=0;i<4&&flag0!=2;i++)
				{
					flag0 = judge_enemy_death(arr,x,y-1,&score);
					if(flag0 == 0)
					{
						arr[y-1][x]=26;
						gotoxy(x+1,y);
						printf("%c",arr[y-1][x]);
					}
					if(flag0 != 2)
					{
						Sleep(100);
						arr[y-1][x]=32;
						gotoxy(x+1,y);
						printf("%c",arr[y-1][x]);
						x++;
					}
				}
				break;//RIGHT			
		case 4:	
				for(i=0;i<4&&flag0!=2;i++)
				{
					flag0 = judge_enemy_death(arr,x-2,y-1,&score);
					if(flag0 == 0)
					{
						arr[y-1][x-2]=27;
						gotoxy(x-1,y);
						printf("%c",arr[y-1][x-2]);
					}
					if(flag0 != 2)
					{
						Sleep(100);
						arr[y-1][x-2]=32;
						gotoxy(x-1,y);
						printf("%c",arr[y-1][x-2]);	
						x--;
					}
				}
				break;//LEFT
		default: break;
		}	
}
int  control(char arr[][45],int n,int *x,int *y)
{

	int gameover_flag0 = 1;
	static	int flag = 2 ;
	char ch;
	int a;
//	static int  x=20;
//  static int y=15;
	if(n==0)
	{
		arr[*y-1][*x-1]='*';
		gotoxy(*x,*y);
		printf("%c",arr[*y-1][*x-1]);
	}
	while(kbhit())
	{
		if(	(ch=getch())==-32)
		{	ch=getch();
			a=ch;	
			arr[*y-1][*x-1] = 32 ;
			gotoxy(*x,*y);
			printf("%c",arr[*y-1][*x-1]);
		switch(a)
			{
					case Down:(*y)++;flag = 1;break;//down      //flag用来判断方向
					case Up:(*y)--;flag = 2;break;//up
					case Right:(*x)++;flag = 3;break;//right
					case Left:(*x)--;flag = 4;break;//left
					default:break;
			}	
			if(arr[*y-1][*x-1]==32)
			{
				arr[*y-1][*x-1]='*';
				gotoxy(*x,*y);
				printf("%c",arr[*y-1][*x-1]);
			}
			else if(arr[*y-1][*x-1]==30||arr[*y-1][*x-1]==31||arr[*y-1][*x-1]==16||arr[*y-1][*x-1]==17)
			{
				return gameover_flag0 = 10000;
			}
			else if(arr[*y-1][*x-1]==22||arr[*y-1][*x-1]==15)
			{
				switch(a)
				{
					case Down:(*y)--;flag = 1;break;//down      //flag用来判断方向
					case Up:(*y)++;flag = 2;break;//up
					case Right:(*x)--;flag = 3;break;//right
					case Left:(*x)++;flag = 4;break;//left
					default:break;
				}
				arr[*y-1][*x-1]='*';
				gotoxy(*x,*y);
				printf("%c",arr[*y-1][*x-1]);
			}
		}
		else 
			if(ch==Kong)
			bullet_run(arr,flag,*x,*y);
	}
	return gameover_flag0 = 1;
}
int Count(NODE *head)
{
	int count = 0;
	NODE *p = head->next;
	for(; p != NULL ; p = p->next)
	{
		count++;
	}
	return count;
}
NODE *Read()
{
	FILE *fp = NULL;
	int nCount = 0;
	NODE *head = NULL,* p1 = NULL ,*p2 = NULL;
	fp = fopen("game's file.dat","r");
	if(fp == NULL)
	{
		gotoxy(10,2);
		printf("打开积分榜失败：\n");
	}
	if(fp == NULL)
		return head;
	head = (NODE *)malloc(sizeof(NODE));
	head->next = NULL;
	while(1)
	{
		p1 = (NODE *)malloc(sizeof(NODE));
		p1->next = NULL;
		fread(p1,sizeof(struct s),1,fp);
		if(feof(fp))
			break;
		if(head->next == NULL)
			head->next = p1;
		else 
		{
			p2->next = p1;

		}
		p2 = p1;
	}
	fclose(fp);
	return head;
}
NODE *create()
{
//	char n;
	NODE *head = NULL,*p1 = NULL,*p2 = NULL;
	head = (NODE *)malloc(sizeof(NODE));
	head->next = NULL;
	while(1)
	{
		p1 = (NODE *)malloc(sizeof(NODE));
		p1->next = NULL;
		system("cls");
		range(arr);
		gotoxy(10,2);
		printf("请输入玩家姓名：Enter创建并保存");
		gotoxy(10,3);
		fflush(stdin);

		scanf("%s",p1->date.name);
		p1->date.myscore = score;
		if(head->next == NULL)
		{
			head->next = p1;
		}
		else 
		{
			p2->next = p1;

		}
		p2 = p1;
		gotoxy(10,4);
		printf("创建成功!!!");
		Sleep(1000);
		gotoxy(10,5);
		printf("按任意键返回上级菜单:");
		gotoxy(10,6);
		getch();
		break;

	}
	return head;
}
void Write(NODE *head)
{
	FILE *fp;
	NODE * p1 = head->next;
//	if(p1 == NULL)
	fp = fopen("game's file.dat","w");
//	if(p1 != NULL)
//	fp = fopen("game's file.dat","a");
	if(fp == NULL)
		return;
	for(;p1 != NULL; p1 = p1->next)
	{
		fwrite(p1,sizeof(struct s),1,fp);
	}
	fclose(fp);
}
void display(NODE *head)
{
	int x = 10,y = 3;
	NODE *p = head->next;
		system("cls");
		range(arr);
	gotoxy(10,2);
	printf("积分榜：\n");
	for(;p != NULL;p = p->next)
	{
		gotoxy(x,y);
		printf("姓名：%s",p->date.name);
		y++;
		gotoxy(x,y);
		printf("分数：%d",p->date.myscore);

	}
	gotoxy(10,11);
	printf("按任意键返回上级菜单:");
	gotoxy(10,12);
	getch();

}
void charu(NODE *head)
{
	NODE *p = head,*p1 = NULL;
//	char n;
	for(;p->next != NULL;p = p->next)
		;
	while(1)
	{
		p1 = (NODE *)malloc(sizeof(NODE));
		p1 ->next = NULL;
		system("cls");
			range(arr);//范围
		gotoxy(10,2);
		printf("请输入玩家姓名：Enter创建并保存:");
		fflush(stdin);
		gotoxy(10,3);
		scanf("%s",p1->date.name);
		p1->date.myscore = score;
		if(head ->next == NULL)
			head->next = p1;
		else 
		{
			p->next = p1;

		}
		gotoxy(10,4);
		printf("创建成功!!!");
		Sleep(1000);
		gotoxy(10,5);
		printf("按任意键返回上级菜单:");
		getch();
		break;
		
	}
	


}
int shanchu1(NODE* head,char id[])//具体到名字
{
	int flag =0;
	NODE* p=head;
	NODE* p1=head;
	p=p1->next;
	
	for(;p!=NULL;)
	{
		
		if(strcmp(p->date.name,id)==0)
		{
			flag = 1;
			p1->next=p->next;
			free(p);
			break;
		}
		p1=p1->next;
		p=p1->next;
	}

	return flag;
}

void main()
{	
	/**************************GAME NEED******************************/
	int gameover_flag0 = 1;
	int gameover_flag = 0;
	int x = 20,y = 15;
	int n=0;
	int i;
	int exit = 1;
	int sum;
	int count_m;
	/**************************FILE NEED*****************************/
	char id[20];
//	int nn;
	int fflag;
	int sel,nCount;
	NODE *pp;
	NODE *head = NULL;
	FILE *fp = NULL;
	fp = fopen("game's file.dat","r");
		if(fp == NULL)
		{
			system("cls");
			range(arr);
			gotoxy(10,2);
			printf("欢迎首次体验坦克小战!!\n");
			gotoxy(10,3);
			printf("本游戏纯属自娱自乐，要求不要太高哦~\n");
			fp = fopen("game's file.dat","w");
			nCount = 0;
			fwrite((void *)&nCount,sizeof(int),1,fp);
			fclose(fp);
			gotoxy(10,4);
			printf("按任意键继续...\n");
			getch();
			gotoxy(10,5);
	
			for(;;)
			{
			system("cls");
			range(arr);//范围
			gotoxy(10,2);
			printf("选择操作:(按相应的数字键选择)");
			gotoxy(10,3);
			printf("1-新建用户：");
			gotoxy(10,4);
			printf("2-显示积分榜:");
			gotoxy(10,5);
			printf("4-开始游戏：");
			gotoxy(10,6);
			printf("0-退出:");
			gotoxy(10,7);
		
				scanf("%d",&sel);
				if(sel == 4)
					break;
				if(sel == 0)
					break;
				switch(sel)
				{
				
				case 2:display(head);break;
				case 1:head = create();/*Write(head)*/;break;
				default:break;
				}
			}

		}
		else 
		{
			
		for(;;)
		{	head = Read();
			system("cls");
				range(arr);
				gotoxy(10,2);
				printf("欢迎体验坦克小战游戏：");
					fp = fopen("game's file.dat","r");
				nCount = 0;
				fwrite((void*)&nCount, sizeof(int), 1, fp);
				fclose(fp);
		
			gotoxy(10,3);
			printf("选择操作：");
			gotoxy(10,4);
			printf("1-新建玩家：");
			gotoxy(10,5);
			printf("2-显示积分榜：");
			gotoxy(10,6);
			printf("3-删除玩家积分：");

			gotoxy(10,7);
			printf("4-开始游戏：");
			gotoxy(10,8);
			printf("0-退出");
			gotoxy(10,9);
			scanf("%d",&sel);
			if(sel == 4)
				break;
			if(sel == 0)
				break;
			switch(sel)
			{
			case 1:charu(head);/*Write(head)*/;break;
			case 2:display(head);break;
			case 3:	
				system("cls");
			range(arr);//范围
			gotoxy(10,2);
				printf("请输入要删除的数据的姓名 \n");
							fflush(stdin);
							gotoxy(10,3);
							gets(id);
							fflag = shanchu1(head,id);
							if(fflag == 1)
							{
								gotoxy(10,5);
								printf("删除成功！ \n");
								Sleep(2000);
								Write(head);
								gotoxy(10,6);
								printf("按任意键返回上级菜单：");
								gotoxy(10,7);
								fflush(stdin);
								getch();
								break;
							}
							else 
							{
								gotoxy(10,5);
								printf("无效数据，删除失败！ \n");
								gotoxy(10,6);
								printf("按任意键返回上级菜单：");
								gotoxy(10,7);
								fflush(stdin);
								getch();
								break;
							}
		
			default:break;
			}
		}
		}
	/**************************FILE NEED*****************************/
	if(sel == 4)
	{
	fclose(fp);
	srand((unsigned) time(NULL) );
	HideCursor();//隐藏光标
	system("cls");
	range(arr);//范围

	tip();//提示
	for(i=0;i<N0;i++)// 敌方坦克
	{
		sum = 0;
		sum = initation(arr);
		i-=sum;
	}
	Sleep(1000);
	while(score<5)
	{
		gameover_flag0 = control(arr,n++,&x,&y);
		Sleep(200);
		gameover_flag = tank_run(arr);
		if(gameover_flag==10000||gameover_flag0==10000)
			break;
		gotoxy(50,17);
		printf("Your score:%d",score);
			if(count_tank<5)
			{
				count_m=rand()%3;
				for(i=0;i<count_m;i++)
				{
					initation(arr);
					count_tank++;
				}
				if(count_tank<3)
				{
					initation(arr);
					count_tank++;
				}
			}
	}
		system("cls");
		range(arr);
		gotoxy(10,10);
	
		if(gameover_flag==0&&gameover_flag0!=10000)
		{
			gotoxy(8,10);
			Sleep(1000);
			printf("Congratulations,You are winner!(胜利~)");
			gotoxy(10,11);
			printf("o(≧v≦)o~~Hayyp New Near\n");
		}
		if(gameover_flag==10000||gameover_flag0==10000)	
		{	
			Sleep(1000);
			printf("Sorry,you are loser(失败~)\n");
			gotoxy(10,11);
			printf("^(*￣(oo)￣)^Happy New Near\n");
		}
		head->next->date.myscore = score;
//		pp = head->next;
//		for(;pp->next == NULL; pp = pp->next)
			;
//		pp->date.myscore = score;
		Write(head);
		
	//	fwrite((void *)&score,sizeof(int),1,fp);
	}
		gotoxy(10,15);
		printf("按任意键退出：");
		gotoxy(10,16);
		fflush(stdin);
		getch();
}
