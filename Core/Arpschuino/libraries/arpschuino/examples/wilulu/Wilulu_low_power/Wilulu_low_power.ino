/* 
officiel 01/12/17
ecrit pour la wilulu  www.arpschuino.fr/wilulu.php
gradation pwm sur un canal
fonctionne avec "emeteur_arpschuino_officiel" ou "emeteur_JLink_DMX" comme emmeteur
adresse = adreesse de l'emeteur + valeur définie par l'encodeur rotatif
En l'absence de signal, la led reste allumé, elle clignote lorsqu'elle recoit

Low power 05/03/18
Désactivation des pullups,de l'ADC, analog comparator et the brown-out detector
la wilulu s'endort au bout de 1 sec sans réception
puis check la reception pendant 1 sec toute les 4 sec
clignotement de la led verte a la frequence d'une sec quand l'alim descend en dessous de 3.1V (low batt = battement de coeur)

official 01/12/17
Written for the wilulu  www.arpschuino.fr/wilulu_e.php
one channel dimmer
work with "emeteur_arpschuino_officiel" or "emeteur_JLink_bridge_arp" as emiter
address = emiter address + value set by the rotary encoder
If there is no signal, the LED remains lit, flashes when it is received

Low power 05/03/18
turn off pullups, ADC, analog comparator and brown-out detector
if 1 seconds without reception: the wilulu falls asleep for 4 sec
then check the recetion again
blinking of the green led at one sec  frequency when the power supply drops below 3.1V (low batt = heartbeat)
*/
  


#include <Arpschuino.h>
#include <ArpRFLib.h>
#include <avr/sleep.h>
byte NODEID = 4;  //Adresse RF 
#define NETWORKID  212  //adresse du reseau commune à toute les machine
uint8_t band = RF12_868MHZ; //frequence de l'emeteur RF12
//RF12_915MHZ America  // RF12_433MHZ

float frequency_setting = 868.00; //beetween 863,00 > 870,00
//float frequency_setting = 915.00 ;//902;1–918 MHz (America, China and eastern Pacific Islands)

const byte WiluluAdress[4]={WiluluAdress0,WiluluAdress1,WiluluAdress2,WiluluAdress3};//
char identifiant=0;

//////////////////////////////////
unsigned long previousMillis = 0;
unsigned long previousMillisLowBatt = 0;
const long interval = 1000; 


void setup() {

    pinMode(LED_BUILTIN,OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);//led allumée en l'abscence de signal



    for (int i =0;i<4;i++)
    {
      pinMode(WiluluAdress[i],INPUT);
      digitalWrite(WiluluAdress[i],HIGH);
    }
    pinMode (WiluluOUT , OUTPUT);

    delay (100);
    rf12_initialize(NODEID, band, NETWORKID,calcul_freq(frequency_setting));

    for(int i=0;i<4;i++)
    {
       if(!digitalRead(WiluluAdress[i]))
       {
        identifiant += 1<<(i);
       }
    }
  //a commenter pour les 16 premiere adresses :    
  //identifiant+=16; //plus 16

//low power :
    for (int i =0;i<4;i++)
    {
      digitalWrite(WiluluAdress[i],LOW);  //desactivation des pullup
    }
  
  ADCSRA &= ~(1<<ADEN);                     //turn off ADC
  ACSR |= _BV(ACD);                         //disable the analog comparator
  MCUCR |= _BV(BODS) | _BV(BODSE);          //turn off the brown-out detector  
}

void sleep4s() {
  cli();
  rf12_sleep(127);                      // wakeup in about 4s
  set_sleep_mode(SLEEP_MODE_PWR_DOWN); // do a complete power-down
  sleep_enable();                      // enable sleep mode
  sei();                               // allow interrupts to end sleep mode                                    
  sleep_cpu();                         // go to sleep
  sleep_disable();                     // disable sleep mode for safety
  analogWrite (WiluluOUT ,0);          //extinction de la led "out"
}

void loop() { 
    if (rf12_recvDone() && rf12_crc == 0) {
      if (rf12_lowbat()){               //Return true if the supply voltage is below 3.1V
        if(millis() - previousMillisLowBatt >= 2000) {
          //led_temoin ();    //la led clignote a la frequence d'une seconde
          digitalWrite(LED_BUILTIN,HIGH);
          delay(30);
          digitalWrite(LED_BUILTIN,LOW);
          
          previousMillisLowBatt = millis();
        }
      }
      else{
        led_temoin ();      //la led clignote a chaque reception d'une trame
      }
      analogWrite (WiluluOUT , rf12_data[identifiant]);
      previousMillis = millis();
   }
   else
   {
      unsigned long currentMillis = millis();
     
      if(currentMillis - previousMillis >= 3000) {
        // une seconde sans reception    
        digitalWrite(LED_BUILTIN,  LOW);
        sleep4s();
       // sleep4s(); //4 sec de plus
       // sleep4s(); //4 sec de plus      
        rf12_sleep (-1);//reveil du RF12
        previousMillis = millis();     
       }
   }
}
