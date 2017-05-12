 
//#include "stm32f10x_lib.h"

#include "stm32f10x.h"

void USART1_IRQHandler(void);
	
//FlagStatus RX_status;

void RCC_cfg( void );
void GPIO_cfg(void);
void USART_cfg(void);
void USARTx_cfg( USART_TypeDef* USARTx ) ;
void NVIC_cfg( void);
void  EXT_INT_cfg ( void );

void GPIOB_Init(void);
void GPIOC_Init(void);
void USARTx_Puts( USART_TypeDef* USARTx, char *str);
void Delay(uint32_t nCount);

void  Value_init (void);
void dbgshow_sw( int sw,  int stat );

int lock_status;
int stat_sw1, stat_sw2;
int oldstat_sw1, oldstat_sw2, oldstat_lock;

int main()
{

	//   char Ln[]="\r\n";
	
     RCC_cfg();
   //  GPIO_cfg();
     NVIC_cfg();
  
	   USARTx_cfg(USART1);
		 USART_ClearFlag(USART1,USART_FLAG_TC); 
	
	   USARTx_cfg(USART2);
		 USART_ClearFlag(USART2,USART_FLAG_TC); 
	
    GPIOB_Init();
	  GPIOC_Init();

		
										
  	EXT_INT_cfg ();
	
	   Value_init ();  // initial  variable
	
	 		while(1) {
					    		
                   
				
						 if( GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_4) == 0 )
						 {	
     							 stat_sw1 = 0;
							     dbgshow_sw( 1,  stat_sw1 );
						 }
						 
						  if( GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5) == 1 )
							{								
								stat_sw2 = 1;
					  //   else    stat_sw2 = 1;
				        dbgshow_sw( 2 ,   stat_sw2 );
							}
						 
				//     else    stat_sw1 = 1;
				//      dbgshow_sw( 1,  stat_sw1 );
				
				   //  if( GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5) == 0 )   stat_sw2 = 0;
					  // else    stat_sw2 = 1;
				   //   dbgshow_sw( 2 ,   stat_sw2 );
				
		
							
				//				USARTx_Puts(USART1, "\x1b[6;0H" );
			//				 USARTx_Puts(USART1, "Status: " );
							
						  if(  lock_status ==  0 )
							{
								    lock_status = 1;
								
						//			USARTx_Puts(USART1, "\x1b[6;0H" );
						//	    USARTx_Puts(USART1, "Status: " );
								
											GPIO_SetBits(GPIOB , GPIO_Pin_13);
										Delay(0xD1FFFF);
									GPIO_ResetBits(GPIOB , GPIO_Pin_13);
									Delay(0xD1FFFF);
							}
							if(  lock_status ==  3 )
							{
								    lock_status = 1;
								
						//			USARTx_Puts(USART1, "\x1b[6;0H" );
						//	    USARTx_Puts(USART1, "Status: " );
								
											GPIO_SetBits(GPIOB , GPIO_Pin_13);
							//			Delay(0xD7FFFF);
							//		GPIO_ResetBits(GPIOB , GPIO_Pin_13);
							//		Delay(0xD7FFFF);
							}
							
								   
			}

}


void  Value_init (void)
{
	   lock_status = 0xff;  oldstat_lock = 0xff;
	   stat_sw1 =0;  oldstat_sw1 =1; 
	   stat_sw2 =0;  oldstat_sw2 =1; 
	
	   dbgshow_sw( 1, stat_sw1);
	   dbgshow_sw( 2, stat_sw2);
}

