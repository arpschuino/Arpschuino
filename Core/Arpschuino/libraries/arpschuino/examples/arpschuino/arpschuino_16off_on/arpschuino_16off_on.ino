/* 
officiel 29/10/17
ecrit pour l'arpschuino  http://arpschuino.fr
un code pour 16 output en off/on (arpower ou relay ou flexinarp)
Adressage avec l'arpdress board (avec I2CattinyDip_09_01_15)
pour que l'arpdress-board fonctionne, le port bas doit être débranché.

Adressage avec l'Arpdress board :
2 clignotements brefs = l'adresse est prise depuis l'EEPROM.
  Si l'arpdress board est branché cela peu vouloir dire que le port bas n'est pas débranché.
1 cligotement jaune (arpdress board) et 1 vert =  l'adresse est prise depuis l'arpdress board et écrite dans l'EEPROM.
1 cligotement jaune (arpdress board) et 10 vert =  l'adresse est non conforme, la derniere adresse ecrite dans l'EEPROM est prise.

Puis la led verte vibrillonne tant qu'elle reçoit du DMX.

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
official 29/10/17
written for arpschuino  http://arpschuino.fr/index_e.php
16 off/on output (arpower or relay board. 
Adressing with arpdress board (with I2CattinyDip_09_01_15)
for the arpdress-board to work, the low port must be unplugged

Addressing with arpdress board :
2 short flashes = the address is taken from the EEPROM.
1 yellow flash on the arpdress-board and 1 green flash = address is taken from the arpdress-board and written in EEPROM.
1 yellow flash on the arpdress-board and 10 green flashes =  the address is not compliant, the last address written in the EEPROM is taken.

Then the green led blink quiquely while the board receive DMX.

05_05_16 : fonctions dans un fichier separe
26_12_16 : debug, la variable adress doit être un int et non un byte pour des adresses supérieures à 255.
28_10_17 : modifie pour integrer arduino core
08_01_19 : library mise a jour
*/
#include<Arpschuino.h>

#include <lib_dmx.h>
int address;
int nbre_circuits (16);

//////////////////////////PATCH////////////////////////////////////////////
byte output [16] = {Arp0,Arp1,Arp2,Arp3,Arp4,Arp5,Arp6,Arp7,Arp8,Arp9,Arp10,Arp11,Arp12,Arp13,Arp14,Arp15};
#define bascule 127  //la valeur où passer de LOW à HIGH
///////////////////////////////////////////////////////////////////////

void setup(){

  pinMode(LED_BUILTIN, OUTPUT);  //met la led verte (temoin) en output
  digitalWrite(LED_BUILTIN,HIGH); //comprtement inverse
  
  Arp_arpdress_board();//prise en charge de l'arpdress board
  //à commenter pour une adresse fixe


  for(int i=0;i<nbre_circuits;i++)//met les 16 sorties en output
  {
     pinMode(output[i], OUTPUT); 
  }
///////////////////////////////////////////////////////////////////////
  ArduinoDmx0.attachRXInterrupt  (frame_received);
  ArduinoDmx0.set_control_pin(ArpDMXControl);    // Arduino output pin for MAX485 input/output control (connect to MAX485 pins 2-3) 
  ArduinoDmx0.set_rx_address(address);      // dmx start address
  ArduinoDmx0.set_rx_channels(nbre_circuits);     // number of rx channels
  ArduinoDmx0.init_rx(DMX512);        // starts universe 0 as rx
}

void loop()
{
  delay(500);// apres 500ms sans reception DMX
  digitalWrite(LED_BUILTIN,HIGH);//la led s'éteind
} 

void frame_received(uint8_t universe) // cette boucle est executé à chaque réception d'une trame DMX
{
   Arp_led_temoin ();//la led clignote si elle reçoit un signal DMX  
   
   for(int i=0;i<nbre_circuits;i++)
   {
      if (ArduinoDmx0.RxBuffer[i]> bascule ){
        digitalWrite(output [i], HIGH);
      }
      else{
        digitalWrite(output [i], LOW);   
      }
   }
}
