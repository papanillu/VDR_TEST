

#include <Wire.h> // Library for I2C communication
#include <LiquidCrystal_I2C.h> // Library for LCD
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 20, 4); 

const int buttonPin = 2;        // Pin asignado al pulsador, es el primero despues de RX y TX
//const int ledPin =  13;         // Pin asignado al led de test de desarrollo, 
                                // Al ser el por defecto y tener un led instalado no necesitamos cablear nada durante el desarrollo
// variables Globales,
int buttonState = 0;            // para leer el pulsador
int contadormenu = 0;           // Para cargar el valor seleccionado
int action = 0;                 // Vamos incializando variables
int OKMenu = 0;                 // esta es para la cuando damos pulsacion larga y convertimos en OK
char mostrar[5];                // Para mostrar valores en pantalla
#define MAXMENU 4               // Numeo Maximo de opciones de menu

unsigned long Tinicio;          //Momento en que se pulsa el pulsador
unsigned long Tfin;             //Momento en que se libera el pulsador
unsigned long Tpulsacion;       //Diferencia Tfin-Tinicio
long Retardopulsacion = 500;    //Tiempo pulsacion larga, 
                                //Hay que afinar este tiempo, ya que parece no cuadran los milisegundos leidos con tiempo real
void setup() {

  lcd.init();
  lcd.backlight();
  // initialize the LED pin as an output:
  // pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
}

void menuread()
{
  Tinicio= millis();
  buttonState = digitalRead(buttonPin); 
  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (buttonState == HIGH) 
  {
    do {
      buttonState = digitalRead(buttonPin);
      Tfin = millis();
    } while (buttonState == HIGH);
    Tpulsacion = (Tfin-Tinicio);
    if (Tpulsacion > Retardopulsacion)
    {
      // significa pulsacion larga, No hay que incrementar opcion de menu y activar OK
      OKMenu=1; //Asignamos 1 como OK
    } else {
      // Enciende LED
      // digitalWrite(ledPin, HIGH); // Para control visual de bonton pulsado, luqego quitar
      OKMenu=0;  //Asignamos 0 como no OK
      ++contadormenu;
      if (contadormenu > MAXMENU ) contadormenu=1;
    }
  } else {
    // turn LED off:
    // digitalWrite(ledPin, LOW);
  }
}

void ramtest()
{
  
  // Aqui vendra la rutina de testear memoria; 
  // de momento ponemos una espera de unos cuantos segundos, y un contador para que sea mas visual
  
  int espera=1000;
  lcd.setCursor(12,2);
  lcd.print(" 0%");
  delay(espera);
  lcd.setCursor(12,2);
  lcd.print("10%");
  delay(espera);
  lcd.setCursor(12,2);
  lcd.print("30%");
  delay(espera);
  lcd.setCursor(12,2);
  lcd.print("50%");
  delay(espera);
  lcd.setCursor(12,2);
  lcd.print("70%");
  delay(espera);
  lcd.setCursor(12,2);
  lcd.print("90%");
  delay(espera);
  lcd.setCursor(11,2);
  lcd.print("100%");
  OKMenu=0;
  action=0;
  lcd.setCursor(0,3);
  lcd.print("RAM OK, Press.....");
  do 
  {
      buttonState = digitalRead(buttonPin);
      Tfin = millis();
  } while (buttonState == LOW); //Esperamos hasta que se pulse el boton

  // Inicializamos y volvemos al Inicio.
  delay(1000);
  contadormenu=0; // Si no inicializamos, permacera activo la ultima seleccionada
}

void loop() {
  // (Columna, Fila) Comenzando por 0
  lcd.setCursor(0, 0); 
  lcd.print("Ram Tester VDR 0.1 "); 
  lcd.setCursor(0,1);
  lcd.print("No chip Selected   ");
  lcd.setCursor(0,2);
  lcd.print("Short press change ");
  lcd.setCursor(0,3);
  lcd.print("long press OK      ");
  

// Bucle donde controlamos el menu
do 
{  
  menuread();
  switch (contadormenu)
  {
    case 1:
      lcd.setCursor(0,1);
      lcd.print(" 4464              ");
      break;
    case 2:
      lcd.setCursor(0,1);
      lcd.print(" 4116              ");
      break;
    case 3:
      lcd.setCursor(0,1);
      lcd.print(" 4164              ");
      break;
    case 4:
      lcd.setCursor(0,1);
      lcd.print("41464              ");
      break;
  }
  
  if (OKMenu==1 && contadormenu != 0 ) action=1; //Validando contadormenu nos aseguramos que no se lanza un test sin opcion.
  
  }while (action==0);
  
  lcd.setCursor(0,2);
  lcd.print("Testing...          ");
  lcd.setCursor(0,3);
  lcd.print("                    ");
  ramtest();
}
