/*
 * software_timer.h
 *
 *  Created on: Lab 3
 *      Author: Trinh Thanh Quang
 */

#ifndef INC_SOFTWARE_TIMER_H_
#define INC_SOFTWARE_TIMER_H_

// Chu ky ngat Timer phan cung (don vi ms)
// Khi thay doi chu ky ngat Timer trong CubeIDE, chi can sua duy nhat hang so nay!
#define TIMER_CYCLE 10

extern int timer1_flag;
extern int timer2_flag;
extern int timer3_flag;
extern int timer4_flag;

void setTimer1(int duration);
void setTimer2(int duration);
void setTimer3(int duration);
void setTimer4(int duration);

void timerRun(void);

#endif /* INC_SOFTWARE_TIMER_H_ */
