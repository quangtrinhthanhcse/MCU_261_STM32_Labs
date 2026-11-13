/*
 * scheduler.h
 *
 * Module: Cooperative Scheduler (Bộ lập lịch hợp tác)
 * Hỗ trợ lập lịch tác vụ định kỳ (periodic) và chạy một lần (one-shot)
 * với độ phức tạp SCH_Update() đạt O(1) dựa trên cấu trúc Delta List.
 */

#ifndef INC_SCHEDULER_H_
#define INC_SCHEDULER_H_

#include "main.h"
#include <stdint.h>

#define SCH_MAX_TASKS   40
#define NO_TASK_ID      0
#define TICK_MS         10     /* Chu kỳ ngắt Timer2 = 10ms */

/* Cấu trúc dữ liệu của một tác vụ trong hàng đợi */
typedef struct {
    void     (*pTask)(void);    /* Con trỏ hàm trỏ tới tác vụ cần thực thi */
    uint32_t Delay;             /* Số tick chờ tương đối so với tác vụ đứng trước (Delta List) */
    uint32_t Period;            /* Chu kỳ lặp lại (tính bằng tick); Period = 0 nếu chạy 1 lần */
    uint8_t  RunMe;             /* Cờ báo hiệu tác vụ đã đến hạn cần chạy */
    uint32_t TaskID;            /* Định danh duy nhất của tác vụ */
} sTask;

/* Các hàm giao diện chính của bộ lập lịch (API) */
void     SCH_Init(void);
void     SCH_Update(void);
void     SCH_Dispatch_Tasks(void);
uint32_t SCH_Add_Task(void (*pFunction)(void), uint32_t DELAY, uint32_t PERIOD);
uint8_t  SCH_Delete_Task(uint32_t taskID);

#endif /* INC_SCHEDULER_H_ */
