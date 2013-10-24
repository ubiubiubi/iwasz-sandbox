#include "stm32fxxx_it.h"
#include "usb_core.h"
#include "usbd_core.h"
#include "usbd_vendor_class.h"
#include "usb_conf.h"
#include <stdio.h>

__IO uint32_t remote_wakeup = 0;
int16_t angle[32];
static uint32_t abC = 0x00;
static uint32_t abD = 0x00;
static uint32_t abE = 0x00;
static uint32_t abF = 0x00;
static int8_t lookup[] = {0, -1, +1, 0, +1, 0, 0, -1, -1, 0, 0, +1, 0, +1, -1, 0};

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
        abD <<= 2;
        abD |= (GPIOD->IDR & 0x3333);
        angle[0] += lookup[abD & 0x0000000f];
        angle[1] += lookup[abD & 0x000000f0];
//        vendorSendReport (&USB_OTG_dev, (uint8_t *)angle, IN_PACKET_SIZE);

//        abC <<= 2;
//        abC |= (GPIOC->IDR & 0x3333);
//        angle[0] += lookup[abC & 0x0000000f];
//        angle[1] += lookup[abC & 0x000000f0];
//        angle[2] += lookup[abC & 0x00000f00];
//        angle[3] += lookup[abC & 0x0000f000];
//        angle[4] += lookup[abC & 0x000f0000];
//        angle[5] += lookup[abC & 0x00f00000];
//        angle[6] += lookup[abC & 0x0f000000];
//        angle[7] += lookup[abC & 0xf0000000];
//
//        abD <<= 2;
//        abD |= (GPIOD->IDR & 0x3333);
//        angle[8] += lookup[abD & 0x0000000f];
//        angle[9] += lookup[abD & 0x000000f0];
//        angle[10] += lookup[abD & 0x00000f00];
//        angle[11] += lookup[abD & 0x0000f000];

//        Do tąd daje radę @ 10kHz + debug.
//        angle[12] += lookup[abD & 0x000f0000];
//        angle[13] += lookup[abD & 0x00f00000];
//        angle[14] += lookup[abD & 0x0f000000];
//        angle[15] += lookup[abD & 0xf0000000];
//
//        abE <<= 2;
//        abE |= (GPIOE->IDR & 0x3333);
//        angle[16] += lookup[abE & 0x0000000f];
//        angle[17] += lookup[abE & 0x000000f0];
//        angle[18] += lookup[abE & 0x00000f00];
//        angle[19] += lookup[abE & 0x0000f000];
//        angle[20] += lookup[abE & 0x000f0000];
//        angle[21] += lookup[abE & 0x00f00000];
//        angle[22] += lookup[abE & 0x0f000000];
//        angle[23] += lookup[abE & 0xf0000000];
//
//        abF <<= 2;
//        abF |= (GPIOF->IDR & 0x3333);
//        angle[24] += lookup[abF & 0x0000000f];
//        angle[25] += lookup[abF & 0x000000f0];
//        angle[26] += lookup[abF & 0x00000f00];
//        angle[27] += lookup[abF & 0x0000f000];
//        angle[28] += lookup[abF & 0x000f0000];
//        angle[29] += lookup[abF & 0x00f00000];
//        angle[30] += lookup[abF & 0x0f000000];
//        angle[31] += lookup[abF & 0xf0000000];
}

/**
 * @brief  This function handles EXTI0_IRQ Handler.
 * @param  None
 * @retval None
 */
void EXTI0_IRQHandler (void)
{
        EXTI_ClearITPendingBit (EXTI_Line0);
//        ab <<= 2;
//        ab |= (GPIOD->IDR & 0x03);
//        angle += lookup[ab & 0x0f];
}

void EXTI1_IRQHandler (void)
{
        EXTI_ClearITPendingBit (EXTI_Line1);
//        ab <<= 2;
//        ab |= (GPIOD->IDR & 0x03);
//        angle += lookup[ab & 0x0f];
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

