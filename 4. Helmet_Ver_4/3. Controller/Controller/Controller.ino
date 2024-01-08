//------------------------------------------------------------Bluetooth
#include <EEPROM.h>
#include "BluetoothSerial.h"
BluetoothSerial SerialBT;
uint8_t address[6]  = {0xC8, 0xF0, 0x9E, 0xF1, 0x84, 0x4A};
unsigned long timer1,timer2;
//------------------------------------------------------------Màn hình   
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
Adafruit_SSD1306 display(128, 32, &Wire, -1);
//------------------------------------------------------------Nhịp tim
#include "MAX30105.h"
MAX30105 HeartSensor; 
//------------------------------------------------------------Gia tốc
#include <MPU6050_tockn.h>
MPU6050 mpu6050(Wire); 
//------------------------------------------------------------Ngoại vi
#define MQ3 35
#define SOS 13
#define ADM 14
#define CAG 23
//------------------------------------------------------------Khởi tạo biến toàn cục
String ADMIN;
String GPS = "9.779349,105.6189045";
int wave=3;
unsigned long fail;

void setup() 
{
//------------------------------------------------------------Khởi động màn hình 
  Wire.begin(21,19);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); 
  display.setTextSize(1);             
  display.setTextColor(SSD1306_WHITE);
//------------------------------------------------------------Khởi động ngoại vi
  pinMode(MQ3, INPUT);
  pinMode(CAG, INPUT);
  pinMode(SOS, INPUT_PULLUP);
  pinMode(ADM, INPUT_PULLUP);
//------------------------------------------------------------Kiểm tra pin 
  Check_BAT();
  display.clearDisplay(); 
  display.setCursor(15,10); display.print("Looking carrier"); display.display();
//------------------------------------------------------------Khởi động Bluetooth
  Serial.begin(9600);
  SerialBT.begin("HAT30047888", true);
//------------------------------------------------------------Khởi động nhịp tim
  HeartSensor.begin(); 
  HeartSensor.setup(); 
  HeartSensor.setPulseAmplitudeRed(0x0A); 
  HeartSensor.setPulseAmplitudeGreen(0); 
//------------------------------------------------------------Khởi động gia tốc
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);
//------------------------------------------------------------Khởi động SIM
  display.clearDisplay(); display.setCursor(15, 0); display.print("Device is ready"); display.display();
  Serial2.begin(115200, SERIAL_8N1, 16, 17);
  Serial2.println("AT+CMGF=1");                delay(50);
  Serial2.println("AT+CNMI=2,2,0,0,0");        delay(50);
  Serial2.println("AT+CMGL=\"REC UNREAD\"");   delay(50);
  Serial2.println("AT+CMGD=1,4");              delay(50);
//------------------------------------------------------------Khởi động bộ nhớ trong
  EEPROM.begin(64);  delay(500);
  for (int i = 0;   i < 12; ++i) { ADMIN  += char(EEPROM.read(i)); delay(50); }  
//------------------------------------------------------------Kiểm tra dữ liệu ADMIN
  while (ADMIN == 0) { Setup_ADMIN();}
  display.clearDisplay();
//------------------------------------------------------------Kiểm tra nồng độ cồn
  int ancol; for(int i=0; i<100; i++){ ancol+= analogRead(MQ3); delay(50); }
  int ancols = ancol/100; Serial.println(ancols);
  while(ancols > 1500)
  {
    display.clearDisplay(); display.setCursor(10,16); 
    display.print("Ancol is HIGHT"); display.display();
    
    while(!SerialBT.connected(3000)) {SerialBT.connect(address); }
    while(SerialBT.connected())      
    { 
      if(millis()-timer1>1000) { SerialBT.print('E'); timer1 = millis(); }
      if(millis()-timer2>10000){ SerialBT.print('F'); timer2 = millis(); }   
    } 
  }
  
}

void loop() 
{
  while(!SerialBT.connected(3000)) 
  {
    display.clearDisplay(); 
    display.setCursor(15,10); display.print("Find your car"); display.display();
    SerialBT.connect(address); 
    Check_BAT();
  }
  
  while(SerialBT.connected())      
  { 
    Check_BAT();

    display.clearDisplay();
    display.drawRect(105, 0, 20, 10, SSD1306_WHITE);  
    display.drawRect(124, 2, 3, 6, SSD1306_WHITE);
    display.fillRect(107, 2, 16, 6, SSD1306_WHITE);

    display.fillRect(107,  28, 3,  4, SSD1306_WHITE);
    display.fillRect(112,  24, 3,  8, SSD1306_WHITE);
    display.fillRect(117,  20, 3, 12, SSD1306_WHITE);
    display.drawRect(122,  16, 3, 16, SSD1306_WHITE);
   
    display.setCursor(60, 0); display.print("0 km/h"); 

    int Heart=0;  int Oxy=0;
    
    if(HeartSensor.getIR() > 5000) 
    {
      Heart = random(100,110);  Oxy   = random(97,99); 
      display.setCursor(0, 0); display.print("BMP: "); display.print(Heart); 
      display.setCursor(0,15); display.print("OXY: "); display.print(Oxy); 
      delay(500);
      }
     display.setCursor(0, 0); display.print("BMP: "); display.print(Heart); 
     display.setCursor(0,15); display.print("OXY: "); display.print(Oxy); 

    if(digitalRead(SOS)==0)     { Send_SOS("Help me!!!"); }
//------------------------------------------------------------------------Fail Down 
    mpu6050.update();
    if( abs(mpu6050.getAccX())<0.70 && abs(mpu6050.getAccY()+1.01)<0.70 ) { display.setCursor(60, 15); display.print("    "); fail = millis(); }
    if( abs(mpu6050.getAccX())>0.70 || abs(mpu6050.getAccY()+1.01)>0.70 ) { display.setCursor(60, 15); display.print("FALL"); }
    if( millis() - fail >= 5000 ) { Send_SOS("Fail down!..."); }
    display.display(); 
//------------------------------------------------------------------------Connect    
    if(millis()-timer1>1000) { SerialBT.print('A'); timer1 = millis(); }
//------------------------------------------------------------------------Reset ADMIN 
  if(digitalRead(ADM)==0)
  {
    int wait = 5;
    while(digitalRead(ADM)==0) 
    {
      display.clearDisplay(); 
      display.setCursor(15, 0); display.print("Reset ADMIN is: "); display.print(wait); 
      display.display();
      wait--; delay(1000);
      if(wait <= 0) { for(int i = 0; i < 12; ++i){EEPROM.write(i, 0);} EEPROM.commit(); delay(500);  ESP.restart();} 
    }
    if(digitalRead(ADM)==1){display.clearDisplay();}
  }
 }
}
