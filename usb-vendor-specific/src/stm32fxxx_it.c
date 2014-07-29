#include "stm32fxxx_it.h"
#include "usb_core.h"
#include "usbd_core.h"
#include "usbd_vendor_class.h"
#include "usb_conf.h"
#include <stdio.h>

#define SENSORS_NUMBER 32
__IO uint32_t remote_wakeup = 0;
int16_t angle[SENSORS_NUMBER] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
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

uint32_t chunkC = 0;
uint32_t chunkD = 0;
uint32_t chunkE = 0;
uint32_t chunkF = 0;

//#define ADD_MASK 0x66666666
#define ADD1 0x22222222
#define ADD2 0x44444444
#define ADD3 0xbbbbbbbb
#define ADD4 0xdddddddd

#define SUB1 0x11111111
#define SUB2 0x77777777
#define SUB3 0x88888888
#define SUB4 0xeeeeeeee

#if 0
#define ADJUST(chunk,angleNo,shift) \
        angleCopy = angle[angleNo]; \
        chunkShifted = chunk & (0xf << shift); \
        angleCopy += !(chunkShifted ^ (0x2 << shift)) + !(chunkShifted ^ (0x4 << shift)) + !(chunkShifted ^ (0xb << shift)) + !(chunkShifted ^ (0xd << shift)); \
        angleCopy -= !(chunkShifted ^ (0x1 << shift)) + !(chunkShifted ^ (0x7 << shift)) + !(chunkShifted ^ (0x8 << shift)) + !(chunkShifted ^ (0xe << shift)); \
        angle[angleNo] = angleCopy;


void SysTick_Handler (void)
{
        uint16_t gpioc = GPIOC->IDR;
        uint16_t gpiod = GPIOD->IDR;
        uint16_t gpioe = GPIOE->IDR;
        uint16_t gpiof = GPIOF->IDR;
        int16_t angleCopy;
        uint32_t chunkShifted;

        chunkC <<= 2;
        chunkC &= 0xcccccccc;                                              // Clear 0b 1100110011001100
        chunkC |= ((gpioc & 0x3333) << 16) | ((gpioc & 0xcccc) >> 2) ;      // 0b 0011001100110011
//        abC |= __PKHBT ((gpioc & 0xcccc) >> 2, gpioc & 0x3333, 16);

//        angleCopy = angle[7];
//        chunkShifted = chunkC & 0xf0000000;
//        angleCopy += !(chunkShifted ^ 0x20000000) + !(chunkShifted ^ 0x40000000) + !(chunkShifted ^ 0xb0000000) + !(chunkShifted ^ 0xd0000000);
//        angleCopy -= !(chunkShifted ^ 0x10000000) + !(chunkShifted ^ 0x70000000) + !(chunkShifted ^ 0x80000000) + !(chunkShifted ^ 0xe0000000);
//        angle[7] = angleCopy;
        ADJUST (chunkC, 0, 0);
        ADJUST (chunkC, 1, 1 * 4);
        ADJUST (chunkC, 2, 2 * 4);
        ADJUST (chunkC, 3, 3 * 4);
        ADJUST (chunkC, 4, 4 * 4);
        ADJUST (chunkC, 5, 5 * 4);
        ADJUST (chunkC, 6, 6 * 4);
        ADJUST (chunkC, 7, 7 * 4);

        chunkD <<= 2;
        chunkD &= 0xcccccccc;
        chunkD |= ((gpiod & 0x3333) << 16) | ((gpiod & 0xcccc) >> 2);
        ADJUST (chunkD, 8, 0);
        ADJUST (chunkD, 9, 1 * 4);
        ADJUST (chunkD, 10, 2 * 4);
        ADJUST (chunkD, 11, 3 * 4);
        ADJUST (chunkD, 12, 4 * 4);
        ADJUST (chunkD, 13, 5 * 4);
        ADJUST (chunkD, 14, 6 * 4);
        ADJUST (chunkD, 15, 7 * 4);

        chunkE <<= 2;
        chunkE &= 0xcccccccc;
        chunkE |= ((gpioe & 0x3333) << 16) | ((gpioe & 0xcccc) >> 2);
        ADJUST (chunkE, 16, 0);
        ADJUST (chunkE, 17, 1 * 4);
        ADJUST (chunkE, 18, 2 * 4);
        ADJUST (chunkE, 19, 3 * 4);
        ADJUST (chunkE, 20, 4 * 4);
        ADJUST (chunkE, 21, 5 * 4);
        ADJUST (chunkE, 22, 6 * 4);
        ADJUST (chunkE, 23, 7 * 4);

        chunkF <<= 2;
        chunkF &= 0xcccccccc;
        chunkF |= ((gpiof & 0x3333) << 16) | ((gpiof & 0xcccc) >> 2);
        ADJUST (chunkF, 24, 0);
        ADJUST (chunkF, 25, 1 * 4);
        ADJUST (chunkF, 26, 2 * 4);
        ADJUST (chunkF, 27, 3 * 4);
        ADJUST (chunkF, 28, 4 * 4);
        ADJUST (chunkF, 29, 5 * 4);
        ADJUST (chunkF, 30, 6 * 4);
        ADJUST (chunkF, 31, 7 * 4);

/*

0, -1, +1, 0, +1, 0, 0, -1,   -1, 0, 0, +1, 0, +1, -1, 0        0-15

addWord 0b 0010100000010100
subWord 0b 0100000110000010


        angle[3] += lookup[(abCe & 0xf000) >> 12];

x - 4bit

0000
0001
0010
0011
0100
0101
0110
0111
1000
1001
1010
1011
1100
1101
1110
1111


// źle m, bołapie 0110 (x & 0x6 == x) || (~x & 0x6 == ~x)
2, 4, 11, 13

add       setBitSum   c ^ 2
  0  0000 0  0000    0000 0010
  1  0001 1  1000    0000 0011
# 2  0010 1  0001    0000 0000
  3  0011 2  1001    0000 0001
# 4  0100 1  0010    0000 0110
  5  0101 2  1010    0000
  6  0110 2  0110    0000
  7  0111 3  0111    0000
  8  1000 1  1000    0000
  9  1001 2  1001    0000
  10 1010 2  1010    0000
# 11 1011 3  1011    0000
  12 1100 2  1100    0000
# 13 1101 3  1101    0000
  14 1110 3  1110    0000
  15 1111 4  1111    0000

 ?
 ?
 ?    1
 ?
 ?    1
 ?
 ?
 ?
 ?
 ?
 ?
 ?    1
 ?
 ?    1
 ?
 ?


angle += (x & addMask) | (~x & addMask)

sub
0000 & 0000 = 0
0001 & 0001 = 1
0010 & 0000 = 0
0011 & 0000 = 0
0100 & 0000 = 0
0101 & 0000 = 0
0110 & 0000 = 0
0111 & 0000 = 1
1000 & 1000 = 1
1001 & 0000 = 0
1010 & 0000 = 0
1011 & 0000 = 0
1100 & 0000 = 0
1101 & 0000 = 0
1110 & 0000 = 1
1111 & 0000 = 0

gpioC = AB 00 AB 00 AB 00 AB 00

pa0 pb0 pa1 pb1 | pa0 pb0 pa1 pb1

 */
}
#endif


