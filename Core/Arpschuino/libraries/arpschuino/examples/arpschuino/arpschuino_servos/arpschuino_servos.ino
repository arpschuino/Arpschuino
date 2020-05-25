/* 
officiel 22/05/20
ecrit pour l'arpschuino  http://arpschuino.fr
un code pour 16 servo-moteurs
Adressage avec l'arpdress board 
pour que l'arpdress-board fonctionne, le port bas doit être débranché.

Adressage avec l'Arpdress board :
2 clignotements brefs = l'adresse est prise depuis l'EEPROM.
  Si l'arpdress board est branché cela peu vouloir dire que le port bas n'est pas débranché.
1 cligotement jaune (arpdress board) et 1 vert =  l'adresse est prise depuis l'arpdress board et écrite dans l'EEPROM.
1 cligotement jaune (arpdress board) et 10 vert =  l'adresse est non conforme, la derniere adresse ecrite dans l'EEPROM est prise.

Puis la led verte vibrillonne tant qu'elle reçoit du DMX.

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
official 22/05/20
written for arpschuino  http://arpschuino.fr/index_e.php
16 servos
Adressing with arpdress board 
for the arpdress-board to work, the low port must be unplugged

Addressing with arpdress board :
2 short flashes = the address is taken from the EEPROM.
1 yellow flash on the arpdress-board and 1 green flash = address is taken from the arpdress-board and written in EEPROM.
1 yellow flash on the arpdress-board and 10 green flashes =  the address is not compliant, the last address written in the EEPROM is taken.

Then the green led blink quiquely while the board receive DMX.

*/
#include<Arpschuino.h>

#include <Servo.h>
Servo servo_0;  // create servo object to control a servo
Servo servo_1;
Servo servo_2;
Servo servo_3;
Servo servo_4;
Servo servo_5;
Servo servo_6;
Servo servo_7;

//Servo servo_8;
//Servo servo_9;
//Servo servo_10;
//Servo servo_11;
//Servo servo_12;
//Servo servo_13;
//Servo servo_14;
//Servo servo_15;

#include <lib_dmx.h>
int address;
int nbre_circuits (16);


void setup(){

  pinMode(LED_BUILTIN, OUTPUT);  //met la led verte (temoin) en output
  digitalWrite(LED_BUILTIN,HIGH); //comprtement inverse
  
  Arp_arpdress_board();//prise en charge de l'arpdress board
  //à commenter pour une adresse fixe


  servo_0.attach(Arp0);
  servo_1.attach(Arp1);
  servo_2.attach(Arp2);
  servo_3.attach(Arp3); 
  servo_4.attach(Arp4);
  servo_5.attach(Arp5);
  servo_6.attach(Arp6);
  servo_7.attach(Arp7);  

//  servo_8.attach(Arp8);
//  servo_9.attach(Arp9);
//  servo_10.attach(Arp10);
//  servo_11.attach(Arp11); 
//  servo_12.attach(Arp12);
//  servo_13.attach(Arp13);
//  servo_14.attach(Arp14);
//  servo_15.attach(Arp15);   

///////////////////////////////////////////////////////////////////////
  ArduinoDmx0.attachRXInterrupt  (frame_received);
  ArduinoDmx0.set_control_pin(ArpDMXControl);    // Arduino output pin for MAX485 input/output control (connect to MAX485 pins 2-3) 
  ArduinoDmx0.set_rx_address(address);      // dmx start address
  ArduinoDmx0.set_rx_channels(nbre_circuits);     // number of rx channels
  ArduinoDmx0.init_rx(DMX512);        // starts universe 0 as rx
}

void loop()
{
  delay(500);// apres 500ms sans reception DMX
  digitalWrite(LED_BUILTIN,HIGH);//la led s'éteind
} 

void frame_received(uint8_t universe) // cette boucle est executé à chaque réception d'une trame DMX
{
   Arp_led_temoin ();//la led clignote si elle reçoit un signal DMX  
   

    // scale it to use it with the servo (value between 0 and 160)
    //you adjust it for your own servo
    servo_0.write(map(ArduinoDmx0.RxBuffer[0], 0, 255, 0, 160));
    servo_1.write(map(ArduinoDmx0.RxBuffer[1], 0, 255, 0, 160));
    servo_2.write(map(ArduinoDmx0.RxBuffer[2], 0, 255, 0, 160));  
    servo_3.write(map(ArduinoDmx0.RxBuffer[3], 0, 255, 0, 160));       
    servo_4.write(map(ArduinoDmx0.RxBuffer[4], 0, 255, 0, 160));
    servo_5.write(map(ArduinoDmx0.RxBuffer[5], 0, 255, 0, 160));  
    servo_6.write(map(ArduinoDmx0.RxBuffer[6], 0, 255, 0, 160));  
    servo_7.write(map(ArduinoDmx0.RxBuffer[7], 0, 255, 0, 160));

//    servo_8.write(map(ArduinoDmx0.RxBuffer[8], 0, 255, 0, 160));          
//    servo_9.write(map(ArduinoDmx0.RxBuffer[9], 0, 255, 0, 160));
//    servo_10.write(map(ArduinoDmx0.RxBuffer[10], 0, 255, 0, 160));  
//    servo_11.write(map(ArduinoDmx0.RxBuffer[11], 0, 255, 0, 160));       
//    servo_12.write(map(ArduinoDmx0.RxBuffer[12], 0, 255, 0, 160));
//    servo_13.write(map(ArduinoDmx0.RxBuffer[13], 0, 255, 0, 160));  
//    servo_14.write(map(ArduinoDmx0.RxBuffer[14], 0, 255, 0, 160));  
//    servo_15.write(map(ArduinoDmx0.RxBuffer[15], 0, 255, 0, 160));

}
