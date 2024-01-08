#include <EEPROM.h>
/*===============EEPROM===============*/

// #include <TinyGsmClient.h>

// #define TINY_GSM_MODEM_SIM7600

// TinyGsm modem(Serial2);
/*===============4G===============*/

#include <Wire.h>
#include <MPU6050_tockn.h>

MPU6050 mpu6050(Wire);
/*===============MPU6050===============*/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
/*===============OLED 0.91===============*/

#include "MAX30105.h"
MAX30105 particleSensor;
/*===============MAX 30102===============*/

#define SOS 13    //SOS button
#define RESET 14  //ADMIN button
#define Acol 35   //Acol sensor
/*===============Sensor===============*/

//BLE

unsigned long Time, t1, t0, t;

int value1, value2, value3;
int count = 6;

String ADMIN, locationInfo;

static float angle = 0.0;
int radius = 15;
int centerX = 64;
int centerY = 32;
int numDots = 8;
/*===============Initialization variable==============*/

const unsigned char speed[] PROGMEM = {
  0xff, 0xff, 0xf0, 0xff, 0x6f, 0xf0, 0xf8, 0x61, 0xf0, 0xf0, 0x60, 0xf0, 0xf0, 0x60, 0xf0, 0xd8,
  0xf1, 0xb0, 0x8f, 0xff, 0x10, 0x87, 0xfe, 0x10, 0x0f, 0xff, 0x00, 0x0f, 0xff, 0x00, 0x0f, 0x0f,
  0x00, 0xff, 0x0f, 0xf0, 0x0f, 0x87, 0x00, 0x0f, 0xc3, 0x00, 0x07, 0xe3, 0x00, 0x87, 0xf9, 0x10,
  0x83, 0xfd, 0x90, 0xc7, 0xff, 0x30, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0
};

const unsigned char rate[] PROGMEM = {
  0xff, 0xff, 0xf0, 0xe0, 0xf0, 0x70, 0xce, 0x67, 0x30, 0x9f, 0x9f, 0x90, 0x3f, 0x9f, 0xc0, 0x7f,
  0xff, 0xe0, 0x7f, 0xff, 0xe0, 0x73, 0x9f, 0xe0, 0x73, 0x9f, 0xe0, 0x01, 0x00, 0xc0, 0x8d, 0x60,
  0xd0, 0x9c, 0x7f, 0x90, 0xde, 0xff, 0xb0, 0xef, 0xff, 0x70, 0xf7, 0xfe, 0xf0, 0xf9, 0xf9, 0xf0,
  0xfc, 0xf3, 0xf0, 0xfe, 0x07, 0xf0, 0xff, 0x9f, 0xf0, 0xff, 0xff, 0xf0
};
/*===============Draw icon==============*/

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200);

  EEPROM.begin(64);
  delay(500);

  /*----------------------EEPROM-----------------------*/

  pinMode(SOS, INPUT_PULLUP);
  pinMode(RESET, INPUT_PULLUP);

  Wire.begin(21, 19);  //Start I2C. 21=SDA, 19=SCL

  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);
  /*----------------------Start MPU6050-----------------------*/

  particleSensor.begin(Wire, I2C_SPEED_FAST);
  particleSensor.setup(0x1F, 8, 3, 100, 411, 4096);
  /*----------------------Start HeartRate sensor-----------------------*/

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("Không tìm thấy màn hình OLED"));
    while (true)
      ;
  }

  Serial.println("Booting time...");
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(23, 8);
  display.println("WELCOME");
  display.display();

  Serial2.println("AT+CMGF=1");
  delay(500);
  Serial2.println("AT+CNMI=2,2,0,0,0");
  delay(500);
  Serial2.println("AT+CMGL=\"REC UNREAD\"");
  delay(500);

  display.clearDisplay();
  /*----------------------Start SIM-----------------------*/


  for (int i = 200; i >= 0; i--) {
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println("SMART HELMET BOOTING");

    for (int i = 0; i < numDots; i++) {
      float dotAngle = radians(angle + i * 360.0 / numDots);
      int x = centerX + radius * cos(dotAngle);
      int y = centerY + radius * sin(dotAngle);
      display.drawPixel(x, y, SSD1306_WHITE);
    }

    angle += random(5, 15);  //Tốc độ quay
    if (angle >= 360.0) {
      angle = 0.0;
    }

    display.display();
    delay(50);
    display.clearDisplay();
  }

  if (millis() - Time >= 2000) {
    for (int i = 0; i < 12; ++i) {
      ADMIN += char(EEPROM.read(i));  //Check ADMIN
    }
    Serial.println(ADMIN);
    while (ADMIN == 0) {  // Thay đổi điều kiện này
      display.clearDisplay();
      display.setTextSize(1);
      display.setCursor(0, 0);
      display.println("No ADMIN");
      display.setCursor(0, 10);
      display.println("Press ADMIN button");
      display.display();
      delay(500);
      if (digitalRead(RESET) == 0) {
        while (digitalRead(RESET) == 0) {
          count--;
          display.clearDisplay();
          display.setTextSize(1);
          display.setCursor(35, 16);
          display.print("Waitting: ");
          display.print(count);
          display.display();
          delay(1000);
          if (count <= 0) {
            resetAdmin();
          }
        }
        if (digitalRead(RESET) == 1) { count = 6; }
      }
    }
  }

  Time = millis();
}
/*----------------------Booting time-----------------------*/

void loop() {

  startDevice();   //Show time start
  activeSensor();  //Run MPU, Acol
  findDevice();    //Map

  if (digitalRead(SOS) == 0) { sendData("Help me!!!"); }  // SOS button

  if (digitalRead(RESET) == 0) {
    display.clearDisplay();
    while (digitalRead(RESET) == 0) {
      count--;
      delay(1000);
      display.setTextSize(1);
      display.setCursor(15, 3);
      display.print("Reset ADMIN: ");
      display.print(count);
      if (count <= 0) {
        resetAdmin();
      }
    }
    if (digitalRead(RESET) == 1) {
      display.clearDisplay();
      count = 6;
    }
  }
  Time = millis();
}
