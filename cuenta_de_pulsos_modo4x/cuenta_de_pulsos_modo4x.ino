/* Programa creado por:
 *  Cadavid Piñero, Leopoldo
 *  Penalva Martínez, Francisco de Asís
 *  Pérez Vilaplana, Ignacio
 *  Salcedo Salcedo, Raquel
 *  
 * Contador de pulsos en modo 4x con el codificador E6B2-CWZC6.
 *   Se debe usar el modo 4x mediante una interrupción para el canal A del codificador.  
     Hay que controlar que la cuenta pueda crecer o disminuir según el sentido de giro detectado   a
      partir del codificador.  
     Cada vez que se detecte un flanco de bajada en el canal I o Z, la cuenta se debe poner a 0.  
     Cada 500ms hay que enviar el valor actual de la cuenta de pulsos por el puerto serie utilizando una
      cadena de texto con el siguiente formato, de forma que se pueda ver el estado de la cuenta en la
      ventana del monitor serie del IDE de Arduino:
      tiempo, sentido, cuenta [CRLF]
     No se permite utilizar bibliotecas de otros autores que faciliten la gestión de codificadores
 * 
 */
// Pines para las interrupciones
#define A_pin 2 
#define B_pin 3
#define Z_pin 4 
// Variable para controlar los pulsos del encoder
volatile long encoderValue = 0;
 
// Intervalo de un segundo
int interval = 500;
 
// Para llevar la cuenta de los millis
long previousMillis = 0;
long currentMillis = 0;

// Variables para controlar el estado de los pines A y B y el sentido de giro
volatile bool estado_A = 0; 
volatile bool estado_B = 0;
volatile bool sentido = 0; 

bool estado_Z = 0;
bool estado_prev_Z = 0;
char sentido_imprimir;

void setup()
{
  // Setup Serial Monitor
  Serial.begin(9600); 
  
  // Set encoder como input con resistencia pullup  
  pinMode(A_pin, INPUT_PULLUP); 
  pinMode(B_pin, INPUT_PULLUP);
  pinMode(Z_pin, INPUT_PULLUP);
 
  
  // Attach interrupt 
  attachInterrupt(digitalPinToInterrupt(A_pin), updateEncoderA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(B_pin), updateEncoderB, CHANGE);
  
  // Setup initial values for timer
  previousMillis = millis();

}
 
void loop(){
  
  estado_prev_Z  = estado_Z;
  estado_Z  = digitalRead(Z_pin);
  
  if(estado_prev_Z == 1 and estado_Z == 0){ //Si detectamos un flanco de bajada en el pin Z
    encoderValue = 0; //Ponemos la cuenta a 0
  }
  currentMillis = millis();
  
  if (currentMillis - previousMillis > interval) { //Realizamos la comprobacion de que haya pasado el intervalo    
   
    previousMillis = currentMillis;
   
    if(sentido){
      sentido_imprimir = 'H';
    }
    else{
      sentido_imprimir = 'A';
    }
    
      Serial.print("tiempo, sentido, cuenta de pulsos");
      Serial.print(currentMillis);
      Serial.print(',');
      Serial.print('\t');
      Serial.print(sentido_imprimir);
      Serial.print(',');
      Serial.print('\t');
      Serial.println(encoderValue);      
      
  }
  
}
 
void updateEncoderA()
{
  // Increment value for each pulse from encoder
  
  estado_A  = digitalRead(A_pin);
  
    if(estado_A != estado_B){
      encoderValue ++;
      sentido = 1;
    }
    else{
      encoderValue --;
      sentido = 0;
    }
  
}
void updateEncoderB()
{
  // Increment value for each pulse from encoder
  
  estado_B  = digitalRead(B_pin);
  if(estado_A == estado_B){
      encoderValue ++;
      sentido = 1;
  }
  else{
      encoderValue --;
      sentido = 0;
  }
}
