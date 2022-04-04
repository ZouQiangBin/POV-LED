/*************************************************************************************************/
/*�ļ���̰����										                                                               */
/*��¼��By Zouqb@2021.7.7																																				 */
/*************************************************************************************************/

#include "SNAKE.h"
#include "MENU.h"

pSnake S0;							//=== SnakePoint0������ͷָ��
pSnake SF;							//=== SnakeFood�����ߵ�ʳ��

/*************************************************************************************************/
/* ���ܣ���ʼ����ͷ����ʳ��																																			 */
/* ���룺����ɫ     	                     																											 */
/* �������			                                                                                 */
/* ��������ͷ��ʼ���� ��ɫ ʳ��               																									 */
/*************************************************************************************************/
void SnakeInit(TYPE_COLOR *p)
{
		pSnake pS;
		
		//=== 0��������ͷ�ڴ�ռ䣬�����β�ڵ�Ϊ�գ�����ʳ���ڴ�
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
		
		//=== 1��ָ����β�ڵ�
		pS = S0;
		while(pS->Next)
		{
				pS = pS->Next;
		}
		
		//=== 2���ߴ���������
		for(Uint8 i = 1; i <= 2; i++)
		{
				pS->Next	= (pSnake)malloc(sizeof(Snake));	//== ��һ���ڵ�����ڴ�
				pS = pS->Next;															//== ָ���һ���ڵ��������
				pS->Pos.Sts = 0;														//== ��һ���ڵ㿪ʼ����������Ϣ	���߳�ʼ������תLED��			
				pS->Pos.X = (RMAXX >> 1) - i;								
				pS->Pos.Y =  RMAXY >> 1;
				pS->Color.R = 0x00;
				pS->Color.G = 0xFF - i*(0xFF >> 2);
				pS->Color.B = 0x00;
		}
		
		//=== 3�����һ���ڵ���β
		pS->Next = NULL;
		
		//=== 4����ʾ��ʼ�����̰����
		pS = S0;
		while(pS)
		{
				LED.RBuf[pS->Pos.X][pS->Pos.Y] = pS->Color;
				
				pS = pS->Next;
		}
		
		//=== 5����ʾ��ʼ�����ʳ��
		LED.RBuf[SF->Pos.X][SF->Pos.Y] = SF->Color;
}

Uint8 First = 0;

