#include  "stm8s.h"
#include  "AD.h"
#define  AD_Point 64
unsigned char AD_count = 0;
unsigned char AD_Flag = 0;
//***********************************************
//***************结构体-------AD*****************
//***********************************************
struct AD UI[2];
//***********************************************
//***************结构体-------BD(标定)***********
//***********************************************
struct BD BD_UI[2];

#include  "main.h"
#include  <math.h>
unsigned int  UI_Value;
unsigned int DYBB_Val,DLBB_Val;
unsigned char AD_Del_Flag = 1;
void  AD_On(unsigned char Channel);
void  AD_Scan(void);
void  AD_Filter(unsigned char i);
unsigned int  Maopao (unsigned char Num,unsigned int *p_ad);
void  Calibration_LowVoltage(void);
void  Calibration_LowCurrent(void);
ulong  ADC_Current[2];
ulong  test;
extern  uchar DYBB[4];
extern  uchar DLBB[4];
extern  uchar CSM_AC3_Function_Flag;
struct BD BD_UI[2]={{4096,514,0},{4096,514,0}};
unsigned int   AD_Judge(unsigned char Channel);
void  AD_Scan(void)
{
    unsigned char i;  
    if (AD_Flag == 1)
    {
                   
   //     AD_Filter(0);                    
        AD_Filter(1);
        AD_Del_Flag = 1; //抛弃第一个AD采样值
        AD_Flag = 0;
    }
}
void  AD_Filter(unsigned char i)
{
    unsigned char j;
	
    test=0;
            
    DYBB_Val = DYBB[3] * 1000 + DYBB[2] * 100 + DYBB[1] * 10 + DYBB[0];
   // DLBB_Val = DLBB[3] * 1000 + DLBB[2] * 100 + DLBB[1] * 10 + DLBB[0];
        for (j=0;j<64;j++)
        {
            UI[i].Val_Init[j] = UI[i].Val_Init[j] - BD_UI[i].B;		//去基准值
            test += ((unsigned long)UI[i].Val_Init[j]) * ((unsigned long)UI[i].Val_Init[j]) ;	//平方和
        }
    //    if (CSM_AC3_Function_Flag == 1)   test[UI[i].Nf1] = test[UI[i].Nf1] * 475 / 64;						//475  5A;230 1A
        test = test * 730 / 64; //1A
        UI[i].Filter1[UI[i].Nf1] = sqrt(test);
        UI[i].Nf1++; 
        
        if (UI[i].Nf1 >= 5)                   //一次滤波   5点取中间值
        {
            UI[i].Filter2[UI[i].Nf2] = Maopao(5,UI[i].Filter1);
            UI[i].Nf1 = 0;
            UI[i].Nf2++;
        }
        if (UI[i].Nf2 >= 10)                    //二次滤波  10点  平均
        {
            BD_UI[i].Value = 0;
            for (j = 0;j < 10;j++)       BD_UI[i].Value += UI[i].Filter2[j];
            ADC_Current[i] = ((BD_UI[i].K * ((unsigned long)BD_UI[i].Value) / 10) >> 12) * DYBB_Val;	
            UI[i].Nf2=0;
        }
    
}
void  AD_Convert(void)
{
    if (AD_Flag == 0) 
    {
  //      UI[0].Val_Init[AD_count] = AD_Judge(0); //电流
     //   UI[1].Val_Init[AD_count] = AD_Judge(3); //电压
        AD_Judge(3);  
        if (AD_Del_Flag == 0)
        {
            AD_count++;
        }
        else
        {
            AD_Del_Flag = 0;
        }      
    }
    if (AD_count >= AD_Point)
    {
        AD_Flag = 1;
        AD_count = 0;
    }
}

unsigned int  AD_Judge(unsigned char Channel)
{
 /*   unsigned int  ADC;  
    AD_On(Channel); 
    ADC = ADC1->DRL;
    ADC += (ADC1->DRH << 8);
    return ADC;   
  */
    AD_On(Channel); 
    UI[1].Val_Init[AD_count] = ADC1->DRL;
    UI[1].Val_Init[AD_count] += (ADC1->DRH << 8); 
}
void  AD_On(unsigned char Channel)
{
    ADC1->CSR = Channel;  
    ADC1->CR1 |= 0x01; //AD开关打开
    while ((ADC1->CSR & 0x80) != 0x80); //等待转换完成
    ADC1->CSR &= 0x7f;                  //清除转换结束标志
}
/**********************************************************/
/*************************冒泡函数*************************/
/**********************************************************/

unsigned int  Maopao (unsigned char Num,unsigned int *p_ad)
{
    unsigned char i,j;
    unsigned int  AD1_middle;
    
    for (j=Num-1;j>0;j--)
    {
        for (i=0;i<j;i++)
        {
            if (*(p_ad+i)>*(p_ad+i+1))
            {
                AD1_middle=*(p_ad+i+1);
                *(p_ad+i+1)=*(p_ad+i);
                *(p_ad+i)=AD1_middle;
            }
        }
    }
    return *(p_ad+((Num-1)>>1));
}
void  Calibration_LowVoltage(void)
{
    unsigned char i;  
    for (i=0;i<2;i++)
    {
       if (UI[i].Val_Init[3] < 400)  
       {
          BD_UI[i].B += UI[i].Val_Init[3];
       }
       else
       {
          BD_UI[i].B = UI[i].Val_Init[3];
       }    
    }
}
void  Calibration_Current(void)
{
    unsigned char i;  
    for (i=0;i<2;i++)
    {  
        BD_UI[i].K = ((unsigned long) BD_UI[i].K) * 2000 / ADC_Current[i];
    }
}
void  Calibration_Voltage(void)
{
    unsigned char i;  
    for (i=0;i<2;i++)
    {  
        BD_UI[i].K = ((unsigned long) BD_UI[i].K) * 1000 / ADC_Current[i];
    }
}
void  Write_BD_UI_B(void)
{
    unsigned char i;  
    for (i=0;i<2;i++)
    {
        if (CSM_AC3_Function_Flag == 2)	
        {
            BD_UI[i].B = 500;
        }
        else
        {
            BD_UI[i].B = 581;
        } 
    }
}