//reçoit du dmx, envoie vers jNode ou wilulu
// commencé le 11/11/15
//v2 le 12
//Fonctionne avec une arpschuino + arpRF
//66 circuit max

#include <lib_dmx.h>   // pour la reception dmx
#include <JeeLib.h>

#define    DMX512     (0) 
#define address (1) //adresse de reception dmx
#define nbre_circuits (66) //nombre de circuit dmx reçu depuis l'adresse
byte NODEID = 1;  //Adresse RF unique pour chaque machine
#define NETWORKID  212  //adresse du reseau commune à toute les machine
int freq = RF12_868MHZ; //frequence de l'emeteur RF12


boolean var_led=0;
byte ledPin[2]={5,6};
byte buffer_to_send[nbre_circuits];

void setup() {
  
  ArduinoDmx0.attachRXInterrupt  (frame_received);
  ArduinoDmx0.set_control_pin(7);   // Arduino output pin for MAX485 input/output control (connect to MAX485-1 pins 2-3) 
  ArduinoDmx0.set_rx_address(address);    // set tx start address
  ArduinoDmx0.set_rx_channels(nbre_circuits); // 2 to 512 in standard mode) See lib_dmx.h 
  ArduinoDmx0.init_rx(DMX512);    

  rf12_initialize(NODEID, freq, NETWORKID);

  pinMode(9, OUTPUT); 
}

void loop() {

  delay(500);//la led s'eteind apres 500ms sans reception DMX
  analogWrite(9,255);
}


void frame_received(uint8_t universe) // Custom ISR: fired when all channels in one universe are received
{
  if (universe == 0) // USART0
  {
    if(var_led == 0)
    {
      analogWrite(9,0); 
      var_led = 1;
    }
    else
    {
      analogWrite(9,255);
      var_led = 0;
    }
  }
  /////////////////

  memcpy((void *)buffer_to_send, (void *)ArduinoDmx0.RxBuffer, nbre_circuits);
  rf12_sendNow(0, &buffer_to_send,nbre_circuits);
}



