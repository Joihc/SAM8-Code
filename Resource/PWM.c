#define _PWM_C_

#include "PWM.h"

uint8 pwm =PWM_MIN;//
uint4 pwmMinus = 0;//1表示需要补偿

void initPWM()
{
  pwm=PWM_MIN;
  pwmMinus =0;
}
#pragma inline=forced
void closePWM()
{
  AJ_OFF;
  P1CONL = 0xFC;
}
#pragma inline=forced
void openPWM()
{
  P1CONL = 0xFD;
  AJ_ON;
}

void fixPWM(uint8 index)
{
    uint16 outCurrent = getADCNumByNum(12);//输出互感器
    uint16 inCurrent = getADCNumByNum(13);//输入互感器
    uint16 p=0;
    di;
    switch(index)
    {
      case 0:
        pwm = PWM_MIN;//30hz
        closePWM();
        ei;
      return;
          case 1:
            p = PWM1;
      break;
          case 2:
            p = PWM2;
      break;
          case 3:
            p = PWM3;
      break;
          case 4:
            p = PWM4;
      break;
          case 5:
            p = PWM5;
      break;
          case 6:
            p = PWM6;
      break;
          case 7:
            p = PWM7;
      break;
          case 8:
            p = PWM8;
      break;
    }
    //if((inCurrent < RETURN_PWM) && (pwm > PWM_RETURN))
    //{
    //  --pwm;
    //}
    //else
    //{
    if(pwmMinus == 1)
    {
      --pwm;
      pwmMinus =0;
      P3INT = 0x08;//开启中断
    }
    else
    {
      if(inCurrent !=0 && (outCurrent*4)/(inCurrent/10)>=32)
      {
        --pwm;
      }
      else
      {
        if(outCurrent<(p-2))
        {
          ++pwm;
        }
        else if(outCurrent>(p+2))
        {
          --pwm;
        }
      }
    }
    //}
    pwm =Clamp(pwm,PWM_MIN,PWM_MAX);

    TBDATAH = pwm;//pwm;//pwm/2 -1;
    TBDATAL = pwm;//pwm;//pwm/2 -1;
    openPWM();
    ei;
}

void PWMPLUS()
{
  if(P1CONL == 0xFD)//在开启状态
  {
    pwmMinus =1;
    P3INT = 0x00;//关闭中断
  }
}
void testPotPwm()
{
    di;
    //pwm++;
    pwm =Clamp(PWM_POT,PWM_MIN,PWM_MAX);//Clamp(pwm,100,200);
    TBDATAH = pwm;//pwm;//pwm/2 -1;
    TBDATAL =pwm;//pwm;//pwm/2 -1;
    openPWM();
    ei;
}

void testPWM(uint8 index)
{
  di;
  if(index == 1)
  {
    pwm++;
    pwm =Clamp(pwm,PWM_MIN,PWM_MAX);
    TBDATAH = pwm;//pwm;//pwm/2 -1;
    TBDATAL =pwm;//pwm;//pwm/2 -1;
    openPWM();
  }
  else if(index == 2)
  {
    pwm--;
    pwm =Clamp(pwm,PWM_MIN,PWM_MAX);
    TBDATAH = pwm;//pwm;//pwm/2 -1;
    TBDATAL =pwm;//pwm;//pwm/2 -1;
    openPWM();
  }
  else
  {
    closePWM();
  }
  ei;
}
uint4 getPWMRate()
{
  uint16 range  = getADCNumByNum(12);
  if(range>PWM8)
  {
    return 8;
  }
  else if(range>PWM7)
  {
    return 7;
  }
  else if(range>PWM6)
  {
    return 6;
  }
  else if(range>PWM5)
  {
    return 5;
  }
  else if(range>PWM4)
  {
    return 4;
  }
  else if(range>PWM3)
  {
    return 3;
  }
  else if(range>PWM2)
  {
    return 2;
  }
  else if(range>PWM1-5)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}
#pragma inline=forced
uint8 Clamp(uint8 num,uint8 mix,uint8 max)
{
  if(num<mix)
  {
    return mix;
  }
  else if(num >max)
  {
    return max;
  }
  return num;
}
