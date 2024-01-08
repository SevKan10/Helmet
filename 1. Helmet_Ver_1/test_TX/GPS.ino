void Load_GPS()
{
  while (gpsSerial.available()) 
  {
    if (gps.encode(gpsSerial.read())) 
    {
       if(gps.location.lat()==0)
       {
        Lat = lt.toFloat();
        Lng = lg.toFloat();     
       }
       if(gps.location.lat()!=0 && millis()-timeGPS>=5000)
       {
        Lat = gps.location.lat();
        Lng = gps.location.lng(); 
        String A, B; 
        char buffer1[15]; char buffer2[15];
        dtostrf(Lat, 7, 6, buffer1);   dtostrf(Lng, 9, 6, buffer2);  
        A = String(buffer1);  B = String(buffer2);
 //       Serial.print(A); Serial.print(","); Serial.println(B);
        for (int i = 0; i < A.length(); ++i){EEPROM.write(15+i, A[i]);} 
        for (int i = 0; i < B.length(); ++i){EEPROM.write(24+i, B[i]);}
        timeGPS = millis();
       }
  oled.setCursor(0,5); oled.print("Lat: "); oled.print(Lat, 6); 
  oled.setCursor(0,6); oled.print("Lng: "); oled.print(Lng, 6);     
    }
  }
}
