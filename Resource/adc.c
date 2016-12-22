#define _ADC_C_

#include "adc.h"
//***************************热敏电阻 10输出量 100K NTC*******************************__tiny
__code const uint16   rtTable1[] =
 {
    0x03fb,	// -40.00		3225.0000
    0x03fb,	// -39.00		3026.0000
    0x03fb,	// -38.00		2840.0000
    0x03fb,	// -37.00		2665.0000
    0x03fa,	// -36.00		2501.0000
    0x03fa,	// -35.00		2348.0000
    0x03fa,	// -34.00		2204.0000
    0x03fa,	// -33.00		2070.0000
    0x03f9,	// -32.00		1945.0000
    0x03f9,	// -31.00		1828.0000
    0x03f9,	// -30.00		1718.0000
    0x03f8,	// -29.00		1614.0000
    0x03f8,	// -28.00		1518.0000
    0x03f7,	// -27.00		1428.0000
    0x03f7,	// -26.00		1344.0000
    0x03f6,	// -25.00		1267.0000
    0x03f6,	// -24.00		1194.0000
    0x03f6,	// -23.00		1127.0000
    0x03f5,	// -22.00		1063.0000
    0x03f4,	// -21.00		1004.0000
    0x03f4,	// -20.00		948.8000
    0x03f3,	// -19.00		896.3000
    0x03f3,	// -18.00		847.1000
    0x03f2,	// -17.00		801.0000
    0x03f1,	// -16.00		757.8000
    0x03f0,	// -15.00		717.2000
    0x03f0,	// -14.00		679.0000
    0x03ef,	// -13.00		643.2000
    0x03ee,	// -12.00		609.6000
    0x03ed,	// -11.00		577.9000
    0x03ec,	// -10.00		548.1000
    0x03eb,	// -9.00		519.7000
    0x03ea,	// -8.00		492.8000
    0x03e9,	// -7.00		467.5000
    0x03e8,	// -6.00		443.5000
    0x03e7,	// -5.00		420.9000
    0x03e6,	// -4.00		399.5000
    0x03e4,	// -3.00		379.3000
    0x03e3,	// -2.00		360.2000
    0x03e1,	// -1.00		342.2000
    0x03e0,	// 0.00		325.1000
    0x03de,	// 1.00		309.4000
    0x03dd,	// 2.00		294.5000
    0x03db,	// 3.00		280.4000
    0x03da,	// 4.00		267.0000
    0x03d8,	// 5.00		254.3000
    0x03d6,	// 6.00		242.2000
    0x03d4,	// 7.00		230.7000
    0x03d2,	// 8.00		219.9000
    0x03d0,	// 9.00		209.5000
    0x03ce,	// 10.00		199.7000
    0x03cb,	// 11.00		190.4000
    0x03c9,	// 12.00		181.6000
    0x03c7,	// 13.00		173.2000
    0x03c4,	// 14.00		165.2000
    0x03c1,	// 15.00		157.7000
    0x03bf,	// 16.00		150.5000
    0x03bc,	// 17.00		143.7000
    0x03b9,	// 18.00		137.2000
    0x03b6,	// 19.00		131.0000
    0x03b3,	// 20.00		125.2000
    0x03b0,	// 21.00		119.6000
    0x03ac,	// 22.00		114.3000
    0x03a9,	// 23.00		109.3000
    0x03a5,	// 24.00		104.5000
    0x03a2,	// 25.00		100.0000
    0x039e,	// 26.00		95.6800
    0x039a,	// 27.00		91.5700
    0x0396,	// 28.00		87.6600
    0x0392,	// 29.00		83.9300
    0x038d,	// 30.00		80.3900
    0x0389,	// 31.00		77.0100
    0x0384,	// 32.00		73.7900
    0x0380,	// 33.00		70.7300
    0x037b,	// 34.00		67.8100
    0x0376,	// 35.00		65.0300
    0x0371,	// 36.00		62.3800
    0x036c,	// 37.00		59.8400
    0x0367,	// 38.00		57.4300
    0x0361,	// 39.00		55.1300
    0x035c,	// 40.00		52.9300
    0x0356,	// 41.00		50.8300
    0x0351,	// 42.00		48.8300
    0x034b,	// 43.00		46.9200
    0x0345,	// 44.00		45.0900
    0x033f,	// 45.00		43.3400
    0x0339,	// 46.00		41.6700
    0x0332,	// 47.00		40.0800
    0x032c,	// 48.00		38.5500
    0x0325,	// 49.00		37.0900
    0x031f,	// 50.00		35.7000
    0x0318,	// 51.00		34.3600
    0x0311,	// 52.00		33.0800
    0x030a,	// 53.00		31.8600
    0x0303,	// 54.00		30.6800
    0x02fc,	// 55.00		29.5600
    0x02f5,	// 56.00		28.4800
    0x02ed,	// 57.00		27.4500
    0x02e6,	// 58.00		26.4600
    0x02de,	// 59.00		25.5100
    0x02d7,	// 60.00		24.6000
    0x02cf,	// 61.00		23.7300
    0x02c7,	// 62.00		22.8900
    0x02c0,	// 63.00		22.0900
    0x02b8,	// 64.00		21.3200
    0x02b0,	// 65.00		20.5800
    0x02a8,	// 66.00		19.8600
    0x02a0,	// 67.00		19.1800
    0x0298,	// 68.00		18.5200
    0x0290,	// 69.00		17.8900
    0x0288,	// 70.00		17.2900
    0x027f,	// 71.00		16.7000
    0x0277,	// 72.00		16.1400
    0x026f,	// 73.00		15.6000
    0x0267,	// 74.00		15.0800
    0x025e,	// 75.00		14.5800
    0x0256,	// 76.00		14.1000
    0x024e,	// 77.00		13.6400
    0x0245,	// 78.00		13.1900
    0x023d,	// 79.00		12.7600
    0x0235,	// 80.00		12.3500
 };
