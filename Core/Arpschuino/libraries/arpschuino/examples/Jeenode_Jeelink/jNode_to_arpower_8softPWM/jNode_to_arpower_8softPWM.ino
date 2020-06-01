//reception JNode, ecriture dans l'arpower.
// S'utilise avec "emeteur_arpschuino_officiel" ou "emeteur_JLink_bridge_arp" comme emeteur.
//8 circuits gradués en softPWM + LED de signal.
//soties sur les 4 ports du JeeNode, out 1 a 8 de l'arpower avec JeeNode>arpower adapter http://www.arpschuino.fr/adapt_JNode.php
//led temoin sur la sorie I, la LED sur le JNode>arpower² http://www.arpschuino.fr/adapt_JNode.php

// JNode reception, writing in arpower.
// Works with "emeteur_arpschuino_officiel" or "emeteur_JLink_bridge_arp" as transmitter.
//8 channels dimed by softPWM PWM + signal LED.
//output on the 4 ports of the JeeNode, out 1 to 8 on arpower with JeeNode>arpower adapter http://www.arpschuino.fr/adapt_JNode_e.php
//signal led on JeeNode's I output, the LED on JNode>arpower² http://www.arpschuino.fr/adapt_JNode_e.php

//http://www.arpschuino.fr


#include <ArpRFLib.h>
#include <Arpschuino.h>
byte NODEID = 3;  //Adresse RF unique pour chaque machine
#define NETWORKID  212  //adresse du reseau commune à toute les machine
uint16_t band = RF12_868MHZ; //frequence de l'emeteur RF12
//RF12_915MHZ America  // RF12_433MHZ

float frequency_setting = 868.00; //beetween 863,00 > 870,00
//float frequency_setting = 915.00 ;//902;1–918 MHz (America, China and eastern Pacific Islands)

int identifiant = 0; //adresse du recepteur = adresse dmx + son adresse
const byte temoin=3;//Jeenode irq  https://jeelabs.net/attachments/download/894/JeeNode%20Pinout%20Diagram.pdf
//////////////////////////////////
unsigned long previousMillis = 0;
const long interval = 1000; 

#define __DEBUG_SOFTPWM__ 0
#include <SoftPWM.h>

//////////////////////////PATCH////////////////////////////////////////////

SOFTPWM_DEFINE_CHANNEL( 0, DDRC, PORTC, PORTC3);
SOFTPWM_DEFINE_CHANNEL( 1, DDRD, PORTD, PORTD7 );
SOFTPWM_DEFINE_CHANNEL( 2, DDRC, PORTC, PORTC2 );
SOFTPWM_DEFINE_CHANNEL( 3, DDRD, PORTD, PORTD6 );
SOFTPWM_DEFINE_CHANNEL( 4, DDRC, PORTC, PORTC1 );
SOFTPWM_DEFINE_CHANNEL( 5, DDRD, PORTD, PORTD5 );
SOFTPWM_DEFINE_CHANNEL( 6, DDRC, PORTC, PORTC0 );
SOFTPWM_DEFINE_CHANNEL( 7, DDRD, PORTD, PORTD4 );


SOFTPWM_DEFINE_OBJECT( 8 );


void setup() 
{ 
    rf12_initialize(NODEID, band, NETWORKID,rf12_calcul_freq(frequency_setting));
    pinMode (temoin, OUTPUT);// led temoin https://jeelabs.net/attachments/download/894/JeeNode%20Pinout%20Diagram.pdf
    pinMode (4, OUTPUT);
    pinMode (5, OUTPUT);
    pinMode (6, OUTPUT);
    pinMode (7, OUTPUT);
    pinMode (14, OUTPUT);
    pinMode (15, OUTPUT);
    pinMode (16, OUTPUT);
    pinMode (17, OUTPUT);  
    
    Palatis::SoftPWM.begin(180);

}

void loop() 
{
    if (rf12_recvDone() && rf12_crc == 0) 
    {
       Arp_led_temoin(temoin);
       for(int i=0;i<8;i++)
       { 
          Palatis::SoftPWM.set(i, rf12_data[i+identifiant]); 
       }
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
     
