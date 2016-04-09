//*****************************
//  author：  hg
//  date:     2016/1/19
//  mcu:      s3f9488
//  project:  电磁炉  8MHZ
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
//003EH -> 1 01011 11   003FH ->1111 1 110
__root const volatile uint8 Smart_Code[]@0x003C={0xFF,0x05,0xAF,0xFE};
__code const uint8 POWER_RATE[]=
{
  #ifdef P_15KW
    0, 2 , 4 , 6, 8 , 10 , 12 , 13 , 15
  #elif defined P_20KW
    0, 2 , 4 , 7 , 9 , 12, 15, 18, 20
  #elif defined P_25KW
    0, 3 , 6 , 9 , 12, 16, 21, 24, 28
  #elif defined P_30KW
    0, 3 , 6 , 9 , 12, 17, 19, 25, 30
  #endif
};

uint4   state_Read1=0;
uint4   state_Read2 =0;

uint8  count_60ms =0;
uint8   count_1s=0;
uint8  count_2s =0;

//volatile uint8 interupt_times =0;
//volatile uint8 show_times =0; //无锅检测中断次数

uint8 while_time =0;
uint8 reset_time = 0;
//当前档位
uint8 rangeNow =0;
//无锅检查时间段
volatile uint8 potTime=0;
//当前显示
volatile uint8 rangeShow =0;
uint8 rangeNext =0;

uint8 buzzTime=0;//BUZZ
volatile uint8 fanTime =0;//FAN

uint16 delay_temp = 0;//温度保护延迟

uint16 tempreture =0;

uint4 first_open =0;//第一次打开标识，

#pragma inline=forced
//--initiation
void sysInit()
{

  WDTCON =0xAA;
  BTCON =0x03;//128 清零
  CLKCON = 0x08;
  SYM =0x00;

  OSCCON =0x04;

  ADCON =0xF4;
  TADATA = 234;
  TACON = 0x0B;
  //TACON = 0x00;

  TBCON =0x77;//0x01   11 0111  /2
  TINTPND =0x00;

  //WTCON  0100 1010
  //WTCON=0x4A;
}

