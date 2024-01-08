//------------------------------------------------------Bluetooth
#include "BluetoothSerial.h"
BluetoothSerial SerialBT;
#include <EEPROM.h>
//------------------------------------------------------Màn hình
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
Adafruit_SH1106G display(128, 64, &Wire, -1);
//------------------------------------------------------Gia tốc
#include <MPU6050_tockn.h>
MPU6050 mpu6050(Wire);
//------------------------------------------------------Giọng nói
#include <DFRobotDFPlayerMini.h>  
DFRobotDFPlayerMini myDFPlayer;
//------------------------------------------------------Ngoại vi
#define SW1 32
#define SW2 33
#define SW3 26
#define SW4 25
#define BUZ 23
#define REL 27
//------------------------------------------------------Biến toàn cục
int key[6] = {0,0,0,0,0,0};  //Mảng chứa dữ liệu nhập
int num1 = 1; int num2 = 1;  //Thứ tự key
int x;                       //Vị trí ô đang nhập
int lock = 1;                //Trạng thái khoá
int flag = 0;                //Số lần mở khoá
String DATA = "";            //Dữ liệu nhập dạng chuỗi
String PW = "";              //Mật khẩu
unsigned long ping; 


void setup() {
  Serial.begin(9600);
  SerialBT.begin("EMS16400078"); 
  EEPROM.begin(16); 
  for (int i = 0; i < 4; ++i){PW += char(EEPROM.read(i)); }
//------------------------------------------------------Khởi động i2c, màn hình
  Wire.begin(18,19);
  display.begin(0x3C,true);
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.clearDisplay();
//------------------------------------------------------Khởi động cảm biến gia tốc
  display.clearDisplay(); display.setCursor(10,25); display.print("Start Acceleration"); display.display();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);
//------------------------------------------------------Khởi động giọng nói
  Serial2.begin(9600);
  myDFPlayer.begin(Serial2, true, false);
//------------------------------------------------------Khởi động ngoại vi
  pinMode(SW1, INPUT);
  pinMode(SW2, INPUT);
  pinMode(SW3, INPUT);
  pinMode(SW4, INPUT);
  
  pinMode(BUZ, OUTPUT);
  pinMode(REL, OUTPUT);
}

void loop()
{
  while(millis() - ping > 3000 and lock == 1)
  {
    digitalWrite(REL,0);

    if(SerialBT.available())
    {
    char Datas = SerialBT.read();
    if(Datas == 'A' or Datas == 'E' or Datas == 'F') { ping = millis(); }
    }

    set_num();
    set_key();
    load_pw();
  
    x = 7+(24*(num1-1)); 
      
    display.setTextSize(2);
    display.clearDisplay();
    display.setCursor(10,25);
    display.print(key[1]); display.print(" "); display.print(key[2]); display.print(" "); display.print(key[3]); display.print(" "); display.print(key[4]);
    
    display.drawCircle(110, 28, 3, SH110X_WHITE);
    display.drawLine(110, 31, 110, 38, SH110X_WHITE);
    display.drawLine(111, 36, 113, 36, SH110X_WHITE);
    display.drawLine(111, 34, 112, 34, SH110X_WHITE);
    
    display.drawRect(x, 22, 16, 20, SH110X_WHITE);
    display.display();
  }
    
  while(lock == 0)
  {
    set_num();
    set_key();
    int flag1 = 0; 
    
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(15,20); display.print("Unlock Device"); 
    display.setCursor(15,45); display.print("Change Password"); 
  
    x = 15+(25*num2);  
    display.drawRect(10, x, 100, 17, SH110X_WHITE);
    display.display();
  
    if( num2==0 and (digitalRead(SW1)==1 or digitalRead(SW2)==1) )
    { 
      flag1=1;
      myDFPlayer.play(4); 
      digitalWrite(REL,1);
      display.clearDisplay(); 
      display.setCursor(0,20); 
      display.print("Device unlocked"); 
      display.display();
      delay(1500);
      myDFPlayer.play(5);
      while(flag1==1)
      {

      }
    }
    
    if( num2==1 and (digitalRead(SW1)==1 or digitalRead(SW2)==1) )
    { 
      flag1=1; DATA="";
      while(flag1==1) { set_pw(); }
    }
  }
  


  if(millis() - ping < 3000){ myDFPlayer.play(4); }
  while(millis() - ping < 3000)
  {
    mpu6050.update();
    display.clearDisplay(); 
    display.setCursor(10,0); 
    display.print("Connected"); 
    display.display(); 

    if(SerialBT.available())
    {
      char Datas = SerialBT.read();
      if(Datas == 'A'){ ping = millis(); digitalWrite(REL,1);  }
      if(Datas == 'S'){ ping = millis(); digitalWrite(REL,1); myDFPlayer.play(3); delay(25000); ping = millis(); }
      
      if(Datas == 'E'){ ping = millis(); digitalWrite(REL,0); }
      if(Datas == 'F'){ ping = millis(); digitalWrite(REL,0); myDFPlayer.play(6); } 
           
    }
  }
  
}
