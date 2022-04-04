#ifndef __USART_H
#define	__USART_H

#include "stm32f10x.h"
#include "main.h"

#define		USART_BUFFSIZE	128			//== 128个字节缓存区

//=== DX-BT05 4.0蓝牙串口模块AT读取指令集
#define		AT_TEST				{"AT\r\n"}								//== 1、测试指令
#define		AT_VERSION		{"AT+VERSION\r\n"})        //== 2、获取软件版本号
#define		AT_LADDR			{"AT+LADDR\r\n"}       //== 3、获取模块蓝牙地址
#define		AT_NAME				{"AT+NAME\r\n"}        //== 4、 查询/设置-设备名称
#define		AT_PIN				{"AT+PIN\r\n"}         //== 5、 查询/设置-配对码
#define		AT_TYPE				{"AT+TYPE\r\n"}        //== 6、 查询/设置-密码配对
#define		AT_BAUD				{"AT+BAUD\r\n"}        //== 7、 查询/设置-串口波特率
#define		AT_STOP				{"AT+STOP\r\n"}        //== 8、 查询/设置-串口停止位
#define		AT_PARI				{"AT+PARI\r\n"}        //== 9、 查询/设置-串口校验位
#define		AT_ADVI				{"AT+ADVI\r\n"}        //== 10、查询/设置-广播时间间隔
#define		AT_NOTI				{"AT+NOTI\r\n"}        //== 11、查询/设置-是否通知上位机链接状态
#define		AT_NOTP				{"AT+NOTP\r\n"}        //== 12、查询/设置-通知上位机链接状态格式
#define		AT_IMME				{"AT+IMME\r\n"}        //== 13、查询/设置-模块是否上电工作
#define		AT_START			{"AT+START\r\n"}       //== 14、查询/设置-模块开始工作指令
#define		AT_UUID				{"AT+UUID\r\n"}        //== 15、查询/设置-UUID
#define		AT_CHAR				{"AT+CHAR\r\n"}        //== 16、查询/设置-Characteristic
#define		AT_IBEA				{"AT+IBEA\r\n"}        //== 17、查询/设置-iBeacon开关
#define		AT_IBE0				{"AT+IBE0\r\n"}        //== 18、查询/设置-iBeacon UUID的值(0)
#define		AT_IBE1				{"AT+IBE1\r\n"}        //== 19、查询/设置-iBeacon UUID的值(1)
#define		AT_IBE2				{"AT+IBE2\r\n"}        //== 20、查询/设置-iBeacon UUID的值(2)
#define		AT_IBE3				{"AT+IBE3\r\n"}        //== 21、查询/设置-iBeacon UUID的值(3)
#define		AT_MARJ				{"AT+MARJ\r\n"}        //== 22、查询/设置-iBeacon Marjor值
#define		AT_MINO				{"AT+MINO\r\n"}        //== 23、查询/设置-iBeacon Minor值
#define		AT_MEA				{"AT+MEA\r\n"}         //== 24、查询/设置-iBeacon MMeasured Power值
#define		AT_RESET			{"AT+RESET\r\n"}       //== 25、查询/设置-软件重启
#define		AT_DEFAULT		{"AT+DEFAULT\r\n"}     //== 26、查询/设置-软件重置
#define		AT_MPWRM			{"AT+PWRM\r\n"}        //== 27、查询/设置-上电是否进入低功耗
#define		AT_SLEEP			{"AT+SLEEP\r\n"}       //== 28、查询/设置-进入低功耗(可被搜索)
#define		AT_ROLE				{"AT+ROLE\r\n"}        //== 29、查询/设置-主/从模式
#define		AT_INQ				{"AT+INQ\r\n"}         //== 30、查询/设置-搜索蓝牙设备(主模式指令)
#define		AT_CONN				{"AT+CONN\r\n"}        //== 31、查询/设置-连接远端设备(主模式指令)
#define		AT_CONA				{"AT+CONA\r\n"}        //== 32、查询/设置-连接远端指定地址从设备(主模式指令)
#define		AT_POWE				{"AT+POWE\r\n"}        //== 33、查询/设置-模块功率
#define		AT_HELP				{"AT+HELP\r\n"}        //== 34、查询/设置-帮助指令


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
