# Smart-Parking-System
Smart Parking project - Network Protocols &amp; Latency Measurement 
# Smart Parking System - Network Protocols & Latency Measurement

Báo cáo chi tiết phần việc của **Người 2: Thiết kế Giao thức Mạng & Đo đạc Độ trễ**.

---

## 1. Thiết kế Kiến trúc MQTT
* **MQTT Broker:** `broker.emqx.io` (Port 1883)
* **QoS Level:** QoS 1 (At least once) đảm bảo tin nhắn được truyền tải tin cậy.
* **Cơ chế Retained Message:** Bật `retained = true` trên topic trạng thái ô đỗ để ứng dụng/dashboard cập nhật tức thì khi vừa đăng ký.
* **Cơ chế LWT (Last Will and Testament):** Đăng ký thông điệp mất kết nối bất ngờ tại topic `smartparking/health/lwt` với payload `{"node_id": "node_01", "status": "OFFLINE"}`.

### Cấu trúc Topic
* Trạng thái vị trí đỗ: `smartparking/status/slots/node_01`
* Điều khiển Barrier: `smartparking/control/barrier/node_01`
* Kiểm tra trạng thái thiết bị (LWT): `smartparking/health/lwt`

---

## 2. Kết quả Đo đạc & Phân tích Độ trễ (Latency Analysis)
* **Cỡ mẫu (Sample Size):** $N = 30$ mẫu đo ngẫu nhiên qua các khoảng thời gian khác nhau.
* **Mốc thời gian gửi ($T_1$):** Timestamp đóng gói tại vi điều khiển ESP32.
* **Mốc thời gian nhận ($T_2$):** Timestamp nhận tại ứng dụng client.
* **Công thức độ trễ:** $\Delta t = T_2 - T_1$ (ms)

### Kết quả Thống kê
* **Độ trễ trung bình ($\bar{X}$):** `188.50 ms`
* **Độ lệch chuẩn ($\sigma$):** `23.06 ms`
* **Đánh giá:** Hệ thống hoạt động ổn định, độ trễ thấp ($< 200\text{ms}$), đáp ứng tốt yêu cầu thời gian thực của hệ thống bãi đỗ xe thông minh.

---

## 3. Danh mục Tài liệu & Mã nguồn trong Repository
* `Bao_cao_Nguoi_2.docx`: Báo cáo chi tiết định dạng Word.
* `latency_data.xlsx`: Tập dữ liệu 30 mẫu đo và công thức tính thống kê.
* `esp32_mqtt.ino.ino`: Mã nguồn điều khiển ESP32 kết nối MQTT, đóng gói JSON và phát bản tin.
