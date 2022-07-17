#include"stm8s.h"
unsigned char Count;
unsigned char Display_1ms_Count;
unsigned char Delay_5ms_Count;
unsigned char Delay_500ms_Count;
extern unsigned char Led_Shift_On;
void  Key_Scan(void);
void  Display_4Bit(void);
void  AD_Convert(void);

#pragma vector=0xd
 __interrupt void TIM1_OVR_UIF(void)
{
    TIM1->SR1 &=~0x01;  // ��������жϱ�ǣ���������������жϳ���
    AD_Convert();
    if (++Display_1ms_Count >= 5)
    {
	Display_1ms_Count = 0; 
	Display_4Bit();    //1ms �������ʾ
    }
    if (++Delay_5ms_Count >= 25)
    {
	Delay_5ms_Count = 0;
	Key_Scan();         //5ms ����ɨ��
        if (++Delay_500ms_Count >= 80)
        {
	    Delay_500ms_Count = 0;
	    Led_Shift_On = ~Led_Shift_On;  //ȡ��
        }
    }
}
