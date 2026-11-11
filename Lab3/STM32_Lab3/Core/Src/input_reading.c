/*
 * input_reading.c
 *
 *  Created on: Oct 29, 2026
 *      Author: Trinh Thanh Quang
 */
#include "input_reading.h"

#define DURATION_FOR_AUTO_INCREASING 100
#define BUTTON_IS_PRESSED GPIO_PIN_RESET
#define BUTTON_IS_RELEASED GPIO_PIN_SET

static GPIO_PinState buttonBuffer[NO_OF_BUTTONS] = {BUTTON_IS_RELEASED, BUTTON_IS_RELEASED, BUTTON_IS_RELEASED};
static GPIO_PinState debounceButtonBuffer1[NO_OF_BUTTONS] = {BUTTON_IS_RELEASED, BUTTON_IS_RELEASED, BUTTON_IS_RELEASED};
static GPIO_PinState debounceButtonBuffer2[NO_OF_BUTTONS] = {BUTTON_IS_RELEASED, BUTTON_IS_RELEASED, BUTTON_IS_RELEASED};

static uint8_t flagForButtonPressed1s[NO_OF_BUTTONS];
static uint16_t counterForButtonPressed1s[NO_OF_BUTTONS];

void button_reading(void){
	for(int i = 0; i < NO_OF_BUTTONS; i++){
		debounceButtonBuffer2[i] = debounceButtonBuffer1[i];
		// doc chan tuong ung voi nut thu i
		if(i==0){
			debounceButtonBuffer1[i] = HAL_GPIO_ReadPin(BUTTON_1_GPIO_Port, BUTTON_1_Pin);
		}
		else if( i==1){
			debounceButtonBuffer1[i] = HAL_GPIO_ReadPin(BUTTON_2_GPIO_Port, BUTTON_2_Pin);
		}
		else if (i ==2){
			debounceButtonBuffer1[i] = HAL_GPIO_ReadPin(BUTTON_3_GPIO_Port, BUTTON_3_Pin);
		}
		// Lọc chống rung 2 tầng
		if(debounceButtonBuffer1[i] == debounceButtonBuffer2[i]){
			buttonBuffer[i] = debounceButtonBuffer1[i];
			if(buttonBuffer[i] == BUTTON_IS_PRESSED){
				if(counterForButtonPressed1s[i] < DURATION_FOR_AUTO_INCREASING)
					counterForButtonPressed1s[i]++;
				else flagForButtonPressed1s[i] = 1;
			}
			else{
				counterForButtonPressed1s[i] = 0;
				flagForButtonPressed1s[i] = 0;
			}
		}
	}
}

unsigned char is_button_pressed(uint8_t index){
	if(index >= NO_OF_BUTTONS) return 0;
	return buttonBuffer[index] == BUTTON_IS_PRESSED;
}

unsigned char is_button_pressed_1s(unsigned char index){
	if(index >= NO_OF_BUTTONS) return 0;
	return (flagForButtonPressed1s[index]) == 1;
}
