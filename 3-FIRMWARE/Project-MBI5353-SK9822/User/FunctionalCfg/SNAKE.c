/*************************************************************************************************/
/*文件：贪吃蛇										                                                               */
/*记录：By Zouqb@2021.7.7																																				 */
/*************************************************************************************************/

#include "SNAKE.h"
#include "MENU.h"

pSnake S0;							//=== SnakePoint0定义蛇头指针
pSnake SF;							//=== SnakeFood定义蛇的食物

/*************************************************************************************************/
/* 功能：初始化蛇头蛇身食物																																			 */
/* 输入：蛇颜色     	                     																											 */
/* 输出：无			                                                                                 */
/* 描述：蛇头初始坐标 颜色 食物               																									 */
/*************************************************************************************************/
void SnakeInit(TYPE_COLOR *p)
{
		pSnake pS;
		
		//=== 0、分配蛇头内存空间，标记蛇尾节点为空，分配食物内存
		S0 = (pSnake)malloc(sizeof(Snake));
		S0->Pos.Sts = 0;
		S0->Pos.X = RMAXX >> 1;
		S0->Pos.Y = RMAXY >> 1;
		S0->Color = *p;
		S0->Next = NULL;
		
		SF = (pSnake)malloc(sizeof(Snake));
		SF->Pos.Sts = 0;
		SF->Pos.X = (RMAXX >> 1);
		SF->Pos.Y = (RMAXY >> 2) - (RMAXY >> 3);
		SF->Color.R = 0x50;
		SF->Color.G = 0x00;
		SF->Color.B = 0x50;
		SF->Next = NULL;
		
		//=== 1、指向蛇尾节点
		pS = S0;
		while(pS->Next)
		{
				pS = pS->Next;
		}
		
		//=== 2、蛇带两节身子
		for(Uint8 i = 1; i <= 2; i++)
		{
				pS->Next	= (pSnake)malloc(sizeof(Snake));	//== 第一个节点分配内存
				pS = pS->Next;															//== 指向第一个节点的数据域
				pS->Pos.Sts = 0;														//== 第一个节点开始分配数据信息	将蛇初始化在旋转LED上			
				pS->Pos.X = (RMAXX >> 1) - i;								
				pS->Pos.Y =  RMAXY >> 1;
				pS->Color.R = 0x00;
				pS->Color.G = 0xFF - i*(0xFF >> 2);
				pS->Color.B = 0x00;
		}
		
		//=== 3、最后一个节点收尾
		pS->Next = NULL;
		
		//=== 4、显示初始化后的贪吃蛇
		pS = S0;
		while(pS)
		{
				LED.RBuf[pS->Pos.X][pS->Pos.Y] = pS->Color;
				
				pS = pS->Next;
		}
		
		//=== 5、显示初始化后的食物
		LED.RBuf[SF->Pos.X][SF->Pos.Y] = SF->Color;
}

Uint8 First = 0;

/*************************************************************************************************/
/* 功能：随机生成食物    																																				 */
/* 输入：无                         																											 			 */
/* 输出：无			                                                                                 */
/* 描述：食物的坐标和颜色生成(食物被吃掉后重新调用一次)																					 */
/*************************************************************************************************/
void SnakeFoodGen(void)
{
		pSnake ST = S0;			//== SnakeTemp创建临时节点等于蛇头

		//=== 0、提供随机数种子
		if(First == 0)
		{
				First = 1;
				srand(TIM2->CNT);
		}
		
		//=== 1、先擦掉之前显示的食物(根据当前空间)(没必要，每次都是被吃才生成)


		//=== 2、生成食物的随机坐标(生成的食物必须不在蛇身上)，随机颜色
Genagain:
//		SF = (pSnake)malloc(sizeof(Snake));
		SF->Pos.Sts = (TIM2->CNT >> 8) > 0x7F;//(rand() >> 1) > 0xFF;
		
		if(SF->Pos.Sts == 0)
		{
				SF->Pos.X = rand() % RMAXX;		//== 这个生成有问题
				SF->Pos.Y = rand() % RMAXY;
				SF->Color.R = rand() >> 9;		//== 计数器TIM2->CNT为16位
				SF->Color.G = rand() >> 9;
				SF->Color.B = rand() >> 9;
		}
		else
		{
				SF->Pos.X = rand() % LMAXX;
				SF->Pos.Y = rand() % LMAXY;
				SF->Color.R = rand() >> 9;		//== 计数器TIM2->CNT为16位
				SF->Color.G = rand() >> 9;
				SF->Color.B = rand() >> 9;
		}
		
		//=== 3、遍历蛇身所有节点确定是否与食物重合，重合后重新生成食物
		ST = S0;
		while(ST)
		{
				if((ST->Pos.Sts == SF->Pos.Sts) &&
					 (ST->Pos.X	  == SF->Pos.X) 	&&
					 (ST->Pos.Y	  == SF->Pos.Y))
				{
						goto Genagain;
				}				 
				ST = ST->Next;
		}
		
		SF->Next = NULL;
		
		//=== 4、显示该坐标点
		if(SF->Pos.Sts == 0)
		{
				LED.RBuf[SF->Pos.X][SF->Pos.Y] = SF->Color;
		}
		else
		{
				LED.LBuf[SF->Pos.X] |= (0x0001 << SF->Pos.Y);
		}
}


