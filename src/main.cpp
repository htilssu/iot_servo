#include <ESP32Servo.h>
#include "../lib/Wifi/WifiConfig.h"

#define BLYNK_TEMPLATE_ID "TMPL6bEzSdfTn"
#define BLYNK_TEMPLATE_NAME "iot"
#define BLYNK_AUTH_TOKEN "Ytov5ONJ9Q-zdX4v7Fy01II0QUQqfzWT"
#define LOW_LIGHT 0x0
#define MEDIUM_LIGHT 0x1
#define HIGH_LIGHT 0x2

#include <BlynkSimpleEsp32.h>

// Pin definitions
constexpr int SERVO_PIN = GPIO_NUM_21;
constexpr int OUTPUT_PIN = GPIO_NUM_4;
constexpr int LIGHT_SENSOR_PIN = GPIO_NUM_34;
constexpr int LED_LOW_PIN = GPIO_NUM_25; // LED cho ánh sáng yếu
constexpr int LED_MEDIUM_PIN = GPIO_NUM_26; // LED cho ánh sáng trung bình
constexpr int LED_HIGH_PIN = GPIO_NUM_27;

#define WIFI_SSID "Thai Thanh"
#define WIFI_PASS "thaithanh2019"

// Ngưỡng ánh sáng (có thể điều chỉnh)
#define LOW_LIGHT_THRESHOLD 3000    // Trên 3000 là ánh sáng thap
#define HIGH_LIGHT_THRESHOLD 1000   // Dưới 1000 là ánh sáng  cao

BlynkTimer timer;
Servo servo;

int on = 0;
int off = 0;
bool moveRight = true;
int servoPos = 0;
int current_light_state = LOW_LIGHT;
int active_rotate = 0;

void updateLightState() {
    uint16_t analog_read = analogRead(LIGHT_SENSOR_PIN);

    Blynk.virtualWrite(V2, analog_read);
}

void handleRotateServo() {
    if (active_rotate == 0) return;
    switch (current_light_state) {
        case LOW_LIGHT:
            // Xoay bình thường để tìm ánh sáng
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
            // Xoay chậm hơn khi phát hiện ánh sáng vừa
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
            // Dừng xoay khi tìm được ánh sáng mạnh
            // Giữ nguyên vị trí hiện tại
            break;
        default: ;
    }
}

BLYNK_WRITE(V0) {
    Serial.print("V0: ");
    Serial.println(param.asInt());
    active_rotate = param.asInt();
}

void setup() {
    Serial.begin(115200);

    // Pin initialization
    pinMode(OUTPUT_PIN, OUTPUT);
    pinMode(LIGHT_SENSOR_PIN, INPUT);
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(LED_LOW_PIN, OUTPUT); // Khởi tạo chân LED LOW
    pinMode(LED_MEDIUM_PIN, OUTPUT); // Khởi tạo chân LED MEDIUM
    pinMode(LED_HIGH_PIN, OUTPUT); // Khởi tạo chân LED HIGH
    servo.attach(SERVO_PIN);
    servo.write(0);

    Blynk.begin(BLYNK_AUTH_TOKEN, WIFI_SSID, WIFI_PASS);
    timer.setInterval(5000, updateLightState); // Cập nhật trạng thái ánh sáng mỗi giây
    timer.setInterval(20, handleRotateServo); // Điều khiển servo mỗi 100ms
    Blynk.virtualWrite(V0, 0); // Đặt giá trị mặc định cho V0
}

void loop() {
    Blynk.run();
    timer.run();
    uint16_t analog_read = analogRead(LIGHT_SENSOR_PIN);

    if (analog_read < HIGH_LIGHT_THRESHOLD) {
        current_light_state = HIGH_LIGHT;
    } else if (analog_read >= HIGH_LIGHT_THRESHOLD && analog_read <= LOW_LIGHT_THRESHOLD) {
        current_light_state = MEDIUM_LIGHT;
    } else {
        current_light_state = LOW_LIGHT;
    }

    digitalWrite(LED_LOW_PIN, current_light_state == LOW_LIGHT ? HIGH : LOW);
    digitalWrite(LED_MEDIUM_PIN, current_light_state == MEDIUM_LIGHT ? HIGH : LOW);
    digitalWrite(LED_HIGH_PIN, current_light_state == HIGH_LIGHT ? HIGH : LOW);

    digitalWrite(LED_BUILTIN, Blynk.connected());
}
