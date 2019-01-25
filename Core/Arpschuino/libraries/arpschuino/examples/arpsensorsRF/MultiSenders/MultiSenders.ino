// Sketch pour chaque Arpsensor lors d'un configuration multi-emeteurs, 
// A partir du Sketch de karistouf pour les gants de control d'Haraka
// Pour éviter de surcharger la fréquence d'emission les ArpSensorsRF ne renovie leur valeurs que à la solicitation du recepteur.
// 1 recepeteur (arpschuino+RF faisant office de gateway)
// Charger le sketch RF_Receiver dans le recepteur

/* Configuration des capteurs
arpsensorsRF 0 >> A0, D0, Aref ......accelero X
             1 >> A1, D1        ......accelero Y
             2 >> A2, D2        ......accelero z
             3 >> A3, D3        ......capa 1
             4 >> A7, D7        ......capa 2
             5 >>   D10          ......capa 3

*/
///////////////////////accelero///////////////////////////////
#define nbr_analog 3
const byte analogIn [nbr_analog] ={Arp0,Arp1,Arp2};
int sensVal[nbr_analog]={0};  //les valeurs analogiques a renvoyer

/////////////////////capa///////////////////////////////////
#define nbr_dig 3
const byte capacitivePin [nbr_dig]={Arp3,Arp4,Arp5};
///////////////////////////////////////////////////////////
//////////////////////////////////////
void read_sensor()
{
  capa ();
  accelero ();
}
///////////////////////////////// R F 12 /////////////////////////
#include <ArpRFLib.h>
#define ID 2//  unique pour chaque carte 2 droite, 3 pour gauche,etc...
#define NETWORKID (212)  //l'id du reseau commun pour toute les cartes
#define NODEID (1) //l'id de la carte qui sert de gateway
int band = RF12_868MHZ; //la frequence de l'emeteur
//RF12_915MHZ America  // RF12_433MHZ

float frequency_setting = 868.00; //beetween 863,00 > 870,00
//float frequency_setting = 915.00 ;//902;1–918 MHz (America, China and eastern Pacific Islands)

typedef struct {
  byte node;
  int val[nbr_analog + nbr_dig];
} Payload;

Payload payload;

void setup () {

  payload.node=ID;
  delay(300);
  rf12_initialize(NODEID, band, NETWORKID,rf12_calcul_freq(frequency_setting));
}

void loop () {
     read_sensor();
  if (rf12_recvDone() && rf12_crc == 0 && rf12_len == 0 && RF12_WANTS_ACK) {

    // start transmission
    rf12_sendStart(RF12_ACK_REPLY, &payload, sizeof payload);
  }
}

void accelero ()
{
  for(byte i=0;i<nbr_analog;i++){         //lecture des analogs 
    payload.val[nbr_dig + i]=analogRead(analogIn[i]);//ecriture des analogs
  }  
}

void capa ()
{   
    for(byte i=0; i<nbr_dig ;i++)
    {
        int val = readCapacitivePin(capacitivePin [i]);
        if(val>1){
          payload.val[i] =127;
        }
        else{
          payload.val[i] =32; 
        }       
    }
}

// readCapacitivePin
//  Input: Arduino pin number
//  Output: A number, from 0 to 17 expressing
//  how much capacitance is on the pin
//  When you touch the pin, or whatever you have
//  attached to it, the number will get higher
uint8_t readCapacitivePin(int pinToMeasure) {
  // Variables used to translate from Arduino to AVR pin naming
  volatile uint8_t* port;
  volatile uint8_t* ddr;
  volatile uint8_t* pin;
  // Here we translate the input pin number from
  //  Arduino pin number to the AVR PORT, PIN, DDR,
  //  and which bit of those registers we care about.
  byte bitmask;
  port = portOutputRegister(digitalPinToPort(pinToMeasure));
  ddr = portModeRegister(digitalPinToPort(pinToMeasure));
  bitmask = digitalPinToBitMask(pinToMeasure);
  pin = portInputRegister(digitalPinToPort(pinToMeasure));
  // Discharge the pin first by setting it low and output
  *port &= ~(bitmask);
  *ddr  |= bitmask;
  delay(1);
  uint8_t SREG_old = SREG; //back up the AVR Status Register
  // Prevent the timer IRQ from disturbing our measurement
  noInterrupts();
  // Make the pin an input with the internal pull-up on
  *ddr &= ~(bitmask);
  *port |= bitmask;

  // Now see how long the pin to get pulled up. This manual unrolling of the loop
  // decreases the number of hardware cycles between each read of the pin,
  // thus increasing sensitivity.
  uint8_t cycles = 3;//8 a la place de 17
  if (*pin & bitmask) { cycles =  0;}
  else if (*pin & bitmask) { cycles =  1;}
  else if (*pin & bitmask) { cycles =  2;}
//  else if (*pin & bitmask) { cycles =  3;}
//  else if (*pin & bitmask) { cycles =  4;}
//  else if (*pin & bitmask) { cycles =  5;}
//  else if (*pin & bitmask) { cycles =  6;}
//  else if (*pin & bitmask) { cycles =  7;}
//  else if (*pin & bitmask) { cycles =  8;}
//  else if (*pin & bitmask) { cycles =  9;}
//  else if (*pin & bitmask) { cycles = 10;}
//  else if (*pin & bitmask) { cycles = 11;}
//  else if (*pin & bitmask) { cycles = 12;}
//  else if (*pin & bitmask) { cycles = 13;}
//  else if (*pin & bitmask) { cycles = 14;}
//  else if (*pin & bitmask) { cycles = 15;}
//  else if (*pin & bitmask) { cycles = 16;}

  // End of timing-critical section; turn interrupts back on if they were on before, or leave them off if they were off before
  SREG = SREG_old;

  // Discharge the pin again by setting it low and output
  //  It's important to leave the pins low if you want to 
  //  be able to touch more than 1 sensor at a time - if
  //  the sensor is left pulled high, when you touch
  //  two sensors, your body will transfer the charge between
  //  sensors.
  *port &= ~(bitmask);
  *ddr  |= bitmask;

  return cycles;
}
