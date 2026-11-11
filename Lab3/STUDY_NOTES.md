# SỔ TAY ÔN TẬP VẤN ĐÁP - VXL & VĐK (CO3009)
## LAB 3: TIMER, INTERRUPT, FSM & MULTIPLEXING 7-SEGMENT DISPLAY

---

### ❓ Câu hỏi 1: Tại sao các biến cờ chia sẻ giữa ngắt (ISR) và vòng lặp `while (1)` bắt buộc phải khai báo từ khóa `volatile`?
* **💡 Trả lời cốt lõi:**
  * Khi bật tối ưu hóa biên dịch (ví dụ `-O2`, `-O3`), trình biên dịch giả định rằng biến trong `while (1)` không bị thay đổi bởi hàm nào khác trong luồng chính, nên nó sẽ nạp biến đó vào thanh ghi CPU (Register Cache) để đọc cho nhanh thay vì truy xuất ô nhớ RAM mỗi chu kỳ.
  * Khi hàm ngắt phần cứng (ISR) thay đổi giá trị biến trong RAM, vòng lặp `while (1)` vẫn đọc giá trị cũ từ thanh ghi CPU $\rightarrow$ dẫn đến vòng lặp bị treo vĩnh viễn (không bao giờ thoát hay nhận cờ).
  * Từ khóa `volatile` báo hiệu cho compiler: *"Biến này có thể bị thay đổi bất ngờ bởi phần cứng hoặc ngắt bên ngoài, mỗi lần đọc/ghi bắt buộc phải truy xuất trực tiếp từ ô nhớ RAM, không được tối ưu vào thanh ghi."*
