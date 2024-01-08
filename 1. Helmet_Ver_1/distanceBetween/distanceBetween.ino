#include <TinyGPS++.h>
TinyGPSPlus gps;
const float targetLat = 10.05448; // Vĩ độ vị trí cần khoanh vùng
const float targetLng = 105.78722; // Kinh độ vị trí cần khoanh vùng
void setup() {
  Serial.begin(9600);

    // Tính khoảng cách giữa vị trí hiện tại và vị trí cần khoanh vùng
    float distance = TinyGPSPlus::distanceBetween(
      10.05636, 
      105.78601, 
      targetLat,
      targetLng
    );

    Serial.print("Distance to target: ");
    Serial.print(distance / 1000, 2); // Chuyển đổi đơn vị sang km và làm tròn đến 2 chữ số thập phân
    Serial.println(" km");
}

void loop() {

}
