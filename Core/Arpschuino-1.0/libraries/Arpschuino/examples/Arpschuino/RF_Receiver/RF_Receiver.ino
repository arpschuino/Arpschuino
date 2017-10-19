//pour ArpschuinoRF
//reçoit 2 arpsensorsRF avec 3 analogs et 3 digitales chacun (ID 1 et 2)
// puis revoie les données en DMX et écrit sur ses sorties
//recepteur 3= main gauche / antenne rose
//recepteur 2= main droite / antenne jaune
#include <SoftPWMMaster_mod.h>
//////////////////////////PATCH//////////////////////
SOFTPWM_DEFINE_CHANNEL( 0, DDRD, PORTD, PORTD3); //D3
SOFTPWM_DEFINE_CHANNEL( 1, DDRD, PORTD, PORTD5 );//D5
SOFTPWM_DEFINE_CHANNEL( 2, DDRD, PORTD, PORTD6 );//D6
SOFTPWM_DEFINE_CHANNEL( 3, DDRB, PORTB, PORTB5 );//D13
SOFTPWM_DEFINE_CHANNEL( 4, DDRB, PORTB, PORTB4 );//D12
SOFTPWM_DEFINE_CHANNEL( 5, DDRB, PORTB, PORTB1 );//D9
SOFTPWM_DEFINE_CHANNEL( 6, DDRB, PORTB, PORTB2 );//D10
SOFTPWM_DEFINE_CHANNEL( 7, DDRB, PORTB, PORTB3 );//D11
SOFTPWM_DEFINE_CHANNEL( 8, DDRD, PORTD, PORTD2 );//D2
SOFTPWM_DEFINE_CHANNEL( 9, DDRB, PORTB, PORTB0 );//D8
SOFTPWM_DEFINE_CHANNEL( 10, DDRC, PORTC, PORTC0 );//A0-D14
SOFTPWM_DEFINE_CHANNEL( 11, DDRC, PORTC, PORTC1 );//A1-D15
SOFTPWM_DEFINE_CHANNEL( 12, DDRC, PORTC, PORTC2 );//A2-D16
SOFTPWM_DEFINE_CHANNEL( 13, DDRC, PORTC, PORTC3 );//A3-D17
SOFTPWM_DEFINE_CHANNEL( 14, DDRC, PORTC, PORTC4 );//A4-D18
SOFTPWM_DEFINE_CHANNEL( 15, DDRC, PORTC, PORTC5 );//A5-D19
 ///////////////////////////////////////////////////////// 
SOFTPWM_DEFINE_OBJECT( 16 );
////////////////////////////reception (RF12)//////////////////////////
#include <ArpRFLib.h>

#define NUM_NODES 2 // Definit le nombre d'émeteurs ArpSensorRF
#define DATA_PER_NODE 6 
#define NETWORKID       77  //l'id du reseau commun pour toute les cartes
#define GATEWAYID     1 //l'id de la carte qui sert de gateway
#define ID GATEWAYID//  unique pour chaque carte
int freq = RF12_868MHZ; //la frequence de l'emeteur
//////////////////////////PATCH////////////////////////////////////////////
#define NBOUTPUT 12
byte output [NBOUTPUT] = {Arp0,Arp1,Arp2,Arp3,Arp4,Arp5,Arp6,Arp7,Arp8,Arp9,Arp10,Arp11};
#define bascule 127  //la valeur où passer de LOW à HIGH
//////////////////////////

#include <lib_dmx.h>
#define default_adress (1)//adresse DMX par defaut
#define nbre_circuits (DATA_PER_NODE*NUM_NODES)//
#define    DMX512     (0)  
int adress = default_adress;


typedef struct {
  byte node;
  int val[DATA_PER_NODE];
} Payload;

byte nextId;
MilliTimer timer;

int data [NUM_NODES*DATA_PER_NODE]={0};//={{0},{0}};

///////////////////////voyant reception////////////////////////////
const byte led_temoin= LED_BUILTIN ;//voyant
bool var_led =0;

unsigned long previousMillis = 0; 

void setup () {
  
  pinMode(led_temoin,OUTPUT);
  digitalWrite(led_temoin,HIGH);//eteind la led
  arpdress_board();//prise en charge de l'arpdress board
  ArduinoDmx0.set_control_pin(ArpDMXControl);    // Arduino output pin for MAX485 input/output control (connect to MAX485 pins 2-3) 
  ArduinoDmx0.set_tx_address(adress);      // dmx start address
  ArduinoDmx0.set_tx_channels(nbre_circuits);     // number of rx channels
  ArduinoDmx0.init_tx(DMX512);  
  SoftPWM.begin(160);
  rf12_initialize(payload.node, freq, NETWORKID);

}
////////////////ecrit les données reçues sur les sorties de l'arpschuino pour un fonctionnement standalone
void write_data_to_output()
{
  if((DATA_PER_NODE*NUM_NODES)>=NBOUTPUT)
  {
    for(int i=0; i< NBOUTPUT;i++)
    {
          SoftPWM.set(i,data[i]);  //buffers 0 indexed
    }
  }
  else
  {
    for(int i=0; i< DATA_PER_NODE*NUM_NODES;i++)
    {
          SoftPWM.set(i,data[i]);  //buffers 0 indexed
    }
  }
}
/////////////////////////////////////////////////////////////
void loop () {

  //Lit les Arpsensors et enregistre les données dans la variable 
  for(int i=0; i<NUM_NODES ;i++)///recceuil les 2 infos avant de les envoyer (on est passé à 2 pour l'instant)
  {
      // switch to next node
      if (++nextId > NUM_NODES)
        nextId = 1;
      // send an empty packet to one specific pollee
      rf12_sendNow(RF12_HDR_ACK | RF12_HDR_DST | nextId, 0, 0);
      // wait up to 10 milliseconds for a reply
      timer.set(10);//moins de 10????
      while (!timer.poll())
        if (rf12_recvDone() && rf12_crc == 0 && rf12_len == sizeof (Payload)) {
          voyant();
          // got a good ACK packet, print out its contents
          const Payload* p = (const Payload*) rf12_data;
          byte ID = (p->node)-1;
          //Serial.print(/*(word)*/ p->node);
          for(int i=0; i<DATA_PER_NODE ;i++)
          {
              data[i+((ID-2)*DATA_PER_NODE)]= p->val[i];       
          }
          break;
        }
  }

//////////////////////////////envois (données et dmx)////////////////////////////////////////
 write_data_to_output();
 memcpy((void *)ArduinoDmx0.TxBuffer, (void *)data, nbre_circuits);  //envoie sur le buffer dmx;     
}
//////////////////////////fin de la loop//////////////////////////////////////////////

/////////////////////////fonctions//////////////////////////////////////////////////

void voyant (){ 
      digitalWrite  (led_temoin, var_led); 
      var_led = !var_led;
}

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
    if(adress<513 && adress>0){}
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

