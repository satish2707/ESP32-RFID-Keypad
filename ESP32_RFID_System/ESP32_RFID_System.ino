/***********************************************************************************************************
 * @details  : This Project Contains Application of RFID using ESP32
 *
 * @author   : Satish Kanawade (Embedded Software developer)
 *
 * @copyright : Satish Kanawade. All rights reserved.
 *
 * @date     : 23 OCT 2024
 *
 * @version  : V.1.0
 **********************************************************************************************************/
#include "Arduino.h"
#include "RFID.h"
#include "stdint.h"
#include "ANALOG.h"
#include "SSD1306.h"
#include "ESP_EEPROM.h"
#include <Ticker.h>

extern const char AdminCardStr[];
extern Adafruit_SSD1306 display;
extern char uidString[12];
char* RFID_UID;

// Initializes serial, GPIO, OLED, EEPROM, RFID, and startup display.
void setup() {
  Serial.begin(115200);
  delay(10);

  FM_START_PRINT();
  delay(10);

  GPIO_INIT();
  delay(10);

  Display_init();
  delay(10);

  EEPROM_INIT();
  delay(10);

  RFID_INIT();
  delay(10);

  DISPLAY_LOGO();
  delay(1000);
}

// Runs RFID scan, admin setup, user recharge, and timed output workflows.
void loop() {
  static bool adminMode = false;
  static unsigned long lastScanTime = 0;
  static unsigned long userCardScanTime = 0;
  unsigned long currentMillis = millis();

  if (currentMillis - lastScanTime >= 500) {
    RFID_UID = SCAN_RFID();

    if (RFID_UID != NULL) {
      Serial.print("RFID UID: ");
      Serial.println(RFID_UID);

      if (compareUIDs(AdminCardStr, RFID_UID)) {
        Serial.println("Admin card detected");

        if (!adminMode) {
          adminMode = true;
          lastScanTime = currentMillis;
          userCardScanTime = currentMillis;

          display.clearDisplay();
          Display_TEXT("ADMIN SETTING", 1, 15);
          Display_TEXT("SCAN USER CARD", 1, 35);
          return;
        }
      } else if (adminMode && currentMillis - userCardScanTime < 10000) {
        Serial.println("User card detected for admin action");
        Serial.print("User Card UID: ");
        Serial.println(RFID_UID);
        strncpy(uidString, RFID_UID, sizeof(uidString) - 1);
        uidString[sizeof(uidString) - 1] = '\0';

        Serial.print("User UID stored: ");
        Serial.println(uidString);

        unsigned long menuStartTime = millis();
        while (millis() - menuStartTime < 30000) {
          Check_button();
          DISPLAY_MENU();
          delay(100);
        }
        SCAN_RFID_GIF();
        adminMode = false;
        return;
      } else {
        Serial.println("User card detected");
        DEDUCT_RECH_AMT(RFID_UID, 1);

        char formattedUID[9];
        int j = 0;
        for (int i = 0; i < strlen(RFID_UID); i++) {
          if (RFID_UID[i] != ' ') {
            formattedUID[j++] = RFID_UID[i];
          }
        }
        formattedUID[j] = '\0';

        uint8_t BALANCE_AMT = CHECK_USER_RECHARGE(formattedUID);
        if (BALANCE_AMT > 0) {
          START_WASH(1);
        }
      }
    }
    lastScanTime = currentMillis;
  }

  if (!adminMode) {
    SCAN_RFID_GIF();
  }
}
