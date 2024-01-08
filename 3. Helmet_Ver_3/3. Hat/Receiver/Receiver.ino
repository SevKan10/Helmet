#include <EEPROM.h>
//----------------------------------------------------------------------------Bluetooth
#include "BluetoothSerial.h"
BluetoothSerial SerialBT;
unsigned long ping; 
String myGPS; String gps;
//----------------------------------------------------------------------------Màn hình 
#include <Wire.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"
SSD1306AsciiWire oled;
//----------------------------------------------------------------------------Cảm biến gia tốc
#include <MPU6050_tockn.h>
#include <Wire.h>
MPU6050 mpu6050(Wire);
float X, Y;
unsigned long fall; 
int flag2;
//----------------------------------------------------------------------------Cảm biến nhịp tim
#include "MAX30105.h"
MAX30105 particleSensor;
//----------------------------------------------------------------------------Khởi tạo ngoại vi
#define SOS   18   //Nút SOS
#define RESET 19   //Nút xoá số đt ADMIN
#define MQ3   35   //Nút xoá số đt ADMIN
//----------------------------------------------------------------------------Khởi tạo biến toàn cục
String ADMIN; 
int flag1 = 6;
unsigned long timer;


void setup() 
{
//----------------------------------------------------------------------------Khởi động màn hình, màn hình gỡ lỗi
  Serial.begin(9600);
  Wire.begin();
  oled.begin(&Adafruit128x32, 0x3C);
  oled.setFont(Adafruit5x7);
//----------------------------------------------------------------------------Khởi động ngoại vi
  pinMode(23, OUTPUT); digitalWrite(23, 1);
  pinMode(SOS,   INPUT_PULLUP);
  pinMode(RESET, INPUT_PULLUP);
  pinMode(MQ3, INPUT);
//----------------------------------------------------------------------------Khởi động cảm biến gia tốc và cảm biến nhịp tim
  oled.setCursor(0,0); oled.print("Start accelerometer");
  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);
  particleSensor.begin(Wire, I2C_SPEED_FAST);
  particleSensor.setup(0x1F, 8, 3, 100, 411, 4096);
//----------------------------------------------------------------------------Khởi động bluetooth 
  oled.setCursor(0,1); oled.print("Start bluetooth");
  delay(1000);
  SerialBT.begin("ESP32test"); 
//----------------------------------------------------------------------------Khởi động SIM
  oled.setCursor(0,2); oled.print("Looking for network");
  for(int i=10; i>0; i--){Serial.println(i); delay(1000);}
  Serial1.begin(4800, SERIAL_8N1, 26, 27);
  Serial1.println("AT+CMGF=1");                delay(100); 
  Serial1.println("AT+CNMI=2,2,0,0,0");        delay(100);
  Serial1.println("AT+CMGL=\"REC UNREAD\"");   delay(100);
  Serial1.println("AT+CMGD=1,4");              delay(100);
//--------------------------------------------------------------------------Khởi động bộ nhớ trong
  EEPROM.begin(64);  delay(500);
  for (int i = 0;  i < 12; ++i) { ADMIN  += char(EEPROM.read(i)); }  
  Serial.print(ADMIN); 
//--------------------------------------------------------------------------Kiểm tra dữ liệu ADMIN
  while (ADMIN == 0) { Setup_ADMIN();}
  
  oled.setCursor(0,3); oled.print("Device is Ready"); 
  delay(2000);
  oled.clear();
}


void loop() 
{
  Setup_MAP();
//----------------------------------------------------------------------------Bluetooth kết nối, lấy dữ liệu GPS từ xe
  if (SerialBT.available()) 
  {
    oled.setCursor(0,0); oled.print("Connected      ");
    ping = millis();
    gps = SerialBT.readString(); 
  }
  if(gps.indexOf(',')>0){ myGPS = gps; }
//----------------------------------------------------------------------------Blutooth mất kết nối
  while(!SerialBT.available() && millis()-ping >= 15000){ oled.setCursor(0,0); oled.print("Lost connection"); }
//----------------------------------------------------------------------------Hiển thị các thông số lên màn hình  
  oled.setCursor(0,1);  oled.print("     "); 
  oled.setCursor(64,1); oled.print("     "); 
  oled.setCursor(0,3);  oled.print(myGPS);
//----------------------------------------------------------------------------Nhấn SOS
  if(digitalRead(SOS)==0){ Send_SOS("Help me!!!"); }
//----------------------------------------------------------------------------Reset ADMIN 
  if(digitalRead(RESET)==0)
  {
    oled.clear();
    while(digitalRead(RESET)==0) 
    {
      flag1--; delay(1000);
      oled.setCursor(15,3); oled.print("Reset ADMIN is: "); oled.print(flag1); 
      if(flag1 <= 0)
      { 
        for (int i = 0; i < 12; ++i) {EEPROM.write(i, 0);} EEPROM.commit(); delay(1000); 
        oled.setCursor(15, 3);  oled.print("Rebooting device..."); 
        delay(2000); 
        ESP.restart(); 
      } 
    }
    if(digitalRead(RESET)==1){oled.clear(); flag1 = 6;} 
  }

//----------------------------------------------------------------------------Nếu phát hiện nhịp tim
  while(particleSensor.getIR()>7000) 
  {
    fall_detection();
    if(analogRead(MQ3) >= 10000)                       { SerialBT.println("stop");  delay(1000);}
    if(analogRead(MQ3) < 10000 && millis()-timer>1000) { SerialBT.println("start"); timer = millis();}
    oled.setCursor(70,0); oled.print(analogRead(MQ3)); oled.print("   "); 
  }



}
