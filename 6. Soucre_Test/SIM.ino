String splitString(String inputString, String delim, uint16_t pos) {
  String tmp = inputString;
  for (int i = 0; i < pos; i++) {
    tmp = tmp.substring(tmp.indexOf(delim) + 1);
    if (tmp.indexOf(delim) == -1
        && i != pos - 1)
      return "";
  }
  return tmp.substring(0, tmp.indexOf(delim));
}
/*--------------------Split string-------------------*/

void resetAdmin() {

  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(54, 16);
  display.print("Setting ADMIN: ");
  display.display();

  while (Serial2.available()) {
    String inputString;
    String Val;
    while (Serial2.available()) { inputString = Serial2.readString(); }
    delay(65);
    Serial.println(inputString);

    if (splitString(inputString, "#", 1) == "ADMIN") {

      for (int i = 9; i < 21; i++) { Val += inputString.charAt(i); }

      for (int i = 0; i < Val.length(); ++i) { EEPROM.write(i, Val[i]); }
      EEPROM.commit();
      delay(500);

      Serial2.println("AT+CMGF=1");
      delay(500);
      Serial2.println("AT+CMGS=\"" + Val + "\"\r");
      delay(500);
      Serial2.println("ADMIN installed");
      Serial2.println((char)26);
      delay(500);

      for (int i = 14; i <= 100; i++) {
        display.clearDisplay();
        display.drawRoundRect(12, 30, 104, 8, 10, WHITE);
        display.fillRoundRect(14, 32, i, 4, 10, WHITE);
        display.setTextSize(1);
        display.setTextColor(WHITE);
        display.setCursor(30, 50);
        display.print("Loading ");
        display.print(i);
        display.print("%");
        display.display();
      }

      display.clearDisplay();
      display.setTextSize(1);
      display.setCursor(54, 16);
      display.println("ADMIN OK");
      display.display();

      delay(2000);
      ESP.restart();
    }
  }
}
/*--------------------Reset ADMIN-------------------*/


void sendData(String data) {
  Serial2.println("AT+CMGF=1");
  delay(500);
  Serial2.println("AT+CMGS=\"" + ADMIN + "\"\r");
  delay(500);
  Serial2.println(data);
  Serial2.print("http://maps.google.com/maps?z=18&q=");
  Serial2.print(locationInfo);
  Serial2.println((char)26);
  delay(5000);

  Serial2.print(F("ATD"));
  Serial2.print(ADMIN);
  Serial2.print(F(";\r\n"));
  delay(20000);
  Serial2.print(F("ATH"));
  Serial2.print(F(";\r\n"));
  delay(500);
}
/*--------------------Call & send SMS-------------------*/

void findDevice() {
  while (Serial2.available()) {
    String inputString;
    while (Serial2.available()) {
      inputString = Serial2.readString();
      delay(65);
    }
    if (splitString(inputString, "#", 1) == "MAP") {
      Serial.println("Send map");
      Serial2.println("AT+CMGF=1");
      delay(500);
      Serial2.println("AT+CMGS=\"" + ADMIN + "\"\r");
      delay(500);
      Serial2.print("http://maps.google.com/maps?z=18&q=");
      Serial2.println(locationInfo);
      Serial2.println((char)26);
      delay(500);
    }
  }
}
/*--------------------Get MAP-------------------*/

void getLocal() {
  Serial2.println("AT+CLBS=4");
  delay(5000);

  while (Serial2.available()) {
    String response = Serial2.readStringUntil('\n');
    if (response.indexOf("+CLBS: ") != -1) {

      locationInfo = response.substring(response.indexOf("+CLBS: ") + 7);
      //Serial.println("Local: " + locationInfo);
    }
  }
}
/*--------------------Get Local-------------------*/
