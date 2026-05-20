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
#include "ANALOG.h"
#include "SSD1306.h"
#include "ESP_EEPROM.h"
#include "stdint.h"

extern Adafruit_SSD1306 display;
extern char uidString[];
uint32_t USER_RECHARGE = 0;
int spacing = 12;
char scannedUID[12] = "";

// Enum for menu options
enum MenuOption {
  OPTION_1,
  OPTION_2,
  OPTION_3,
  OPTION_4,
  OPTION_COUNT
};

// Structure to hold menu information
struct Menu {
  const char* title;
};

Menu menus[OPTION_COUNT] = {
  { "1. ADD USER" },
  { "2. DELETE USER" },
  { "3. USER RECHARGE" },
  { "4. CHECK RECHARGE" },
};

// Track which menu is selected
volatile int selected = 0;

// Initialize buttons with respective pins
Button button1 = { BUTTON1, 0, false };
Button button2 = { BUTTON2, 0, false };
Button button3 = { BUTTON3, 0, false };
Button button4 = { BUTTON4, 0, false };

// Variables to track button press timing
unsigned long lastButtonTime[4] = { 0, 0, 0, 0 };
const unsigned long debounceDelay = 250;  // Debounce time in ms

// Interrupt Service Routine (ISR) for handling button presses
void IRAM_ATTR isr_button(int buttonIndex) {
  unsigned long currentTime = millis();
  if (currentTime - lastButtonTime[buttonIndex] > debounceDelay) {
    lastButtonTime[buttonIndex] = currentTime;
    switch (buttonIndex) {
      case 0: button1.pressed = true; break;
      case 1: button2.pressed = true; break;
      case 2: button3.pressed = true; break;
      case 3: button4.pressed = true; break;
    }
  }
}

// Function to initialize GPIO pins and interrupts
void GPIO_INIT() {
  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2, INPUT_PULLUP);
  pinMode(BUTTON3, INPUT_PULLUP);
  pinMode(BUTTON4, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  attachInterrupt(
    BUTTON1, []() {
      isr_button(0);
    },
    FALLING);
  attachInterrupt(
    BUTTON2, []() {
      isr_button(1);
    },
    FALLING);
  attachInterrupt(
    BUTTON3, []() {
      isr_button(2);
    },
    FALLING);
  attachInterrupt(
    BUTTON4, []() {
      isr_button(3);
    },
    FALLING);
}

// Function to perform the action based on the selected menu option
void PerformAction(int selectedOption) {
  char formattedUID[9];
  int j = 0;
  for (int i = 0; i < strlen(uidString); i++) {
    if (uidString[i] != ' ') {
      formattedUID[j++] = uidString[i];
    }
  }
  formattedUID[j] = '\0';

  Serial.print("Formatted RFID UID: ");
  Serial.println(formattedUID);
  Serial.print("Formatted UID Length: ");
  Serial.println(strlen(formattedUID));

  // Now proceed with the actions based on selected menu
  switch (selectedOption) {
    case OPTION_1:
      Serial.println("Action: Adding User");
      ADD_USER_RFID(formattedUID);
      break;
    case OPTION_2:
      Serial.println("Action: Deleting User");
      DELETE_USER_RFID(formattedUID);
      break;
    case OPTION_3:
      Serial.println("Action: User Recharge");
      ADD_USER_RECHARGE_RFID(formattedUID, 100);
      break;
    case OPTION_4:
      Serial.println("Action: Checking Recharge");
      USER_RECHARGE = CHECK_USER_RECHARGE(formattedUID);
      Serial.print("User Recharge is: ");
      Serial.println(USER_RECHARGE);
      break;
    default:
      Serial.println("Invalid option");
      break;
  }
}


// Function to check the button state and take action
void Check_button() {
  if (button1.pressed) {
    button1.pressed = false;
    Serial.println("Button 1 pressed");
    ESP.restart();
  }

  if (button2.pressed) {
    button2.pressed = false;
    selected = (selected + 1) % OPTION_COUNT;  // Move to next option
    Serial.print("Selected Menu: ");
    Serial.println(menus[selected].title);
  }

  if (button3.pressed) {
    button3.pressed = false;
    selected = (selected - 1 + OPTION_COUNT) % OPTION_COUNT;  // Move to previous option
    Serial.print("Selected Menu: ");
    Serial.println(menus[selected].title);
  }

  if (button4.pressed) {
    button4.pressed = false;
    Serial.print("OK button pressed. Selected Option: ");
    Serial.println(menus[selected].title);
    PerformAction(selected);  // Perform selected action
  }
}

// This function displays the menu on the OLED display
void DISPLAY_MENU() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("-* ADMIN DASHBOARD *-");
  display.println("---------------------");

  for (int i = 0; i < OPTION_COUNT; i++) {
    if (i == selected) {
      display.fillRect(0, 15 + i * spacing, SCREEN_WIDTH, spacing, SSD1306_WHITE); 
      display.setTextColor(SSD1306_BLACK);
    } else {
      display.setTextColor(SSD1306_WHITE);
    }
    display.setCursor(5, 18 + i * spacing);
    display.println(menus[i].title);
  }
  display.display();
}

// This function beep a buzzer
void BUZZER_ALERT(uint8_t count) {
  for (uint8_t i = 0; i < count; i++) {
    digitalWrite(BUZZER_PIN, HIGH); 
    delay(500); 
    digitalWrite(BUZZER_PIN, LOW);  
    delay(500);
  }
}
