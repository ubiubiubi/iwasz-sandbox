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
/* Motor constants                                                          */
/****************************************************************************/

// GPIO port for motor.
#define SYSCTL_PERIPH_GPIO_MOTOR SYSCTL_PERIPH_GPIOE
#define GPIO_PORT_MOTOR_BASE GPIO_PORTE_BASE

#define GPIO_PIN_MOTOR_APHASE GPIO_PIN_0
#define GPIO_PIN_MOTOR_AENABLE GPIO_PIN_1
#define GPIO_PIN_MOTOR_BPHASE GPIO_PIN_2
#define GPIO_PIN_MOTOR_BENABLE GPIO_PIN_3

/// Maximum motor speed (steps per second) 5 * 160 - 352
#define MOTOR_MAX_PPS 448

/// Maximum time (in custom units suitable for SysCtlDelay) that the motor can be excited without overheating.
#define MOTOR_MAX_T_SCD (DELAY_COEFFICIENT_SCD / MOTOR_MAX_PPS)

#endif /* CONFIG_H_ */
