#ifndef _MAIN_H_
#define _MAIN_H_

#include "config.h"

#ifndef _MAIN_C_

#endif

void sysInit();
void ioInit();
void defaultValue();
//�����
void threeVCheck();
//��λ�����Ƿ�·
void switchCheck();
//���׿�·
void underPotCheckNull();
//�����¶�
void underPotNullCheckTemp();
//�����¶�
void coilCheckTemp();
//���̿�·
void coilCheckNull();
//IGBT
void igbtCheckTemp();
//igbt��·���
void igbtCheckNull();
//igbt����
void igbtDriver();
//����Ƿ��޹�
void checkPotNull();
//��ٲ�������
void noWayToDrive();
void SwitchSet();
void ViewSet();

 void TAInterupt();
 void P33Interupt();
 void P34Interupt();
__interrupt void int_9488();

#endif
