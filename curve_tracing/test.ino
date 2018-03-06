#include <Servo.h>

//rotary encoder variable declaration
#define outputAM1 2 //pin at which outputA is connected for motor 1
#define outputBM1 3 //pin at which outputB is connected for motor 1
#define outputAM2 18 //pin at which outputA is connected for motor 2
#define outputBM2 19 //pin at which outputB is connected for motor 2
int counterM1 = 0;
int counterM2 = 0;

//motor variable declaration
#define omotM1 8 //speed control pin of motor 1
#define omotM2  13 //speed control pin of motor 2
#define cmot1M1  9 //direction control pin of motor 1
#define cmot2M1  10 //direction control pin of motor 1
#define cmot1M2  11 //direction control pin of motor 2
#define cmot2M2  12 //direction control pin of motor 2
int pwmM1 = 200;
int pwmM2 = 200;
#define activeregion 3
#define permissiableError 67
#define constant 1


//pid controller variable declaration
int error;
int errorIntegral = 0;
int errorDifferential = 0;
int errorLast = 0;
int errorT = 0;
#define KP 0.2 //values are still not checked by hit n trial 
#define KI 0
#define KD 0

//function variable declaration
float x = 0;
float y , ddy;
double theta, dy;
double adjustmentValue = 0;
double veloM1 , veloM2;

//SERVO
Servo servo;

void setup() {
  Serial.begin(9600);
  servo.attach(40);
  servo.write(60);

  //encoder setup
  pinMode(outputAM2, INPUT);
  pinMode(outputBM2, INPUT);
  pinMode(outputAM1, INPUT);
  pinMode(outputBM1, INPUT);
  attachInterrupt(digitalPinToInterrupt(outputAM1), countAM1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(outputBM1), countBM1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(outputAM2), countAM2, CHANGE);
  attachInterrupt(digitalPinToInterrupt(outputBM2), countBM2, CHANGE);

  //motor setup
  pinMode(omotM1, OUTPUT);
  pinMode(omotM2, OUTPUT);
  pinMode(cmot1M1, OUTPUT);
  pinMode(cmot1M2, OUTPUT);
  pinMode(cmot2M1, OUTPUT);
  pinMode(cmot2M2, OUTPUT);

  //function setup
  y = x * x;
  dy = 2 * x;
  ddy = 2;
  theta = atan(dy);

  //initial displacement
  if (y > 0) {
    while (counterM1 == y * constant ) {
      pd();
      analogWrite(omotM1, pwmM1 - errorM1);
      analogWrite(omotM2, pwmM2 - errorM2);

      digitalWrite(cmot1M1, HIGH);
      digitalWrite(cmot2M1, LOW);
      digitalWrite(cmot1M2, HIGH);
      digitalWrite(cmot2M2, LOW);
    }
  }
  else {
    while (counterM1 == -y * constant ) {
      pd();
      analogWrite(omotM1, pwmM1 - errorM1);
      analogWrite(omotM2, pwmM2 - errorM2);

      digitalWrite(cmot1M1, LOW);
      digitalWrite(cmot2M1, HIGH);
      digitalWrite(cmot1M2, LOW);
      digitalWrite(cmot2M2, HIGH);
    }
  }
  //initial angle
  theta = theta > 1.5708 ? theta + 90 : 90 - theta; // 1.5708 = pi/2
  while (counterM1 < theta * constant) {
    analogWrite(omotM1, pwmM1);
    analogWrite(omotM2, pwmM2);

    digitalWrite(cmot1M1, LOW);
    digitalWrite(cmot2M1, HIGH);
    digitalWrite(cmot1M2, HIGH);
    digitalWrite(cmot2M2, LOW);
  }
  servo.write(30);
}


