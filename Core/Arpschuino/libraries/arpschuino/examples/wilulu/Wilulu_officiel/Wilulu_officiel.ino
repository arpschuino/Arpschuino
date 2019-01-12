/* 
officiel 01/12/17
ecrit pour la wilulu  www.arpschuino.fr/wilulu.php
gradation pwm sur un canal
fonctionne avec "emeteur_arpschuino_officiel" ou "emeteur_JLink_DMX" comme emmeteur
adresse = adreesse de l'emeteur + valeur définie par l'encodeur rotatif
En l'absence de signal, la led reste allumé, elle clignote lorsqu'elle recoit

official 01/12/17
Written for the wilulu  www.arpschuino.fr/wilulu_e.php
one channel dimmer
work with "emeteur_arpschuino_officiel" or "emeteur_JLink_bridge_arp" as emiter
address = emiter address + value set by the rotary encoder
If there is no signal, the LED remains lit, flashes when it is received
*/

#include <Arpschuino.h>
#include <ArpRFLib.h>
byte NODEID = 4;  //Adresse RF 
#define NETWORKID  212  //adresse du reseau commune à toute les machine
uint8_t band = RF12_868MHZ; //frequence de l'emeteur RF12
//RF12_915MHZ America  // RF12_433MHZ

float frequency_setting = 868.00; //beetween 863,00 > 870,00
//float frequency_setting = 915.00 ;//902;1–918 MHz (America, China and eastern Pacific Islands)

const byte dip[4]={WiluluAdress0,WiluluAdress1,WiluluAdress2,WiluluAdress3};//
char identifiant=0;

//////////////////////////////////
unsigned long previousMillis = 0;
const long interval = 1000; 


void setup() {

    pinMode(LED_BUILTIN,OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);//led allumée en l'abscence de signal

    for (int i =0;i<4;i++)
    {
      pinMode(dip[i],INPUT);
      digitalWrite(dip[i],HIGH);
    }
    pinMode (WiluluOUT , OUTPUT);

    delay (100);
    rf12_initialize(NODEID, band, NETWORKID,calcul_freq(frequency_setting));

    for(int i=0;i<4;i++)
    {
       if(!digitalRead(dip[i]))
       {
        identifiant += 1<<(i);
       }
    }
  //a commenter pour les 16 premiere adresses :    
  //identifiant+=16; //plus 16
}

void loop() {

      if (rf12_recvDone() && rf12_crc == 0) {
        led_temoin ();
        analogWrite (WiluluOUT , rf12_data[identifiant]);
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
}
