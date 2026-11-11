/*
 * traffic_light.c
 *
 *  Created on: Oct 30, 2026
 *      Author: Trinh Thanh Quang
 */

#include "main.h"
#include "traffic_light.h"
#include "software_timer.h"
#include "fsm_mode.h"
#include "display7seg.h"

enum trafficState {RED1_GREEN2, RED1_YELLOW2, GREEN1_RED2, YELLOW1_RED2};

static enum trafficState traffic_state = RED1_GREEN2;

static int count1 = 0;
static int count2 = 0;

void traffic_light_init(void){
	red_time = yellow_time + green_time;

	traffic_state = RED1_GREEN2;
	count1 = red_time;
	count2 = green_time;

	//  Tuyến 1
	HAL_GPIO_WritePin(GPIOA, RED_1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, YELLOW_1_Pin | GREEN_1_Pin, GPIO_PIN_SET);
	//Tuyến 2
	HAL_GPIO_WritePin(GPIOA, GREEN_2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, RED_2_Pin | YELLOW_2_Pin, GPIO_PIN_SET);

	setTimer3(1000);
}


void traffic_light_run(void){
	updateLEDBuffer(count1, count2);

	if(timer3_flag == 1){
		setTimer3(1000); count1--; count2--;

		switch(traffic_state){
			case RED1_GREEN2:
				if(count2 <= 0){
					traffic_state = RED1_YELLOW2; count2 = yellow_time;
					HAL_GPIO_WritePin(GPIOA, YELLOW_2_Pin, RESET);
					HAL_GPIO_WritePin(GPIOA, GREEN_2_Pin, SET);
				}
				break;

			case RED1_YELLOW2:
				if(count2 <= 0){
					traffic_state = GREEN1_RED2;
					count1 = green_time;
					count2 = red_time;

					HAL_GPIO_WritePin(GPIOA, GREEN_1_Pin, RESET);
					HAL_GPIO_WritePin(GPIOA, RED_1_Pin, SET);

					HAL_GPIO_WritePin(GPIOA, RED_2_Pin, RESET);
					HAL_GPIO_WritePin(GPIOA, YELLOW_2_Pin, SET);
				}
				break;

			case GREEN1_RED2:
				if(count1 <= 0){
					traffic_state = YELLOW1_RED2; count1 = yellow_time;
					HAL_GPIO_WritePin(GPIOA, YELLOW_1_Pin, RESET);
					HAL_GPIO_WritePin(GPIOA, GREEN_1_Pin, SET);
				}
				break;

			case YELLOW1_RED2:
					if(count1 <= 0){
						traffic_state = RED1_GREEN2;
						count1 = red_time;
						count2 = green_time;

						HAL_GPIO_WritePin(GPIOA, RED_1_Pin, RESET);
						HAL_GPIO_WritePin(GPIOA, YELLOW_1_Pin, SET);

						HAL_GPIO_WritePin(GPIOA, GREEN_2_Pin, RESET);
						HAL_GPIO_WritePin(GPIOA, RED_2_Pin, SET);
					}
					break;
			}
	}
}

