/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef HEAD_LTP3445_H_
#define HEAD_LTP3445_H_

#include <stdint.h>
#include <stdbool.h>

/**
 * Initialize all stuff for head management (pins etc.)
 * Use least signifficant bits.
 */
extern void headInit (void);

/**
 * Submit 1 line (832 pixels) of data. 1bit color depth.
 * Data must be no shorter than 104 bytes.
 */
extern void headTransferLine1Bit (uint8_t const *data);

extern void headCtrl (bool bit);

extern void headLatch (void);

/**
 * HEAD_NUMBER_OF_PAGES (usually 13) least significatn bits are transfered.
 */
extern void headTransferBdat (uint16_t pages);

/**
 * Read the thermistor and return the temperature of head's heat sink in Celsius.
 */
extern float headThermistorCelsius ();

/**
 * Calculate and return the length of the DST (heat) pulse in units suitable for SysCtlDelay.
 * \param dotsNum Number of dots driven at the same time
 */
extern float headPulseWidthLTP3445SCD (uint16_t dotsNum);

/**
 * Invoke the pulse.
 */
extern void headHeatPulse (uint32_t scdDelay);

#endif /* HEAD_H_ */