void dbgshow_sw( int sw,  int stat )
{
	   char TxBuf_sw1[] = " SW-1 ";
     char TxBuf_sw2[] = " SW-2 ";
	   char TxBuf_unlock[] = " UNLOCK ";
	   char TxBuf_locked[] = " LOCKED ";
	   char TxBuf_reset[] = " RESET  ";
	
					if(  (stat_sw1 !=oldstat_sw1)  &&   sw == 1 )    // sw-1 status is change!
					{
								USARTx_Puts(USART1, "\x1b[2;0H" );
								USARTx_Puts(USART1, TxBuf_sw1 );
								
										 if(  stat  == 0 )   USARTx_Puts(USART1,  "0");
										 else   USARTx_Puts(USART1,  "1");
						   oldstat_sw1 = stat_sw1;
					}
						if( (stat_sw2 !=oldstat_sw2)  &&    sw == 2 )   // sw-2 status is change!
					{
								USARTx_Puts(USART1, "\x1b[4;0H" );
								USARTx_Puts(USART1, TxBuf_sw2 );
								
										 if(  stat  == 0 ) {
										 USARTx_Puts(USART1,  "0");  
											 
											 Delay(0x2FFFFF);
										 GPIO_ResetBits(GPIOB , GPIO_Pin_13); 
								//		 lock_status = 0; 
											 
										 }
										 else   USARTx_Puts(USART1,  "1");
						   oldstat_sw2 = stat_sw2;
					}
					
					if(  ( lock_status !=  oldstat_lock )  &&   sw == 3 )    // sw-3  ( lock status) is change!
					{
						    	USARTx_Puts(USART1, "\x1b[6;0H" );
						      USARTx_Puts(USART1, "Status: ");
								
										 if(  stat  == 0 )   USARTx_Puts(USART1, TxBuf_unlock);
										 else if(  stat  == 1 )   USARTx_Puts(USART1,  TxBuf_locked);
										 else if(  stat  == 2 )   USARTx_Puts(USART1,  TxBuf_reset);						
						    oldstat_lock = lock_status;
					}
						
}
//=========================================
#define MAX_STRLEN 100
char buff [MAX_STRLEN] = "";
char received_string [MAX_STRLEN] = "";
int  cnt = 0;

char * My_Usart2_ReadLine()
{    
/*
	char c = USART_ReceiveData(USART2);        
							char tmp[3];
							tmp[0] = c; tmp[1] = 0;
						 USARTx_Puts( USART1, tmp );
return (char *)0;
*/
	
	if( cnt == 0 )
			{
				   memset( received_string, 0, MAX_STRLEN );
			}
			
			
     if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET){     
			
					char c = USART_ReceiveData(USART2);        
			
		
			 
					 if(c != '\r' && c != '\n' && (cnt < MAX_STRLEN) ){

							received_string[cnt] = c;
							cnt++;
					 }
						else{ 
							         received_string[cnt] = '\0';
											
							       
							         cnt = 0;
											 // printf("UART Message: %c", received_string);
											 return received_string; 
						}
				}
		return "";
	
}

char time[100];
char latiplace1[100];
char latiplace2[100];


