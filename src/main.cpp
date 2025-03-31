#include <ESP32Servo.h>
#include <WiFi.h> // Thêm thư viện WiFi gốc nếu chưa có
#include <WiFiUdp.h>
#include <NTPClient.h> // Thư viện để lấy thời gian qua mạng

#define BLYNK_TEMPLATE_ID "TMPL6bEzSdfTn"
#define BLYNK_TEMPLATE_NAME "iot"
#define BLYNK_AUTH_TOKEN "Ytov5ONJ9Q-zdX4v7Fy01II0QUQqfzWT"
#define LOW_LIGHT 0x0
#define MEDIUM_LIGHT 0x1
#define HIGH_LIGHT 0x2

#include <BlynkSimpleEsp32.h>

// Pin definitions
constexpr int SERVO_PIN = GPIO_NUM_21;
constexpr int OUTPUT_PIN = GPIO_NUM_4; // Vẫn định nghĩa nhưng không dùng trong logic chính
constexpr int LIGHT_SENSOR_PIN = GPIO_NUM_34;
constexpr int LED_LOW_PIN = GPIO_NUM_25; // LED cho ánh sáng yếu
constexpr int LED_MEDIUM_PIN = GPIO_NUM_26; // LED cho ánh sáng trung bình
constexpr int LED_HIGH_PIN = GPIO_NUM_27; // LED cho ánh sáng mạnh

#define WIFI_SSID "Thai Thanh"
#define WIFI_PASS "thaithanh2019"

#define LOW_LIGHT_THRESHOLD 3000    // Trên 3000 là ánh sáng thấp
#define HIGH_LIGHT_THRESHOLD 1000   // Dưới 1000 là ánh sáng cao

// --- Cấu hình NTP ---
const long UTC_OFFSET_SECONDS = 7 * 3600; // GMT+7 cho Việt Nam
const char *NTP_SERVER = "pool.ntp.org";
const int DAY_START_HOUR = 6; // Giờ bắt đầu ban ngày (6 AM)
const int DAY_END_HOUR = 18; // Giờ kết thúc ban ngày (6 PM)

BlynkTimer timer;
Servo servo;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, NTP_SERVER, UTC_OFFSET_SECONDS);

// Biến toàn cục
int on = 0; // Có vẻ không được sử dụng?
int off = 0; // Có vẻ không được sử dụng?
bool moveRight = true;
int servoPos = 0;
int current_light_state = LOW_LIGHT;
int active_rotate = 0;

// Hàm cập nhật trạng thái ánh sáng và gửi lên Blynk
void update_blynk_state() {
    uint16_t analog_read = analogRead(LIGHT_SENSOR_PIN);
    Blynk.virtualWrite(V2, analog_read);
    Blynk.virtualWrite(V3, servoPos);
}

// Hàm xử lý quay servo (CHỈ KHI ĐIỀU KHIỂN BẬT VÀ VÀO BAN NGÀY)
void handleRotateServo() {
    // 1. Kiểm tra xem chế độ quay có được bật không
    if (active_rotate == 0) {
        // Nếu chế độ quay bị tắt, không làm gì cả
        return;
    }

    int currentHour = timeClient.getHours();
    bool isDayTime = (currentHour >= DAY_START_HOUR && currentHour < DAY_END_HOUR);

    if (!isDayTime) {
        servo.write(90); // Về vị trí giữa chẳng hạn
        return; // Thoát hàm nếu là ban đêm
    }

    // 4. Nếu được bật VÀ là ban ngày, thì mới thực hiện logic quay dựa trên ánh sáng
    switch (current_light_state) {
        case LOW_LIGHT:
            if (moveRight) {
                servoPos += 2; // Tốc độ bình thường
                if (servoPos >= 180) {
                    moveRight = false;
                }
            } else {
                servoPos -= 2;
                if (servoPos <= 0) {
                    moveRight = true;
                }
            }
            servo.write(servoPos);
            break;

        case MEDIUM_LIGHT:
            if (moveRight) {
                servoPos += 1; // Tốc độ chậm
                if (servoPos >= 180) {
                    moveRight = false;
                }
            } else {
                servoPos -= 1;
                if (servoPos <= 0) {
                    moveRight = true;
                }
            }
            servo.write(servoPos);
            break;

        case HIGH_LIGHT:
            // Dừng xoay khi tìm được ánh sáng mạnh (vẫn giữ nguyên logic này vào ban ngày)
            // Giữ nguyên vị trí hiện tại
            break;
        default:
            ; // Không làm gì
    }
}

// Hàm xử lý tín hiệu từ Blynk V0 để bật/tắt chế độ quay
BLYNK_WRITE(V0) {
    Serial.print("V0 received: ");
    active_rotate = param.asInt(); // Cập nhật trạng thái bật/tắt
    Serial.println(active_rotate);
    if (active_rotate == 0) {
        servo.write(servoPos); // Giữ vị trí cuối cùng
    }
}

BLYNK_WRITE(V3) {
    Serial.print("V3 received: ");
    servoPos = param.asInt();
    Serial.println(servoPos);
    servo.write(servoPos);
}

void setup() {
    Serial.begin(115200);

    // Pin initialization
    pinMode(OUTPUT_PIN, OUTPUT);
    pinMode(LIGHT_SENSOR_PIN, INPUT);
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(LED_LOW_PIN, OUTPUT);
    pinMode(LED_MEDIUM_PIN, OUTPUT);
    pinMode(LED_HIGH_PIN, OUTPUT);
    servo.attach(SERVO_PIN);
    servo.write(0); // Vị trí ban đầu

    // Kết nối WiFi và Blynk
    Blynk.begin(BLYNK_AUTH_TOKEN, WIFI_SSID, WIFI_PASS);

    // Khởi tạo NTP client sau khi có kết nối WiFi
    timeClient.begin();

    // Cài đặt timer
    timer.setInterval(5000L, update_blynk_state); // Cập nhật trạng thái ánh sáng mỗi 5 giây
    timer.setInterval(50L, handleRotateServo); // Điều khiển servo mỗi 50ms (Có thể tăng lên nếu servo giật)

    Blynk.virtualWrite(V0, 0); // Đặt giá trị mặc định cho V0 là OFF
    active_rotate = 0; // Đảm bảo trạng thái ban đầu là tắt
}

void loop() {
    Blynk.run(); // Chạy các tác vụ nền của Blynk
    timer.run(); // Chạy các hàm đã lên lịch

    timeClient.update();

    // Đọc cảm biến và xác định trạng thái ánh sáng
    uint16_t analog_read = analogRead(LIGHT_SENSOR_PIN);

    if (analog_read < HIGH_LIGHT_THRESHOLD) {
        current_light_state = HIGH_LIGHT;
    } else if (analog_read >= HIGH_LIGHT_THRESHOLD && analog_read <= LOW_LIGHT_THRESHOLD) {
        current_light_state = MEDIUM_LIGHT;
    } else {
        current_light_state = LOW_LIGHT;
    }

    // Cập nhật trạng thái các đèn LED
    digitalWrite(LED_LOW_PIN, current_light_state == LOW_LIGHT ? HIGH : LOW);
    digitalWrite(LED_MEDIUM_PIN, current_light_state == MEDIUM_LIGHT ? HIGH : LOW);
    digitalWrite(LED_HIGH_PIN, current_light_state == HIGH_LIGHT ? HIGH : LOW);

    // Báo hiệu trạng thái kết nối Blynk
    digitalWrite(LED_BUILTIN, Blynk.connected());
}
