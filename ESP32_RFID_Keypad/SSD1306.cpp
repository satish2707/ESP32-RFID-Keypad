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

// Define GIF image variable Here
int LOADING_GIF = 0;
int ADD_USER_frame = 0;
int DELETE_USER_frame = 0;
int SCAN_RFID_frame = 0;
int CHECK_USER_RECH_frame = 0;
int ADD_USER_RECH_frame = 0;
int COMPLETE_frame = 0;
int ERROR_frame = 0;

#define LOADING_GIF_TIME 3000
#define ADD_USER_GIF_TIME 1500
#define DELETE_USER_GIF_TIME 3000
#define SCAN_RFID_GIF_TIME 500
#define CHECK_USER_RECH_TIME 3000
#define ADD_USER_RECH_TIME 3000
#define COMPLETE_TIME 3000
#define ERROR_TIME 2000

// Initialize OLED display object
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// This function initilize the Display SSD1306
void Display_init() {
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 initialized failed");
  } else {
    Serial.println("SSD1306 initialized successfully");
  }
  display.clearDisplay();
}

// This function Display the menu1 on display
void Display_TEXT(const char* Print_Name, int fontSize, int y) {
  display.drawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SSD1306_WHITE);
  int textHeight = 8 * fontSize;
  int textLength = strlen(Print_Name);
  int16_t textWidth = textLength * 6 * fontSize;

  display.fillRect(2, y, SCREEN_WIDTH - textWidth, textHeight, SSD1306_BLACK);
  display.setTextSize(fontSize);
  display.setTextColor(SSD1306_WHITE);

  int16_t x = (SCREEN_WIDTH - textWidth) / 2;
  display.setCursor(x, y);
  display.println(Print_Name);
  display.display();
}


// This function Display Scan RFID GIF image on display
void SCAN_RFID_GIF() {
  unsigned long startTime = millis();
  while (millis() - startTime < SCAN_RFID_GIF_TIME) {
    display.clearDisplay();
    display.drawBitmap(32, 0, SCAN_RFID_BITMAPS[SCAN_RFID_frame], SCAN_RFID_FRAME_WIDTH, SCAN_RFID_FRAME_HEIGHT, 1);
    display.display();
    SCAN_RFID_frame = (SCAN_RFID_frame + 1) % SCAN_RFID_FRAME_COUNT;
    delay(SCAN_RFID_FRAME_DELAY);
  }
  display.display();
}

// This function Display Loading GIF image on display
void Loading_GIF() {
  unsigned long startTime = millis();
  while (millis() - startTime < LOADING_GIF_TIME) {
    display.clearDisplay();
    display.drawBitmap(32, 0, LOADING_frames[LOADING_GIF], LOADING_FRAME_WIDTH, LOADING_FRAME_HEIGHT, 1);
    display.display();
    LOADING_GIF = (LOADING_GIF + 1) % LOADING_FRAME_COUNT;
    delay(LOADING_FRAME_DELAY);
  }
  display.clearDisplay();
  display.display();
  display.clearDisplay();
}

// This function Display Add user GIF image on display
void ADD_USER_GIF() {
  unsigned long startTime = millis();
  while (millis() - startTime < ADD_USER_GIF_TIME) {
    display.clearDisplay();
    display.drawBitmap(32, 0, ADD_USER[ADD_USER_frame], ADD_USER_FRAME_WIDTH, ADD_USER_FRAME_HEIGHT, 1);
    display.display();
    ADD_USER_frame = (ADD_USER_frame + 1) % ADD_USER_FRAME_COUNT;
    delay(ADD_USER_DELAY);
  }
  display.clearDisplay();
  display.display();
  display.clearDisplay();
}

// This function Display Delete user GIF image on display
void DELETE_USER_GIF() {
  unsigned long startTime = millis();
  while (millis() - startTime < DELETE_USER_GIF_TIME) {
    display.clearDisplay();
    display.drawBitmap(32, 0, DELETE_USER[DELETE_USER_frame], DELETE_USER_FRAME_WIDTH, DELETE_USER_RAME_HEIGHT, 1);
    display.display();
    DELETE_USER_frame = (DELETE_USER_frame + 1) % DELETE_USER_FRAME_COUNT;
    delay(DELETE_USER_FRAME_DELAY);
  }
  display.clearDisplay();
  display.display();
  display.clearDisplay();
}

// This function Display Check user Recharge GIF image on display
void CHECK_USER_RECH_GIF() {
  unsigned long startTime = millis();
  while (millis() - startTime < CHECK_USER_RECH_TIME) {
    display.clearDisplay();
    display.drawBitmap(32, 0, CHECK_USER_RECH[CHECK_USER_RECH_frame], CHECK_USER_RECH_FRAME_WIDTH, CHECK_USER_RECH_FRAME_HEIGHT, 1);
    display.display();
    CHECK_USER_RECH_frame = (CHECK_USER_RECH_frame + 1) % FRAME_COUNT;
    delay(CHECK_USER_RECH_FRAME_DELAY);
  }
  display.clearDisplay();
  display.display();
  display.clearDisplay();
}

// This function Display Add user Recharge GIF image on display
void ADD_USER_RECH_GIF() {
  unsigned long startTime = millis();
  while (millis() - startTime < ADD_USER_RECH_TIME) {
    display.clearDisplay();
    display.drawBitmap(32, 0, ADD_USER_RECHARGE[ADD_USER_RECH_frame], ADD_USER_RECH_FRAME_WIDTH, ADD_USER_RECH_FRAME_HEIGHT, 1);
    display.display();
    ADD_USER_RECH_frame = (ADD_USER_RECH_frame + 1) % ADD_USER_RECH_FRAME_COUNT;
    delay(ADD_USER_RECH_FRAME_DELAY);
  }
  display.clearDisplay();
  display.display();
  display.clearDisplay();
}

// This function Display Check user Recharge GIF image on display
void COMPLETE_GIF() {
  unsigned long startTime = millis();
  while (millis() - startTime < COMPLETE_TIME) {
    display.clearDisplay();
    display.drawBitmap(32, 0, COMPLETE_frames[COMPLETE_frame], COMPLETE_FRAME_WIDTH, COMPLETE_FRAME_HEIGHT, 1);
    display.display();
    COMPLETE_frame = (COMPLETE_frame + 1) % COMPLETE_FRAME_COUNT;
    delay(COMPLETE_FRAME_DELAY);
  }
  display.clearDisplay();
  display.display();
  display.clearDisplay();
}


// This function Display ERROR GIF image on display
void ERROR_GIF() {
  unsigned long startTime = millis();
  while (millis() - startTime < ERROR_TIME) {
    display.clearDisplay();
    display.drawBitmap(32, 0, ERROR_frames[ERROR_frame], ERROR_FRAME_WIDTH, ERROR_FRAME_HEIGHT, 1);
    display.display();
    ERROR_frame = (ERROR_frame + 1) % ERROR_FRAME_COUNT;
    delay(ERROR_FRAME_DELAY);
  }
  display.clearDisplay();
  display.display();
  display.clearDisplay();
}

// This Function Display the startup logo
void DISPLAY_LOGO() {
  display.clearDisplay();
  display.drawBitmap(0, 0, Company_logo, LOGO_Height, LOGO_Width, WHITE);
  display.display();
}

// Print the initial Serial Print on Serial
void FM_START_PRINT() {
  Serial.println("|---------------------------------------------------------------|");
  Serial.println("|--------------------- ESP32 RFID KEYPAD -----------------------|");
  Serial.println("|---------------------------------------------------------------|");
  Serial.println("");
}
