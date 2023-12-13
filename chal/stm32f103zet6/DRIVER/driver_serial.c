/**
 * @file driver_serial.c
 * @author zc (387646983@qq.com)
 * @brief 
 * @version 0.1
 * @date 2023-12-02
 * @note serial driver
 * @copyright Copyright (c) 2023
 * 
 */
#include "chal_serial.h"
#include "stm32f1xx_hal.h"
#include "chal_export.h"
#include "chal_def.h"
/* private function prototype ----------------------------------------------- */
// static int32_t _read(chal_serial_t *this,void *buffer,uint32_t size);
static int32_t _write(chal_serial_t *this,const void *buffer,uint32_t size);



typedef struct driver_serial_data
{
    chal_serial_t *device;           /**<father class*/
    const char *name;                /**<serial name*/

}drv_serial_t;



static  chal_serial_ops_t serial_ops =
{
    .read =   NULL,
    .write =  _write,
    .set_tx = NULL,
    .config = NULL
};

//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#include "stdio.h"
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
	USART1->DR = (uint8_t) ch;      
	return ch;
}
#endif

// #define RXBUFFERSIZE  1
// uint8_t aRxBuffer[RXBUFFERSIZE];//HAL库使用的串口接收缓冲
static USART_HandleTypeDef USART1_Handler;
static chal_serial_t usart1;

static drv_serial_t serial1={
.device=&usart1,
.name ="usart1"
};




void driver_uart_init(void)
{
	GPIO_InitTypeDef GPIO_Initure={0};
	


	
		__HAL_RCC_GPIOA_CLK_ENABLE();			//使能GPIOA时钟
		__HAL_RCC_USART1_CLK_ENABLE();			//使能USART1时钟
		__HAL_RCC_AFIO_CLK_ENABLE();
	
		GPIO_Initure.Pin=GPIO_PIN_9;			//PA9
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//复用推挽输出
		GPIO_Initure.Pull=GPIO_PULLUP;			//上拉
		GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;//高速
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//初始化PA9

		GPIO_Initure.Pin=GPIO_PIN_10;			//PA10
		GPIO_Initure.Mode=GPIO_MODE_AF_INPUT;	//模式要设置为复用输入模式！	
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//初始化PA10
 
		// HAL_NVIC_EnableIRQ(USART1_IRQn);				//使能USART1中断通道
		// HAL_NVIC_SetPriority(USART1_IRQn,3,3);			//抢占优先级3，子优先级3
    
        //UART 初始化设置
        USART1_Handler.Instance=USART1;					    //USART1
        USART1_Handler.Init.BaudRate=115200;				    //波特率
        USART1_Handler.Init.WordLength=UART_WORDLENGTH_8B;   //字长为8位数据格式
        USART1_Handler.Init.StopBits=UART_STOPBITS_1;	    //一个停止位
        USART1_Handler.Init.Parity=UART_PARITY_NONE;		    //无奇偶校验位
 
        USART1_Handler.Init.Mode=UART_MODE_TX;		    //收发模式
        HAL_USART_Init(&USART1_Handler);					    //HAL_UART_Init()会使能UART1
        
        //HAL_UART_Receive_IT(& UART1_Handler, (uint8_t *)serial1.rx_buffer, serial1.rx_buffersize);//该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量
    
    
//默认模式全双工，attr传入NULL参数会在chal层设置为全双工
    chal_serial_register(serial1.device,serial1.name,&serial_ops,NULL,&USART1_Handler);
}

#if CHAL_EXPORT_EN
INIT_EXPORT(driver_uart_init, EXPORT_DRVIVER);
#else
#endif

// static int32_t _read(chal_serial_t *this,void *buffer,uint32_t size)
// {
// drv_serial_t *driver_data=(drv_serial_t *)this->super.user_data;
// buffer=driver_data->rx_buffer;
// size=driver_data->rx_buffersize;

// CHAL_ERR_t ret=CHAL_OK;
// return ret;
// }

static int32_t _write(chal_serial_t *this,const void*buffer,uint32_t size)
{
USART_HandleTypeDef *driver_data=(USART_HandleTypeDef *)this->super.user_data;
HAL_StatusTypeDef HAL_Status=HAL_USART_Transmit(driver_data,(uint8_t *)buffer,size,0xffff);
CHal_Err_t ret=CHAL_ERROR;

if(HAL_Status==HAL_OK)
{
ret=CHAL_OK;

}

return ret;

}

// static CHAL_ERR_t _config(chal_serial_t *this, chal_serial_config_t *attr)
// {
// CHAL_ERR_t ret=CHAL_ERROR;
// drv_serial_t *driver_data=(drv_serial_t *)this->super.user_data;

// driver_data->UART_Handler.Init.BaudRate=attr->data_bits;
// driver_data->UART_Handler.Init.StopBits=attr->stop_bits;
// driver_data->UART_Handler.Init.Parity=attr->parity;

// }