__code const uint16   rtTable2[] =
{
    0x022c,	// 81.00		11.9500
    0x0224,	// 82.00		11.5600
    0x021c,	// 83.00		11.1900
    0x0213,	// 84.00		10.8300
    0x020b,	// 85.00		10.4900
    0x0203,	// 86.00		10.1500
    0x01fb,	// 87.00		9.8300
    0x01f2,	// 88.00		9.5180
    0x01ea,	// 89.00		9.2170
    0x01e2,	// 90.00		8.9270
    0x01da,	// 91.00		8.6480
    0x01d2,	// 92.00		8.3800
    0x01ca,	// 93.00		8.1210
    0x01c2,	// 94.00		7.8710
    0x01ba,	// 95.00		7.6300
    0x01b3,	// 96.00		7.3980
    0x01ab,	// 97.00		7.1750
    0x01a3,	// 98.00		6.9590
    0x019c,	// 99.00		6.7510
    0x0194,	// 100.00		6.5500
    0x018d,	// 101.00		6.3570
    0x0186,	// 102.00		6.1700
    0x017f,	// 103.00		5.9890
    0x0178,	// 104.00		5.8150
    0x0171,	// 105.00		5.6470
    0x016a,	// 106.00		5.4850
    0x0163,	// 107.00		5.3280
    0x015c,	// 108.00		5.1760
    0x0156,	// 109.00		5.0300
    0x014f,	// 110.00		4.8880
    0x0149,	// 111.00		4.7510
    0x0143,	// 112.00		4.6190
    0x013d,	// 113.00		4.4910
    0x0136,	// 114.00		4.3670
    0x0130,	// 115.00		4.2470
    0x012b,	// 116.00		4.1310
    0x0125,	// 117.00		4.0180
    0x011f,	// 118.00		3.9090
    0x0119,	// 119.00		3.8040
    0x0114,	// 120.00		3.7020
    0x010e,	// 121.00		3.6020
    0x0109,	// 122.00		3.5060
    0x0104,	// 123.00		3.4130
};
      //--------130/5 =[0]-[26]
