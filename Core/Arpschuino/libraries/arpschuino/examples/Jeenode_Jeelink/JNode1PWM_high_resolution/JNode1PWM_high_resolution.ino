//reception JNode, ecriture dans l'arpower.
// S'utilise avec "emeteur_arpschuino_officiel" ou "emeteur_JLink_bridge_arp" comme emeteur.
//un code pour un seul output en pwm + led de signal.
//resolution variable de 8 à 16 bit (editez la ligne: byte RESOLUTION = 12;)
//soties sur SEL1 (ou B1) du JeeNode, a utiliser avec un arpower mini http://arpschuino.fr/arpowerMini.php
//led temoin sur la sorie I (la LED sur le JNode>arpower²) ou PA4 du JeeNode, out 1 de l'arpower avec JeeNode>arpower adapter http://www.arpschuino.fr/adapt_JNode.php

// JNode reception, writing in arpower.
// Works with "emeteur_arpschuino_officiel" or "emeteur_JLink_bridge_arp" as transmitter.
//Source code for only one PWM output.
//variable resolution from 8 to 16 bits (edit line: byte RESOLUTION = 12;)
//output SEL1 (or B1) on JeeNode,you can use it with an arpower mini http://arpschuino.fr/arpowerMini_e.php
//signal LED on JeeNode's I output (the LED on JNode>arpower²) or PA4,out 1 on arpower with JeeNode>arpower adapter http://www.arpschuino.fr/adapt_JNode_e.php

//http://www.arpschuino.fr

#include <Arpschuino.h>

#include <ArpRFLib.h>
#include <Arpschuino.h>
byte NODEID = 2;  //Adresse RF
#define NETWORKID  212  //adresse du reseau commune à toute les machine

uint16_t band = RF12_868MHZ; //frequence de l'emeteur RF12
//RF12_915MHZ America  // RF12_433MHZ

float frequency_setting = 868.00; //beetween 863,00 > 870,00
//float frequency_setting = 915.00 ;//902;1–918 MHz (America, China and eastern Pacific Islands)

int identifiant = 0; //adresse du recepteur = adresse dmx + son adresse
const byte temoin=3;//Jeenode irq  https://jeelabs.net/attachments/download/894/JeeNode%20Pinout%20Diagram.pdf
//or :
//const byte temoin=17;//Jeenode's PA4 
const uint8_t PWM=9;
//////////////////////////////////
unsigned long previousMillis = 0;
const long interval = 1000; 

#include "courbe.h"


//choisisez la résolution - choose the resolution :
byte RESOLUTION = 12;//3906Hz (very good)
//byte RESOLUTION = 13;//1953Hz (good)
//byte RESOLUTION = 14;//976Hz (ok)
//byte RESOLUTION = 16;//244Hz (bad: fliker !)




void setup()
{ 
    rf12_initialize(NODEID, band, NETWORKID,rf12_calcul_freq(frequency_setting));
    setupPWM(RESOLUTION);
    pinMode(temoin,OUTPUT);
    //pinMode (PWM, OUTPUT);
}



void loop() 
{
     if (rf12_recvDone() && rf12_crc == 0) 
     {
        Arp_led_temoin(temoin);
        analogWrite16 (9, level_12B[rf12_data[0+identifiant]]);
     } 
     else
     {
       unsigned long currentMillis = millis();     
       if(currentMillis - previousMillis >= interval) 
       {
          // une seconde sans reception
          previousMillis = currentMillis;   
          digitalWrite(temoin,  LOW);      
       }
     }      
} 

void setupPWM(int resolution) {
  uint16_t icr;
  switch(resolution) {
    case 16:icr=0xffff;break;
    case 15:icr=0x7fff;break;
    case 14:icr=0x3fff;break;
    case 13:icr=0x1fff;break;
    case 12:icr=0x0fff;break;
    case 11:icr=0x07ff;break;
    case 10:icr=0x03ff;break;
    case  9:icr=0x01ff;break;
    case  8:icr=0x00ff;break;
    default:icr=0x00ff;break;
  }
/*Changing the prescaler will effect the frequency of the PWM signal.
Frequency[Hz}=CPU/(ICR1+1) where in this case CPU=16 MHz
16-bit PWM will be>>> 16000000/(65535+1)=244Hz
14-bit PWM : 976Hz ----- 12-bit PWM : 3906Hz 
*/    
  DDRB  |= _BV(PB1);// | _BV(PB2);       /* set pin 9 as output */
  TCCR1A =_BV(COM1A1)  /*   |_BV(COM1B1) non-inverting PWM */
        | _BV(WGM11);                 /* mode 14: fast PWM, TOP=ICR1 */
  TCCR1B = _BV(WGM13) | _BV(WGM12)
        | _BV(CS10);                  /* prescaler 1 */
  ICR1 = icr;                         /* TOP counter value (freeing OCR1A*/
}

/* 16-bit version of analogWrite(). Works only on pins 9 and 10. */
void analogWrite16(uint8_t pin, uint16_t val)
{
    OCR1A = val;//pin9-sel1
} 
