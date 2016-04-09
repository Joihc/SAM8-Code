#define _TM1629_C_

#include "TM1629.h"
/*
	Function List : 1 indate()  д��
			2 display() ���õ�ַ�Զ���1��ʾ
			3 display2() ���ù̶���ַ��ʽ��ʾ
*/

__code const uint8 RightCode[]={
  0x7E, //0 111 1110
  0x30,//1 011 0000
  0x6D, //2 110 1101
  0x79, //3 111 1001
  0x33, //4 011 0011
  0x5B, //5 101 1011
  0x5F, //6 101 1111
  0x70, //7 111 0000
  0x7F, //8 111 1111
  0x7B, //9 111 1011
  0x01, //10 0000 0001  -��
  0x00,//����ʾ

  0x77, //A 0111 0111
  0x1F, //B 0001 1111
  0x4E, //C 0100 1110

};
__code const uint8 LeftCode[]={
  0x3F, //0  00 11 1111
  0x18,//1  00 01 1000
  0x36, //2  0011 0110
  0x3C, //3 0011 1100
  0x19, //4 0001 1001
  0x2D, //5 0010 1101
  0x2F, //6 0010 1111
  0x38, //7 0011 1000
  0x3F, //8 0011 1111
  0x3D, //9 0011 1101
  0x00, //X 0000 0000 -��
  0x00,//����ʾ

  0X27,//E 00 10 0111
};


//C0H��ʼ
volatile uint8 buffCode_TM1629[]={
    0x01, //0
    0x3E, //1 00 11 1110
    0x00, //2
    0x00, //3
    0x00, //4
    0x00, //5
    0x00, //6
    0x00, //7
    0x04, //8
    0x00, //9
    0x00, //10
    0x00, //11
    0x00, //12
    0x00, //13
    0x00, //14
    0x3E, //15
};

volatile uint8 startTimeSecond=0;
volatile uint8 startTimeMin =0;
volatile uint8 startTimeHour =0;
volatile uint8 stateTime =0;

enum LED_STATE upNumState;
enum LED_STATE timeMarkState;
enum LED_STATE tempMarkState;
enum LED_STATE waterState;
enum LED_STATE fellState;
enum LED_STATE tempState;
enum LED_STATE potState;
enum LED_STATE hotState;
enum LED_STATE proState;
enum LED_STATE colonState;
enum LED_STATE phoneState;
enum LED_STATE vlotateState;
enum LED_STATE kwState;
enum LED_STATE buzzState;

uint4 lastState;
uint4 listIndex;

