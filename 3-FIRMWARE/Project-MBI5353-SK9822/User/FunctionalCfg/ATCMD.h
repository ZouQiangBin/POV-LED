/*************************************************************************************************/
/*�ļ���AT����H�ļ�			                                                               */
/*��¼��By Zouqb@2022.1.1																																			 	 */
/*************************************************************************************************/
#ifndef __ATCMD_H
#define	__ATCMD_H

#include "main.h"
#include "USART.h"

extern void AT_CMD(Uint8 *Sel);
extern void USART2_BuadSelfCheck(void);
extern void BT05_Config(TYPE_BLUETOOTH *p);

#endif
