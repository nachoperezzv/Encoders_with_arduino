
//Código para contar en modo 4x para el circuito tinkercad y control manual del motor
 
// Pines para las interrupciones
#define A_pin 2 
#define B_pin 3
#define button_pin 4 
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

bool estado_button = 0;
bool estado_prev_button = 0;
char sentido_imprimir;
void setup()
{
  // Setup Serial Monitor
  Serial.begin(9600); 
  
  // Set encoder como input con resistencia pullup  
  pinMode(A_pin, INPUT); 
  pinMode(B_pin, INPUT);
  pinMode(button_pin, INPUT_PULLUP);
 
  
  // Attach interrupt 
  attachInterrupt(digitalPinToInterrupt(A_pin), updateEncoderA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(B_pin), updateEncoderB, CHANGE);
  
  // Setup initial values for timer
  previousMillis = millis();

}
 
void loop(){

    estado_button = digitalRead(button_pin); // Lee estado del pulsador: LOW o HIGH
    
    if ((estado_prev_button==HIGH) && (estado_button == LOW)) { // Si se ha pulsado el botón      
      encoderValue = 0;  
    }
    estado_prev_button = estado_button;



  
 

  
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
  
 
  
    if((PIND & B00000100) or (PIND & B00001000)){
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
  
  
  if((PIND & B00000100) or (PIND & B00001000)){
      encoderValue --;
      sentido = 0;
  }
  else{
      encoderValue ++;
      sentido = 1;
  }
}
