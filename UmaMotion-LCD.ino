
#include <Adafruit_LiquidCrystal.h>

// Configuración del display LCD I2C.
// 0x27 es la dirección I2C más común. Si no funciona, suele ser 0x3F.
Adafruit_LiquidCrystal lcd(0);
int tank1_level;
int tank2_level;
int tank3_level;
bool tank1_shutdown;
bool tank2_shutdown;
bool tank3_shutdown;
int ev1;
int ev2;
int ev3;
bool bomba3_estado = false;
bool bomba2_estado = false;

void setup()
{
  Serial.begin(9600);
  Serial.println("Circuito de Tanques - Aguilas Imperiales 2026 - E2");
  lcd.begin(16, 2);
  lcd.setBacklight(1);

  // Pin config setting for mode operatDDion for Pott. Controller
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);

  // Pin config setting for mode operation for LED Bombs
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);

  // Pin config Setting for mode operation for LED Water Tanks ElectroValves
  pinMode(2, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(7, OUTPUT);
  // pin config para control de screen
  pinMode(13, INPUT_PULLUP);
  pinMode(12, INPUT_PULLUP);
  pinMode(11, INPUT_PULLUP);
  // Buzzer
  pinMode(6, OUTPUT);
}

void loop()
{
  // Mapping zone
  tank1_level = map(analogRead(A0), 0, 1023, 100, 0);
  tank2_level = map(analogRead(A1), 0, 1023, 100, 0);
  tank3_level = map(analogRead(A2), 0, 1023, 100, 0);
  unsigned long currentMillis = millis();
  static unsigned long AntiMillis = 0;

  operationalConditionsSensors();
  screenFunction(currentMillis, AntiMillis, bomba2_estado, bomba3_estado);

  if (currentMillis - AntiMillis >= 700)
  {
    serialPrintInformacion();
    AntiMillis = currentMillis;
  }

  // --- MONITOREO SERIAL ---
}

void screenFunction(unsigned long currentMillis, unsigned long AntiMillis, bool bomba2_estado, bool bomba3_estado)
{
  // pagcount = contador de pagina osea en que pagina estamos y variable ultima pagian, son para la pantalla asi se reinicia
  static int pagcount = 0;
  static int ultima_pagina = -1;
  // right y left son contadores para evitar que el cambio de pagina sea muy rapido, osea que se cambie de pagina cada vez que se presione el boton, sino que se cambie cada cierto tiempo, para evitar que se cambie de pagina por accidente
  int right = 0;
  int left = 0;
  // aca se leen los botones, y se asignan a variables, para luego usarlas en el if de cambio de pagina, y en el if de menu, para evitar que se cambie de pagina o se entre al menu por accidente, osea que se cambie de pagina o se entre al menu cada vez que se presione el boton, sino que se cambie de pagina o se entre al menu cada cierto tiempo, para evitar que se cambie de pagina o se entre al menu por accidente
  int pushbutton_right = digitalRead(13);
  int pushbutton_menu = digitalRead(12);
  int pushbutton_left = digitalRead(11);
  // Variables para evitar el rebote o muchas lecturas del boton, osea que se cambie de pagina cada vez que se presione el boton, sino que se cambie de pagina cada cierto tiempo, para evitar que se cambie de pagina por accidente
  static int ultimoEstadoRight = HIGH;
  static int ultimoEstadoLeft = HIGH;
  static int ultimoEstadoMenu = HIGH;
  // porque STATIC? porque static hace que la variable mantenga su valor entre llamadas a la función, osea que no se reinicie cada vez que se llama a la función, sino que mantenga su valor, para evitar que se reinicie cada vez que se llama a la función, y asi poder usarla para evitar el rebote o muchas lecturas del boton, osea que se cambie de pagina cada vez que se presione el boton, sino que se cambie de pagina cada cierto tiempo, para evitar que se cambie de pagina por accidente
  static unsigned long tiempoUltimoRight = 0;
  static unsigned long tiempoUltimoLeft = 0;
  static unsigned long tiempoUltimoMenu = 0;
  const int rangoAntiRebote = 100;

  // IF del boton de derecha, con el de antirepeticion o rebote del mismo.
  // Tener que escribir comentarios y Documentacion de este codigo es una cagada, porque hacemos lo nuestro osea trabajamos en el mismo codigo pero nos comunicamos por coments.

  if (pushbutton_right == LOW && ultimoEstadoRight == HIGH)
  {
    if (currentMillis - tiempoUltimoRight > rangoAntiRebote)
    {
      pagcount++;
      quehagaRuidito();
      tiempoUltimoRight = currentMillis;
    }
  }
  else if (pushbutton_left == LOW && ultimoEstadoLeft == HIGH)
  {
    if (currentMillis - tiempoUltimoLeft > rangoAntiRebote)
    {
      pagcount--;
      quehagaRuidito();
      tiempoUltimoLeft = currentMillis;
    }
  }
  else if (pushbutton_menu == LOW && ultimoEstadoMenu == HIGH)
  {
    if (currentMillis - tiempoUltimoMenu > rangoAntiRebote)
    {
      pagcount = 0;
      quehagaRuidito();
      tiempoUltimoMenu = currentMillis;
    }
  }

// Estos son los LIMITES de PAGINA asi se evita que se vayan a la mierda.
  if (pagcount < 0)
  {
    pagcount = 5;
  } else if (pagcount > 5)
  {
    pagcount = 0;
  };

  // Este if, hace que limpie la pantalla una sola vez cuando cambias de boton, porque, si lo pones en el switch se va a borrar siempre
  if (pagcount != ultima_pagina)
  {
    lcd.clear();
    ultima_pagina = pagcount;
  };

  if (right >= 1)
  {
    pagcount++;
    if (currentMillis - AntiMillis >= 1000)
    {
      AntiMillis = currentMillis;
      right = 0;
    };
  };
  if (left >= 1)
  {
    pagcount--;
    if (currentMillis - AntiMillis >= 1000)
    {
      AntiMillis = currentMillis;
      left = 0;
    };
  };

  /*String erraseWord () {
      if(tank2_level < 100){lcd.print("   ");};
      if(tank2_level < 10){lcd.print("   ");}
  }*/

  switch (pagcount)
  {
  default:
    MenuScreen();
    break;
  case 1:
    informationScreen();
    break;
  case 2:
    informationScreen2();
    break;
  case 3:
    informationScreen3();
    break;
  case 4:
    pumpScreen1(bomba2_estado);
    break;
  
  case 5:
    pumpScreen2(bomba3_estado);
    break;
  }
}

