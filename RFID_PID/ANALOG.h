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
#include <Arduino.h>

#ifndef ANALOG_H
#define ANALOG_H

// Define button pin numbers
#define BUTTON1 26
#define BUTTON2 32
#define BUTTON3 25
#define BUTTON4 33
#define BUZZER_PIN 13
#define LED_PIN 2

// Structure to hold button properties
struct Button {
  const uint8_t PIN;
  uint32_t numberKeyPresses;
  bool pressed;
};

// Function prototypes
void GPIO_INIT();
/**
 * @details: This function initializes the GPIO of ESP32
 * @param  : None
 * @returns: None
 */

void DISPLAY_MENU(void);
/**
 * @details: This function display the menu on OLED
 * @param  : None
 * @returns: None
 */
 
void Check_button();
/**
 * @details: Function to check the button state and print the number of presses
 * @param  : button - Reference to the Button struct
 * @returns: None
 */

void BUZZER_ALERT(uint8_t count);
/**
 * @details: This function beep a buzzer
 * @param  : cnt
 * @returns: None
 */

#endif
