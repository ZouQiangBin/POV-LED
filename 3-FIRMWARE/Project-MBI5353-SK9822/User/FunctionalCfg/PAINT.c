/*************************************************************************************************/
/*�ļ�����ͼC�ļ�			                                                               						 */
/*��¼��By Zouqb@2022.1.1																																			 	 */
/*************************************************************************************************/

#include "PAINT.h"
#include "ASCII.h"

/*************************************************************************************************/
/* ���ܣ���ʾ�������                                    																					 */
/* ���������������ʾ                                    										 */
/*************************************************************************************************/
void DrawClear(void)
{
		for(Uint8 i = 0; i < RMAXX; i++)
		{
				for(Uint8 j = 0; j < RMAXY; j++)	//== ����
				{
						LED.RBuf[i][j].R = 0;
						LED.RBuf[i][j].G = 0;
						LED.RBuf[i][j].B = 0;	
				}
				
				LED.LBuf[i] = 0;									//== ƽ��
		}
}

/*************************************************************************************************/
/* ���ܣ�����                                          																					 */
/* ��������ָ��λ�û�ָ����ɫ�ĵ�(����˸)                                    										 */
/*************************************************************************************************/
void DrawPoint(Uint8 x, Uint8 y, Uint8 en, TYPE_COLOR *p)
{
		if(x >= RMAXX || y >= RMAXY) return;
		
		if(en)
		{
				LED.RBuf[x][y].R = p->R;
				LED.RBuf[x][y].G = p->G;
				LED.RBuf[x][y].B = p->B;
		}
		else
		{
				LED.RBuf[x][y].R = 0;
				LED.RBuf[x][y].G = 0;
				LED.RBuf[x][y].B = 0;		
		}
}

/*************************************************************************************************/
/* ���ܣ����ָ������                                         																				 */
/* ��������ָ��λ�û�ָ����ɫ�ĳ�����                                           									 */
/*************************************************************************************************/
void DrawArea(Uint8 x1, Uint8 y1, Uint8 x2, Uint8 y2, Uint8 en, TYPE_COLOR *p)
{
		for(Uint8 i = x1; i < x2; i++)
		{
				for(Uint8 j = y1; j < y2; j++)
				{
						DrawPoint(i, j, en, p);
				}
		}
}

/*************************************************************************************************/
/* ���ܣ�����                                          																					 */
/* ��������ָ��λ�û�ָ����ɫ����                                           										 */
/*************************************************************************************************/
void DrawLine(Uint8 x1, Uint8 y1, Uint8 x2, Uint8 y2, Uint8 en, TYPE_COLOR *p)
{
		int8 dx, dy, err;
		
    dx = x2-x1;
    dy = y2-y1;
		
    if(dx >= 0)
    {
        if(dy >= 0)
        {
            if(dx >= dy)
            {
                err = dy - (dx >> 1);
                while(x1 <= x2)
                {
                    DrawPoint(x1, y1, en, p);
										
                    if(err > 0) {y1 += 1; err -= dx;}
                    x1 += 1;
                    err += dy;
                }
            }
            else
            {
                err = dx - (dy >> 1);
                while(y1 <= y2)
                {
                    DrawPoint(x1, y1, en, p);
										
                    if(err > 0){x1 += 1; err -= dy;}
                    y1 += 1;
                    err += dx;
                }
            }
        }
        else 
        {
            dy = -dy;
            if(dx >= dy)
            {
                err = dy - (dx >> 1);
                while(x1 <= x2)
                {
                    DrawPoint(x1, y1, en, p);
										
                    if(err > 0) {y1 -=1; err -= dx;}
                    x1 += 1;
                    err += dy;
                }
            }
            else        // 7/8 octant
            {
                err= dx - (dy >> 1);
                while(y1 >= y2)
                {
                    DrawPoint(x1, y1, en, p);
										
                    if(err > 0){x1 += 1; err -= dy;}
                    y1 -= 1;
                    err += dx;
                }
            }
        }
    }
    else //dx<0
    {
        dx = -dx;        //dx=abs(dx)
        if(dy >= 0)    // dy>=0
        {
            if(dx >= dy) // 4/8 octant
            {
                err = dy - (dx >> 1);
                while(x1 >= x2)
                {
                    DrawPoint(x1, y1, en, p);
                    if(err > 0){y1 += 1; err -= dx;}
                    x1 -= 1;
                    err += dy;
                }
            }
            else        // 3/8 octant
            {
                err = dx - (dy >> 1);
                while(y1 <= y2)
                {
                    DrawPoint(x1, y1, en, p);
                    if(err > 0){x1 -= 1;err -= dy;}
                    y1 += 1;
                    err += dx;
                }
            }
        }
        else           // dy<0
        {
            dy = -dy;    // dy=abs(dy)
            if(dx >= dy) // 5/8 octant
            {
                err = dy - (dx >> 1);
                while(x1 >= x2)
                {
                    DrawPoint(x1, y1, en, p);
                    if(err > 0){y1 -= 1;err -= dx;}
                    x1 -= 1;
                    err += dy;
                }
            }
            else        // 6/8 octant
            {
                err = dx - (dy >> 1);
                while(y1 >= y2)
                {
                    DrawPoint(x1, y1, en, p);
                    if(err > 0){x1 -= 1; err -= dy;}
                    y1 -= 1;
                    err += dx;
                }
            }
        }
    }
}

/*************************************************************************************************/
/* ���ܣ����ַ�                                          																				 */
/* ��������ָ��λ�û�ָ����ɫ���ַ�                                           									 */
/*************************************************************************************************/
void DrawOneChar(Uint8 x, Uint8 y, Uint8 en, char Code, TYPE_COLOR *p)
{
		Uint16 Temp;

		if((x >= (RMAXX - 0)) || 
			 (y >= (RMAXY - 0))	||
			 (Code < ' ') 			||
			 (Code > '~'))
			 return;

		for(Uint8 i = 0; i < WIDTH; i++)
		{
				if((x + i) >= RMAXX) return;
				
				Temp = (ASCII_Code6x12[Code - ' '][i<<1] << 8) | (ASCII_Code6x12[Code - ' '][(i<<1) +1]);
		
				for(Uint8 j = 0; j < HIGH; j++)
				{
						if((y + j) >= RMAXY) return;
						
						if(Temp & 0x8000)
						{
								DrawPoint(x+i, y+j, en, p);
						}
						else
						{
								DrawPoint(x+i, y+j, !en, p);
						}
						Temp <<= 1;
				}
		}
}

/*************************************************************************************************/
/* ���ܣ����ַ���                                         																				 */
/* ��������ָ��λ�û�ָ����ɫ���ַ���                                           									 */
/*************************************************************************************************/
void DrawChars(Uint8 x, Uint8 y, Uint8 en, char *Code, TYPE_COLOR *p)
{
		while(*Code != '\0')																											//===�Ƿ���ʾ���
		{
				DrawOneChar(x, y, en, *Code, p);
				x += WIDTH;		//== ÿ���ַ���8��
				Code++;
		}
}

