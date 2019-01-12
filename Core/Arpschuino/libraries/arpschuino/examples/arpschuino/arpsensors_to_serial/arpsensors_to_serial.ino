//code de test, permet de monitorer les valeurs envoyées par l'arpsensors
//a uploader dans l'apschuino (ou arduino)
//test avec plusieurs arpsensors fonctionne avec 3 ;)
//reception de 6 analogs et de 3 digitales

// test code, allows to monitor the values sent by the arpsensors
// to upload in the apschuino (or arduino)
// test with multiple arpsensors works with 3;)
// receive 6 analogs and 3 digitals

//23/10/2015
//3 arp

#include <Wire.h>

#define nbr_arpsensors 3

int SLAVE_ADDR[nbr_arpsensors]={0x27,0X28,0X29}; //les adresses I2C des attinys
byte val_recues[7 * nbr_arpsensors];//les valeures recues
byte buffer_to_send[9 * nbr_arpsensors]={0};  //les valeurs a afficher


void setup(){
  Serial.begin(19200);  //baudrate
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
      
      for(int i=0;i<9;i++){
        Serial.print (buffer_to_send[i]);
        Serial.print (" ; ");          
      }
      
      Serial.print(":");
      
  }                
  Serial.println();
           
}

void decodeByte(){
  for(int i=8;i>5;i--){
    buffer_to_send[i]= val_recues[6] & B00000001;//sélectionne le bit 0 uniquement
    val_recues[6] = val_recues[6] >> 1;//on decale d'un bit pourlire la valeure suivante
  }
}
