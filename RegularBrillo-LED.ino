
  void setup() {
    pinMode(5, OUTPUT);
    pinMode(A0, INPUT);
    Serial.begin(9600);

  }

  void loop() {
    int lighting_led = map(analogRead(A0),0,1023,0,255);
    Serial.print(analogRead(A0));
    Serial.print("  LUMEN:  ");
    Serial.print(lighting_led);
    Serial.println("  ");
    analogWrite(5, lighting_led);

  }
