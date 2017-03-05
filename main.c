//*****************************
//  author��  hg
//  date:     2016/1/19
//  mcu:      s3f9488
//  project:  ���¯  8MHZ
//*****************************
#define _MAIN_C_


#include "main.h"
#include "adc.h"
#include "PWM.h"

#ifdef Screen_74HC164
#include "74HC164.h"
#elif defined  Screen_TM1629
#include "TM1629.h"
#endif
//003EH -> 1 01011 11   003FH ->1111 1 110   D3
__root const volatile uint8 Smart_Code[]@0x003C={0xFF, 0x05, 0xD3, 0xFE};
__code const uint8 POWER_RATE[] =
{
#ifdef P_15KW
	0, 2 , 4 , 6, 8 , 10 , 12 , 13 , 15
#elif defined P_20KW
	0, 3 , 5 , 7 , 9 , 11, 14, 17, 20
#elif defined P_25KW
	0, 4 , 6 , 8 , 12, 16,18, 22,25
#elif defined P_30KW
	0, 5 , 8 , 9 , 14, 18, 21, 25, 30
#elif defined P_35KW
        0, 5 , 6 , 10 , 18, 22, 26, 30, 35
#endif
};
uint4 firstOpen = TRUE;
uint4 haveViewSet = FALSE;
uint8 count_60ms = 0;
uint8 count_1s = 0;
uint8 count_2s = 0;

uint8 buzzTime = 0;		//����������ʱ��

uint8 fanTime = FAN_ALL_TIME;		//������תʱ�� 0��ʾ����ʱ״̬ >0��ʾ��ʱ״̬

uint4 rangeNow = 0;			//��ǰ��λ

uint16 statusViewNum = 0;		//ÿλ��⵽״̬��ʾ����,1��ʾ����0��ʾ����  0�޹�/1���̳���/2���̿�·/3IGBT1����/4IGBT1��·/5IGBT2����/6IGBT2��·
								//7��ѹ��/8��ѹ��/9ȱ��/10��λ���ؿ�·/11���׿�·/12���׳���/13IGBT��������/14�����������/15���̲�ͨ
uint4 nullPot = 0;//����޹�����
uint4 vLow = 0;   //��ѹ�ʹ���
uint4 vHight = 0; //��ѹ�ߴ���
uint4 vCut = 0;   //��ѹȱ�����
uint4 cTransformerCut = 0;//����״̬

uint4 nulligbtToLay=0;//igbterror�˳��ӳٴ���

uint4 checkTimeOn = FALSE;//����ʱ���
uint8 nullPotCheckTime = 60;//�����ʱ
uint8 igbtErrorCheckTime = 60;//igbt�����ָ���ʱ
uint8 temperatureCheckTime = 40;//�¶ȼ����ʱ

uint4 checkTransformerCut =0;//����ʱ��������
uint4 checkNullPot =0;//�޹�ʱ�����޹�


uint4 turnOnLay=0;
uint4 nullPotLay=0;//�޹���ʾ�ӳ�
uint4 nulligbtLay=0;//igbtError��ʾ�ӳ�

uint4 whiletimes =0;

#ifdef Screen_TM1629
int16 tempurature =0;
#endif

#pragma inline=forced
								   //--initiation
void sysInit()
{

	WDTCON = 0xAA;
	BTCON = 0x03;//128 ����
	CLKCON = 0x18;//00/16 08/8 10/2 18/1   
	SYM = 0x00;

	OSCCON = 0x04;

	ADCON = 0xF0;
	TADATA = 234;
	TACON = 0x0B;
	//TACON = 0x00;

	TBCON = 0x77;//0x01   11 0111  /2
	TINTPND = 0x00;

	//WTCON  0100 1010
	//WTCON=0x4A;

	VLDCON = 0x17;//3.9V
}

