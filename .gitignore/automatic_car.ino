#include <Servo.h>

int trigpin = 7;
int echopin = 6;
int pin5 = 5;
int omot1 = 10;
int omot2 = 11;
int cmot11 = 8;
int cmot12 = 9;
int cmot21 = 12;
int cmot22 = 13;
int distmove = 27;
float pulsetime;
float pulsetime1;
float pulsetime2;
float dist;
float dist1;
float dist2;

Servo myservo;

void setup() {
  Serial.begin(9600);
  pinMode(trigpin, OUTPUT);
  pinMode(echopin, INPUT);
  pinMode(omot1, OUTPUT);
  pinMode(omot2, OUTPUT);
  pinMode(cmot11, OUTPUT);
  pinMode(cmot12, OUTPUT);
  pinMode(cmot21, OUTPUT);
  pinMode(cmot22, OUTPUT);
  myservo.attach(pin5);

}

void loop() {
  myservo.write(95);
  delay(250);
  digitalWrite(trigpin, LOW);
  delayMicroseconds(200);
  digitalWrite(trigpin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigpin, LOW);
  pulsetime = pulseIn(echopin, HIGH);
  dist = 347 * pulsetime / 20000;
  delay(500);

  myservo.write(0);
  digitalWrite(trigpin, LOW);
  delayMicroseconds(200);
  digitalWrite(trigpin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigpin, LOW);
  pulsetime1 = pulseIn(echopin, HIGH);
  dist1 = 347 * pulsetime1 / 20000;
  delay(500);
  myservo.write(95);
  delay(250);

  myservo.write(180);
  digitalWrite(trigpin, LOW);
  delayMicroseconds(200);
  digitalWrite(trigpin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigpin, LOW);
  pulsetime2 = pulseIn(echopin, HIGH);
  dist2 = 347 * pulsetime2 / 20000;
  delay(500);
  myservo.write(95);
  delay(250);

  Serial.println(dist);
  delay(250);
  Serial.println(dist1);
  delay(250);
  Serial.println(dist2);
  delay(250);

  if (dist >= distmove) {
    analogWrite(omot1, 255);
    analogWrite(omot2, 255);

    digitalWrite(cmot11, HIGH);
    digitalWrite(cmot12, LOW);
    digitalWrite(cmot21, HIGH);
    digitalWrite(cmot22, LOW);

    delay(1000);
  }
  else if (dist <= distmove && dist1 >= dist2) {
    analogWrite(omot1, 255);
    analogWrite(omot2, 255);

    digitalWrite(cmot11, HIGH);
    digitalWrite(cmot12, LOW);
    digitalWrite(cmot21, LOW);
    digitalWrite(cmot22, HIGH);

    delay (500);
  }
  else if (dist <= distmove && dist1 <= dist2 ) {
    analogWrite(omot1, 255);
    analogWrite(omot2, 255);

    digitalWrite(cmot11, LOW);
    digitalWrite(cmot12, HIGH);
    digitalWrite(cmot21, HIGH);
    digitalWrite(cmot22, LOW);

    delay(500);

  }
  else {
    analogWrite(omot1, 100);
    analogWrite(omot2, 255);

    digitalWrite(cmot11, LOW);
    digitalWrite(cmot12, HIGH);
    digitalWrite(cmot21, LOW);
    digitalWrite(cmot22, HIGH);

    delay(250);
  }

  analogWrite(omot1, 0);
  analogWrite(omot2, 0);

  digitalWrite(cmot11, LOW);
  digitalWrite(cmot12, HIGH);
  digitalWrite(cmot21, LOW);
  digitalWrite(cmot22, HIGH);

  delay(250);
}
