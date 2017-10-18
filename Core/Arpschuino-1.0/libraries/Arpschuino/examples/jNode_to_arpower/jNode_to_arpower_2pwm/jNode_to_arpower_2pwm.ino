//reception JNode, ecriture dans l'arpower
//OK, fonctionne avec [jee_link_to_JNode]
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

#include <RF12.h>

const byte led_temoin=17;//Jeenode PA4
boolean led =0;



void setup() {

    
    TCCR0B = TCCR0B & 0b11111000 | 001; //frequence 
    rf12_initialize(2, RF12_868MHZ, 212);

    pinMode(led_temoin,OUTPUT);
    digitalWrite(led_temoin, HIGH);

    pinMode (5, OUTPUT);
    pinMode (6, OUTPUT);

   


}

void loop() {
  

      if (rf12_recvDone() && rf12_crc == 0) {
        if(led == 0)
        {
          digitalWrite  (led_temoin,  LOW); 
          led = 1;
        }
        else
        {
          digitalWrite  (led_temoin,  HIGH);
          led = 0;
        }

        analogWrite (6, rf12_data[0]);

        analogWrite (5, rf12_data[1]);
        delay(15);
     }
     
     else{
      digitalWrite(led_temoin, LOW);
     }
     
            
}
