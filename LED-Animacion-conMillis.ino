const int button_pin = 9;
int led_actual = 0;
long tiempo_anterior = 0;
//Segun la documentacion de arduino millis se guarda en long
// pa que no se, tipo que puede llegar a los 50dias en milis
// y eso no se que es

void setup() {
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(9, INPUT_PULLUP);
/*
  Este PULSADOR DE MIERDA es impresionante la batalla que dio
  lo puse en pullup y tuve que ver el diagrama para ver
  como mrd va conectado al arduino, y ahi arranco, si esta
  en input normal te odia.
  */
  Serial.begin(9600);
    Serial.println("IDA Y VUELTA LEDS - CON EL PTO MILLIS AG-ANTROP");
  
}

void loop() {
  int button_status = digitalRead(9);
  long now = millis();

  while (now - tiempo_anterior >= 1000) {
    tiempo_anterior = now;

    digitalWrite(3, LOW);
    //Aca no se como carajos hacer alguna expresion para que
    //sea como un delay, mando el commit asi nomas
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
    digitalWrite(6, LOW);
    digitalWrite(7, LOW);

    if (led_actual == 0) 
    {digitalWrite(3, HIGH);
    };
    if (led_actual == 1) 
    {digitalWrite(4, HIGH);
    };
    if (led_actual == 2)
    { digitalWrite(5, HIGH);
    };
    if (led_actual == 3)
    { digitalWrite(6, HIGH);
    };
    if (led_actual == 4)
    { digitalWrite(7, HIGH);
    };
   
// La cucaracha la cucaracha, ya no puede caminar
// Aca llegue un punto que me fui a los forks del codigo
// Del año pasado y me tuve que inspirar ahi y en lo del
// HelloPad porque si no esta mrd no se xd.
    
    if (button_status == LOW) {
      led_actual = led_actual + 1;
      if (led_actual > 4)
      		{ led_actual = 0;
             Serial.println("de Izquierda a Derecha +1");
              // No sabes lo horrible que es identar
              // en esta porqueria de IDE de Thinkercad
            };
    } else {
      led_actual = led_actual - 1;
      if (led_actual < 0)
      { led_actual = 4;
       Serial.println("de Derecha a Izquierda -1");
      };
    }
  }
}