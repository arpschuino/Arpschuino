//reception JNode, ecriture dans l'arpower
// work with "emeteur_arpschuino_officiel" or "emeteur_JLink_bridge_arp" as emiter
//2 circuits gradués en PWM ardware + led de signal
//modification frequence pwm
//aucun fliker, moteurs silencieux
//soties sur PD2 et 3 du JeeNode, out 4 et 6 de l'arpower avec JeeNode>arpower adapter http://www.arpschuino.fr/adapt_JNode.php
//led temoin sur PA4 du JeeNode, out 1 de l'arpower avec JeeNode>arpower adapter http://www.arpschuino.fr/adapt_JNode.php

// JNode reception, writing in arpower
// OK, works with [jee_link_to_JNode]
//2 channels dimed by hardware PWM + signal led
//pwm frequency modification
//no flicker, quiet motors
//output PD2 and 3 on JeeNode, out 4 and 6 on arpower with JeeNode>arpower adapter http://www.arpschuino.fr/adapt_JNode_e.php
//signal led on JeeNode's PA4,out 1 on arpower with JeeNode>arpower adapter http://www.arpschuino.fr/adapt_JNode_e.php

//http://www.arpschuino.fr

#include <ArpRFLib.h>
#include <Arpschuino.h>
byte NODEID = 2;  //Adresse RF unique pour chaque machine
#define NETWORKID  212  //adresse du reseau commune à toute les machine
int freq = RF12_868MHZ; //frequence de l'emeteur RF12
int identifiant = 0; //adresse du recepteur = adresse dmx + son adresse
const byte temoin=17;//Jeenode PA4  https://jeelabs.net/attachments/download/894/JeeNode%20Pinout%20Diagram.pdf


void setup() {
    
    TCCR0B = TCCR0B & 0b11111000 | 001; //frequence 
    rf12_initialize(NODEID, freq, NETWORKID);

    pinMode(temoin,OUTPUT);
    pinMode (5, OUTPUT);
    pinMode (6, OUTPUT);
}

void loop() {
  
      if (rf12_recvDone() && rf12_crc == 0) {

        led_temoin(temoin);
        analogWrite (6, rf12_data[0+identifiant]);

        analogWrite (5, rf12_data[1+identifiant]);
        delay(15);
     }        
}
