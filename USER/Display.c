#include"stm8s.h"
#include "main.h"
#include  "Display.h"
extern  unsigned int  UI_Value;
extern  unsigned char CSM_AC5_Function_Flag;
extern  unsigned char MainMenu2_Flag;
extern  unsigned char CalibrationMenu2_Flag;
unsigned char const LedChooseCode[5] = {0xEF,0xDF,0xBF,0x7F,0xFC};// 数码管位选1-4, 第5位为不显示
unsigned char  const Led_Num[10] = {0xFC,0x84,0xBA,0xAE,0xC6,0x6E,0x7E,0xA4,0xFE,0xEE};
unsigned char  Display_Data[5] = {0xFC,0x84,0xBA,0xAE,0xC6};
unsigned char  Display_Bit = 0;
unsigned long  ADC_Value[4];
unsigned int   Dot_Flag = 0;
unsigned char  LED_Flag = 0;
extern  unsigned long  ADC_Current[2];
uchar DYBB[4] = {1,0,0,0};
uchar DLBB[4] = {1,0,0,0};
extern  unsigned char CSM_AC5_AV_Flag;
extern  unsigned char  CalibrationMenu_Flag;
extern  unsigned char MainMenu_Flag;
extern  uchar Year;  
extern  uint  Date; 
extern  uint  Version; 
void  Display_4Bit(void);
void  Display_Scan(void);
void  Display_MainMenu(void);
void  Display_CalibrationMenu(void);
void  MainMenu_Voltage(unsigned char i);
void  MainMenu_Current(unsigned char i);
void  Dec_TO_Hex(unsigned long Value);
void  MainMenu_UI(void);
void  Display_4Byte(uchar Byte_3,uchar Byte_2,uchar Byte_1,uchar Byte_0,uchar dot);
void  MainMenu_DLBB(void);
void  MainMenu_DYBB(void);
void  Disp_DXBB(unsigned char * DXBB);
void  CalibrationMenu_LowVoltage(void);
void  CalibrationMenu_Voltage(void);
void  CalibrationMenu_1AV1(void);
void  Display_Date(uchar Year,uint  Date);
void  Display_Version(uint Version);
void  Disp_DXBB(unsigned char * DXBB);
void  DOT_Judge(void);
void  Display_Scan(void)
{
    if (CalibrationMenu_Flag == 0)
    {
        Display_MainMenu();
    }
    else
    {
        Display_CalibrationMenu();
    }
}
void  Display_4Bit(void)
{
    GPIOD->ODR |= LedChooseCode[4]; 
    GPIOE->ODR &= 0xDf;
    if (Display_Bit >= 5)
    {
        Display_Bit = 0;
    }
    if (Display_Bit != 4)
    {
        if (Dot_Flag == Display_Bit)       GPIOE->ODR |= 0x20;
        GPIOC->ODR = Display_Data[Display_Bit]; 
        GPIOD->ODR &= LedChooseCode[Display_Bit];
    }
    else  //LED 显示
    {
        if (LED_Flag == 1)    GPIOD->ODR &= 0xF7;
    }
    Display_Bit++;
    
}
void  Display_MainMenu(void)
{
    //Dot_Flag = 0;    //清除小数点
    switch (MainMenu_Flag)
    {
        case 0 :  MainMenu_UI();	break;
        case 1 :  MainMenu_DYBB();	break;
        case 2 :  MainMenu_DLBB();	break;
    //    default : MainMenu_UI();	break;
        default : MainMenu_UI();  break;
    }
}
void  Display_CalibrationMenu(void)
{
//    Dot_Flag = 0;    //清除小数点
    switch (CalibrationMenu_Flag)
    {
        case CSM_AC5_1AV1_Flag :  CalibrationMenu_1AV1();break;
        case CSM_AC5_LowVoltage_Flag :  CalibrationMenu_LowVoltage();	break;
        case CSM_AC5_CalVoltage_Flag :  CalibrationMenu_Voltage();	break;
   //     case CSM_AC3_CalCurrent_Flag :  CalibrationMenu_Current();	break;
        case CSM_AC5_Date_Flag : Display_Date(Year,Date);break;
        case CSM_AC5_Version_Flag : Display_Version(Version);break;
        default : break;
    }
}
void  CalibrationMenu_LowVoltage(void)
{
    if (CSM_AC5_AV_Flag == CSM_AC5_1A)
    {
        if (CalibrationMenu2_Flag)
        {	
            Dec_TO_Hex(ADC_Current[0]);
            Display_4Byte(Led_Num[ADC_Value[3]],Led_Num[ADC_Value[2]],Led_Num[ADC_Value[1]],Led_Num[ADC_Value[0]],1);
        }
        else 
        {
            Display_4Byte(L_Byte,Led_Num[0],__Byte,A_Byte,4);
        }
    }
    else 
    {
        if (CalibrationMenu2_Flag)
        {	
            Dec_TO_Hex(ADC_Current[1]);
            Display_4Byte(Led_Num[ADC_Value[3]],Led_Num[ADC_Value[2]],Led_Num[ADC_Value[1]],Led_Num[ADC_Value[0]],1);
        }
        else 
        {
            Display_4Byte(U_Byte,Led_Num[0],__Byte,A_Byte,4);
        }
    }
}

