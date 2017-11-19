#ifndef _TM1629_2_H_
#define _TM1629_2_H_

#include "config.h"

#ifndef _TM1629_2_C_

#endif

enum LED_STATE
{
  OFF,
  ON,
  FLUSH
};


#define STB_0 (Clr_Bit(P2,5))
#define STB_1 (Set_Bit(P2,5))
#define CLK_0 (Clr_Bit(P2,4))
#define CLK_1 (Set_Bit(P2,4))
#define DIO_0 (Clr_Bit(P2,3))
#define DIO_1 (Set_Bit(P2,3))

#define dissetmode 0x03
#define writedatamode_z 0x40 //���õ�ַ��һ��ʽд
#define startaddress 0xc0 //��ʼ��ַ
#define disconmode 0x8C //��ʾ���� ���� 0x80-0x8F��8 9 A B C D E F��
#define datacount 16 //���õ�ַ�Զ���һ��ʽ�������ݵĸ�

void sc2_init();
void sc2_whileUpdate();
void sc2_quicklyUpdate();
void sc2_interuptUpdate();

void sc2_set_TM1629_Up(uint8 switch_num,uint8 power_lvl,uint8 up,int16 num);


void sc2_indate(unsigned char p);
void sc2_display(short state);
//void display2(unsigned char adress,unsigned char disa);
int16 sc2_absolute();

#endif
