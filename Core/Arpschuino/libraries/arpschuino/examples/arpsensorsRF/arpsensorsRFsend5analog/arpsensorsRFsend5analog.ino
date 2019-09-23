/*
 *ecrit pour l'arpsensorsRF (http://arpschuino.fr/arpsensorsRF.php)
 *lit les valeurs de 5 capteurs analogiques (entrees 0 a 4)
 *lit 16 fois les valeurs et envoies la moyenne par liaison radio (RF12)
 *Clignote sur la sortie 5 a chaque emission
 *recepteurs : wilulu, arpschuino+arpRF, Jeelink ...
 *

 *writen for arpsensorsRF (http://arpschuino.fr/arpsensorsRF_e.php)  
 * reads the values of 5 analog sensors (inputs 0 to 4)
 * reads 16 times the values and sends the average over the air (RF12)
 * Blinks on output 5 with each broadcast
 * receivers: wilulu, arpschuino arpRF, Jeelink ... 
 */
 
#include <ArpRFLib.h>

byte NODEID = 1;  
#define NETWORKID  212  //adresse du reseau commune à toute les machine
uint16_t band = RF12_868MHZ; //frequence de l'emeteur RF12
//RF12_915MHZ America  // RF12_433MHZ

float frequency_setting = 868.00; //beetween 863,00 > 870,00
//float frequency_setting = 915.00 ;//902;1–918 MHz (America, China and eastern Pacific Islands)

#define nbr_analog 5
const byte analogIn [nbr_analog] ={A0,A1,A2,A3,A4};
int sensVal[nbr_analog]={0};  //les valeurs analogiques a renvoyer
bool var=0;

void setup() 
{
    rf12_initialize(NODEID, band, NETWORKID,rf12_calcul_freq(frequency_setting));
    pinMode(Arp5,OUTPUT);
}

void loop() {
  for(byte i=0;i<nbr_analog;i++)
  {         //lecture des analogs 
      uint16_t sum=0;
      uint16_t temp_val =0;
      for (int j = 0;j < 16;j++)//On lit 16 fois la valeur
      {
        temp_val = analogRead(analogIn[i]); 
        sum = sum +temp_val;
      }
      temp_val = sum>>4;//la moyenne des 16 samples
      
      sensVal[i]=byte(temp_val>>2);//decale la valeur de 2 bits pour avoir une valeur 8 bits
                                   //peut être remplacé par un calibrage
  } 
  delay(10); 

  digitalWrite (Arp5,var);
  var = var-1;
  rf12_sendNow(0, &sensVal,nbr_analog);
  delay(10);
}
