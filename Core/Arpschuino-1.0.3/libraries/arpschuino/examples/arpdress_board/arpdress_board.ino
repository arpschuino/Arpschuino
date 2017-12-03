//arpdress board , adressage de l'arpschuino http://www.arpschuino.fr

//http://kasey.fr/ATTiny-capteur-universel-en-I2C.html
/*ATtiny2313

1-|   |-8
c-|(|)|
4-|   |-2  encodeur rotatif

http://www.fibidi.com/arduino-programming-attiny2313/attiny2313-pinout/
*/
#include "TinyWireS.h"                  // on inclus la lib I2C Slave pour ATTiny

#define I2C_SLAVE_ADDR  0x26            // adresse I2C

byte byteRcvd;

byte dip1[4]={11,13,12,15};//les 4 sorties de la roue unité
byte dip10[4]={4,0,5,1};//les 4 sorties de la roue dizaine
byte dip100[4]={8,6,9,7};//les 4 sorties de la roue centaine


void setup(){
  //attention vérifier comment initialiser les pin slc et sda
  DDRB  = B00000010;//port B en input, PB1(10)en output (led debug), 
  PORTB = B01011101;//pullUp, PB7(slc) et PB5'sda) input mais pas pull up
   
  DDRD  = B00000000;//port D en input
  PORTD = B11111111;//pullUp
  
  TinyWireS.begin(I2C_SLAVE_ADDR);      // on rejoint le bus avec une adresse d'esclave (similaire a la lib wire)
  byteRcvd = 0;
}

void loop(){
   char unite=B00000000;
   for(int i=0;i<4;i++){         //lecture des dip switchs
     if(!digitalRead(dip1[i]))
     {
      unite += 1<<(i);
     }
  }
   char disaine=0;
   for(int i=0;i<4;i++){         //lecture des dip switchs
     if(!digitalRead(dip10[i]))
     {
      disaine += 1<<(i);
     }
  }
   char centaine=0;
   for(int i=0;i<4;i++){         //lecture des dip switchs
     if(!digitalRead(dip100[i]))
     {
      centaine += 1<<(i);
     }
  }
  
  if (TinyWireS.available()){           // si on voit quelque chose sur le bus I2C
    byteRcvd = TinyWireS.receive();     // on l'enregistre
    if (byteRcvd = 0xB){
      char arpdress_code[8] = {'a','r','p','d','r','e','s','s'};
      for(int i=0; i<8; i++){
        TinyWireS.send(arpdress_code[i]);//envoie du code
      } 
    
    }
    
    if (byteRcvd = 0xDE){

      TinyWireS.send(unite);
      TinyWireS.send(disaine);
      TinyWireS.send(centaine);
      Blink();                    // on blink un coup pour montrer que l'adresse est envoye
    }

  }
}

void Blink() {                  // 
 //digitalWrite(led,HIGH);   //optimise avec bitWrite
 bitWrite (PORTB,1,1);
 delay(200);
 //digitalWrite(led,LOW);
 bitWrite (PORTB,1,0);
 delay(200);
}
