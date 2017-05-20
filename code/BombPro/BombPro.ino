#include <Wire.h> 
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
//#include <LiquidCrystal.h>
/*
 Bomba Airsoft Arduino UNO
 
 Desarrollado por:
 Óscar J. Fernández Rodríguez
 Tec. Superior en Mantenimiento Electrónico
 Julio de 2015 versión 1.0
 
 */

LiquidCrystal_I2C lcd(0x27,16,2);
//LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
const byte ROWS = 4;       //Matriz de 4 FILAS
const byte COLS = 4;       //Matriz de 4 COLUMNAS
char keys[ROWS][COLS] = {
  {
    '1','2','3','a'                          }
  ,
  {
    '4','5','6','b'                          }
  ,
  {
    '7','8','9','c'                          }
  ,
  {
    '*','0','#','d'                          }
};

byte rowPins[ROWS] = {9,8,7,6};       //conecto las filas  a los pines del arduino
byte colPins[COLS] = {5,4,3,2};       //conecto las columnas  a los pines del arduino

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );


char enteredText[8];
byte time[4];
byte refresh=0;        
char password[8];
int key=-1;
char lastKey;
char var;
boolean passwordEnable=false;

//BOTONES DE TECLADO PARA CONTROLAR PROGRAMA
char BT_RIGHT = '4';
char BT_UP = 'a';
char BT_DOWN = 'b';
char BT_LEFT = '6';
char BT_SEL = 'd';   // Ok key  
char BT_CANCEL = 'c';
char BT_DEFUSER = 'x';   // no implementeado

//LEDS
const int REDLED = 11;
const int GREENLED = 10;
//const int BLUELED = 10;

//MOSFET
boolean mosfetEnable = false;
const int mosfet = 12;         //No implementado en la placa
const int MOSFET_TIME = 5000;  //No implementado en placa


//VARIABLES FM
const int ARMARFM = A0; // ACTIVA BOMBA MEDIANTE FM
const int ARMARFM1 = A1; // ACTIVA BOMBA MEDIANTE FM
const int DESARMARFM = A2; // DESACTIVA BOMBA MEDIANTE FM
const int DESARMARFM1 = A3; // DESACTIVA BOMBA MEDIANTE FM
               


//TIEMPOS PARA JUEGO Y ESTADO DE JUEGO
int GAMEHOURS = 0;
int GAMEMINUTES = 45;
int BOMBMINUTES = 4;
int ACTIVATESECONDS = 5;
boolean endGame = false;
boolean sdStatus = false;     //search and destroy 
boolean saStatus = false;     //para sabotage
boolean doStatus = false;     //para demolocion
boolean start = true;
boolean defuseando;
boolean cancelando;

// TONOS SONIDO
boolean soundEnable = true;
int tonepin = 13; // Lo uso para el sonido
int tonoPitido = 3000;
int tonoAlarma1 = 700;
int tonoAlarma2 = 2600;
int tonoActivada = 1330;
int errorTone = 100;

unsigned long iTime;
unsigned long timeCalcVar;
unsigned long redTime;
unsigned long greenTime;
unsigned long iZoneTime;//inicializacion de zona
byte team=0; // 0 = neutral, 1 = equipo verde, 2 = equipo rojo

void setup(){
  //lcd.begin(16, 2);
  Serial.begin(9600);
  lcd.init();                      // inicializo la LCD
  lcd.backlight();                 // enciende el backlight de la Lcd
  lcd.setCursor(0,0);              // cambio el inicio del cursos a la posición 0 en X y 0 en Y
  tone(tonepin,2400,30);
  lcd.print("BOMBA AIRSOFT v1");   
  lcd.setCursor(0,1);
  lcd.print("Oscar J.Fer Rodz");   
  keypad.setHoldTime(50);
  keypad.setDebounceTime(50);
  keypad.addEventListener(keypadEvent);
  delay(4000);
  pinMode(GREENLED, OUTPUT);     
//  pinMode(8, OUTPUT);  
//  digitalWrite(8,HIGH);
  pinMode(REDLED, OUTPUT); 
  pinMode(mosfet, OUTPUT); 
  
  // CONFIGURAR LOS BAR
  byte bar1[8] = {
    B10000,
    B10000,
    B10000,
    B10000,
    B10000,
    B10000,
    B10000,
    B10000,
  };
  byte bar2[8] = {
    B11000,
    B11000,
    B11000,
    B11000,
    B11000,
    B11000,
    B11000,
    B11000,
  };
  byte bar3[8] = {
    B11100,
    B11100,
    B11100,
    B11100,
    B11100,
    B11100,
    B11100,
    B11100,
  };
  byte bar4[8] = {
    B11110,
    B11110,
    B11110,
    B11110,
    B11110,
    B11110,
    B11110,
    B11110,
  };
  byte bar5[8] = {
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
  };
  byte up[8] = {
    B00000,
    B00100,
    B01110,
    B11111,
    B11111,
    B00000,
    B00000,
  };

  byte down[8] = {
    B00000,
    B00000,
    B11111,
    B11111,
    B01110,
    B00100,
    B00000,
  };
  lcd.createChar(0,bar1);
  lcd.createChar(1,bar2);
  lcd.createChar(2,bar3);
  lcd.createChar(3,bar4);
  lcd.createChar(4,bar5);
  lcd.createChar(5,up);
  lcd.createChar(6,down);
}

