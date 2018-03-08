/* 
officiel 05/03/18
code source pour arpschuino ecrit pour la spider board  www.arpschuino.fr/spider-board.php
12 bit pwm dimming, 1526Hz 16 chanels

official 05/03/18
source code for arpschuino written for the spider board  www.arpschuino.fr/spider-board_e.php
gradation pwm 12 bit, 1526Hz sur 16 circuits
*/

#include <Arpschuino.h>
#include <Wire.h>
#include <arpPCA9685.h>
#include <lib_dmx.h>
#define default_adress (1)//adresse DMX par defaut
#define    DMX512     (0) 
#include "courbe_led.h"

int nbre_circuits (32);
int adress = default_adress;

// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm1 = Adafruit_PWMServoDriver(0x40);
Adafruit_PWMServoDriver pwm2 = Adafruit_PWMServoDriver(0x41);

// you can also call it with a different address you want
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x41);

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  arpdress_board();//prise en charge de l'arpdress board
  //à commenter pour une adresse fixe
  pwm1.begin();
  pwm2.begin();
  pwm1.setPWMFreq(1600);  // This is the maximum PWM frequency
  pwm2.setPWMFreq(1600);  // This is the maximum PWM frequency
    
  // save I2C bitrate
  uint8_t twbrbackup = TWBR;
  // must be changed after calling Wire.begin() (inside pwm.begin())
  TWBR = 12; // upgrade to 400KHz!

  bitWrite (PORTD,4,1);// equivalent a digitalWrite(led_temoin,HIGH);

  ArduinoDmx0.attachRXInterrupt  (frame_received);
  ArduinoDmx0.set_control_pin(7);    // Arduino output pin for MAX485 input/output control (connect to MAX485 pins 2-3) 
  ArduinoDmx0.set_rx_address(adress);      // dmx start address
  ArduinoDmx0.set_rx_channels(nbre_circuits);     // number of rx channels
  ArduinoDmx0.init_rx(DMX512);        // starts universe 0 as rx
}

void loop() {
  
  //delay(500);//la led s'eteind apres 500ms sans reception DMX
  //bitWrite (PORTD,4,1);
 for(int i=0;i<16;i++)
 {
    //pwm.setPWM(i, 0,ArduinoDmx0.RxBuffer[i]<<4);//courbe droite
    pwm1.setPWM(i, 0, level_12B[ArduinoDmx0.RxBuffer[i]]);//courbe led
    pwm2.setPWM(i, 0, level_12B[ArduinoDmx0.RxBuffer[i+16]]);//courbe led
 }
}

void frame_received(uint8_t universe) // cette boucle est executé à chaque réception d'une trame DMX
{
  led_temoin ();//la led clignote si elle reçoit un signal DMX  
}
