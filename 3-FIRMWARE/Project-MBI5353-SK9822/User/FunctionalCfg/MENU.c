/*************************************************************************************************/
/*文件：菜单C文件			                                                               						 */
/*记录：By Zouqb@2022.1.1																																			 	 */
/*************************************************************************************************/
#include "MENU.h"
#include "MUSIC.h"
#include "SNAKE.h"
#include "ASCII.h"
#include "PAINT.h"
#include "MBI5353.h"
#include "SK9822.h"
#include "SPI.h"	

#define CR	0x40
#define CG	0x00
#define CB	0x00

TYPE_MENU Menu = {0};

void DispUpdateLine(void);
void DispUpdateRote(void);

/*************************************************************************************************/
/* 功能：显示更新 2000Hz																																		 		 */
/* 描述：刷新平面和球形LED数据           																									 			 */
/*************************************************************************************************/
void DispUpdate(void)
{
		DispUpdateLine();
		DispUpdateRote();
}

/*************************************************************************************************/
/* 功能：平面显示更新 2000Hz																																 		 */
/* 描述：刷新平面数据           																									 							 */
/*************************************************************************************************/
void DispUpdateLine(void)
{
		if(++LED.LAxisCntX >= LED.LAxisMaxX)	LED.LAxisCntX = 0;

		SPI2_WriteByte(Reverse16(LED.LBuf[LED.LAxisCntX]));
}

/*************************************************************************************************/
/* 功能：球形显示更新 2000Hz																																 		 */
/* 描述：刷新球形数据           																									 							 */
/*************************************************************************************************/
void DispUpdateRote(void)
{
		Uint8 *p = &LED.RBuf[LED.RAxisCntX][0].R;
		
		if(++LED.RAxisCntX >= LED.RAxisMaxX)	LED.RAxisCntX = 0;
		
#if (DEVICE_SEL == 1)
		MBI5353_Update_DMA(p);
#else
		SK9822_Update_DMA(p);	
#endif

}

/*************************************************************************************************/
/* 功能：初始功能加载																																			 		 */
/* 描述：加载屏				              																									 			 */
/*************************************************************************************************/
void DispInit(void)
{
		static Uint8 Y = 0, Y1 = RMAXY, Sts = 0;
		static Uint8 YOld = 0;
		
		if(++Y >= Y1)
		{
				Y = 0;
				if(Y1 == 0)
				{
						Y1 = RMAXY;
//						X += 5;
//						if(X >= RMAXX)
//						{
//								X = 0;
								if(++Sts >= 3)
								{
										Sts = 0;
								}
//						}
				}
				else
				{
						Y1--;
				}
				for(Uint8 i = 0; i < RMAXX; i++)
				{
						LED.RBuf[i][Y1-1].R = (Sts == 0) ? Menu.Rgb.R : 0;
						LED.RBuf[i][Y1-1].G = (Sts == 1) ? Menu.Rgb.G : 0;
						LED.RBuf[i][Y1-1].B = (Sts == 2) ? Menu.Rgb.B : 0;
				}
		}
		else
		{
				for(Uint8 i = 0; i < RMAXX; i++)
				{
						LED.RBuf[i][YOld].R = 0;
						LED.RBuf[i][YOld].G = 0;
						LED.RBuf[i][YOld].B = 0;	
				}
		}

		for(Uint8 i = 0; i < RMAXX; i++)
		{
				LED.RBuf[i][Y].R = (Sts == 0) ? Menu.Rgb.R : 0;
				LED.RBuf[i][Y].G = (Sts == 1) ? Menu.Rgb.G : 0;
				LED.RBuf[i][Y].B = (Sts == 2) ? Menu.Rgb.B : 0;	
		}
		YOld = Y;

}

/*************************************************************************************************/
/* 功能：球形显示功能2																																			 		 */
/* 描述：显示图片					              																									 			 */
/*************************************************************************************************/
void DispPic(void)
{
		const Uint8 *p = &Pic1[0][0];
		
		if(Menu.CurSel == 0)			p = &Pic1[0][0];
		else if(Menu.CurSel == 1)	p = &Pic2[0][0];
		else											p = &Pic3[0][0];
		
		for(Uint8 i = 0; i < RMAXX; i++)
		{
				for(Uint8 j = 0; j < RMAXY; j++)
				{
//						LED.RBuf[i][j].R = *(p+i*RMAXY*3+j*3+0) >> Menu.PicShiftBits;
//						LED.RBuf[i][j].G = *(p+i*RMAXY*3+j*3+1) >> Menu.PicShiftBits;
//						LED.RBuf[i][j].B = *(p+i*RMAXY*3+j*3+2) >> Menu.PicShiftBits;
						
						LED.RBuf[i][RMAXY-1-j].R = *(p+i*RMAXY*3+j*3+0) >> Menu.PicShiftBits; //== 逆序
						LED.RBuf[i][RMAXY-1-j].G = *(p+i*RMAXY*3+j*3+1) >> Menu.PicShiftBits;
						LED.RBuf[i][RMAXY-1-j].B = *(p+i*RMAXY*3+j*3+2) >> Menu.PicShiftBits;
				}
		}
}