void whileUpdate_TM1629()
{
  set_TM1629_TimeMark();
  set_TM1629_TempMark();
  set_TM1629_Waterg();
  set_TM1629_Fell();
  set_TM1629_Temp();
  set_TM1629_Pot();
  set_TM1629_Hot();
  set_TM1629_Pro();
  set_TM1629_Colon();
  set_TM1629_Phone();
  set_TM1629_Vlotage();
  set_TM1629_UpNum();
  set_TM1629_Kw();
  display(1);
  stateTime = startTimeSecond;
}
void interuptUpdate_TM1629()
{
  startTimeSecond++;
  if(startTimeSecond >=60)
  {
    startTimeSecond =0;
    startTimeMin++;
    if(startTimeMin>=60)
    {
      startTimeMin =0;
      startTimeHour++;
    }
  }
}
void init_TM1629()
{
  //DIO_1;
  //CLK_1;
  //STB_1;
  //indate(dissetmode);
  //STB_1;
  display(0);//
  lastState =0;
}
//�������� 1-8
void set_TM1629_Leftstring(unsigned char n)
{
  switch(n)
  {
    case 1:
    buffCode_TM1629[10] =0xC0;
    buffCode_TM1629[11] =0x3F;
    buffCode_TM1629[14] =0x00;
    buffCode_TM1629[15] &=0x3E;  //0011 1110
    break;
    case 2:
    buffCode_TM1629[10] =0xF0;
    buffCode_TM1629[11] =0xFF;
    buffCode_TM1629[14] =0x00;
    buffCode_TM1629[15] &=0x3E;  //0011 1110
    break;
    case 3:
    buffCode_TM1629[10] =0xFE;
    buffCode_TM1629[11] =0xFF;
    buffCode_TM1629[14] =0x00;
    buffCode_TM1629[15] &=0x3E;  //0011 1110
    break;
  case 4:
    buffCode_TM1629[10] =0xFF;
    buffCode_TM1629[11] =0xFF;
    buffCode_TM1629[14] =0x03;
    buffCode_TM1629[15] &=0x3E;  //0011 1110
    break;
    case 5:
    buffCode_TM1629[10] =0xFF;
    buffCode_TM1629[11] =0xFF;
    buffCode_TM1629[14] =0x0F;
    buffCode_TM1629[15] &=0x3E;  //0011 1110
    buffCode_TM1629[15] |=0x40;  //0011 1110
    break;
    case 6:
    buffCode_TM1629[10] =0xFF;
    buffCode_TM1629[11] =0xFF;
    buffCode_TM1629[14] =0x1F;
    buffCode_TM1629[15] &=0x3E;  //0011 1110
    buffCode_TM1629[15] |=0xC0;  //0011 1110
    break;
    case 7:
    buffCode_TM1629[10] =0xFF;
    buffCode_TM1629[11] =0xFF;
    buffCode_TM1629[14] =0x7F;
    buffCode_TM1629[15] &=0x3E;  //0011 1110
    buffCode_TM1629[15] |=0xC0;  //0011 1110
    break;
  case 8:
    buffCode_TM1629[10] =0xFF;
    buffCode_TM1629[11] =0xFF;
    buffCode_TM1629[14] =0xFF;
    buffCode_TM1629[15] &=0x3E;  //0011 1110
    buffCode_TM1629[15] |=0xC1;  //0011 1110
    break;
  default:
    buffCode_TM1629[10] =0x00;
    buffCode_TM1629[11] =0x00;
    buffCode_TM1629[14] =0x00;
    buffCode_TM1629[15] &=0x3E;  //0011 1110
    break;
  }
  //display2(0xCA,buffCode_TM1629[10]);
  //display2(0xCB,buffCode_TM1629[11]);
  //display2(0xCE,buffCode_TM1629[14]);
  //display2(0xCF,buffCode_TM1629[15]);
}
//���õ�λ
void set_TM1629_LeftNum(unsigned char n)
{
  switch(n)
  {
    case 1:
    buffCode_TM1629[12] =0X20;//|=(1<<5);
    buffCode_TM1629[13] =0x20;// |=(1<<5);
    break;
    case 2:
    buffCode_TM1629[12] =0x10;//|=(1<<4);
    buffCode_TM1629[13] =0x10;//|=(1<<4);
    break;
    case 3:
    buffCode_TM1629[12] =0x00;
    buffCode_TM1629[13] =0x88;//|=(1<<3);
    break;
    case 4:
    buffCode_TM1629[12] =0x00;
    buffCode_TM1629[13] =0x44;//|=(1<<6);
    break;
    case 5:
    buffCode_TM1629[12] =0x08;//|=(1<<3);
    buffCode_TM1629[13] =0x02;//|=(1<<1);
    break;
    case 6:
    buffCode_TM1629[12] =0x04;//|=(1<<2);
    buffCode_TM1629[13] =0x01;//|=(1<<0);
    break;
    case 7:
    buffCode_TM1629[12] =0x82;//|=(1<<7);
    buffCode_TM1629[13] =0x00;
    break;
    case 8:
    buffCode_TM1629[12] =0x41;//|=(1<<6);
    buffCode_TM1629[13] =0x00;
    break;
    default:
    buffCode_TM1629[12] =0x00;//|=(1<<6);
    buffCode_TM1629[13] =0x00;//|=(1<<0);
    break;
  }
  //display2(0xCC,buffCode_TM1629[12]);
  //display2(0xCD,buffCode_TM1629[13]);
}
/* up = 101 �޹�
        102 ���̳���
        103 ����̽ͷ��·
        104 IGBT����
        105 IGBT ��·
        106 ��ѹ��
        107 ��ѹ��
        108 ��ѹȱ��
        109 ��λ���ؿ�·
        110 ����̽ͷ��·
        111 ����̽ͷ����

        112 igbt������ͨ

        200+KW �޹�
        KW     ����
*/
void set_TM1629_Up(uint8 up)
{
  uint4 head =0;
  uint4 tail =0;
  switch (up) {
    case 101:
    head = 12;
    tail =1;
    waterState = OFF;
    fellState = OFF;
    tempState = OFF;
    potState = FLUSH;
    hotState = OFF;
    proState = FLUSH;
    phoneState = ON;
    vlotateState = ON;
    upNumState = FLUSH;
    kwState =OFF;
    buzzState=OFF;
    break;
    case 102:
    head = 12;
    tail = 2;
    waterState = OFF;
    fellState = OFF;
    tempState = FLUSH;
    potState = OFF;
    hotState = OFF;
    proState = FLUSH;
    phoneState = FLUSH;
    vlotateState = ON;
    upNumState = FLUSH;
    kwState =OFF;
     buzzState=ON;
    break;
    case 103:
    head = 12;
    tail = 3;
    waterState = OFF;
    fellState = OFF;
    tempState = OFF;
    potState = OFF;
    hotState = OFF;
    proState = FLUSH;
    phoneState = FLUSH;
    vlotateState = ON;
    upNumState = FLUSH;
    kwState =OFF;
    buzzState=ON;
    break;
    case 104:
    head = 12;
    tail = 4;
    waterState = OFF;
    fellState = OFF;
    tempState = FLUSH;
    potState = OFF;
    hotState = OFF;
    proState = FLUSH;
    phoneState = FLUSH;
    vlotateState = ON;
    upNumState = FLUSH;
    kwState =OFF;
    buzzState=ON;
    break;
    case 105:
    head = 12;
    tail = 5;
    waterState = OFF;
    fellState = OFF;
    tempState = OFF;
    potState = OFF;
    hotState = OFF;
    proState = FLUSH;
    phoneState = FLUSH;
    vlotateState = ON;
    upNumState = FLUSH;
    kwState =OFF;
    buzzState=ON;
    break;
    case 106:
    head = 12;
    tail = 6;
    waterState = OFF;
    fellState = OFF;
    tempState = OFF;
    potState = OFF;
    hotState = OFF;
    proState = FLUSH;
    phoneState = FLUSH;
    vlotateState = ON;
    upNumState = FLUSH;
    kwState =OFF;
    buzzState=ON;
    break;
    case 107:
    head = 12;
    tail = 7;
    waterState = OFF;
    fellState = OFF;
    tempState = OFF;
    potState = OFF;
    hotState = OFF;
    proState = FLUSH;
    phoneState = FLUSH;
    vlotateState = ON;
    upNumState = FLUSH;
    kwState =OFF;
    buzzState=ON;
    break;
    case 108:
    head = 12;
    tail = 8;
    waterState = OFF;
    fellState = OFF;
    tempState = OFF;
    potState = OFF;
    hotState = OFF;
    proState = FLUSH;
    phoneState = FLUSH;
    vlotateState = ON;
    upNumState = FLUSH;
    kwState =OFF;
    buzzState=ON;
    break;
    case 109:
    head = 12;
    tail = 9;
    waterState = OFF;
    fellState = OFF;
    tempState = OFF;
    potState = OFF;
    hotState = OFF;
    proState = FLUSH;
    phoneState = FLUSH;
    vlotateState = ON;
    upNumState = FLUSH;
    kwState =OFF;
    buzzState=ON;
    break;
    case 110:
    head = 12;
    tail = 12;
    waterState = OFF;
    fellState = OFF;
    tempState = OFF;
    potState = OFF;
    hotState = OFF;
    proState = FLUSH;
    phoneState = FLUSH;
    vlotateState = ON;
    upNumState = FLUSH;
    kwState =OFF;
    buzzState=ON;
    break;
    case 111:
    head = 12;
    tail = 13;
    waterState = OFF;
    fellState = OFF;
    tempState = FLUSH;
    potState = OFF;
    hotState = OFF;
    proState = FLUSH;
    phoneState = FLUSH;
    vlotateState = ON;
    upNumState = FLUSH;
    kwState =OFF;
    buzzState=ON;
    break;
  case 112:
    head = 12;
    tail = 14;
    waterState = OFF;
    fellState = FLUSH;
    tempState = OFF;
    potState = OFF;
    hotState = OFF;
    proState = FLUSH;
    phoneState = FLUSH;
    vlotateState = ON;
    upNumState = FLUSH;
    kwState =OFF;
    buzzState=ON;
    break;
    default:
    head = up/10%10;
    tail = up%10;
    waterState = up?FLUSH:OFF;
    fellState = OFF;
    tempState = OFF;
    potState = OFF;
    hotState = up?ON:OFF;
    proState = OFF;
    phoneState = ON;
    vlotateState = up?ON:FLUSH;
    upNumState = ON;
    kwState =ON;
    break;
  }
  set_TM1629_SignNum(2,head?head:11);
  set_TM1629_SignNum(3,tail);
}
//�������������  1��ʾ���� 0
void set_TM1629_Down(int16 num,uint4 trim)//-999 - 9999
{
  uint4 mask=0;
  uint4 i = 7;
  uint4 num_use =0;

  if(trim)//�¶�ģʽ
  {
    timeMarkState = OFF;
    tempMarkState = ON;
    colonState = OFF;
  }
  else//ʱ��ģʽ
  {
    timeMarkState = ON;
    tempMarkState = OFF;
    colonState = FLUSH;
    num = startTimeHour*100+startTimeMin;
  }

  if(num <-999)//��ʾ
  {
    num =-999;
  }
  if(num >9999)
  {
    num =9999;
  }

  mask = (num >=0) ?1:0;
  num = absolute(num);

  if(num)
  {
      num_use =num%10;
      while(num)
      {
        set_TM1629_SignNum(i,num_use);
        i--;
        num /=10;
        num_use =num%10;
      }
      if(mask == 0)
      {
        set_TM1629_SignNum(i,10);
        i--;
      }
   }
   else
   {
      set_TM1629_SignNum(i,0);
      i--;
   }
    for(;i>=4;i--)
    {
      set_TM1629_SignNum(i,trim?11:0);
    }

}
#pragma inline = forced
//pos = 2��3 4����5 6����7 0-9 10��ʾ����  11��ʾ��
void set_TM1629_SignNum(uint4 pos,uint4 marks)
{
  if(pos%2)
  {
    buffCode_TM1629[pos-1] &= 0x80;
    buffCode_TM1629[pos-1] |= RightCode[marks];
  }
  else
  {
    buffCode_TM1629[pos+1] = LeftCode[marks];
    if(marks == 0 || marks==1|| marks== 7 || marks ==11)//����Ҫǰ��λ
    {
      buffCode_TM1629[pos] &=~(1<<7);
    }
    else
    {
      buffCode_TM1629[pos] |=(1<<7);
    }

  }

}
#pragma inline = forced
void set_TM1629_UpNum()
{
  if(upNumState == OFF)
   {
      buffCode_TM1629[2] &=0x00;
      buffCode_TM1629[3] &=0xC0;//11 00 0000
   }
    else if(upNumState == FLUSH)
    {
      if(stateTime%2)
      {
        if(lastState == 0)
        {
          lastState = 1;
          if(buzzState == ON)
          {
            BUZZ_ON;
          }
        }
      }
      else
      {
        buffCode_TM1629[2] &=0x00;
        buffCode_TM1629[3] &=0xC0;
        lastState = 0;
      }
    }
}
#pragma inline = forced
void set_TM1629_TimeMark()
{
  if(timeMarkState == ON) {
    buffCode_TM1629[0]|=(1<<2);
  }
  else if(timeMarkState ==OFF)
  {
    buffCode_TM1629[0]&=~(1<<2);
  }
  else
  {
    if(stateTime%2)
    {
      buffCode_TM1629[0]|=(1<<2);
    }
    else
    {
      buffCode_TM1629[0]&=~(1<<2);
    }
  }
}
#pragma inline = forced
void set_TM1629_TempMark()
{
  if(tempMarkState == ON) {
      buffCode_TM1629[0]|=(1<<1);
  }
  else if(tempMarkState ==OFF)
  {
    buffCode_TM1629[0]&=~(1<<1);
  }
  else
  {
    if(stateTime%2)
    {
      buffCode_TM1629[0]|=(1<<1);
    }
    else
    {
      buffCode_TM1629[0]&=~(1<<1);
    }
  }
}
//���ö�����ˮ
void set_TM1629_Waterg()
{
  switch (waterState) {
    case ON:
    buffCode_TM1629[8] |=0x80;
    buffCode_TM1629[9]=0x3F;
    break;
    case OFF:
    buffCode_TM1629[8] &=0x7F;
    buffCode_TM1629[9]=0x00;
    break;
    case FLUSH:
    switch(stateTime % 6)
    {
      case 0:
        buffCode_TM1629[8] &=0x7E;
        buffCode_TM1629[9]=0x00;
      break;
      case 1:
          buffCode_TM1629[8] &=0x7E;
          buffCode_TM1629[9]=0x30;
      break;
      case 2:
          buffCode_TM1629[8] &=0x7E;
          buffCode_TM1629[9]=0x38;
      break;
      case 3:
          buffCode_TM1629[8] &=0x7E;
          buffCode_TM1629[9]=0x3C;
      break;
      case 4:
          buffCode_TM1629[8] &=0x7E;
          buffCode_TM1629[9]=0x3E;
      break;
      case 5:
          buffCode_TM1629[8] |=0x80;
          buffCode_TM1629[9]=0x3F;
      break;
    }
    break;
  }

  //display2(0xC9,buffCode_TM1629[9]);

}
#pragma inline = forced
//����   0 ��   1��
void set_TM1629_Fell()
{
  if(fellState == ON) {
    buffCode_TM1629[0] |= 0x18;//
  }
   else if(fellState == OFF)
   {
    buffCode_TM1629[0] &= 0xE7;//
   }
  else
  {
    if(stateTime%2)
    {
      buffCode_TM1629[0] |= 0x18;//
    }
    else
    {
      buffCode_TM1629[0] &= 0xE7;//
    }
  }
}
#pragma inline = forced
//����
void set_TM1629_Temp()
{
  if(tempState == ON) {
    buffCode_TM1629[0] |= 0x60;//
  }
  else if(tempState == OFF){
    buffCode_TM1629[0] &= 0x9F;//
  }
  else
  {
    if(stateTime%2)
    {
      buffCode_TM1629[0] |= 0x60;//
    }
    else
    {
      buffCode_TM1629[0] &= 0x9F;//
    }
  }
}
#pragma inline = forced
//��
void set_TM1629_Pot()
{
  if(potState == ON) {
    buffCode_TM1629[0] |= 0x80;//
    buffCode_TM1629[1] |= 0x01;
  }
   else if(potState == OFF)
   {
    buffCode_TM1629[0] &= 0x7F;//
    buffCode_TM1629[1] &= 0xFE;
    }
   else
   {
      if(stateTime%2)
      {
        buffCode_TM1629[0] |= 0x80;//
        buffCode_TM1629[1] |= 0x01;
      }
      else
      {
        buffCode_TM1629[0] &= 0x7F;//
        buffCode_TM1629[1] &= 0xFE;
      }
  }
}
#pragma inline = forced
//����
void  set_TM1629_Hot()
{
  if(hotState == ON) {
    buffCode_TM1629[8] |=0x18;
  }
  else if(hotState ==OFF)
  {
    buffCode_TM1629[8] &=0xE7;
  }
  else
  {
    if(stateTime%2)
    {
      buffCode_TM1629[8] |=0x18;
    }
    else
    {
      buffCode_TM1629[8] &=0xE7;
    }
  }
}
#pragma inline = forced
//����
void set_TM1629_Pro()
{
  if(proState == ON) {
    buffCode_TM1629[8] |=0x60;
  }
  else if(proState == OFF)
  {
    buffCode_TM1629[8] &=0x9F;
  }
  else
  {
    if(stateTime%2)
    {
      buffCode_TM1629[8] |=0x60;
    }
    else
    {
      buffCode_TM1629[8] &=0x9F;
    }
  }
}
#pragma inline = forced
void set_TM1629_Phone()
{
  if(phoneState == ON)
  {
      buffCode_TM1629[1] |=0x3E;//0011 1110
  }
  else if(phoneState == OFF)
  {
      buffCode_TM1629[1] &=0xC1;//1100 0001
  }
  else
  {
      buffCode_TM1629[1] |=0x3E;//0011 1110
      buffCode_TM1629[1]&=~(1<<((stateTime%5) +1));
  }
}
#pragma inline = forced
//1��
void set_TM1629_Vlotage()
{
  if(vlotateState == ON) {
      buffCode_TM1629[8] |=(1<<2);
  }
  else if(vlotateState == OFF)
  {
      buffCode_TM1629[8]&=~(1<<2);
  }
  else
  {
    if(stateTime%2)
    {
      buffCode_TM1629[8] |=(1<<2);
    }
    else
    {
      buffCode_TM1629[8]&=~(1<<2);
    }
  }
}
#pragma inline = forced
//KW
void set_TM1629_Kw()
{
  if(kwState == ON) {
    buffCode_TM1629[0] |=0x01;//0000 0010
  }
  else if(kwState ==OFF){
    buffCode_TM1629[0] &=0xFE;
  }
  else
  {
    if(stateTime%2)
    {
      buffCode_TM1629[0] |=0x01;//0000 0010
    }
    else
    {
      buffCode_TM1629[0] &=0xFE;
    }
  }
}
#pragma inline = forced
//ð�� 0 1��
void set_TM1629_Colon()
{
  if(colonState == ON) {
      buffCode_TM1629[8] |=0x02;//0000 0010
  }
  else if(colonState ==OFF)
  {
      buffCode_TM1629[8] &=0xFD;
  }
  else
  {
    if(stateTime%2)
    {
      buffCode_TM1629[8] |=0x02;//0000 0010
    }
    else
    {
      buffCode_TM1629[8] &=0xFD;
    }
  }
}
#pragma inline = forced
void indate(unsigned char p)
{
	unsigned int i;
	STB_0;	//STB=0;//��֤��STB��Ϊ�͵�ƽ������������֮ǰ�˿ڵ�״̬
	//��֤������ʵ�������в�����֡��˿���ʧ��
	for(i=0;i<8;i++)
	{
		CLK_0;//CLK=0;//�Ƚ���CLK������
		if(p &(1<<0))
		{
			DIO_1;//DIO=1;//��Ҫ���͵����ݵĵ�λΪ��1������ѡ�DIO������
    }
		else
		{
			DIO_0;//DIO=0;//��Ҫ���͵����ݵĵ�λΪ��0������ѡ�DIO���ø�
		}
		CLK_1;//CLK=1;//��ʱ�ӵ�������
		p=p>>1;//׼������һ��BIT
	}//����һ���ֽں��˳�ѭ��
}
#pragma inline = forced
//���õ�ַ�Զ���1��ʽ
void display(short state)
{
	DIO_1;//DIO=1;
	CLK_1;//CLK=1;
	STB_1;//STB=1; //ͨѶ��ʼǰͨѶ�˿�ȫ����ʼ��Ϊ��1��
	indate(writedatamode_z);  //����������������ò��õ�ַ�Զ���1��ʽд��ʾ���ݣ�
  STB_1;//STB=1; //����������������Ҫ����STB���á�1��
	indate(startaddress); //����ʼ��ַ
	//��ַ�����󣬡�STB������Ϊ��0����������Ҫ��ʾ������
  for(listIndex=0;listIndex<datacount;listIndex++)
  {
  		indate(state?buffCode_TM1629[listIndex]:0xFF);  //����������ʾ�����ݹ����У���STB��һֱ����Ϊ��0��
  }
	STB_1;//STB=1; //�������е���ʾ���ݺ����16BYTE���󣬽���STB�á�1��
	indate(disconmode); //����ʾ��������
	STB_1;//STB=1;    //��ʾ���������󽫡�STB���á�1��
}
//----------------------------------------------���ù̶���ַ��ʽ�����ַ�����ݿ�ʼ-----------
//���ù̶���ַ��ʽ
/*void  display2(unsigned char adress,unsigned char disa)
{
	DIO_1;//DIO=1;
	CLK_1;//CLK=1;
	STB_1;//STB=1;  //ͨѶ��ʼǰͨѶ�˿�ȫ����ʼ��Ϊ��1��
	indate(writedatamode_g); //������������
	STB_1;//STB=1;
	indate(adress); //����ʾ���ݶ�Ӧ�ĵ�ַ
	indate(disa); //���֡�STB��=��0������1BYTE��ʾ����
	STB_1;//STB=1; //������ʾ���ݺ󽫡�STB���á�1��
}
*/
#pragma inline = forced
int16 absolute(int16 value)
{
  if(value <0)
  return -value;
  return value;
}