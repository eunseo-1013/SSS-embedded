void setup() {
//  pinMode(A0, INPUT); analog는 굳이 pinmode 필요는 없다
  Serial.begin(9600);
  pinMode(9, INPUT);
}

void loop() {
  long res = analogRead(A0);
  Serial.println(res);
  res = res/4;
  res = map(res, 0, 255);
  analogWrite(9, res);
}