// aca vamo a hacer voids para pantalla y para sensores, para que el loop quede mas limpio y ordenado, y se pueda entender mejor el codigo
void informationScreen()
{
  lcd.setCursor(0, 0);
  lcd.print("Cisterna");
  lcd.setCursor(10, 0);
  if (tank1_level < 100)
  {
    lcd.print("  ");
  };
  if (tank1_level < 10)
  {
    lcd.print(" ");
  };
  lcd.print(tank1_level);
  lcd.setCursor(15, 0);
  lcd.print("% ");
  lcd.setCursor(0, 1);
  lcd.print("ElectV.1");
  if (ev1)
  {
    lcd.setCursor(10, 1);
    lcd.print("ONLINE");
  }
  else
  {
    lcd.setCursor(10, 1);
    lcd.print("OFF   ");
  }
}
void informationScreen2()
{
  lcd.setCursor(0, 0);
  lcd.print("Tanque 1");
  lcd.setCursor(10, 0);
  if (tank2_level < 100)
  {
    lcd.print(" ");
  };
  if (tank2_level < 10)
  {
    lcd.print(" ");
  };
  lcd.print(tank2_level);
  lcd.setCursor(15, 0);
  lcd.print("% ");
  lcd.setCursor(0, 1);
  lcd.print("ElectV.2");
  if (ev2)
  {
    lcd.setCursor(10, 1);
    lcd.print("ONLINE");
  }
  else
  {
    lcd.setCursor(10, 1);
    lcd.print("OFF   ");
  }
}
void informationScreen3()
{
  lcd.setCursor(0, 0);
  lcd.print("Tanque 2");
  lcd.setCursor(10, 0);
  if (tank3_level < 100)
  {
    lcd.print(" ");
  };
  if (tank3_level < 10)
  {
    lcd.print(" ");
  };
  lcd.print(tank3_level);
  lcd.setCursor(15, 0);
  lcd.print("% ");
  lcd.setCursor(0, 1);
  lcd.print("ElectV.3");
  if (ev3)
  {
    lcd.setCursor(10, 1);
    lcd.print("ONLINE");
  }
  else
  {
    lcd.setCursor(10, 1);
    lcd.print("OFF   ");
  }
}
void pumpScreen1(bool bomba2_estado)
{
  lcd.setCursor(0, 0);
  lcd.print("Bomba Tanque 2");
  if (bomba2_estado)
  {
    lcd.setCursor(0, 1);
    lcd.print("ONLINE");
  }
  else
  {
    lcd.setCursor(0, 1);
    lcd.print("OFF   ");
  }
}
void pumpScreen2(bool bomba3_estado)
{
  lcd.setCursor(0, 0);
  lcd.print("Bomba Tanque 3");
  if (bomba3_estado)
  {
    lcd.setCursor(0, 1);
    lcd.print("ONLINE");
  }
  else
  {
    lcd.setCursor(0, 1);
    lcd.print("OFF   ");
  }
}