/*************************************************************************************************/
/* 功能：界面0																																							 		 */
/* 描述：上电加载功能             																									 			 */
/*************************************************************************************************/
void Layer0(void)
{
		//=== 1、触发了任意按键均可进入菜单
		if((Menu.KeySts == UP) 	 || (Menu.KeySts == LEFT)  ||	
		   (Menu.KeySts == DOWN) || (Menu.KeySts == RIGHT) ||
			 (Menu.KeySts == OK)	 || (Menu.KeySts == STOP))	
		{
				Menu.CurSel = 0;
				Menu.Layer = 1;
				Menu.KeySts = ELSE;
				DrawClear();
				return;
		}
		
		//=== 2、显示
		if(!Menu.Init) DispInit();
		
}

/*************************************************************************************************/
/* 功能：界面1																																							 		 */
/* 描述：菜单1             																									 			 */
/*************************************************************************************************/
void Layer1(void)
{
		char Temp[16];
		TYPE_COLOR RGB = {CR,CG,CB};
		
		//=== 1、键盘操作
		if(Menu.KeySts == DOWN)	
		{
				Menu.CurSel = (Menu.CurSel == 3) ? 0 : Menu.CurSel+1;	
		}
		else if(Menu.KeySts == UP)	
		{
				Menu.CurSel = (Menu.CurSel == 0) ? 3 : Menu.CurSel-1;	
		}
		else if(Menu.KeySts == OK)	
		{
				Menu.Layer += Menu.CurSel + 1;	//== 最大3
				
				Menu.KeySts = ELSE;
				DrawClear();
				
				if(Menu.CurSel == 3)	//== 贪吃蛇需要初始化
				{
						Menu.Rgb.R = 0xFF;
						Menu.Rgb.G = 0;
						Menu.Rgb.B = 0;
						Menu.KeySts = UP;
						Menu.Flag.bit.SnakeRunDir	  = UP;			//== 初始运动状态：暂停
						Menu.Flag.bit.SnakeRunDirOld = UP;		//== 初始运动状态：暂停
						SnakeReset();
						SnakeInit(&Menu.Rgb);
						
						Menu.SnakeRunDiv = 10;
						Menu.Flag.bit.SnakeGameOver = 0;			//== 一进游戏用户自己确定
				}
				Menu.CurSel = 0;
				return;
		}
		Menu.KeySts = ELSE;
		
		//=== 2、显示	
		if(Menu.CurSel == 3)
		{
				#if (DEVICE_SEL == 1)
				sprintf(Temp, "2.Pictures");
				DrawChars(0, 6+HIGH*0, Menu.CurSel != 1, Temp, &RGB);	
				sprintf(Temp, "3.Music   ");
				DrawChars(0, 6+HIGH*1, Menu.CurSel != 2, Temp, &RGB);
				sprintf(Temp, "4.Game Snake");
				DrawChars(0, 6+HIGH*2, Menu.CurSel != 3, Temp, &RGB);
				#else
				sprintf(Temp, "2.Pictures");
				DrawChars(0, 2+HIGH*0, Menu.CurSel != 1, Temp, &RGB);	
				sprintf(Temp, "3.Music   ");
				DrawChars(0, 2+HIGH*1, Menu.CurSel != 2, Temp, &RGB);
				sprintf(Temp, "4.Game Snake");
				DrawChars(0, 2+HIGH*2, Menu.CurSel != 3, Temp, &RGB);
				#endif
		}
		else
		{
				#if (DEVICE_SEL == 1)
				sprintf(Temp, "1.Painting");
				DrawChars(0, 6+HIGH*0, Menu.CurSel != 0, Temp, &RGB);
				sprintf(Temp, "2.Pictures");
				DrawChars(0, 6+HIGH*1, Menu.CurSel != 1, Temp, &RGB);	
				sprintf(Temp, "3.Music   ");
				DrawChars(0, 6+HIGH*2, Menu.CurSel != 2, Temp, &RGB);
				#else
				sprintf(Temp, "1.Painting");
				DrawChars(0, 2+HIGH*0, Menu.CurSel != 0, Temp, &RGB);
				sprintf(Temp, "2.Pictures");
				DrawChars(0, 2+HIGH*1, Menu.CurSel != 1, Temp, &RGB);	
				sprintf(Temp, "3.Music   ");
				DrawChars(0, 2+HIGH*2, Menu.CurSel != 2, Temp, &RGB);
				#endif
		}

}

