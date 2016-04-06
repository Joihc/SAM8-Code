#ifndef _MAIN_H_
#define _MAIN_H_

#include "config.h"

#ifndef _MAIN_C_

#endif

void sysInit();
void ioInit();
void defaultValue();
//三相电
void threeVCheck();
//档位开关是否开路
void switchCheck();
//锅底开路
void underPotCheckNull();
//锅底温度
void underPotNullCheckTemp();
//线盘温度
void coilCheckTemp();
//线盘开路
void coilCheckNull();
//IGBT
void igbtCheckTemp();
//igbt开路检查
void igbtCheckNull();
//igbt驱动
void igbtDriver();
//检测是否无锅
void checkPotNull();
//万劫不复函数
void noWayToDrive();
void SwitchSet();
void ViewSet();

 void TAInterupt();
 void P33Interupt();
 void P34Interupt();
__interrupt void int_9488();

#endif
