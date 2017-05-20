//// lee la salida del modulo FM, si esta en alto se enciende el led 13, sino esta apagado


const int ARMARFM = A0; // El pin digital 2 tiene un pushbutton conectado a él, le damos un nombre void setup() 
int led=13;


void setup()
{
Serial.begin(9600); // Inicializamos la comunicacion serial a 9600 baudios

pinMode(ARMARFM, INPUT); // Establecemos el Pin 2 denominado pushbutton como Entrada
pinMode(led,OUTPUT);
}

void loop() 
{
int buttonState = analogRead(ARMARFM); // Creamos una variable que almacena el estado digital del pin 2
Serial.println(buttonState); // Imprimimos el estado digital del pin 2 sobre el Monitor Serial
delay(1); // Generamos un retardo entre las lecturas para darle estabilidad  
  if (buttonState>0){       //Si el valor es distinto de 0 ( le va a entrar un valor alto 1023) se enciende
    digitalWrite(led,HIGH);
  }
  
  else{
    digitalWrite(led,LOW);
  }


}
