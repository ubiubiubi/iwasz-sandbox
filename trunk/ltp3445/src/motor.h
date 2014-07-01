/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef MOTOR_LTP_4335_H_
#define MOTOR_LTP_4335_H_

/**
 * Initialize all things to use the motor properly.
 */
extern void motorInit (void);

/**
 * Feeds paper forward (when steps is positive) or backwards (otherwise). Accelerates if necessary.
 * Leaves the motor in pause mode (no excitation). Issues start and stop steps at the beginnig and end
 * accordingly.
 */
extern void motorRun (int16_t steps);

/**
 * One step forward.
 */
extern void motorForward1Step (void);

/**
 * One step backward.
 */
extern void motorBackward1Step (void);

/**
 * Issue the start step (necessary after pause).
 */
extern void motorStartStep (void);

/**
 * Issue the stop step (necessary before pause).
 */
extern void motorStopStep (void);

#endif /* MOTOR_H_ */
