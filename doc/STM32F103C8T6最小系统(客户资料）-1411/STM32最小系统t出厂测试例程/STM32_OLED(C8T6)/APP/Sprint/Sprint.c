/******************************************************************************************
        ;;文件名称: Sprint.c
        ;;文件概述: 打印机驱动文件
        ;;作    者: 
        ;;编写日期: 2009年04月22日
        ;;版 本 号: V1.0
        ;;变更记录:
******************************************************************************************/
 
#include "Sprint.H"

extern unsigned char SP_DATA[72];//取一个字符的缓冲区

unsigned char SprintZF[24][64];//打印一行字符的点阵数组


/*************************************************************************
 
         发送一个半字符到打印头的缓冲区
 
 **************************************************************************/
 
 void Send_OneHalfChar(unsigned int OneHalfChar)

  {
    
    unsigned int i;
    
    unsigned int j;
 
    for(i=0;i<12;i++)
    {
    
      j=OneHalfChar&0x8000;
      
      OneHalfChar=OneHalfChar<<1;
      
      if(j==0x8000)
      {
      
        PR_DIN_SET();

      }
      else
      {
      
        PR_DIN_REST();

      }
       
       
	     PR_CLK_REST();//数据传输时钟低电平接收数据
 
       PR_CLK_SET();//数据传输时钟高电平接锁定数据
	   
    }
 
  }

 
/**********************按照一个半字节模式送数并且打印***********************************************************/ 
 
 void Sprint_OneHalfHang(unsigned char *P_Hdata)//打印一行数据，建立一行数据的缓冲区
 {
   
	  unsigned char i;
	  unsigned int  j;
	  unsigned char Half_char[2];
	  
  
	  for(i=0;i<32;i++)
	  {
	    
	    Half_char[0]=*P_Hdata;
	    
	    P_Hdata++;
	    
	    Half_char[1]=*P_Hdata;
	    
	    P_Hdata++;
	  
	    j=(unsigned int )Half_char[0];
	    
	    j=j<<8;
	    
	    j=j+(unsigned int )Half_char[1];
	    
	    Send_OneHalfChar(j);
	  
	  }
	     
  
 }


void Step_Time(void)////走步延时

 {
   unsigned int i;
   for(i=0;i<MotorDly;i++);
 
 }

/***********************步进电机工作函数***********************************************/
 
unsigned int Num_Step=0;//步进电机走的步数 
 
unsigned char OneStep;//走一步的变量 

unsigned char Single_Srep;//打印单步走的一步
 
void Step_by_Step(void) 
 {
      
     while(Num_Step>0)
        {
                  
        if(Single_Srep==0)
        {
        
         if(OneStep==0) //步进电机走第一步
          {
             MOT1_SET();//MOT1=1;	 
        	   MOT2_REST();//MOT2=0;  	
        	   MOT3_SET();//MOT3=0;	
        	   MOT4_REST();//MOT4=1;
        	   OneStep=1;
			       Step_Time();//走步延时
          }
          else if(OneStep==1) //步进电机走第二步
          {
             MOT1_SET();//MOT1=1;  
             MOT2_REST();//MOT2=0;  
             MOT3_REST();//MOT3=1;	
             MOT4_SET();//MOT4=0;
             OneStep=0;
             Single_Srep=1;//切换步调
             Num_Step--; //走步计数减.
			       Step_Time();//走步延时   
          }
          
        }
        else
        {
        
          if(OneStep==0) //步进电机走第三步
          {
             MOT1_REST();//MOT1=0;  
             MOT2_SET();//MOT2=1;   
             MOT3_REST();//MOT3=1;	
             MOT4_SET();//MOT4=0;
             OneStep=1; 
			       Step_Time();//走步延时 
          }
          else if(OneStep==1) //步进电机走第四步
          {
             MOT1_REST();//MOT1=0;	
             MOT2_SET();//MOT2=1;  	
             MOT3_SET();//MOT3=0;	 
             MOT4_REST();//MOT4=1;
             OneStep=0;
             Single_Srep=0;//切换步调 
             Num_Step--; //走步计数减.
			       Step_Time();//走步延时
			    
          }
         
        }
      
      }
      //else //步进电机停止 
      {  
         MOT1_REST();//MOT1=0;  //停止后驱动引脚的状态
         MOT2_REST();//MOT2=0;
         MOT3_REST();//MOT3=0;
         MOT4_REST();//MOT4=0;

	  }


 }






/*************************************************************************************

加热一次走一步函数


***************************************************************************************/ 

void Hot_Time(void)//加热延时

 {
   unsigned int i;
   for(i=0;i<Hotup_Time;i++);
 
 }


//走一步的加热次数可以根据实际应用情况设定，例如，如果是电池供电，可以分为多次加热

 void OneHotStep(void)
 {
 
     
/***************************  分两次加热   **********************************************/

//第一次加热        
     
         STB1_SET();//加热打印数据
	       STB2_SET();//加热打印数据
	       STB3_SET();//加热打印数据
	       
	       Hot_Time();//加热延时
	 
//	       STB2_REST();//加热打印数据
//         
//	       STB3_REST();//加热打印数据


//第二次加热       
       
//         STB2_SET();//加热打印数据
//	 
//	       STB1_REST();//加热打印数据
//         
//	       STB3_REST();//加热打印数据

//         Hot_Time();//加热延时
				 
//第三次加热    
       
//         STB3_SET();//加热打印数据
//	 
//	       STB1_REST();//加热打印数据
//         
//	       STB2_REST();//加热打印数据

//         Hot_Time();//加热延时
         

//停止加热
       
       
       STB1_REST();//停止加热打印数据
         
       STB2_REST();//停止加热打印数据
       
       STB3_REST();//停止加热打印数据			 
      
     
 /***********************步进电机工作函数***********************************************/ 
	   
	    Num_Step=1; //走步
	   
	    Step_by_Step();



 }




/***********************纸张有无函数***********************************************/ 

unsigned char  Paper_Conversion(void)
{
 
 unsigned char ReadValue;

 ReadValue = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1);

 return ReadValue;
  
}




