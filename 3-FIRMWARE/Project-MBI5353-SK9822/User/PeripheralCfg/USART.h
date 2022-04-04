#ifndef __USART_H
#define	__USART_H

#include "stm32f10x.h"
#include "main.h"

#define		USART_BUFFSIZE	128			//== 128���ֽڻ�����

//=== DX-BT05 4.0��������ģ��AT��ȡָ�
#define		AT_TEST				{"AT\r\n"}								//== 1������ָ��
#define		AT_VERSION		{"AT+VERSION\r\n"})        //== 2����ȡ����汾��
#define		AT_LADDR			{"AT+LADDR\r\n"}       //== 3����ȡģ��������ַ
#define		AT_NAME				{"AT+NAME\r\n"}        //== 4�� ��ѯ/����-�豸����
#define		AT_PIN				{"AT+PIN\r\n"}         //== 5�� ��ѯ/����-�����
#define		AT_TYPE				{"AT+TYPE\r\n"}        //== 6�� ��ѯ/����-�������
#define		AT_BAUD				{"AT+BAUD\r\n"}        //== 7�� ��ѯ/����-���ڲ�����
#define		AT_STOP				{"AT+STOP\r\n"}        //== 8�� ��ѯ/����-����ֹͣλ
#define		AT_PARI				{"AT+PARI\r\n"}        //== 9�� ��ѯ/����-����У��λ
#define		AT_ADVI				{"AT+ADVI\r\n"}        //== 10����ѯ/����-�㲥ʱ����
#define		AT_NOTI				{"AT+NOTI\r\n"}        //== 11����ѯ/����-�Ƿ�֪ͨ��λ������״̬
#define		AT_NOTP				{"AT+NOTP\r\n"}        //== 12����ѯ/����-֪ͨ��λ������״̬��ʽ
#define		AT_IMME				{"AT+IMME\r\n"}        //== 13����ѯ/����-ģ���Ƿ��ϵ繤��
#define		AT_START			{"AT+START\r\n"}       //== 14����ѯ/����-ģ�鿪ʼ����ָ��
#define		AT_UUID				{"AT+UUID\r\n"}        //== 15����ѯ/����-UUID
#define		AT_CHAR				{"AT+CHAR\r\n"}        //== 16����ѯ/����-Characteristic
#define		AT_IBEA				{"AT+IBEA\r\n"}        //== 17����ѯ/����-iBeacon����
#define		AT_IBE0				{"AT+IBE0\r\n"}        //== 18����ѯ/����-iBeacon UUID��ֵ(0)
#define		AT_IBE1				{"AT+IBE1\r\n"}        //== 19����ѯ/����-iBeacon UUID��ֵ(1)
#define		AT_IBE2				{"AT+IBE2\r\n"}        //== 20����ѯ/����-iBeacon UUID��ֵ(2)
#define		AT_IBE3				{"AT+IBE3\r\n"}        //== 21����ѯ/����-iBeacon UUID��ֵ(3)
#define		AT_MARJ				{"AT+MARJ\r\n"}        //== 22����ѯ/����-iBeacon Marjorֵ
#define		AT_MINO				{"AT+MINO\r\n"}        //== 23����ѯ/����-iBeacon Minorֵ
#define		AT_MEA				{"AT+MEA\r\n"}         //== 24����ѯ/����-iBeacon MMeasured Powerֵ
#define		AT_RESET			{"AT+RESET\r\n"}       //== 25����ѯ/����-�������
#define		AT_DEFAULT		{"AT+DEFAULT\r\n"}     //== 26����ѯ/����-�������
#define		AT_MPWRM			{"AT+PWRM\r\n"}        //== 27����ѯ/����-�ϵ��Ƿ����͹���
#define		AT_SLEEP			{"AT+SLEEP\r\n"}       //== 28����ѯ/����-����͹���(�ɱ�����)
#define		AT_ROLE				{"AT+ROLE\r\n"}        //== 29����ѯ/����-��/��ģʽ
#define		AT_INQ				{"AT+INQ\r\n"}         //== 30����ѯ/����-���������豸(��ģʽָ��)
#define		AT_CONN				{"AT+CONN\r\n"}        //== 31����ѯ/����-����Զ���豸(��ģʽָ��)
#define		AT_CONA				{"AT+CONA\r\n"}        //== 32����ѯ/����-����Զ��ָ����ַ���豸(��ģʽָ��)
#define		AT_POWE				{"AT+POWE\r\n"}        //== 33����ѯ/����-ģ�鹦��
#define		AT_HELP				{"AT+HELP\r\n"}        //== 34����ѯ/����-����ָ��


typedef struct{
	
		Uint16		status:	3;
		Uint16		start:	1;
		Uint16		BaudSet: 4;
		Uint16		cmd:		4;
		Uint16		rsvd:	  4;

}BT05_BITS;

typedef union{

		Uint16			all;
		BT05_BITS		bit;

}BT05_WORD;

typedef struct{
	
		BT05_WORD		flag;
		Uint8				TX_Buf[USART_BUFFSIZE];
		Uint8 			RX_Buf[USART_BUFFSIZE];
		char				AT_Buf[USART_BUFFSIZE];
	
}TYPE_BLUETOOTH;

extern Uint32 Baud[];
extern TYPE_BLUETOOTH BT05;

void USART1_DMA_Init(Uint32 bound);
void USART2_DMA_Init(Uint32 bound);
void USART2_Config(Uint32 Baud);
void USART2_TX_START(Uint8 *Buf,Uint16 Len);

#endif
