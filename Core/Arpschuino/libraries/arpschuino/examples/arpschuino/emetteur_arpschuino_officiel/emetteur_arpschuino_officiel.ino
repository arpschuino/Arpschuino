//reçoit du dmx, envoie vers jNode, wilulu, arpsensorsRF
//officiel 26/03/18
//Synchronise version rewrite from official by Anton with transmit periode to make it clean with your country legislation and avoid to block every 433 or 868 MHZ public frequency users.
// It may add a little bit latency in the system
// 66 circuits

#include <Arpschuino.h>
#include <lib_dmx.h>   // pour la recepionn dmx

int nbre_circuits (66);//
int adress;

#include <ArpRFLib.h>
byte NODEID = 1;  //Adresse RF unique pour chaque machine de 1 à 30
#define NETWORKID  212  //adresse du reseau commune à toute les machine
int freq = RF12_868MHZ; //frequence de l'emeteur RF12

int TRANSMITPERIOD = 50; //transmit periode to maintain a resonable duty cycle without to much latency
/////It's add a little bit of latency on the system but allow other users to use the same system in the same time without to much conflict and make you quite clean with the RF legislation of your country

byte buffer_to_send[66];

void setup() {

  arpdress_board();//prise en charge de l'arpdress board, à commenter pour une adresse fixe


  ArduinoDmx0.attachRXInterrupt  (frame_received);
  ArduinoDmx0.set_control_pin(ArpDMXControl);   // Arduino output pin for MAX485 input/output control (connect to MAX485-1 pins 2-3) 
  ArduinoDmx0.set_rx_address(adress);    // set tx start address
  ArduinoDmx0.set_rx_channels(nbre_circuits); // 2 to 512 in standard mode) See lib_dmx.h 
  ArduinoDmx0.init_rx(DMX512);    

  rf12_initialize(NODEID, freq, NETWORKID);
  pinMode(LED_BUILTIN, OUTPUT); 
}

unsigned long lastPeriod = 0;
void loop() 
{
  int currPeriod = millis()/TRANSMITPERIOD;
  if (currPeriod != lastPeriod)
  {
    radio.receiveDone();
    if(radio.canSend())
    {
      lastPeriod=currPeriod;
      rf12_sendNow(0, &ArduinoDmx0.RxBuffer,nbre_circuits);
    }
  }
  //la led s'eteind apres 500ms sans reception DMX
  if(lastPeriod+500<millis())
  {
  digitalWrite(LED_BUILTIN,HIGH);//la led s'eteind
  }
}


void frame_received(uint8_t universe) // Custom ISR: fired when all channels in one universe are received
{
  led_temoin ();  
}



