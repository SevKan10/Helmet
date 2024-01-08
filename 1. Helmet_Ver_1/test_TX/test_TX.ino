 
 //--------------------------------------------------------OLED
#include "SSD1306Ascii.h"
#include "SSD1306AsciiAvrI2c.h" 
SSD1306AsciiAvrI2c oled;
//--------------------------------------------------------SIM - EPROM
#include <SoftwareSerial.h>
#include <EEPROM.h>
SoftwareSerial simSerial(2, 3); 
//--------------------------------------------------------GPS 
#include <AltSoftSerial.h>
AltSoftSerial gpsSerial;
#include <TinyGPS++.h>
TinyGPSPlus gps;
//--------------------------------------------------------RF 
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(9, 10); // CE, CSN
const byte diachi[6] = "01234"; 
//--------------------------------------------------------MPU
#include "I2Cdev.h"
#include "MPU6050.h"

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif
MPU6050 accelgyro;
int16_t ax, ay, az;  int16_t gx, gy, gz;
#define OUTPUT_READABLE_ACCELGYRO

//--------------------------------------------------------Cấu trúc Data
struct package_send{ int TX_Connect; };
package_send data_send;
//--------------------------------------------------------Hàm Reset
void(* resetFunc) (void) = 0;
//--------------------------------------------------------Khởi tạo biến
unsigned long interrupt; //Time chạy RF
unsigned long timeGPS;   //Time lưu GPS
unsigned long fall;      //Time tính té ngã
unsigned long timeLight; //Time delay đèn
String ADMIN;            //Số ĐT ADMIN
String lt, lg;           //Tọa độ trong EPPROM
float Lat, Lng;          //Toạ độ trực tiếp
int flag2;               //Đếm ngược reset  

//--------------------------------------------------------Khởi tạo nút nhấn
#define SOS_btn  4        //Nút SOS
#define RS_Phone 5        //Nút xoá số đt ADMIN
#define Lux      6        //Cảm biến ánh sáng
#define Ancol    A0       //Cảm biến cồn
#define Buzz     A2       //Còi
#define Light    A3       //Đèn

void setup() 
{
  Serial.begin(9600);
  oled.begin(&Adafruit128x64, 0x3C);
  oled.setFont(System5x7);

  for(int i=15; i>0; i--){oled.setCursor(0,0); oled.print("Ready is: "); oled.print(i); oled.print("  ");delay(1000);}
  
  pinMode(SOS_btn,  INPUT_PULLUP);
  pinMode(RS_Phone, INPUT_PULLUP);
  pinMode(Lux,      INPUT);
  pinMode(Ancol,    INPUT);  
  pinMode(Buzz,     OUTPUT);
  pinMode(Light,    OUTPUT);
  
  simSerial.begin(9600);
  simSerial.println("AT+CMGF=1");   delay(500);  
  simSerial.println("AT+CNMI=2,2,0,0,0");   delay(500);
  simSerial.println("AT+CMGL=\"REC UNREAD\"");   delay(500); 

  for (int i = 0;  i < 12; ++i) { ADMIN  += char(EEPROM.read(i)); }
  for (int i = 15; i < 23; ++i) { lt     += char(EEPROM.read(i)); }
  for (int i = 24; i < 34; ++i) { lg     += char(EEPROM.read(i)); }
  Serial.print(ADMIN); Serial.print(" | "); Serial.print(lt); Serial.print(","); Serial.println(lg); 

  gpsSerial.begin(9600);          

  if (ADMIN==0){
    oled.setCursor(0,0);  oled.print(" Setting ADMIN  ");
    while (ADMIN==0) {Setup_ADMIN();}
  }

  if (radio.begin()) { Serial.println("NRF24L01 Start!"); } 
  radio.openWritingPipe(diachi); 
  radio.setPALevel(RF24_PA_MIN); 
  radio.setDataRate(RF24_250KBPS);  

  #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    Wire.begin();
  #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
    Fastwire::setup(400, true);
  #endif
  accelgyro.initialize();
  
  oled.setCursor(0,0);  oled.print("Device is ready!");  delay(1000); oled.clear();
}
void loop() 
{
  int flag = 5;
  Load_GPS();
 if(digitalRead(Lux) == 1) { timeLight = millis(); digitalWrite(Light, 1); }
 if(millis() - timeLight >= 3000) { digitalWrite(Light, 0); }

//-----------------------------------------------------------SOS  
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  #ifdef OUTPUT_READABLE_ACCELGYRO
    oled.setCursor(0,3);  oled.print(ax); oled.print("  |  ");   oled.print(ay);
  #endif
  
  if(digitalRead(SOS_btn)==0 || analogRead(Ancol)>1000)
  {
    oled.setCursor(0,0); oled.print("Send SOS!!!          "); 
    Send_SOS();
    oled.clear();
  }

  if(flag2==0){fall = millis(); }
  if(abs(ax)>13000 || abs(ay)>14000 && flag2==0) {flag2=1; oled.setCursor(0,0); oled.print("FALL"); }
  if(abs(ax)<13000 && abs(ay)<14000) {flag2=0; oled.setCursor(0,0); oled.print("            ");}
  if(millis()-fall >= 3000)
  {
    oled.setCursor(0,0); oled.print("Send SOS!!!          "); 
    Send_SOS();
    oled.clear();
  }
//-----------------------------------------------------------Reset ADMIN và chế độ lấy MAP
  if(digitalRead(RS_Phone)==0)
  {
    oled.clear();
    while(digitalRead(RS_Phone)==0) 
    {
      oled.setCursor(0,0); oled.print("Reset ADMIN is: "); oled.print(flag); 
      flag--; delay(1000);
      if(flag <= 0){ for (int i = 0; i < 12; ++i) {EEPROM.write(i, 0);} delay(500); resetFunc();} 
    }
    if(digitalRead(RS_Phone)==1){oled.clear();} 
  }
//-----------------------------------------------------------Load RF 
  if(millis() - interrupt >= 1000) 
  {
   data_send.TX_Connect = 5;
   radio.write(&data_send, sizeof(package_send));
   interrupt = millis();
  } 
}
