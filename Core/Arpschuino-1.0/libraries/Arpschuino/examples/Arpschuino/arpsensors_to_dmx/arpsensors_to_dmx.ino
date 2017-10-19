//code permet d'envoyer en DMX les valeurs reçu par 3 arpsensors
//a uploader dans l'apschuino (ou arduino)
//test avec plusieurs arpsensors fonctionne avec 3 ;)
//reception de 6 analogs et de 3 digitales

//23/10/2015
//3 arp

#include <Wire.h>
#define nbr_arpsensors 3

int SLAVE_ADDR[nbr_arpsensors]={0x27,0X28,0X29}; //les adresses I2C des attinys
byte val_recues[7 * nbr_arpsensors];//les valeures recues
byte buffer_to_send[9]={0};  //les valeurs a afficher

#include <lib_dmx.h>
#define default_adress (1)//adresse DMX par defaut
#define nbre_circuits (9 * nbr_arpsensors)
#define    DMX512     (0) 
int adress = default_adress;

void setup(){
arpdress_board();
ArduinoDmx0.set_control_pin(ArpDMXControl);    // Arduino output pin for MAX485 input/output control (connect to MAX485 pins 2-3) 
ArduinoDmx0.set_tx_address(adress);      // dmx start address
ArduinoDmx0.set_tx_channels(nbre_circuits);     // number of rx channels
ArduinoDmx0.init_tx(DMX512);  
  
Wire.begin();  
}



void loop(){
  for(byte j=0;j<nbr_arpsensors;j++){
  
  //////envoie requete/////// 
      Wire.beginTransmission(SLAVE_ADDR[j]);      // déclare l'adresse de l'esclave qui va recevoir le code
      Wire.write(0xDE);                        // envoie la donnée
      Wire.endTransmission();                  // arrête la transmission 
      
      //////////on attend 6 bytes//////////////////////////////////////////
      Wire.requestFrom(SLAVE_ADDR[j], 7);     // request 7 bytes from slave, 6 valeures digitales + 1 octet pour les digitales
      if (Wire.available()) {
          for(int i=0;i<7;i++){
            val_recues[i]=Wire.read();//ofset
            buffer_to_send[i]=val_recues[i];
          }
      }
      
      decodeByte();

       // copy 200 channels from rx buffer 2 to tx buffer 3, position 200  (patch from 1-200 to 201-400)
      memcpy((void *)&ArduinoDmx0.TxBuffer[j*9], (void *)buffer_to_send, 9);    
  }                           
}

void decodeByte(){
  for(int i=8;i>5;i--){
    buffer_to_send[i]= val_recues[6] & B00000001;//sélectionne le bit 0 uniquement
    val_recues[6] = val_recues[6] >> 1;//on decale d'un bit pourlire la valeure suivante
  }
}
