/*************************************************************************************************/
/*�ļ����˵�H�ļ�			                                                               */
/*��¼��By Zouqb@2022.1.1																																			 	 */
/*************************************************************************************************/
#ifndef __MENU_H
#define	__MENU_H

#include "main.h"

typedef struct
{
		Uint32		ScrollMode:2;				//== ��תLED��ʾ����ѡ��0-������ 1-˳ʱ�� 2-��ʱ��
		Uint32		DataTxSts:1;				//== ���ڷ���״̬
				
		Uint32		SnakeGameOver:1;		//== ��Ϸ״̬ 0-���� 1-����
		Uint32		SnakeReStart:1;			//== ��Ϸ���³�ʼ�� 0-����ʼ�� 1-��ʼ��
		Uint32		SnakeRunDir:3;			//== ̰�����˶�����
		Uint32		SnakeRunDirOld:3;		//== ̰�����˶�ֹͣǰ���˶�����
		
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
		Uint8			Layer;						//== ����ҳ
		Uint8 		CurSel;						//== ��ǰѡ����
					
		Uint8			KeySts;						//== ����״̬
		
		Uint8			RgbX,RgbY;				//== �ɿ�RGB����X,Y
	TYPE_COLOR 	Rgb;
	
		Uint8			X1,Y1,X2,Y2;			//== ����
		Uint8			DrawLineSel;
	
		Uint8			PicShiftBits;			//== ͼƬ��ʾ��λ��

		Uint8			ATSel;						//== AT����ѡ��(����)
		
		Uint8			SnakeRunDiv;			//== ��Ϸ�Ѷ�
		
		Uint8			Init;
		
	MENU_WORD		Flag;
		
}TYPE_MENU;

extern TYPE_MENU Menu;

extern void DispInit(void);
extern void DispUpdate(void);
extern void (*MenuRun[6])(void);

#endif


