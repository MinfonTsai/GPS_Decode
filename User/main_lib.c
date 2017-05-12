#include "stm32f10x.h"

FlagStatus RX_status;


//RCCʱ������

void RCC_cfg( void)

{

       //�������״̬����

       ErrorStatus HSEStartUpStatus;

      

       //��RCC�Ĵ�����������ΪĬ��ֵ

       RCC_DeInit();



       //���ⲿ����ʱ�Ӿ���

       RCC_HSEConfig(RCC_HSE_ON);



       //�ȴ��ⲿ����ʱ�Ӿ�����

       HSEStartUpStatus = RCC_WaitForHSEStartUp();

       if(HSEStartUpStatus == SUCCESS)

       {

              //����AHBʱ��(HCLK)Ϊϵͳʱ��

              RCC_HCLKConfig(RCC_SYSCLK_Div1);



              //���ø���AHBʱ��(APB2)ΪHCLKʱ��

              RCC_PCLK2Config(RCC_HCLK_Div1);



              //���õ���AHBʱ��(APB1)ΪHCLK��2��Ƶ

              RCC_PCLK1Config(RCC_HCLK_Div2);

             

              //����FLASH������ʱ

              FLASH_SetLatency(FLASH_Latency_2);



              //ʹ��Ԥȡָ����

              FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);



              //����PLLʱ�ӣ�ΪHSE��9��Ƶ 8MHz * 9 = 72MHz

              RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);



              //ʹ��PLL

              RCC_PLLCmd(ENABLE);



              //�ȴ�PLL׼������

              while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);



              //����PLLΪϵͳʱ��Դ

              RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);



              //�ж�PLL�Ƿ���ϵͳʱ��

              while(RCC_GetSYSCLKSource() != 0x08);

       }

         //��GPIOʱ�ӣ����ù��ܣ�����1��ʱ��

        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO | RCC_APB2Periph_USART1, ENABLE);
		    RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART3| RCC_APB1Periph_USART2, ENABLE);
}



//IO������

void GPIO_cfg( void)

{

       GPIO_InitTypeDef GPIO_InitStructure;

/*

       //PA9��ΪUART-1��TX�ˣ��򿪸��ã�����������

       GPIO_StructInit(&GPIO_InitStructure);

       GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;

       GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;

       GPIO_Init(GPIOA , &GPIO_InitStructure);



       //PA10��ΪUART-1��RX�ˣ������������

       GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;

       GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;

       GPIO_Init(GPIOA, &GPIO_InitStructure);



  //PB10��ΪUR3��TX�ˣ��򿪸��ã�����������

       GPIO_StructInit(&GPIO_InitStructure);

       GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;

       GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;

       GPIO_Init(GPIOB , &GPIO_InitStructure);



       //PB11��ΪUR3��RX�ˣ������������

       GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;

       GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;

       GPIO_Init(GPIOB, &GPIO_InitStructure);
			 
*/			 
			 

       //LED��ʾ�������ڷ���/��������

       GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;

       GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;

       GPIO_Init(GPIOB, &GPIO_InitStructure);

}



//���ڳ�ʼ��

void USARTx_cfg( USART_TypeDef* USARTx ) 
{
			 GPIO_InitTypeDef GPIO_InitStructure;
		    USART_InitTypeDef USART_InitStructure;
	
	       //���ṹ������Ϊȱʡ״̬

       USART_StructInit(&USART_InitStructure);


	
      if(  USARTx == USART1)
			{
					//PA9��ΪUART-1��TX�ˣ��򿪸��ã�����������
				GPIO_StructInit(&GPIO_InitStructure);
       GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
       GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
       GPIO_Init(GPIOA , &GPIO_InitStructure);

       //PA10��ΪUART-1��RX�ˣ������������
       GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
       GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
       GPIO_Init(GPIOA, &GPIO_InitStructure);

			    //����������Ϊ115200

       USART_InitStructure.USART_BaudRate = 115200;


			}
			else if (  USARTx == USART2)
			{
				  //PA2��ΪUR2��TX�ˣ��򿪸��ã�����������
       GPIO_StructInit(&GPIO_InitStructure);
       GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
       GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
       GPIO_Init(GPIOA , &GPIO_InitStructure);

       //PA3��ΪUR2��RX�ˣ������������
       GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
       GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
       GPIO_Init(GPIOA, &GPIO_InitStructure);		 

			    //����������Ϊ9600

       USART_InitStructure.USART_BaudRate =  115200; // 9600;


			}
			else if (  USARTx == USART3)
			{
				  //PB10��ΪUR3��TX�ˣ��򿪸��ã�����������
       GPIO_StructInit(&GPIO_InitStructure);
       GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
       GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
       GPIO_Init(GPIOB , &GPIO_InitStructure);

       //PB11��ΪUR3��RX�ˣ������������
       GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
       GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
       GPIO_Init(GPIOB, &GPIO_InitStructure);		 


			    //����������Ϊ115200

       USART_InitStructure.USART_BaudRate = 115200;



			}
	
       //һ֡���ݵĿ������Ϊ8bits

       USART_InitStructure.USART_WordLength = USART_WordLength_8b;

       //��֡��β����1��ֹͣλ

       USART_InitStructure.USART_StopBits = USART_StopBits_1;

       //��żʧ��ģʽ������żУ��

       USART_InitStructure.USART_Parity = USART_Parity_No;

       //����/����ʹ��

       USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

       //Ӳ��������ʧ��

       USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;

       //���ô���1

       USART_Init(USARTx, &USART_InitStructure);
      

       //�򿪴���1���ж���Ӧ����

      USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);

       //�򿪴���1

       USART_Cmd(USARTx, ENABLE);

}



//�����ж�

void NVIC_cfg( void )

{
        NVIC_InitTypeDef NVIC_InitStructure;

        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);                                               //ѡ���жϷ���2
        NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;                              //ѡ�񴮿�1�ж�
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;                                 //��ռʽ�ж����ȼ�����Ϊ0
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;                                            //��Ӧʽ�ж����ȼ�����Ϊ0
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                                                 //ʹ���ж�
        NVIC_Init(&NVIC_InitStructure);
    //	 USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	
}

void  EXT_INT_cfg( void )
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	//GPIO ����ȷ����
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

	//��NVIC_Configuration ��Configures the NVIC and Vector Table base address
	
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

