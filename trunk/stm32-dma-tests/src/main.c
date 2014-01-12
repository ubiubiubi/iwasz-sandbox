#include <stm32f4xx.h>
#include <stdio.h>
#include "logf.h"

/**
 * For printf.
 */
void initUsart (void)
{
#define USE_USART 1
#define USE_PIN_SET 1

#if USE_USART == 1
#define USART USART1
#define RCC_USART RCC_APB2Periph_USART1
#define RCC_APBxPeriphClockCmd RCC_APB2PeriphClockCmd
#define GPIO_AF_USART GPIO_AF_USART1

#if USE_PIN_SET == 1
#define RCC_GPIO RCC_AHB1Periph_GPIOB
#define PORT GPIOB
#define TX_PIN_SOURCE GPIO_PinSource6
#define RX_PIN_SOURCE GPIO_PinSource7
#define TX_PIN GPIO_Pin_6
#define RX_PIN GPIO_Pin_7
#endif

#if USE_PIN_SET == 2
#define RCC_GPIO RCC_AHB1Periph_GPIOA
#define PORT GPIOA
#define TX_PIN_SOURCE GPIO_PinSource9
#define RX_PIN_SOURCE GPIO_PinSource10
#define TX_PIN GPIO_Pin_9
#define RX_PIN GPIO_Pin_10
#endif
#endif

#if USE_USART == 2
#define USART USART2
#define RCC_USART RCC_APB1Periph_USART2
#define RCC_APBxPeriphClockCmd RCC_APB1PeriphClockCmd
#define GPIO_AF_USART GPIO_AF_USART2

#if USE_PIN_SET == 1
#define RCC_GPIO RCC_AHB1Periph_GPIOA
#define PORT GPIOA
#define TX_PIN_SOURCE GPIO_PinSource2
#define RX_PIN_SOURCE GPIO_PinSource3
#define TX_PIN GPIO_Pin_2
#define RX_PIN GPIO_Pin_3
#endif

#if USE_PIN_SET == 2
#define RCC_GPIO RCC_AHB1Periph_GPIOA
#define PORT GPIOA
#define TX_PIN_SOURCE GPIO_PinSource9
#define RX_PIN_SOURCE GPIO_PinSource10
#define TX_PIN GPIO_Pin_9
#define RX_PIN GPIO_Pin_10
#endif
#endif

        RCC_APBxPeriphClockCmd (RCC_USART, ENABLE);
        GPIO_InitTypeDef gpioInitStruct;

        RCC_AHB1PeriphClockCmd (RCC_GPIO, ENABLE);
        gpioInitStruct.GPIO_Pin = TX_PIN | RX_PIN;
        gpioInitStruct.GPIO_Mode = GPIO_Mode_AF;
        gpioInitStruct.GPIO_Speed = GPIO_High_Speed;
        gpioInitStruct.GPIO_OType = GPIO_OType_PP;
        gpioInitStruct.GPIO_PuPd = GPIO_PuPd_UP;
        GPIO_Init (PORT, &gpioInitStruct);
        GPIO_PinAFConfig (PORT, TX_PIN_SOURCE, GPIO_AF_USART); // TX
        GPIO_PinAFConfig (PORT, RX_PIN_SOURCE, GPIO_AF_USART); // RX

        USART_InitTypeDef usartInitStruct;
        usartInitStruct.USART_BaudRate = 9600;
        usartInitStruct.USART_WordLength = USART_WordLength_8b;
        usartInitStruct.USART_StopBits = USART_StopBits_1;
        usartInitStruct.USART_Parity = USART_Parity_No;
        usartInitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
        usartInitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
        USART_Init (USART, &usartInitStruct);
        USART_Cmd (USART, ENABLE);
}

uint8_t dummyStrlen (char const *s)
{
        uint8_t len = 0;

        while (*s++) {
                ++len;
        }

        return len;
}

