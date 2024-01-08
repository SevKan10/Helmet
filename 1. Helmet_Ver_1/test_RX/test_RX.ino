#include <EEPROM.h>
//--------------------------------------------------------RF
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(9, 10); // CE, CSN
const byte diachi[] = "01234";  
//--------------------------------------------------------RF
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);
//--------------------------------------------------------Key Pad
#include <Keypad.h>
const byte ROWS = 4; 
const byte COLS = 4;

char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','D'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};  
byte rowPins[ROWS] = {A0, A1, A2, A3};
byte colPins[COLS] = {4, 5, 6, 7};

Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 
//--------------------------------------------------------Cấu trúc Data
struct package_receive{ int TX_Connect; };
package_receive data_receive;
//--------------------------------------------------------Khởi tạo ngoại vi
#define Buzze    2        //Còi
#define Power    3        //Kích nguồn
//--------------------------------------------------------Hàm Reset
void(* resetFunc) (void) = 0;
//--------------------------------------------------------Khởi tạo biến
unsigned long interrupt = 0; //Time chạy RF
unsigned long ping = 0;      //Độ trì hoãn
String data; String datas; String sdata;
String PW="";
int flag1;
int flag2;

void setup() 
{
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();

  for (int i = 0;  i < 6; ++i) { if(EEPROM.read(i)!=0) { PW  += char(EEPROM.read(i)); }} delay(500);
  Serial.println(PW);
  
  if (radio.begin()) { Serial.println("NRF24L01 Start!"); } 
  radio.openReadingPipe(1, diachi);
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate(RF24_250KBPS);
  radio.startListening();

  pinMode(Buzze,  OUTPUT);
  pinMode(Power,  OUTPUT);

  lcd.setCursor(0,0); lcd.print("Device is ready!");
  delay(2000);
  lcd.clear();

  
}

void loop() 
{

 NRF();
} 
