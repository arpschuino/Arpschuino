﻿Version en cours de developpement :

	Exemple arpsensorsRF : arpsensorsRFsend5analog : debug, ajout d'une initialisation de Arp5 en output

	Suppression de arpschuino_16_soft_PWM doublon précédament remplacé par arpschuino_16_softPWM (compatible avec la nouvelle version de softPWM)
	
	Suppression de notes-de-versions.txt (en doublon avec Version.md)


1.0.7 : 22/02/19

    Debug des exemples rf12 : 

        uint8_t band remplacé par uint16_t band (risque de débordement)

    Exemple spider board :

        Wire.setClock(400000); commenté par défaut (risque de conflit avec d'autres periphériques I²C)


1.0.6 25/01/18

	Exemples wilulu :		Afin de preserver les accus li-ion, la wilulu se met en veille si la tension d'alimentation descend sous 2.9v

	Debug des exemples rf12 :	calcul_freq 	remplacé par:	 rf12_calcul_freq

	Exemple spider board :		uint8_t twbrbackup = TWBR; supprimé car inutile
					TWBR = 12;  remplacé par	Wire.setClock(400000);   plus lisible!

	Exemples arpschuino : changements de noms :		
					Pour une meilleure intelligibilité :
					arpsensors_to_dmx	devient :	arpschuino_arpsensors_to_dmx
					arpsensors_to_serial	devient :	arpschuino_arpsensors_to_serial



1.0.5 : 12/01/19 

	libary arpschuino :

		nouveau nom : 		adress = address
					led_temoin() = Arp_led_temoin ()
					arpdress_board() = Arp_arpdress_board()
					(les anciens noms restent valides)
					
		nouvelles fonctions :	Arp_arpdress_board();//prend l'adresse depuis l'arpdress board, l'ecrit dans l'EEPROM. Retourne l'adresse lue.
					Arp_write_dmx_address();//ecrit l'adresse dans l'eeprom//retourne 0 si l'adresse n'est pas conforme, 1 sinon
					Arp_read_dmx_address();//lit l'eeprom, retourne l'adresse DMX
					Arp_default_EEPROM() ;//met les valeurs par defaut dans l'eeprom
					
		nouveau :		Dans la fonction Arp_arpdress_board(), l'eeprom n'est reécrite qu'en cas de changement d'adresse.
					En cas d'adresse non conforme, l'adresse n'est pas changé (précédemment, adresse 001 en cas d'adresse non conforme)
					la fonction retourne l'adresse lue.
					Changement de féquence possible dans tout les exemples avec RF12.

		nouveaux exemples :	aprschuino2PWM_high_resolution : 2 output en pwm sorties 9 et 10 (Arp5 et 6), resolution variable de 8 à 16 bit 
					artefact_transceiver : Verifier l'adresse en mode recepteur

		exemple retiré :  	one_emeter_with_Ack

		arpdress board layout :	mise en place d'une convention pour l'utilisation des adresses autres que 1>512
					
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

	libary ArpRF12 :	
						
		nouvelles fonctions :	rf12_default_EEPROM(); remet les valeurs par defaut (ID + band/groupe/frequency	)
					rf12_read_EEPROM_band(); lit la bande de frequence ecrite dans l'EEPROM	
					rF12_read_EEPROM_frequency(); lit la frequence ecrite dans l'EEPROM
					rF12_calcul_freq (float freq_set); transforme une frequence ecrite avec un nombre à 2 chiffre apres la virgule
					en valeur utilisable par la librairie.
										
		Eeprom Layout :		mise en place d'emplacements conventionnels pour les données stockées dans l'EEPROM 

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
						
						
1.0.4 : 08/03/18

    arpdress board retiré des exemples

    ajout de la library arpPCA9685

    ajout wilulu low power dans les exemples

    ajout optiboot_atmega328.hex (pour éviter un warning à la compilation).

    et empty84at8.hex		


1.0.3 : 03/12/17

    Debug exemples fonctions.h et .cpp supprimés (inclus dans la library).

    Modif du fichier board.txt pour que la variante jeenode_jeelink soit reconnue.

    Modif aussi de ArpRF12.cpp pour un bon fonctionnement avec les différentes cartes.


1.0.2 :

    grada laps retiré

    Jeenode_jeelink ajouté dans "variants"


    library erpschuino, led_temoin() : n°pin remplacé par LED_BUILTIN (comptabilité avec les autre boards)

    + 2eme fonction led_temoin() avec argument (pin) pour les cartes sans LED_BUILTIN

    led_temoin()retourne la valeur de var led

    Dans arpdress_board() : TWBR=0;

    TWCR=0; remplacé par Wire.end();(compatibilité)

    la lib compile avec toute les cartes, led_temoin() compatible, arpdress_board() avec l'arpschuino uniquement


    exemples arpschuino : fonctions.h et .cpp supprimés (inclu dans la library). n°pin remplacé par LED_BUILTIN, bitWrite remplacé par digitalWrite pour plus de lisibilité et compatibilité

    exemple arpdress board nettoyé

    exemples arpsensors, arpsensorsRF, wilulu, jeenode/jeelink corrigés, portage de la library

				
						

1.0.1 : première version fonctionelle 11/11/17 
	
				