/*************************************************************************************************/
/* 功能：界面2																																							 		 */
/* 描述：RGB控制						             																									 			 */
/*************************************************************************************************/
void Layer2(void)
{
		static Uint16 Timer = 0;
		
		//=== 1、键盘操作
		if(Menu.KeySts == DOWN)	
		{
				Menu.RgbY = (Menu.RgbY == RMAXY) ? 0 : Menu.RgbY+1;
		}
		else if(Menu.KeySts == UP)	
		{
				Menu.RgbY = (Menu.RgbY == 0) ? RMAXY : Menu.RgbY-1;
		}
		else if(Menu.KeySts == LEFT)	
		{
				Menu.RgbX = (Menu.RgbX == 0) ? RMAXX : Menu.RgbX-1;				
		}
		else if(Menu.KeySts == RIGHT)	
		{
				Menu.RgbX = (Menu.RgbX == RMAXX) ? 0 : Menu.RgbX+1;
		}
		else if(Menu.KeySts == OK || Menu.KeySts == STOP)	
		{
//				Menu.RgbX = 0;
//				Menu.RgbY = 0;
//				Timer = 0;
				Menu.Layer = 1;	//== 返回界面1

				Menu.KeySts = ELSE;
				DrawClear();
				return;
		}
		Menu.KeySts = ELSE;
		
		//=== 2、显示及选择闪烁
		if(++Timer > 50) Timer = 0;
		DrawPoint(Menu.RgbX, Menu.RgbY, 1, &Menu.Rgb);
		if(Menu.DrawLineSel == 1)
		{
				DrawClear();
		}
		else if(Menu.DrawLineSel == 2)
		{
				DrawPoint(Menu.X1, Menu.Y1, 1, &Menu.Rgb);
		}
		else if(Menu.DrawLineSel == 3)
		{
				DrawLine(Menu.X1, Menu.Y1, Menu.X2, Menu.Y2, 1, &Menu.Rgb);
		}
		Menu.DrawLineSel = 0;
}

/*************************************************************************************************/
/* 功能：界面3																																							 		 */
/* 描述：图片控制						             																									 			 */
/*************************************************************************************************/
void Layer3(void)
{
		//=== 1、键盘操作
		if(Menu.KeySts == UP || Menu.KeySts == LEFT)	
		{
				Menu.CurSel = (Menu.CurSel == 2) ? 0 : Menu.CurSel+1;					
		}
		else if(Menu.KeySts == DOWN || Menu.KeySts == RIGHT)	
		{
				Menu.CurSel = (Menu.CurSel == 0) ? 2 : Menu.CurSel-1;	
		}
		else if(Menu.KeySts == OK || Menu.KeySts == STOP)	
		{
				Menu.CurSel = 0;
				Menu.Layer = 1;
				Menu.KeySts = ELSE;
				DrawClear();
				return;
		}
		Menu.KeySts = ELSE;
		
		//=== 2、显示
		DispPic();
}

/*************************************************************************************************/
/* 功能：界面4																																							 		 */
/* 描述：音乐控制						             																									 			 */
/*************************************************************************************************/
void Layer4(void)
{
		char Temp[16];
		TYPE_COLOR RGB = {CR,CG,CB};

		//=== 1、键盘操作
		if(Menu.KeySts == DOWN)	
		{
				Menu.CurSel = (Menu.CurSel == 1) ? 0 : Menu.CurSel+1;					
		}
		else if(Menu.KeySts == UP)	
		{
				Menu.CurSel = (Menu.CurSel == 0) ? 1 : Menu.CurSel-1;	
		}
		else if(Menu.KeySts == OK)	
		{
				if(Menu.CurSel == 0)
				{
						Music.bit.Mode = (Music.bit.Mode >= 2) ? 0 : Music.bit.Mode+1;
				}
				else
				{
						Music.bit.SongSel = (Music.bit.SongSel == (MAX_SONGS-1)) ? 0 : Music.bit.SongSel+1;
				}
		}
		else if(Menu.KeySts == STOP)	
		{
				Menu.CurSel = 0;
				Menu.Layer = 1;
				Menu.KeySts = ELSE;
				DrawClear();
				return;
		}
		Menu.KeySts = ELSE;
		
		//=== 2、显示(要对齐，否则有残留)
		sprintf(Temp, "1.Mode: ");
		DrawChars(0, HIGH, 1, Temp, &RGB);
		if(Music.bit.Mode == 0)				sprintf(Temp, "OFF");
		else if(Music.bit.Mode == 1)	sprintf(Temp, "ON ");
		else if(Music.bit.Mode == 2)	sprintf(Temp, "CYC");
		else													sprintf(Temp, "ING");
		DrawChars(54, HIGH, Menu.CurSel!=0, Temp, &RGB);
		
		sprintf(Temp, "2.Music: ");
		DrawChars(0, HIGH*2, 1, Temp, &RGB);	
		if(Music.bit.SongSel == 0)			sprintf(Temp, "JYLY    ");
		else if(Music.bit.SongSel == 1)	sprintf(Temp, "FZYDYZDY");
		else														sprintf(Temp, "SRKL    ");
		DrawChars(54, HIGH*2, Menu.CurSel!=1, Temp, &RGB);

		//=== 3、音乐播放(放定时调度)
}

