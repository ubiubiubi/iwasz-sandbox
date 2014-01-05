#include "usb_bsp.h"
#include "usbd_conf.h"
#include "misc.h"
#include <stm32f10x_tim.h>

#define TIM_MSEC_DELAY                     0x01
#define TIM_USEC_DELAY                     0x02

//static void USB_OTG_BSP_TimeInit (void);
//static void BSP_SetTime (uint8_t unit);

__IO uint32_t BSP_delay = 0;

/**
 * @brief  USB_OTG_BSP_Init Initilizes BSP configurations
 * @param  None
 * @retval None
 */

void USB_OTG_BSP_Init (USB_OTG_CORE_HANDLE *pdev)
{
        // Really so simple!? Only 2 lines as compared to bazzilion lines for stm32f4?
        RCC_OTGFSCLKConfig(RCC_OTGFSCLKSource_PLLVCO_Div3);
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_OTG_FS, ENABLE) ;

        /* enable the PWR clock */
        RCC_APB1PeriphResetCmd (RCC_APB1Periph_PWR, ENABLE);
//        USB_OTG_BSP_TimeInit();
}

/**
 * @brief  USB_OTG_BSP_EnableInterrupt
 *         Enabele USB Global interrupt
 * @param  None
 * @retval None
 */
void USB_OTG_BSP_EnableInterrupt (USB_OTG_CORE_HANDLE *pdev)
{
        NVIC_InitTypeDef NVIC_InitStructure;

        NVIC_PriorityGroupConfig (NVIC_PriorityGroup_1 );
        NVIC_InitStructure.NVIC_IRQChannel = OTG_FS_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init (&NVIC_InitStructure);

#ifdef USB_OTG_HS_DEDICATED_EP1_ENABLED
        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
        NVIC_InitStructure.NVIC_IRQChannel = OTG_HS_EP1_OUT_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&NVIC_InitStructure);

        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
        NVIC_InitStructure.NVIC_IRQChannel = OTG_HS_EP1_IN_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&NVIC_InitStructure);
#endif
}


/**
  * @brief  USB_OTG_BSP_TimeInit
  *         Initializes delay unit using Timer2
  * @param  None
  * @retval None
  */
//static void USB_OTG_BSP_TimeInit (void)
//{
//        NVIC_InitTypeDef NVIC_InitStructure;
//        /* Set the Vector Table base address at 0x08000000 */
//        NVIC_SetVectorTable (NVIC_VectTab_FLASH, 0x00);
//        /* Configure the Priority Group to 2 bits */
//        NVIC_PriorityGroupConfig (NVIC_PriorityGroup_2 );
//        /* Enable the TIM2 gloabal Interrupt */
//        NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
//        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
//        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
//        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//        NVIC_Init (&NVIC_InitStructure);
//        RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM2, ENABLE);
//}

/**
  * @brief  USB_OTG_BSP_TimerIRQ
  *         Time base IRQ
  * @param  None
  * @retval None
  */

//void USB_OTG_BSP_TimerIRQ (void)
//{
//        if (TIM_GetITStatus (TIM2, TIM_IT_Update) != RESET) {
//                TIM_ClearITPendingBit (TIM2, TIM_IT_Update);
//                if (BSP_delay > 0x00) {
//                        BSP_delay--;
//                }
//                else {
//                        TIM_Cmd (TIM2, DISABLE);
//                }
//        }
//}

/**
  * @brief  BSP_Delay
  *         Delay routine based on TIM2
  * @param  nTime : Delay Time.
  * @param  unit : Delay Time unit : mili sec / micro sec
  * @retval None
  */
//static void BSP_Delay (uint32_t nTime, uint8_t unit)
//{
//        BSP_delay = nTime;
//        BSP_SetTime (unit);
//        while (BSP_delay != 0)
//                ;
//        TIM_Cmd (TIM2, DISABLE);
//}

/**
  * @brief  BSP_SetTime
  *         Configures TIM2 for delay routine based on TIM2
  * @param  unit : msec /usec
  * @retval None
  */
//static void BSP_SetTime (uint8_t unit)
//{
//        TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
//
//        TIM_Cmd (TIM2, DISABLE);
//        TIM_ITConfig (TIM2, TIM_IT_Update, DISABLE);
//
//        if(unit == TIM_USEC_DELAY)
//        {
//                TIM_TimeBaseStructure.TIM_Period = 11;
//        }
//        else if(unit == TIM_MSEC_DELAY)
//        {
//                TIM_TimeBaseStructure.TIM_Period = 11999;
//        }
//        TIM_TimeBaseStructure.TIM_Prescaler = 5;
//        TIM_TimeBaseStructure.TIM_ClockDivision = 0;
//        TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
//
//        TIM_TimeBaseInit (TIM2, &TIM_TimeBaseStructure);
//        TIM_ClearITPendingBit (TIM2, TIM_IT_Update );
//        TIM_ARRPreloadConfig (TIM2, ENABLE);
//
//        /* TIM IT enable */
//        TIM_ITConfig (TIM2, TIM_IT_Update, ENABLE);
//        /* TIM2 enable counter */
//        TIM_Cmd (TIM2, ENABLE);
//}


/**
 * @brief  USB_OTG_BSP_uDelay
 *         This function provides delay time in micro sec
 * @param  usec : Value of delay required in micro sec
 * @retval None
 */
void USB_OTG_BSP_uDelay (const uint32_t usec)
{
//        BSP_Delay (usec, TIM_USEC_DELAY);

        // -O3
        volatile uint32_t count = 0;
        const uint32_t utime = ((SystemCoreClock/10000000) * usec / 7);
        do {
                if (++count > utime) {
                        return;
                }
        } while (1);
}

/**
 * @brief  USB_OTG_BSP_mDelay
 *          This function provides delay time in milli sec
 * @param  msec : Value of delay required in milli sec
 * @retval None
 */
void USB_OTG_BSP_mDelay (const uint32_t msec)
{
//        BSP_Delay (msec, TIM_MSEC_DELAY);
        USB_OTG_BSP_uDelay (msec * 1000);
}


