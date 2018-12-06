#include "BaseInc.H"	

extern const char VendorName[5];			//��˾����		5
extern const char ProductCode[9]; 	//Ӳ�����		9  												   
extern const char ProductRevision[7]; 		//Ӳ���汾��	7 
extern const char MajorMinorRevision[7];   	//����汾�� 	7
extern const char CompileVersion[7]; 		//�������汾 	7
extern const char CompileDate[12];  	//�������� 	  12
extern const char CompileTime[9];		//����ʱ��		9	

//������
//2018��12��06�޸�
//�������޸�
int main(void)
{		 
//	unsigned int Flashi,FlashTimeout=0;
//	unsigned char *pFlashData;
	unsigned char ftp_flag=0;
	unsigned char *pReadFlag;
	volatile FLASH_Status FLASHStatus;
	unsigned char FTPFlag=0;
	setlocale(LC_ALL,"C");    //���ñ�������   70 72 475
	IRWorkFlag = 1;
	LocalTime = 0;
	SysMsCnt = 0;
	needGPSRawIden = 0;
	OpenBatteryFlag = 0;
	OpenBatTimeOut = 0; 
	SysInitFlag = 0;
	upload_ratio = 1;
	GprsPlayLen = 0;
	DownloadFileFlag = 0;
	
	GPRS_FTP_FLAG = 0;
	SateOutNum = 0;
	GpsSpeet = 0;
	FTP_Cnt = 0;
	IwdgFlag = 0;
	OpenFtpTimeOut = 0;
	LockFlagTimeOut = 0;
	LockFlag = 0;
	
	TCP_MODE_FLAG = 0;
	
	LoginNum = 0; 
	ReadCNUMFlag = 0;
	ReadIDFlag = 0;
	ReadCSQFlag = 0;
	
	SndUsrGPSPkg = 5;
	SndIdleGPSPkg = 60;
	SndBatPkg = 60;
	SndIdleTimeout = SndIdleGPSPkg;
	RevIdleTimeout = SndIdleGPSPkg;
	RevResetTimeOut = SysMsCnt+RevIdleTimeout*3*1000;
	SndResetTimeOut = SysMsCnt+SndIdleTimeout*3*1000;
	
	//����ģʽ��ʼ��
	TestModeFlag = 0;
	TestStatus = 0;
	TestTimeout = 0;
	TestModeTimeout = 0;
	CheckModeFlag = 0;
	CheckFlashFlag = 0;
	SysWorkFlag = 0;
	OutWavIndex = 0;
	MobileNum = 0;
	ResetSysFlag = 0;
	sysStopState = 0;
	LinkedNum = 0;
	
	TCP_LinkFlag = 0;
	TCP_LinkCnt = 0;
	ExtPowerState = 1;
	GPS_STATE = 1;
	bikeStatus = 0;
	
	FtpLinkCount = 0;
	
	GPSTimeOut = 0;
	BATTimeOut = 2000;
	
	AgingTestTimeout = 0;
	AgingTestNum = 0;
	ExtPLLCLK = 1;
	
	SendMpu6050TimeOut = 0;
	SendMpu6050Num = 0;
	SendMpu6050Cnt = 0;
	SendMpu6050Avg = 0;
	AccMoveFlag = 0;
	ACCMoveNum = 0;
	
	TcpSendDataLen = 0;
	
  RCC_Configuration(); //��ʼ��ʱ��  	
	SysTick_Config(56000L); //��ʼ�����Ķ�ʱ��
	W25X_Init();
	SysParsInit(0);

	GPIO_Configuration();//��ʼ��IO��	
	USART_AllPortInit();
	W25X_Init();
	ADC_AI_Init();
	WavePlayInit();
	IR_Init();
	CmdFifoInit(&gsUsart2CmdCtrl);
	CmdFifoInit(&gsUsart3CmdCtrl);
	CmdFifoInit(&gsUsart4CmdCtrl);
	GPRSInit();
	AddCmdToFifo(&gsUsart2CmdCtrl,CLOSE_GPRMC_1,400,1);
	IIC_GPIO_Config();
	MPU6050_Inital();
	if(CheckModeFlag==0)
	{
		USART3_SendData((unsigned char *)VendorName,0x38,1);
		USART4_SendData((unsigned char *)VendorName,0x38,1);
	}
//  FTP���Կ�ʼ
	FTPFlag = 0;
	if(FTPFlag==1)
	{
		FTPFlag = 0;
		GPRS_FTP_FLAG = 0;//1������FTP��0������
		OpenFtpTimeOut = SysMsCnt+10*60*1000;
		memcpy(Update_IpAddr,"123.56.235.223",15);
		memcpy(Update_FilePath,"/",11);
		memcpy(Update_FileName,"deerbike_v41_dev.bin",21);
		memcpy(Update_User,"deerbike",9);
		memcpy(Update_Password,"123456",7);
		pReadFlag = (unsigned char *)DownFlagAddr;
		if((pReadFlag[0x00]==0xAA)&&(pReadFlag[0x40]==0x55)&&(pReadFlag[0x80]==0xAA)&&(pReadFlag[0xC0]==0x55))//������±�ʶ
		{
			//���Flash�� 
			FLASH_Unlock();
			//������б�־ 
			FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);	
			FLASHStatus = FLASH_ErasePage(DownFlagAddr);
			//����Flash�� 
			FLASH_Lock();
		}
	}
	// FTP���Խ���
  while(1)
	{
		USART1_Handle();
		USART1_CmdFifoHandle();
		USART2_Handle();
		USART2_CmdFifoHandle();
		USART3_Handle();
		USART3_CmdFifoHandle();
		USART4_Handle();
		USART4_CmdFifoHandle(); 
		SendGPSPkgTimeHandle();
		SendBATPkgTimeHandle();
		SendACCPkgTimeHandle();
		SendDonePkgTimeHandle();
		IRHandle();	
		PressHandle();
		Read_MPU6050_Handle();
		AccHandle();
		AI_Handle();
		OpenFlashHandle();
		OpenRingbellHandle();
		CheckTimesHandle();
		LoadWaveFile();
		TimeReloadIWDG();		
		if(ftp_flag==1)
		{
			ftp_flag = 0;
			AddCmdToFifo(&gsUsart1CmdCtrl,AT_SAPBR_CONTYPE_FLAG,1000,1);
		}
	}
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
  */



void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif



/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
