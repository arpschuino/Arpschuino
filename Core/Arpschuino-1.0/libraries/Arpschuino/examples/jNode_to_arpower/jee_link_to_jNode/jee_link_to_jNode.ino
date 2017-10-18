//pour jee link ou jee node via FTDI USB/TTL adapter
//faire reconnaitre le Jee link comme une Enttec open par le logiciel
//reçoit du dmx, envoie vers jNode
//fonctionne avec [jNode_to_arpower_8offOn] ou  [jNode_to_arpower_8pwm] comme recepteur


// To jee link or jee node via FTDI USB / TTL adapter
//Do reconize the jee link as an enttec open by the sofware
//Receives dmx, send to JNode
// Works with [jNode_to_arpower_8offOn] or [jNode_to_arpower_8pwm] as receiver

//http://www.arpschuino.fr

#include <lib_dmx.h>   // pour la recepion dmx
#define    DMX512     (0) 
#define default_adress (1)//adresse par defaut - default address
#define nbre_circuits (8)//
byte address=1;


#include <RF12.h>


boolean var_led=0;


//byte ledPin[2]={5,6};

byte buffer_to_send[nbre_circuits];

void setup() {
  
  ArduinoDmx0.attachRXInterrupt  (frame_received);
  ArduinoDmx0.set_control_pin(-1);   // no MAX485 input/output control
  ArduinoDmx0.set_rx_address(address);    // set tx start address
  ArduinoDmx0.set_rx_channels(nbre_circuits); // 2 to 512 in standard mode) See lib_dmx.h 
  ArduinoDmx0.init_rx(DMX512);    

  rf12_initialize(1, RF12_868MHZ, 212);

  pinMode(9, OUTPUT);
  digitalWrite(9,HIGH); 


}

void loop() {

  delay(500);//la led s'eteind apres 500ms sans reception DMX
  bitWrite (PORTB,1,1);
  
}


void frame_received(uint8_t universe) // Custom ISR: fired when all channels in one universe are received
{///////////////////////////////////////////////////led temoin///////////////////////////////////////////
  if (universe == 0) // USART0
  {
    if(var_led == 0)
    {
      bitWrite (PORTB,1,0); 
      var_led = 1;
    }
    else
    {
      bitWrite (PORTB,1,1);
      var_led = 0;
    }
  }


  
  /////////////////

  for(int i=0; i < nbre_circuits; i++)
  {
     buffer_to_send[i]=ArduinoDmx0.RxBuffer[i];
  }


  rf12_sendNow(0, &buffer_to_send,nbre_circuits);

}



