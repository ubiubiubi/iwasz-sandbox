#include <stm32f4xx.h>
#include "stm32fxxx_it.h"
#include "logf.h"

/******************************************************************************/
/*             Cortex-M Processor Exceptions Handlers                         */
/******************************************************************************/

/**
 * @brief   This function handles NMI exception.
 * @param  None
 * @retval None
 */
void NMI_Handler (void)
{
}

/**
 * @brief  This function handles Hard Fault exception.
 * @param  None
 * @retval None
 */
void HardFault_Handler (void)
{
        logf ("HardFault_Handler\r\n");

        /* Go to infinite loop when Hard Fault exception occurs */
        while (1) {
        }
}

/**
 * @brief  This function handles Memory Manage exception.
 * @param  None
 * @retval None
 */
void MemManage_Handler (void)
{
        logf ("MemManage_Handler\r\n");

        /* Go to infinite loop when Memory Manage exception occurs */
        while (1) {
        }
}

/**
 * @brief  This function handles Bus Fault exception.
 * @param  None
 * @retval None
 */
void BusFault_Handler (void)
{
        logf ("BusFault_Handler\r\n");

        /* Go to infinite loop when Bus Fault exception occurs */
        while (1) {
        }
}

/**
 * @brief  This function handles Usage Fault exception.
 * @param  None
 * @retval None
 */
void UsageFault_Handler (void)
{
        logf ("UsageFault_Handler\r\n");

        /* Go to infinite loop when Usage Fault exception occurs */
        while (1) {
        }
}

/**
 * @brief  This function handles SVCall exception.
 * @param  None
 * @retval None
 */
void SVC_Handler (void)
{
}

/**
 * @brief  This function handles Debug Monitor exception.
 * @param  None
 * @retval None
 */
void DebugMon_Handler (void)
{
}

/**
 * @brief  This function handles PendSVC exception.
 * @param  None
 * @retval None
 */
void PendSV_Handler (void)
{
}

/**
 * @brief  This function handles SysTick Handler.
 * @param  None
 * @retval None
 */
void SysTick_Handler (void)
{
}

void DMA2_Stream7_IRQHandler (void)
{
        if (DMA2->LISR & DMA_FLAG_TCIF7) {
                logf ("Transfer complete\r\n");
                DMA_ClearFlag (DMA2_Stream7, DMA_FLAG_TCIF7 | DMA_FLAG_FEIF7);
        }
        if (DMA2->LISR & DMA_FLAG_HTIF7) {
                logf ("Half-transfer complete\r\n");
                DMA_ClearFlag (DMA2_Stream7, DMA_FLAG_HTIF7);
        }
        if (DMA2->LISR & DMA_FLAG_TEIF7) {
                logf ("Transfer error\r\n");
                DMA_ClearFlag (DMA2_Stream7, DMA_FLAG_TEIF7);
        }
        if (DMA2->LISR & DMA_FLAG_FEIF7) {
                logf ("FIFO overrun/underrun\r\n");
                DMA_ClearFlag (DMA2_Stream7, DMA_FLAG_FEIF7);
        }
        if (DMA2->LISR & DMA_FLAG_DMEIF7) {
                logf ("Direct mode error\r\n");
                DMA_ClearFlag (DMA2_Stream7, DMA_FLAG_DMEIF7);
        }
}
