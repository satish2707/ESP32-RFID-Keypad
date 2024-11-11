/***********************************************************************************************************
 * @details  : This Project Contains Application of RFID using ESP32
 *
 * @author   : Satish Kanawade (Embedded Software developer)
 *
 * @copyright: Omnitrix Technologies PVT LTD
 *
 * @date     : 23 OCT 2024
 *
 * @version  : V.1.0
 **********************************************************************************************************/
#include "Arduino.h"
#include "RFID.h"
#include "ANALOG.h"
#include "SSD1306.h"
#include "ESP_EEPROM.h"
#include <SPI.h>
#include <MFRC522.h>

MFRC522 rfid(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;

char uidString[12] = "";

// This function Initlize the RFID Module
void RFID_INIT() {
  SPI.begin();
  rfid.PCD_Init();
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
  Serial.println("MFRC522 RFID initialized successfully");
}


// This function Compair two RFID card
bool compareUIDs(const char* uid1, const char* uid2) {
  char normalizedUID1[12];
  char normalizedUID2[12];

  int j = 0;
  for (int i = 0; uid1[i] != '\0'; i++) {
    if (uid1[i] != ' ') {
      normalizedUID1[j++] = uid1[i];
    }
  }
  normalizedUID1[j] = '\0';

  j = 0;
  for (int i = 0; uid2[i] != '\0'; i++) {
    if (uid2[i] != ' ') {
      normalizedUID2[j++] = uid2[i];
    }
  }
  normalizedUID2[j] = '\0';

  return strcmp(normalizedUID1, normalizedUID2) == 0;
}


// This function scans the RFID card and returns the UID dynamically
char* SCAN_RFID() {
  // Check if there's a new card present
  if (!rfid.PICC_IsNewCardPresent()) {
    return nullptr;
  }

  // Read the card serial (UID)
  if (!rfid.PICC_ReadCardSerial()) {
    return nullptr;
  }

  char* uidString = (char*)malloc(rfid.uid.size * 3 + 1);  

  if (uidString == nullptr) {
    Serial.println("Memory allocation failed!");
    return nullptr;
  }

  int index = 0;
  for (byte i = 0; i < rfid.uid.size; i++) {
    sprintf(&uidString[index], "%02X ", rfid.uid.uidByte[i]);
    index += 3; 
  }

  uidString[index - 1] = '\0';

  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();

  BUZZER_ALERT(1);

  return uidString;
}
