/* Programa creado por:
 *  Cadavid Piñero, Leopoldo
 *  Penalva Martínez, Francisco de Asís
 *  Pérez Vilaplana, Ignacio
 *  Salcedo Salcedo, Raquel
 *  
 * Contador de pulsos en modo 1x con el codificador E6B2-CWZC6.
 *   Se debe usar el modo 1x mediante una interrupción para el canal A del codificador.  
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
//Declaración de los pines
const int pinISR1 = 2;
const int pinISR2 = 3;
const int pinChZ  = 4;

//Declaración de las funciones de las interrupciones
void ChA_ISR();
void ChB_ISR();

//Declaración de otras variables
long int c_pulsos = 0;     // Contador de pulsos/segundo
bool ChA = false;
bool ChB = false;

void setup() {
  // Configuración de las interrupciones
  attachInterrupt(digitalPinToInterrupt(pinISR1), ChA_ISR, RISING);
  attachInterrupt(digitalPinToInterrupt(pinISR2), ChB_ISR, RISING);

  // Configuración del Serial port
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

  if(digitalRead(pinChZ)==LOW)
    c_pulsos = 0;

  Serial.println(c_pulsos);
}

void ChA_ISR(){
  if(ChB)  // Si B esta a nivel alto
    c_pulsos++;    
  else        // Si B esta a nivel bajo
    c_pulsos--;
}

void ChB_ISR(){
  noInterrupts();
  if(PIND & B00000010)
    ChB = false;
  else
    ChB = true;
  interrupts();
}
