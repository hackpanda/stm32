

#include "includes.h"


#define FLASH_SECTOR_LEN    1024
#define DEVICE_NOMBER_ADDR   0X0800F800  
#define LOCK_REV_LEN         6

#define CMD_MAX_NUM          5
#define LOCK_OFF_LINE_SEND_MAX_NUM 3
#define DEFAULT_CMD                1
#define CHECK_MAX_TIME             20000
TLockCommData LockCommDataBuf[LOCK_NUM];  //全局结构体


unsigned char gCurrentLoopSendIndex=0;
unsigned int  gCheckMaxTime=2;
//TProtocolPort gProtocolPort;
TCommBuf gLockSendData;
const unsigned char Modbus_auchCRCHi[] = 
{ 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 
0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 
0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 
0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 
0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 
0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 
0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40 
} ; 
/*
*********************************************************************************************************

*********************************************************************************************************
*/
const unsigned char  Modbus_auchCRCLo[] = 
{ 
0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 
0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD, 
0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09, 
0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 
0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4, 
0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3, 
0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 
0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4, 
0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 
0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 
0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED, 
0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26, 
0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 
0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 
0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 
0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 
0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 
0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5, 
0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 
0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 
0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C, 
0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 
0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B, 
0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C, 
0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 
0x43, 0x83, 0x41, 0x81, 0x80, 0x40 
};


//typedef void (*TmdSTransactionFun)(TProtocolPort * pProtocolPort); 
unsigned short Modbus_CRC16(unsigned char *puchMsg, unsigned int usDataLen) 
{ 
unsigned char uchCRCHi = 0xFF;
unsigned char uchCRCLo = 0xFF;
unsigned int  uIndex;
 while (usDataLen--)
 { 
  uIndex = uchCRCHi ^ *puchMsg++;
  uchCRCHi = uchCRCLo ^ Modbus_auchCRCHi[uIndex]; 
  uchCRCLo = Modbus_auchCRCLo[uIndex]; 
 } 
 return (uchCRCHi << 8 | uchCRCLo); 
}
/*
*********************************************************************************************************

*********************************************************************************************************
*/
int Modbus_CRCVerify(unsigned char *pBuf,unsigned int Len)
{
  unsigned short TempCRC;
	if(Len<3){return -1;}

	TempCRC=Modbus_CRC16(pBuf,Len-2);
 	if(pBuf[Len-2]!=(unsigned char)(TempCRC>>8))
	{
	 	return -2;
	}

 	if(pBuf[Len-1]!=(unsigned char)(TempCRC&0xff))
	{
	 	return -3;
	}

	return 0;

}

#define NULL 0     



//TLockCommData GetLockCommDataHandle(void)
//{
//  return LockCommDataBuf;
//}


/******************************************************************************************/

/******************************************************************************************/



void Lock_Comm_Data_Buf_Init(void)
{
	unsigned char index;
	for(index=0;index<LOCK_NUM;index++)
	{
		LockCommDataBuf[index].lockNum[0]='0';
		LockCommDataBuf[index].lockNum[1]=index+0x31;
		LockCommDataBuf[index].cmd=DEFAULT_CMD;//
	}
}


static void LOCK_Cmd_Create(TLockCommData *pLock,const UartxPortStru * pRev)
{
	unsigned short crc;
  TLockSendData *pSend=(TLockSendData *)gLockSendData.Buf;
	pSend->lockNum[0]=pLock->lockNum[0];
	pSend->lockNum[1]=pLock->lockNum[1];
	pSend->cmd=pLock->cmd;
	pSend->lockState=pLock->lockState;
	pSend->carState=pLock->carState;
	pSend->batteryState=pLock->batteryState;
	crc=Modbus_CRC16((unsigned char *)pSend,6);
	pSend->crc[0]=crc>>8;
	pSend->crc[1]=crc&0xff;
	gLockSendData.Len=8;
	
	pRev->pSendDataHandle(gLockSendData.Buf,gLockSendData.Len,pRev->port);
}

static void Lock_Comm_Rev_Deal(TLockCommData *pLockComm,const UartxPortStru * pUartxRevData)
{
	TLockRevData *pRev;
	unsigned int time;
	time=OSTimeGet();
  
	
	if(pUartxRevData->revLen==UARTX_FRAM_NULL)return;
	if(pUartxRevData->revLen!=10)return;//
	

	pRev=(TLockRevData *)pUartxRevData->revBuf;
	if(pRev->lockNum[0]!=pLockComm->lockNum[0]||pRev->lockNum[1]!=pLockComm->lockNum[1])
	{
		return;
	}
	pLockComm->lockState=pRev->lockState;
	pLockComm->carState=pRev->carState;
	pLockComm->batteryState=pRev->batteryState;
	
	//已发送给服务器的状态与现有的状态一致就不作为紧急发送了
	if(pLockComm->sendNewState==0)
	{
		if(pLockComm->lockState!=pLockComm->newLockState||pLockComm->carState!=pLockComm->newCarState||pLockComm->batteryState!=pLockComm->newBatteryState)
		{
			pLockComm->newLockState=pLockComm->lockState;
			pLockComm->newCarState=pLockComm->carState;
			pLockComm->newBatteryState=pLockComm->batteryState;
			pLockComm->sendNewState=1;
		}
	}
	//没有接收到新的命令就不再作为紧急发送了
	if(pLockComm->cmd!=DEFAULT_CMD&&pLockComm->cmd==pLockComm->newCmd)
	{
		 pLockComm->revNewCmd=0;
		 pLockComm->cmd=DEFAULT_CMD;
	}
	return;	
}

void Send_Cmd_To_Lock(const UartxPortStru * pRev)//直接由任务调用
{
	unsigned char index;
	//unsigned int osTime;
	for(index=0;index<LOCK_NUM;index++)
	{
	   if(LockCommDataBuf[index].revNewCmd==1)//&&LockCommDataBuf[index].offLine!=1)//收到新的命令马上处理
		 {
			 LockCommDataBuf[index].cmd=LockCommDataBuf[index].newCmd;
			 LOCK_Cmd_Create(&LockCommDataBuf[index],pRev);	
			 Lock_Comm_Rev_Deal(&LockCommDataBuf[index],pRev);
			 break;
		 }
	}
	if(index==LOCK_NUM)
	{
	   LOCK_Cmd_Create(&LockCommDataBuf[gCurrentLoopSendIndex],pRev);
		 Lock_Comm_Rev_Deal(&LockCommDataBuf[gCurrentLoopSendIndex],pRev);
		 gCurrentLoopSendIndex++;
		 gCurrentLoopSendIndex%=LOCK_NUM;
	}
}


