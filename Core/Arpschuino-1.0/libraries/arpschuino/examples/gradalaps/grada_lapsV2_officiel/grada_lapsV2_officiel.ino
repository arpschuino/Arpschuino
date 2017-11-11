
#include <lib_dmx.h>

 int myDipPins[9]={ArpDIP0,ArpDIP1,ArpDIP2,ArpDIP3,ArpDIP4,ArpDIP5,ArpDIP6,ArpDIP7,ArpDIP8}; 
 int address = 0;
 boolean mode = 0;
 int channels[6]={Arp0,Arp1,Arp2,Arp3,Arp4,Arp5};
 int led =0;
int nbcircuit = 6;
#define    DMX512     (0)  

void setup()
{
  pinMode(LED_BUILTIN,OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  //lecture du dixieme dip switch
  pinMode(ArpDIP9, INPUT_PULLUP);
  mode=digitalRead(ArpDIP9);
 
 //lecture des dip switchs :
  for(int i=0;i<9;i++)
  {
   pinMode(myDipPins[i], INPUT_PULLUP);       //Set DIP switch pins as inputs
   if(!digitalRead(myDipPins[i]))
    {
    address += 1<<(i);
    }
  }

int debordement = adress+nbcircuit-512;
if(debordement>0)
{
  nbcircuit = nbcircuit-debordement;
}
/////////////////////////////setUp DMX///////////////////////////////////////////////////////
ArduinoDmx0.attachRXInterrupt  (frame_received);
ArduinoDmx0.set_control_pin(ArpDMXControl);    // Arduino output pin for MAX485 input/output control (connect to MAX485 pins 2-3) 
ArduinoDmx0.set_rx_address(address);      // set rx0 dmx start address
ArduinoDmx0.set_rx_channels(nbcircuit);     // number of rx channels
ArduinoDmx0.init_rx(DMX512);        // starts universe 0 as rx, NEW Parameter DMX mode
}

void loop()
{
  delay(200);//la led s'éteind apres 200ms sans reception DMX
  digitalWrite  (LED_BUILTIN,  LOW);
  
} 

void frame_received(uint8_t universe) // Custom ISR: fired when all channels in one universe are received
{
  //led verte
  
    if (universe == 0) // USART0
  {
    if(led == 0)
    {
      digitalWrite  (LED_BUILTIN,  LOW); 
      led = 1;
    }
    else
    {
      digitalWrite  (LED_BUILTIN,  HIGH);
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