#if 0
/**
 * @brief  This function handles SysTick Handler.
 * @param  None
 * @retval None
 */
#define ADD_MASK 0x06
#define SUB_MASK 0x09

#define CORRECTION(ab,mask,port,shift)                                  \
        x = (ab & mask) >> shift;                                          \
        angle[port] += ((x & ADD_MASK) | (~x & ADD_MASK)) != 0;   \
        angle[port] -= ((x & SUB_MASK) | (~x & SUB_MASK)) != 0;

void SysTick_Handler (void)
{
        uint16_t gpioc = GPIOC->IDR;
        uint16_t gpiod = GPIOD->IDR;
        uint16_t gpioe = GPIOE->IDR;
        uint16_t gpiof = GPIOF->IDR;

        register uint16_t x;

        abCe <<= 2;
        abCe &= 0xcccc;            // 0b 1100110011001100
        abCe |= (gpioc & 0x3333);  // 0b 0011001100110011
        CORRECTION (abCe, 0x000f, 0, 0);
        CORRECTION (abCe, 0x00f0, 2, 4);
        CORRECTION (abCe, 0x0f00, 4, 8);
        CORRECTION (abCe, 0xf000, 8, 12);

        abCo <<= 2;
        abCo &= 0xcccc;
        abCo |= ((gpioc & 0xcccc) >> 2);
        CORRECTION (abCo, 0x000f, 1, 0);
        CORRECTION (abCo, 0x00f0, 3, 4);
        CORRECTION (abCo, 0x0f00, 5, 8);
        CORRECTION (abCo, 0xf000, 7, 12);




        abDe <<= 2;
        abDe &= 0xcccc;
        abDe |= (gpiod & 0x3333);
        CORRECTION (abDe, 0x000f, 8, 0);
        CORRECTION (abDe, 0x00f0, 10, 4);
        CORRECTION (abDe, 0x0f00, 12, 8);
        CORRECTION (abDe, 0xf000, 14, 12);

        abDo <<= 2;
        abDo &= 0xcccc;
        abDo |= (gpiod & 0xcccc) >> 2;
        CORRECTION (abDo, 0x000f, 9, 0);
        CORRECTION (abDo, 0x00f0, 11, 4);
        CORRECTION (abDo, 0x0f00, 13, 8);
        CORRECTION (abDo, 0xf000, 15, 12);

        abEe <<= 2;
        abEe &= 0xcccc;
        abEe |= (gpioe & 0x3333);
        CORRECTION (abDe, 0x000f, 16, 0);
        CORRECTION (abDe, 0x00f0, 18, 4);
        CORRECTION (abDe, 0x0f00, 20, 8);
        CORRECTION (abDe, 0xf000, 22, 12);

        abEo <<= 2;
        abEo &= 0xcccc;
        abEo |= (gpioe & 0xcccc) >> 2;
        CORRECTION (abEo, 0x000f, 17, 0);
        CORRECTION (abEo, 0x00f0, 19, 4);
        CORRECTION (abEo, 0x0f00, 21, 8);
        CORRECTION (abEo, 0xf000, 23, 12);

        abFe <<= 2;
        abFe &= 0xcccc;
        abFe |= (gpiof & 0x3333);
        CORRECTION (abFe, 0x000f, 24, 0);
        CORRECTION (abFe, 0x00f0, 26, 4);
        CORRECTION (abFe, 0x0f00, 28, 8);
        CORRECTION (abFe, 0xf000, 30, 12);

        abFo <<= 2;
        abFo &= 0xcccc;
        abFo |= (gpiof & 0xcccc) >> 2;
        CORRECTION (abFo, 0x000f, 25, 0);
        CORRECTION (abFo, 0x00f0, 27, 4);
        CORRECTION (abFo, 0x0f00, 29, 8);
        CORRECTION (abFo, 0xf000, 31, 12);
}
#endif


