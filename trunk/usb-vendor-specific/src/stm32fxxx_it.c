#include "stm32fxxx_it.h"
#include "usb_core.h"
#include "usbd_core.h"
#include "usbd_vendor_class.h"
#include "usb_conf.h"
#include <stdio.h>

__IO uint32_t remote_wakeup = 0;
static int16_t angle = 0;
static int8_t lookup[] = {0, -1, +1, 0, +1, 0, 0, -1, -1, 0, 0, +1, 0, +1, -1, 0};
static uint8_t oldAb = 0x00;

/* Private function prototypes -----------------------------------------------*/
extern USB_OTG_CORE_HANDLE USB_OTG_dev;
extern uint32_t USBD_OTG_ISR_Handler (USB_OTG_CORE_HANDLE *pdev);

#ifdef USB_OTG_HS_DEDICATED_EP1_ENABLED 
extern uint32_t USBD_OTG_EP1IN_ISR_Handler (USB_OTG_CORE_HANDLE *pdev);
extern uint32_t USBD_OTG_EP1OUT_ISR_Handler (USB_OTG_CORE_HANDLE *pdev);
#endif

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
        printf ("HardFault_Handler\r\n");

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
        static uint8_t myBuf[4] = { 0x00, 0x00, 0x00, 0x00 };
        myBuf[0] = angle;
        myBuf[1] = angle >> 8;
        vendorSendReport (&USB_OTG_dev, myBuf, 4);
//        printf ("Angle %d!!!\r\n", angle);
}

/**
 * @brief  This function handles EXTI0_IRQ Handler.
 * @param  None
 * @retval None
 */
void EXTI0_IRQHandler (void)
{
        EXTI_ClearITPendingBit (EXTI_Line0);
        uint8_t gpiod = GPIOD->IDR & 0x03;
        uint8_t ab = ((gpiod & 0x01) << 1) | ((gpiod & 0x02) >> 1);
        uint8_t sum = oldAb << 2 | ab;
        int8_t l = lookup[sum];
        angle += l;
        oldAb = ab;
//        printf ("gpiod = %d, sum = %d, lookup = %d, angle = %d\r\n", gpiod, sum, l, angle);
}

void EXTI1_IRQHandler (void)
{
        EXTI_ClearITPendingBit (EXTI_Line1);
        uint8_t gpiod = GPIOD->IDR & 0x03;
        uint8_t ab = ((gpiod & 0x01) << 1) | ((gpiod & 0x02) >> 1);
        uint8_t sum = oldAb << 2 | ab;
        int8_t l = lookup[sum];
        angle += l;
        oldAb = ab;
//        printf ("gpiod = %d, sum = %d, lookup = %d, angle = %d\r\n", gpiod, sum, l, angle);
}

/**
 * @brief  This function handles EXTI15_10_IRQ Handler.
 * @param  None
 * @retval None
 */
void OTG_FS_WKUP_IRQHandler (void)
{
        if (USB_OTG_dev.cfg.low_power) {
                /* Reset SLEEPDEEP and SLEEPONEXIT bits */
                SCB ->SCR &= (uint32_t) ~((uint32_t) (SCB_SCR_SLEEPDEEP_Msk | SCB_SCR_SLEEPONEXIT_Msk));

                /* After wake-up from sleep mode, reconfigure the system clock */
                SystemInit ();
                USB_OTG_UngateClock (&USB_OTG_dev);
        }
        EXTI_ClearITPendingBit (EXTI_Line18);
}

/**
 * @brief  This function handles OTG_HS Handler.
 * @param  None
 * @retval None
 */
void OTG_FS_IRQHandler (void)
{
        USBD_OTG_ISR_Handler (&USB_OTG_dev);
}

