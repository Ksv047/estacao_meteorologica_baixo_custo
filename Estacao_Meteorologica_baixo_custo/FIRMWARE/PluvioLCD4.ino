#include <LiquidCrystal.h> //Inclui a biblioteca do LCD
 
LiquidCrystal lcd(12, 11, 5, 4, 3, 2); //Configura os pinos do Arduino para se comunicar com o LCD

// Constantes:
  const int REED = 6;              //The reed switch outputs to digital pin 9
 
  // Variáveis:
int val = 0;                    //Current value of reed switch

int old_val = 0;                //Old value of reed switch

int REEDCOUNT = 0;              //This is the variable that hold the count of switching

 // as variaveis a seguir sao do tipo long pois o tempo, medido em milissegundos,
 // rapidamente se tornara em um numero grande demais para ser armazenado em uma int.
 
 
 void setup() {
   // initializa o pino do switch como entrada
   pinMode (REED, INPUT_PULLUP); //This activates the internal pull up resistor

   // initializa a comunicaçao serial:
   Serial.begin(9600);
   digitalWrite(2,HIGH);
lcd.begin(16, 2); //Inicia o LCD com dimensões 16x2(Colunas x Linhas)
lcd.setCursor(0, 0); //Posiciona o cursor na primeira coluna(0) e na primeira linha(0) do LCD
lcd.print("Pulsos "); //Escreve no LCD "Olá Garagista!"
lcd.setCursor(0, 1); //Posiciona o cursor na primeira coluna(0) e na segunda linha(1) do LCD
lcd.print("Chuva "); //Escreve no LCD "LabdeGaragem"
 }
 
 void loop() {
   // ler o estado do switch pelo pino de entrada:
  val = digitalRead(REED);      //Read the status of the Reed swtich


 if ((val == LOW) && (old_val == HIGH)){    //Check to see if the status has changed

   delay(10);                   // Delay put in to deal with any "bouncing" in the switch.

   REEDCOUNT = REEDCOUNT + 1;   //Add 1 to the count of bucket tips

   old_val = val;              //Make the old value equal to the current value
       Serial.print("Medida de chuva (contagem): ");
       Serial.print(REEDCOUNT);//*0.2794); 
       Serial.println(" pulso");
       Serial.print("Medida de chuva (calculado): ");
       Serial.print(REEDCOUNT*0.25); 
       Serial.println(" mm");
       lcd.setCursor(8, 0); //Posiciona o cursor na décima quarta coluna(13) e na segunda linha(1) do LCD
       lcd.print(REEDCOUNT); //Escreve o valor atual da variável de contagem no LCD
       lcd.setCursor(8, 1); //Posiciona o cursor na décima quarta coluna(13) e na segunda linha(1) do LCD
       lcd.print(REEDCOUNT*0.25); //Escreve o valor atual da variável de contagem no LCD
       lcd.setCursor(14, 1); //Posiciona o cursor na primeira coluna(0) e na segunda linha(1) do LCD
       lcd.print("mm"); //Escreve no LCD "LabdeGaragem"
       } 
 
     else {

   old_val = val;              //If the status hasn't changed then do nothing

 }

}
