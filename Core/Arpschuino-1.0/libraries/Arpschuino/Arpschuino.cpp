
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

#include <Wire.h>
#define ARPDRESS_BOARD 0x26

#include <EEPROM.h>
int EEPROMaddr = 0;
//__________________________________________________________ 
//                                                          |
//                       led_temoin                         |
//__________________________________________________________|

bool var_led=0;

void led_temoin ()
{
  bitWrite (PORTD,4,var_led);
  var_led = var_led-1;
}

//__________________________________________________________
//                                                          |
//             adressage avec l'arpdress board              |
//__________________________________________________________|
 
extern int adress;//debug 26/12/16 pour les adresses au delas de 255...
extern int nbre_circuits;

void arpdress_board(){
///////////////presence de l'arpdress-board ? /////////////////////////
  boolean arpdress = true;//variable pour indiquer la presence (ou non) de l'arpdress board 
  if ((digitalRead (18) == HIGH) && (digitalRead (19) == HIGH)){
 
    char arpdress_code[8] = {'a','r','p','d','r','e','s','s'};
    
  //  PORTC = PORTC | 00110000; //pullup sur sda et slc
    Wire.begin(); 
    Wire.beginTransmission(ARPDRESS_BOARD);      // declare l'adresse de l'esclave qui va recevoir le code
    Wire.write(0xB);                        // envoie la donnee
    Wire.endTransmission();                  // arrete la transmission
    
  //  digitalWrite(led_temoin,LOW);
    delay (500);
  //  digitalWrite(led_temoin,HIGH);
    
    Wire.requestFrom(ARPDRESS_BOARD, 8);     // request 8 bytes from slave
    
    if (Wire.available()) {
      char reponse[8] = {0};
      delay(200);
      for(int i=0; i<8; i++){
        reponse[i] = Wire.read();
      }
      delay(200);
      for(int i=0;i<8;i++) {
         if (arpdress_code[i]!=reponse[i]) arpdress = false;
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
    byte unite = 0;
    byte disaine = 0;
    byte centaine = 0;
    
    if (arpdress == true){//si l'arpdress-board est branche et le port bas debranche
  /////////////////////////I2C///////////////////////////////////////////
  //////envoie requete///////
      //Wire.begin(); 
      Wire.beginTransmission(ARPDRESS_BOARD);      // declare l'adresse de l'esclave qui va recevoir le code
      Wire.write(0xDE);                        // envoie la donnee
      Wire.endTransmission();                  // arrete la transmission
      delay(500);
      
      //////////on attend 3 byte//////////////////////////////////////////
  
      Wire.requestFrom(ARPDRESS_BOARD, 3);     // request 3 bytes from slave
       
      if (Wire.available()) {
    
         unite = Wire.read();
         disaine = Wire.read();
         centaine = Wire.read();
         
      //////////////////////////////EEPROM/////////////////////////////////
         EEPROM.write(EEPROMaddr, centaine);
         EEPROMaddr ++;
         EEPROM.write(EEPROMaddr, disaine);
         EEPROMaddr ++;
         EEPROM.write(EEPROMaddr, unite);
         bitWrite (PORTD,4,0);
         delay (500);
         bitWrite (PORTD,4,1);
      }
    }
    else{
       centaine = EEPROM.read(EEPROMaddr);
       EEPROMaddr ++;
       disaine = EEPROM.read(EEPROMaddr);
       EEPROMaddr ++;
       unite = EEPROM.read(EEPROMaddr);

       bitWrite (PORTD,4,0);     //  digitalWrite(led_temoin,LOW);
       delay (100);
       bitWrite (PORTD,4,1);
       delay (100);
       bitWrite (PORTD,4,0);
       delay (100);
       bitWrite (PORTD,4,1);
       
    }
    adress =(centaine*100)+(disaine*10)+(unite);
    if(adress<513 && adress>0){
		int debordement = adress+nbre_circuits-513;//anti debordement debug 29/10/2017
		if(debordement>0)
		{
			nbre_circuits = nbre_circuits-debordement;
		}
	}
    else{
       adress=default_adress;
       for (int i=0;i<10;i++){
          bitWrite (PORTD,4,0);
          delay (500);
          bitWrite (PORTD,4,1);
          delay (500);
       } 
    }
   
    delay(1000);
    
    TWBR=0;
    TWCR=0;//desactive l'I2C

    pinMode(18, OUTPUT);
    pinMode(19, OUTPUT);
	


}