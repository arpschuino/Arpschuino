/* 
officiel 01/12/17
ecrit pour la wilulu  www.arpschuino.fr/wilulu.php
on/off sur un canal
fonctionne avec "emeteur_arpschuino_officiel" ou "emeteur_JLink_DMX" comme emmeteur
adresse = adreesse de l'emeteur + valeur définie par l'encodeur rotatif
En l'absence de signal, la led reste allumé, elle clignote lorsqu'elle recoit
20/01/19 : Afin de preserver les accus li-ion, la wilulu se met en veille si la tension d'alimentation descend sous 2.8v

official 01/12/17
Written for the wilulu  www.arpschuino.fr/wilulu_e.php
on/off one channel
work with "emeteur_arpschuino_officiel" or "emeteur_JLink_bridge_arp" as emiter
address = emiter address + value set by the rotary encoder
If there is no signal, the LED remains lit, flashes when it is received
20/01/19: In order to preserve the li-ion batteries, the wilulu goes to sleep mode if the supply voltage drops below 2.8v
*/

#include <Arpschuino.h>
#include <ArpRFLib.h>
#include <avr/sleep.h>
uint8_t NODEID = 4;  //Adresse RF
#define NETWORKID  212  //adresse du reseau commune à toute les machine
uint8_t band = RF12_868MHZ; //frequence de l'emeteur RF12
//RF12_915MHZ America  // RF12_433MHZ

float frequency_setting = 868.00; //beetween 863,00 > 870,00
//float frequency_setting = 915.00 ;//902;1–918 MHz (America, China and eastern Pacific Islands)

#define bascule 127

const byte dip[4]={WiluluAdress0,WiluluAdress1,WiluluAdress2,WiluluAdress3};//
char identifiant=0;

//////////////////////////////////
unsigned long previousMillis = 0;
const long interval = 1000; 


void setup() {

    pinMode(LED_BUILTIN,OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);//led allumée en l'abscence de signal

    pinMode (WiluluOUT , OUTPUT);

    delay (100);
    rf12_initialize(NODEID, band, NETWORKID,rf12_calcul_freq(frequency_setting));

    for(int i=0;i<4;i++)
    {
      pinMode(dip[i],INPUT);  //
      digitalWrite(dip[i],HIGH);    // pullup
      if(!digitalRead(dip[i]))  identifiant += 1<<(i);
      digitalWrite(dip[i],LOW);  //desactivation des pullup
    }
  //a commenter pour les 16 premiere adresses :    
  //identifiant+=16; //plus 16

  //////////////////////////////energy saver/////////////////////////
  ADCSRA &= ~(1<<ADEN);                     //turn off ADC
  ACSR |= _BV(ACD);                         //disable the analog comparator
  MCUCR |= _BV(BODS) | _BV(BODSE);          //turn off the brown-out detector      

  rf12_control(0xC043);//detection low-batt//https://tools.jeelabs.org/rfm12b.html
  //C040:2.2V;C041:2.3V;..........C043:2.5V;............C04F:3.7V;
  
  //après passage dans la diode et le régulateur, la tension chute de 0.3v environ.
  //donc si la detection est réglé a 2.5v le wilulu se met en veille a environ 2.8v

   // after passing through the diode and regulator, the voltage drops about 0.3v.
   // so if the detection is set to 2.5v the wilulu goes to sleep at about 2.8v  
}

void loop() {
      
    if (rf12_recvDone() && rf12_crc == 0) {
      Arp_led_temoin ();
      if(rf12_data[identifiant]>bascule)
      {
        digitalWrite (WiluluOUT , HIGH);
      }
      else
      {
        digitalWrite (WiluluOUT , LOW);
      }
   }
   else
   {
      unsigned long currentMillis = millis();
     
      if(currentMillis - previousMillis >= interval) {
        // une seconde sans reception
        previousMillis = currentMillis;   
        digitalWrite(LED_BUILTIN,  HIGH);      
       }
   }
   if (rf12_lowbat())
   {
      while (rf12_lowbat()) sleep4s();
      rf12_sleep(-1);//reveille le rf12 
   }
}

void sleep4s() {
  digitalWrite(LED_BUILTIN, LOW);      //extinction de la led verte
  analogWrite (WiluluOUT ,0);          //extinction de la led "out"
  cli();                               //disable interrupt
  rf12_sleep(127);                     // wakeup in about 4s
  set_sleep_mode(SLEEP_MODE_PWR_DOWN); // do a complete power-down
  sleep_enable();                      // enable sleep mode
  sei();                               // allow interrupts to end sleep mode                                    
  sleep_cpu();                         // go to sleep
  sleep_disable();                     // disable sleep mode for safety 
}
