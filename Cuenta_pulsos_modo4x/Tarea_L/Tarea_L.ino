
//Código para contar en modo 4x para el circuito real
 
// Pines para las interrupciones
#define A_pin 2 
#define B_pin 3
#define Z_pin 4 
// Variable para controlar los pulsos del encoder
volatile int encoderValue = 0;
 
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
//  pinMode(A_pin, INPUT); 
//  pinMode(B_pin, INPUT);
  pinMode(Z_pin, INPUT);
 
  
  // Attach interrupt 
  attachInterrupt(digitalPinToInterrupt(A_pin), updateEncoderA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(B_pin), updateEncoderB, CHANGE);
  
  // Setup initial values for timer
  previousMillis = millis();

}
 
void loop(){
  
  //estado_prev_Z  = estado_Z;
  //estado_Z  = digitalRead(Z_pin);
  
//  if(estado_prev_Z == 1 and estado_Z == 0){ //Si detectamos un flanco de bajada en el pin Z
//    encoderValue = 0; //Ponemos la cuenta a 0
//  }
  currentMillis = millis();
  
  if (currentMillis - previousMillis > interval) { //Realizamos la comprobacion de que haya pasado el intervalo    
   
    previousMillis = currentMillis;
   
    if(sentido){
      sentido_imprimir = 'H';
    }
    else{
      sentido_imprimir = 'A';
    }
    
      Serial.print(currentMillis);
      Serial.print(',');
      Serial.print(sentido_imprimir);
      Serial.print(',');
      Serial.println(encoderValue);      
      
  }
  
}

void updateEncoderA(){
//  if(!(PIND & B00010000))
//    encoderValue = 0;
//  
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
//  if(!(PIND & B00010000))
//    encoderValue = 0;

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

 
//void updateEncoderA()
//{
//  // Increment value for each pulse from encoder
//    
//    if((PIND & B00000100)){
//      encoderValue ++;
//      sentido = 1;
//    }
//    else{
//      encoderValue --;
//      sentido = 0;
//    }
//  
//}
//void updateEncoderB()
//{
//  // Increment value for each pulse from encoder
//  
//  
//  if((PIND & B00000100) or (PIND & B00001000)){
//      encoderValue --;
//      sentido = 0;
//  }
//  else{
//      encoderValue ++;
//      sentido = 1;
//  }
//}
