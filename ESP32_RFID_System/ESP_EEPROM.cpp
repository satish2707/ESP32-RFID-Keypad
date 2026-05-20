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
#include <stdint.h>
#include <sys/types.h>
#include "Arduino.h"
#include "RFID.h"
#include "ANALOG.h"
#include "SSD1306.h"
#include <EEPROM.h>
#include "ESP_EEPROM.h"

extern Adafruit_SSD1306 display;

// User Sturcture for EEPROM
struct User {
  char uid[UID_LENGTH + 1];
  uint32_t recharge;
};

// This function initilize the ESP32 EEPROM
void EEPROM_INIT() {
  if (EEPROM.begin(EEPROM_SIZE)) {
    Serial.println("EEPROM Initialize Successfully");
  } else {
    Serial.println("EEPROM Initialize Failed");
  }
}

// This function clear EEPROM OF ESP32
void clearEEPROM() {
  for (int i = 0; i < EEPROM_SIZE; i++) {
    EEPROM.write(i, 0);
  }
  EEPROM.commit();
  Serial.println("EEPROM cleared");
}

// This function adds a new user to EEPROM
bool ADD_USER_RFID(const char* uid) {
  Serial.print("UID Length in ADD_USER_RFID: ");
  Serial.println(strlen(uid));
  if (strlen(uid) != UID_LENGTH) {
    Serial.println("UID length mismatch");
    return false;
  }
  for (int i = 0; i < MAX_USERS; i++) {
    User user;
    EEPROM.get(i * sizeof(User), user);
    if (strcmp(user.uid, uid) == 0) {
      Serial.println("Duplicate user. Not able to add this user");
      ERROR_GIF();
      Display_TEXT("DUPLICATE USER", 1, 28);
      delay(2000);
      return false;
    }
  }
  for (int i = 0; i < MAX_USERS; i++) {
    User user;
    EEPROM.get(i * sizeof(User), user);
    if (strcmp(user.uid, "") == 0) {
      strncpy(user.uid, uid, UID_LENGTH);
      user.uid[UID_LENGTH] = '\0';
      EEPROM.put(i * sizeof(User), user);
      EEPROM.commit();  // Save the changes to EEPROM
      Serial.print("User added: ");
      Serial.println(user.uid);
      ADD_USER_GIF();
      Display_TEXT("USER ADD SUCCESSFUL", 1, 15);
      Display_TEXT(user.uid, 1, 35);
      delay(2000);
      return true;
    }
  }
  Serial.println("User list is full");
  return false;
}

// This function delete user from EEPROM
bool DELETE_USER_RFID(const char* uid) {
  int index = FIND_USER(uid);
  if (index != -1) {
    User user;
    EEPROM.get(index * sizeof(User), user);
    strcpy(user.uid, "");
    user.recharge = 0;
    EEPROM.put(index * sizeof(User), user);
    EEPROM.commit();
    Serial.println("User deleted successfully");
    DELETE_USER_GIF();
    Display_TEXT("USER DELETE", 1, 15);
    Display_TEXT("SUCCESSFULLY", 1, 35);
    delay(2000);
    return true;
  }
  Serial.println("User not found");
  ERROR_GIF();
  Display_TEXT("USER NOT FOUND", 1, 28);
  delay(2000);
  return false;
}

// This function Check recharge of User and scan from EEPROM
uint32_t CHECK_USER_RECHARGE(const char* uid) {
  int index = FIND_USER(uid);
  int USER_EX = 0;
  if (index != -1) {
    User user;
    EEPROM.get(index * sizeof(User), user);
    if (user.recharge == 0) {
      Serial.println("User recharge expired");
      Display_TEXT("USER RECHARGE EXPIRED", 1, 28);
      delay(2000);
      USER_EX = 1;
    } else {
      CHECK_USER_RECH_GIF();
      Display_TEXT("USER RECHARGE AMOUNT", 1, 15);
      char buffer[10];
      sprintf(buffer, "%u", user.recharge);
      Display_TEXT(buffer, 2, 35);
      delay(2000);
      return user.recharge;
    }
  }
  if (USER_EX == 0) {
    Serial.println("User not found");
    ERROR_GIF();
    Display_TEXT("USER NOT FOUND", 1, 28);
    delay(2000);
    USER_EX = 0;
    return 0;
  }
  return 0;
}

