//reçoit du dmx, envoie vers jNode ou wilulu
//66 circuit max
//officiel 02/12/17

//16/05/18 reglage de la frequence

#include <lib_dmx.h>   // pour la reception dmx
#include <ArpRFLib.h>

#define    DMX512     (0) 
int address (1); //adresse de reception dmx
int nbre_circuits (66); //nombre de circuit dmx reçu depuis l'adresse
byte NODEID = 1;  //Adresse RF unique
#define NETWORKID  212  //adresse du reseau commune à toute les machine
uint8_t band = RF12_868MHZ; //frequence de l'emeteur RF12
//RF12_915MHZ America  // RF12_433MHZ

float frequency_setting = 868.00; //beetween 863,00 > 870,00
//float frequency_setting = 915.00 ;//902;1–918 MHz (America, China and eastern Pacific Islands)

boolean var_led=0;
byte buffer_to_send[66];

void setup() {

  int debordement = address+nbre_circuits-513;
  if(debordement>0)
  {
    nbre_circuits = nbre_circuits-debordement;
  }

  ArduinoDmx0.attachRXInterrupt  (frame_received);
  ArduinoDmx0.set_control_pin(-1);   // Arduino output pin for MAX485 input/output control (connect to MAX485-1 pins 2-3) 
  ArduinoDmx0.set_rx_address(address);         // set tx start address
  ArduinoDmx0.set_rx_channels(nbre_circuits); // 2 to 512 in standard mode) See lib_dmx.h 
  ArduinoDmx0.init_rx(DMX512);    

  rf12_initialize(NODEID, band, NETWORKID,rf12_calcul_freq(frequency_setting));

  pinMode(9, OUTPUT); 
}

void loop() {

  delay(500);//la led s'eteind apres 500ms sans reception DMX
  analogWrite(9,255);
  delay(500);

}


void frame_received(uint8_t universe) // Custom ISR: fired when all channels in one universe are received
{
  if (universe == 0) // USART0
  {
    if(var_led == 0)
    {
      analogWrite(9,150); 
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
