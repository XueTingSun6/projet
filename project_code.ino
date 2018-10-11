#include <OneWire.h>
 
/*
 * Exemple d'application utilisant un DS1994 et la librairie OneWire
 * Hardware: 
 * GND arduino -> Coté "extérieur" = MASSE du DS1994
 * VCC 5v arduino -> résistance 4k7 -> D10 arduino
 * D10 arduino -> Coté "intérieur" du DS1994
 */
 
/* Création d'une instance de la classe OneWire */
OneWire ds(10); // Le DS1990 sera en mode "parasite" (utilise le bus de donnée comme alimentation) sur la broche D10
 
/* Variables 
 * i: itérateur pour les boucles
 * data: 5 octets de données venant du DS1994
 * addr: adresse trouvé sur le bus OneWire
 * timestamp: nombre de secondes écoulé
 */
byte i;
byte data[5];
byte addr[8];
unsigned long timestamp;
 
/* Setup() */
void setup(void) {
 
  /* Initialisation du port série */
  Serial.begin(9600);
  Serial.println("Lecture DS1990");
  pinMode(LED_BUILTIN, OUTPUT);
}

void PrintHex8(uint8_t *data, uint8_t length) // prints 8-bit data in hex with leading zeroes
{
     char tmp[16];
       for (int i=0; i<length; i++) {
         sprintf(tmp, "%.2X",data[i]);
         Serial.print(tmp); Serial.print(" ");
         digitalWrite(LED_BUILTIN, HIGH); 
         delay(50); 
         digitalWrite(LED_BUILTIN, LOW);
       }
}
 
/* loop() */
void loop(void) {
  
  /* Recherche des DSxx présent sur le bus OneWire */
  Serial.print("Recherche ... ");
  if (!ds.search(addr)) {
    Serial.println("Fin du scan");
    ds.reset_search();
    delay(500);
    return;
  }
 
  /* Affichage de la ROM du DSxx détecté */
  Serial.print("ROM : ");
  for(i = 0; i < 8; i++) {
    Serial.write(' ');
    //Serial.print(addr[i], HEX); //n'affiche pas les 0 du déburt en hexa, par ex 7 au lieu de 07
    PrintHex8(&addr[i],1); //marche avec les 0 au début !
  }
  Serial.println();

   Serial.print("Serial Number : ");
  for(i = 1; i < 7; i++) {
    Serial.write(' ');
    //Serial.print(addr[7-i], HEX);
    PrintHex8(&addr[7-i],1);
  }
  Serial.println();
  /* Vérification de la validité de la ROM reçu */
  if (OneWire::crc8(addr, 7) != addr[7]) {
    Serial.println("Erreur de CRC sur addr");
    return;
  }
 
  /* Vérification du type de DSxx */
  if(addr[0] != 0x01) {
    Serial.println("DSxx de type autre que DS1990");
    return;
  }  
 
  
}
 
