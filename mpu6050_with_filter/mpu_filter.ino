#include <Wire.h>

long t;
double dt;
long accelX, accelY, accelZ;
float gForceX, gForceY, gForceZ;
float angx = 0 , angy = 0 , angz = 0;

long gyroX, gyroY, gyroZ;
double rotX, rotY, rotZ;
double dx, dy, dz;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  setupMPU();
}


void loop() {

  recordGyroRegisters();


  // angle();
  printData();
}

void setupMPU() {
  Wire.beginTransmission(0b1101000);
  Wire.write(0x6B);
  Wire.write(0b00000000);
  Wire.endTransmission();
  Wire.beginTransmission(0b1101000);
  Wire.write(0x1B);
  Wire.write(0);
  Wire.endTransmission();
  Wire.beginTransmission(0b1101000);
  Wire.write(0x1C);
  Wire.write(0);
  Wire.endTransmission();
  t = micros();

}



void recordGyroRegisters() {
  Wire.beginTransmission(0b1101000);
  Wire.write(0x43);
  Wire.endTransmission();
  Wire.requestFrom(0b1101000, 6);
  while (Wire.available() < 6);
  gyroX = Wire.read() << 8 | Wire.read();
  gyroY = Wire.read() << 8 | Wire.read();
  gyroZ = Wire.read() << 8 | Wire.read();
  dt = (double)(micros() - t) / 1000000;
  t = micros();
  Wire.beginTransmission(0b1101000);
  Wire.write(0x3B);
  Wire.endTransmission();
  Wire.requestFrom(0b1101000, 6);
  while (Wire.available() < 6);
  accelX = Wire.read() << 8 | Wire.read();
  accelY = Wire.read() << 8 | Wire.read();
  accelZ = Wire.read() << 8 | Wire.read();
  processGyroData();

}

void processGyroData() {

  rotX = (float)gyroX / 131.0;
  rotY = (float)gyroY / 131.0;
  rotZ = (float)gyroZ / 131.0;
  gForceX = accelX / 16384;
  gForceY = accelY / 16384;
  gForceZ = accelZ / 16384;
  dx = dx + rotX * dt;
  dy = dy + rotY * dt;
  dz = dz + rotZ * dt;
}

void printData() {


  Serial.print("Gyro (deg)");
  Serial.print(" X=");
  Serial.print(dx);
  Serial.print(" Y=");
  Serial.print(dy);
  Serial.print(" Z=");
  Serial.println(dz);

}


void angle() {//not tested
  float tempx = sqrt(gForceY * gForceY + gForceZ * gForceZ);
  float tempy = sqrt(gForceX * gForceX + gForceZ * gForceZ);
  float tempz = sqrt(gForceY * gForceY + gForceX * gForceX);
  angx = (0.98) * (dx) + (.02) * (180 / 3.14) * (atan2(tempx , gForceX)) ;
  angy = (0.98) * (dy) + (.02) * (180 / 3.14) * (atan2(tempy , gForceY)) ;
  angz = (0.98) * (dz) + (.02) * (180 / 3.14) * (atan2(tempz , gForceZ)) ;

}

