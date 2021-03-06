//reception JNode, ecriture dans l'arpower.
// S'utilise avec "emeteur_arpschuino_officiel" ou "emeteur_JLink_bridge_arp" comme emeteur.
//8 circuits off/on + led de signal.
//soties sur les 4 ports du JeeNode, out 1 a 8 de l'arpower avec JeeNode>arpower adapter http://www.arpschuino.fr/adapt_JNode.php
//led temoin sur la sorie I, la LED sur le JNode>arpower² http://www.arpschuino.fr/adapt_JNode.php

// JNode reception, writing in arpower.
// Works with "emeteur_arpschuino_officiel" or "emeteur_JLink_bridge_arp" as transmitter.
//8 channels off/on + signal LED
//output on the 4 ports of the JeeNode, out 1 to 8 on arpower with JeeNode>arpower adapter http://www.arpschuino.fr/adapt_JNode_e.php
//signal led on JeeNode's I output, the LED on JNode>arpower² http://www.arpschuino.fr/adapt_JNode_e.php

//http://www.arpschuino.fr



#include <ArpRFLib.h>
#include <Arpschuino.h>
byte NODEID = 3;  //Adresse RF unique pour chaque machine

uint16_t band = RF12_868MHZ; //frequence de l'emeteur RF12
//RF12_915MHZ America  // RF12_433MHZ

#define NETWORKID  212  //adresse du reseau commune à toute les machine
float frequency_setting = 868.00; //beetween 863,00 > 870,00
//float frequency_setting = 915.00 ;//902;1–918 MHz (America, China and eastern Pacific Islands)

int identifiant = 0; //adresse du recepteur = adresse dmx + son adresse
const byte temoin=3;//Jeenode irq  https://jeelabs.net/attachments/download/894/JeeNode%20Pinout%20Diagram.pdf
//////////////////////////////////
unsigned long previousMillis = 0;
const long interval = 1000; 

////////////////////////PATCH////////////////////////////////////////////
byte output [16] = {17,7,16,6,15,5,14,4};

#define bascule 127  //la valeur ou passer de LOW à HIGH



void setup() 
{  
    rf12_initialize(NODEID, band, NETWORKID,rf12_calcul_freq(frequency_setting));

    for (byte i=0; i<8 ;i++)
    {
       pinMode (output[i], OUTPUT);
    } 
    pinMode (temoin, OUTPUT);// led temoin https://jeelabs.net/attachments/download/894/JeeNode%20Pinout%20Diagram.pdf
}

void loop() 
{
    if (rf12_recvDone() && rf12_crc == 0) 
    {
       Arp_led_temoin(temoin);
       for(int i=0;i<8;i++)
       { 
          if (rf12_data[i]> bascule ){
            digitalWrite(output[i+identifiant], HIGH);
          }
          else{
            digitalWrite(output[i+identifiant], LOW);   
          }       
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
     
