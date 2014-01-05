#include <stm32f10x.h>
#include <stdio.h>

#include <usb_core.h> // OTG driver
#include <usbd_core.h> // Device library
#include <usbd_usr.h> // USBD_Usr_cb_TypeDef structure implementaion
#include "usbd_hid_core.h"// Class library
#include "usbd_desc.h"

__ALIGN_BEGIN USB_OTG_CORE_HANDLE  USB_OTG_dev __ALIGN_END;

/**
 * For printf.
 */
void initUsart (void)
{
        RCC_APB1PeriphClockCmd (RCC_APB1Periph_USART2, ENABLE);
        RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA, ENABLE);

        GPIO_InitTypeDef GPIO_InitStructure;
        GPIO_PinRemapConfig (GPIO_Remap_USART2, ENABLE);

        /* Configure USART2 Rx as input floating */
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
        GPIO_Init (GPIOA, &GPIO_InitStructure);

        /* Configure USART1 Tx as alternate function push-pull */
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_Init (GPIOA, &GPIO_InitStructure);

        USART_InitTypeDef usartInitStruct;
        usartInitStruct.USART_BaudRate = 9600;
        usartInitStruct.USART_WordLength = USART_WordLength_8b;
        usartInitStruct.USART_StopBits = USART_StopBits_1;
        usartInitStruct.USART_Parity = USART_Parity_No;
        usartInitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
        usartInitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
        USART_Init (USART2, &usartInitStruct);
        USART_Cmd (USART2, ENABLE);
}

void initTestPin (void)
{
        RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOB, ENABLE);

        GPIO_InitTypeDef gpioInitStruct;

        gpioInitStruct.GPIO_Pin = GPIO_Pin_All;
        gpioInitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
        gpioInitStruct.GPIO_Speed = GPIO_Speed_2MHz;
        GPIO_Init (GPIOB, &gpioInitStruct);
}


void uDelay (const uint32_t usec)
{

        volatile uint32_t count = 0;
        const uint32_t utime = ((SystemCoreClock/10000000) * usec / 7);
        do {
                if (++count > utime) {
                        return;
                }
        } while (1);
}

void testSequence (void)
{
        for (int i = 0; i < 5; ++i) {
                //GPIOB->ODR = 0xffff;
                /* Set */
                GPIOB->BSRR = 0x00000001;


                uDelay (1000);

//                GPIOB->ODR = 0x0000;
                /* Reset PD0 and PD2 */
                GPIOB->BRR  = 0x00000001;

                uDelay (1000);
        }
}

void initExti (void)
{
        // Konfiguracja portu jak zwykle.
        RCC_APB1PeriphClockCmd (RCC_APB2Periph_GPIOA, ENABLE);
        GPIO_InitTypeDef gpioInitStruct;
        gpioInitStruct.GPIO_Pin = GPIO_Pin_0;
        gpioInitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
        gpioInitStruct.GPIO_Speed = GPIO_Speed_2MHz;
        GPIO_Init (GPIOA, &gpioInitStruct);

#if 0
        /*
         * Podłaczenie pinu portu i linii EXTI.  EXTI_PinSource1 oznacza, pin1 (portu E) podłączony
         * będzie do linii 1.
         */
        SYSCFG_EXTILineConfig (EXTI_PortSourceGPIOD, EXTI_PinSource0);
        SYSCFG_EXTILineConfig (EXTI_PortSourceGPIOD, EXTI_PinSource1);

        // Konfiguracja linii 1 EXTI.
        EXTI_InitTypeDef extiInitStructure;
        extiInitStructure.EXTI_Line = EXTI_Line0;
        extiInitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
        extiInitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
        extiInitStructure.EXTI_LineCmd = ENABLE;
        EXTI_Init(&extiInitStructure);

        extiInitStructure.EXTI_Line = EXTI_Line1;
        extiInitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
        extiInitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
        extiInitStructure.EXTI_LineCmd = ENABLE;
        EXTI_Init(&extiInitStructure);

        // Konfiguracja NVIC. Grupa jest ustawiona w pliku usb_bsp.c na 1
        // NVIC_PriorityGroupConfig (NVIC_PriorityGroup_0);
#endif
        NVIC_InitTypeDef nvicInitStructure;
//        nvicInitStructure.NVIC_IRQChannel = EXTI0_IRQn;
//        nvicInitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//        nvicInitStructure.NVIC_IRQChannelSubPriority = 0;
//        nvicInitStructure.NVIC_IRQChannelCmd = ENABLE;
//        NVIC_Init (&nvicInitStructure);
//
//        nvicInitStructure.NVIC_IRQChannel = EXTI1_IRQn;
//        nvicInitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//        nvicInitStructure.NVIC_IRQChannelSubPriority = 0;
//        nvicInitStructure.NVIC_IRQChannelCmd = ENABLE;
//        NVIC_Init (&nvicInitStructure);

        // SysTick ma mniejszy priorytet niż GPIO.
        nvicInitStructure.NVIC_IRQChannel = SysTick_IRQn;
        nvicInitStructure.NVIC_IRQChannelPreemptionPriority = 0;
        nvicInitStructure.NVIC_IRQChannelSubPriority = 0;
        nvicInitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init (&nvicInitStructure);
}

/**
 * Configure the USB OTG engine.
 */
void initUsb (void)
{
        USBD_Init(&USB_OTG_dev,
                  USB_OTG_FS_CORE_ID,
                  &USR_desc,
                  &USBD_HID_cb,
                  &USR_cb);
}

int main (void)
{
        initUsart ();
        initTestPin ();
        testSequence();
        initUsb ();
        printf ("after initUsb\r\n");
        initExti ();
        printf ("after initExti\r\n");

        while (1) {
        }
}