/***********************启动打印缓冲区数据***********************************************/ 

extern unsigned char Startup;  //设置启动打印标记

extern unsigned char RxBuffer[1000]; //串口接收缓冲区

extern unsigned int Num_Sprint;//要打印字节区位码的字节数


void StartSprint(void)
{

  if(Startup==0xff)					//如果检测到接收数据完成标记，则启动开始打印数据
  {

		
			__set_PRIMASK(1);//关闭总中断
		
			USART_ITConfig(USART1, USART_IT_RXNE,DISABLE); //关闭接收中断

			Startup=0;						//清除启动打印标记
			
			Sprint_Char(RxBuffer,Num_Sprint);//打印

			Num_Sprint=0;					//打印完数据后对数据进行清零

			USART_ITConfig(USART1, USART_IT_RXNE,ENABLE); //打开接收中断
		
			__set_PRIMASK(0);    //开中断
		
		
  }

}









/************************************************************************************************************

函数名:void SprintLin(void)

打印一行16个汉字;缓冲区内的点阵字符已经解析好

																								
************************************************************************************************************/


unsigned int Py=0;    //缓冲区数据的偏移

unsigned char Lin_DATA[72];//取一个字符的缓冲区

unsigned char CharDot[1536];//放置要打印一行数据的点阵数据

void SprintLin(void)

{


   unsigned char *Pchar;//指针
   
   unsigned char Cyc_H=0;//循环临时变量
   
   unsigned char Cyc_L=0;//循环临时变量
   
 
  //检测是否有纸张
         
    
   for(Cyc_L=0;Cyc_L<24;Cyc_L++)
         {

           Pchar=Lin_DATA;//指向一行要打印数据的缓冲区
           
           for(Cyc_H=0;Cyc_H<32;Cyc_H++)
           {
              
              Py=(unsigned int)Cyc_H*48+(unsigned int)Cyc_L*2;
              
              *Pchar=CharDot[Py];
              
               Pchar++;
               
              *Pchar=CharDot[Py+1];
              
               Pchar++;
              
           
           }
     
	  
	      
      Sprint_OneHalfHang(Lin_DATA);//输出打印一行字符
                     
      //Sprint_LA_REST();//输入一行数据到打印寄存器
	  
	  Sprint_LA_REST();//输入一行数据到打印寄存器
           
      Sprint_LA_SET();//输入一行数据到打印寄存器
		   

	  OneHotStep();//加热一行，并且走纸一行


//对一行数据进行重复打印，等于高度放大一倍。
	 

//		Sprint_OneHalfHang(Lin_DATA);//输出打印一行字符
//                     
//      //Sprint_LA_REST();//输入一行数据到打印寄存器
//	  
//	  Sprint_LA_REST();//输入一行数据到打印寄存器
//           
//      Sprint_LA_SET();//输入一行数据到打印寄存器
//		   

//	  OneHotStep();//加热一行，并且走纸一行

	   } 

        

}           





