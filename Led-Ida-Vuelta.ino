const int pin_pulsador = 12;
int pulsador_status = 0;
int selector_caso_prueba = 0;
int value = 0;
void setup() {
    DDRB = B001111;
    DDRD = B11111100;
  Serial.begin(9600);
  pinMode(A1, INPUT);
  
  
}

void loop() {

 value = analogRead(A1);
  
pulsador_status = digitalRead(pin_pulsador);
Serial.print(pulsador_status);

if(pulsador_status == 1){
    selector_caso_prueba = 1;
    Serial.print(" ;  1-Status");
}else {
    Serial.print(" ;  0-Status");
    
}

    
    
    switch(selector_caso_prueba){
        case 0:
            for(int i = 2; i < 12; i++){
                digitalWrite(i, HIGH);
                delay(value);
                digitalWrite(i, LOW);
              
            
        };
     Serial.print(" ;  I+ ");
          selector_caso_prueba = 0;
            break;
        case 1: 
            for(int i = 12; i > 1; i--){
                digitalWrite(i, HIGH);
                delay(value);
                digitalWrite(i, LOW);
      
        };
                  Serial.print(" ;  I- ");
      	    selector_caso_prueba = 0;
            break;
    }

  Serial.println("");
  Serial.println(value);

}