__code const uint8  rtTable3[] =
{
  0xff,	// 124.00		3.3220
  0xf9,//---0x0f9,	// 125.00		3.2340
  0xf4,	// 126.00		3.1440
  0xef,	// 127.00		3.0570
  0xea,	// 128.00		2.9730
  0xe5,	// 129.00		2.8910
  0xe0,	// 130.00		2.8120
  0xdb,	// 131.00		2.7350
  0xd7,	// 132.00		2.6610
  0xd2,	// 133.00		2.5890
  0xcd,	// 134.00		2.5190
  0xc9,	// 135.00		2.4520
  0xc5,	// 136.00		2.3870
  0xc0,	// 137.00		2.3230
  0xbc,	// 138.00		2.2620
  0xb8,	// 139.00		2.2030
  0xb4,	// 140.00		2.1460
  0xb0,	// 141.00		2.0910
  0xad,	// 142.00		2.0370
  0xa9,	// 143.00		1.9850
  0xa5,	// 144.00		1.9350
  0xa2,	// 145.00		1.8860
  0x9e,	// 146.00		1.8390
  0x9b,	// 147.00		1.7930
  0x98,	// 148.00		1.7490
  0x95,	// 149.00		1.7060
  0x92,//---0x092,	// 150.00		1.6650
  0x8f,	// 151.00		1.6250
  0x8c,	// 152.00		1.5860
  0x89,	// 153.00		1.5480
  0x86,	// 154.00		1.5110
  0x83,	// 155.00		1.4760
  0x80,	// 156.00		1.4420
  0x7e,	// 157.00		1.4080
  0x7b,	// 158.00		1.3760
  0x79,	// 159.00		1.3440
  0x76,	// 160.00		1.3140
  0x74,	// 161.00		1.2840
  0x72,	// 162.00		1.2550
  0x6f,	// 163.00		1.2270
  0x6d,	// 164.00		1.2000
  0x6b,//---0x06b,	// 165.00		1.1730
  0x69,	// 166.00		1.1480
  0x67,	// 167.00		1.1230
  0x65,	// 168.00		1.0980
  0x63,	// 169.00		1.0740
  0x61,	// 170.00		1.0510
  0x5f,	// 171.00		1.0286
  0x5d,	// 172.00		1.0066
  0x5b,	// 173.00		0.9852
  0x59,	// 174.00		0.9643
  0x58,	// 175.00		0.9439
  0x56,	// 176.00		0.9225
  0x54,	// 177.00		0.9016
  0x52,	// 178.00		0.8813
  0x51,	// 179.00		0.8615
  0x4f,	// 180.00		0.8423
  0x4d,	// 181.00		0.8236
  0x4c,	// 182.00		0.8053
  0x4a,	// 183.00		0.7877
  0x49,	// 184.00		0.7705
  0x47,//---0x047,	// 185.00		0.7537
  0x46,	// 186.00		0.7375
  0x44,	// 187.00		0.7216
  0x43,	// 188.00		0.7063
  0x42,	// 189.00		0.6913
  0x40,//---0x040,	// 190.00		0.6768
  0x3f,	// 191.00		0.6627
  0x3e,	// 192.00		0.6489
  0x3d,	// 193.00		0.6356
  0x3b,	// 194.00		0.6226
  0x3a,	// 195.00		0.6100
  0x39,	// 196.00		0.5977
  0x38,	// 197.00		0.5858
  0x37,	// 198.00		0.5742
  0x36,	// 199.00		0.5629
  0x35,	// 200.00		0.5519
  0x34,	// 201.00		0.5412
  0x33,	// 202.00		0.5308
  0x32,	// 203.00		0.5207
  0x31,	// 204.00		0.5108
  0x30,	// 205.00		0.5012
  0x2f,	// 206.00		0.4918
  0x2f,	// 207.00		0.4827
  0x2e,	// 208.00		0.4738
  0x2d,	// 209.00		0.4651
  0x2c,	// 210.00		0.4566
  0x2b,	// 211.00		0.4484
  0x2b,	// 212.00		0.4403
  0x2a,	// 213.00		0.4324
  0x29,	// 214.00		0.4247
  0x28,	// 215.00		0.4172
  0x28,	// 216.00		0.4099
  0x27,	// 217.00		0.4027
  0x26,	// 218.00		0.3956
  0x26,	// 219.00		0.3887
  0x25,	// 220.00		0.3820
  0x25,	// 221.00		0.3754
  0x24,	// 222.00		0.3689
  0x23,	// 223.00		0.3626
  0x23,	// 224.00		0.3563
  0x22,	// 225.00		0.3502
  0x22,	// 226.00		0.3438
  0x21,	// 227.00		0.3374
  0x20,	// 228.00		0.3312
  0x20,	// 229.00		0.3252
  0x1f,	// 230.00		0.3192
  0x1f,	// 231.00		0.3134
  0x1e,	// 232.00		0.3077
  0x1d,	// 233.00		0.3021
  0x1d,	// 234.00		0.2966
  0x1c,	// 235.00		0.2913
  0x1c,	// 236.00		0.2860
  0x1b,	// 237.00		0.2809
  0x1b,	// 238.00		0.2759
  0x1a,	// 239.00		0.2710
  0x1a,	// 240.00		0.2662
  0x1a,	// 241.00		0.2615
  0x19,	// 242.00		0.2569
  0x19,	// 243.00		0.2524
  0x18,	// 244.00		0.2480
  0x18,	// 245.00		0.2437
  0x17,	// 246.00		0.2395
  0x17,	// 247.00		0.2354
  0x17,	// 248.00		0.2314
  0x16,	// 249.00		0.2275
  0x16,	// 250.00		0.2236
  0x16,	// 251.00		0.2199
  0x15,	// 252.00		0.2162
  0x15,	// 253.00		0.2126
  0x14,	// 254.00		0.2090
  0x14,	// 255.00		0.2056
  0x14,	// 256.00		0.2022
  0x13,	// 257.00		0.1989
  0x13,	// 258.00		0.1957
  0x13,	// 259.00		0.1925
  0x13,	// 260.00		0.1894
  0x12,	// 261.00		0.1864
  0x12,	// 262.00		0.1834
  0x12,	// 263.00		0.1805
  0x11,	// 264.00		0.1776
  0x11,	// 265.00		0.1748
  0x11,	// 266.00		0.1720
  0x11,	// 267.00		0.1694
  0x10,	// 268.00		0.1667
  0x10,	// 269.00		0.1641
  0x10,	// 270.00		0.1616
  0x10,	// 271.00		0.1591
};
__code const short  rtTable4[] =
{
  0xf,	        // 272.00		0.1566
  0xf,	        // 273.00		0.1542
  0xf,	        // 274.00		0.1519
  0xf,	// 275.00		0.1496
  0xe,	        // 276.00		0.1473
  0xe,	        // 277.00		0.1451
  0xe,	        // 278.00		0.1429
  0xe,	        // 279.00		0.1407
  0xd,	// 280.00		0.1386
  0xd,	        // 281.00		0.1365
  0xd,	        // 282.00		0.1345
  0xd,	        // 283.00		0.1324
  0xd,	        // 284.00		0.1304
  0xc,	// 285.00		0.1285
  0xc,	        // 286.00		0.1266
  0xc,	        // 287.00		0.1247
  0xc,	        // 288.00		0.1228
  0xc,	        // 289.00		0.1210
  0xc,	// 290.00		0.1192
  0xb,	        // 291.00		0.1174
  0xb,	        // 292.00		0.1156
  0xb,	        // 293.00		0.1139
  0xb,	        // 294.00		0.1122
  0xb,	// 295.00		0.1105
  0xb,	        // 296.00		0.1088
  0xa,	        // 297.00		0.1071
  0xa,	        // 298.00		0.1055
  0xa,	        // 299.00		0.1039
  0xa	// 300.00		0.1023
};