#if 1
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
        abDe |= (gpiod & 0x3333);
        angle[28] += lookup[abDe & 0x000f];
        angle[26] += lookup[(abDe & 0x00f0 ) >> 4];
        angle[19] += lookup[(abDe & 0x0f00) >> 8];
        angle[8] += lookup[(abDe & 0xf000) >> 12];

        abDo <<= 2;
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
        abFe |= (gpiof & 0x3333);
        angle[10] += lookup[abFe & 0x000f];
        angle[12] += lookup[(abFe & 0x00f0) >> 4];
        angle[14] += lookup[(abFe & 0x0f00) >> 8];
        angle[33] += lookup[(abFe & 0xf000) >> 12];

        abFo <<= 2;
        abFo |= (gpiof & 0xcccc) >> 2;
        angle[11] += lookup[abFo & 0x000f];
        angle[13] += lookup[(abFo & 0x00f0) >> 4];
        angle[32] += lookup[(abFo & 0x0f00) >> 8];
        angle[34] += lookup[(abFo & 0xf000) >> 12];
}
#endif

#if 0
void SysTick_Handler (void)
{
        uint16_t gpioc = GPIOC->IDR;
        uint16_t gpiod = GPIOD->IDR;
        uint16_t gpioe = GPIOE->IDR;
        uint16_t gpiof = GPIOF->IDR;

        abCe <<= 2;
        abCe |= (gpioc & 0x3333);
        angle[0] += lookup[abCe & 0x000f];
        angle[2] += lookup[(abCe & 0x00f0) >> 4];
        angle[4] += lookup[(abCe & 0x0f00) >> 8];
        angle[6] += lookup[(abCe & 0xf000) >> 12];

        abCo <<= 2;
        abCo |= ((gpioc & 0xcccc) >> 2);
        angle[1] += lookup[abCo & 0x000f];
        angle[3] += lookup[(abCo & 0x00f0) >> 4];
        angle[5] += lookup[(abCo & 0x0f00) >> 8];
        angle[7] += lookup[(abCo & 0xf000) >> 12];

        abDe <<= 2;
        abDe |= (gpiod & 0x3333);
        angle[8] += lookup[abDe & 0x000f];
        angle[10] += lookup[(abDe & 0x00f0 ) >> 4];
        angle[12] += lookup[(abDe & 0x0f00) >> 8];
        angle[14] += lookup[(abDe & 0xf000) >> 12];

        abDo <<= 2;
        abDo |= (gpiod & 0xcccc) >> 2;
        angle[9] += lookup[abDo & 0x000f];
        angle[11] += lookup[(abDo & 0x00f0) >> 4];
        angle[13] += lookup[(abDo & 0x0f00) >> 8];
        angle[15] += lookup[(abDo & 0xf000) >> 12];

        abEe <<= 2;
        abEe |= (gpioe & 0x3333);
        angle[16] += lookup[abEe & 0x000f];
        angle[18] += lookup[(abEe & 0x00f0) >> 4];
        angle[20] += lookup[(abEe & 0x0f00) >> 8];
        angle[22] += lookup[(abEe & 0xf000) >> 12];

        abEo <<= 2;
        abEo |= (gpioe & 0xcccc) >> 2;
        angle[17] += lookup[abEo & 0x000f];
        angle[19] += lookup[(abEo & 0x00f0) >> 4];
        angle[21] += lookup[(abEo & 0x0f00) >> 8];
        angle[23] += lookup[(abEo & 0xf000) >> 12];

        abFe <<= 2;
        abFe |= (gpiof & 0x3333);
        angle[24] += lookup[abFe & 0x000f];
        angle[26] += lookup[(abFe & 0x00f0) >> 4];
        angle[28] += lookup[(abFe & 0x0f00) >> 8];
        angle[30] += lookup[(abFe & 0xf000) >> 12];

        abFo <<= 2;
        abFo |= (gpiof & 0xcccc) >> 2;
        angle[25] += lookup[abFo & 0x000f];
        angle[27] += lookup[(abFo & 0x00f0) >> 4];
        angle[29] += lookup[(abFo & 0x0f00) >> 8];
        angle[31] += lookup[(abFo & 0xf000) >> 12];
}
#endif

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

