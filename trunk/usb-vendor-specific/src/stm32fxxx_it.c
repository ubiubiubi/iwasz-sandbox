#include "stm32fxxx_it.h"
#include "usb_core.h"
#include "usbd_core.h"
#include "usbd_vendor_class.h"
#include "usb_conf.h"
#include <stdio.h>

__IO uint32_t remote_wakeup = 0;
int16_t angle[SENSORS_NUMBER] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
static uint32_t abBe = 0x00;
static uint32_t abBo = 0x00;
static uint32_t abCe = 0x00;
static uint32_t abCo = 0x00;
static uint32_t abDe = 0x00;
static uint32_t abDo = 0x00;
static uint32_t abEe = 0x00;
static uint32_t abEo = 0x00;
static uint32_t abFe = 0x00;
static uint32_t abFo = 0x00;
static int8_t lookup[] = { 0, -1, +1, 0, +1, 0, 0, -1, -1, 0, 0, +1, 0, +1, -1, 0 };

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

void SysTick_Handler (void)
{
        uint16_t gpiob = GPIOB->IDR;
        uint16_t gpioc = GPIOC->IDR;
        uint16_t gpiod = GPIOD->IDR;
        uint16_t gpioe = GPIOE->IDR;
        uint16_t gpiof = GPIOF->IDR;

        abBe <<= 2;
        abBe &= 0xcccc;
        abBe |= (gpiob & 0x3333);
        angle[31] += lookup[abBe & 0x000f];
        angle[23] += lookup[(abBe & 0x00f0) >> 4];
        angle[21] += lookup[(abBe & 0x0f00) >> 8];
        angle[17] += lookup[(abBe & 0xf000) >> 12];

        abBo <<= 2;
        abBo &= 0xcccc;
        abBo |= ((gpiob & 0xcccc) >> 2);
        angle[24] += lookup[abBo & 0x000f];
        angle[22] += lookup[(abBo & 0x00f0) >> 4];
//        angle[] += lookup[(abBo & 0x0f00) >> 8];
        angle[18] += lookup[(abBo & 0xf000) >> 12];

        abCe <<= 2;
        abCe &= 0xcccc;
        abCe |= (gpioc & 0x3333);
        angle[16] += lookup[abCe & 0x000f];
        angle[30] += lookup[(abCe & 0x00f0) >> 4];
        angle[6] += lookup[(abCe & 0x0f00) >> 8];
        angle[3] += lookup[(abCe & 0xf000) >> 12];

        abCo <<= 2;
        abCo &= 0xcccc;
        abCo |= ((gpioc & 0xcccc) >> 2);
        angle[15] += lookup[abCo & 0x000f];
        angle[5] += lookup[(abCo & 0x00f0) >> 4];
        angle[29] += lookup[(abCo & 0x0f00) >> 8];
        angle[4] += lookup[(abCo & 0xf000) >> 12];


        abDe <<= 2;
        abDe &= 0xcccc;
        abDe |= (gpiod & 0x3333);
        angle[28] += lookup[abDe & 0x000f];
        angle[26] += lookup[(abDe & 0x00f0 ) >> 4];
        angle[19] += lookup[(abDe & 0x0f00) >> 8];
        angle[8] += lookup[(abDe & 0xf000) >> 12];

        abDo <<= 2;
        abDo &= 0xcccc;
        abDo |= (gpiod & 0xcccc) >> 2;
        angle[27] += lookup[abDo & 0x000f];
        angle[25] += lookup[(abDo & 0x00f0) >> 4];
        angle[9] += lookup[(abDo & 0x0f00) >> 8];
        angle[7] += lookup[(abDo & 0xf000) >> 12];


        abEe <<= 2;
        abEe &= 0xcccc;
        abEe |= (gpioe & 0x3333);
        angle[20] += lookup[abEe & 0x000f];
        angle[1] += lookup[(abEe & 0x00f0) >> 4];
        angle[35] += lookup[(abEe & 0x0f00) >> 8];
        angle[37] += lookup[(abEe & 0xf000) >> 12];

        abEo <<= 2;
        abEo &= 0xcccc;
        abEo |= (gpioe & 0xcccc) >> 2;
        angle[0] += lookup[abEo & 0x000f];
        angle[2] += lookup[(abEo & 0x00f0) >> 4];
        angle[36] += lookup[(abEo & 0x0f00) >> 8];
//        angle[] += lookup[(abEo & 0xf000) >> 12];


        abFe <<= 2;
        abFe &= 0xcccc;
        abFe |= (gpiof & 0x3333);
        angle[10] += lookup[abFe & 0x000f];
        angle[12] += lookup[(abFe & 0x00f0) >> 4];
        angle[14] += lookup[(abFe & 0x0f00) >> 8];
        angle[33] += lookup[(abFe & 0xf000) >> 12];

        abFo <<= 2;
        abFo &= 0xcccc;
        abFo |= (gpiof & 0xcccc) >> 2;
        angle[11] += lookup[abFo & 0x000f];
        angle[13] += lookup[(abFo & 0x00f0) >> 4];
        angle[32] += lookup[(abFo & 0x0f00) >> 8];
        angle[34] += lookup[(abFo & 0xf000) >> 12];
}

void setCountersToZero ()
{
        for (uint8_t i = 0; i < SENSORS_NUMBER; ++i) {
                angle[i] = 0;
        }

        abCe = 0x00;
        abCo = 0x00;
        abDe = 0x00;
        abDo = 0x00;
        abEe = 0x00;
        abEo = 0x00;
        abFe = 0x00;
        abFo = 0x00;
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

