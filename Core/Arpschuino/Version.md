Arpschuino
=======
1.0.0 : First uploaded version with some bugs in example files

V 1.0.3
grada laps retiré
Jeenode_jeelink ajouté dans "variants"

library erpschuino, led_temoin() : n°pin remplacé par LED_BUILTIN (comptabilité avec les autre boards)
	+ 2eme fonction led_temoin() avec argument (pin) pour les cartes sans LED_BUILTIN
	led_temoin()retourne la valeur de var led
	Dans arpdress_board() :     TWBR=0;
    TWCR=0; remplacé par Wire.end();(compatibilité)
	la lib compile avec toute les cartes, led_temoin() compatible, arpdress_board() avec l'arpschuino uniquement

	exemples arpschuino : fonctions.h et .cpp supprimés (inclu dans la library). n°pin remplacé par LED_BUILTIN, bitWrite remplacé par digitalWrite pour plus de lisibilité et compatibilité
	exemple arpdress board netoyé
	exemples arpsensors, arpsensorsRF, wilulu, jeenode/jeelink corrigés, portage de la library