/*
 * fsm_mode.h
 *
 *  Created on: Oct 29, 2026
 *      Author: Home
 */

#ifndef INC_FSM_MODE_H_
#define INC_FSM_MODE_H_

extern int mode;
extern int red_time;
extern int temp_red_time;
extern int yellow_time;
extern int temp_yellow_time;
extern int green_time;
extern int temp_green_time;

void fsm_mode_run(void);

#endif /* INC_FSM_MODE_H_ */
