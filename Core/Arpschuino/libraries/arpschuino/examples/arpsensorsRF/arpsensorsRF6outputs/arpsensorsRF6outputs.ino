/*09/01/19
/*6 sorties pour l'arpsensorsRF (http://arpschuino.fr/arpsensorsRF.php)
 *6 outputs for arpsensorsRF (http://arpschuino.fr/arpsensorsRF_e.php)
 */
#include <Arpschuino.h>
#include <ArpRFLib.h>
byte NODEID = 4;  //Adresse RF
#define NETWORKID  212  //adresse du reseau commune à toute les machine
uint16_t band = RF12_868MHZ; //frequence de l'emeteur RF12
//RF12_915MHZ America  // RF12_433MHZ

float frequency_setting = 868.00; //beetween 863,00 > 870,00
//float frequency_setting = 915.00 ;//902;1–918 MHz (America, China and eastern Pacific Islands)

int identifiant = 0;

#define bascule 127

byte output [8] = {Arp0,Arp1,Arp2,Arp3,Arp4,Arp5};



void setup() {
  rf12_initialize(NODEID, band, NETWORKID,rf12_calcul_freq(frequency_setting));
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
