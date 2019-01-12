#ifndef Arpschuino_h
#define Arpschuino_h

#define default_adress (1)//adresse par defaut
/////////////////:compatibilite avec les versions 1.0.x////////
#define led_temoin() Arp_led_temoin ()
#define arpdress_board() Arp_arpdress_board()
#define adress address
///////////////////////////////////////////////////////
bool Arp_led_temoin ();//change l'etat (allume ou eteind) de la led verte de l'arpschuino
bool Arp_led_temoin (int pin);

int Arp_arpdress_board();//prend l'adresse depuis l'arpdress board, l'ecrit dans l'EEPROM. Retourne l'adresse lue.
bool Arp_write_dmx_address();//ecrit l'adresse dans l'eeprom//retourne 0 si l'adresse n'est pas conforme, 1 sinon
int Arp_read_dmx_address();//lit l'eeprom, retourne l'adresse DMX
void Arp_default_EEPROM() ;//met les valeurs par defaut dans l'eeprom

#endif

/*
Eeprom Layout

byte 0x000 DMX address centaines      **** *nnn  1 - 5
byte 0x001 DMX address disaines       **** nnnn  1 - 9
byte 0x002 DMX address unites         **** nnnn  1 - 9

byte 0x003 Mode                       **** ***n  0 - 1 (0=emmeteur,1=recepteur)) 
byte 0x004 Nombre de circuits         nnnn nnnn  0 - 255 
byte 0x005 Nombre de circuits         **** ***n  0 - 1 
ou byte 0x003             ???         n*** **** 
byte 0x01D Undefined

---------------------------------------------------------------------------------------
byte 0x020 Node number in bits        ***n nnnn  1 - 30
           Reserved                   **0* ****  Reserved 
           Band to use                00** ****  Don't use, unsupported 325 Mhz band
            "                         01** ****  433 MHZ  0x40
            "                         10** ****  868 MHZ  0x80
            "                         11** ****  915 MHZ  0xC0
byte 0x021 Group number               1101 0100  // 212 0xD4
byte 0x022 Eeprom format              MMMM mmmm  Major/minor version
---------------------------------------------------------------------------------------
byte 0x023 Hex output style           01** ****  1: Hex output, 2: hex+ascii
           Collect Mode               **1* ****  Don't send acks
           Quiet mode                 ***1 ****  Don't report bad packets
           Reserved                   **** 0000  Reserved  
---------------------------------------------------------------------------------------
word 0x024 Frequency offset LSB       llll llll  Frequency offset word
 "   0x025 Frequency offset MSB       mmmm mmmm
     0x026 Padded at the end with NUL 0000 0000
     0x02D Padded at the end with NUL 0000 0000
word 0x02E CRC                                   CRC of values w/ offset 0x020-02F
 "   0x02F  as above
byte 0x030 Reserved
     0x03F  as above
byte 0x040 Key storage for encryption algorithm
     0x04F  as above
byte 0x050 Reserved
     0x400  as above
---------------------------------------------------------------------------------------

/*
	arpdress board layout :

000         ---------------------
001-512     DMX start address
513         RFM12 receiver mode
514-601     ---------------------
602-666     NOMBRE DE CIRCUITS n-600 Circuits
667-739     ---------------------
740-760     frequency fine
761-867     ---------------------
868         RF12_868MHZ frequency range
869-914     ---------------------
915         RF12_915MHZ frequency range
916-998     ---------------------
999         reload default
*/