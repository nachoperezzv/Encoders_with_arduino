/* Programa creado por:
 *  Cadavid Piñero, Leopoldo
 *  Penalva Martínez, Francisco de Asís
 *  Pérez Vilaplana, Ignacio
 *  Salcedo Salcedo, Raquel
 *  
 * Contador de pulsos en modo 1x con el 
 * motor-codificador-reductor de Tinkercad.
 */

//Declaración de los pines
const int pinA = 2;
const int pinB = 3;
const int pinZ  = 4;

//Declaración de otras variables
volatile  int       c_pulsos = 0;   
bool      CW;
long int  previousMillis = 0;
long int  currentMillis  = 0;

//Declaración funciones interrupciones
void A_ISR();

void setup() {
  //Configuración puertos
  pinMode(pinZ,INPUT);
  
  //Configuración de las interrupciones
  attachInterrupt(digitalPinToInterrupt(pinA), A_ISR, RISING);

  //Configuración del Serial Port
  Serial.begin(9600);

  //Configuración de millis
  previousMillis = millis();
}

void loop() {
  // El canal Z esta conectado en la placa Arduino UNO al pin4
  // En esta placa, ese pin no tiene interrupción por lo que la
  // comprobación se va a hacer en el loop, mirando la entrada
  // del puerto 4 directamente

  //Esta sentnecia se puede comentar para que la cuenta
  //de pulsos no se reestablezca tras dar una vuelta
  //completa y poder calcular mediante un promedio
  //el valor PPR
  //if(!(PIND & B00010000))  // Máscara para el pin4 --> canal Z
  //  c_pulsos = 0;       // Si se activa el flanco se pone la cuenta
                        // de pulsos a 0

  // Cada 500ms se imprime por el terminal el tiempo,
  // el sentido de giro actual y el número de pulsos
  currentMillis=millis()-previousMillis;
  if( currentMillis >= 500){
    Serial.print(millis());
    Serial.println(","+(String)(CW?"H":"A")+","+c_pulsos);
    previousMillis = millis();
  }  
}


void A_ISR(){
  // Esta interrupción salta cuando se detecte un desplazamiento
  // del eje del motor, es decir, se ha producido un movimiento.
  // Se sabe que la orientación de este giro (CW-clockwise o 
  // CCW-counterClockwise) se puede conocer a raíz del desfase
  // entre los pulsos del canal A. Si el pulso del canal A esta 
  // adelantado con respecto a B el sentido es CW, si no, CCW.

  // canal A --> pin2 --> 0000 0100
  // canal B --> pin3 --> 0000 1000
  
  noInterrupts(); // Se desactivan interrupciones para que no haya
                  // lecturas erroneas del canal A
  //if(!(PIND & B00010000))
  //  c_pulsos = 0;
  
  if(PIND & B00001000){ // Máscara para el canal B (pin3)
    c_pulsos=c_pulsos-1;// Si es B=1 (CCW) devuelve 1, si no, 0
    CW = false;
  } 
  else{                 // Para el caso contrario, se esta girando
    c_pulsos=c_pulsos+1;// en sentido CW
    CW = true;
  }
  interrupts();   // Se vuelven a permitir las interrupciones
}
