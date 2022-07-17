#include  "main.h"
#define Key_Next   2 
#define Key_Enter  1
#define Key_Add    0
void  Key_Scan(void);
void  Key_StateJudge(void);
void  MainMenu_Add_Operation(void);
void  CalibrationMenu_Add_Operation(void);
void  MainMenu_Dec_Operation(void);
void  CalibrationMenu_Dec_Operation(void);
static uchar Key_Long[3] = {0,0,0};		//长按标识：1为长按标识
static uint  Key_count1 = 0;
static uint  Key_count2 = 0;
static uchar Keypress = 1;			//端口0代表按键按下，1代表按键没按下
static uchar Key_Bit = 0;
static uchar Keystate[3] = {1,1,1};
static unsigned char Key_Flag[3] = {1,1,1};