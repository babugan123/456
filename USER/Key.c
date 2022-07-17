#include  "stm8s.h"
#include  "Key.h"
#include  "main.h"
unsigned char MainMenu3_Flag = 0;
unsigned char MainMenu2_Flag = 0;
unsigned char  CSM_AC5_Function_Flag = 1;
unsigned char  MainMenu_Flag;
unsigned char  CalibrationMenu2_Flag;
unsigned char  CalibrationMenu_Flag;
unsigned char  Led_Shift = 0;
extern  unsigned char CSM_AC5_AV_Flag;
extern  unsigned char  DYBB[];
extern  unsigned char  DLBB[];
void  Calibration_LowVoltage(void);
void  Calibration_Voltage(void);
void  EEPROM_WriteData(void);

void  Key_Scan(void)
{
    if      ((GPIOA->IDR&0x08) == 0x00)	{Keypress = 0;Key_Bit= Key_Enter;}
    else if ((GPIOF->IDR&0x10) == 0x00) {Keypress = 0;Key_Bit = Key_Next;}
    else if ((GPIOB->IDR&0x80) == 0x00) {Keypress = 0;Key_Bit = Key_Add;}
    else  Keypress = 1;
    Key_StateJudge();
}

void  Key_StateJudge(void)
{	
	switch (Keystate[Key_Bit])
	{
		case 1 :					//1 没有按键按下状态
			{
				if (Keypress == 0)	Keystate[Key_Bit] = 2;
				else 				Keystate[Key_Bit] = 1;
			}break;
				
		case 2 :					//2 确认有按键按下状态
			{
				if (Keypress == 0)	
				{
					Key_count1++;
					if(Key_count1 >= 20)
					{
						Keystate[Key_Bit] = 3;
					}
				}
				else 		Keystate[Key_Bit] = 1;
			}break;

		case 3 :					//3 按键稳定按下状态
			{
				if (Keypress == 0)		
				{
					if (Key_count2++ >= 500)
					{
						Key_Long[Key_Bit] = 1;
					}
					else if ((Key_count1++ > 100)&&(MainMenu3_Flag != 0))
					{	Key_Flag[Key_Bit] = 0;
					
						Key_count1 = 0;
					}
//					Key_Pre[Key_Bit] = 1;
//					Key_count1 = 0;
				}
				else Keystate[Key_Bit] = 4;
			}break;

		case 4 :					//4 按键释放状态
			{
				if	(Keypress)	
				{
					Keystate[Key_Bit] = 1;
					if (Key_Long[Key_Bit] == 0)   Key_Flag[Key_Bit] = 0;
					else Key_Long[Key_Bit] = 0;
					Key_count1 = 0;		
					Key_count2 = 0;
				}
				else			Keystate[Key_Bit] = 3;
			}break;

		default :
			{
				Keystate[Key_Bit] = 0;
				Key_count1 = 0;
			}
			break;		
	}
}
void  Buttom_Scan(void)
{	
/******************************************************************************/
/************************************翻页键************************************/
/******************************************************************************/
    if ((Key_Flag[Key_Enter] == 0) && (Key_Long[Key_Enter] == 0))
    {
        if (Led_Shift != 0)
        {
            Led_Shift = 0;
        }
        else
        {
            if (CalibrationMenu_Flag == 0)
            {
                if (MainMenu2_Flag == 0)
                {
                    EEPROM_WriteData();
                    //界面判断    
                    if ((MainMenu_Flag == MainMenuUI_Flag)&&(CSM_AC5_AV_Flag == CSM_AC5_1A))
                    {
                        MainMenu_Flag = MainMenuDYBB_Flag;
                    }  
                    if ((MainMenu_Flag == MainMenuDYBB_Flag)&&(CSM_AC5_AV_Flag == CSM_AC5_1V))
                    {
                        MainMenu_Flag = MainMenuDLBB_Flag;
                    }
                    if (++MainMenu_Flag >= Max_MainMenu_Flag)
                    {
                        MainMenu_Flag = MainMenuUI_Flag;
                    }
                }
                else
                {
                    MainMenu2_Flag = 0;
                }
            }
            else
            {
                if (CalibrationMenu2_Flag == 0)
		{
                    EEPROM_WriteData();
                    if (++CalibrationMenu_Flag >= Max_CalibrationMenu_Flag)  CalibrationMenu_Flag = 0;	
                    if ((CSM_AC5_AV_Flag == CSM_AC5_1V)&&((CalibrationMenu_Flag == CSM_AC5_CalCurrent_Flag)||(CalibrationMenu_Flag == CSM_AC5_CT1_5A_Flag)))
		    {
			CalibrationMenu_Flag += 1;
		    }	
		    else if ((CSM_AC5_AV_Flag == CSM_AC5_1A)&&(CalibrationMenu_Flag == CSM_AC5_CalVoltage_Flag))
		    {
			CalibrationMenu_Flag += 1;
		    }	  
                }
                else
		{
	            CalibrationMenu2_Flag = 0;
		}
            }
        }
        Key_Flag[Key_Enter] = 1;	
    }
    else if ((Key_Long[Key_Enter] == 1)&&(MainMenu_Flag == 0))
    {
        if (CalibrationMenu_Flag == 0)
	{
	    CalibrationMenu_Flag = 1;
	}    
        Key_Flag[Key_Enter] = 1;
    }
/******************************************************************************/
/************************************切换键************************************/
/******************************************************************************/
    else if ((Key_Flag[Key_Next] == 0) && (CalibrationMenu_Flag == 0))
    {
        if ((MainMenu_Flag != 0)&&(MainMenu2_Flag == 0))
        {
	    MainMenu2_Flag = 1;
	}
        else if (((MainMenu_Flag == MainMenuDYBB_Flag)||(MainMenu_Flag == MainMenuDLBB_Flag))) 
        {
            if (Led_Shift++ > 3)		Led_Shift = 0;
        }
        Key_Flag[Key_Next] = 1;
    }
    else if ((Key_Flag[Key_Next] == 0) && (CalibrationMenu_Flag != 0))
    {
        if (CalibrationMenu2_Flag == 0)
        {
            CalibrationMenu2_Flag = 1;
        }
        else if (CalibrationMenu_Flag == CSM_AC5_Date_Flag)
	{
	    if (CalibrationMenu2_Flag != 1) CalibrationMenu2_Flag = 1;
	    else 					 CalibrationMenu2_Flag = 2;
	}
        else if ((CalibrationMenu_Flag == CSM_AC5_LowVoltage_Flag)||(CalibrationMenu_Flag == CSM_AC5_CalVoltage_Flag))
        {
            if (Led_Shift++ >= 2)		Led_Shift = 0;
        }
        Key_Flag[Key_Next] = 1;
    }
/******************************************************************************/
/***************************************按键加*********************************/
/******************************************************************************/
    else if ((Key_Flag[Key_Add] == 0)&&(Key_Long[Key_Add] == 0))
    {
        if (CalibrationMenu_Flag == 0)
        {
            MainMenu_Add_Operation();
        }
        else
        {
            CalibrationMenu_Add_Operation();
        }
        Key_Flag[Key_Add] = 1;
    }
    else if (Key_Long[Key_Add] == 1)
    {
    }
}
void  MainMenu_Add_Operation(void)
{
    switch (MainMenu_Flag)
    {
        case MainMenuDYBB_Flag : 
          {
              if (Led_Shift != 0)
              {
                  if (DYBB[Led_Shift-1]++ >= 9)	DYBB[Led_Shift-1] = 0;
              }
          }break;
        case MainMenuDLBB_Flag : 
          {
              if (Led_Shift != 0)
              {
              if (DLBB[Led_Shift-1]++ >= 9)	DLBB[Led_Shift-1] = 0;
              }
          }break;
        default:
          break;
    }
}
void  CalibrationMenu_Add_Operation(void)
{
    switch (CalibrationMenu_Flag)
    {
        case CSM_AC5_1AV1_Flag :
          {
              if (CSM_AC5_Function_Flag == 2)	CSM_AC5_Function_Flag = 1;
              else                              CSM_AC5_Function_Flag = 2;
          }break;
        case CSM_AC5_LowVoltage_Flag :
	  {
	      Calibration_LowVoltage();
	  }break;
        case CSM_AC5_CalVoltage_Flag :
	  {
	      Calibration_Voltage();  
	  }break;
        default : break;
    }
}
void  MainMenu_Dec_Operation(void)
{
    switch (MainMenu_Flag)
    {
        case MainMenuDYBB_Flag : 
          {
              if (Led_Shift != 0)
              {
                  if (DYBB[Led_Shift-1]-- <= 0)	DYBB[Led_Shift-1] = 9;
              }
          }break;
        case MainMenuDLBB_Flag : 
          {
              if (Led_Shift != 0)
              {
                  if (DLBB[Led_Shift-1]-- <= 0)	DLBB[Led_Shift-1] = 9;
              }
          }break;
        default:
          break;
    }
}