//uint16 buf[FILTER_N] = {600,600,600};

/// P0.3/ADC8/三项输入电压互感 10K接地 10K接入 380：3变压 3是电压过高 2是低。1缺相 0表示正常
uint4 get_03ADC(uint4 last_index)
{
  uint16 three = getADCNumByNum(3);// 565 679  310-456V  942     250-500-760
  if(three < 250-AREA)
  {
    return 1;
  }
  else if(three >250+AREA && three <500-AREA)
  {
    return 2;
  }
  else if(three >500+AREA && three <760-AREA)
  {
    return 0;
  }
  else if(three >760+AREA)
  {
    return 3;
  }
  
  if(three>=250-AREA && three <= 250+AREA)
  {
    return last_index==1?1:2;
  }
  else if(three >=500-AREA && three <=500+AREA)
  {
    if(last_index ==2 || last_index ==1)
    {
      return 2;
    }
    else if(last_index ==0 ||last_index ==3)
    {
      return 0;
    }
  }
  else if(three >=760-AREA && three <= 760+AREA)
  {
    return last_index == 3 ? 3:0;
  }
  if(VLDCON & 0x40)
  {
    return 1;
  }
  return last_index;
  //}
  //else if(three > 736)//736.56
  //{
  //  return 3;
  //}
  //return 0;
}
///* P0.5/ADC6/档位 10K 5V*/  开路 >4.5V 1   0正常
uint4 get_05ADC()
{
  uint16 Switch = getADCNumByNum(5);//
  if(Switch > 800)
  {
    return 1;
  }
  return 0;
}
// P0.4/ADC7/IGBT1温度10K 5V  0正常1表示开路  2.3温高  2表示超温   65C
uint4 get_04ADC()
{
  uint16  IGBTTemp = getADCNumByNum(4);
  if(IGBTTemp > NULL_NUM)
  {
    return 1;
  }
  else if(IGBTTemp <=0x025e)//75
  {
    return 2;
  }
  else if(IGBTTemp <=0x03a2)//0x03a2 25
  {
    return 3;
  }
  return 0;
}
// P1.1/ADC2/IGBT1温度10K 5V   0正常1表示开路  2.3温高  2表示超温  65C
uint4 get_11ADC()
{
  uint16  IGBTTemp = getADCNumByNum(11);
  if(IGBTTemp > NULL_NUM)
  {
    return 1;
  }
  else if(IGBTTemp <=0x025e)//75
  {
    return 2;
  }
  else if(IGBTTemp <=0x03a2)//0x03a2 25
  {
    return 3;
  }
  return 0;
}
//P0.7/ADC4/线盘温度 10K 5V   0正常 1表示超温 2表示开路   120
uint4 get_07ADC()
{
  uint16  StringTemp = getADCNumByNum(7);
  if(StringTemp >NULL_NUM)
  {
    return 2;
  }
  else if(StringTemp <=0x0114)
  {
    return 1;
  }
  return 0;
}
/* P0.6/ADC5/锅底温度 10K 5V   0x13*/
uint4 get_06ADC()
{
  uint16  PotTemp = getADCNumByNum(6);
  if(PotTemp >NULL_NUM)
  {
    return 2;
  }
  else if(PotTemp <=0xa)
  {
    return 1;
  }
  return 0;
}