void loop(){
  menuPrincipal();
}
void keypadEvent(KeypadEvent key){
  switch (keypad.getState()){
    case PRESSED:
      switch (key){

      }
    break;
    case RELEASED:
      switch (key){
         case 'd': defuseando= false;
         //Serial.println("d Releases");
         break;
         case 'c': cancelando=false;
         //Serial.println("c Releases");
         break;
      }
    break;
    case HOLD:
      switch (key){
        case 'd': defuseando= true;
        //Serial.println("d hold");
        break;
        case 'c': cancelando=true;
        //Serial.println("c hold");
        break;
      }
    break;
  }
}

void disarmedSplash(){
  endGame = false;
  digitalWrite(REDLED, LOW); 
  digitalWrite(GREENLED, LOW);
  if(sdStatus || saStatus){
    lcd.clear();
    lcd.setCursor(1,0);
    lcd.print("BOMBA DESARMADA");
    lcd.setCursor(2,1);
    lcd.print("POLICIA GANA");
    digitalWrite(GREENLED, HIGH);  
    //delay(5000);
    digitalWrite(GREENLED, LOW); 
    
    while(1)   //HAGO UN CICLO PARA QUE QUEDE PUESTO QUE LA POLICIA GANA Y NO SE SALE HASTA QUE SE MANTENGA PULSADO LA LETRA "D"
    {

    delay(3000);
    keypad.getKey();
    if(defuseando){
      keypad.getKey();
      break;
    }
    }    
  }
  //fin codigo
  
  lcd.clear();
  lcd.print("Jugar de nuevo?");
  lcd.setCursor(0,1);
  lcd.print("A : Si B : No");
  digitalWrite(REDLED, LOW);  
  digitalWrite(GREENLED, LOW); 
  while(1)
  {
    var = keypad.waitForKey();
    if(var == 'a' ){
      tone(tonepin,2400,30);
      //ara buscar y destruir y sabotage podemos volver a jugar con las mismas opciones
      if(sdStatus){
        startGameCount();
        search();
      }
      if(saStatus){
        saStatus=true;
        startGameCount();
        start=true; 
        sabotage();
      }
    }  
    if(var == 'b' ){
      tone(tonepin,2400,30);
      menuPrincipal();
      break;
    }  
  } 
}

void explodeSplash(){
  digitalWrite(REDLED, LOW);  
  digitalWrite(GREENLED, LOW); 
  cls();
  delay(100);
  endGame = false;
  lcd.setCursor(1,0);
  lcd.print("POLICIA PIERDE");
  lcd.setCursor(3,1);
  lcd.print("GAME OVER");
  for(int i = 200; i>0; i--)    //si la policia pierde suena un sonido 
  {
    tone(tonepin,i);
    delay(20);
  }
  noTone(tonepin);
  if(mosfetEnable){
    activateMosfet(); 
  }
  delay(5000);
  cls();

  //end code
  lcd.print("Jugar de nuevo?");
  lcd.setCursor(0,1);
  lcd.print("A : Si B : No");
  while(1)
  {
    var = keypad.waitForKey();
    if(var == 'a' ){
      tone(tonepin,2400,30);
      if(sdStatus){
        startGameCount();
        search();
      }
      if(saStatus){
        saStatus=true;
        startGameCount();
        start=true; 
        sabotage();
      }
    }  
    if(var == 'b' ){
      tone(tonepin,2400,30);
      menuPrincipal();

      break;
    }  
  } 
}

