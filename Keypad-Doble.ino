

void setup()
{
  Serial.begin(9600);
  pinMode(14, OUTPUT);#include <Keypad.h>

int cont = 0;
bool en=false;
String cl = "";
const String c1 = "AC57";

const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
 {'1', '2', '3', 'A'},
 {'4', '5', '6', 'B'},
 {'7', '8', '9', 'C'},
 {'*', '0', '#', 'D'}};

char ob_keys[ROWS][COLS] = {
 {'1', '2', '3', 'A'},
 {'4', '5', '6', 'B'},
 {'7', '8', '9', 'C'},
 {'*', '0', '#', 'D'}};

byte rowPins[ROWS] = {5, 4, 3, 2};
byte colPins[COLS] = {8, 7, 6, 9};
byte aguilas[COLS] = {10, 11, 12, 13};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
Keypad aguilas_keypad = Keyp
  Serial.println("AGUILAS ORIGINALES - INNOVACION EN NUESTRA SANGRE");
  
}

void loop()
{
  char key = keypad.getKey();
  char ob_key = aguilas_keypad.getKey();


  if (key)
  {
    
    Serial.println(key);
    cl = cl + key;
    cont++;
    Serial.println(cont);
  }


  if (ob_key)
  {
    Serial.println(ob_key);
    cl = cl + ob_key; 
    cont++;
    Serial.println(cont);
  }


  if (cont == 4) 
  {
    Serial.println(cl);

    if (cl == c1 && !en) {
      digitalWrite(14, HIGH);
      en=!en;
      Serial.println("entro 1");
          cl = "";
    cont = 0;
    } 
 	if (cl == c1 && en) {
      digitalWrite(14, LOW);
   en=!en;
       Serial.println("entro 2");
          cl = "";
    cont = 0;
    }
 
    
  }   }