#pragma inline=forced
void ioInit()
{
  /* PO I/O口 不用IO口均设置为输入上拉 默认*/
  P0CONH = 0x55;//01(ADC4) 01(ADC5) 01(ADC6) 01(ADC7)
  P0CONL = 0xC3;//11(ADC8) 00(P0.2/reset) 00(P0.1/XTOUT) 11(P0.0/FAN)
  P0PUR = 0x07;//0 0 0 0 0   0 1 1 1
  /* P1 I/O口 */
  P1CONH = 0x0A;//00(P1.7) 00(P1.6) 10(P1.5/蜂鸣器) 10(P1.4/A316J低电平复位信号)
  P1CONL = 0xFC;//11(P1.3/ADC0) 11(P1.2/ADC1) 11(P1.1/ADC2) 00(关闭)/01(P1.0/PWM)
  P1PUR = 0xF1;//1 1 1 1  0 0 0 1
  /*  P2 I/0口*/
  P2CONH = 0x2A;//00(P2.7) 10(P2.6/LED) 10(P2.5/164 DAT) 10(P2.4/164 CLK)
#ifdef Screen_74HC164
              P2CONL = 0xAA;//10(P2.3/COM1) 10(P2.2/COM2) 10出 00入(P2.1/IIC-SDA) 10(P2.0/IIC-SCL)
#elif defined Screen_TM1629
              P2CONL = 0x8A;//10(P2.3/COM1) 00(P2.2/COM2) 10(P2.1/IIC-SDA) 10(P2.0/IIC-SCL)
#endif

  /*  P3 I/0口*/
  P3CONH = 0x04;//00(P3.6/SCLK) 00(P3.5/SDAT) 01(P3.4/上升) 01(P3.3/下降沿)
  P3CONL = 0x00;//000(P3.2) 000(P3.1) 00(P3.0)

  P3INT =0x00;//00(P3.6/SCLK) 00(P3.5/SDAT) 00(P3.4/上升)10 00(P3.3/下降沿)10
  P3PND = 0x00;//0000 1(P3.6/SCLK) 0(P3.5/SDAT) 0(P3.4/下降沿) 0(P3.3/下降沿) -中断挂起 置0重置
   /*  P4 I/0口 全部挂起*/
}
#pragma inline=forced
void defaultValue()
{
  state_Read1 = 0;
  state_Read2 =0;


  count_60ms =0;
  count_1s=0;
  count_2s =0;

  rangeNow =0;
  rangeNext =0;
  rangeShow =0;

  potTime=0;
  fanTime =200;//关闭风扇
  
 // interupt_times=0;
 // show_times =0;
  
  reset_time = 0;
  
  tempreture =0;
  
  first_open =0;
  
  delay_temp = 1000;
}
int main()
{
  di;
  //系统初始化 IO初始化
  sysInit();
  ioInit();

  #ifdef Screen_74HC164
        init_74HC164();
  #elif defined Screen_TM1629
        init_TM1629();
  #endif

  defaultValue();

  ei;
  BUZZ_ON;
  delay(100);

  while(1)
  {
#ifdef DEBUG
    CLEAR_WD;
    SwitchSet();
    rangeShow = rangeNow;
    testPWM(rangeNow);
#else
    //开启就需要检查的
    //三相电
    if((rangeShow == 106 ||rangeShow == 107 || rangeShow == 108 )&& rangeNow ==0)
    {
      rangeShow = rangeNow;
    }
    threeVCheck();
    CLEAR_WD;
    //档位开关
    if(rangeShow == 109 && rangeNow ==0)
    {
      rangeShow = rangeNow;
    }
    switchCheck();
    //锅底温度开路检查
    if(rangeShow == 110 && rangeNow ==0)
    {
      rangeShow = rangeNow;
    }
    if(delay_temp == 0)
    {
      underPotCheckNull();
    }
    CLEAR_WD;
    //线盘温度开路检查
    if(rangeShow == 103 && rangeNow == 0)
    {
      rangeShow = rangeNow;
    }
    if(delay_temp == 0)
    {
      coilCheckNull();
    }
    //igbt温度开路检查
    if(rangeShow == 105 && rangeNow ==0)
    {
      rangeShow = rangeNow;
    }
    if(delay_temp == 0)
    {
      igbtCheckNull();
    }
    //------------------------------开启检查 0档刷新-------------------------------
    CLEAR_WD;

    if((rangeShow<100 || rangeShow ==101))
    {
          //线盘温度检查
          coilCheckTemp();
    }
    if((rangeShow<100 || rangeShow ==101))
    {
          //锅底温度
         underPotNullCheckTemp();
    }
    if((rangeShow<100 ||rangeShow ==101))
    {
          //igbt温度
        igbtCheckTemp();
    }
    CLEAR_WD;
    
    if(rangeShow<100 || rangeShow ==101)
    {
        //igbt驱动 重点检查，如果问题。机器不能启动,在检锅下也检查
        igbtDriver();
        if(rangeShow == 112 && rangeNow !=0)
        {
          reset_time =0;
        }
    }
        //是否跳转到无锅
    if(rangeShow<100)
    {
        if(first_open == 1 && getPWMCanTakeNullPot())
        {
          checkPotNull();
        }
       if(rangeShow ==101 && rangeNow !=0)//正常情况转到无锅状态下
       {
          potTime =0;//重置检锅时间
       }
    }

    SwitchSet();

    CLEAR_WD;
    //----------------------------基础检查无错------------------
    if(rangeShow == 102 || rangeShow == 111 ||rangeShow == 104)//超温状态或
    {
      if(rangeNow == 0)//0档直接重置
      {
        rangeShow = rangeNow;//结束温度检查
      }
    }
    if(rangeShow == 112)//检查IGBT状态下
    {
      if(rangeNow == 0)
      {
        rangeShow = rangeNow;
      }
    }
    if(rangeShow ==101)//无锅状态下
    {
      if(rangeNow == 0)//0档直接重置
      {
        rangeShow = rangeNow;//结束检锅
      }
      else if(potTime >0 && potTime<6)
      {
        rangeShow = rangeNow;//是否结束检锅
        checkPotNull();
      }
    }
    //----------------------------有无锅检查--------------------
    CLEAR_WD;
    if(rangeShow<100)//正常频道
    {
      if(rangeNow == 0)
      {
        if(fanTime == 0)
        {
          fanTime =1;
        }
        first_open = 1;//开机0档复位标识
      }
      else
      {
        fanTime =0;
      }
      if(first_open == 1)
      {
        fixPWM(rangeNow);
        if(delay_temp>0)
        {
          delay_temp--;
        }
      }
      else
      {
        fixPWM(0);
      }
    }
    else if(rangeShow == 101)//检锅频道
    {
      if(fanTime == 0)
      {
        fanTime =1;
      }
      if( potTime<6)
      {
        testPotPwm();
        
      }
      else
      {
        fixPWM(0);
      }
    }
    else if(rangeShow == 112)//IGBT状态频道
    {
      if(fanTime == 0)
      {
        fanTime =1;
      }
      if(reset_time == 1)
      {
        testPotPwm();
        delay(2);
        rangeShow = rangeNow;
        igbtDriver();
        if(rangeShow == 112)
        {
          fixPWM(0);
        }
      }
      else
      {
        fixPWM(0);
      }
    }
    else{
      if(fanTime == 0)
      {
        fanTime =1;
      }
      fixPWM(0);
    }
    /*-------------------------运转------------------------*/
#endif
    CLEAR_WD;
    ViewSet();
    CLEAR_WD;
    
    reset_time++;
    if(reset_time >=20)
    {
       reset_time =0;        
    } 
    while_time++;
    if(while_time>=200)
    {
        while_time =0;
        tempreture = getTemperatureByAnum(6);  //捡个便宜
    }
  }
}
//三相电
#pragma inline=forced
void threeVCheck()
{
  state_Read1 = get_03ADC();
    if(state_Read1 == 1)
    {
      //缺相
      rangeShow =108;
    }
    else if(state_Read1 == 2)
    {
      //低压
      rangeShow = 106;
    }
    else if(state_Read1 == 3)
    {
      //高压
      rangeShow =107;
    }
}
//档位开关是否开路
#pragma inline=forced
void switchCheck()
{
    state_Read1 = get_05ADC();
    if(state_Read1 == 1)
    {
        rangeShow = 109;
    }
}
//锅底开路
#pragma inline=forced
void underPotCheckNull()
{
   state_Read1 = get_06ADC();
    if(state_Read1 == 2)
    {//开路
        rangeShow = 110;
    }
}
//锅底温度
#pragma inline=forced
void underPotNullCheckTemp()
{
  state_Read1 = get_06ADC();
  if(state_Read1 ==1)
  {//高温
       rangeShow = 111;
  }
}
//线盘温度
#pragma inline=forced
void coilCheckTemp()
{
    state_Read1 = get_07ADC();
    if(state_Read1 ==1)
    {
      //温高
       rangeShow = 102;
    }
}
//线盘开路
#pragma inline=forced
void coilCheckNull()
{
  state_Read1 = get_07ADC();
  if(state_Read1 == 2)
  {
      //开路
      rangeShow = 103;
  }
}
//IGBT
#pragma inline=forced
void igbtCheckTemp()
{
     state_Read1 = get_04ADC();
     state_Read2 = get_11ADC();
     if((state_Read1 == 2) || (state_Read2 ==2))//有一个IGBT高温
     {
         rangeShow = 104;
     }
}
//igbt开路检查
#pragma inline=forced
void igbtCheckNull()
{
     state_Read1 = get_04ADC();
     state_Read2 = get_11ADC();
     if((state_Read1 == 1) || (state_Read2 == 1))//有一个IBGT开路
     {
        rangeShow = 105;
     }
}
//igbt驱动
#pragma inline=forced
void igbtDriver()
{
    if(!Test_Bit(P3,3))//|| !Test_Bit(P3,4))//IGBT驱动问题  输出互感器|| (show_times >=300
    {
       rangeShow = 112;
    }
}

