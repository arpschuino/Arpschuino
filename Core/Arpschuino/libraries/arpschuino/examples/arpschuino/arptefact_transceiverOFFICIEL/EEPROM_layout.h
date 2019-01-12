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

    // 0x01E fine frequency             nnnn nnnn (least significant bit)
    // 0x01F fine frequency             nnnn nnnn (most significant bit)
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

void default_EEPROM() 
{
  EEPROM.update(0x0,0);//centaines
  EEPROM.update(0x1,0);//disaines
  EEPROM.update(0x2,1);//unites
  EEPROM.update(0x3,0);//mode
  EEPROM.update(0x20,B10000001);//ID + band
  EEPROM.update(0x21,212);//groupe
  EEPROM.put(0x24,1600);//frequency
}
*/
