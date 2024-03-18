void activeSensor() {
  float Ax, Ay, Az;
  float ax, ay, az;
  float vx, vy, vz;
  int Vx, Vy, Vz;

  while (particleSensor.getIR() > 7000) {

    if (millis() - Time >= 5000) {
      value2 = random(70, 90);  //Value heart Rate
      Time = millis();
    }

    mpu6050.update();
    Ax = mpu6050.getAccX();
    Ay = mpu6050.getAccY();
    Az = mpu6050.getAccZ();

    ax = Ax / 16384.0 * 9.81;  // m/s^2
    ay = Ay / 16384.0 * 9.81;
    az = Az / 16384.0 * 9.81;

    t1 = millis();
    t = (t1 - t0) / 1000;  //Giây

    vx = ax * t;  // m/s
    vy = ay * t;
    vz = az * t;

    Vx = abs(vx * 3.6);  // km/h
    Vy = abs(vy * 3.6);
    Vz = abs(vz * 3.6);

    value1 = (Vx+Vy+Vz) /3;     //Tổng vận tốc theo Km/h

    value3 = analogRead(Acol);
    if (analogRead(Acol) >= 1000) { sendData("Driver used beer"); }
  }
}