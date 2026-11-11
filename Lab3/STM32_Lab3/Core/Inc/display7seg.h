/*
 * display7seg.h
 *
 *  Created on: Oct 30, 2026
 *      Author: Trinh Thanh Quang
 */

#ifndef INC_DISPLAY7SEG_H_
#define INC_DISPLAY7SEG_H_

#include "main.h"

extern int led_buffer[4];

void display7seg(int num);
void update7seg(int index);
void updateLEDBuffer(int time, int mode_num);

#endif /* INC_DISPLAY7SEG_H_ */
