String a;

void setup() {
  Serial.begin(9600);

}

void loop() {
  while (Serial.available() == 0) {}
  int i ;
  while (Serial.available() != 0) {
    i = Serial.read();
    a = a + char(i);
    delay(10);
  }
  Serial.print(a);
}
