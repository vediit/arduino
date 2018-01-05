#define encoderValue 57.2977

#define outputAM2 7 //pin at which outputA is connected for motor 2
#define outputBM2 6 //pin at which outputB is connected for motor 2

#define outputAM1 5 //pin at which outputA is connected for motor 1
#define outputBM1 4 //pin at which outputB is connected for motor 1

#define omotM1  10 //speed control pin of motor 1
#define omotM2  11 //speed control pin of motor 2
#define cmot1M1  8 //direction control pin of motor 1
#define cmot2M1  9 //direction control pin of motor 1
#define cmot1M2  12 //direction control pin of motor 2
#define cmot2M2  13 //direction control pin of motor 2

float x , func[10];// function assigning variable
double slope[10]; // slope storing
bool forOnce = true;

//rotary encoder
bool ALastM1, AcurrentM1;
bool ALastM2, AcurrentM2;
int counterM1 = 0;
int counterM2 = 0;


void setup() {
  Serial.begin(9600);
  x = -500;
  pinMode(outputAM1, INPUT);
  pinMode(outputBM2, INPUT);


}

void loop() {
  for (int i = 0; i < 10; i++) {
    func[i] = x + 1; // for some x value of function is stored
    x++;
  }

  for (int i = 0; i < 9; i++) {
    slope[i] = atan2(func[i + 1] - func[i], 1) * 57.29577; // slope of tangent at x is stored
  }

  ALastM1 = digitalRead(outputAM1);
  ALastM2 = digitalRead(outputBM2);

  if (forOnce) {
    while (countM1 <= int(func[0]) && countM2 <= int(func[0])) {
      if (countM1 == countM2) {
        analogWrite(omotM1, 150);
        analogWrite(omotM2, 150);

        digitalWrite(cmot1M1, HIGH);
        digitalWrite(cmot2M1, LOW);
        digitalWrite(cmot1M2, HIGH);
        digitalWrite(cmot2M2, LOW);
      }
      if ()
    }

    counterM1 = 0;
    counterM2 = 0;

    while (slope) {

    }
  }
  forOnce = false;

  for (int i = 1; i < 10; i++) {
    while (countM1 <= int(func[i] - func[i - 1]) && countM2 <= int(func[i] - func[i - 1])) {

      analogWrite(omotM1, 150);
      analogWrite(omotM2, 150);

      digitalWrite(cmot1M1, HIGH);
      digitalWrite(cmot2M1, LOW);
      digitalWrite(cmot1M1, HIGH);
      digitalWrite(cmot2M2, LOW);

    }
  }
}


int countM1 () {
  AcurrentM1 = digitalRead(outputAM1);
  if (AcurrentM1 != ALastM1) {
    if (digitalRead(outputBM1) != AcurrentM1) {
      counterM1++;
    }
    else {
      counterM1--;
    }
    ALastM1 = AcurrentM1;
  }
  return counterM1;
}

int countM2 () {
  AcurrentM2 = digitalRead(outputAM2);
  if (AcurrentM2 != ALastM2) {
    if (digitalRead(outputBM2) != AcurrentM2) {
      counterM2++;
    }
    else {
      counterM2--;
    }
    ALastM2 = AcurrentM2;
  }
  return counterM2;
}