void GPS_analysis( char *buf)
{

	   int  res, i, num, gps, tt , ss, mm;
	  float latitude;
     char str[100];

	
	res = strlen( buf );
	
	//USARTx_Puts( USART1, buf );
	
	   if((buf[3]=='G')&&(buf[4]=='G')&&(buf[5]=='A')) //
		{
			USARTx_Puts( USART1, " GGA  " );
			num=0;
			for(i=0;i < res;i++) 
			{
				if(buf[i]!=',') { continue; } //
				switch(num){

					case 6:
						//
						if(buf[i+1]=='0') { //
							USARTx_Puts( USART1, " GPA isn't true!! \n " );
						//	printf("GPA isn't true!! \n"); 
							gps=0; // 
							break; //
						} //if 
				} //switch

			} //for
	
	num=0; 
	if(gps!=0){ 
		
		  USARTx_Puts( USART1, " GPS OK  " );
		
		for(i=0;i<res;i++) {
	
			if(buf[i]!=',') {continue;} 
			else {num++;} 
			switch(num) {
	
			   case 1://
				time[1]=buf[i+2];
				time[2]='\0';
				tt=atoi(time)+8; //
				time[1]= buf[i+4];
				time[2]='\0';
			 mm= atoi(time); //
				time[0]= buf[i+5];
				time[1]= buf[i+6];
				time[2]='\0';
				ss= atoi(time); //
				break;
			case 2://
				 latiplace1[0] = buf[i+1];//
				 latiplace1[1] = buf[i+2];//
				 latiplace1[2] = '\0'
				 latiplace2[0] = buf[i+3];//
				 latiplace2[1] = buf[i+4];//
				 latiplace2[2] = buf[i+5];//buf[i+5] .
				 latiplace2[3] = buf[i+6];//
				 latiplace2[4] = buf[i+7];//
				 latiplace2[5] = buf[i+8];//
				 latiplace2[6] = buf[i+9];//
				 latiplace2[7] = '\0';
				 latitude = atof(latiplace1) + atof(latiplace2)/60;
				 break;
			} //switch
		} //for(i=0;i<res;i++)
	}//if(gps!=0)
	
	
	
  } //if((buf[3]=='G')&&(buf[4]=='G')&&(buf[5]=='A'))

	
}

void USART2_IRQHandler()   // connect to ESP8266
{

	char *data;
	
		//USARTx_Puts( USART1, "." );
    // 
    data = My_Usart2_ReadLine();
  // USARTx_Puts(USART1,data );    //debug
		
 //		GPS_analysis( data);
		
		#if 1
   
    if(!strcmp(data, "unlock")){       
     //    USARTx_Puts(USART1, " unlock! " );
		   	lock_status = 0; 
			   dbgshow_sw( 3,  lock_status );
    } 
		 else if(!strcmp(data, "locked")){  
		     lock_status = 1; 
			   dbgshow_sw( 3,  lock_status );
		 }
		 else if(!strcmp(data, "reset")){  
		     lock_status =2; 
			   dbgshow_sw( 3,  lock_status );
		 }
		#endif
}

//=========================================

void USARTx_Puts( USART_TypeDef* USARTx,  char *str)
{
   while(*str)
    {
       USART_SendData(USARTx, *str++);
			//    GPIO_SetBits(GPIOB,GPIO_Pin_5);  //
       while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
			//    GPIO_ResetBits(GPIOB,GPIO_Pin_5);
    }
}

void GPIOB_Init(void) {
	GPIO_InitTypeDef GPIO_InitStructure; //
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //


	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_13; //
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP; //l
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; //
	GPIO_Init(GPIOB, &GPIO_InitStructure); //
}

void GPIOC_Init(void) {
	GPIO_InitTypeDef GPIO_InitStructure; //
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); //

	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_4; //
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU; //
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; //
	GPIO_Init(GPIOC, &GPIO_InitStructure); //
	

}


void Delay(uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}



//=========================================
void EXTI4_IRQHandler()     // Trigger by Rising;
{

    //  USARTx_Puts(USART1, "  IRQ-4 " );
	
	     /* Clear the EXTI line 4 pending bit */
        EXTI_ClearITPendingBit(EXTI_Line4);

      					 if( GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_4) == 1 )
								 {									 stat_sw1 = 1;
				     //else    stat_sw1 = 1;
				      dbgshow_sw( 1,  stat_sw1 );	
								 }
}

void EXTI9_5_IRQHandler()   //Trigger by Falling;
{
	
	   if(EXTI_GetITStatus(EXTI_Line5) != RESET)
    {
	     // USARTx_Puts(USART1, "  IRQ-5 " );
	
			if( GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5) == 0 )
				{									 stat_sw2 = 0;
				     //else    stat_sw1 = 1;
				      dbgshow_sw( 2,  stat_sw2 );	
				}
	     /* Clear the EXTI line 5 pending bit */
        EXTI_ClearITPendingBit(EXTI_Line5);		 
								 
		}
	
}




