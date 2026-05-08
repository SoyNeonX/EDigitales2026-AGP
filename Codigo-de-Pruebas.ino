void setup(){
    Serial.begin(9600);
    Serial.println("Codigo de Prueba - Aguilas Imperiales 2026 - E2");
    //Todos los pines configurados como OUTPUT para probar configuracion
    pinMode(3, OUTPUT);
    pinMode(4, OUTPUT);
    pinMode(7, OUTPUT);
    pinMode(9, OUTPUT);
    pinMode(10, OUTPUT);
    pinMode(11, OUTPUT);
    pinMode(12, OUTPUT);
    pinMode(13, OUTPUT);
    pinMode(5, OUTPUT);
    pinMode(6, OUTPUT);
    pinMode(8, OUTPUT);
    pinMode(2, OUTPUT);
}   

void loop(){
    for(int i = 1; i <= 14; i++){
        digitalWrite(i, HIGH);
        Serial.print("Pin ");
        Serial.print(i);
        Serial.println(" encendido");
        delay(500);
        digitalWrite(i, LOW);
        delay(500);
    }

}