#ifndef _ADC_H_
#define _ADC_H_

#include "config.h"
#include "time.h"
#include "AT24C01.h"

#ifndef _ADC_C_
/*
  10λADC�����������ADValue = (ģ����/��׼��ѹ)*1023
  12λADC�����������ADValue = (ģ����/��׼��ѹ)*4095
  17λADC�����������ADValue = (ģ����/��׼��ѹ)*131071
*/
//extern uint16 adc10 = 1023;
/*
  �޹�������
*/

#endif

#define NULL_NUM (1000)//��·����  4.98V
#define FILTER_N (6) //��λ��
#define AREA (5)//����ģ������С

uint4 get_12ADC();
uint4 get_03ADC(uint4 last_index);
uint4 get_05ADC();
uint4 get_04ADC();
uint4 get_11ADC();
uint4 get_07ADC();
uint4 get_06ADC();
uint4 get_13ADC();
uint4 get_13ADCQuickly();

uint16 getADCNumByNum(uint8 IO_P);
uint16 getADCNum(uint8 IO_P);
int16 getTemperatureByAnum(uint8 IO_P);
uint8 getSwitchByAnum();
void writeToAT24C1();

uint8 TestAT24C01();

uint16 getVADCNum();

#endif /* _ADC_H_ */
