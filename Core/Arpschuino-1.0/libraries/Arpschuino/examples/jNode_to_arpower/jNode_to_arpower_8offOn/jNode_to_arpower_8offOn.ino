//reception JNode, ecriture dans l'arpower
//OK, fonctionne avec [jee_link_to_JNode]
//8 circuits off/on


// JNode reception, writing in arpower
// OK, works with [jee_link_to_JNode]
//8 channels off/on

//http://www.arpschuino.fr

#include <RF12.h>

////////////////////////PATCH////////////////////////////////////////////
byte output [16] = {17,7,16,6,15,5,14,4};

#define bascule 127  //la valeure ou passer de LOW à HIGH



void setup() {
  
  rf12_initialize(2, RF12_868MHZ, 212);

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
          digitalWrite(output [i], HIGH);
        }
        else{
          digitalWrite(output [i], LOW);   
        }       
     }

     delay(15);
  }
}
     
