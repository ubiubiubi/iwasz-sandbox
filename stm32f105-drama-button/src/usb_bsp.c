#include "usb_bsp.h"
#include "usbd_conf.h"
#include "misc.h"

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
 * @brief  USB_OTG_BSP_uDelay
 *         This function provides delay time in micro sec
 * @param  usec : Value of delay required in micro sec
 * @retval None
 */
void USB_OTG_BSP_uDelay (const uint32_t usec)
{
        uint32_t count = 0;
        const uint32_t utime = (120 * usec / 7);
        do {
                if (++count > utime) {
                        return;
                }
        } while (1);
//        usleep (usec);
}

/**
 * @brief  USB_OTG_BSP_mDelay
 *          This function provides delay time in milli sec
 * @param  msec : Value of delay required in milli sec
 * @retval None
 */
void USB_OTG_BSP_mDelay (const uint32_t msec)
{
        USB_OTG_BSP_uDelay (msec * 1000);
}


