//www.elegoo.com
//2016.12.09

/*
 * --------------------------------------------------------------------------------------------------------------------
 * Example to change UID of changeable MIFARE card.
 * --------------------------------------------------------------------------------------------------------------------
 * This is a MFRC522 library example; for further details and other examples see: https://github.com/miguelbalboa/rfid
 * 
 * This sample shows how to set the UID on a UID changeable MIFARE card.
 * NOTE: for more informations read the README.rst
 * 
 * @author Tom Clement
 * @license Released into the public domain.
 *
 * Typical pin layout used:
 * -----------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
 *             Reader/PCD   Uno           Mega      Nano v3    Leonardo/Micro   Pro Micro
 * Signal      Pin          Pin           Pin       Pin        Pin              Pin
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
 * SPI SS      SDA(SS)      10            53        D10        10               10
 * SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
 * SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
 * SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
 */

#include <SPI.h>
#include <MFRC522.h>
#include "pitches.h"

#define RST_PIN   (9)     // Configurable, see typical pin layout above
#define SS_PIN    (10)    // Configurable, see typical pin layout above

#define LED_ROUGE (4)
#define LED_VERTE (3)

/*byte carte_maman[]={0x1B,0x8B,0xFF,0xCB};
byte badge_bleu[]={0xCE,0x46,0xD8,0x73};
byte carte_paul[]={0xD0,0x32,0x03,0x25};
byte carte_old_cb[]={0x02,0x04,0x19,0x0D};
byte carte_bernard[]={0xFA,0x62,0x2C,0xAA};
byte carte_christelle[]={0xCA,0x32,0xFF,0xA9};*/

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance

/* Set your new UID here! */
//#define NEW_UID {0xDE, 0xAD, 0xBE, 0xAF}

MFRC522::MIFARE_Key key;


void setup() 
{
  pinMode(LED_ROUGE, OUTPUT);
  pinMode(LED_VERTE, OUTPUT);
  digitalWrite(LED_ROUGE,LOW);
  digitalWrite(LED_VERTE,LOW);

  Serial.begin(9600);  // Initialize serial communications with the PC
  while (!Serial);     // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)

  SPI.begin();         // Init SPI bus
  mfrc522.PCD_Init();  // Init MFRC522 card
  Serial.println(F("Attente d'un badge..."));

 digitalWrite(LED_ROUGE,LOW);
}

void loop() 
{  
  String sCmd=Serial.readStringUntil('\n');
  if (sCmd=="ledvon")
  {
   digitalWrite(LED_VERTE,HIGH);
   Serial.println("ok");
  }
  else if (sCmd=="ledvoff")
  {
   digitalWrite(LED_VERTE,LOW);
   Serial.println("ok");
  }
  else if (sCmd=="ledron")
  {
   digitalWrite(LED_ROUGE,HIGH);
   Serial.println("ok");
  }
  else if (sCmd=="ledroff")
  {
   digitalWrite(LED_ROUGE,LOW);
   Serial.println("ok");
  }
  
  if ( ! mfrc522.PICC_IsNewCardPresent() || ! mfrc522.PICC_ReadCardSerial() ) 
  {
    delay(50);
    return;
  }
  
  //Serial.print(F("Card UID:"));
  String sNum="carte=";
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
    sNum+=String(mfrc522.uid.uidByte[i],HEX);
  } 
  sNum+="\n";
  Serial.println(sNum);

  for (int i=0;i<5;i++)
  {
   digitalWrite(LED_VERTE,LOW);
   delay(100);
   digitalWrite(LED_VERTE,HIGH);
   delay(50);
  }

  while (mfrc522.PICC_IsNewCardPresent())
    delay(50); 
}
