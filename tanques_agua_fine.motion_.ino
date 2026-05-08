
int tank1_level;
int tank2_level;
int tank3_level;
bool tank1_shutdown;
bool tank2_shutdown;
bool tank3_shutdown;

void setup() {
  Serial.begin(9600);
  Serial.println("Circuito de Tanques - Aguilas Imperiales 2026 - E2");
    // pin config setting for mode operation for Pott. Controller
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  // pin config setting for mode operation for LED Bombs
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  // Pin config Setting for mode operation for LED Water Tanks ElectroValves
  pinMode(2, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(7, OUTPUT);
}

void loop() {
  // Mapping zone
  tank1_level = map(analogRead(A0), 0, 1023, 100, 0);
  tank2_level = map(analogRead(A1), 0, 1023, 100, 0);
  tank3_level = map(analogRead(A2), 0, 1023, 100, 0);

// bool config
// Tanque 1
if (tank1_level <= 15) {
  tank1_shutdown = false;
} else {
  tank1_shutdown = true;
};
// Tanque 2
if (tank2_level <= 10) {
  tank2_shutdown = false;
} else {
  tank2_shutdown = true;
};
//Tanque 3
if (tank3_level <= 10) {
  tank3_shutdown = false;
} else {
  tank3_shutdown = true;
};
// <--------->
// Electrovalvulas
if (tank1_level <= 70) {
  digitalWrite(2, HIGH);
} else if (tank1_level == 100) {
  digitalWrite(2, LOW);
};

if (tank2_shutdown) {
  digitalWrite(4, HIGH);
}else if (!tank2_shutdown) {
  digitalWrite(4, LOW);
};

if (tank3_shutdown) {
  digitalWrite(7, HIGH);
}else if (!tank3_shutdown) {
  digitalWrite(7, LOW);
};



// Bombas
// Si el nivel del tanque 3 o 2, esta por debajo de cierto nivel se activen las bombas
// siempre y cuando el tanque 1 se los permita

if (tank3_level <= 75 && tank1_shutdown) {
  digitalWrite(10, HIGH);
} else if (tank3_level == 100 || !tank1_shutdown) {
  digitalWrite(10, LOW);
};

if (tank2_level <= 73 && tank1_shutdown) {
  digitalWrite(9, HIGH);
} else if (tank2_level == 100 || !tank1_shutdown) {
  digitalWrite(9, LOW);
};

Serial.print("T1: ");Serial.print(tank1_level);Serial.print("%  ");
Serial.print("T2: ");Serial.print(tank2_level);Serial.print("%  ");
Serial.print("T3: ");Serial.print(tank3_level);Serial.print("%  ");
Serial.print("Blq: ");Serial.print(tank1_shutdown);Serial.print("  ");
Serial.print("Blq: ");Serial.print(tank2_shutdown);Serial.print("  ");
Serial.print("Blq: ");Serial.print(tank3_shutdown);Serial.println("  ");

}
