/*************************************************************************************************/
/*文件：菜单H文件			                                                               */
/*记录：By Zouqb@2022.1.1																																			 	 */
/*************************************************************************************************/
#ifndef __MENU_H
#define	__MENU_H

#include "main.h"

typedef struct
{
		Uint32		ScrollMode:2;				//== 旋转LED显示滚动选择：0-不滚动 1-顺时针 2-逆时针
		Uint32		DataTxSts:1;				//== 串口发送状态
				
		Uint32		SnakeGameOver:1;		//== 游戏状态 0-正常 1-结束
		Uint32		SnakeReStart:1;			//== 游戏重新初始化 0-不初始化 1-初始化
		Uint32		SnakeRunDir:3;			//== 贪吃蛇运动方向
		Uint32		SnakeRunDirOld:3;		//== 贪吃蛇运动停止前的运动方向
		
		Uint32		BleErr:1;
		Uint32		rsvd:20;
}MENU_BITS;
typedef union
{
		Uint32		all;
		MENU_BITS	bit;
}MENU_WORD;
typedef struct
{
		Uint8			Layer;						//== 界面页
		Uint8 		CurSel;						//== 当前选择项
					
		Uint8			KeySts;						//== 按键状态
		
		Uint8			RgbX,RgbY;				//== 可控RGB坐标X,Y
	TYPE_COLOR 	Rgb;
	
		Uint8			X1,Y1,X2,Y2;			//== 画线
		Uint8			DrawLineSel;
	
		Uint8			PicShiftBits;			//== 图片显示移位数

		Uint8			ATSel;						//== AT功能选择(测试)
		
		Uint8			SnakeRunDiv;			//== 游戏难度
		
		Uint8			Init;
		
	MENU_WORD		Flag;
		
}TYPE_MENU;

extern TYPE_MENU Menu;

extern void DispInit(void);
extern void DispUpdate(void);
extern void (*MenuRun[6])(void);

#endif


