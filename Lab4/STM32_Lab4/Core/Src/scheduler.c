/*
 * scheduler.c
 *
 * Hiện thực Bộ lập lịch hợp tác (Cooperative Scheduler) với SCH_Update() O(1).
 * Nguyên lý Delta List:
 * - Mảng SCH_tasks_G luôn được sắp xếp tăng dần theo thời điểm đến hạn.
 * - Trường Delay của mỗi phần tử lưu khoảng thời gian chờ TƯƠNG ĐỐI so với phần tử đứng ngay trước nó.
 * - Nhờ đó, mỗi nhịp ngắt Timer (10ms), hàm SCH_Update() chỉ cần giảm Delay của phần tử đầu tiên [0].
 */

#include "scheduler.h"
#include <string.h>

static sTask    SCH_tasks_G[SCH_MAX_TASKS];
static uint8_t  SCH_count = 0;
static uint32_t newTaskID = 0;

/* Khóa ngắt tạm thời để bảo vệ vùng dữ liệu dùng chung giữa main() và ISR */
static inline uint32_t SCH_Lock(void) {
    uint32_t primask = __get_PRIMASK();
    __disable_irq();
    return primask;
}

/* Khôi phục trạng thái ngắt ban đầu */
static inline void SCH_Unlock(uint32_t primask) {
    __set_PRIMASK(primask);
}

/* Cấp phát TaskID mới (khác NO_TASK_ID) */
static uint32_t Get_New_Task_ID(void) {
    if (++newTaskID == NO_TASK_ID) {
        newTaskID++;
    }
    return newTaskID;
}

/* Khởi tạo bộ lập lịch: xóa sạch mảng tác vụ */
void SCH_Init(void) {
    memset(SCH_tasks_G, 0, sizeof(SCH_tasks_G));
    SCH_count = 0;
    newTaskID = 0;
}

/*
 * SCH_Update: Gọi trong hàm ngắt Timer (mỗi 10ms).
 * Độ phức tạp O(1): Chỉ kiểm tra và giảm Delay của phần tử đầu hàng đợi [0].
 */
void SCH_Update(void) {
    if (SCH_count == 0) {
        return;
    }
    if (SCH_tasks_G[0].Delay > 0) {
        SCH_tasks_G[0].Delay--;
    }
    if (SCH_tasks_G[0].Delay == 0) {
        SCH_tasks_G[0].RunMe = 1;
    }
}

/*
 * Hàm nội bộ chèn tác vụ vào danh sách theo thứ tự Delta List (O(N)).
 * Các tác vụ có cùng thời điểm đến hạn sẽ giữ nguyên thứ tự FIFO.
 */
static uint32_t Insert_Task(void (*pFunction)(void), uint32_t DELAY, uint32_t PERIOD, uint32_t id) {
    uint32_t m = SCH_Lock();
    if (SCH_count >= SCH_MAX_TASKS) {
        SCH_Unlock(m);
        return NO_TASK_ID;
    }

    uint8_t  idx = 0;
    uint32_t sum = 0;

    /* Tìm vị trí chèn sao cho tổng Delay cộng dồn <= DELAY của tác vụ mới */
    while (idx < SCH_count && (sum + SCH_tasks_G[idx].Delay <= DELAY)) {
        sum += SCH_tasks_G[idx].Delay;
        idx++;
    }

    /* Dịch chuyển các phần tử phía sau sang phải 1 ô để nhường chỗ */
    for (uint8_t i = SCH_count; i > idx; i--) {
        SCH_tasks_G[i] = SCH_tasks_G[i - 1];
    }

    /* Ghi thông tin tác vụ mới vào vị trí idx với Delay tương đối */
    sTask *t  = &SCH_tasks_G[idx];
    t->pTask  = pFunction;
    t->Delay  = DELAY - sum;
    t->Period = PERIOD;
    t->TaskID = id;
    t->RunMe  = (idx == 0 && t->Delay == 0) ? 1 : 0;

    /* Cập nhật lại Delay tương đối của phần tử đứng ngay sau tác vụ mới */
    if (idx < SCH_count) {
        SCH_tasks_G[idx + 1].Delay -= t->Delay;
    }

    SCH_count++;
    SCH_Unlock(m);
    return id;
}

/*
 * Thêm một tác vụ mới vào hàng đợi của Scheduler.
 * - DELAY : Thời gian chờ trước lần chạy đầu tiên (đơn vị: tick 10ms)
 * - PERIOD: Chu kỳ lặp lại (đơn vị: tick 10ms; bằng 0 nếu chỉ chạy 1 lần)
 */
uint32_t SCH_Add_Task(void (*pFunction)(void), uint32_t DELAY, uint32_t PERIOD) {
    uint32_t m  = SCH_Lock();
    uint32_t id = Get_New_Task_ID();
    SCH_Unlock(m);
    return Insert_Task(pFunction, DELAY, PERIOD, id);
}

/*
 * Xóa một tác vụ khỏi danh sách dựa vào taskID.
 * Khi xóa, phần tử đứng ngay sau sẽ cộng dồn thêm phần Delay còn lại của tác vụ bị xóa.
 */
uint8_t SCH_Delete_Task(uint32_t taskID) {
    if (taskID == NO_TASK_ID) {
        return 0;
    }

    uint8_t  found = 0;
    uint32_t m     = SCH_Lock();

    for (uint8_t i = 0; i < SCH_count; i++) {
        if (SCH_tasks_G[i].TaskID != taskID) {
            continue;
        }

        /* Trao lại phần Delay còn lại cho tác vụ kế tiếp */
        if (i + 1 < SCH_count) {
            SCH_tasks_G[i + 1].Delay += SCH_tasks_G[i].Delay;
            SCH_tasks_G[i + 1].RunMe  = (i == 0 && SCH_tasks_G[i + 1].Delay == 0) ? 1 : 0;
        }

        /* Dịch các phần tử phía sau lên 1 ô */
        for (uint8_t j = i; j + 1 < SCH_count; j++) {
            SCH_tasks_G[j] = SCH_tasks_G[j + 1];
        }

        SCH_count--;
        memset(&SCH_tasks_G[SCH_count], 0, sizeof(sTask));
        found = 1;
        break;
    }

    SCH_Unlock(m);
    return found;
}

/*
 * SCH_Dispatch_Tasks: Gọi liên tục trong vòng lặp while(1) của main().
 * Thực thi các tác vụ ở đầu danh sách đã đến hạn (RunMe > 0),
 * sau đó xóa tác vụ cũ và chèn lại vào hàng đợi nếu là tác vụ định kỳ (Period > 0).
 */
void SCH_Dispatch_Tasks(void) {
    while (SCH_count > 0 && SCH_tasks_G[0].RunMe > 0) {
        sTask currentTask = SCH_tasks_G[0];

        /* Thực thi hàm công việc của tác vụ */
        (*currentTask.pTask)();

        /* Xóa tác vụ vừa chạy xong; nếu có chu kỳ lặp lại thì chèn lại vào Delta List */
        if (SCH_Delete_Task(currentTask.TaskID) && currentTask.Period > 0) {
            Insert_Task(currentTask.pTask, currentTask.Period, currentTask.Period, currentTask.TaskID);
        }
    }
}
