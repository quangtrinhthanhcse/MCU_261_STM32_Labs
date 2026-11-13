# MCU_261_STM32_Labs

> **Môn học:** Vi xử lý - Vi điều khiển (CO3009)  
> **Trường:** Đại học Bách Khoa - ĐHQG TP.HCM (HCMUT)  
> **Học kỳ:** HK261  
> **Nền tảng:** ARM Cortex-M3 (STM32F103)  
> **Công cụ phát triển:** STM32CubeIDE & Proteus 8 Professional  

---

## 📌 Giới thiệu

Repository này lưu trữ toàn bộ mã nguồn và file mô phỏng các bài thực hành (Lab) môn Vi xử lý - Vi điều khiển. Các bài thực hành được thiết kế xoay quanh vi điều khiển dòng **STM32F103** sử dụng thư viện **STM32 HAL (Hardware Abstraction Layer)** và mô phỏng trực tiếp trên phần mềm **Proteus**.

---

## 📂 Cấu trúc Repository

```text
MCU_261_STM32_Labs/
├── .gitignore                      # Cấu hình bỏ qua các file build rác (.o, .d, .su,...)
├── README.md                       # Tài liệu hướng dẫn tổng quan repo
├── Lab1/                           # Lab 1: GPIO & LED Animations
│   ├── Lab1_STM32/                 # Project STM32CubeIDE
│   └── Lab1_Sim.pdsprj             # File mô phỏng Proteus
├── Lab2/                           # Lab 2: Timer Interrupt & LED Scanning
│   ├── STM32_Lab2/                 # Project STM32CubeIDE
│   └── Proteus/                    # File mô phỏng Proteus
├── Lab3/                           # Lab 3: Button Debounce & FSM
│   ├── STM32_Lab3/                 # Project STM32CubeIDE
│   └── Proteus/                    # File mô phỏng Proteus
└── Lab4/                           # Lab 4: Cooperative Scheduler O(1)
    ├── STM32_Lab4/                 # Project STM32CubeIDE
    └── Proteus/                    # File mô phỏng Proteus
```

---

## 📖 Danh sách các bài Lab

| Bài Lab | Nội dung chính | Mô phỏng / Phần cứng | Trạng thái |
| :--- | :--- | :--- | :---: |
| **Lab 1** | **GPIO & LED Animations**: Cấu hình I/O cơ bản, điều khiển LED đơn, hiển thị LED 7 đoạn và mô phỏng hệ thống đèn giao thông 4 pha / đồng hồ kim LED. | STM32F103C6 + Proteus | ✅ Hoàn thành |
| **Lab 2** | **Timer Interrupt & LED Scanning**: Sử dụng ngắt Timer định thời chính xác, kỹ thuật quét LED 7 đoạn và điều khiển ma trận LED 8x8. | STM32F103C6 + Proteus | ✅ Hoàn thành |
| **Lab 3** | **Button Debounce & State Machine (FSM)**: Chống rung nút nhấn (debounce), xử lý nhấn đúp / nhấn giữ (long-press), xây dựng máy trạng thái hữu hạn (FSM) cho đèn giao thông và chế độ chỉnh thời gian. | STM32F103C6 + Proteus | ✅ Hoàn thành |
| **Lab 4** | **Cooperative Scheduler**: Hiện thực bộ lập lịch tác vụ cộng tác với giải thuật Delta List $O(1)$, điều phối 5 tác vụ độc lập chạy chu kỳ 0.5s, 1s, 1.5s, 2s, 2.5s. | STM32F103C6 + Proteus | ✅ Hoàn thành |
| **Lab 5** | **UART Communication**: Giao tiếp nối tiếp không đồng bộ, truyền nhận dữ liệu và thực thi lệnh qua UART. | STM32F103C6 + Proteus | ⏳ Kế hoạch |

---

## 🛠 Hướng dẫn mở Project & Mô phỏng

### 1. Mở mã nguồn trên STM32CubeIDE
1. Khởi động **STM32CubeIDE**.
2. Chọn menu **File** > **Import...**
3. Chọn **General** > **Existing Projects into Workspace** > nhấn **Next**.
4. Trỏ đường dẫn (Browse) tới thư mục bài Lab cần mở (ví dụ: `MCU_261_STM32_Labs/Lab1/Lab1_STM32`).
5. Nhấn **Finish**.
6. Để biên dịch, nhấn chuột phải vào Project chọn **Build Project** (Ctrl + B).

### 2. Chạy mô phỏng trên Proteus
1. Mở file `.pdsprj` trong thư mục `Proteus/` của bài lab tương ứng.
2. Nhấp đúp vào con chip **STM32F103C6**.
3. Tại mục **Program File**, kiểm tra đường dẫn đã trỏ đúng tới file `.hex` nằm trong thư mục `Debug/` của project STM32 vừa build.
4. Nhấn nút **Play** (F12) ở góc dưới bên trái để chạy mô phỏng.
