void startDevice() {

  display.clearDisplay();
  display.drawBitmap(0, 0, speed, 20, 20, WHITE);
  display.setTextSize(2);
  display.setCursor(50, 0);
  display.print(value1);  //Data MPU

  display.drawBitmap(0, 45, rate, 20, 20, WHITE);
  display.setTextSize(2);
  display.setCursor(50, 45);
  display.print(value2);  //Data Heart
  display.display();
/*-------------------Show content-------------------*/
}
void checkAdmin() {
  for (int i = 0; i < 12; ++i) {
    ADMIN += char(EEPROM.read(i));  //Check ADMIN
  }
  Serial.println(ADMIN);
  while (ADMIN == "") {  
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
        if (count <= 0) { resetAdmin(); }
      }
      if (digitalRead(RESET) == 1) { count = 6; }
    }
  }
}
/*-------------------Check & Setting ADMIN-------------------*/
