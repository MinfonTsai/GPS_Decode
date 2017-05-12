#include "stm32f10x.h"

FlagStatus RX_status;


//RCC时钟配置

void RCC_cfg( void)

{

       //定义错误状态变量

       ErrorStatus HSEStartUpStatus;

      

       //将RCC寄存器重新设置为默认值

       RCC_DeInit();



       //打开外部高速时钟晶振

       RCC_HSEConfig(RCC_HSE_ON);



       //等待外部高速时钟晶振工作

       HSEStartUpStatus = RCC_WaitForHSEStartUp();

       if(HSEStartUpStatus == SUCCESS)

       {

              //设置AHB时钟(HCLK)为系统时钟

              RCC_HCLKConfig(RCC_SYSCLK_Div1);



              //设置高速AHB时钟(APB2)为HCLK时钟

              RCC_PCLK2Config(RCC_HCLK_Div1);



              //设置低速AHB时钟(APB1)为HCLK的2分频

              RCC_PCLK1Config(RCC_HCLK_Div2);

             

              //设置FLASH代码延时

              FLASH_SetLatency(FLASH_Latency_2);



              //使能预取指缓存

              FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);



              //设置PLL时钟，为HSE的9倍频 8MHz * 9 = 72MHz

              RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);



              //使能PLL

              RCC_PLLCmd(ENABLE);



              //等待PLL准备就绪

              while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);



              //设置PLL为系统时钟源

              RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);



              //判断PLL是否是系统时钟

              while(RCC_GetSYSCLKSource() != 0x08);

       }

         //打开GPIO时钟，复用功能，串口1的时钟

        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO | RCC_APB2Periph_USART1, ENABLE);
		    RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART3| RCC_APB1Periph_USART2, ENABLE);
}



//IO口配置

void GPIO_cfg( void)

{

       GPIO_InitTypeDef GPIO_InitStructure;

/*

       //PA9作为UART-1的TX端，打开复用，负责发送数据

       GPIO_StructInit(&GPIO_InitStructure);

       GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;

       GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;

       GPIO_Init(GPIOA , &GPIO_InitStructure);



       //PA10作为UART-1的RX端，负责接收数据

       GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;

       GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;

       GPIO_Init(GPIOA, &GPIO_InitStructure);



  //PB10作为UR3的TX端，打开复用，负责发送数据

       GPIO_StructInit(&GPIO_InitStructure);

       GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;

       GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;

       GPIO_Init(GPIOB , &GPIO_InitStructure);



       //PB11作为UR3的RX端，负责接收数据

       GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;

       GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;

       GPIO_Init(GPIOB, &GPIO_InitStructure);
			 
*/			 
			 

       //LED显示串口正在发送/接收数据

       GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;

       GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;

       GPIO_Init(GPIOB, &GPIO_InitStructure);

}



//串口初始化

void USARTx_cfg( USART_TypeDef* USARTx ) 
{
			 GPIO_InitTypeDef GPIO_InitStructure;
		    USART_InitTypeDef USART_InitStructure;
	
	       //将结构体设置为缺省状态

       USART_StructInit(&USART_InitStructure);


	
      if(  USARTx == USART1)
			{
					//PA9作为UART-1的TX端，打开复用，负责发送数据
				GPIO_StructInit(&GPIO_InitStructure);
       GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
       GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
       GPIO_Init(GPIOA , &GPIO_InitStructure);

       //PA10作为UART-1的RX端，负责接收数据
       GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
       GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
       GPIO_Init(GPIOA, &GPIO_InitStructure);

			    //波特率设置为115200

       USART_InitStructure.USART_BaudRate = 115200;


			}
			else if (  USARTx == USART2)
			{
				  //PA2作为UR2的TX端，打开复用，负责发送数据
       GPIO_StructInit(&GPIO_InitStructure);
       GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
       GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
       GPIO_Init(GPIOA , &GPIO_InitStructure);

       //PA3作为UR2的RX端，负责接收数据
       GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
       GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
       GPIO_Init(GPIOA, &GPIO_InitStructure);		 

			    //波特率设置为9600

       USART_InitStructure.USART_BaudRate =  115200; // 9600;


			}
			else if (  USARTx == USART3)
			{
				  //PB10作为UR3的TX端，打开复用，负责发送数据
       GPIO_StructInit(&GPIO_InitStructure);
       GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
       GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
       GPIO_Init(GPIOB , &GPIO_InitStructure);

       //PB11作为UR3的RX端，负责接收数据
       GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
       GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
       GPIO_Init(GPIOB, &GPIO_InitStructure);		 


			    //波特率设置为115200

       USART_InitStructure.USART_BaudRate = 115200;



			}
	
       //一帧数据的宽度设置为8bits

       USART_InitStructure.USART_WordLength = USART_WordLength_8b;

       //在帧结尾传输1个停止位

       USART_InitStructure.USART_StopBits = USART_StopBits_1;

       //奇偶失能模式，无奇偶校验

       USART_InitStructure.USART_Parity = USART_Parity_No;

       //发送/接收使能

       USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

       //硬件流控制失能

       USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;

       //设置串口1

       USART_Init(USARTx, &USART_InitStructure);
      

       //打开串口1的中断响应函数

      USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);

       //打开串口1

       USART_Cmd(USARTx, ENABLE);

}



//配置中断

void NVIC_cfg( void )

{
        NVIC_InitTypeDef NVIC_InitStructure;

        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);                                               //选择中断分组2
        NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;                              //选择串口1中断
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;                                 //抢占式中断优先级设置为0
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;                                            //响应式中断优先级设置为0
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                                                 //使能中断
        NVIC_Init(&NVIC_InitStructure);
    //	 USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	
}

void  EXT_INT_cfg( void )
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	//GPIO 的正确设置
/* Enable GPIOD clock */
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
/* Configure PD.03, PC.04, as input floating */
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_4 ;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
GPIO_Init(GPIOC, &GPIO_InitStructure);

// Interrupt initial...
// Configure EXTI Line4 to generate an interrupt on  Rising edge
EXTI_InitStructure.EXTI_Line = EXTI_Line4;
EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
EXTI_InitStructure.EXTI_LineCmd = ENABLE;
EXTI_Init(&EXTI_InitStructure);	
GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource4);

// Configure EXTI Line5 to generate an interrupt on falling edge
EXTI_InitStructure.EXTI_Line = EXTI_Line5;
EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; 
EXTI_InitStructure.EXTI_LineCmd = ENABLE;
EXTI_Init(&EXTI_InitStructure);	
GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource5);

	//在NVIC_Configuration 中Configures the NVIC and Vector Table base address
	
	/* Configure the Priority Group to 2 bits */
NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
/* enabling interrupt */
//NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQChannel;
//NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
//NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//NVIC_Init(&NVIC_InitStructure);

/* Enable the EXTI3 Interrupt on PD.5 */
NVIC_InitStructure.NVIC_IRQChannel =EXTI4_IRQn;
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
NVIC_Init(&NVIC_InitStructure);

/* Enable the EXTI3 Interrupt on PD.5 */
NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;  
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
NVIC_Init(&NVIC_InitStructure);
}

