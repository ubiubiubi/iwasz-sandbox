/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef CONFIG_LTP4335_H_
#define CONFIG_LTP4335_H_

#define HEAD_PIN_TH    // 7  Thermistor
#define HEAD_PIN_DST   // 8  Thermal head print activation instruction signal
#define HEAD_PIN_BCLR  // 10 Block select all clear
#define HEAD_PIN_BDAT  // 11 Block select data input (serial input)
#define HEAD_PIN_BCLK  // 12 Block select data transfer synchronization signal
#define HEAD_PIN_CTRL  // 13 History control signal
#define HEAD_PIN_HDAT  // 14 Print data input (serial input)
#define HEAD_PIN_LATCH // 17 Print data latch (memory storage)
#define HEAD_PIN_HCLK  // 18 Synchronizing signal for data transfer
#define HEAD_PIN_HDO   // 9  Print data *OUTPUT*

#define GPIO_PIN_MOTOR_APHASE GPIO_PIN_0
#define GPIO_PIN_MOTOR_AENABLE GPIO_PIN_1
#define GPIO_PIN_MOTOR_BPHASE GPIO_PIN_2
#define GPIO_PIN_MOTOR_BENABLE GPIO_PIN_3
#define GPIO_PIN_MOTOR_PS // 6 Photo Tr Collector *OUTPUT*
#define GPIO_PIN_MOTOR_HS // 9 Head up detector *OUTPUT*

// GPIO port for motor.
#define SYSCTL_PERIPH_GPIO_MOTOR SYSCTL_PERIPH_GPIOA
#define GPIO_PORT_MOTOR_BASE GPIO_PORTA_BASE

/// Maximum motor speed (steps per second) 5 * 160 - 352
#define MOTOR_MAX_PPS 448
#define DELAY_COEFFICIENT 1000000

/// Maximum time (in custom units suitable for SysCtlDelay) that the motor can be excited without overheating.
#define MOTOR_MAX_T (DELAY_COEFFICIENT / 448)

#endif /* CONFIG_H_ */
