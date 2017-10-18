
#include <lib_dmx.h>

 int myDipPins[9]={8,12,13,14,15,16,17,18,19}; 
 int address = 0;
 int dip10 = 7;
 boolean mode = 0;
 int channels[6]={3,5,6,9,10,11};
 int led_temoin=4;
 int led =0;

#define    DMX512     (0)  





void setup()
{
  pinMode(led_temoin,OUTPUT);
  digitalWrite(led_temoin, HIGH);

  //lecture du dixieme dip switch
  pinMode(dip10, INPUT_PULLUP);
  mode=digitalRead(dip10);
 
 //lecture des dip switchs :
  for(int i=0;i<9;i++)
  {
   pinMode(myDipPins[i], INPUT_PULLUP);       //Set DIP switch pins as inputs
   if(!digitalRead(myDipPins[i]))
    {
    address += 1<<(i);
    }
  }
/////////////////////////////setUp DMX///////////////////////////////////////////////////////
ArduinoDmx0.attachRXInterrupt  (frame_received);
ArduinoDmx0.set_control_pin(2);    // Arduino output pin for MAX485 input/output control (connect to MAX485 pins 2-3) 
ArduinoDmx0.set_rx_address(address);      // set rx0 dmx start address
ArduinoDmx0.set_rx_channels(6);     // number of rx channels
ArduinoDmx0.init_rx(DMX512);        // starts universe 0 as rx, NEW Parameter DMX mode
}

void loop()
{
  delay(200);//la led s'éteind apres 200ms sans reception DMX
  digitalWrite  (led_temoin,  LOW);
  
} 

void frame_received(uint8_t universe) // Custom ISR: fired when all channels in one universe are received
{
  //led verte
  
    if (universe == 0) // USART0
  {
    if(led == 0)
    {
      digitalWrite  (led_temoin,  LOW); 
      led = 1;
    }
    else
    {
      digitalWrite  (led_temoin,  HIGH);
      led = 0;
    }
  }
  
  switch (mode){

    case 0:
  //tous les circuits à full
       for(int i=0;i<6;i++)
        { 
        analogWrite(channels[i],255);
        }
      break;
      
    case 1:
  //write values from dmx channels 1-6 universe 0 to arduino pwm pins
      for(int i=0;i<6;i++)
        {
        analogWrite(channels[i], ArduinoDmx0.RxBuffer[i]);
        }
      break;
      
  }
}  // end of ISR