void  CalibrationMenu_Voltage(void)
{
    if (CalibrationMenu2_Flag)
    {	
        Dec_TO_Hex(ADC_Current[1]);
        Display_4Byte(Led_Num[ADC_Value[3]],Led_Num[ADC_Value[2]],Led_Num[ADC_Value[1]],Led_Num[ADC_Value[0]],1);
    }
    else 
    {
        Display_4Byte(U_Byte,E_Byte,__Byte,A_Byte,4);
    };	
}
void  CalibrationMenu_Current(void)
{
    if (CalibrationMenu2_Flag)
    {	
        Dec_TO_Hex(ADC_Current[0]);
        Display_4Byte(Led_Num[ADC_Value[3]],Led_Num[ADC_Value[2]],Led_Num[ADC_Value[1]],Led_Num[ADC_Value[0]],3);
    }
    else 
    {
        Display_4Byte(L_Byte,E_Byte,__Byte,A_Byte,4);
    };
}
void  CalibrationMenu_1AV1(void)
{
    if (CalibrationMenu2_Flag)
    {
        if (Led_Shift == 0)
        {
            if (CSM_AC5_AV_Flag == CSM_AC5_1V)	Display_4Byte(__Byte,Led_Num[1],U_Byte,Led_Num[CSM_AC5_Function_Flag],4);
            else				Display_4Byte(__Byte,Led_Num[1],A_Byte,Led_Num[CSM_AC5_Function_Flag],4);
        }
        else 
        {
            if (Led_Shift_On == 0)
            {
               (Led_Shift == 1) ? (Display_Data[0] = 0) : (Display_Data[0] = Led_Num[CSM_AC5_Function_Flag]); 
                //(Led_Shift == 2) ? (Display_Data[1] = 0) : (Display_Data[1] = Led_Num[DYBB[1]]);
                if (Led_Shift == 2)
                {
                    Display_Data[1] = 0;
                }
                else
                {	
                    if (CSM_AC5_AV_Flag == CSM_AC5_1V)	        Display_Data[1] = U_Byte;
                    else 					Display_Data[1] = A_Byte;
                }
                Display_Data[2] = Led_Num[1];
                Display_Data[3] = __Byte;
            }
            else
            {
                if (CSM_AC5_AV_Flag == CSM_AC5_1V)	Display_4Byte(__Byte,Led_Num[1],U_Byte,Led_Num[CSM_AC5_Function_Flag],1);
                else					Display_4Byte(__Byte,Led_Num[1],A_Byte,Led_Num[CSM_AC5_Function_Flag],3);
            }		
        }
    
    }
    else
    {
        Display_4Byte(d_Byte,o_Byte,o_Byte,d_Byte,4);
    }
}
void  Display_Date(uchar Year,uint  Date)
{
    if (CalibrationMenu2_Flag == 1)
    {
        Display_4Byte(Led_Num[2],Led_Num[0],Led_Num[Year % 100 / 10],Led_Num[Year % 10],4);	
    }
    else if (CalibrationMenu2_Flag == 2)
    {
        Dec_TO_Hex(Date);
        Display_4Byte(Led_Num[ADC_Value[3]],Led_Num[ADC_Value[2]],Led_Num[ADC_Value[1]],Led_Num[ADC_Value[0]],4);	
    }
    else
    {
        Display_4Byte(d_Byte,A_Byte,t_Byte,E_Byte,4);	
    }
}
void  Display_Version(uint Version)
{ 
    if (CalibrationMenu2_Flag == 0)
    {
        Display_4Byte(U_Byte,E_Byte,r_Byte,__Byte,4);
    }
    else
    {
        Display_4Byte(U_Byte,Led_Num[Version % 1000 / 100],Led_Num[Version % 100 / 10],Led_Num[Version % 10],2);
    }
}
void  MainMenu_UI(void)
{
    if (CSM_AC5_AV_Flag == CSM_AC5_1V)
    {
        MainMenu_Voltage(1);
    }
    else
    {
   //     MainMenu_Current(0);
    }
}
void  MainMenu_Voltage(unsigned char i)
{
   //   Dot_Flag = Data_Send(ADC_Current[0]);
  //  Dot_Flag = 0;  
        if (ADC_Current[i] >= 10000000)
        {
            Dec_TO_Hex(ADC_Current[i]/10000);
            Dot_Flag = 4;
            LED_Flag = 1;
        }
        else if (ADC_Current[i] >= 1000000)
        {
            Dec_TO_Hex(ADC_Current[i]/1000);
            Dot_Flag = 1;
            LED_Flag = 1;
        }        
        else if (ADC_Current[i] >= 100000)
        {
            Dec_TO_Hex(ADC_Current[i]/100);
            Dot_Flag = 2;
            LED_Flag = 1;
        }        
        else if (ADC_Current[i] >= 10000)
        {
            Dec_TO_Hex(ADC_Current[i]/10);
            Dot_Flag = 3;
            LED_Flag = 1;
        }
        else
        {
            Dec_TO_Hex(ADC_Current[i]);
            Dot_Flag = 1;
            LED_Flag = 0;
        }
        Display_4Byte(Led_Num[ADC_Value[3]],Led_Num[ADC_Value[2]],Led_Num[ADC_Value[1]],Led_Num[ADC_Value[0]],Dot_Flag);
}
void  MainMenu_Current(unsigned char i)
{
    Dec_TO_Hex(ADC_Current[i]);
    Display_4Byte(Led_Num[ADC_Value[3]],Led_Num[ADC_Value[2]],Led_Num[ADC_Value[1]],Led_Num[ADC_Value[0]],1);
}
void  MainMenu_DYBB(void)
{
    Disp_DXBB(&DYBB[0]);
}
void  Disp_DXBB(unsigned char * DXBB)
{
    if (MainMenu2_Flag)
    {
        if (Led_Shift == 0)
        {
            Display_4Byte(Led_Num[DXBB[3]],Led_Num[DXBB[2]],Led_Num[DXBB[1]],Led_Num[DXBB[0]],4);
        }
        else 
        {
            if (Led_Shift_On == 0)
            {
                (Led_Shift == 1) ? (Display_Data[0] = 0) : (Display_Data[0] = Led_Num[DXBB[0]]); 
                (Led_Shift == 2) ? (Display_Data[1] = 0) : (Display_Data[1] = Led_Num[DXBB[1]]);
                (Led_Shift == 3) ? (Display_Data[2] = 0) : (Display_Data[2] = Led_Num[DXBB[2]]);
                (Led_Shift == 4) ? (Display_Data[3] = 0) : (Display_Data[3] = Led_Num[DXBB[3]]);
            }
            else
            {
                Display_4Byte(Led_Num[DXBB[3]],Led_Num[DXBB[2]],Led_Num[DXBB[1]],Led_Num[DXBB[0]],4);
            }		
        }
    }
    else
    {
        if (CSM_AC5_AV_Flag == CSM_AC5_1V)            Display_4Byte(d_Byte,y_Byte,b_Byte,b_Byte,4);
        else                                          Display_4Byte(d_Byte,L_Byte,b_Byte,b_Byte,4);
    } 
}
void  MainMenu_DLBB(void)
{
    Disp_DXBB(&DLBB[0]);
}

void  Display_4Byte(uchar Byte_3,uchar Byte_2,uchar Byte_1,uchar Byte_0,uchar dot)
{
    Display_Data[0] = Byte_0;   
    Display_Data[1] = Byte_1;   
    Display_Data[2] = Byte_2;   
    Display_Data[3] = Byte_3; 
    Dot_Flag = dot;
}
void  Dec_TO_Hex(unsigned long Value)
{
    ADC_Value[0] = Value % 10; 
    ADC_Value[1] = Value /10 % 10; 
    ADC_Value[2] = Value /100 % 10; 
    ADC_Value[3] = Value /1000 % 10; 
}