/**************************************************************************************
       
函数功能：打印数据处理进程 1

          将接收到的数据解析;并且将数据打印

输入参数：unsigned int Count：要处理的字符个数

返回值 ：返回的一行不满的字符个数

**************************************************************************************/ 


unsigned char QW[2];//区位码临时放置缓冲区

unsigned int Half_Char[2];//半个字符的缓冲区

unsigned int Buffer_PY=0;//读取区位码缓冲区地址偏移

unsigned int LNum=0;//点阵缓冲区计数

unsigned char SpintHalf[72];//打印半字节处理临时缓冲区

unsigned char Lin=32;//一行中打印的字符个数

 
void Sprint_Char(unsigned char *Buffer,unsigned int Count)
 
 {
    
    unsigned char Half;//取半字符的标记
    
    unsigned char Half_Cyc;//半字符处理循环
    
    unsigned char *P1_Half;//半字符处理指针
    
    unsigned char *P2_Half;//半字符处理指针

	Buffer_PY=0;//打印字符从缓冲区第二个字节开始，第一个字节为命令字

	LNum=0;     //点阵缓冲区计数

	Lin=32;	   //一行中打印的字符个数


	while(Count>0)
	{
	
	 Copy_Mem(QW,&RxBuffer[Buffer_PY],2);//从缓冲区中取两个字节的区位码
    
	 Buffer_PY++; //转到下一个要读出区位码的位置

     if((QW[0]==0x0D)|(QW[0]==0x0A))//判断当前要处理的字符是否为回车符号
      {
        
        Count--;  //转移到下一个要判断的区位码需要判断的是两字节的区位码
        
        if((QW[1]==0x0D)|(QW[1]==0x0A))
        {
        
        
          Buffer_PY++;
          
          Count--;  //转移到下一个要判断的区位码需要判断的是两字节的区位码
        
        }

	 while(Lin>0) //判断一行之内是否还有数据空数据需要填充
        {
          
         Lin--;//直到一行数据存储完成
          
         Init_Buffer(SP_DATA,48,0);//将字符的缓冲区填充成0;
           
         Copy_Char(&CharDot[LNum],SP_DATA,48);//将数据拷贝到打印点阵缓冲区

         LNum=LNum+48;//转到下一个要写入的数据
        
         if(LNum==1536) //如果装满了缓冲区;
          
          {
            
            LNum=0;
              
           SprintLin();//启动打印缓冲区数据

            if( NO_Paper==Paper_Conversion())//判断是否无纸，无纸则退出打印
               {
                     
                  Startup=0;//清除打印标记 
    
                  Count=0;//打印完成，清除打印标记

                  return;
                     
                }
              
            }
            
            
          }
        
        if(LNum!=0)
        {
          
          
          SprintLin();//启动打印缓冲区数据
          
          if( NO_Paper==Paper_Conversion())//判断是否无纸，无纸则退出打印
               {
                     
                  Startup=0;//清除打印标记 
    
                  Count=0;//打印完成，清除打印标记

                  return;
                     
                }
        
        }
        
        LNum=0;        //清除填充缓冲区指针
        
        Lin=32;
      
      }
	  
     
 
 /*******************  如果不是回车符号，是全角字符，则读取字符点阵  *****************************************/      
	        
      else if(QW[0]&0x80)//判断是否为一个字符全角
       {
          if(QW[1]&0x80) 
          {
            
            Buffer_PY++;
                        
            for(Half=0;Half<2;Half++)//如果是一个字符的话，一次取半个字符
            {
              
              Char_Addr(QW[0],QW[1]);//输入参数为汉字的区位内码
             
			  Copy_Mem(SpintHalf,SP_DATA,72); //将取出的字符放置到一个临时缓冲区中
              
              P1_Half=SpintHalf;
              
              P2_Half=SP_DATA;
              
              for(Half_Cyc=0;Half_Cyc<24;Half_Cyc++)
              {
               if(Half==0)
               {
                
                   *P2_Half=*P1_Half;
                    P2_Half++;
                    P1_Half++;
                   *P2_Half=*P1_Half;
                   P2_Half++;
                   P1_Half++;
                   P1_Half++;
                 
               }
               else
               {
                 P1_Half++;
                 
                 Half_Char[0]=(unsigned int )*P1_Half;
                 
                 Half_Char[0]=Half_Char[0]<<12;
                 
                 P1_Half++;
                 
                 Half_Char[1]=*P1_Half;
                 
                 P1_Half++;
                 
                 Half_Char[1]=(unsigned int )Half_Char[1]<<4;
                 
                 Half_Char[0]=Half_Char[0]+Half_Char[1];
                 
                 *P2_Half=(unsigned char)(Half_Char[0]>>8);
                 
                  P2_Half++;
                  
                 *P2_Half=(unsigned char)Half_Char[0]&0xff;
                 
                  P2_Half++;
                  
               }
              }
              
			  
			  Copy_Char(&CharDot[LNum],SP_DATA,48);//将数据拷贝到打印点阵缓冲区
              
              LNum=LNum+48;//转到下一个要写入的数据
              
              if(LNum==1536) //如果装满了点阵缓冲区，则启动打印;
              
              {
              	SprintLin();//启动打印缓冲区数据
				LNum=0;
                
              }
              
              
              Lin--;  //如果是24的点阵字库，则一行的汉字个数为16个
              
              if(Lin==0)
              {
                
                Lin=32;//防止溢出
                
              }
             
			  Count--;  //转移到下一个要判断的区位码需要判断的是两字节的区位码
            
              }
              
			}
        
        
	        else //显示ASC扩展字符
	        {
	           
	           
	           
	              ASC_Addr(QW[0]);//取ASCII码字符。
	           
	              
				  Copy_Mem(&CharDot[LNum],SP_DATA,48);//将数据拷贝进入要写入Flash的缓冲区
				                
	              LNum=LNum+48;//转到下一个要写入的数据
	              
	              if(LNum==1536) //如果装满了点阵缓冲区，则启动打印;
	              
	              {
	                  
	                  SprintLin();//启动打印缓冲区数据
	                  LNum=0;                 
	              
	              }
	              
	              
	              Lin--;  //如果是24的点阵字库，则一行的汉字个数为16个
	              
	              if(Lin==0)
	              {
	              
	                
	                Lin=32;//防止溢出
	                
	              }
	              
	              Count=Count-1;  //转移到下一个要判断的区位码需要判断的是两字节的区位码
	
	       	   }
	        
		}

		else   //如果不是两字节字符，则取ASC码的点阵字符
		{

		 
              ASC_Addr(QW[0]);//取ASCII码字符。
           
              Copy_Mem(&CharDot[LNum],SP_DATA,48);//将数据拷贝进入要写入Flash的缓冲区
              
              LNum=LNum+48;//转到下一个要写入的数据
              
              if(LNum==1536) //如果装满了点阵缓冲区，则启动打印;
              
              {
              
                  SprintLin();//启动打印缓冲区数据
              	  LNum=0;
              }
              
              
              Lin--;  //如果是24的点阵字库，则一行的汉字个数为16个
              
              if(Lin==0)
              {
              
                
                Lin=32;//防止溢出
                
              }
              
              Count=Count-1;  //转移到下一个要判断的区位码需要判断的是两字节的区位码


		}
     
      }

    if(Lin<32)//如果最后一行不是以回车结尾的，则将最后一行剩余的字节数填充满，并且将存储页数加1
     {
         
	   while(Lin>0) //判断一行之内是否还有数据空数据需要填充
        {
          
         Lin--;//直到一行数据存储完成
          
         Init_Buffer(SP_DATA,48,0);//将字符的缓冲区填充成0;
           
         Copy_Char(&CharDot[LNum],SP_DATA,48);//将数据拷贝到打印点阵缓冲区

         LNum=LNum+48;//转到下一个要写入的数据
        
         if(LNum==1536) //如果装满了缓冲区，则写一次Flash;
          
          {
            
            LNum=0;
              
           SprintLin();//启动打印缓冲区数据

            if( NO_Paper==Paper_Conversion())//判断是否无纸，无纸则退出打印
               {
                     
                  Startup=0;//清除打印标记 
    
                  Count=0;//打印完成，清除打印标记

                  return;
                     
                }
              
            }
            
          }
        
        if(LNum!=0)
        {
          
          
          SprintLin();//启动打印缓冲区数据
          
          if( NO_Paper==Paper_Conversion())//判断是否无纸，无纸则退出打印
               {
                     
                  Startup=0;//清除打印标记 
    
                  Count=0;//打印完成，清除打印标记

                  return;
                     
                }
        
        }
        
        LNum=0;        //清除填充缓冲区指针
        
        Lin=32;
         
         }

  }
     

