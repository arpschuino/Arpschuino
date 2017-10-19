
/* 
officiel 05_05_16
ecrit pour l'arpschuino  www.arpschuino.fr
gradation soft PWM sur les 16 canneaux. 
Frequence PWM 160 hz
Adressage avec l'Arpdress board (avec I2CattinyDip_09_01_15)
pour que l'arpdress-board fonctionne, le port bas doit etre debranche.

Adressage avec l'Arpdress board :
2 clignotements brefs = l'adresse est prise depuis l'EEPROM.
1 cligotement jaune (arpdress board) et 1 vert =  l'adresse est prise depuis l'arpdress board et ÃƒÂ©crite dans l'EEPROM.
1 cligotement jaune (arpdress board) et 10 vert =  l'adresse est non conforme, l'adresse par defaut est prise.

Puis la led verte vibrillonne tant qu'elle recoit du DMX.

Addressing with arpdress board :
2 short flashes = the address is taken from the EEPROM.
1 yellow flash on the arpdress-board and 1 green flash = address is taken from the arpdress-board and written in EEPROM.
1 yellow flash on the arpdress-board and 10 green flashes =  the address is not compliant, the default address is taken.

Then the green led blink quiquely while the board receive DMX.

05_05_16 : fonctions dans un fichier separe
26_12_16 : debug, la variable adress doit être un int et non un byte pour des adresses supérieures à 255.
*/

#include "fonctions.h"

#include <lib_dmx.h>
#define default_adress (1)//adresse DMX par defaut
#define nbre_circuits (16)
#define    DMX512     (0) 

#define __DEBUG_SOFTPWM__ 0
#include <SoftPWMMaster_mod.h>

 
// byte adress;
int adress;//debug 26/12/16 pour les adresses au delas de 255...

//////////////////////////PATCH//////////////////////
SOFTPWM_DEFINE_CHANNEL( 0, DDRD, PORTD, PORTD3); //D3
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
 /////////////////////////////////////////////////////////
 
SOFTPWM_DEFINE_OBJECT( nbre_circuits );



void setup(){
 /////////////////////pin 2 à  17 en sorties////////////////////////////
  for(int i=2;i<18;i++)
    {
     pinMode(i, OUTPUT); 
    }
  
  bitWrite (PORTD,4,1);// equivalent a digitalWrite(led_temoin,HIGH);

  arpdress_board();//prise en charge de l'arpdress board
  //à commenter pour une adresse fixe
  
  SoftPWM.begin(160);//NE FONCTIONNE PAS A 200// Ok 180
  
  ArduinoDmx0.attachRXInterrupt  (frame_received);
  ArduinoDmx0.set_control_pin(ArpDMXControl);    // Arduino output pin for MAX485 input/output control (connect to MAX485 pins 2-3) 
  ArduinoDmx0.set_rx_address(adress);      // dmx start address
  ArduinoDmx0.set_rx_channels(nbre_circuits);     // number of rx channels
  ArduinoDmx0.init_rx(DMX512);        // starts universe 0 as rx
}

void loop()
{
  delay(500);//la led s'eteind apres 500ms sans reception DMX
  bitWrite (PORTD,4,1);
} 


void frame_received(uint8_t universe) // cette boucle est executé à chaque réception d'une trame DMX
{
 led_temoin ();//la led clignote si elle reçoit un signal DMX  
  
 for(int i=0;i<nbre_circuits;i++)
   {
    SoftPWM.set(i, ArduinoDmx0.RxBuffer[i]);  //buffers 0 indexed
   }
}