/*************************************************************************************************/
/* ���ܣ��������ʳ��    																																				 */
/* ���룺��                         																											 			 */
/* �������			                                                                                 */
/* ������ʳ����������ɫ����(ʳ�ﱻ�Ե������µ���һ��)																					 */
/*************************************************************************************************/
void SnakeFoodGen(void)
{
		pSnake ST = S0;			//== SnakeTemp������ʱ�ڵ������ͷ

		//=== 0���ṩ���������
		if(First == 0)
		{
				First = 1;
				srand(TIM2->CNT);
		}
		
		//=== 1���Ȳ���֮ǰ��ʾ��ʳ��(���ݵ�ǰ�ռ�)(û��Ҫ��ÿ�ζ��Ǳ��Բ�����)


		//=== 2������ʳ����������(���ɵ�ʳ����벻��������)�������ɫ
Genagain:
//		SF = (pSnake)malloc(sizeof(Snake));
		SF->Pos.Sts = (TIM2->CNT >> 8) > 0x7F;//(rand() >> 1) > 0xFF;
		
		if(SF->Pos.Sts == 0)
		{
				SF->Pos.X = rand() % RMAXX;		//== �������������
				SF->Pos.Y = rand() % RMAXY;
				SF->Color.R = rand() >> 9;		//== ������TIM2->CNTΪ16λ
				SF->Color.G = rand() >> 9;
				SF->Color.B = rand() >> 9;
		}
		else
		{
				SF->Pos.X = rand() % LMAXX;
				SF->Pos.Y = rand() % LMAXY;
				SF->Color.R = rand() >> 9;		//== ������TIM2->CNTΪ16λ
				SF->Color.G = rand() >> 9;
				SF->Color.B = rand() >> 9;
		}
		
		//=== 3�������������нڵ�ȷ���Ƿ���ʳ���غϣ��غϺ���������ʳ��
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
		
		//=== 4����ʾ�������
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
/* ���ܣ��ߵ��˶�		    																																				 */
/* ���룺�˶�����                         																											 */
/* �������			                                                                                 */
/* ������0-ÿ���˶�ǰ�����Լ�,ȷ���Ƿ�ҧ���Լ�																									 */
/*			 1-�Ƿ�ײǽ(�����ӣ�����Ҫ���ӿռ���Ծ)																									 */
/*			 2-�ú���ÿ����һ��̰���߱���ָ������ǰ��һ��																		 				 */																 
/*************************************************************************************************/
void SnakeRun(Uint8 Dir)
{
		int8 DeltX = 0, DeltY = 0, Sts = 0;
		pSnake ST = S0;			//== ������ʱ�ڵ������ͷ
		pSnake S0N = NULL;	//== �����µ���ͷ
		
		S0N = (pSnake)malloc(sizeof(Snake));
		
		//=== 0��ָ�������һ��
		ST = ST->Next;
		
		//=== 1�������������нڵ�ȷ���Ƿ�����ͷ�ص����ص�����ͣ��Ϸ���˳�����̰���߽���״̬
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
		
		//=== 2�����ݷ�������λ��
		switch(Dir & 0x0F)
		{
				case UP:			//== �����˶�(֮ǰ���������˶�)
											DeltY = -1;
											break;
				case DOWN: 		//== �����˶�
											DeltY = 1;
											break;
				case LEFT: 		//== �����˶� ��ʼ��������ͷ���
											DeltX = -1;
											break;
				case RIGHT:		//== �����˶�
											DeltX = 1;
											break;
				default:			//== ���˶�
											//SN0->Pos.X = S0->Pos.X;
											return;
		}
		
		//=== 3����Ϸδ�������ж���ͷ�Ƿ�Ե�ʳ��(�غ�)
		if((SF->Pos.Sts == S0->Pos.Sts) &&
			 (SF->Pos.X	  == S0->Pos.X) 	&&
			 (SF->Pos.Y	  == S0->Pos.Y))
		{
				//== (1)�Ե�ʳ���ͷ�ƶ���ͬʱ����ͷֱ�ӱ��ʳ��
				Sts = S0->Pos.Sts;	//== ��¼����ͷ�Ŀռ�״̬
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
				
				//== (2)����ͷ����S0
				S0N->Next = S0;
				S0 = S0N;	
				
				//== (3)����ͷ��������ڵ����		
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
				
				//== (4)�����µ�ʳ��
				SnakeFoodGen();
		}
		//=== δ�Ե�ʳ�̰������ָ��λ��λ��
		else
		{
				//== (1)��ͷ�ƶ����ƶ���̳�ԭ��ͷ��ɫ����
				Sts = S0->Pos.Sts;		//== ��¼����ͷ�Ŀռ�״̬
				if(S0->Pos.Sts == 0)	//== ������ʾ��
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
								
								//=== ����������X����任
								S0N->Pos.X = (Uint16)S0N->Pos.X * (LMAXX - 0) / (RMAXX - 0);
								S0N->Pos.Y = LMAXY - 1;
						}
						else if(S0->Pos.Y == (RMAXY-1) && DeltY > 0)
						{
								S0->Pos.Sts = 1;
								
								//=== ����������X����任
								S0N->Pos.X = (Uint16)S0N->Pos.X * (LMAXX - 0) / (RMAXX - 0);
								S0N->Pos.Y = 0;		
						}						
						else		S0N->Pos.Y = S0->Pos.Y + DeltY;
				}
				else		//== ƽ����ʾ��
				{
								 if(S0->Pos.X == 0 		 		 && DeltX < 0) S0N->Pos.X = LMAXX - 1;
						else if(S0->Pos.X == (LMAXX-1) && DeltX > 0) S0N->Pos.X = 0;							
						else																		 		 S0N->Pos.X = S0->Pos.X + DeltX;
						
						
						if(S0->Pos.Y == 0 && DeltY < 0) 
						{
								S0->Pos.Sts = 0;
								
								//=== ����������X����任
								S0N->Pos.X = (Uint16)S0N->Pos.X * (RMAXX - 0) / (LMAXX - 0);
								S0N->Pos.Y = RMAXY - 1;
						}
						else if(S0->Pos.Y == (LMAXY-1) && DeltY > 0)
						{
								S0->Pos.Sts = 0;
								
								//=== ����������X����任
								S0N->Pos.X = (Uint16)S0N->Pos.X * (RMAXX - 0) / (LMAXX - 0);
								S0N->Pos.Y = 0;	
						}						
						else	S0N->Pos.Y = S0->Pos.Y + DeltY;
				}
				S0N->Pos.Sts = S0->Pos.Sts;
				S0->Pos.Sts = Sts;
				S0N->Color = S0->Color;
				
				//== (2)����ͷ����
				S0N->Next = S0;
				S0 = S0N;				
				
				//== (3)����ͷ��������ڵ����		
				if(S0->Pos.Sts == 0)
				{
						LED.RBuf[S0->Pos.X][S0->Pos.Y] = S0->Color;
				}
				else
				{
						LED.LBuf[S0->Pos.X] |= (0x0001 << S0->Pos.Y);
				}
				
				//== (4)����֮ǰ�����нڵ㣬ͬʱ��������ͷ����ǰ��һ��(��ɫ�̳�)
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
				
				//== (5)ɾ����β ɾ����ʱ���ɵĽڵ�(�ѷ����ڴ�)
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
		
		//=== end����¼�����˶�����
		Menu.Flag.bit.SnakeRunDirOld = Menu.Flag.bit.SnakeRunDir;
}

/*************************************************************************************************/
/* ���ܣ���λ��Ϸ																																			 */
/* ���룺��     	                     																											 */
/* �������			                                                                                 */
/* ��������λ̰���ߺ�ʳ�ͬʱ������ʾ              																									 */
/*************************************************************************************************/
void SnakeReset(void)
{
		pSnake ST = S0;
		
		Menu.Flag.bit.SnakeGameOver = 0;
		
		//== 1���Ȳ���̰����
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
		
		//== 2���ٲ���ʳ��
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
		
		//== 3���ٸ�λ����
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
