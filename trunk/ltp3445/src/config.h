/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef CONFIG_LTP4335_H_
#define CONFIG_LTP4335_H_

/****************************************************************************/
/* Common constants                                                         */
/****************************************************************************/

#define DELAY_COEFFICIENT_SCD 30000000

/****************************************************************************/
/* Head constants                                                           */
/****************************************************************************/

// GPIO port for head.
#define SYSCTL_PERIPH_GPIO_HEAD SYSCTL_PERIPH_GPIOB
#define GPIO_PORT_HEAD_BASE GPIO_PORTB_BASE

#define GPIO_PIN_HEAD_DST   GPIO_PIN_0 // 8  Thermal head print activation instruction signal
#define GPIO_PIN_HEAD_BCLR  GPIO_PIN_1 // 10 Block select all clear
#define GPIO_PIN_HEAD_BDAT  GPIO_PIN_2 // 11 Block select data input (serial input)
#define GPIO_PIN_HEAD_BCLK  GPIO_PIN_3 // 12 Block select data transfer synchronization signal
#define GPIO_PIN_HEAD_CTRL  GPIO_PIN_4 // 13 History control signal
#define GPIO_PIN_HEAD_HDAT  GPIO_PIN_5 // 14 Print data input (serial input)
#define GPIO_PIN_HEAD_LATCH GPIO_PIN_6 // 17 Print data latch (memory storage)
#define GPIO_PIN_HEAD_HCLK  GPIO_PIN_7 // 18 Synchronizing signal for data transfer

#define GPIO_PIN_HEAD_ALL (   \
        GPIO_PIN_HEAD_DST   | \
        GPIO_PIN_HEAD_BCLR  | \
        GPIO_PIN_HEAD_BDAT  | \
        GPIO_PIN_HEAD_BCLK  | \
        GPIO_PIN_HEAD_CTRL  | \
        GPIO_PIN_HEAD_HDAT  | \
        GPIO_PIN_HEAD_LATCH | \
        GPIO_PIN_HEAD_HCLK    \
)

#define HEAD_PIXELS_IN_LINE 832
#define HEAD_BYTES_IN_LINE (HEAD_PIXELS_IN_LINE / 8)
#define HEAD_DATA_CLOCK_HZ 1000000
// TODO make sure it divides OK.
#define HEAD_DATA_CLOCK_SCD (DELAY_COEFFICIENT_SCD / HEAD_DATA_CLOCK_HZ)
// TODO make sure it divides OK.
#define HEAD_DATA_CLOCK_SCD_2 (HEAD_DATA_CLOCK_SCD / 2)
// TODO make sure it divides OK.
#define HEAD_DATA_CLOCK_SCD_4 (HEAD_DATA_CLOCK_SCD / 4)

#define HEAD_NUMBER_OF_PAGES 13

/****************************************************************************/
/* Motor constants                                                          */
/****************************************************************************/

// GPIO port for motor.
#define SYSCTL_PERIPH_GPIO_MOTOR SYSCTL_PERIPH_GPIOD
#define GPIO_PORT_MOTOR_BASE GPIO_PORTD_BASE

#define GPIO_PIN_MOTOR_APHASE GPIO_PIN_0
#define GPIO_PIN_MOTOR_AENABLE GPIO_PIN_1
#define GPIO_PIN_MOTOR_BPHASE GPIO_PIN_2
#define GPIO_PIN_MOTOR_BENABLE GPIO_PIN_3

/// Maximum motor speed (steps per second) 5 * 160 - 352
#define MOTOR_MAX_PPS 448

/****************************************************************************/
/* Inputs from the head and the sensors.                                    */
/****************************************************************************/

// GPIO port for hdo and thermistor.
#define SYSCTL_PERIPH_GPIO_INPUTS SYSCTL_PERIPH_GPIOA
#define GPIO_PORT_INPUTS_BASE GPIO_PORTA_BASE

#define GPIO_PIN_HEAD_TH GPIO_PIN_4   // 7  Thermistor *OUTPUT*
#define GPIO_PIN_HEAD_HDO GPIO_PIN_5  // 9  Print data *OUTPUT*
#define GPIO_PIN_MOTOR_PS GPIO_PIN_6 // 6 Photo Tr Collector *OUTPUT*
#define GPIO_PIN_MOTOR_HS GPIO_PIN_7 // 9 Head up detector *OUTPUT*

/****************************************************************************/
/* DST pulse width constants                                                */
/****************************************************************************/

/// Maximum time (in custom units suitable for SysCtlDelay) that the motor can be excited without overheating.
#define MOTOR_MAX_T_SCD (DELAY_COEFFICIENT_SCD / 448)

#define HEAD_RESISTANCE_B_OHM 170
#define HEAD_RESISTANCE_C_OHM 153 // *


//#define APPLIED_VOLTAGE (5 * 1.26 - 2.46)
//const static uint16_t HEAD_MAIN_PULSE_WIDTH_MS = ENERGY * HEAD_RESISTANCE_C_OHM


#endif /* CONFIG_H_ */
