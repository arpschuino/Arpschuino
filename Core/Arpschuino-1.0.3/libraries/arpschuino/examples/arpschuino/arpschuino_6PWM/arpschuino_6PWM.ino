/* 
officiel 05_05_16
ecrit pour l'arpschuino  www.arpschuino.fr
un code pour 6 output en pwm (arpower ou relay ou flexinarp)
Adressage avec l'Arpdress board (avec I2CattinyDip_09_01_15)
pour que l'arpdress-board fonctionne, le port bas doit être débranché.

Adressage avec l'Arpdress board :
2 clignotements brefs = l'adresse est prise depuis l'EEPROM.
  Si l'arpdress board est branché cela peu vouloir dire que le port bas n'est pas débranché.
1 cligotement jaune (arpdress board) et 1 vert =  l'adresse est prise depuis l'arpdress board et écrite dans l'EEPROM.
1 cligotement jaune (arpdress board) et 10 vert =  l'adresse est non conforme, l'adresse par defaut est prise.

Puis la led verte vibrillonne tant qu'elle reçoit du DMX.

Addressing with arpdress board :
2 short flashes = the address is taken from the EEPROM.
1 yellow flash on the arpdress-board and 1 green flash = address is taken from the arpdress-board and written in EEPROM.
1 yellow flash on the arpdress-board and 10 green flashes =  the address is not compliant, the default address is taken.

Then the green led blink quiquely while the board receive DMX.

05_05_16 : fonctions dans un fichier separe
26_12_16 : debug, la variable adress doit être un int et non un byte pour des adresses supérieures à 255.
28_10_17 : modifie pour integrer arduino core
*/
#include <Arpschuino.h>

#include <lib_dmx.h>
int adress;//debug 26/12/16 pour les adresses au delas de 255...
int nbre_circuits (6);///////////////////

//////////////////////////PATCH////////////////////////////////////////////
byte output [6] = {Arp0,Arp1,Arp2,Arp5,Arp6,Arp7};
//////////////////////////////////////////////////////////////////////////

void setup()
{ 
  pinMode(LED_BUILTIN, OUTPUT);  //met la led verte (temoin) en output
  digitalWrite(LED_BUILTIN,HIGH);
  
//modification de la fréquence http://playground.arduino.cc/Code/PwmFrequency
//decommenter si besoin
//  TCCR0B = TCCR0B & 0b11111000 | 001;// PWM out 5 et 6, timer 1 (62500 Hz).
//  TCCR1B = TCCR1B & 0b11111000 | 001;// PWM out 9 et 10, timer 2 (31250 Hz).
//  TCCR2B = TCCR2B & 0b11111000 | 001;// PWM out 3 et 11, timer 3 (31250 Hz).

  arpdress_board();//prise en charge de l'arpdress board
  //à commenter pour une adresse fixe

  for(int i=0;i<=nbre_circuits;i++)
  {
    pinMode(output[i], OUTPUT); 
  }
//////////////////////////////////////////////////////////////////:
  ArduinoDmx0.attachRXInterrupt  (frame_received);
  ArduinoDmx0.set_control_pin(ArpDMXControl);    // Arduino output pin for MAX485 input/output control (connect to MAX485 pins 2-3) 
  ArduinoDmx0.set_rx_address(adress);      // dmx start address
  ArduinoDmx0.set_rx_channels(nbre_circuits);     // number of rx channels
  ArduinoDmx0.init_rx(DMX512);        // starts universe 0 as rx, NEW Parameter DMX mode
}

void loop() 
{
  delay(500);//la led s'éteind apres 500ms sans reception DMX
  digitalWrite(LED_BUILTIN,HIGH);  //
} 

void frame_received(uint8_t universe) // cette boucle est executé à chaque réception d'une trame DMX
{
  led_temoin ();//la led clignote si elle reçoit un signal DMX  
   
  ///////////////lecture et report des niveau DMX//////////////////////////////////
  for(int i=0;i<nbre_circuits;i++)
  {
    analogWrite(output[i],ArduinoDmx0.RxBuffer[i]);
  } 
} 