void MenuScreen()
{
  lcd.setCursor(0, 0);
  lcd.print("Menu Principal");
  lcd.setCursor(0, 1);
  lcd.print("Tanques - Bombas");
  /*for (int i = 0; i < 300; i++)
  {
    lcd.scrollDisplayLeft();
  }*/
}

void quehagaRuidito(){
  //Segun Documentacion TONE es
  // tone(pin, frecuencia, duracion);
  tone(6, 1000, 100);
}

void operationalConditionsSensors()
{
  if (tank1_level <= 15)
  {
    tank1_shutdown = false;
  }
  else
  {
    tank1_shutdown = true;
  }
  // Tanque 2
  if (tank2_level <= 10)
  {
    tank2_shutdown = false;
  }
  else
  {
    tank2_shutdown = true;
  }

  // Tanque 3
  if (tank3_level <= 10)
  {
    tank3_shutdown = false;
  }
  else
  {
    tank3_shutdown = true;
  }

  // <--------->
  // Electrovalvulas
  if (tank1_level <= 70)
  {
    digitalWrite(2, HIGH);
    ev1 = 1;
  }
  else if (tank1_level == 100)
  {
    digitalWrite(2, LOW);
    ev1 = 0;
  }

  if (tank2_shutdown)
  {
    digitalWrite(4, HIGH);
    ev2 = 1;
  }
  else if (!tank2_shutdown)
  {
    digitalWrite(4, LOW);
    ev2 = 0;
  }

  if (tank3_shutdown)
  {
    digitalWrite(7, HIGH);
    ev3 = 1;
  }
  else if (!tank3_shutdown)
  {
    digitalWrite(7, LOW);
    ev3 = 0;
  }

  // Bombas
  // Si el nivel del tanque 3 o 2, esta por debajo de cierto nivel se activen las bombas
  // siempre y cuando el tanque 1 se los permita


  if (tank3_level <= 75 && tank1_shutdown)
  {
    digitalWrite(10, HIGH);
    bomba3_estado = true;
  }
  else if (tank3_level == 100 || !tank1_shutdown)
  {
    digitalWrite(10, LOW);
    bomba3_estado = false;
  }

  if (tank2_level <= 73 && tank1_shutdown)
  {
    digitalWrite(9, HIGH);
    bomba2_estado = true;
  }
  else if (tank2_level == 100 || !tank1_shutdown)
  {
    digitalWrite(9, LOW);
    bomba2_estado = false;
  }
}

/* void start_message()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Aguilas");
  lcd.setCursor(0, 1);
  lcd.print("Imperiales");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Sist. Control");
  lcd.setCursor(0, 1);
  lcd.print("Uma-ONE");
  delay(2200);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("- - - - - - - - -");
  lcd.blink();
  lcd.cursor();
  delay(500);
  lcd.noBlink();
  lcd.noCursor();
  lcd.clear();
} */

void serialPrintInformacion()
{
  Serial.print("T1: ");
  Serial.print(tank1_level);
  Serial.print("%  ");
  Serial.print("T2: ");
  Serial.print(tank2_level);
  Serial.print("%  ");
  Serial.print("T3: ");
  Serial.print(tank3_level);
  Serial.print("%  ");
  Serial.print("Blq: ");
  Serial.print(tank1_shutdown);
  Serial.print("  ");
  Serial.print("Blq: ");
  Serial.print(tank2_shutdown);
  Serial.print("  ");
  Serial.print("Blq: ");
  Serial.println(tank3_shutdown);
}