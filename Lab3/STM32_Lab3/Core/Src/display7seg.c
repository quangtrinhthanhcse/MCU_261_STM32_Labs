/*
 * display7seg.c
 *
 *  Created on: Oct 30, 2026
 *      Author: Trinh Thanh Quang
 */
#include "display7seg.h"

int led_buffer[4] = {0, 0, 0, 0};

void display7seg(int num){
    HAL_GPIO_WritePin(GPIOB, a_Pin | b_Pin | c_Pin | d_Pin | e_Pin | f_Pin | g_Pin, GPIO_PIN_SET);

    switch (num) {
            case 0: HAL_GPIO_WritePin(GPIOB, a_Pin | b_Pin | c_Pin | d_Pin | e_Pin | f_Pin, GPIO_PIN_RESET); break;
            case 1: HAL_GPIO_WritePin(GPIOB, b_Pin | c_Pin, GPIO_PIN_RESET); break;
            case 2: HAL_GPIO_WritePin(GPIOB, a_Pin | b_Pin | d_Pin | e_Pin | g_Pin, GPIO_PIN_RESET); break;
            case 3: HAL_GPIO_WritePin(GPIOB, a_Pin | b_Pin | c_Pin | d_Pin | g_Pin, GPIO_PIN_RESET); break;
            case 4: HAL_GPIO_WritePin(GPIOB, b_Pin | c_Pin | f_Pin | g_Pin, GPIO_PIN_RESET); break;
            case 5: HAL_GPIO_WritePin(GPIOB, a_Pin | c_Pin | d_Pin | f_Pin | g_Pin, GPIO_PIN_RESET); break;
            case 6: HAL_GPIO_WritePin(GPIOB, a_Pin | c_Pin | d_Pin | e_Pin | f_Pin | g_Pin, GPIO_PIN_RESET); break;
            case 7: HAL_GPIO_WritePin(GPIOB, a_Pin | b_Pin | c_Pin, GPIO_PIN_RESET);break;
            case 8: HAL_GPIO_WritePin(GPIOB, a_Pin | b_Pin | c_Pin | d_Pin | e_Pin | f_Pin | g_Pin, GPIO_PIN_RESET); break;
            case 9: HAL_GPIO_WritePin(GPIOB, a_Pin | b_Pin | c_Pin | d_Pin | f_Pin | g_Pin, GPIO_PIN_RESET); break;
            default: break;
    }
}

void update7seg(int index){
    HAL_GPIO_WritePin(GPIOA, EN0_Pin | EN1_Pin | EN2_Pin | EN3_Pin, GPIO_PIN_SET);

    switch (index) {
            case 0:HAL_GPIO_WritePin(GPIOA, EN0_Pin, GPIO_PIN_RESET); display7seg(led_buffer[0]); break;
            case 1: HAL_GPIO_WritePin(GPIOA, EN1_Pin, GPIO_PIN_RESET); display7seg(led_buffer[1]); break;
            case 2: HAL_GPIO_WritePin(GPIOA, EN2_Pin, GPIO_PIN_RESET); display7seg(led_buffer[2]); break;
            case 3: HAL_GPIO_WritePin(GPIOA, EN3_Pin, GPIO_PIN_RESET); display7seg(led_buffer[3]); break;
            default: break;
    }
}

void updateLEDBuffer(int time, int mode_num) {
    led_buffer[0] = time / 10;
    led_buffer[1] = time % 10;
    led_buffer[2] = mode_num / 10;
    led_buffer[3] = mode_num % 10;
}
