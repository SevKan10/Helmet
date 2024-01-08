void fall_detection()
{
  oled.setCursor(0,1);  oled.print(X); oled.print("   "); 
  oled.setCursor(64,1); oled.print(Y); oled.print("   "); 
//----------------------------------------------------------------------------Đọc dữ liệu MPU6050 và calip 
  mpu6050.update();
  X = mpu6050.getAccX() - 0.70;
  Y = mpu6050.getAccY() + 0.13;
//----------------------------------------------------------------------------Kiểm tra té ngã
  if(flag2==0){fall = millis(); }
  if(abs(X)>0.70 || abs(Y)>0.50 && flag2==0) {flag2=1; oled.setCursor(0,2); oled.print("FALL"); }
  if(abs(X)<0.70 && abs(Y)<0.50) {flag2=0; oled.setCursor(0,2); oled.print("            ");}
  if(millis()-fall >= 3000) { Send_SOS("Fail Down!!!"); } 

//----------------------------------------------------------------------------Bluetooth kết nối, lấy dữ liệu GPS từ xe
  if (SerialBT.available()) 
  {
    gps = SerialBT.readString(); 
    if(gps.indexOf(',')>0){ myGPS = gps; }
  }
//----------------------------------------------------------------------------Nhấn SOS
  if(digitalRead(SOS)==0){ Send_SOS("Help me!!!"); }

  
}