#pragma inline=forced
void ioInit()
{
	/* PO I/O�� ����IO�ھ�����Ϊ�������� Ĭ��*/
        P0CONH = 0x55;//01(ADC4) 01(ADC5) 01(ADC6) 01(ADC7)
	
	P0CONL = 0xC3;//11(ADC8) 00(P0.2/reset) 00(P0.1/XTOUT) 11(P0.0/FAN)
	P0PUR = 0x07;//0 0 0 0 0   0 1 1 1
				 /* P1 I/O�� */
	P1CONH = 0x0A;//00(P1.7) 00(P1.6) 10(P1.5/������) 10(P1.4/A316J�͵�ƽ��λ�ź�)
	P1CONL = 0xFC;//11(P1.3/ADC0) 11(P1.2/ADC1) 11(P1.1/ADC2) 00(�ر�)/01(P1.0/PWM)
	P1PUR = 0xF1;//1 1 1 1  0 0 0 1
				 /*  P2 I/0��*/
	P2CONH = 0x2A;//00(P2.7) 10(P2.6/LED) 10(P2.5/164 DAT) 10(P2.4/164 CLK)
#ifdef Screen_74HC164
	P2CONL = 0xAA;//10(P2.3/COM1) 10(P2.2/COM2) 10�� 00��(P2.1/IIC-SDA) 10(P2.0/IIC-SCL)
#elif defined Screen_TM1629
	P2CONL = 0x8A;//10(P2.3/COM1) 00(P2.2/COM2) 10(P2.1/IIC-SDA) 10(P2.0/IIC-SCL)
#endif

				  /*  P3 I/0��*/
	P3CONH = 0x05;//00(P3.6/SCLK) 00(P3.5/SDAT) 01(P3.4/�½�) 01(P3.3/�½���)
	P3CONL = 0x00;//000(P3.2) 000(P3.1) 00(P3.0)

	P3INT = 0x0A;//00(P3.6/SCLK) 00(P3.5/SDAT) 00(P3.4/�½�)10 00(P3.3/�½���)10
	P3PND = 0x00;//0000 1(P3.6/SCLK) 0(P3.5/SDAT) 0(P3.4/�½���) 0(P3.3/�½���) -�жϹ��� ��0����
				 /*  P4 I/0�� ȫ������*/
}
#pragma inline=forced
void defaultValue()
{
	firstOpen = TRUE;
	haveViewSet = FALSE;
	count_60ms = 0;
	count_1s = 0;
	count_2s = 0;
        
	buzzTime = 0;		//����������ʱ��

	fanTime = FAN_ALL_TIME;		//������תʱ�� 0��ʾ����ʱ״̬ >0��ʾ��ʱ״̬

	rangeNow = 0;			//��ǰ��λ

	statusViewNum = 0;		//ÿλ��⵽״̬��ʾ����,1��ʾ����0��ʾ����  0�޹�/1���̳���/2���̿�·/3IGBT1����/4IGBT1��·/5IGBT2����/6IGBT2��·
							//7��ѹ��/8��ѹ��/9ȱ��/10��λ���ؿ�·/11���׿�·/12���׳���/13IGBT��������/14�����������/15���̲�ͨ
	nullPot = 0;//����޹�����
	vLow = 0;   //��ѹ�ʹ���
	vHight = 0; //��ѹ�ߴ���
	vCut = 0;   //��ѹȱ�����
	cTransformerCut = 0;//����״̬

        nulligbtToLay=0;//igbterror�˳��ӳٴ���

	nullPotCheckTime = 60;//�����ʱ
	temperatureCheckTime = 40;//�¶ȼ����ʱ40s
        
        turnOnLay =0;
        nullPotLay =0;
        nulligbtLay =0;
        
}
int main()
{
	di;
	//ϵͳ��ʼ�� IO��ʼ��
	sysInit();
	ioInit();

#ifdef Screen_74HC164
	init_74HC164();
#elif defined Screen_TM1629
	init_TM1629();
#endif

	defaultValue();
        initPWM();
	ei;
        BUZZ_ON;
        CLEAR_WD;
        while(turnOnLay<TURN_ALL_TIME);
        
        rangeNow = getSwitchByAnum();   
#ifdef Screen_TM1629
        tempurature = getTemperatureByAnum(6);
#endif


	while (1)
	{
/*#ifdef DEBUG
		CLEAR_WD;
		//SwitchSet();
		//ViewSet(rangeNow);
                //P1CONL = 0xFC;
                //Set_Bit(P1,0);
                //AJ_ON;
                SwitchSet();
                ViewSet(rangeNow);
#else*/
                CLEAR_WD;
                whiletimes++;
                if(whiletimes>6)
                {
                  whiletimes =0;
                }
                
                
		haveViewSet = FALSE;
		checkTimeOn = FALSE;
		SwitchSet();//���õ�λ
		
                if(whiletimes==6)
                {
		  DetectCoilHot();//���̳���
		  DetectCoilCut();//����̽ͷ��·
		  DetectIGBTHot_1();//IGBT����
		  DetectIGBTCut_1();//IGBT̽ͷ��·
		  DetectIGBTHot_2();//IGBT����
                  CLEAR_WD;
		  DetectIGBTCut_2();//IGBT̽ͷ��·
		  DetectVLow();//��ѹ���
		  DetectVHight();//��ѹ���
		  DetectVCut();//ȱ����
		  DetectSwitchCut();//��λ���ؿ�·
		  DetectUnderPotCut();//����̽ͷ��·
		  DetectUnderPotHot();//���׳���
                }
                CLEAR_WD;

                if(P1CONL == 0xFD)//ֻ�ڿ�ͨ״̬�¼��
                {                                         
                  DetectTransformerCut();//���̶��˻����������������
		  DetectIgbtError();//IGBT��������
                  DetectNullPot();//�޹���� 
                }

                CLEAR_WD;
		  //��ѹ
		  if ((statusViewNum & ((uint16)1 << 7)) && !haveViewSet)
		  {
			ViewSet(108);
			haveViewSet = TRUE;
		  }
		  //��ѹ
		  if ((statusViewNum & ((uint16)1 << 8)) && !haveViewSet)
		  {
			ViewSet(109);
			haveViewSet = TRUE;
		  }
		  //ȱ��
		  if ((statusViewNum & ((uint16)1 << 9)) && !haveViewSet)
		  {
			ViewSet(108);
			haveViewSet = TRUE;
		  }
		  //��λ��·
		  if ((statusViewNum & ((uint16)1 << 10)) && !haveViewSet)
		  {
			ViewSet(113);
			haveViewSet = TRUE;
		  }
		  //���̿�·
		  if ((statusViewNum & ((uint16)1 << 2)) && !haveViewSet && !temperatureCheckTime)
		  {
			ViewSet(103);
			haveViewSet = TRUE;
		  }
		  //IGBT1̽ͷ��·
		  if ((statusViewNum & ((uint16)1 << 4)) && !haveViewSet && !temperatureCheckTime)
		  {
			ViewSet(105);
			haveViewSet = TRUE;
		  }
		  //IGBT2̽ͷ��·
		  if ((statusViewNum & ((uint16)1 << 6)) && !haveViewSet && !temperatureCheckTime)
		  {
			ViewSet(105);
			haveViewSet = TRUE;
		  }
		//����̽ͷ��·
		if ((statusViewNum & ((uint16)1 << 11)) && !haveViewSet && !temperatureCheckTime)
		{
			ViewSet(107);
			haveViewSet = TRUE;
		}
                CLEAR_WD;
		if (rangeNow == 0)
		{
			//Ϊ0��λʱ
			firstOpen = FALSE;
			if (fanTime == 0)
			{
				fanTime = 1;//�ȴ��رշ��
			}
			if (!haveViewSet)
			{
				ViewSet(0);
			}
			fixPWM(0);
                        nullPot =0;//�޹�����
	                cTransformerCut = 0;//����״̬
                        //���ù���
                        statusViewNum &= ~((uint16)1 << 0);//�޹� ����
                        statusViewNum &= ~((uint16)1 << 1);//���̳�����0 ����
                        statusViewNum &= ~((uint16)1 << 3);//IGBT1������0 ����
                        statusViewNum &= ~((uint16)1 << 5);//IGBT2������0 ����
                        statusViewNum &= ~((uint16)1 << 12);//���׳�����0 ����
                        statusViewNum &= ~((uint16)1 << 13);//IGBT����������0 ����
                        statusViewNum &= ~((uint16)1 << 15);//���̲�ͨ�����������������0 ����
		}
		else
		{
			//���̳���
			if ((statusViewNum & ((uint16)1 << 1)) && !haveViewSet)
			{
				ViewSet(102);
				haveViewSet = TRUE;
			}
			//IGBT1����
			if ((statusViewNum & ((uint16)1 << 3)) && !haveViewSet)
			{
				ViewSet(104);
				haveViewSet = TRUE;
			}
			//IGBT2����
			if ((statusViewNum & ((uint16)1 << 5)) && !haveViewSet)
			{
				ViewSet(104);
				haveViewSet = TRUE;
			}
			//���׳���
			if ((statusViewNum & ((uint16)1 << 12)) && !haveViewSet)
			{
				ViewSet(106);
				haveViewSet = TRUE;
			}
                        //���̲�ͨ���������������
                        if((statusViewNum & ((uint16)1 << 15)) && !haveViewSet)
                        {                        
                            ViewSet(111);
                            haveViewSet = TRUE;
                            nullPot =0;//�޹�����
                        }
			//IGBT��������
			if ((statusViewNum & ((uint16)1 << 13)) && !haveViewSet && !checkTimeOn)
			{
                                if(nulligbtLay<2)
                                {
                                  ViewSet(rangeNow);
                                }
                                else
                                {
				  ViewSet(112);
                                }
				haveViewSet = TRUE;
				checkTimeOn = TRUE;
                                if (igbtErrorCheckTime >=DELAY_TIME)
				{
				    igbtErrorCheckTime =0;
                                    if(nulligbtLay <3)
                                    {
                                      nulligbtLay++;
                                    }
				}
				if(Test_Bit(P3, 3)&& P33interrptOpen())
				{
                                  if(igbtErrorCheckTime == 0)//ʱ���������ڸ�λ���״̬
                                  {
                                    fixPWM(rangeNow);
                                    nulligbtToLay =1;
                                    igbtErrorCheckTime++;
                                  }
                                }
                                else
                                {
                                    fixPWM(0);
                                    nulligbtToLay =0;
                                }
                                
                                nullPot =0;//�޹�����
                                //igbtError=0;//IGBT��������
                                //cTransformer = 0;//���������
	                        cTransformerCut = 0;//����״̬
			}
                        /*
			//���̲�ͨ���������������
			if ((statusViewNum & ((uint16)1 << 15)) && !haveViewSet && !checkTimeOn)
			{
                          if(nullTransformerCut<3)
                          {
                            ViewSet(rangeNow);
                          }
                          else
                          {
			    ViewSet(111);
                          }
				haveViewSet = TRUE;
				checkTimeOn = TRUE;
				if (cTransformerCutCheckTime>=DELAY_TIME)
				{
			            cTransformerCutCheckTime=0;
                                    checkTransformerCut =0;
                                    if(nullTransformerCut<3)
                                    {
                                      nullTransformerCut++;
                                    }
				}
                                if(checkTransformerCut >4)
                                {
                                    fixPWM(0);
                                }
                                else
                                {
                                    fixPWM(rangeNow);
                                }
                                if(checkTransformerCut<=4)
                                {
                                    checkTransformerCut++;
                                }
                                nullPot =0;//�޹�����
                                //  igbtError=0;//IGBT��������
                                //cTransformer = 0;//���������
	                        //cTransformerCut = 0;//����״̬
			}
			else
			{
				cTransformerCutCheckTime = DELAY_TIME+1;
			}*/
                        //�޹�
			if ((statusViewNum & ((uint16)1 << 0)) && !haveViewSet && !checkTimeOn)
			{
                            if(nullPotLay <2)
                            {
                              ViewSet(rangeNow);
                            }
                            else
                            {
                              ViewSet(101);
                            }
				
				haveViewSet = TRUE;
				checkTimeOn = TRUE;
				if (nullPotCheckTime >=DELAY_TIME)
				{
				    nullPotCheckTime = 0;
                                    checkNullPot =0;
                                    if(nullPotLay<3)
                                    {
                                      nullPotLay++;
                                    }
				}
                                if(checkNullPot>4)
                                {
                                    fixPWM(0);                               
                                }
                                else
                                {
                                    fixPWM(rangeNow);
                                }
                                if(checkNullPot<=4)
                                {
                                  checkNullPot++;
                                }
                                //nullPot =0;//�޹�����
                                //  igbtError=0;//IGBT��������
                                //cTransformer = 0;//���������
	                        cTransformerCut = 0;//����״̬
			}
			if (!haveViewSet)
			{
				if (firstOpen)
				{
					fixPWM(0);//�ر����
					ViewSet(0);//��ʾ0��λ
					if (fanTime == 0)
					{
						fanTime = 1;//�ȴ��رշ��
					}
                                        //0��������0
                                        nullPot =0;//�޹�����
                                      //  igbtError=0;//IGBT��������
                                        //cTransformer = 0;//���������
	                                cTransformerCut = 0;//����״̬
				}
				else
				{
					fixPWM(rangeNow);//�������
					ViewSet(rangeNow);//��ʾ��λ
					fanTime = 0;//�������   
				}
			}
			else
			{
                          	if (firstOpen || !checkTimeOn)
				{
				    fixPWM(0);//�ر����
                                    nullPot =0;//�޹�����
                                      //  igbtError=0;//IGBT��������
                                    //cTransformer = 0;//���������
	                            cTransformerCut = 0;//����״̬
                                }
				if (fanTime == 0)
				{
					fanTime = 1;//�ȴ��رշ��
				}
			}
		}
//#endif
	}
}
#pragma inline=forced
void DetectNullPot()
{
	uint16 temp_2 = (uint16)1 << 0;
        uint4 temp = get_13ADC();// 1��ʾ�޹�
	if ((temp != 1) && !(statusViewNum & temp_2))
	{
		//��⵽�й�����ʾ�й�
		nullPot = 0;
		return;
	}
	if ((temp != 1) && (statusViewNum & temp_2))
	{
		//��⵽�й�����ʾ�޹�
		delay(2);//��ʱ2ms
		if (get_13ADC() == 1)
			return;
		statusViewNum &= ~temp_2;//��0 ����
		nullPot = 0;
		return;
	}
	if ((temp == 1) && (statusViewNum & temp_2))
	{
		//��⵽�޹����޹�
                nullPot=0;
		return;
	}
	if ((temp == 1) && !(statusViewNum & temp_2))
	{
		//��⵽�޹����й�
		delay(2);//��ʱ2ms
		if (get_13ADC() != 1)
			return;
                nullPot++;
		if (nullPot >= 10)
		{
			nullPot = 0;
                        nullPotCheckTime =0;
                        nullPotLay =0;
                        checkNullPot =0;
			statusViewNum |= temp_2;//��1 �޹�״̬
		}
	}
}
#pragma inline=forced
void DetectCoilHot()
{
	uint16 temp_2 = (uint16)1 << 1;
	uint4 temp = get_07ADC();//1 �¸�
	if ((temp != 1) && !(statusViewNum & temp_2))
	{
		//����¶���������ʾ�¶�����
		//coilHot = 0;
		return;
	}
	if ((temp != 1) && (statusViewNum & temp_2))
	{
		//����¶���������ʾ�¶Ȳ�����
		//delay(2);//��ʱ2ms
		//if (get_07ADC() == 1)
		//	return;
		//statusViewNum &= ~temp_2;//��0 ����
		//coilHot = 0;
		return;
	}
	if ((temp == 1) && (statusViewNum & temp_2))
	{
		//����¶Ȳ���������ʾ�¶Ȳ�����
		//coilHot = 0;
		return;
	}
	if ((temp == 1) && !(statusViewNum & temp_2))
	{
		//����¶Ȳ���������ʾ�¶�����
		delay(2);//��ʱ2ms
		if (get_07ADC() != 1)
			return;
		//coilHot++;
		//if (coilHot >= 3)
		//{
		//	coilHot = 0;
			statusViewNum |= temp_2;//��1 ������
		//}
	}

}
#pragma inline=forced
void DetectCoilCut()
{
	uint16 temp_2 = (uint16)1 << 2;
	uint4 temp = get_07ADC();//2 ̽ͷ��·
	if ((temp != 2) && !(statusViewNum & temp_2))
	{
		//�����������ʾ����
		//coilCut = 0;
		return;
	}
	if ((temp != 2) && (statusViewNum & temp_2))
	{
		//�����������ʾ������
		delay(2);//��ʱ2ms
		if (get_07ADC() == 2)
			return;
		statusViewNum &= ~temp_2;//��0 ����
		//coilCut = 0;
		return;
	}
	if ((temp == 2) && (statusViewNum & temp_2))
	{
		//��鲻��������ʾ������
		//coilCut = 0;
		return;
	}
	if ((temp == 2) && !(statusViewNum & temp_2))
	{
		//��鲻��������ʾ����
		delay(2);//��ʱ2s
		if (get_07ADC() != 2)
			return;
		//coilCut++;
		//if (coilCut >= 3)
		//{
		//	coilCut = 0;
			statusViewNum |= temp_2;//��1 ������
		//}
	}
}
#pragma inline=forced
void DetectIGBTHot_1()
{
	uint16 temp_2 = (uint16)1 << 3;
	uint4 temp = get_04ADC();//2����
	if ((temp != 2) && !(statusViewNum & temp_2))
	{
		//����������
		//igbtHot_1 = 0;
		return;
	}
	if ((temp != 2) && (statusViewNum & temp_2))
	{
		//�����Ҳ�����
		//delay(2);
		//if (get_04ADC() == 2)
		//	return;
		//statusViewNum &= ~temp_2;//��0 ����
		//igbtHot_1 = 0;
		return;
	}
	if ((temp == 2) && (statusViewNum & temp_2))
	{
		//�������Ҳ�����
		//igbtHot_1 = 0;
		return;
	}
	if ((temp == 2) && !(statusViewNum & temp_2))
	{
		//������������
		delay(2);
		if (get_04ADC() != 2)
			return;
		//igbtHot_1++;
		//if (igbtHot_1 >= 3)
		//{
		//	igbtHot_1 = 0;
			statusViewNum |= temp_2;//��1 ������
		//}
	}
}
#pragma inline=forced
void DetectIGBTCut_1()
{
	uint16 temp_2 = (uint16)1 << 4;
	uint4 temp = get_04ADC();//1��·
	if ((temp != 1) && !(statusViewNum & temp_2))
	{
		//����������
		//igbtCut_1 = 0;
		return;
	}
	if ((temp != 1) && (statusViewNum & temp_2))
	{
		//�����Ҳ�����
		delay(2);
		if (get_04ADC() == 1)
			return;
		statusViewNum &= ~temp_2;//��0 ����
		//igbtCut_1 = 0;
		return;
	}
	if ((temp == 1) && (statusViewNum & temp_2))
	{
		//�������Ҳ�����
		//igbtCut_1 = 0;
		return;
	}
	if ((temp == 1) && !(statusViewNum & temp_2))
	{
		//������������
		delay(2);
		if (get_04ADC() != 1)
			return;
		//igbtCut_1++;
		//if (igbtCut_1 >= 3)
		//{
		//	igbtCut_1 = 0;
			statusViewNum |= temp_2;//��1 ������
		//}
	}
}
#pragma inline=forced
void DetectIGBTHot_2()
{
	uint16 temp_2 = (uint16)1 << 5;
	uint4 temp = get_11ADC();//2����
	if ((temp != 2) && !(statusViewNum & temp_2))
	{
		//����������
		//igbtHot_2 = 0;
		return;
	}
	if ((temp != 2) && (statusViewNum & temp_2))
	{
		//�����Ҳ�����
		//delay(2);
		//if (get_11ADC() == 2)
		//	return;
		//statusViewNum &= ~temp_2;//��0 ����
		//igbtHot_2 = 0;
		return;
	}
	if ((temp == 2) && (statusViewNum & temp_2))
	{
		//�������Ҳ�����
		//igbtHot_2 = 0;
		return;
	}
	if ((temp == 2) && !(statusViewNum & temp_2))
	{
		//������������
		delay(2);
		if (get_11ADC() != 2)
			return;
		//igbtHot_2++;
		//if (igbtHot_2 >= 3)
		//{
		//	igbtHot_2 = 0;
			statusViewNum |= temp_2;//��1 ������
		//}
	}
}
#pragma inline=forced
void DetectIGBTCut_2()
{
	uint16 temp_2 = (uint16)1 << 6;
	uint4 temp = get_11ADC();//1��·
	if ((temp != 1) && !(statusViewNum & temp_2))
	{
		//����������
		//igbtCut_2 = 0;
		return;
	}
	if ((temp != 1) && (statusViewNum & temp_2))
	{
		//�����Ҳ�����
		delay(2);
		if (get_11ADC() == 1)
			return;
		statusViewNum &= ~temp_2;//��0 ����
		//igbtCut_2 = 0;
		return;
	}
	if ((temp == 1) && (statusViewNum & temp_2))
	{
		//�������Ҳ�����
		//igbtCut_2 = 0;
		return;
	}
	if ((temp == 1) && !(statusViewNum & temp_2))
	{
		//������������
		delay(2);
		if (get_11ADC() != 1)
			return;
		//igbtCut_2++;
		//if (igbtCut_2 >= 3)
		//{
		//	igbtCut_2 = 0;
			statusViewNum |= temp_2;//��1 ������
		//}
	}
}
#pragma inline=forced
void DetectVLow()
{
	uint16 temp_2 = (uint16)1 << 7;
	uint4 temp_1 = 0;
        uint4 temp = 0;
	if (statusViewNum & temp_2)
	{
		//��ѹ��
		temp_1 = 2;
	}
	else if (statusViewNum & ((uint16)1 << 8))
	{
		//��ѹ��
		temp_1 = 3;
	}
	else if (statusViewNum & ((uint16)1 << 9))
	{
		//ȱ��
		temp_1 = 1;
	}
	temp = get_03ADC(temp_1);//2 ѹ��
	if ((temp != 2) && !(statusViewNum & temp_2))
	{
		//����������
		vLow = 0;
		return;
	}
	if ((temp != 2) && (statusViewNum & temp_2))
	{
		//�����Ҳ�����
		delay(2);
		if (get_03ADC(temp_1) == 2)
			return;
		statusViewNum &= ~temp_2;//��0 ����
		vLow = 0;
		return;
	}
	if ((temp == 2) && (statusViewNum & temp_2))
	{
		//�������Ҳ�����
		vLow = 0;
		return;
	}
	if ((temp == 2) && !(statusViewNum & temp_2))
	{
		//������������
		delay(2);
		if (get_03ADC(temp_1) != 2)
			return;
		vLow++;
		if (vLow >= 3)
		{
			vLow = 0;
			statusViewNum |= temp_2;//��1 ������
		}
	}
}
#pragma inline=forced
void DetectVHight()
{
	uint16 temp_2 = (uint16)1 << 8;
	uint4 temp_1 = 0;
        uint4 temp =0;
	if ((statusViewNum & ((uint16)1 << 7)))
	{
		//��ѹ��
		temp_1 = 2;
	}
	else if ((statusViewNum & temp_2))
	{
		//��ѹ��
		temp_1 = 3;
	}
	else if ((statusViewNum & ((uint16)1 << 9)))
	{
		//ȱ��
		temp_1 = 1;
	}
	temp = get_03ADC(temp_1);//2 ѹ��
	if ((temp != 3) && !(statusViewNum & temp_2))
	{
		//����������
		vHight = 0;
		return;
	}
	if ((temp != 3) && (statusViewNum & temp_2))
	{
		//�����Ҳ�����
		delay(2);
		if (get_03ADC(temp_1) == 3)
			return;
		statusViewNum &= ~temp_2;//��0 ����
		vHight = 0;
		return;
	}
	if ((temp == 3) && (statusViewNum & temp_2))
	{
		//�������Ҳ�����
		vHight = 0;
		return;
	}
	if ((temp == 3) && !(statusViewNum & temp_2))
	{
		//������������
		delay(2);
		if (get_03ADC(temp_1) != 3)
			return;
		vHight++;
		if (vHight >= 3)
		{
			vHight = 0;
			statusViewNum |= temp_2;//��1 ������
		}
	}
}
#pragma inline=forced
void DetectVCut()
{
	uint16 temp_2 = (uint16)1 << 9;
	uint4 temp_1 = 0;
        uint4 temp =0;
	if ((statusViewNum & ((uint16)1 << 7)))
	{
		//��ѹ��
		temp_1 = 2;
	}
	else if ((statusViewNum & ((uint16)1 << 8)))
	{
		//��ѹ��
		temp_1 = 3;
	}
	else if ((statusViewNum & temp_2))
	{
		//ȱ��
		temp_1 = 1;
	}
	temp = get_03ADC(temp_1);//1 ȱ��
	if ((temp != 1) && !(statusViewNum & temp_2))
	{
		//����������
		vCut = 0;
		return;
	}
	if ((temp != 1) && (statusViewNum & temp_2))
	{
		//�����Ҳ�����
		delay(2);
		if (get_03ADC(temp_1) == 1)
			return;
		statusViewNum &= ~temp_2;//��0 ����
		vCut = 0;
		return;
	}
	if ((temp == 1) && (statusViewNum & temp_2))
	{
		//�������Ҳ�����
		vCut = 0;
		return;
	}
	if ((temp == 1) && !(statusViewNum & temp_2))
	{
		//������������
		delay(2);
		if (get_03ADC(temp_1) != 1)
			return;
		vCut++;
		if (vCut >= 3)
		{
			vCut = 0;
			statusViewNum |= temp_2;//��1 ������
		}
	}
}
#pragma inline=forced
void DetectSwitchCut()
{
	uint16 temp_2 = (uint16)1 << 10;
	uint4 temp = get_05ADC();//1 ��·
	if ((temp != 1) && !(statusViewNum & temp_2))
	{
		//����������
		//switchCut = 0;
		return;
	}
	if ((temp != 1) && (statusViewNum & temp_2))
	{
		//�����Ҳ�����
		delay(2);
		if (get_05ADC() == 1)
			return;
		statusViewNum &= ~temp_2;//��0 ����
		//switchCut = 0;
		return;
	}
	if ((temp == 1 )&& (statusViewNum & temp_2))
	{
		//�������Ҳ�����
		//switchCut = 0;
		return;
	}
	if ((temp == 1) && !(statusViewNum & temp_2))
	{
		//������������
		delay(2);
		if (get_05ADC() != 1)
			return;
		//switchCut++;
		//if (switchCut >= 3)
		//{
		//	switchCut = 0;
			statusViewNum |= temp_2;//��1 ������
		//}
	}
}
#pragma inline=forced
void DetectUnderPotCut()
{
	uint16 temp_2 = (uint16)1 << 11;
	uint4 temp = get_06ADC();//2 ����̽ͷ��·
	if ((temp != 2) && !(statusViewNum & temp_2))
	{
		//����������
		//underPotCut = 0;
		return;
	}
	if ((temp != 2) && (statusViewNum & temp_2))
	{
		//�����Ҳ�����
		delay(2);
		if (get_06ADC() == 2)
			return;
		statusViewNum &= ~temp_2;//��0 ����
		//underPotCut = 0;
		return;
	}
	if ((temp == 2) && (statusViewNum & temp_2))
	{
		//�������Ҳ�����
		//underPotCut = 0;
		return;
	}
	if ((temp == 2) && !(statusViewNum & temp_2))
	{
		//������������
		delay(2);
		if (get_06ADC() != 2)
			return;
		//underPotCut++;
		//if (underPotCut >= 3)
		//{
		//	underPotCut = 0;
			statusViewNum |= temp_2;//��1 ������
		//}
	}
}
#pragma inline=forced
void DetectUnderPotHot()
{
	uint16 temp_2 = (uint16)1 << 12;
	uint4 temp = get_06ADC();//1����
	if ((temp != 1) && !(statusViewNum & temp_2))
	{
		//����������
		//underPotHot = 0;
		return;
	}
	if ((temp != 1) && (statusViewNum & temp_2))
	{
		//�����Ҳ�����
		//delay(2);
		//if (get_06ADC() == 1)
		//	return;
		//statusViewNum &= ~temp_2;//��0 ����
		//underPotHot = 0;
		return;
	}
	if ((temp == 1) && (statusViewNum & temp_2))
	{
		//�������Ҳ�����
		//underPotHot = 0;
		return;
	}
	if ((temp == 1 )&& !(statusViewNum & temp_2))
	{
		//������������
		delay(2);
		if (get_06ADC() != 1)
			return;
		//underPotHot++;
		//if (underPotHot >= 3)
		//{
		//	underPotHot = 0;
			statusViewNum |= temp_2;//��1 ������
		//}
	}
}
#pragma inline=forced
void DetectIgbtError()
{
	uint16 temp_2 = (uint16)1 << 13;
	uint4 temp = Test_Bit(P3, 3);//0������
	if (temp && P33interrptOpen() && !(statusViewNum & temp_2))
	{
		//����������
		//igbtError = 0;
		return;
	}
	if (temp && P33interrptOpen() && (statusViewNum & temp_2))
	{
		//�����Ҳ�����
		delay(2);
		if (!Test_Bit(P3, 3))
			return;	
                if(nulligbtToLay>=4)
                {
                    statusViewNum &= ~temp_2;//��0 ����
                }
                else if(nulligbtToLay>=1)
                {
                  nulligbtToLay++;
                }
		return;
	}
	if ((!temp||!P33interrptOpen()) && (statusViewNum & temp_2))
	{
		//�������Ҳ�����
		return;
	}
	if ((!temp||!P33interrptOpen()) && !(statusViewNum & temp_2))
	{
		//������������
		//delay(2);
		//if (Test_Bit(P3, 3))
		//	return;
                statusViewNum |= temp_2;//��1 ������
                nulligbtLay =0;
                nulligbtToLay =0;//���ù��ϻָ�ʱ��
	}

}
/*
#pragma inline=forced
void DetectTransformer()
{
	uint16 temp_2 = (uint16)1 << 14;
	temp = Test_Bit(P3, 4);//0 ������
	if (temp && !(statusViewNum & temp_2))
	{
		//����������
		cTransformer = 0;
		return;
	}
	if (temp && (statusViewNum & temp_2))
	{
		//�����Ҳ�����
		delay(2);
		if (!Test_Bit(P3, 4))
			return;
		statusViewNum &= ~temp_2;//��0 ����
		cTransformer = 0;
		return;
	}
	if (!temp && (statusViewNum & temp_2))
	{
		//�������Ҳ�����
                cTransformer = 0;
		return;
	}
	if (!temp && !(statusViewNum & temp_2))
	{
		//������������
		delay(2);
		if (Test_Bit(P3, 4))
			return;
		cTransformer++;
		if (cTransformer >= 3)
		{
			cTransformer = 0;
			statusViewNum |= temp_2;//��1 ������
		}
	}

}*/
#pragma inline=forced
void DetectTransformerCut()
{
	uint16 temp_2 = (uint16)1 << 15;
	uint4 temp = getADCNum(12);//0��ʾ���̶���
	if (temp && !(statusViewNum & temp_2))
	{
		//����������
		cTransformerCut = 0;
		return;
	}
	if (temp && (statusViewNum & temp_2))
	{
		//�����Ҳ�����
		delay(2);
		if (!getADCNum(12))
			return;
		statusViewNum &= ~temp_2;//��0 ����
		cTransformerCut = 0;
		return;
	}
	if (!temp&& (statusViewNum & temp_2))
	{
		//�������Ҳ�����
		//delay(2);
                //cTransformerCut = 0;
		return;
	}
	if (!temp && !(statusViewNum & temp_2))
	{
		//������������
		delay(2);
		if (getADCNum(12))
			return;
		cTransformerCut++;
		if (cTransformerCut >= 8)
		{
			cTransformerCut = 0;
                        //checkTransformerCut =0;
                        //nullTransformerCut =0;
			statusViewNum |= temp_2;//��1 ������
		}
	}

}
#pragma inline=forced
void SwitchSet()
{
	uint4 rangeNext = getSwitchs();
	if (rangeNext != rangeNow && rangeNext != 9)
	{
          rangeNow=rangeNext;
          BUZZ_ON;
	}
}
#ifdef Screen_74HC164
void ViewSet(uint8 ShowNum)
{
	setNum_74HC164(ShowNum);
	whileUpdate_74HC164();
}
#elif defined Screen_TM1629
void ViewSet(uint8 ShowNum)
{
        uint16 tempnum =0;
        if(ShowNum>100)
        {
          set_TM1629_Up(ShowNum);
        }
        else
        {
           set_TM1629_Up(POWER_RATE[ShowNum]);
        }
	set_TM1629_LeftNum(rangeNow);
        set_TM1629_Leftstring(getPWMRate());
	if (ShowNum<100 && ShowNum>0)//�¶�ģʽ
	{
                tempnum =getTemperatureByAnum(6);
          	if(tempurature+3<tempnum)
                {
                  tempurature++;
                }
                else if(tempurature-3>tempnum)
                {
                  tempurature--;
                }
		// tempreture =getTemperatureByAnum(6);//�����¶�
		//getADCNum(13) ���뻥�������� AD
		//getADCNum(12) ���������������С
		set_TM1629_Down(tempurature, 1);            
	}
	else//ʱ��ģʽ
	{
		set_TM1629_Down(0, 0);
                //set_TM1629_Down(getVo(), 1);
	}
	whileUpdate_TM1629();
}
#endif
//�ж�
#pragma vector=0x00
__interrupt void int_9488()
{
	if (TINTPND & 0x01)//timer A(interal timer)
	{
                TAInterupt();
		TINTPND &= ~0x01;
	}
	else if (P3PND & 0x08)//int3(pulse input)
	{
		P3PND &= ~0x08;
	}
	else if (P3PND & 0x01)//int0(active pulse input)
	{
		P33Interupt();
                P3PND &= ~0x01;
	}
	else if (P3PND & 0x02)//int1(reactive pulse input)
	{	
		P34Interupt();
                P3PND &= ~0x02;
	}
	else if (P3PND & 0x04)//int2(powercut input)
	{
		P3PND &= ~0x04;
	}
	else if (UARTPND & 0x02)//receive
	{
		UARTPND &= ~0x02;
	}
	else if (UARTPND & 0x01)//transfer
	{
		UARTPND &= ~0x01;
	}
	else if (WTCON & 0x01)
	{
		WTCON &= ~0x01;
	}
	else if (TINTPND & 0x04)//timer B(interal timer)
	{
		TINTPND &= ~0x04;
	}
}
#pragma inline=forced
//A��ʱ��
void TAInterupt()
{
	CLEAR_WD;
	count_60ms++;
        if(nullPotCheckTime <DELAY_TIME)
        {
          nullPotCheckTime++;
        }
        if(igbtErrorCheckTime < DELAY_TIME)
        {
          igbtErrorCheckTime++;
        }
        //if(cTransformerCutCheckTime<DELAY_TIME)
        //{
        //  cTransformerCutCheckTime++;
        //}
        
	if (BUZZ_Test)
	{
		buzzTime++;
		if (buzzTime >= BUZZ_ALL_TIME)
		{
			buzzTime = 0;
			BUZZ_OFF;
		}
		else
		{
			BUZZ_ON;
		}
	}
	if (count_60ms == 2)//60ms
	{
		count_60ms = 0;
		count_2s++;
		count_1s++;

		if (count_2s == 34)//2s
		{
			count_2s = 0;

			if (fanTime >= 1 && fanTime<FAN_ALL_TIME)
			{
				fanTime++;
			}

			if (fanTime <FAN_ALL_TIME - 5)
			{
				FAN_ON;
			}
			else
			{
				FAN_OFF;
			}
			CLEAR_WD;
		}
		if (count_1s == 17)
		{
			count_1s = 0;
                        if(turnOnLay<TURN_ALL_TIME)
                        {
                          turnOnLay++;
                        }
			Com_Bit(P2, 6);
			//INTERUPT ��������
#ifdef Screen_74HC164
			interuptUpdate_74HC164();
#elif defined Screen_TM1629
			interuptUpdate_TM1629();
#endif
                    if(P1CONL == 0xFD && temperatureCheckTime)//ֻ�ڿ�ͨ״̬����¶���ת
                    {
                      temperatureCheckTime--;//��·��ʱ����ʱ
                    }
		}
	}
	CLEAR_WD;
}
#pragma inline=forced
void P33Interupt()
{
   //�ر����
  if(P1CONL == 0xFD)//�ڿ���״̬
  {
    P3INT &= 0xFC;//�ر��ж�
  }
}
#pragma inline=forced
uint4 P33interrptOpen()//1��ʾ����
{
  return (P3INT & 0x02) != 0;
}
#pragma inline=forced
void P34Interupt()
{
  //��λ����
  if(P1CONL == 0xFD)//�ڿ���״̬
  {
    P3INT &= 0xF3;//�ر��ж�
  }
}
