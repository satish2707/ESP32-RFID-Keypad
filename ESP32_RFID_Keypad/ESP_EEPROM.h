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
#include <Arduino.h>
#include "EEPROM.h"

#ifndef ESP_EEPROM_H
#define ESP_EEPROM_H

#define EEPROM_SIZE 2056  // Adjust size based on your needs
#define MAX_USERS 10      // Maximum number of users
#define UID_LENGTH 8

void EEPROM_INIT();
/**
 * @details: Initializes the ESP32 EEPROM.
 * @param  : None
 * @returns: None
 */

bool ADD_USER_RFID(const char* uid);
/**
 * @details: Adds a new user with the given UID to EEPROM.
 * @param  : USER UID
 * @returns: True / false
 */

bool DELETE_USER_RFID(const char* uid);
/**
 * @details: Deletes the user with the given UID from EEPROM.
 * @param  : USER UID
 * @returns: True / false
 */

uint32_t CHECK_USER_RECHARGE(const char* uid);
/**
 * @details: Checks the recharge amount of the user with the given UID.
 * @param  : USER UID
 * @returns: The recharge amount or 0 if user not found or expired.
 */

bool ADD_USER_RECHARGE_RFID(const char* uid, uint32_t amount);
/**
 * @details: Adds the specified amount of recharge to the user with the given UID.
 * @param  : USER UID
 * @returns: True / false
 */

int FIND_USER(const char* uid);
/**
 * @details: Finds the index of the user with the given UID in EEPROM.
 * @param  : USER UID
 * @returns: The index of the user or -1 if not found.
 */

bool DEDUCT_RECH_AMT(const char* uid, uint32_t amount);
/**
 * @details: Deducts the specified amount from the recharge of the user with the given UID.
 * @param  : USER UID , Amount
 * @returns: True / false
 */

void clearEEPROM();
/**
 * @details: Clears all data in the EEPROM.
 * @param  : None
 * @returns: None
 */

void testUserFunctions();
/**
 * @details: Tests various user functions for functionality.
 * @param  : None
 * @returns: None
 */

void START_WASH(uint8_t Count);
/**
 * @details: This function start washing machine
 * @param  : cout
 * @returns: None
 */

void LIST_ALL_USER();
/**
 * @details: Lists all users stored in the EEPROM.
 * @param  : None
 * @returns: None
 */

#endif
