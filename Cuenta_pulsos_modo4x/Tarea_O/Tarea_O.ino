/* Programa creado por:
 *  Cadavid Piñero, Leopoldo
 *  Penalva Martínez, Francisco de Asís
 *  Pérez Vilaplana, Ignacio
 *  Salcedo Salcedo, Raquel
 *  
 * Contador de pulsos en modo 4x con el 
 * motor-codificador-reductor de Tinkercad.
 */
 
// Pines para las interrupciones
#define A_pin 2 
#define B_pin 3
#define Z_pin 4 

// Variable para controlar los pulsos del encoder
volatile int encoderValue = 0;
 
// Intervalo de un segundo
int interval = 500;
 
// Declaración de otras variables
long previousMillis = 0;
long currentMillis = 0;
volatile bool sentido = 0; 

void setup()
{
  // Setup Serial Monitor
  Serial.begin(9600); 
  
  // Se establece el pin4 como una simulación de la entrada
  // del canal Z. La activación se realiza de forma manual 
  pinMode(Z_pin, INPUT);
   
  // Attach interrupt 
  attachInterrupt(digitalPinToInterrupt(A_pin), updateEncoderA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(B_pin), updateEncoderB, CHANGE);
  
  // Setup initial values for timer
  previousMillis = millis();
}
 
void loop(){  
  currentMillis = millis();  
  //Se realiza la comprobacion de que haya pasado el intervalo
  if (currentMillis - previousMillis > interval) { 
    Serial.print(millis());
    Serial.println(","+(String)(sentido?"H":"A")+","+encoderValue);
    previousMillis = millis();  
  }  
}

void updateEncoderA(){
  //Esta sentencia reestablece la cuenta de pulsos en
  //caso de que se active el boton que simula el canal Z
  if(!(PIND & B00010000))
    encoderValue = 0;

  //La lógica para esta interrupción y la interupción en B,
  //es la misma, por lo que solo se explica esta y se asume
  //que en B, la lógica es la misma pero seguiendo un sentido
  //contrario en el giro.
  //En la cuenta 4x la interrupción se situa en el cambio de
  //estado, de manera pero por cada canal habrá dos interrupciones.
  //De esta manera, ya no se puede comprobar si al saltar el canal
  //A hay pulso o no en el canal B (como se hacía en el modo 1x). 
  //Lo que se hace es hacer una comprobación inicial de si esta en
  //en subida o bajada y a partir de ahí ver si B tiene el flanco 
  //activado o no, par saber el sentido. 
    
  if(PIND & B00000100){
    if(PIND & B00001000){
      sentido=0;
      encoderValue=encoderValue-1;
    }
    else{
      sentido=1;
      encoderValue=encoderValue+1; 
    }
  }
  else{
    if(PIND & B00001000){
      sentido=1;
      encoderValue=encoderValue+1;
    }
    else{
      sentido=0;
      encoderValue=encoderValue-1;
    }
  }
}

void updateEncoderB(){
  //Esta sentencia reestablece la cuenta de pulsos en
  //caso de que se active el boton que simula el canal Z
  if(!(PIND & B00010000))
    encoderValue = 0;

  if(PIND & B00001000){
    if(PIND & B00000100){
      sentido=1;
      encoderValue=encoderValue+1; 
    }
    else{
      sentido=0;
      encoderValue=encoderValue-1;
    }
  }
  else{
    if(PIND & B00000100){
      sentido=0;
      encoderValue=encoderValue-1;
    }
    else{
      sentido=1;
      encoderValue=encoderValue+1;
    }
  }  
}