/* 输入互感器 P1.3*/
uint4 get_13ADC()
{
  uint16  PotTemp = getADCNumByNum(13);
  if(PotTemp <10)//0.5V
  {
    return 1;
  }
  return 0;
}
/*
INTERNAL A/D CONVERSION PROCEDURE
1. Analog input must remain between the voltage range of VSS and AVREF.
2. Configure P0.3–P0.7 and P1.0–P1.3 for analog input before A/D conversions. To do this, you have to load the
appropriate value to the P0CONH, P0CONL and P1CONL (for ADC0–ADC8) registers.
3. Before the conversion operation starts, you must first select one of the eight input pins (ADC0–ADC8) by writing
the appropriate value to the ADCON register.
4. When conversion has been completed, (50 clocks have elapsed), the EOC, ADCON.3 flag is set to "1", so that
a check can be made to verify that the conversion was successful.
5. The converted digital value is loaded to the output register, ADDATAH (8-bit) and ADDATAL (2-bit), then the ADC
module enters an idle state.
6. The digital conversion result can now be read from the ADDATAH and ADDATAL register.
*/
//#pragma inline=forced
uint16 getADCNum(uint8 IO_P)
{
  uint16 AD_Dat =0;
  uint16 Tmp=0;
  di;
  switch(IO_P)
  {
  case 13:/* P1.3/ADC0/输入互感器 10K电阻串*/
      //P1CONL |= 0xc0;               //0x1100 0000
      ADCON = 0x05;                 //0x0000 0 01 1  fxx/8 step3
      break;
  case 12:/* P1.2/ADC1/5K滑动变阻器 串100K电阻，测试电压 100欧姆接地电压*/
      //P1CONL |= 0x30;               //0x0011 0000
      ADCON = 0x15;                 //0x0001 0 01 1  fxx/8 step3
    break;
  case 11:/* P1.1/ADC2/IBGT2温度传感器10K 5V*/
      //P1CONL |= 0x0c;               //0x0000 1100
      ADCON = 0x25;                 //0x0010 0 01 1  fxx/8 step3
    break;
    case 7:/* P0.7/ADC4/线盘温度 10K 5V*/
      //P0CONH |= 0x40;               //0x0100 0000
      ADCON = 0x45;                 //0x0100 0 01 1  fxx/8 step3
    break;
    case 6:/* P0.6/ADC5/锅底温度 10K 5V*/
      //P0CONH |= 0x10;               //0x0001 0000
      ADCON = 0x55;                 //0x0101 0 01 1  fxx/8 step3
    break;
    case 5:/* P0.5/ADC6/档位 10K 5V*/
      //P0CONH |= 0x04;               //0x0000 0100
      ADCON = 0x65;                 //0x0110 0 01 1  fxx/8 step3
    break;
    case 4:/* P0.4/ADC7/IGBT1温度10K 5V*/
      //P0CONH |= 0x01;               //0x0000 0001
      ADCON = 0x75;                 //0x0111 0 01 1  fxx/8 step3
    break;
    case 3:/* P0.3/ADC8/三项输入电压互感 10K接地 10K接入 380：3变压*/
      //P0CONL |= 0xc0;               //0x1100 0000
      ADCON = 0x85;                 //0x1000 0 01 1  fxx/8 step3
    break;
  }
  
  while((ADCON & (1<<3)) == 0)  //转换结束  step4
  AD_Dat = ADDATAH;             //高8节
  Tmp = ADDATAL;                //低8节 
 
  AD_Dat <<=2;
  AD_Dat |=Tmp;
  ADCON = 0xF0;
  ei;
  return AD_Dat;
}

