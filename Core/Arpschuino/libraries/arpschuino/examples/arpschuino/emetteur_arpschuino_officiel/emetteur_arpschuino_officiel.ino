/*officiel 02/12/17
ecrit pour l'arpschuino  www.arpschuino.fr
Un arpRF doit etre branche sur le port haut (http://arpschuino.fr/arpRF.php)
attention le jumper de tension en position 3v !
reçoit du dmx, envoie vers jNode, wilulu, arpsensorsRF ou arptefact::transceiver

Adressage avec l'arpdress board (http://arpschuino.fr/arpdress_board.php)
pour que l'arpdress-board fonctionne, le port bas doit etre debranche.

Adressage avec l'Arpdress board :
2 clignotements brefs = l'adresse est prise depuis l'EEPROM.
1 cligotement jaune (arpdress board) et 1 vert =  l'adresse est prise depuis l'arpdress board et ecrite dans l'EEPROM.
1 cligotement jaune (arpdress board) et 10 vert =  l'adresse est non conforme, la derniere adresse ecrite dans l'EEPROM est prise..

Puis la led verte vibrillonne tant qu'elle recoit du DMX.

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
official 29/10/17
written for arpschuino  http://arpschuino.fr/index_e.php
An arpRF must be connected to the high port (http://arpschuino.fr/arpRF_e.php)
watch out the voltage jumper in 3v position! 
receives from dmx, transmit to jNode, wilulu, arpsensorsRF or artefact::transceiver

Adressing with arpdress board (http://arpschuino.fr/arpdress_board_e.php)
for the arpdress-board to work, the low port must be unplugged

Addressing with arpdress board :
2 short flashes = the address is taken from the EEPROM.
1 yellow flash on the arpdress-board and 1 green flash = address is taken from the arpdress-board and written in EEPROM.
1 yellow flash on the arpdress-board and 10 green flashes =  the address is not compliant, the last address written in the EEPROM is taken.

Then the green led blink quiquely while the board receive DMX.

//06/01/19 :  changement de fréquence
*/
#include <Arpschuino.h>
#include <lib_dmx.h>   // pour la recepionn dmx

int nbre_circuits (66);//min 2, max 66
int address;

#include <ArpRFLib.h>
byte NODEID = 1;  //Adresse RF unique pour chaque machine de 1 à 30
#define NETWORKID  212  //adresse du reseau commune à toute les machine
uint8_t band = RF12_868MHZ; //frequence de l'emeteur RF12
//RF12_915MHZ America  // RF12_433MHZ

float frequency_setting = 868.00; //beetween 863,00 > 870,00
//float frequency_setting = 915.00 ;//902;1–918 MHz (America, China and eastern Pacific Islands)

byte buffer_to_send[66];

void setup() {

  Arp_arpdress_board();//prise en charge de l'arpdress board, à commenter pour une adresse fixe


  ArduinoDmx0.attachRXInterrupt  (frame_received);
  ArduinoDmx0.set_control_pin(ArpDMXControl);   // Arduino output pin for MAX485 input/output control (connect to MAX485-1 pins 2-3) 
  ArduinoDmx0.set_rx_address(address);    // set tx start address
  ArduinoDmx0.set_rx_channels(nbre_circuits); // 2 to 512 in standard mode) See lib_dmx.h 
  ArduinoDmx0.init_rx(DMX512);    

  rf12_initialize(NODEID, band, NETWORKID,calcul_freq(frequency_setting));

  pinMode(LED_BUILTIN, OUTPUT); 
}

void loop() {

  delay(500);// apres 500ms sans reception DMX
  digitalWrite(LED_BUILTIN,HIGH);//la led s'eteind
}


void frame_received(uint8_t universe) // Custom ISR: fired when all channels in one universe are received
{
  Arp_led_temoin ();  
  /////////////////

  memcpy((void *)buffer_to_send, (void *)ArduinoDmx0.RxBuffer, nbre_circuits);

  rf12_sendNow(0, &buffer_to_send,nbre_circuits);
}