// This function add recharge to User and store in EEPROM
bool ADD_USER_RECHARGE_RFID(const char* uid, uint32_t amount) {
  int index = FIND_USER(uid);
  if (index != -1) {
    User user;
    EEPROM.get(index * sizeof(User), user);
    user.recharge += (amount);
    EEPROM.put(index * sizeof(User), user);
    EEPROM.commit();
    ADD_USER_RECH_GIF();
    Serial.print("Recharge added successfully for ");
    Serial.print(uid);
    Serial.print(": New balance = ");
    Display_TEXT("RECHARGE SUCCESSFUL", 1, 15);
    Display_TEXT("100", 2, 35);
    delay(2000);
    Serial.println(user.recharge);
    return true;
  }
  Serial.println("User not found");
  ERROR_GIF();
  Display_TEXT("USER NOT FOUND", 1, 28);
  delay(2000);
  return false;
}

// Deducts the specified amount from the recharge of the user with the given UID.
bool DEDUCT_RECH_AMT(const char* uid, uint32_t amount) {

  char formattedUID[9];
  int j = 0;
  for (int i = 0; i < strlen(uid); i++) {
    if (uid[i] != ' ') {
      formattedUID[j++] = uid[i];
    }
  }
  formattedUID[j] = '\0';
  display.clearDisplay();
  int index = FIND_USER(formattedUID);
  if (index != -1) {
    User user;
    EEPROM.get(index * sizeof(User), user);
    if (user.recharge >= amount) {
      user.recharge -= amount;
      EEPROM.put(index * sizeof(User), user);
      EEPROM.commit();
      Serial.print("Deducted ");
      Serial.print(amount);
      Serial.print(" from user ");
      Serial.print(formattedUID);
      Serial.print(": New balance = ");
      Serial.println(user.recharge);
      // Display_TEXT("Deducted AMOUNT " + amount, 1, 15);
      return true;
    } else {
      Serial.println("Insufficient balance.");
      Display_TEXT("INSUFFICIENT BALANCE", 1, 28);
      delay(2000);
      display.clearDisplay();
      return false;
    }
  }
  Serial.println("Invalid User");
  return false;
}

// This function find index from EEPROM
int FIND_USER(const char* uid) {
  for (int i = 0; i < MAX_USERS; i++) {
    User user;
    EEPROM.get(i * sizeof(User), user);
    if (strcmp(user.uid, uid) == 0) {
      return i;
    }
  }
  return -1;
}

// This function print all user from eeprom
void LIST_ALL_USER() {
  Serial.println("Current users in EEPROM:");
  for (int i = 0; i < MAX_USERS; i++) {
    User user;
    EEPROM.get(i * sizeof(User), user);
    Serial.print("User ");
    Serial.print(i);
    Serial.print(": UID = ");
    Serial.print(user.uid);
    Serial.print(", Recharge = ");
    Serial.println(user.recharge);
  }
}

// This function start washing machine
void START_WASH(uint8_t Count) {
  Serial.println("Start Washing");
  unsigned long totalTime = 1500000 * Count;
  unsigned long startTime = millis();
  char timeBuffer[6];

  display.clearDisplay();
  Display_TEXT("WASHING STARTED", 1, 30);
  delay(1500);
  display.clearDisplay();

  while (millis() - startTime < totalTime) {
    display.clearDisplay();
    unsigned long elapsedTime = millis() - startTime;
    unsigned long remainingTime = totalTime - elapsedTime;

    unsigned int minutes = remainingTime / 60000;
    unsigned int seconds = (remainingTime % 60000) / 1000;

    sprintf(timeBuffer, "%02d:%02d", minutes, seconds);

    Serial.print("Remaining Time: ");
    Serial.println(timeBuffer);
    Display_TEXT("REMAINING TIME", 1, 10);
    Display_TEXT(timeBuffer, 2, 30);

    digitalWrite(LED_PIN, HIGH);
    delay(1000);
  }
  digitalWrite(LED_PIN, LOW);
  Serial.println("Washing Complete");
}
