
/* 
officiel 29/10/17
ecrit pour l'arpschuino  www.arpschuino.fr
gradation soft PWM sur les 16 canneaux. 
Frequence PWM 180 hz
Adressage avec l'arpdress board (avec I2CattinyDip_09_01_15)
pour que l'arpdress-board fonctionne, le port bas doit etre debranche.

Adressage avec l'Arpdress board :
2 clignotements brefs = l'adresse est prise depuis l'EEPROM.
1 cligotement jaune (arpdress board) et 1 vert =  l'adresse est prise depuis l'arpdress board et ecrite dans l'EEPROM.
1 cligotement jaune (arpdress board) et 10 vert =  l'adresse est non conforme, la derniere adresse ecrite dans l'EEPROM est prise..

Puis la led verte vibrillonne tant qu'elle recoit du DMX.

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
official 29/10/17
written for arpschuino  www.arpschuino.fr
soft PWM 16 channels dimmer. 
Frequency PWM 180 hz
Adressing with arpdress board (with I2CattinyDip_09_01_15)
for the arpdress-board to work, the low port must be unplugged.

Addressing with arpdress board :
2 short flashes = the address is taken from the EEPROM.
1 yellow flash on the arpdress-board and 1 green flash = address is taken from the arpdress-board and written in EEPROM.
1 yellow flash on the arpdress-board and 10 green flashes =  the address is not compliant, the last address written in the EEPROM is taken.

Then the green led blink quiquely while the board receive DMX.

05_05_16 : fonctions dans un fichier separe
26_12_16 : debug, la variable adress doit etre un int et non un byte pour des adresses superieures Ã  255.
29_10_17 : modifie pour integrer arpschuino core
06_01_19 : modifie pour Arduino 1.8.6 et suivantes, library mise a jour
*/
#include<Arpschuino.h>

#include <lib_dmx.h>
int nbre_circuits (16);
int address;

#include <SoftPWM.h>

///////////////PATCH/SOFTPWM/////////////////////
SOFTPWM_DEFINE_CHANNEL( 0, DDRD, PORTD, PORTD3 ); //D3
SOFTPWM_DEFINE_CHANNEL( 1, DDRD, PORTD, PORTD5 );//D5
SOFTPWM_DEFINE_CHANNEL( 2, DDRD, PORTD, PORTD6 );//D6
SOFTPWM_DEFINE_CHANNEL( 3, DDRB, PORTB, PORTB5 );//D13
SOFTPWM_DEFINE_CHANNEL( 4, DDRB, PORTB, PORTB4 );//D12
SOFTPWM_DEFINE_CHANNEL( 5, DDRB, PORTB, PORTB1 );//D9
SOFTPWM_DEFINE_CHANNEL( 6, DDRB, PORTB, PORTB2 );//D10
SOFTPWM_DEFINE_CHANNEL( 7, DDRB, PORTB, PORTB3 );//D11
SOFTPWM_DEFINE_CHANNEL( 8, DDRD, PORTD, PORTD2 );//D2
SOFTPWM_DEFINE_CHANNEL( 9, DDRB, PORTB, PORTB0 );//D8
SOFTPWM_DEFINE_CHANNEL( 10, DDRC, PORTC, PORTC0 );//A0-D14
SOFTPWM_DEFINE_CHANNEL( 11, DDRC, PORTC, PORTC1 );//A1-D15
SOFTPWM_DEFINE_CHANNEL( 12, DDRC, PORTC, PORTC2 );//A2-D16
SOFTPWM_DEFINE_CHANNEL( 13, DDRC, PORTC, PORTC3 );//A3-D17
SOFTPWM_DEFINE_CHANNEL( 14, DDRC, PORTC, PORTC4 );//A4-D18
SOFTPWM_DEFINE_CHANNEL( 15, DDRC, PORTC, PORTC5 );//A5-D19
 
SOFTPWM_DEFINE_OBJECT( 16 );
 /////////////////////////////////////////////////////////


void setup(){
 /////////////////////pin 2 a 17 en sorties////////////////////////////
  for(int i=2;i<18;i++)
  {
     pinMode(i, OUTPUT); 
  }
  
  digitalWrite(LED_BUILTIN,HIGH);

  Arp_arpdress_board();//prise en charge de l'arpdress board
  //Ã  commenter pour une adresse fixe


  Palatis::SoftPWM.begin(180);//NE FONCTIONNE PAS A 200// Ok 180
  
  ArduinoDmx0.attachRXInterrupt  (frame_received);
  ArduinoDmx0.set_control_pin(ArpDMXControl);       // Arduino output pin for MAX485 input/output control (connect to MAX485 pins 2-3) 
  ArduinoDmx0.set_rx_address(adress);              // dmx start address
  ArduinoDmx0.set_rx_channels(nbre_circuits);     // number of rx channels
  ArduinoDmx0.init_rx(DMX512);                   // starts universe 0 as rx
}

void loop()
{
  delay(500);//apres 500ms sans reception DMX
  digitalWrite(LED_BUILTIN,HIGH);//la led s'eteind 
} 


void frame_received(uint8_t universe) // cette boucle est execute a  chaque reception d'une trame DMX
{
  Arp_led_temoin ();//la led clignote si elle recoit un signal DMX    
  for(int i=0;i<nbre_circuits;i++)
  {
    Palatis::SoftPWM.set(i, ArduinoDmx0.RxBuffer[i]);  //buffers 0 indexed
  }
}