void loop() {
  t = micros();
  countIM1 = counterM1;
  countIM2 = counterM2;
  theta = atan(dy);
  Serial.print("motor 1 : ");
  Serial.println(counterM1);
  Serial.print("motor 2 : ");
  Serial.println(counterM2);

  if (ddy > 0) {
    adjustmentValue = ddy / pow((1 + sq(dy)), 1.5);
    veloM1 = 30.16;
    veloM2 = 30.16 * (1 - 10 * adjustmentValue) / (1 + 10 * adjustmentValue);
  }
  else {
    adjustmentValue = ddy / pow((1 + sq(dy)), 1.5);
    veloM2 = 30.16;
    veloM1 = 30.16 * (1 - 10 * adjustmentValue) / (1 + 10 * adjustmentValue);
  }

  pwmM1 = map(veloM1, 0, 30.16, 153, 255);
  pwmM2 = map(veloM2, 0, 30.16, 153, 255);

  pd();
  analogWrite(omotM1, pwmM1 - errorM1);
  analogWrite(omotM2, pwmM2 - errorM2);

  digitalWrite(cmot1M1, HIGH);
  digitalWrite(cmot2M1, LOW);
  digitalWrite(cmot1M2, HIGH);
  digitalWrite(cmot2M2, LOW);

  speedM1 = (counterM1 - countIM1) / t * 1000000 ;
  speedM2 = (counterM2 - countIM2) / t * 1000000;
  x += (counterM1 - countIM1) * cos(theta) * constant;

  // if ((counterM1 + counterM2) < -permissiableValue || (counterM2 + counterM1) > permissiableValue ) {
  //
  //    //    errorpid();
  //
  //    if (abs(counterM1) >  counterM2) {
  //
  //      analogWrite(omotM1, pwmM1 );
  //      analogWrite(omotM2, pwmM2);
  //
  //      digitalWrite(cmot1M1, HIGH);
  //      digitalWrite(cmot2M1, LOW);
  //      digitalWrite(cmot1M2, HIGH);
  //      digitalWrite(cmot2M2, LOW);
  //    }
  //    else {
  //
  //      analogWrite(omotM1, pwmM1);
  //      analogWrite(omotM2, pwmM2);
  //
  //      digitalWrite(cmot1M1, HIGH);
  //      digitalWrite(cmot2M1, LOW);
  //      digitalWrite(cmot1M2, HIGH);
  //      digitalWrite(cmot2M2, LOW);
  //
  //    }
  //  }

}

void countAM1() {
  if ( digitalRead ( outputAM1 ) == HIGH) {
    if ( digitalRead ( outputBM1 ) == LOW) {
      counterM1 = counterM1 + 1 ;
    }
    else {
      counterM1 = counterM1 - 1 ;
    }
  }
  else {
    if ( digitalRead(outputBM1 ) == HIGH) {
      counterM1 = counterM1 + 1 ;
    }
    else {
      counterM1 = counterM1 - 1 ;

    }
  }
}


void countBM1 () {
  if (digitalRead(outputBM1) == HIGH) {
    if (digitalRead(outputAM1) == HIGH) {
      counterM1 = counterM1 + 1;
    }
    else {
      counterM1 = counterM1 - 1;
    }
  }
  else {
    if (digitalRead(outputAM1) == LOW) {
      counterM1 = counterM1 + 1;
    }
    else {
      counterM1 = counterM1 - 1;
    }
  }
}

void countAM2() {
  if ( digitalRead ( outputAM2 ) == HIGH) {
    if ( digitalRead ( outputBM2 ) == LOW) {
      counterM2 = counterM2 + 1 ;
    }
    else {
      counterM2 = counterM2 - 1 ;
    }
  }
  else {
    if ( digitalRead(outputBM2 ) == HIGH) {
      counterM2 = counterM2 + 1 ;
    }
    else {
      counterM2 = counterM2 - 1 ;

    }
  }
}


void countBM2 () {
  if (digitalRead(outputBM2) == HIGH) {
    if (digitalRead(outputAM2) == HIGH) {
      counterM2 = counterM2 + 1;
    }
    else {
      counterM2 = counterM2 - 1;
    }
  }
  else {
    if (digitalRead(outputAM2) == LOW) {
      counterM2 = counterM2 + 1;
    }
    else {
      counterM2 = counterM2 - 1;
    }
  }
}

void pd() {
  peM1 = speedM1 - veloM1; 
}
//void errorpid () {
//
//  error = abs(counterM1 - counterM2);
//  errorDifferential = error - errorLast;
//
//  if (errorT != 0 && errorIntegral < activeregion) {
//    errorIntegral += error;
//  }
//  else{
//      errorIntegral = 0;
//  }
//  if (errorT > 40){
//    errorT = 40;
//  }
//
//  errorT = KP * error + KI * errorIntegral + KD * errorDifferential;
//  errorLast = error;
//
//}


