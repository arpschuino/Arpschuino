///////////////////////////////// R F 12 /////////////////////////
#include <ArpRFLib.h>
#define ID 1//  unique pour chaque carte 2 droite, 3 pour gauche,etc...
#define NETWORKID       212  //l'id du reseau commun pour toute les cartes
#define GATEWAYID     1 //l'id de la carte qui sert de gateway
int freq = RF12_868MHZ; //la frequence de l'emeteur
ISR(WDT_vect) { Sleepy::watchdogEvent(); }

///////////////////////potentiometre///////////////////////////////
#define nbr_analog 3
#define NBAVERAGE 5
const byte analogIn [nbr_analog] ={Arp0,Arp1,Arp2};
int sensVal[nbr_analog]={0};  //les valeurs analogiques a renvoyer
/////////////////////boutton///////////////////////////////////
#define nbr_dig 3
const byte digitalPin [nbr_dig]={Arp3,Arp4,Arp5};
bool butVal[nbr_dig]={false};  //les valeurs analogiques a renvoyer

bool change = true;
#define ACK_TIME 100


byte payload[nbr_analog+nbr_dig]={0};
MilliTimer sendTimer;
static byte waitForAck() {
  MilliTimer ackTimer;
  while (!ackTimer.poll(ACK_TIME)){
    if (rf12_recvDone() && rf12_crc == 0 && ((rf12_hdr & RF12_HDR_ACK) == 0) && ((rf12_hdr & RF12_HDR_CTL) == 128) ){
      return 1;
    }
  }
  return 0;
}
void   initPin()
{
  for(int i =0;i<nbr_analog;i++)
  {
    pinMode(analogIn[i],INPUT);
  }
  for(int i=0;i<nbr_dig;i++)
  {
    pinMode(digitalPin[i],INPUT_PULLUP);
  }
}
void read_sensor()
{
for(int i=0;i<nbr_analog;i++)
{
  int val = 0;
  for(int j=0;j<NBAVERAGE;j++)
  {
   val += analogRead(analogIn[i]);
  }
  val = val/NBAVERAGE;
  if(sensVal[i]!=val)
  {
   change = true;
   sensVal[i]=val;
   payload[i]=sensVal[i]/4;
  }
}
for(int i=0;i<nbr_dig;i++)
{
  int val = digitalRead(digitalPin[i]);
  if(butVal[i]!=val)
  {
   change = true;
   butVal[i]=val;
   if(val==HIGH)
   {
       payload[i+nbr_analog]=0;
   }
   else
   {
       payload[i+nbr_analog]=255;
   }
  }
}
}


void setup() {
  initPin();
  delay(300);
  rf12_initialize(ID, freq, NETWORKID);
  rf12_sleep(RF12_SLEEP);
}

void loop() {

  read_sensor();
  if(change)
  {    
      rf12_sleep(RF12_WAKEUP);

      rf12_recvDone();
  if (rf12_canSend() && sendTimer.poll(100)) {
    rf12_sendStart(RF12_HDR_ACK, &payload, sizeof payload);
    rf12_sendWait(2);
    if (waitForAck()){
      change=false;
    }
    }
            rf12_sleep(RF12_SLEEP);
  }
}