/*************************************************************************************************/
/* 功能：蛇的运动		    																																				 */
/* 输入：运动方向                         																											 */
/* 输出：无			                                                                                 */
/* 描述：0-每次运动前进行自检,确认是否咬到自己																									 */
/*			 1-是否撞墙(先无视，后续要增加空间跳跃)																									 */
/*			 2-该函数每调用一次贪吃蛇便向指定方向前进一步																		 				 */																 
/*************************************************************************************************/
void SnakeRun(Uint8 Dir)
{
		int8 DeltX = 0, DeltY = 0, Sts = 0;
		pSnake ST = S0;			//== 创建临时节点等于蛇头
		pSnake S0N = NULL;	//== 创建新的蛇头
		
		S0N = (pSnake)malloc(sizeof(Snake));
		
		//=== 0、指向蛇身第一节
		ST = ST->Next;
		
		//=== 1、遍历蛇身所有节点确认是否与蛇头重叠，重叠后暂停游戏并退出，置贪吃蛇结束状态
		while(ST)
		{
				if((ST->Pos.Sts == S0->Pos.Sts) &&
					 (ST->Pos.X	  == S0->Pos.X) 	&&
					 (ST->Pos.Y	  == S0->Pos.Y))
				{
						Menu.Flag.bit.SnakeGameOver = 1;
						return;
				}				 
				ST = ST->Next;
		}
		
		//=== 2、根据方向生成位移
		switch(Dir & 0x0F)
		{
				case UP:			//== 向上运动(之前不是向下运动)
											DeltY = -1;
											break;
				case DOWN: 		//== 向下运动
											DeltY = 1;
											break;
				case LEFT: 		//== 向左运动 初始蛇身在蛇头左侧
											DeltX = -1;
											break;
				case RIGHT:		//== 向右运动
											DeltX = 1;
											break;
				default:			//== 不运动
											//SN0->Pos.X = S0->Pos.X;
											return;
		}
		
		//=== 3、游戏未结束，判断蛇头是否吃到食物(重合)
		if((SF->Pos.Sts == S0->Pos.Sts) &&
			 (SF->Pos.X	  == S0->Pos.X) 	&&
			 (SF->Pos.Y	  == S0->Pos.Y))
		{
				//== (1)吃到食物，蛇头移动，同时旧蛇头直接变成食物
				Sts = S0->Pos.Sts;	//== 记录旧蛇头的空间状态
				if(S0->Pos.Sts == 0)
				{
						if(S0->Pos.Y == 0 && DeltY < 0) 
						{					
								S0->Pos.Sts = 1;
								S0N->Pos.Y = LMAXY - 1;
						}
						else if(S0->Pos.Y == (RMAXY-1) && DeltY > 0)
						{
								S0->Pos.Sts = 1;
								S0N->Pos.Y = 0;		
						}						
						else		S0N->Pos.Y = S0->Pos.Y + DeltY;
						
								 if(S0->Pos.X == 0 		 		 && DeltX < 0) S0N->Pos.X = RMAXX - 1;
						else if(S0->Pos.X == (RMAXX-1) && DeltX > 0) S0N->Pos.X = 0;							
						else																		 		 S0N->Pos.X = S0->Pos.X + DeltX;
				}
				else
				{
								 if(S0->Pos.X == 0 		 		 && DeltX < 0) S0N->Pos.X = RMAXX - 1;
						else if(S0->Pos.X == (RMAXX-1) && DeltX > 0) S0N->Pos.X = 0;							
						else																		 		 S0N->Pos.X = S0->Pos.X + DeltX;
						
						if(S0->Pos.Y == 0 && DeltY < 0) 
						{
								S0->Pos.Sts = 0;
								S0N->Pos.Y = RMAXY - 1;
						}
						else if(S0->Pos.Y == (LMAXY-1) && DeltY > 0)
						{
								S0->Pos.Sts = 0;
								S0N->Pos.Y = 0;	
						}						
						else	S0N->Pos.Y = S0->Pos.Y + DeltY;
				}
				S0N->Pos.Sts = S0->Pos.Sts;
				S0->Pos.Sts = Sts;
				S0N->Color = S0->Color;
				S0->Color = SF->Color;
				
				//== (2)新蛇头生成S0
				S0N->Next = S0;
				S0 = S0N;	
				
				//== (3)新蛇头及新蛇身节点绘制		
				if(S0->Pos.Sts == 0)
				{
						LED.RBuf[S0->Pos.X][S0->Pos.Y] = S0->Color;
				}
				else
				{
						LED.LBuf[S0->Pos.X] |= (0x0001 << S0->Pos.Y);
				}
				if(S0->Next->Pos.Sts == 0)
				{
						LED.RBuf[S0->Next->Pos.X][S0->Next->Pos.Y ] = SF->Color;
				}
				else
				{
						LED.LBuf[S0->Pos.X] |= (0x0001 << S0->Pos.Y);
				}
				
				//== (4)创建新的食物
				SnakeFoodGen();
		}
		//=== 未吃到食物，贪吃蛇向指定位置位移
		else
		{
				//== (1)蛇头移动，移动后继承原蛇头颜色属性
				Sts = S0->Pos.Sts;		//== 记录旧蛇头的空间状态
				if(S0->Pos.Sts == 0)	//== 球形显示屏
				{
								 if(S0->Pos.X == 0 		 		 && DeltX < 0) S0N->Pos.X = RMAXX - 1;
						else if(S0->Pos.X == (RMAXX-1) && DeltX > 0) S0N->Pos.X = 0;							
						else																		 		 S0N->Pos.X = S0->Pos.X + DeltX;
//								 if(S0->Pos.Y == 0 		 && DeltY < 0) S0N->Pos.Y = RMAXY - 1;
//						else if(S0->Pos.Y == RMAXY && DeltY > 0) S0N->Pos.Y = 0;							
//						else																		 S0N->Pos.Y = S0->Pos.Y + DeltY;
						
						if(S0->Pos.Y == 0 && DeltY < 0) 
						{					
								S0->Pos.Sts = 1;
								
								//=== 按比例进行X坐标变换
								S0N->Pos.X = (Uint16)S0N->Pos.X * (LMAXX - 0) / (RMAXX - 0);
								S0N->Pos.Y = LMAXY - 1;
						}
						else if(S0->Pos.Y == (RMAXY-1) && DeltY > 0)
						{
								S0->Pos.Sts = 1;
								
								//=== 按比例进行X坐标变换
								S0N->Pos.X = (Uint16)S0N->Pos.X * (LMAXX - 0) / (RMAXX - 0);
								S0N->Pos.Y = 0;		
						}						
						else		S0N->Pos.Y = S0->Pos.Y + DeltY;
				}
				else		//== 平面显示屏
				{
								 if(S0->Pos.X == 0 		 		 && DeltX < 0) S0N->Pos.X = LMAXX - 1;
						else if(S0->Pos.X == (LMAXX-1) && DeltX > 0) S0N->Pos.X = 0;							
						else																		 		 S0N->Pos.X = S0->Pos.X + DeltX;
						
						
						if(S0->Pos.Y == 0 && DeltY < 0) 
						{
								S0->Pos.Sts = 0;
								
								//=== 按比例进行X坐标变换
								S0N->Pos.X = (Uint16)S0N->Pos.X * (RMAXX - 0) / (LMAXX - 0);
								S0N->Pos.Y = RMAXY - 1;
						}
						else if(S0->Pos.Y == (LMAXY-1) && DeltY > 0)
						{
								S0->Pos.Sts = 0;
								
								//=== 按比例进行X坐标变换
								S0N->Pos.X = (Uint16)S0N->Pos.X * (RMAXX - 0) / (LMAXX - 0);
								S0N->Pos.Y = 0;	
						}						
						else	S0N->Pos.Y = S0->Pos.Y + DeltY;
				}
				S0N->Pos.Sts = S0->Pos.Sts;
				S0->Pos.Sts = Sts;
				S0N->Color = S0->Color;
				
				//== (2)新蛇头生成
				S0N->Next = S0;
				S0 = S0N;				
				
				//== (3)新蛇头及新蛇身节点绘制		
				if(S0->Pos.Sts == 0)
				{
						LED.RBuf[S0->Pos.X][S0->Pos.Y] = S0->Color;
				}
				else
				{
						LED.LBuf[S0->Pos.X] |= (0x0001 << S0->Pos.Y);
				}
				
				//== (4)遍历之前的所有节点，同时蛇身向蛇头方向前进一步(颜色继承)
				ST = S0;
				while(ST->Next->Next)
				{
						ST->Next->Color = ST->Next->Next->Color;
						
						if(ST->Next->Pos.Sts == 0)
						{
								LED.RBuf[ST->Next->Pos.X][ST->Next->Pos.Y] = ST->Next->Color;
						}
						else
						{
								LED.LBuf[ST->Next->Pos.X] |= (0x0001 << ST->Next->Pos.Y);
						}
						
						ST = ST->Next;
				}
				
				//== (5)删除蛇尾 删除临时生成的节点(已分配内存)
				if(ST->Next->Pos.Sts == 0)
				{
						LED.RBuf[ST->Next->Pos.X][ST->Next->Pos.Y].R = 0;
						LED.RBuf[ST->Next->Pos.X][ST->Next->Pos.Y].G = 0;
						LED.RBuf[ST->Next->Pos.X][ST->Next->Pos.Y].B = 0;
				}
				else
				{
						LED.LBuf[ST->Next->Pos.X] &= ~(0x0001 << ST->Next->Pos.Y);
				}
				free(ST->Next);
				ST->Next = NULL;
//				free(S0N);
//				S0N->Next = NULL;
		}
		
		//=== end：记录本次运动方向
		Menu.Flag.bit.SnakeRunDirOld = Menu.Flag.bit.SnakeRunDir;
}

