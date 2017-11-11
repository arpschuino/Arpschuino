//code écris pour l'arpsensors: http://www.arpschuino.fr/arpsensors.php
//1 ou plusieurs arpsensors (testé avec 4), esclave de l'arpschuino (fonctionne avec test_master012)
//renvoie 7 bit, 6 valeures analogiques (0>255), un octet contenant les 3 digitales (0/1)

//necessite la library TinyWire : https://github.com/rambo/TinyWire/tree/master/TinyWireS
//test avec 6 capteurs
//fonctionne avec arpsensors_to_dmx/ino, arpsensors_to_serial01
//
//23/10/2015


//une source d'inspiration : http://kasey.fr/ATTiny-capteur-universel-en-I2C.html
//ATtiny 44 ou 84

#include "TinyWireS.h"                  // on inclus la lib I2C Slave pour ATTiny

#define I2C_SLAVE_ADDR  0x27         // adresse I2C differente de l'arpdress board (0x26)
 

byte byteRcvd;

//PWMOUT Arp4, Arp5, ARP6 DIGITALONLY ARP7,ARP8
byte analog[6]={A0,A1,A2,A3,A4,A5};
//byte analog[6]={Arp0,Arp1,Arp2,Arp3,Arp4,Arp5};//les 6 entrée analog

unsigned int sensVal[6]={0};  //les valeurs analogiques a renvoyer

byte digitales =B00000000;//l'octet contenant les digitales, B00000111 par example pour les 3 digitales au niveau haut


void setup(){
 
  DDRA  = B00000000;//port A en input, PB7(slc) et PB5'sda)aussi
  //PORTA = B01011101;//pas de pullUp sur le port B
   
  DDRB  = B00000000;//port D en input
  PORTB = B00001111;//PB3 (reset)pullUp, PB0,1,2 digitales in pullup
  
  TinyWireS.begin(I2C_SLAVE_ADDR);      // on rejoint le bus avec une adresse d'esclave (similaire a la lib wire)
  byteRcvd = 0;
}

void loop(){
 
  for(int i=0;i<6;i++){         //lecture des analogs
      unsigned int sum=0;
      unsigned int temp_val =0;
      for (int j = 0;j < 16;j ++)//On lit 16 fois la valeur
      {
	temp_val = analogRead(analog[i]); 
	sum = sum +temp_val;
      }
     temp_val = sum>>4;//la moyenne des 16 samples
      
      sensVal[i]=byte(temp_val>>2);//decale la valeur de 2 bits pour avoir une valeur 8 bits
                                   //peut être remplacé par un calibrage

  }
  
  digitales = PINB ^ B11111111; //lecture des digitales + inversion
  

  if (TinyWireS.available()){           // si on voit quelque chose sur le bus I2C

    for(int i=0;i<6;i++){  //envoie des analogs
      TinyWireS.send(sensVal[i]);
      //delayMicroseconds(10);
    }
    TinyWireS.send(digitales);//envois des digitales sous la forme d'un seul octet

  }
}

