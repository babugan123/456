
#include"stm8s.h"
#include  "main.h"
unsigned char CSM_AC5_AV_Flag = CSM_AC5_1V;
uchar Year = 14;  //14Äê
uint  Date = 1014; //10ÔÂ14ÈÕ
uint  Version = 101; //V1.01
void  Hardware_Init(void);
void  Display_Scan(void);
void  Buttom_Scan(void);
void  AD_Scan(void);
void  WDT_Ret(void);
void  EEPROM_ReadInit(void);
int main(void)
{
    Hardware_Init();
    EEPROM_ReadInit();
    while(1)
    {
       Display_Scan();
       Buttom_Scan();
       AD_Scan();
       WDT_Ret();
    }
    return 0;
}