int main (void)
{
        /* This would be a function parameter or something like that. */
        char *outputBuffer = "Ala ma kota, a kot ma ale, to jest taki wierszyk z czytanki dla dzieci, ktora jest tylko w Polsce.\r\n";

        /*
         * Enable the peripheral clock for DMA2. I want to use DMA with USART1, so according to
         * table 44 in reference manual for STM32F407 (RM0090) this would be DMA2 peripheral.
         * Description in stm32f4xx_dma.c advises to do this as the first operarion.
         */
        RCC_AHB1PeriphResetCmd (RCC_AHB1Periph_DMA2, ENABLE);

/*--------------------------------------------------------------------------*/

        /*
         * Enable the USART1 device as usual.
         */
        initUsart ();
        logf("Init\r\n");

/*--------------------------------------------------------------------------*/

        /* Reset DMA Stream registers (for debug purpose). For DMA2_Stream7 exmplanation read on. */
        DMA_DeInit (DMA2_Stream7);

        /*
         * Check if the DMA Stream is disabled before enabling it.
         * Note that this step is useful when the same Stream is used multiple times:
         * enabled, then disabled then re-enabled... In this case, the DMA Stream disable
         * will be effective only at the end of the ongoing data transfer and it will
         * not be possible to re-configure it before making sure that the Enable bit
         * has been cleared by hardware. If the Stream is used only once, this step might
         * be bypassed.
         */
        while (DMA_GetCmdStatus (DMA2_Stream7) != DISABLE) {
        }

        /* Configure the DMA stream. */
        DMA_InitTypeDef  dmaInitStructure;

        /*
         * Possible values for DMA_Channel are DMA_Channel_[0..7]. Refer to table 44 in reference manual
         * mentioned earlier. USART1_RX is communicate with DMA via streams 2 and 5 (both on channel 4).
         * USART1_TX uses stream7 / channel 4.
         */
        dmaInitStructure.DMA_Channel = DMA_Channel_4;

        /* Possible values : DMA_DIR_PeripheralToMemory, DMA_DIR_MemoryToPeripheral, DMA_DIR_MemoryToMemory. */
        dmaInitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;

        /* Why DMA_PeripheralBaseAddr is of type uint32_t? Shouldn't it be void *? */
        dmaInitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(USART1->DR);
        dmaInitStructure.DMA_Memory0BaseAddr = (uint32_t)outputBuffer;

        /* Only valid values here are : DMA_PeripheralDataSize_Byte, DMA_PeripheralDataSize_HalfWord, DMA_PeripheralDataSize_Word */
        dmaInitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;

        /*
         * I guess, that for memory is is always good to use DMA_MemoryDataSize_Word (32bits), since this is
         * a 32 bit micro. But haven't checked that. But here I use Byte instead for easier  DMA_BufferSize
         * calculations.
         */
        dmaInitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;

        /*
         * Length of the data to be transferred by the DMA. Unit of this length is DMA_MemoryDataSize when
         * direction is from memory to peripheral or DMA_PeripheralDataSize otherwise. Since I set both sizes
         * to one byte, I simply put strlen here.
         */
        dmaInitStructure.DMA_BufferSize = dummyStrlen (outputBuffer);

        /*
         * DMA_PeripheralInc_Disable means to read or to write to the same location everytime. DMA_MemoryInc_Enable would
         * increase memory or peripheral location after each read/write.
         */
        dmaInitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
        dmaInitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;

        /* DMA_Mode_Normal or DMA_Mode_Circular here. */
        dmaInitStructure.DMA_Mode = DMA_Mode_Normal;

        /* DMA_Priority_Low, DMA_Priority_Medium, DMA_Priority_High or DMA_Priority_VeryHigh */
        dmaInitStructure.DMA_Priority = DMA_Priority_High;

        /* DMA_FIFOMode_Disable means direst mode, DMA_FIFOMode_Enable means FIFO mode. FIFO is good. */
        dmaInitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;


        /*
         * DMA_FIFOThreshold_1QuarterFull, DMA_FIFOThreshold_HalfFull, DMA_FIFOThreshold_3QuartersFull or
         * DMA_FIFOThreshold_Full.
         */
        dmaInitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;

        /*
         * Specifies whether to use single or busrt mode. If burst, then it specifies how much "beats" to use.
         * DMA_MemoryBurst_Single, DMA_MemoryBurst_INC4, DMA_MemoryBurst_INC8 or DMA_MemoryBurst_INC16.
         */
        dmaInitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
        dmaInitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;

        /* Configure DMA, but still leave it turned off. */
        DMA_Init (DMA2_Stream7, &dmaInitStructure);

/*--------------------------------------------------------------------------*/

        /* Enable the DMA Stream. */
        DMA_Cmd (DMA2_Stream7, ENABLE);


        /*
         * And check if the DMA Stream has been effectively enabled.
         * The DMA Stream Enable bit is cleared immediately by hardware if there is an
         * error in the configuration parameters and the transfer is no started (ie. when
         * wrong FIFO threshold is configured ...)
         */
        uint16_t timeout = 10000;
        while ((DMA_GetCmdStatus (DMA2_Stream7) != ENABLE) && (timeout-- > 0)) {
        }

        /* Check if a timeout condition occurred */
        if (timeout == 0) {
                /* Manage the error: to simplify the code enter an infinite loop */
                while (1) {
                }
        }

        /*
         * The DMA stream is turned on now and waits for DMA requests. As far as I know, if this
         * were to be memory-to-memory transfer, it would start immedialtely without enabling any
         * channels. But for peripherals one has to enable the channel for requests. After following
         * statement, you should see data on serial console.
         */
        USART_DMACmd (USART1, USART_DMAReq_Tx, ENABLE);

        /* Waiting the end of Data transfer */
        while (USART_GetFlagStatus (USART1, USART_FLAG_TC) == RESET)
                ;

        logf("1\r\n");

        while (DMA_GetFlagStatus (DMA2_Stream7, DMA_FLAG_TCIF7) == RESET)
                ;

        logf("2\r\n");

        /* Clear DMA Transfer Complete Flags */
        DMA_ClearFlag (DMA2_Stream7, DMA_FLAG_TCIF7);

        logf("3\r\n");

        /* Clear USART Transfer Complete Flags */
        USART_ClearFlag (USART1, USART_FLAG_TC);

        logf("It worked, and didn't hanged\r\n");

        /* Infinite loop */
        while (1) {
        }
}