uint16 getADCNumByNum(uint8 IO_P)
{
  int4 i;
  //uint16 buf[FILTER_N];
  uint16 filter_temp = 0;


  for(i = 0; i < FILTER_N; i++) {
      filter_temp+= getADCNum(IO_P);
      delay(2);
      CLEAR_WD;
  }
  /*
  for(j = 0; j < FILTER_N - 1; j++) {
    for(i = 0; i < FILTER_N - 1 - j; i++) {
      if(buf[i] > buf[i + 1]) {
        filter_temp = buf[i];
        buf[i] = buf[i + 1];
        buf[i + 1] = filter_temp;
      }
    }
  }
  */

  filter_temp= filter_temp/FILTER_N;
  return filter_temp;
}

//温度转换
int16 getTemperatureByAnum(uint8 IO_P)
{
  uint8 i = 0;

  uint16 Anum = getADCNumByNum(IO_P);

  //-40 -80
  if(Anum > 556)
  {
    while(i <=120 && Anum < rtTable1[i])
    {
      i++;
    }
    return i-40;
  }
  // 80 - 123
  else if(Anum > 255)
  {
    while(i<=43 && Anum < rtTable2[i])
    {
      i++;
    }
    return i+80;
  }
  // 124 -271
  else if(Anum > 15)
  {
    while(i<=147 && Anum < rtTable3[i] )
    {
      i++;
    }
    return i+124;
  }
  else
  {
    // 272 -300
    while(i<= 28 && Anum < rtTable4[i])
    {
      i++;
    }
    return i+272;
  }
}
//档位开关档数
/*
  0 - 0.4545V         93
  1 - 0.833V          0.6 ->122.76
  2 - 1.1538V         1.0 ->204.6
  3 - 1.4285V         1.3 ->265.98
  4 - 1.875V          1.6 ->327.36
  5 - 2.222V          2.0 ->409.2
  6 - 2.5V            2.3 ->470.58
  7 - 2.727V          2.6 ->531.96
  8 - 3V              2.85->583.11
*/
uint8 getSwitchByAnum()// 96 172 237 294 384 455 512 578 630
{
  uint16 Anum = getADCNum(5);
  if(Anum >90 && Anum <102)
  {
    return 0;
  }
  else if(Anum > 166 && Anum <178)
  {
    return 1;
  }
    else if(Anum > 231 && Anum <243)
  {
    return 2;
  }
    else if(Anum > 288 && Anum <300)
  {
    return 3;
  }
    else if(Anum > 378 && Anum <390)
  {
    return 4;
  }
    else if(Anum > 449 && Anum <461)
  {
    return 5;
  }
    else if(Anum > 506 && Anum <518)
  {
    return 6;
  }
    else if(Anum > 572 && Anum <584)
  {
    return 7;
  }
    else if(Anum > 624 && Anum <636)
  {
    return 8;
  }
  return 9;
}