//检测是否无锅
#pragma inline=forced
void checkPotNull()
{
    state_Read1 = get_13ADC();
    if(state_Read1 == 1)// && show_times !=0)
    {
        rangeShow = 101;
    }
}

#pragma inline=forced
void SwitchSet()
{
    rangeNext = getSwitchByAnum();
    if(rangeNext != rangeNow && rangeNext != 9 )
    {
      rangeNow = rangeNext;
      BUZZ_ON;
    }
    else
    {
      rangeNext = rangeNow;
    }
}
#ifdef Screen_74HC164
void ViewSet()
{
    if(rangeShow < 100)
    {
      setNum_74HC164(rangNow);
    }
    else
    {
       setNum_74HC164(rangeShow);
    }
    whileUpdate_74HC164();
}
#elif defined Screen_TM1629
void ViewSet()
{
     if(rangeShow<100)
     {
        set_TM1629_Up(POWER_RATE[rangeNow]);
     }
     else
     {
        set_TM1629_Up(rangeShow);
     }
     set_TM1629_LeftNum(rangeNow);
     set_TM1629_Leftstring(getPWMRate());
      if(rangeShow<100 && rangeNow>0)//温度模式
      {
        // temperature =getTemperatureByAnum(6);//锅底温度
        //getADCNum(13) 输入互感器电流 AD
        //getADCNum(12) 输出互感器电流大小
        set_TM1629_Down(tempreture,1);//temperature,1);
      }
      else//时间模式
      {
        set_TM1629_Down(0,0);
      }
      whileUpdate_TM1629();
}
#endif
//中断
#pragma vector=0x00
__interrupt void int_9488()
{
   if(TINTPND&0x01)//timer A(interal timer)
    {
        TINTPND&=~0x01;
        TAInterupt();
    }
    else if(P3PND&0x08)//int3(pulse input)
    {
        P3PND&=~0x08;
    }
    else if(P3PND&0x01)//int0(active pulse input)
    {
        P3PND&=~0x01;
        P33Interupt();
    }
    else if(P3PND&0x02)//int1(reactive pulse input)
    {
        P3PND&=~0x02;
        P34Interupt();
    }
    else if(P3PND&0x04)//int2(powercut input)
    {
        P3PND&=~0x04;
    }
    else if(UARTPND&0x02)//receive
    {
        UARTPND&=~0x02;
    }
    else if(UARTPND&0x01)//transfer
    {
        UARTPND&=~0x01;
    }
    else if(WTCON&0x01)
    {
        WTCON&=~0x01;
    }
    else if(TINTPND&0x04)//timer B(interal timer)
    {
        TINTPND&=~0x04;
    }
}
#pragma inline=forced
//A定时器
void TAInterupt()
{
    CLEAR_WD;
    count_60ms++;
    
    //show_times = interupt_times;
    //interupt_times = 0;
          
    if(BUZZ_Test)
    {
      buzzTime++;
      if(buzzTime >=4)
      {
        buzzTime = 0;
        BUZZ_OFF;
      }
      else
      {
        BUZZ_ON;
      }
    }
    if(count_60ms == 2)//60ms
    {
      count_60ms=0;
      count_2s++;
      count_1s++;
      
      if(potTime < 40)
      {
        potTime++;
        if(potTime == 40)
        {
          potTime =0;
        }
      }
      
      if(count_2s == 34)//2s
      {
        count_2s =0;
      
        if(fanTime>=1 && fanTime<40)
        {
          fanTime++;
        }
    
        if(fanTime <30)
        {
          FAN_ON;
        }
        else
        {
          FAN_OFF;
        }
        CLEAR_WD;
      }
      if(count_1s == 17)
      {
        count_1s=0;

        Com_Bit(P2,6);
        //INTERUPT 更新区域
        #ifdef Screen_74HC164
              interuptUpdate_74HC164();
        #elif defined Screen_TM1629
              interuptUpdate_TM1629();
        #endif
              
      }
    }
    CLEAR_WD;
}
#pragma inline=forced
void P33Interupt()
{
  //if(reset_time == 0)
  //{
  //  reset_time =1;//开始驱动检测标志
  //}
}
#pragma inline=forced
void P34Interupt()
{
  //if(rangeNow != 0)
  //{
  //  fixPWM(0);//+
  //}
  //if(interupt_times <200)
  //{
  //    interupt_times++;
  //}
  //CLEAR_WD;
}
