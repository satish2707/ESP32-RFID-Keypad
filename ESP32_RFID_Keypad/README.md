# ESP32 RFID Keypad

This project is an ESP32 RFID user access and recharge system built with an MFRC522 RFID reader, 0.96 inch SSD1306 OLED display, four-button keypad, EEPROM user storage, buzzer alert, and status output.

## Main Logic

The main system flow is preserved:

1. System initializes serial, GPIO, OLED, EEPROM, and RFID modules.
2. OLED shows the startup logo.
3. Normal user card scan deducts one recharge unit.
4. If balance is available, the timed output workflow starts.
5. Admin card scan opens admin setup mode.
6. In admin mode, scan a user card and use the four-button menu to add, delete, recharge, or check the user.

## Hardware

| Module | Signal | ESP32 Pin |
| --- | --- | --- |
| MFRC522 RFID | SS/SDA | GPIO 27 |
| MFRC522 RFID | RST | GPIO 14 |
| MFRC522 RFID | MISO | GPIO 19 |
| MFRC522 RFID | MOSI | GPIO 23 |
| MFRC522 RFID | SCK | GPIO 18 |
| SSD1306 OLED | SDA | GPIO 21 |
| SSD1306 OLED | SCL | GPIO 22 |
| Keypad Button 1 | Restart / Exit | GPIO 26 |
| Keypad Button 2 | Menu Next | GPIO 32 |
| Keypad Button 3 | Menu Previous | GPIO 25 |
| Keypad Button 4 | OK / Select | GPIO 33 |
| Buzzer | Alert Output | GPIO 13 |
| Status Output | LED / Load Output | GPIO 2 |

## Required Libraries

Install these Arduino libraries:

- `MFRC522`
- `Adafruit SSD1306`
- `Adafruit GFX`
- `EEPROM`
- `Ticker`

## Important Configuration

| File | Value | Purpose |
| --- | --- | --- |
| `RFID.h` | `AdminCardStr` | Admin RFID card UID. |
| `RFID.h` | `SS_PIN`, `RST_PIN` | MFRC522 SPI control pins. |
| `ANALOG.h` | `BUTTON1` to `BUTTON4` | Four-button keypad pins. |
| `ANALOG.h` | `BUZZER_PIN`, `LED_PIN` | Alert and status output pins. |
| `ESP_EEPROM.h` | `MAX_USERS` | Maximum stored RFID users. |
| `ESP_EEPROM.h` | `EEPROM_SIZE` | ESP32 EEPROM allocation size. |

## Admin Menu

| Button | Action |
| --- | --- |
| Button 1 | Restart system |
| Button 2 | Move to next menu item |
| Button 3 | Move to previous menu item |
| Button 4 | Select current menu item |

Menu options:

1. Add user
2. Delete user
3. User recharge
4. Check recharge

## How To Use

1. Open `ESP32_RFID_Keypad.ino` in Arduino IDE.
2. Select the correct ESP32 board and COM port.
3. Install the required libraries.
4. Update `AdminCardStr` in `RFID.h` with your admin RFID card UID.
5. Wire the modules according to the table above.
6. Upload the sketch.
7. Open Serial Monitor at `115200` baud.

## Notes

- RFID UIDs are stored as 8-character hexadecimal strings without spaces.
- Admin setup waits for a user card after the admin card is scanned.
- Recharge value is currently fixed in code as `100`.
- A valid normal user scan deducts `1` unit and starts the timed output workflow.
- The status output timer is handled in `START_WASH()`.

@copyright : Satish Kanawade. All rights reserved.
