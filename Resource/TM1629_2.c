#define _TM1629_2_C_

#include "TM1629_2.h"
/*
	Function List : 1 indate()  д��
			2 sc2_display() ���õ�ַ�Զ���1��ʾ
			3 display2() ���ù̶���ַ��ʽ��ʾ
*/
__code const uint8 sc2_Num[]={
  0x3F,//0
  0x06,//1
  0x5B,//2
  0x4F,//3
  0x66,//4
  0x6D,//5
  0x7D,//6
  0x07,//7
  0x7F,//8
  0x6F,//9
  0x77,//A
  0x7C,//b
  0x39,//C
  0x5E,//d
  0x79,//E
  0x71,//F
  0x40,//-
  0x00,//�հ�  17
};


//C0H��ʼ
volatile uint8 sc2_Buff[]={
    0xFF, //0
    0x7F, //1 00 11 1110
    0x00, //2
    0x00, //3
    0x00, //4
    0x00, //5
    0x00, //6
    0x00, //7
    0x00, //8
    0x00, //9
    0x00, //10
    0x00, //11
    0x00, //12
    0x00, //13
    0x00, //14
    0x00, //15
};
void sc2_big_num(uint8 ten_bit,uint8 bit);
void sc2_small_num(uint8 tho_bit,uint8 hun_bit,uint8 ten_bit,uint8 bit,bool bl,bool point);
void sc2_error_state(uint8 state);
void sc2_power_lvl(uint8 power_lvl);
void sc2_switch_num(uint8 switch_num);
void sc2_operation();
void set2_bignum_kw(bool state);
void sc2_small_state(bool state,int16 temperature);


uint8 sc2_quickly_time =0;
uint8 sc2_time =0;
uint8 sc2_kw=0;
uint32 sc2_kws=0;//KW/S  ���ٶȵ�ҪX3600
uint8 sc2_listIndex =0;

bool buzz_state = false;

enum LED_STATE sc2_bigNumState;//������״̬
enum LED_STATE sc2_smallNumState;//С����״̬


void sc2_init()
{
  sc2_time = 0;
  sc2_kw = 0;
  sc2_kws = 0;
  sc2_display(0);
}
void sc2_whileUpdate()
{
  sc2_operation();
  sc2_display(1);
}

