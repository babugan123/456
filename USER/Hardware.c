#include "stm8s.h"
void  Clk_Init(void);
void  Hardware_Init(void);
void  IO_Init(void);
void  Timer1_Init(void);
void   AD_Init(void);
void  WDG_Init(void);
void  WDT_Ret(void);
void  Hardware_Init(void)
{
    asm("sim");//��ȫ���ж�
    Clk_Init();
    IO_Init();
    AD_Init();
    Timer1_Init();
    WDG_Init();
    asm("rim");//��ȫ���ж�
}
void Clk_Init(void)
{

    CLK->CKDIVR = 0x00;/*��Ƶʱ��Դ 1��Ƶ  */
    CLK->ECKR |= 0x03;/*ʹ���ⲿʱ��*/
    CLK->SWCR |= 0x02;  /* ʹ��ת�� */ 
  
    CLK->SWR = 0xB4;/*����ʱ��Ϊ�ⲿ7.3728M����ʱ��*/
    while (!(CLK->SWCR & 0x08));  /* �ȴ�ֱ��ת����� */ 
  //  CLK->CKDIVR = 0x08;// 16M�ڲ�RC��2��Ƶ��ϵͳʱ��Ϊ8M
}
/**********************************************************/
/*************************IO�ڳ�ʼ��***********************/
/**********************************************************/
void  IO_Init(void)
{
    
    GPIOB->DDR &= 0x7F;//PB7 DEC    ����
    GPIOB->DDR &= 0xBF;//PB6 ENTER  ����
    GPIOF->DDR &= 0xEF;//PF4 ADD    ����
    GPIOA->DDR &= 0xF7;//PA3 SHIFT  ����
    GPIOB->CR1 |= 0xC0;//��������
    GPIOF->CR1 |= 0x10;
    GPIOA->CR1 |= 0x08;
//    GPIOB->CR2 &= 0xF3;//�������ٶ�Ϊ2MHz
//    GPIOF->CR2 &= 0xF3;//�������ٶ�Ϊ2MHz
//    GPIOA->CR2 &= 0xF3;//�������ٶ�Ϊ2MHz
    
    GPIOD->DDR |= 0xFC;//Led����˿�  PD2��PD3 ;�����λѡ PD4��5��6��7
    GPIOD->CR1 |= 0xFC;//�������
    GPIOD->CR2 &= 0xF3;//�������ٶ�Ϊ2MHz
    
    GPIOC->DDR |= 0xFE;//����ܶ�PC1~PC7
    GPIOC->CR1 |= 0xFE;//�������
    GPIOC->CR2 &= 0xF3;//�������ٶ�Ϊ2MHz
    
    GPIOD->ODR = 0x00;
    GPIOC->ODR = 0xff;
    
    GPIOE->DDR |= 0x20;//����ܶ�
    GPIOE->CR1 |= 0x20;//�������
    GPIOE->CR2 &= 0xF3;//�������ٶ�Ϊ2MHz
    
    GPIOB->DDR &= 0xf6;//�����˿� PB3��PB0
    GPIOB->CR1 &= 0xf6;//��©����
    GPIOB->CR2 &= 0xf6;//��ֹ�ⲿ�ж�
    
}
/**********************************************************/
/**********************��ʱ��1��ʼ��***********************/
/**********************************************************/

void  Timer1_Init(void)
{
    CLK->PCKENR1 |= 0x80;//������ʱ��1����ʱ�� 
    TIM1->CR1 = 0x95;//(1<<ARPE) | (0<<CMS) | (1<<DIR) | (1<<URS) | (1<<CEN);
    TIM1->IER = 0x01;  //��������ж�
    TIM1->SR1 &= 0xfe;
    TIM1->PSCRH = (2-1)/256; //2��Ƶ
    TIM1->PSCRL = (2-1)%256;
    TIM1->ARRH = 1152/256;    //��ʱ20ms/64
    TIM1->ARRL = 1152%256;   
}
//***********************************
//************AD������ʼ��***********
//***********************************

void   AD_Init(void)
{
    CLK->PCKENR2 |= 0x08; //ADCʱ�Ӵ�
    ADC1->CSR = 0x00;     //ģ��ͨ��ΪAIN0
    ADC1->CR1 = 0x50;     //10��Ƶ������ɨ��ת��
    ADC1->CR2 = 0x08;     //�����Ҷ��룬ɨ��ģʽʧ��
    ADC1->CR3 = 0x00;     //���ݻ����
    ADC1->CR1 |= 0x01;    //��һ��д1�Ǵӵ͹���ģʽ����
    
//    CLK->PCKENR2 |= 0x08; //ADCʱ�Ӵ�
  //  ADC1->CSR = 0x02;     //ģ��ͨ��ΪAIN0~AIN2
    //ADC1->CR1 = 0x50;     //10��Ƶ������ɨ��ת��
    //ADC1->CR2 = 0x0A;     //�����Ҷ��룬ɨ��ģʽʹ��
    //ADC1->CR3 = 0x80;     //���ݻ���ʹ��
    //ADC1->CR1 |= 0x01;    //��һ��д1�Ǵӵ͹���ģʽ����
} 
void  WDG_Init(void)
{
    IWDG->KR = 0xCC; //0xCC �������Ź� �� 0xAA ˢ�¿��Ź� ��
    IWDG->KR = 0x55; //��� PR �� RLR ��д����
    IWDG->PR = 0x05;  //101   128��Ƶ
    IWDG->RLR = 0xff; // 128��Ƶ  510ms  
}
void  WDT_Ret(void)
{
    IWDG->KR = 0xAA;
}