* **🔗 Liên kết code:**
  * Khai báo cờ Timer: [software_timer.h](file:///d:/HCMUT/MON%20HOC/HK261/VXL-VDK/Lab/Lab3/STM32_Lab3/Core/Inc/software_timer.h#L16-L20) & [software_timer.c](file:///d:/HCMUT/MON%20HOC/HK261/VXL-VDK/Lab/Lab3/STM32_Lab3/Core/Src/software_timer.c#L10-L21)
  * Kiểm tra cờ trong vòng lặp chính: [main.c](file:///d:/HCMUT/MON%20HOC/HK261/VXL-VDK/Lab/Lab3/STM32_Lab3/Core/Src/main.c#L111-L115)

---

### ❓ Câu hỏi 2: Các chân điều khiển BJT (`EN0..EN3`) và thanh LED 7 đoạn (`a..g`) phải cấu hình GPIO ở chế độ nào? Tại sao không dùng Input hay Open-drain?
* **💡 Trả lời cốt lõi:**
  * Bắt buộc phải là **GPIO Output Push-Pull** (`GPIO_MODE_OUTPUT_PP`).
  * **Tại sao không thể là Input?** Vì đây là các chân vi điều khiển xuất tín hiệu ra để đóng/ngắt transistor và bật/tắt thanh LED, chứ không phải chân nhận tín hiệu từ cảm biến hay nút nhấn.
  * **Tại sao là Push-Pull mà không phải Open-Drain?**
    * Push-Pull có hai transistor nội (PMOS kéo lên $3.3\text{V}$, NMOS kéo xuống $0\text{V}$), giúp xuất mức cao và mức thấp dứt khoát, cung cấp đủ dòng đóng/ngắt cực B của transistor PNP và hút dòng từ LED.
    * Open-Drain chỉ có NMOS kéo xuống $0\text{V}$, khi muốn lên mức $3.3\text{V}$ thì chân bị thả nổi (Hi-Z), bắt buộc phải có điện trở kéo lên ngoài (Pull-up resistor) thì mới ngắt được transistor PNP.
* **🔗 Liên kết code:**
  * Cấu hình trong hàm `MX_GPIO_Init()`: [main.c](file:///d:/HCMUT/MON%20HOC/HK261/VXL-VDK/Lab/Lab3/STM32_Lab3/Core/Src/main.c#L282-L296)

---

### ❓ Câu hỏi 3: Quy tắc tính giá trị nạp vào thanh ghi Prescaler (`PSC`) và Auto-Reload (`ARR`) của Timer?
* **💡 Trả lời cốt lõi:**
  * Công thức tần số ngắt ngắt:
    $$f_{INT} = \frac{f_{CLK}}{(PSC + 1) \times (ARR + 1)}$$
  * **Quy tắc quan trọng nhất:** Luôn phải nhớ **cộng thêm 1** (`+1`) vào giá trị cấu hình của cả `PSC` và `ARR` vì bộ đếm của vi điều khiển đếm bắt đầu từ $0$ đến giá trị cài đặt (ví dụ cài $9$ tức là đếm $10$ xung: $0, 1, 2, ..., 9$).
  * **Ví dụ:** $f_{CLK} = 72\text{ MHz}$, muốn tạo ngắt $1\text{ ms}$ ($f_{INT} = 1000\text{ Hz}$):
    $$(PSC + 1) \times (ARR + 1) = \frac{72 \times 10^6}{1000} = 72,000$$
    Nếu chọn $ARR = 999$ ($ARR + 1 = 1000$) thì $PSC + 1 = 72 \Rightarrow PSC = 71$.
* **🔗 Liên kết code:**
  * Cấu hình phần cứng trong CubeMX: [software_timer.h](file:///d:/HCMUT/MON%20HOC/HK261/VXL-VDK/Lab/Lab3/STM32_Lab3/Core/Inc/software_timer.h#L14) (`TIMER_CYCLE 10`)

---

### ❓ Câu hỏi 4: Hiện tượng bóng mờ (Ghosting) khi quét LED 7 đoạn là gì và trình tự dập bóng mờ chuẩn?
* **💡 Trả lời cốt lõi:**
  * **Nguyên nhân:** Xảy ra khi dữ liệu của chữ số mới xuất hiện trong khi LED cũ vẫn còn đang được cấp nguồn (hoặc LED mới bật lên trong khi dữ liệu cũ chưa kịp xóa).
  * **Trình tự chuẩn để triệt tiêu bóng mờ (Blanking trước khi chuyển):**
    1. **Tắt LED cũ** (ngắt nguồn tất cả chân kích `EN0..EN3`).
    2. **Xuất mã 7 đoạn** của LED mới ra cổng dữ liệu (`a..g`).
    3. **Bật chân kích của LED mới** để bắt đầu hiển thị.
* **🔗 Liên kết code:**
  * Hàm xóa chân Enable trước khi hiển thị: [display7seg.c](file:///d:/HCMUT/MON%20HOC/HK261/VXL-VDK/Lab/Lab3/STM32_Lab3/Core/Src/display7seg.c#L33-L44)

---

### ❓ Câu hỏi 5: Vai trò của biến `KeyReg3` trong thuật toán lọc dội nút nhấn 3 tầng là gì?
* **💡 Trả lời cốt lõi:**
  * `KeyReg0`, `KeyReg1`, `KeyReg2` dùng để lấy mẫu và xác nhận tín hiệu đã ổn định (qua bộ lọc 3 mẫu bằng nhau).
  * `KeyReg3` lưu trữ **trạng thái ổn định ở chu kỳ trước đó**.
  * So sánh `KeyReg3 != KeyReg2` giúp phát hiện **sự kiện chuyển sườn (Edge Detection)** — tức là người dùng vừa mới bấm nút (chuyển từ nhả sang nhấn), nhằm kích hoạt hành động một lần duy nhất và reset bộ đếm thời gian nhấn giữ 1 giây.
* **🔗 Liên kết code:**
  * Thuật toán lọc dội: [input_reading.c](file:///d:/HCMUT/MON%20HOC/HK261/VXL-VDK/Lab/Lab3/STM32_Lab3/Core/Src/input_reading.c#L35-L55)

---

### ❓ Câu hỏi 6: Tại sao thời gian đèn giao thông ngã tư bắt buộc phải thỏa $T_{\text{Đỏ}} = T_{\text{Xanh}} + T_{\text{Vàng}}$?
* **💡 Trả lời cốt lõi:**
  * Tại ngã tư 2 làn độc lập: Trong lúc Tuyến 1 bật đèn ĐỎ thì Tuyến 2 phải lần lượt trải qua 2 giai đoạn: (1) Đèn XANH để xe lưu thông, sau đó đến (2) Đèn VÀNG để xe giảm tốc dừng lại an toàn.
  * Hết pha Vàng của Tuyến 2 thì Tuyến 1 mới được chuyển sang Xanh. Do đó, tổng thời gian chờ đèn Đỏ của một tuyến bằng tổng thời gian Xanh + Vàng của tuyến còn lại.
* **🔗 Liên kết code:**
  * Ép ràng buộc chu kỳ trong khởi tạo: [traffic_light.c](file:///d:/HCMUT/MON%20HOC/HK261/VXL-VDK/Lab/Lab3/STM32_Lab3/Core/Src/traffic_light.c#L23)

---

### ❓ Câu hỏi 7: Tại sao nên chuyển `button_reading()` ra khỏi hàm ngắt Timer (`HAL_TIM_PeriodElapsedCallback()`)?
* **💡 Trả lời cốt lõi:**
  * **Nguyên tắc ISR Minimalism:** Hàm phục vụ ngắt (ISR) chỉ nên làm nhiệm vụ tối thiểu (cập nhật bộ đếm thời gian, bật cờ báo hiệu).
  * Việc đọc chân GPIO, chạy vòng lặp xử lý logic nút nhấn bên trong ngắt làm kéo dài thời gian thực thi ISR (ISR execution time), có nguy cơ làm trễ hoặc nghẽn các ngắt khác có độ ưu tiên cao hơn và vi phạm kiến trúc hệ thống thời gian thực.
* **🔗 Liên kết code:**
  * Ngắt chỉ chạy `timerRun()`: [main.c](file:///d:/HCMUT/MON%20HOC/HK261/VXL-VDK/Lab/Lab3/STM32_Lab3/Core/Src/main.c#L249-L253)
  * Đọc nút nhấn trong `while(1)` qua cờ 10ms: [main.c](file:///d:/HCMUT/MON%20HOC/HK261/VXL-VDK/Lab/Lab3/STM32_Lab3/Core/Src/main.c#L111-L115)
