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
    asm("sim");//关全局中断
    Clk_Init();
    IO_Init();
    AD_Init();
    Timer1_Init();
    WDG_Init();
    asm("rim");//开全局中断
}
void Clk_Init(void)
{

    CLK->CKDIVR = 0x00;/*分频时钟源 1分频  */
    CLK->ECKR |= 0x03;/*使能外部时钟*/
    CLK->SWCR |= 0x02;  /* 使能转化 */ 
  
    CLK->SWR = 0xB4;/*设置时钟为外部7.3728M高速时钟*/
    while (!(CLK->SWCR & 0x08));  /* 等待直到转换完成 */ 
  //  CLK->CKDIVR = 0x08;// 16M内部RC经2分频后系统时钟为8M
}
/**********************************************************/
/*************************IO口初始化***********************/
/**********************************************************/
void  IO_Init(void)
{
    
    GPIOB->DDR &= 0x7F;//PB7 DEC    输入
    GPIOB->DDR &= 0xBF;//PB6 ENTER  输入
    GPIOF->DDR &= 0xEF;//PF4 ADD    输入
    GPIOA->DDR &= 0xF7;//PA3 SHIFT  输入
    GPIOB->CR1 |= 0xC0;//输入上拉
    GPIOF->CR1 |= 0x10;
    GPIOA->CR1 |= 0x08;
//    GPIOB->CR2 &= 0xF3;//最大输出速度为2MHz
//    GPIOF->CR2 &= 0xF3;//最大输出速度为2MHz
//    GPIOA->CR2 &= 0xF3;//最大输出速度为2MHz
    
    GPIOD->DDR |= 0xFC;//Led输出端口  PD2，PD3 ;数码管位选 PD4、5、6、7
    GPIOD->CR1 |= 0xFC;//推挽输出
    GPIOD->CR2 &= 0xF3;//最大输出速度为2MHz
    
    GPIOC->DDR |= 0xFE;//数码管段PC1~PC7
    GPIOC->CR1 |= 0xFE;//推挽输出
    GPIOC->CR2 &= 0xF3;//最大输出速度为2MHz
    
    GPIOD->ODR = 0x00;
    GPIOC->ODR = 0xff;
    
    GPIOE->DDR |= 0x20;//数码管段
    GPIOE->CR1 |= 0x20;//推挽输出
    GPIOE->CR2 &= 0xF3;//最大输出速度为2MHz
    
    GPIOB->DDR &= 0xf6;//采样端口 PB3、PB0
    GPIOB->CR1 &= 0xf6;//开漏输入
    GPIOB->CR2 &= 0xf6;//禁止外部中断
    
}
/**********************************************************/
/**********************定时器1初始化***********************/
/**********************************************************/

void  Timer1_Init(void)
{
    CLK->PCKENR1 |= 0x80;//开启定时器1外设时钟 
    TIM1->CR1 = 0x95;//(1<<ARPE) | (0<<CMS) | (1<<DIR) | (1<<URS) | (1<<CEN);
    TIM1->IER = 0x01;  //允许更新中断
    TIM1->SR1 &= 0xfe;
    TIM1->PSCRH = (2-1)/256; //2分频
    TIM1->PSCRL = (2-1)%256;
    TIM1->ARRH = 1152/256;    //定时20ms/64
    TIM1->ARRL = 1152%256;   
}
//***********************************
//************AD采样初始化***********
//***********************************

void   AD_Init(void)
{
    CLK->PCKENR2 |= 0x08; //ADC时钟打开
    ADC1->CSR = 0x00;     //模拟通道为AIN0
    ADC1->CR1 = 0x50;     //10分频，单次扫描转换
    ADC1->CR2 = 0x08;     //数据右对齐，扫描模式失能
    ADC1->CR3 = 0x00;     //数据缓存关
    ADC1->CR1 |= 0x01;    //第一次写1是从低功耗模式唤醒
    
//    CLK->PCKENR2 |= 0x08; //ADC时钟打开
  //  ADC1->CSR = 0x02;     //模拟通道为AIN0~AIN2
    //ADC1->CR1 = 0x50;     //10分频，单次扫描转换
    //ADC1->CR2 = 0x0A;     //数据右对齐，扫描模式使能
    //ADC1->CR3 = 0x80;     //数据缓存使能
    //ADC1->CR1 |= 0x01;    //第一次写1是从低功耗模式唤醒
} 
void  WDG_Init(void)
{
    IWDG->KR = 0xCC; //0xCC 启动看门狗 ； 0xAA 刷新看门狗 ；
    IWDG->KR = 0x55; //解除 PR 及 RLR 的写保护
    IWDG->PR = 0x05;  //101   128分频
    IWDG->RLR = 0xff; // 128分频  510ms  
}
void  WDT_Ret(void)
{
    IWDG->KR = 0xAA;
}