void sc2_quicklyUpdate()
{
  if(++sc2_quickly_time>60)
  {
    sc2_quickly_time = 0;

  }
}
void sc2_interuptUpdate()
{
  if(++sc2_time>60)
  {
    sc2_time = 0;
  }
  if(PWM_OPEN)
  {
    sc2_kws += sc2_kw;
  }
  if(buzz_state && sc2_time%2)
  {
    BUZZ_ON;
  }
}
//0-99
void sc2_big_num(uint8 ten_bit,uint8 bit)
{
  sc2_Buff[12] &=0x80;
  sc2_Buff[14] &=0x80;
  
  
  if(sc2_bigNumState == OFF || (sc2_bigNumState == FLUSH && !(sc2_time%2)))
  {
      sc2_Buff[12] |=sc2_Num[17];
      sc2_Buff[14] |=sc2_Num[17];
  }
  else
  {
    sc2_Buff[12] |=sc2_Num[bit];
    if(ten_bit == 0)
    {
      sc2_Buff[14] |=sc2_Num[17];
    }
    else
    {
      sc2_Buff[14] |=sc2_Num[ten_bit];
    }
  }
}
//bl �� true, �� false  point ��. true  ��. false
void sc2_small_num(uint8 tho_bit,uint8 hun_bit,uint8 ten_bit,uint8 bit,bool bl,bool point)
{
  sc2_Buff[10]&=0x80;
  sc2_Buff[8]&=0x80;
  sc2_Buff[6]&=0x80;
  sc2_Buff[4]&=0x80;
  
    sc2_Buff[4]|=sc2_Num[bit];
    if(tho_bit == 0)
    {
      if(hun_bit == 0 && !point)
      {
        if(ten_bit == 0 && !point)
        {
          if(bit !=0 && !bl)
          {
            sc2_Buff[6]|=sc2_Num[16];
          }
        }
        else    
        {
          sc2_Buff[6]|=sc2_Num[ten_bit];
          if(!bl)
          {
            sc2_Buff[8]|=sc2_Num[16];
          }
        }
      }
      else
      {
        sc2_Buff[8]|=sc2_Num[hun_bit];
        sc2_Buff[6]|=sc2_Num[ten_bit];
        if(!bl)
        {
          sc2_Buff[10]|=sc2_Num[16];
        }
      }
    }
    else
    {
      sc2_Buff[10]|=sc2_Num[tho_bit];
      sc2_Buff[8]|=sc2_Num[hun_bit];
      sc2_Buff[6]|=sc2_Num[ten_bit];
    }
  //}
}
//0- ������ 1-�޹� 2-���� 3-��ѹ 4-���� 5-���� 
void sc2_error_state(uint8 state)
{
  sc2_Buff[2]&=0xF0;
  sc2_Buff[13]&=0xC0;
  
  if(sc2_time%2)
  {
    switch(state)
    {
    case 1:
     sc2_Buff[2]|=0x03;
      break;
    case 2:
      sc2_Buff[2]|=0x0C;
      break;
    case 3:
      sc2_Buff[13]|=0x03;
     break;
    case 4:
      sc2_Buff[13]|=0x0C;
      break;
    case 5:
      sc2_Buff[13]|=0x30;
      break;
    }
  }
}
void sc2_power_lvl(uint8 power_lvl)
{
  sc2_Buff[5] &=0x00;
  sc2_Buff[7] &=0x00;
  sc2_Buff[9] &=0x00;
  sc2_Buff[11] &=0x0F;
  
  switch(power_lvl)
  {
    case 1:
        sc2_Buff[9] |=0x10;
        sc2_Buff[11] |= 0x60;
    break;
    case 2:   
        sc2_Buff[9] |=0x70;
        sc2_Buff[11] |= 0x60;
    break;
    case 3:
        sc2_Buff[9] |=0x76;
        sc2_Buff[11] |= 0x60;
    break;
    case 4:
        sc2_Buff[7] |=0x01;
        sc2_Buff[9] |=0xF6;
        sc2_Buff[11] |= 0x60;
    break;
    case 5:
        sc2_Buff[7] |=0x07;
        sc2_Buff[9] |=0xF6;
        sc2_Buff[11] |= 0xF0;
    break;
    case 6:    
        sc2_Buff[7] |=0x3F;
        sc2_Buff[9] |=0xF6;
        sc2_Buff[11] |= 0xF0;
    break;
    case 7:
        sc2_Buff[5] |=0xE0;
        sc2_Buff[7] |=0xFF;
        sc2_Buff[9] |=0xFF;
        sc2_Buff[11] |= 0xF0;
    break;
    case 8:
        sc2_Buff[5] |=0xFF;
        sc2_Buff[7] |=0xFF;
        sc2_Buff[9] |=0xFF;
        sc2_Buff[11] |= 0xF0;
    break;
  }
}
void sc2_switch_num(uint8 switch_num)
{
  sc2_Buff[3]&=0x00;
  sc2_Buff[11]|=0x0F;
  switch(switch_num)
  {
    case 1:
      sc2_Buff[3]|=0x01;
      //sc2_Buff[11]|=0x00;
    break;
    case 2:
      sc2_Buff[3]|=0x02;
      //sc2_Buff[11]|=0x01;
    break;
    case 3:
      sc2_Buff[3]|=0x04;
      //sc2_Buff[11]|=0x01;
    break;
    case 4:
      sc2_Buff[3]|=0x08;
      //sc2_Buff[11]|=0x03;
    break;
    case 5:
      sc2_Buff[3]|=0x10;
     // sc2_Buff[11]|=0x03;
    break;
    case 6:
      sc2_Buff[3]|=0x20;
     // sc2_Buff[11]|=0x07;
    break;
    case 7:
      sc2_Buff[3]|=0x40;
      sc2_Buff[11]|=0x07;
    break;
    case 8:
      sc2_Buff[3]|=0x80;
     // sc2_Buff[11]|=0x0F;
    break;
  }
}
void sc2_operation()
{
  sc2_Buff[1] &=0x7F;
  sc2_Buff[13] &=0x3F;
  sc2_Buff[15] &=0x00;
  
  if(FAN_TEST)
  {
    sc2_Buff[1] |=0x80;
    sc2_Buff[15] |=0x01;
    switch(sc2_quickly_time%3)
    {
    case 0:
      sc2_Buff[13] |=0x40;
      sc2_Buff[15] |=0x24;
      break;
    case 1:
      sc2_Buff[15] |=0x92;
      break;
    case 2:
      sc2_Buff[13] |=0x80;
      sc2_Buff[15] |=0x48;
      break;
    }
  }
  else
  {
    if(sc2_time%2)
    {
        sc2_Buff[13] |=0x80;
        sc2_Buff[15] |=0x49;
        //sc2_Buff[1] |=0x70;
        //sc2_Buff[13] |=0xC0;
        //sc2_Buff[15] |=0xFF;
    }
  }
}
void set2_bignum_kw(bool state)
{
  sc2_Buff[12] &=0x7F;
  if(state)
  {
    sc2_Buff[12] |=0x80;
  }
}
//true �¶ȣ�flase ���ٶȵ� �������¶�
void sc2_small_state(bool state,int16 temperature)
{
  uint16 temp =0;
  sc2_Buff[4] &= 0x7F;
  sc2_Buff[6] &= 0x7F;
  sc2_Buff[8] &= 0x7F;
  sc2_Buff[2] &= 0xCF;
  
  if(state)
  {
    if(temperature<0)
    {
        if(temperature<-999)
        {
          temperature = -999;
        }
        temperature = sc2_absolute(temperature);
        sc2_small_num(temperature/1000%10,temperature/100%10,temperature/10%10,temperature%10,false,false);
    }
    else
    {
        sc2_small_num(temperature/1000%10,temperature/100%10,temperature/10%10,temperature%10,true,false);
    }
    sc2_Buff[6] |= 0x80;
    sc2_Buff[2] |= 0x30;
  }
  else
  {
    temp = sc2_kws/36;
    if(temp>9999)
    {
      temp = temp/100;
      if(temp>9999)
      {
        temp = 9999;
      }
      sc2_small_num(temp/1000%10,temp/100%10,temp/10%10,temp%10,true,false);
    }
    else
    {
      sc2_small_num(temp/1000%10,temp/100%10,temp/10%10,temp%10,true,true);
      sc2_Buff[8] |= 0x80;//С����
    }
    sc2_Buff[4] |= 0x80;
    if(sc2_time%2)
    {
      sc2_Buff[2] |= 0x30;
    }
  }
}
/* 
E1	101 �޹�
E2      102 ���̳���
E3      103 ����̽ͷ��·
E4      104 IGBT����
E5      105 IGBT ��·
E6      106 ����̽ͷ����
E7      107 ����̽ͷ��·
E8      108 ��ѹȱ����
E9      109 ��ѹ��
EA      110 ���̻��߻������𻵻��ߵ����ʵ���С���߹��粻��
Eb      111 
EC      112 �������ϣ�������
Ed	113 ��λ���ؿ�·

*/
void sc2_set_TM1629_Up(uint8 switch_num,uint8 power_lvl,uint8 up,int16 num)
{
  switch(up)
  {
    case 101:
      sc2_big_num(14,1);
      set2_bignum_kw(false);
      sc2_small_state(false,num);
      
      sc2_power_lvl(power_lvl);
      sc2_switch_num(switch_num);
      
      sc2_error_state(1);
      
      sc2_bigNumState =FLUSH;//������״̬
      
      buzz_state = false;
    break;
    case 102:
      sc2_big_num(14,2);
      set2_bignum_kw(false);
      sc2_small_state(false,num);
      
      sc2_power_lvl(power_lvl);
      sc2_switch_num(switch_num);
      
      sc2_error_state(2);
      
      sc2_bigNumState =FLUSH;//������״̬
      
      buzz_state = true;
    break;
    case 103:
      sc2_big_num(14,3);
      set2_bignum_kw(false);
      sc2_small_state(false,num);
      
      sc2_power_lvl(power_lvl);
      sc2_switch_num(switch_num);
      
      sc2_error_state(5);
      
      sc2_bigNumState =FLUSH;//������״̬
      
      buzz_state = true;
    break;
    case 104:
      sc2_big_num(14,4);
      set2_bignum_kw(false);
      sc2_small_state(false,num);
      
      sc2_power_lvl(power_lvl);
      sc2_switch_num(switch_num);
      
      sc2_error_state(2);
      
      sc2_bigNumState =FLUSH;//������״̬
      
      buzz_state = true;
    break;
    case 105:
      sc2_big_num(14,5);
      set2_bignum_kw(false);
      sc2_small_state(false,num);
      
      sc2_power_lvl(power_lvl);
      sc2_switch_num(switch_num);
      
      sc2_error_state(5);
      
      sc2_bigNumState =FLUSH;//������״̬
      
      buzz_state = true;
    break;
    case 106:
      sc2_big_num(14,6);
      set2_bignum_kw(false);
      sc2_small_state(false,num);
      
      sc2_power_lvl(power_lvl);
      sc2_switch_num(switch_num);
      
      sc2_error_state(2);
      
      sc2_bigNumState =FLUSH;//������״̬
      
      buzz_state = true;
    break;
    case 107:
      sc2_big_num(14,7);
      set2_bignum_kw(false);
      sc2_small_state(false,num);
      
      sc2_power_lvl(power_lvl);
      sc2_switch_num(switch_num);
      
      sc2_error_state(5);
      
      sc2_bigNumState =FLUSH;//������״̬
      
      buzz_state = true;
    break;
    case 108:
      sc2_big_num(14,8);
      set2_bignum_kw(false);
      sc2_small_state(false,num);
      
      sc2_power_lvl(power_lvl);
      sc2_switch_num(switch_num);
      
      sc2_error_state(3);
      
      sc2_bigNumState =FLUSH;//������״̬
      
      buzz_state = true;
    break;
    case 109:
      sc2_big_num(14,9);
      set2_bignum_kw(false);
      sc2_small_state(false,num);
      
      sc2_power_lvl(power_lvl);
      sc2_switch_num(switch_num);
      
      sc2_error_state(3);
      
      sc2_bigNumState =FLUSH;//������״̬
      
      buzz_state = true;
    break;
    case 110:
      sc2_big_num(14,10);
      set2_bignum_kw(false);
      sc2_small_state(false,num);
      
      sc2_power_lvl(power_lvl);
      sc2_switch_num(switch_num);
      
      sc2_error_state(4);
      
      sc2_bigNumState =FLUSH;//������״̬
      
      buzz_state = true;
    break;
    case 111:
      sc2_big_num(14,11);
      set2_bignum_kw(false);
      sc2_small_state(false,num);
      
      sc2_power_lvl(power_lvl);
      sc2_switch_num(switch_num);
      
      sc2_error_state(5);
      
      sc2_bigNumState =FLUSH;//������״̬
      
      buzz_state = true;
    break;
    case 112:
      sc2_big_num(14,12);
      set2_bignum_kw(false);
      sc2_small_state(false,num);
      
      sc2_power_lvl(power_lvl);
      sc2_switch_num(switch_num);
      
      sc2_error_state(4);
      
      sc2_bigNumState =FLUSH;//������״̬
      
      buzz_state = true;
    break;
    case 113:
      sc2_big_num(14,13);
      set2_bignum_kw(false);
      sc2_small_state(false,num);
      
      sc2_power_lvl(power_lvl);
      sc2_switch_num(switch_num);
      
      sc2_error_state(5);
      
      sc2_bigNumState =FLUSH;//������״̬
      
      buzz_state = true;
    break;
    default:
      if(up>99)
      {
        up=99;
      } 
      sc2_kw = up;
      sc2_big_num(up/10,up%10);
      set2_bignum_kw(true);
      sc2_small_state(switch_num != 0,num);//num
      
      sc2_power_lvl(power_lvl);
      sc2_switch_num(switch_num);
      
      sc2_error_state(0);
      
      sc2_bigNumState = ON;//������״̬
      
      buzz_state = false;
      break;
  }
}

void sc2_indate(unsigned char p)
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
//���õ�ַ�Զ���1��ʽ
void sc2_display(short state)
{
	DIO_1;//DIO=1;
	CLK_1;//CLK=1;
	STB_1;//STB=1; //ͨѶ��ʼǰͨѶ�˿�ȫ����ʼ��Ϊ��1��
	sc2_indate(writedatamode_z);  //����������������ò��õ�ַ�Զ���1��ʽд��ʾ���ݣ�
        STB_1;//STB=1; //����������������Ҫ����STB���á�1��
	sc2_indate(startaddress); //����ʼ��ַ
	//��ַ�����󣬡�STB������Ϊ��0����������Ҫ��ʾ������
        for(sc2_listIndex=0;sc2_listIndex<datacount;sc2_listIndex++)
        {
  	  sc2_indate(state?sc2_Buff[sc2_listIndex]:0xFF);  //����������ʾ�����ݹ����У���STB��һֱ����Ϊ��0��
        }
	STB_1;//STB=1; //�������е���ʾ���ݺ����16BYTE���󣬽���STB�á�1��
	sc2_indate(disconmode); //����ʾ��������
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
int16 sc2_absolute(int16 value)
{
  if(value <0)
  return -value;
  return value;
}
