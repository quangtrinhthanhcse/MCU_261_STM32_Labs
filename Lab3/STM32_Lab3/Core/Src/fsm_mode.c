/*
 * fsm_mode.c
 *
 *  Created on: Oct 29, 2026
 *      Author: Trinh Thanh Quang
 */
#include "main.h"
#include "input_reading.h"
#include "fsm_mode.h"
#include "traffic_light.h"

int mode = 1;
int temp_red_time = 5;
int temp_yellow_time = 2;
int temp_green_time = 3;

enum ButtonState {BUTTON_RELEASED, BUTTON_PRESSED};
enum ButtonState btn1State = BUTTON_RELEASED;
enum ButtonState btn2State = BUTTON_RELEASED;
enum ButtonState btn3State = BUTTON_RELEASED;

void fsm_mode_run(void) {
	// Nút 1: chuyển trạng thái máy 01 -> 02-> 03-> 04-> 01
    switch (btn1State) {
        case BUTTON_RELEASED:
            if (is_button_pressed(0)) { // Nút 1 (Mode) được nhấn
                btn1State = BUTTON_PRESSED;
                mode++;
                if (mode > 4) mode = 1;
                if (mode == 1) traffic_light_init();
                if (mode == 2) temp_red_time = red_time;
                else if(mode == 3) temp_yellow_time = yellow_time;
                else if(mode == 4) temp_green_time = green_time;
            }
            break;
        case BUTTON_PRESSED:
            if (!is_button_pressed(0)) { // Chờ người dùng thả tay ra
                btn1State = BUTTON_RELEASED;
            }
            break;
    }
    // Xử lý nút BTN2 và BTN3 khi đang ở mode 02 (sửa thời gian đèn đỏ)
    if(mode == 2){
    	switch(btn2State){
    	case BUTTON_RELEASED:
    		if(is_button_pressed(1)){
    			btn2State = BUTTON_PRESSED;
    			temp_red_time++;
    			if(temp_red_time > 99) temp_red_time = 1;
    		}
    		break;
    	case BUTTON_PRESSED:
    		if(!is_button_pressed(1)){
    			btn2State = BUTTON_RELEASED;
    		}
    		break;
    	}

    	switch(btn3State){
    	case BUTTON_RELEASED:
    		if(is_button_pressed(2)){
    			btn3State = BUTTON_PRESSED;
    			red_time = temp_red_time;
    		}
    		break;
    	case BUTTON_PRESSED:
    		if(!is_button_pressed(2)){
    			btn3State = BUTTON_RELEASED;
    		}
    		break;
    	}
    }
    // Xử lý nút BTN2 và BTN3 khi đang ở mode 03 (sửa thời gian đèn vàng)
    if(mode == 3){
    	switch(btn2State){
    	case BUTTON_RELEASED:
    		if(is_button_pressed(1)){
    			btn2State = BUTTON_PRESSED;
    			temp_yellow_time++;
    			if(temp_yellow_time > 99) temp_yellow_time = 1;
    		}
    		break;
    	case BUTTON_PRESSED:
    		if(!is_button_pressed(1)){
    			btn2State = BUTTON_RELEASED;
    		}
    		break;
    	}

    	switch(btn3State){
    	case BUTTON_RELEASED:
    		if(is_button_pressed(2)){
    			btn3State = BUTTON_PRESSED;
    			yellow_time = temp_yellow_time;
    		}
    		break;
    	case BUTTON_PRESSED:
    		if(!is_button_pressed(2)){
    			btn3State = BUTTON_RELEASED;
    		}
    		break;
    	}
    }
    // Xử lý nút BTN2 và BTN3 khi đang ở mode 03 (sửa thời gian đèn vàng)
    else if(mode == 4){
    	switch(btn2State){
    	case BUTTON_RELEASED:
    		if(is_button_pressed(1)){
    			btn2State = BUTTON_PRESSED;
    			temp_green_time++;
    			if(temp_green_time > 99) temp_green_time = 1;
    		}
    		break;
    	case BUTTON_PRESSED:
    		if(!is_button_pressed(1)){
    			btn2State = BUTTON_RELEASED;
    		}
    		break;
    	}

    	switch(btn3State){
    	case BUTTON_RELEASED:
    		if(is_button_pressed(2)){
    			btn3State = BUTTON_PRESSED;
    			green_time = temp_green_time;
    		}
    		break;
    	case BUTTON_PRESSED:
    		if(!is_button_pressed(2)){
    			btn3State = BUTTON_RELEASED;
    		}
    		break;
    	}
    }
}


