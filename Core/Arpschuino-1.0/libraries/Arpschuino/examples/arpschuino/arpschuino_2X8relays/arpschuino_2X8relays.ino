/* 
officiel, tuto 20_11_16
ecrit pour l'arpschuino  www.arpschuino.fr
un code pour 2 cartes 8 relais http://www.produktinfo.conrad.com/datenblaetter/75000-99999/095842-da-01-fr-RELAIS.pdf
Adressage avec l'Arpdress board (avec I2CattinyDip_09_01_15)
pour que l'arpdress-board fonctionne, le port bas doit être débranché.

Adressage avec l'Arpdress board :
2 clignotements brefs = l'adresse est prise depuis l'EEPROM.
  Si l'arpdress board est branché cela peu vouloir dire que le port bas n'est pas débranché.
1 cligotement jaune (arpdress board) et 1 vert =  l'adresse est prise depuis l'arpdress board et écrite dans l'EEPROM.
1 cligotement jaune (arpdress board) et 10 vert =  l'adresse est non conforme, l'adresse par defaut est prise.

Puis la led verte vibrillonne tant qu'elle reçoit du DMX.

Pas encore officiel, tuto 18_11_16
written for arpschuino  www.arpschuino.fr
For 2x 8 relais board http://www.produktinfo.conrad.com/datenblaetter/75000-99999/095842-da-01-en-RELAIS.pdf

Addressing with arpdress board :
2 short flashes = the address is taken from the EEPROM.
1 yellow flash on the arpdress-board and 1 green flash = address is taken from the arpdress-board and written in EEPROM.
1 yellow flash on the arpdress-board and 10 green flashes =  the address is not compliant, the default address is taken.

Then the green led blink quiquely while the board receive DMX.

26/12/16 rajout d'une initialisation relais ouverts
*/
#include "fonctions.h"

#include <lib_dmx.h>
#define default_adress (1)//adresse DMX par defaut
#define nbre_circuits (16)//
#define    DMX512     (0)  

int adress = default_adress;

//////////////////////////PATCH////////////////////////////////////////////
byte output [16] = {Arp0,Arp1,Arp2,Arp3,Arp4,Arp5,Arp6,Arp7,Arp8,Arp9,Arp10,Arp11,Arp12,Arp13,Arp14,Arp15};

#define bascule 127  //la valeur où passer de LOW à HIGH
///////////////////////////////////////////////////////////////////////

void setup(){

  pinMode(4, OUTPUT);  //met la led verte (temoin) en output
  bitWrite (PORTD,4,1);// equivalent a digitalWrite(led_temoin,HIGH);
  
  arpdress_board();//prise en charge de l'arpdress board
  //à commenter pour une adresse fixe
  
  for(int i=0;i<nbre_circuits;i++)//met les 16 sorties en output
    {
     pinMode(output [i], OUTPUT); //initialisation en sortie
     digitalWrite(output [i], HIGH);//initialisation relais ouvets    
    }
///////////////////////////////////////////////////////////////////////
  ArduinoDmx0.attachRXInterrupt  (frame_received);
  ArduinoDmx0.set_control_pin(7);    // Arduino output pin for MAX485 input/output control (connect to MAX485 pins 2-3) 
  ArduinoDmx0.set_rx_address(adress);      // dmx start address
  ArduinoDmx0.set_rx_channels(nbre_circuits);     // number of rx channels
  ArduinoDmx0.init_rx(DMX512);        // starts universe 0 as rx, NEW Parameter DMX mode
}

void loop()
{
  delay(500);//la led s'éteind apres 500ms sans reception DMX
  bitWrite (PORTD,4,1);
} 

void frame_received(uint8_t universe) // cette boucle est executé à chaque réception d'une trame DMX
{
 led_temoin ();//la led clignote si elle reçoit un signal DMX  
 
 for(int i=0;i<=nbre_circuits;i++)
   {
      if (ArduinoDmx0.RxBuffer[i]> bascule ){
        digitalWrite(output [i], LOW);
      }
      else{
        digitalWrite(output [i], HIGH);   
      }
   }
}