/*************************************************************************************************/
/* 功能：界面5																																							 		 */
/* 描述：贪吃蛇						             																									 			 */
/*************************************************************************************************/
void Layer5(void)
{
		static Uint8 Cnt = 0;
		
		char Temp[16];
		TYPE_COLOR RGB = {CR,CG,CB};
		
		//=== 1、键盘操作
		if(Menu.KeySts == OK)	
		{
				if(Menu.Flag.bit.SnakeGameOver && Menu.CurSel)
				{
						Menu.Flag.bit.SnakeGameOver = 0;
						Menu.CurSel = 0;
						Menu.Flag.bit.SnakeReStart = 1;
				}
				else
				{
						Menu.Layer = 1;
						Menu.KeySts = ELSE;
						DrawClear();
						return;
				}
		}
		switch(Menu.Flag.bit.SnakeRunDirOld)
		{
				case UP:			if(Menu.KeySts != DOWN)	Menu.Flag.bit.SnakeRunDir = Menu.KeySts;	break;
				case DOWN: 		if(Menu.KeySts != UP)		Menu.Flag.bit.SnakeRunDir = Menu.KeySts;	break;
				case LEFT: 		if(Menu.KeySts != RIGHT)Menu.Flag.bit.SnakeRunDir = Menu.KeySts;	break;
				case RIGHT:		if(Menu.KeySts != LEFT)	Menu.Flag.bit.SnakeRunDir = Menu.KeySts;	break;
				default:			break;
		}
//		Menu.KeySts = ELSE;
		
		//=== 2、显示
		if(++Cnt >= Menu.SnakeRunDiv)
		{
				Cnt = 0;
				if(!Menu.Flag.bit.SnakeGameOver && Menu.Flag.bit.SnakeRunDir <= 3) SnakeRun(Menu.Flag.bit.SnakeRunDir);	
		}
		
		if(Menu.Flag.bit.SnakeGameOver)	//== 游戏结束显示界面
		{
				Menu.Flag.bit.SnakeRunDir	 = STOP;				//== 初始运动状态：暂停
				Menu.Flag.bit.SnakeRunDirOld = STOP;			//== 初始运动状态：暂停
		
				if(Menu.KeySts == LEFT)
				{
						Menu.CurSel = (Menu.CurSel == 0) ? 1 : Menu.CurSel+1;
				}
				else if(Menu.KeySts == RIGHT)
				{
						Menu.CurSel = (Menu.CurSel == 1) ? 0 : Menu.CurSel+1;
				}
				Menu.KeySts = ELSE;

				#if (DEVICE_SEL == 1)
				sprintf(Temp, "Game Start?");
				DrawChars(0, 6+HIGH*0, 1, Temp, &RGB);
				sprintf(Temp, "NO");
				DrawChars(0, 6+HIGH*1, Menu.CurSel!=0, Temp, &RGB);
				sprintf(Temp, "YES");
				DrawChars(WIDTH*6, 6+HIGH*1, Menu.CurSel!=1, Temp, &RGB);
				#else
				sprintf(Temp, "Game Start?");
				DrawChars(0, 2+HIGH*0, 1, Temp, &RGB);
				sprintf(Temp, "NO");
				DrawChars(0, 2+HIGH*1, Menu.CurSel!=0, Temp, &RGB);
				sprintf(Temp, "YES");
				DrawChars(WIDTH*6, 2+HIGH*1, Menu.CurSel!=1, Temp, &RGB);
				#endif
		}
		
		if(Menu.Flag.bit.SnakeReStart)
		{
				Menu.Flag.bit.SnakeReStart = 0;
				Menu.Flag.bit.SnakeRunDir	 = STOP;				//== 初始运动状态：暂停
				Menu.Flag.bit.SnakeRunDirOld = STOP;				//== 初始运动状态：暂停
				
				SnakeReset();
				SnakeInit(&Menu.Rgb);
		}
		
}

/*************************************************************************************************/
/* 功能：菜单控制																																						 		 */
/* 描述：根据菜单实现对应的功能          																									 			 */
/*************************************************************************************************/
void (*MenuRun[6])(void) =
{
		Layer0, Layer1, Layer2, Layer3, Layer4, Layer5,
};
