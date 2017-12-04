//reception JNode, ecriture dans l'arpower
// work with "emeteur_arpschuino_officiel" or "emeteur_JLink_bridge_arp" as emiter
//8 circuits gradués en softPWM

// JNode reception, writing in arpower
// OK, works with [jee_link_to_JNode]
//8 channels dimed by softPWM

//http://www.arpschuino.fr

#include <ArpRFLib.h>
#include <Arpschuino.h>
byte NODEID = 3;  //Adresse RF unique pour chaque machine
#define NETWORKID  212  //adresse du reseau commune à toute les machine
int freq = RF12_868MHZ; //frequence de l'emeteur RF12
int identifiant = 0; //adresse du recepteur = adresse dmx + son adresse

#define __DEBUG_SOFTPWM__ 0
#include <SoftPWMMaster_mod.h>

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


void setup() {
  
    rf12_initialize(NODEID, freq, NETWORKID);
  pinMode (LED_BUILTIN, OUTPUT);//D9 led temoin https://jeelabs.net/attachments/download/894/JeeNode%20Pinout%20Diagram.pdf
  pinMode (4, OUTPUT);
  pinMode (5, OUTPUT);
  pinMode (6, OUTPUT);
  pinMode (7, OUTPUT);
  pinMode (14, OUTPUT);
  pinMode (15, OUTPUT);
  pinMode (16, OUTPUT);
  pinMode (17, OUTPUT);  
  
  SoftPWM.begin(180);

}

void loop() {

  if (rf12_recvDone() && rf12_crc == 0) {
     led_temoin();
     for(int i=0;i<8;i++)
     { 
        SoftPWM.set(i, rf12_data[i+identifiant]); 
     }

     delay(15);
  }
}
     
