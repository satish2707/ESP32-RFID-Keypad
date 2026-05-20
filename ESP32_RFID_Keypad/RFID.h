/***********************************************************************************************************
 * @details : This Project Contains Application of RFID using ESP32
 *
 * @author  : Satish Kanawade (Embedded Software developer)
 *
 * @copyright : Satish Kanawade. All rights reserved.
 *
 * @date    : 23 OCT 2024
 *
 * @version : V.1.0
 **********************************************************************************************************/
#ifndef _RFID_H_
#define _RFID_H_

#define SS_PIN  27
#define RST_PIN 14
#define RFID_UID_BUFFER_SIZE 32

const char AdminCardStr[] = "63 02 B1 E2";

void RFID_INIT();
/**
 * @details  This function Initilize the RFID Module
 * @param    None 
 * @returns  None
 */

bool compareUIDs(const char* uid1, const char* uid2);
/**
 * @details  This function Compair two RFID card
 * @param    None 
 * @returns  None
 */

bool CompareUID(byte *storedUID, byte *readUID);
/**
 * @details  This function compare RFID card
 * @param    byte 
 * @returns  bool
 */

void printUID(byte *buffer, byte bufferSize);
/**
 * @details  This function compare RFID card
 * @param    byte 
 * @returns  None
 */

char *SCAN_RFID();
/**
 * @details  This function SCAN the new RFID card
 * @param    None 
 * @returns  None
 */
#endif
