#include <EEPROM.h>
//----------------------------------------------------------------------------Bluetooth
#include "BluetoothSerial.h"
BluetoothSerial SerialBT;
uint8_t address[6]  = {0xA0, 0xB7, 0x65, 0xFE, 0xB3, 0xEE};
//----------------------------------------------------------------------------Màn hình 
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);
//----------------------------------------------------------------------------GPS
#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>
TinyGPSPlus gps;
//----------------------------------------------------------------------------Bàn phím
#include <Keypad.h>
const byte ROWS = 4; 
const byte COLS = 4;

char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','D'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};  
byte rowPins[ROWS] = {26, 25, 33, 32}; 
byte colPins[COLS] = {13, 12, 14, 27}; 
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 
//----------------------------------------------------------------------------Loa
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>  
SoftwareSerial mySoftwareSerial(19,18); 
DFRobotDFPlayerMini myDFPlayer;
//----------------------------------------------------------------------------Biến toàn cục
int flag1; int flag2; int flag3;
String PW; String data; String datas; String sdata;
unsigned long time_reconnect;
float Lat,Lng;

void setup() {
//----------------------------------------------------------------------------Khởi động màn hình gỡ lỗi, LCD
  Serial.begin(9600);
  lcd.init();  lcd.backlight();
  EEPROM.begin(512); 
  for (int i = 0;  i < 6; ++i) { if(EEPROM.read(i)!=0) { PW  += char(EEPROM.read(i)); }} delay(500);
  Serial.println(PW);
//----------------------------------------------------------------------------Khời động loa 
  mySoftwareSerial.begin(9600);
  if (!myDFPlayer.begin(mySoftwareSerial, true, false)) { Serial.println("not"); while(true){delay(0); }   }
  myDFPlayer.volume(30);
  
//----------------------------------------------------------------------------Khởi động bluetooth 
  lcd.setCursor(0,0); lcd.print("Starting BLE");
  SerialBT.begin("ESP32test", true);  SerialBT.connect(address);
//----------------------------------------------------------------------------Khởi động GPS
  Serial2.begin(9600);
//--------------------------------------Khởi động ngoại vi
  pinMode(23, OUTPUT); digitalWrite(23, 0);

}



void loop() {
  while(!SerialBT.connected())
  {
    lcd.setCursor(0,0); lcd.print("Lost connected"); 
    digitalWrite(23, 0); 
    PASSWORD();
    if(millis() - time_reconnect >= 15000){SerialBT.connect(address); time_reconnect = millis();}
    flag3=0;
  }




  while(SerialBT.connected())
  {
    String myString = SerialBT.readString();
    if(myString.indexOf('a')>0 && flag3==0){ digitalWrite(23, 1); myDFPlayer.play(4); flag3=1; }
    if(myString.indexOf('o')>0)            { digitalWrite(23, 0); myDFPlayer.play(6); while(1); }
    
    lcd.setCursor(0,0); lcd.print("Connected       "); 
    lcd.setCursor(0,1); lcd.print(Lat, 5);
    lcd.setCursor(8,1); lcd.print(Lng, 5);

    Load_GPS();

    String DATA = DATA + String(Lat,6) +","+String(Lng,6);
    if(millis()-time_reconnect >= 10000){ SerialBT.println(DATA); time_reconnect = millis(); }   
  }
  
}
