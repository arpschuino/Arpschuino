/* 
officiel 30/11/18
Emmeteur/recepteur DMX>RF12 (radio)
Ecrit pour l'arptefact::transceiver http://arpschuino.fr/arptefact_tranceiver.php
Adressage et parametrage :  http://arpschuino.fr/pdf/setting-arptefact-transceiver.pdf

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Transceiver  DMX>RF12 (radio)
official 30/11/18
written for arptefact::transceiver www.arpschuino.fr/arptefact_transceiver_e.php
Adressage et parametrage :  http://arpschuino.fr/pdf/setting-arptefact-transceiver.pdf
*/

#include <EEPROM.h>
#include <Arpschuino.h> 

#include <lib_dmx.h>   // recepion dmx

int nbre_circuits (16);//
int address;

#include <ArpRFLib.h>
uint8_t NODEID = 1;  
#define NETWORKID  212  //adresse du reseau commune à toute les machine
uint8_t band = RF12_868MHZ; //plage de frequence l'emeteur RF12 : RF12_868MHZ europe, 
//RF12_915MHZ America 


byte buffer_to_send[66]={0};

unsigned long previousMillis = 0; 

bool mode=0;

uint16_t Frequency_offset_F868[21]={1200,1240,1280,1320,1360,1400,1440,1480,1520,1560,1600,1640,1680,1720,1760,1800,1840,1880,1920,1960,2000};
uint16_t Frequency_offset_F915[21]={280,400,520,640,760,880,1160,1400,1640,1880,2000,2040,2080,2120,2160,2200,2240,2280,2320,2360,2400};

 
void setup() {
  delay(5);// on attend que l'arpdress board se réveille !
  pinMode(LED_BUILTIN,OUTPUT);
  digitalWrite(LED_BUILTIN,HIGH);//eteind la led

  ///////////////////////////////////////////////////////

  //readRF12EEPROM();
  rf12_read_EEPROM_band();
  uint16_t frequency = eeprom_read_word((uint16_t*) (RF12_EEPROM_ADDR + 4));
  //uint16_t frequency = EEPROM.get(0x24,frequency);
  nbre_circuits=EEPROM.read(0x004);
  if(nbre_circuits<2||nbre_circuits>66) nbre_circuits=16;

  //anti debordement
  int debordement = address+nbre_circuits-513;//anti debordement debug 29/10/2017
  if(debordement>0)
  {
    nbre_circuits = nbre_circuits-debordement;
  }
  /////////////////////////////////////////////////////
  int val=Arp_arpdress_board();
  switch (val)
  {    
    case 513:
      mode=1;  
      break;
      
    case 868:
      band = RF12_868MHZ;
      EEPROM.update(0x20,(band<<6)+NODEID);
      frequency = 1600;
      EEPROM.put(0x24,frequency) ;
      break;  
        
    case 915:
      band = RF12_915MHZ;
      EEPROM.update(0x20,(band<<6)+NODEID);
      frequency=2000;
      EEPROM.put(0x24,frequency) ;    
      break;
      
    case 999:    // Reset eeprom
        Arp_default_EEPROM();
        rf12_default_EEPROM();
        address=1;  
        nbre_circuits=16;
        band==RF12_868MHZ;
        frequency = 1600;
        
        
      break;
      
    default:
      if(val>=740 && val <=760)
      {
        if(band==RF12_868MHZ)
        {
          frequency = Frequency_offset_F868[val-740];
        }
        else if(band==RF12_915MHZ)
        {
          frequency = Frequency_offset_F915[val-740];
        } 
        EEPROM.put(0x24,frequency) ;        
      }
      else if(val>=602 && val <=666)
      {
        nbre_circuits=val-600;
        EEPROM.put(0x004,nbre_circuits);      
      }
              
  }
///////////////////////////////////////////////////////////////////
  if(mode)//recepteur
  {
      ArduinoDmx0.set_control_pin(ArpDMXControl);    // Arduino output pin for MAX485 input/output control (connect to MAX485 pins 2-3) 
      ArduinoDmx0.set_tx_address(address);      // dmx start address
      ArduinoDmx0.set_tx_channels(512);     // number of tx channels
      ArduinoDmx0.init_tx(DMX512);

      NODEID = 2;
  }
  else//emmeteur
  {
      ArduinoDmx0.attachRXInterrupt  (frame_received);
      ArduinoDmx0.set_control_pin(ArpDMXControl);   // Arduino output pin for MAX485 input/output control (connect to MAX485-1 pins 2-3) 
      ArduinoDmx0.set_rx_address(address);    // set tx start address
      ArduinoDmx0.set_rx_channels(nbre_circuits); // 2 to 512 in standard mode) See lib_dmx.h 
      ArduinoDmx0.init_rx(DMX512);    
  }
  rf12_initialize(NODEID, band, NETWORKID,frequency);
}

void loop() {
    if(mode)//recepteur
    {
       if (rf12_recvDone() && rf12_crc == 0) 
       {
          Arp_led_temoin ();

          memcpy((void *)ArduinoDmx0.TxBuffer, (void *)rf12_data, rf12_len);//nbre_circuits);
          previousMillis = millis(); //nouveau
       }
       else
       {
          unsigned long currentMillis = millis();
         
          if(currentMillis - previousMillis >= 1000) {
            // une seconde sans reception
            previousMillis = currentMillis;   
            digitalWrite(LED_BUILTIN,  HIGH);      
           }
       } 
    }
   else
   {
      unsigned long currentMillis = millis();
     
      if(currentMillis - previousMillis >= 1000) {
        // une seconde sans reception
        previousMillis = currentMillis;   
        digitalWrite(LED_BUILTIN,  HIGH);      
       }
   }  
}

void frame_received(uint8_t universe) // Custom ISR: fired when all channels in one universe are received
{
  Arp_led_temoin ();  
  /////////////////
  memcpy((void *)buffer_to_send, (void *)ArduinoDmx0.RxBuffer, nbre_circuits);

  rf12_sendNow(0, &buffer_to_send,nbre_circuits);
  previousMillis = millis();
}
