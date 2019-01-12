
#include "Arpschuino.h"
#include <avr/io.h>
#include <util/crc16.h>
#include <avr/eeprom.h>
#include <avr/sleep.h>
#if ARDUINO >= 100
#include <Arduino.h> // Arduino 1.0
#else
#include <WProgram.h> // Arduino 0022
#endif

#define ARPDRESS_BOARD 0x26

#include <EEPROM.h>
int EEPROMaddr = 0;

#if defined(ARPSCHUINO) || defined(JEENODE_JEELINK)
#define voyant     LED_BUILTIN
#include <Wire.h>   

#elif defined(WILULU)
#define voyant     LED_BUILTIN     
  
#elif defined (ARPSENSORS) || defined(ARPSENSORSRF)
#define voyant     0

#define led_temoin() Arp_led_temoin ()

#endif
//__________________________________________________________ 
//                                                          |
//                       led_temoin                         |
//__________________________________________________________|

bool var_led=0;

bool Arp_led_temoin (int pin)
{
  digitalWrite (pin,var_led);
  var_led = var_led-1;
  return (var_led);
}

bool Arp_led_temoin ()
{
  digitalWrite (voyant,var_led);
  var_led = var_led-1;
  return (var_led);
}
  #if defined(ARPSCHUINO)

//__________________________________________________________
//                                                          |
//             adressage avec l'arpdress board              |
//__________________________________________________________|

//29/06/18 delay racourcis
//lecture prealable de l'eeprom
//adresse 1 si l'eeprom est vierge
 
extern int adress;//debug 26/12/16 pour les adresses au dela de 255...
extern int nbre_circuits;

int Arp_arpdress_board(){

  int address_read=0;
  byte unite = 0;
  byte disaine = 0;
  byte centaine = 0;

  ////////////////lecture préalable de l'eeprom//////////////
 
  int previous_address =Arp_read_dmx_address(); 

///////////////presence de l'arpdress-board ? /////////////////////////
  boolean arpdress = true;//variable pour indiquer la presence (ou non) de l'arpdress board 
  if ((digitalRead (18) == HIGH) && (digitalRead (19) == HIGH)){

  char arpdress_code[8] = {'a','r','p','d','r','e','s','s'};
  
  Wire.begin(); 
  Wire.beginTransmission(ARPDRESS_BOARD);      // declare l'adresse de l'esclave qui va recevoir le code
  Wire.write(0xB);                        // envoie la donnee
  Wire.endTransmission();                  // arrete la transmission
  

  //delay (500);
  delay (250);//divisé par 2, a tester
  
  Wire.requestFrom(ARPDRESS_BOARD, 8);     // request 8 bytes from slave
  
  if (Wire.available()) 
  {     
      char reponse[8] = {0};
      delay(200);
      for(int i=0; i<8; i++){
        reponse[i] = Wire.read();
      delay(10);
      }
      delay(100);
      for(int i=0;i<8;i++) {
         if (arpdress_code[i]!=reponse[i])
         {
          arpdress = false;
         }

      }
   }    
    else {
      arpdress = false;
    }
  } 
  else {
    arpdress = false;
  }
   /////////////////////////////////////////////////////////////////////////// 
    if (arpdress == true){//si l'arpdress-board est branche et le port bas debranche
  /////////////////////////I2C///////////////////////////////////////////
  //////envoie requete///////

      Wire.beginTransmission(ARPDRESS_BOARD);      // declare l'adresse de l'esclave qui va recevoir le code
      Wire.write(0xDE);                        // envoie la donnee
      Wire.endTransmission();                  // arrete la transmission
      //delay(500);
      delay (250);//divisé par 2, a tester(reduire encore ?)
         
      //////////Lecture de l'arpdress board, on attend 3 byte//////////////////////////////////////////
  
      Wire.requestFrom(ARPDRESS_BOARD, 3);     // request 3 bytes from slave
                              delay(200);
      if (Wire.available()) {
    
         byte unite_read = Wire.read();
                                                           
         byte disaine_read = Wire.read();
                                                                 
         byte centaine_read = Wire.read();
                                      
         address_read =(centaine_read*100)+(disaine_read*10)+(unite_read);
       
         adress = address_read;
      }
      else
      {
        adress = previous_address;
      }
      
      if (Arp_write_dmx_address())
      {
        digitalWrite(LED_BUILTIN,LOW);
        delay (500);
        digitalWrite(LED_BUILTIN,HIGH);       
      }
      else{
         adress=previous_address;
         for (int i=0;i<10;i++){
            bitWrite (PORTD,4,0);
            delay (250);//divisé par 2, a tester
            bitWrite (PORTD,4,1);
            delay (250);//divisé par 2, a tester
         } 
      }      
    }
////////////////////si l'apdress board n'est pas présente/////////// 
    else
    {
       adress = previous_address;
       bitWrite (PORTD,4,0);     //  digitalWrite(led_temoin,LOW);LED_BUILTIN?
       delay (100);
       bitWrite (PORTD,4,1);
       delay (100);
       bitWrite (PORTD,4,0);
       delay (100);
       bitWrite (PORTD,4,1);
    }          
///////////////////////////////////////////////////
//////////////////////////////////////////////////
////////////////////////////////////////////

    int debordement = adress+nbre_circuits-513;//anti debordement debug 29/10/2017
    if(debordement>0)
    {
      nbre_circuits = nbre_circuits-debordement;
    }

    delay(500);//divisé par 2, a tester
    
    Wire.end(); 
  
    pinMode(18, OUTPUT);
    pinMode(19, OUTPUT);

    return (address_read); 
}


//__________________________________________________________
//                                                          |
//             ecriture de l'adresse                        |
//__________________________________________________________|


bool Arp_write_dmx_address()
{
  bool  test = 0;
  if(address<513 && address>0){
   uint8_t cent = address/100;
   uint8_t dis = (address%100)/10;
   uint8_t unit = address%10;   
//////////////////////////////EEPROM/////////////////////////////////
   EEPROM.write(EEPROMaddr, cent);       //centaines
   EEPROM.write(EEPROMaddr+1, dis); //disaines
   EEPROM.write(EEPROMaddr+2, unit);      //unites
   test = 1;
  }
  else test = 0;
  return test;
}

//__________________________________________________________
//                                                          |
//             lecture de l'adresse                         |
//__________________________________________________________|

int Arp_read_dmx_address()
{
   bool test = 0;
   uint8_t cent = EEPROM.read(EEPROMaddr);
   if(cent>5) test=1;
   uint8_t dis = EEPROM.read(EEPROMaddr+1);
   if(dis>9) test=1;
   uint8_t unit = EEPROM.read(EEPROMaddr+2);
   if(unit>9) test=1;
   uint16_t readed_address =(cent*100)+(dis*10)+(unit);
   if(readed_address==0||readed_address>512||test==1) readed_address=1;
   return readed_address;
}
 
 //__________________________________________________________
//                                                          |
//             ecriture des valeurs par defaut              |
//__________________________________________________________|
 
void Arp_default_EEPROM() 
{
  EEPROM.update(0x0,0);//centaines
  EEPROM.update(0x1,0);//disaines
  EEPROM.update(0x2,1);//unites
  EEPROM.update(0x3,0);//mode
  EEPROM.put(0x4,16);//nombre de circuit
}
  #endif