//reception JNode, ecriture dans l'arpower
// work with "emeteur_arpschuino_officiel" or "emeteur_JLink_bridge_arp" as emiter
//8 circuits off/on


// JNode reception, writing in arpower
// OK, works with [jee_link_to_JNode]
//8 channels off/on

//http://www.arpschuino.fr

#include <ArpRFLib.h>
byte NODEID = 3;  //Adresse RF unique pour chaque machine
#define NETWORKID  212  //adresse du reseau commune à toute les machine
int freq = RF12_868MHZ; //frequence de l'emeteur RF12
int identifiant = 0; //adresse du recepteur = adresse dmx + son adresse
////////////////////////PATCH////////////////////////////////////////////
byte output [16] = {17,7,16,6,15,5,14,4};

#define bascule 127  //la valeure ou passer de LOW à HIGH



void setup() {
  
    rf12_initialize(NODEID, freq, NETWORKID);

  for (byte i=0; i<8 ;i++)
  {
     pinMode (output[i], OUTPUT);
  } 
}

void loop() {

  if (rf12_recvDone() && rf12_crc == 0) {

     for(int i=0;i<8;i++)
     { 
        if (rf12_data[i]> bascule ){
          digitalWrite(output[i+identifiant], HIGH);
        }
        else{
          digitalWrite(output[i+identifiant], LOW);   
        }       
     }

     delay(15);
  }
}
     
