#include <stm32f4xx.h>
#include <stdio.h>

/**
 * For printf.
 */
void initUsart (void)
{
        RCC_APB2PeriphClockCmd (RCC_APB2Periph_USART1, ENABLE);
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

        GPIO_InitTypeDef gpioInitStruct;

        gpioInitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
        gpioInitStruct.GPIO_Mode = GPIO_Mode_AF;
        gpioInitStruct.GPIO_Speed = GPIO_Speed_50MHz;
        gpioInitStruct.GPIO_OType = GPIO_OType_PP;
        gpioInitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
        GPIO_Init(GPIOB, &gpioInitStruct);
        GPIO_PinAFConfig (GPIOB, GPIO_PinSource6, GPIO_AF_USART1); // TX
        GPIO_PinAFConfig (GPIOB, GPIO_PinSource7, GPIO_AF_USART1); // RX

        USART_InitTypeDef usartInitStruct;
        usartInitStruct.USART_BaudRate = 9600;
        usartInitStruct.USART_WordLength = USART_WordLength_8b;
        usartInitStruct.USART_StopBits = USART_StopBits_1;
        usartInitStruct.USART_Parity = USART_Parity_No;
        usartInitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
        usartInitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
        USART_Init (USART1, &usartInitStruct);
        USART_Cmd (USART1, ENABLE);
}

int main (void)
{
        initUsart ();
}
