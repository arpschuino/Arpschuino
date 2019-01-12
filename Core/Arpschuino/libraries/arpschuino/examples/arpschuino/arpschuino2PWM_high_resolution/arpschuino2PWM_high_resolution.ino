/* 
officiel 08/01/19
ecrit pour l'arpschuino  www.arpschuino.fr
un code pour 2 output en pwm sorties 9 et 10 (Arp5 et 6)
resolution variable de 8 à 16 bit (editez la ligne: byte RESOLUTION = 12;)
Adressage avec l'Arpdress board (avec I2CattinyDip_09_01_15)
pour que l'arpdress-board fonctionne, le port bas doit être débranché.

Adressage avec l'Arpdress board :
2 clignotements brefs = l'adresse est prise depuis l'EEPROM.
  Si l'arpdress board est branché cela peu vouloir dire que le port bas n'est pas débranché.
1 cligotement jaune (arpdress board) et 1 vert =  l'adresse est prise depuis l'arpdress board et écrite dans l'EEPROM.
1 cligotement jaune (arpdress board) et 10 vert =  l'adresse est non conforme, l'adresse par defaut est prise.

Puis la led verte vibrillonne tant qu'elle reçoit du DMX.

official 08/01/19
written for arpschuino www.arpschuino.fr/index_e
a source code for 2 output in pwm out 9 and 10 (Arp5 et 6)
variable resolution from 8 to 16 bit (edit line: byte RESOLUTION = 12;)
Addressing with Arpdress board (with I2CattinyDip_09_01_15)
for the arpdress-board to work, the low port must be unplugged.

Addressing with arpdress board :
2 short flashes = the address is taken from the EEPROM.
1 yellow flash on the arpdress-board and 1 green flash = address is taken from the arpdress-board and written in EEPROM.
1 yellow flash on the arpdress-board and 10 green flashes =  the address is not compliant, the last address written in the EEPROM is taken.

Then the green led blink quiquely while the board receive DMX.

*/

#include <Arpschuino.h>

#include <lib_dmx.h>
int address;
int nbre_circuits (2);///////////////////

#include "courbe.h"

//////////////////////////PATCH////////////////////////////////////////////
byte output [2] = {Arp5,Arp6};
//////////////////////////////////////////////////////////////////////////
//choisisez la résolution - choose the resolution :
byte RESOLUTION = 12;//3906Hz (very good)
//byte RESOLUTION = 13;//1953Hz (good)
//byte RESOLUTION = 14;//976Hz (ok)
//byte RESOLUTION = 16;//244Hz (bad: fliker !)

void setup()
{ 
  pinMode(LED_BUILTIN, OUTPUT);  //met la led verte (temoin) en output
  digitalWrite(LED_BUILTIN,HIGH);//comprtement inverse

  Arp_arpdress_board();//prise en charge de l'arpdress board
  //à commenter pour une adresse fixe

//////////////////////////////////////////////////////////////////:
  ArduinoDmx0.attachRXInterrupt  (frame_received);
  ArduinoDmx0.set_control_pin(ArpDMXControl);    // Arduino output pin for MAX485 input/output control (connect to MAX485 pins 2-3) 
  ArduinoDmx0.set_rx_address(address);      // dmx start address
  ArduinoDmx0.set_rx_channels(nbre_circuits);     // number of rx channels
  ArduinoDmx0.init_rx(DMX512);        // starts universe 0 as rx, NEW Parameter DMX mode

  setupPWM(RESOLUTION);  
}

void loop() 
{
  delay(500);//la led s'éteind apres 500ms sans reception DMX
  digitalWrite(LED_BUILTIN,HIGH);  //la led s'éteind
} 

void frame_received(uint8_t universe) // cette boucle est executé à chaque réception d'une trame DMX
{
  Arp_led_temoin ();//la led clignote si elle reçoit un signal DMX  
   
  ///////////////lecture et report des niveau DMX//////////////////////////////////
  for(int i=0;i<nbre_circuits;i++)
  {
    //analogWrite16(output[i],level_16B [ArduinoDmx0.RxBuffer[i]]);
    switch(RESOLUTION) {
//      case 16:analogWrite16(output[i],level_16B [ArduinoDmx0.RxBuffer[i]]);break;
//      case 15:analogWrite16(output[i],(level_16B [ArduinoDmx0.RxBuffer[i]])>>1);break;
//      case 14:analogWrite16(output[i],level_14B [ArduinoDmx0.RxBuffer[i]]);break;
      case 13:analogWrite16(output[i],level_13B [ArduinoDmx0.RxBuffer[i]]);break;
      case 12:analogWrite16(output[i],level_12B [ArduinoDmx0.RxBuffer[i]]);break;
//      case 11:analogWrite16(output[i],(level_12B [ArduinoDmx0.RxBuffer[i]])>>1);break;
//      case 10:analogWrite16(output[i],(level_12B [ArduinoDmx0.RxBuffer[i]])>>2);break;
//      case  9:analogWrite16(output[i],(level_12B [ArduinoDmx0.RxBuffer[i]])>>3);break;
//      case  8:analogWrite16(output[i],ArduinoDmx0.RxBuffer[i]);break;
      default:analogWrite16(output[i],ArduinoDmx0.RxBuffer[i]);break;
    }
    delay(10);
  } 
}

void setupPWM(int resolution) {
  uint16_t icr;
  switch(resolution) {
    case 16:icr=0xffff;break;
    case 15:icr=0x7fff;break;
    case 14:icr=0x3fff;break;
    case 13:icr=0x1fff;break;
    case 12:icr=0x0fff;break;
    case 11:icr=0x07ff;break;
    case 10:icr=0x03ff;break;
    case  9:icr=0x01ff;break;
    case  8:icr=0x00ff;break;
    default:icr=0x00ff;break;
  }
/*Changing the prescaler will effect the frequency of the PWM signal.
Frequency[Hz}=CPU/(ICR1+1) where in this case CPU=16 MHz
16-bit PWM will be>>> 16000000/(65535+1)=244Hz
14-bit PWM : 976Hz ----- 12-bit PWM : 3906Hz 
*/    
  DDRB  |= _BV(PB1) | _BV(PB2);       /* set pins as outputs */
  TCCR1A = _BV(COM1A1) | _BV(COM1B1)  /* non-inverting PWM */
        | _BV(WGM11);                 /* mode 14: fast PWM, TOP=ICR1 */
  TCCR1B = _BV(WGM13) | _BV(WGM12)
        | _BV(CS10);                  /* prescaler 1 */
  ICR1 = icr;                         /* TOP counter value (freeing OCR1A*/
}

/* 16-bit version of analogWrite(). Works only on pins 9 and 10. */
void analogWrite16(uint8_t pin, uint16_t val)
{
  switch (pin) {
    case  9: OCR1A = val; break;
    case 10: OCR1B = val; break;
  }
} 

