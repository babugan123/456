#include  "stm8s.h"
#include  "main.h"
#include  "AD.h"
extern struct BD BD_UI[2];
extern  unsigned char CSM_AC5_Function_Flag;
extern  unsigned char CSM_AC5_AV_Flag;
extern  unsigned char CalibrationMenu_Flag;
extern  unsigned char MainMenu_Flag;
extern  unsigned char DYBB[];
extern  unsigned char DLBB[];
void  EEPROM_Write(unsigned char Addr,unsigned char Data);
unsigned int   EEPROM_Read_2Byte(unsigned char Addr);
unsigned char  EEPROM_Read(unsigned char Addr);
void  EEPROM_WriteData(void);
void  EEPROM_ReadInit(void)
{
    unsigned char i;  
    if (EEPROM_Read(1) != 0)
    {
         CSM_AC5_Function_Flag = EEPROM_Read(1);
//         CSM_AC5_AV_Flag = EEPROM_Read(2);
         
    }
    if (EEPROM_Read(3) != 0)
    {
        for (i=0;i<2;i++)
        {
           BD_UI[i].B = EEPROM_Read_2Byte(3 + i*2);
           BD_UI[i].K = EEPROM_Read_2Byte(9 + i*2);
        }
    }
    for (i=0;i<4;i++)
    {
        if (CSM_AC5_AV_Flag == CSM_AC5_1V)  DYBB[i] = EEPROM_Read(50+i);
        else                                DLBB[i] = EEPROM_Read(54+i);
    }
    if ((DYBB[0] == 0)&&(DYBB[1] == 0)&&(DYBB[2] == 0)&&(DYBB[3] == 0))
    {
        DYBB[0] = 1;
    }
    if ((DLBB[0] == 0)&&(DLBB[1] == 0)&&(DLBB[2] == 0)&&(DLBB[3] == 0))
    {
        DLBB[0] = 1;
    }
}
unsigned int  EEPROM_Read_2Byte(unsigned char Addr)
{
    unsigned int Data;  
    Data = ((unsigned int)EEPROM_Read(Addr)) << 8;
    Data += EEPROM_Read(Addr + 1);
    return Data;
}
void  EEPROM_WriteData(void)
{
    if (CalibrationMenu_Flag == 0)
    {
        switch (MainMenu_Flag)
        {
            case  MainMenuDYBB_Flag : 
              {
                  EEPROM_Write(50,DYBB[0]);
                  EEPROM_Write(51,DYBB[1]);
                  EEPROM_Write(52,DYBB[2]);

                  EEPROM_Write(53,DYBB[3]);
              }break;
            case  MainMenuDLBB_Flag :
              {
                  EEPROM_Write(54,DLBB[0]);
                  EEPROM_Write(55,DLBB[1]);
                  EEPROM_Write(56,DLBB[2]);

                  EEPROM_Write(57,DLBB[3]);
              }break;
            default :
                break;
        }
    }
    else
    {
        switch (CalibrationMenu_Flag)
        {
            case  CSM_AC5_1AV1_Flag :
              {
                  EEPROM_Write(1,CSM_AC5_Function_Flag);
                  EEPROM_Write(2,CSM_AC5_AV_Flag);
              }break;
            case  CSM_AC5_LowVoltage_Flag :
              {
                  EEPROM_Write(3,BD_UI[0].B >> 8);
                  EEPROM_Write(4,BD_UI[0].B);
                  EEPROM_Write(5,BD_UI[1].B >> 8);
                  EEPROM_Write(6,BD_UI[1].B);
              }break;
            case  CSM_AC5_CalVoltage_Flag :
              {
                  EEPROM_Write(9,BD_UI[0].K >>  8); 
                  EEPROM_Write(10,BD_UI[0].K);
                  EEPROM_Write(11,BD_UI[1].K >> 8); 
                  EEPROM_Write(12,BD_UI[1].K);
              }break;
            default:
              break;
        }
    }
    
}
void  EEPROM_Init(void)
{
    FLASH->DUKR = 0xAE; //第一个硬件密匙
    FLASH->DUKR = 0x56;//第二个硬件密匙
    while(!(FLASH->IAPSR&0x08));
}

void  EEPROM_Write(unsigned char Addr,unsigned char Data)
{
    unsigned char  *p;
    FLASH->DUKR = 0xAE; //第一个硬件密匙
    FLASH->DUKR = 0x56;//第二个硬件密匙
    while(!(FLASH->IAPSR&0x08));
    
   // asm("sim");//关全局中断
    p = (unsigned char *) (Addr + 0x4000);
    *p = Data;
    while(!(FLASH->IAPSR&0x04)); //等待写操作成功
}
unsigned char  EEPROM_Read(unsigned char Addr)
{
    unsigned char *p;
    p = (unsigned char *) (Addr + 0x4000);
    return *p;
}