/*************************************************************************************************/
/* 功能：复位游戏																																			 */
/* 输入：无     	                     																											 */
/* 输出：无			                                                                                 */
/* 描述：复位贪吃蛇和食物，同时擦除显示              																									 */
/*************************************************************************************************/
void SnakeReset(void)
{
		pSnake ST = S0;
		
		Menu.Flag.bit.SnakeGameOver = 0;
		
		//== 1、先擦除贪吃蛇
		while(ST)
		{
				if(ST->Pos.Sts == 0)
				{
						LED.RBuf[ST->Pos.X][ST->Pos.Y].R = 0;
						LED.RBuf[ST->Pos.X][ST->Pos.Y].G = 0;
						LED.RBuf[ST->Pos.X][ST->Pos.Y].B = 0;
				}
				else
				{
						LED.LBuf[ST->Pos.X] &= ~(0x0001 << ST->Pos.Y);
				}
				ST = ST->Next;
		}
		
		//== 2、再擦除食物
		if(SF->Pos.Sts == 0)
		{
				LED.RBuf[SF->Pos.X][SF->Pos.Y].R = 0;
				LED.RBuf[SF->Pos.X][SF->Pos.Y].G = 0;
				LED.RBuf[SF->Pos.X][SF->Pos.Y].B = 0;
		}
		else
		{
				LED.LBuf[SF->Pos.X] &= ~(0x0001 << SF->Pos.Y);
		}
		
		//== 3、再复位链表
		if(S0)
		{
				free(S0);
				S0->Next = NULL;
		}
		if(SF)
		{
				free(SF);
				SF->Next = NULL;
		}
}
