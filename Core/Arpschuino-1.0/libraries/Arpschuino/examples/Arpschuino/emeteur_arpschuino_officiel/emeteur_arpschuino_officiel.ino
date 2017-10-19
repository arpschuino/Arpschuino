//reçoit du dmx, envoie vers jNode
// commencé le 11/11/15

//OK, fonctionne avec eric_jnode2/3_to_arpower05
//wilulu

//reprise 28/12/16
//adresse en int
//66 circuits
//sortie directe sur le port du bas
//memcpy
//a verifier !OK!!
//adresse max 447.(446+66=512)

#include "fonctions.h"
#include <lib_dmx.h>   // pour la recepionn dmx
#define    DMX512     (0) 
#define default_adress (1)//adresse par defaut
#define nbre_circuits (66)//
int adress=1;

#include <ArpRFLib.h>
byte NODEID = 1;  //Adresse RF unique pour chaque machine de 1 à 30
#define NETWORKID  212  //adresse du reseau commune à toute les machine
int freq = RF12_868MHZ; //frequence de l'emeteur RF12

byte ledPin[8]={2,8,14,15,16,17,18,19};

byte buffer_to_send[nbre_circuits];

void setup() {

  arpdress_board();//prise en charge de l'arpdress board, à commenter pour une adresse fixe
  
  ArduinoDmx0.attachRXInterrupt  (frame_received);
  ArduinoDmx0.set_control_pin(7);   // Arduino output pin for MAX485 input/output control (connect to MAX485-1 pins 2-3) 
  ArduinoDmx0.set_rx_address(adress);    // set tx start address
  ArduinoDmx0.set_rx_channels(nbre_circuits); // 2 to 512 in standard mode) See lib_dmx.h 
  ArduinoDmx0.init_rx(DMX512);    

  rf12_initialize(NODEID, freq, NETWORKID);
  pinMode(LED_BUILTIN, OUTPUT); 
}

void loop() {

  delay(500);//la led s'eteind apres 500ms sans reception DMX
  bitWrite (PORTD,4,1);
}


void frame_received(uint8_t universe) // Custom ISR: fired when all channels in one universe are received
{
  led_temoin ();  
  /////////////////

  memcpy((void *)buffer_to_send, (void *)ArduinoDmx0.RxBuffer, nbre_circuits);

  rf12_sendNow(0, &buffer_to_send,nbre_circuits);
}



