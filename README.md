# ESP32 NRF24L01+ Chat Device  
*A project by **RDH Robotics***

This project implements a two-way chat system using ESP32 microcontrollers, NRF24L01+ transceivers, and TFT displays with a custom button-based keyboard interface. Two device-specific Arduino sketches (`Device1_Chat.ino` and `Device2_Chat.ino`) enable communication between two devices over a 2.4GHz wireless link. Users can input messages via a 5x5 keyboard displayed on a TFT screen or through a Serial terminal, with support for letters, numbers, punctuation, and emojis. Received messages are stored and displayed, with robust error handling for stable communication.

---

## 🚀 Features

- **Wireless Communication**: Uses NRF24L01+ modules for reliable 2.4GHz communication.  
- **TFT Interface**: Displays a 5x5 keyboard, recipient code, current message, and up to 5 received messages.  
- **Button Controls**: Navigate and type using analog buttons (UP, DOWN, LEFT, RIGHT, MIDDLE, PAGE, EMOJI).  
- **Serial Interface**: Send messages, set recipient codes, and control ADC debugging via Serial commands.  
- **Message History**: Stores and displays up to 5 received messages in smaller text.  
- **Stable Transmission**: Configured with retries and low power settings for reliable communication.  
- **Dynamic Recipient Codes**: Set 4-digit recipient codes via Serial or buttons.

---

## 🧰 Hardware Requirements

- 2x ESP32 Development Boards (e.g., ESP32-WROOM-32)  
- 2x NRF24L01+ Transceivers  
- 2x TFT Displays (compatible with TFT_eSPI, e.g., 240x320 ILI9341)  
- Analog Button Circuit connected to ADC pin (GPIO36/VP)  
  - Voltage Thresholds:
    - UP: 1.5V
    - DOWN: 0.6V
    - LEFT: 1.0V
    - RIGHT: 0.15V
    - MIDDLE: 0V
    - PAGE: 0.3V
    - EMOJI: 0.11V

---

## 🔌 Wiring

### NRF24L01+
| NRF24L01+ | ESP32 GPIO |
|-----------|-------------|
| CE        | GPIO5       |
| CSN       | GPIO17      |
| SCK       | GPIO18      |
| MOSI      | GPIO23      |
| MISO      | GPIO19      |

### TFT Display
Configured via `TFT_eSPI`'s `User_Setup.h` file.

### Buttons
All buttons connected to GPIO36 (VP) using a resistor divider network.

---

## 🖥️ Software Requirements

- Arduino IDE  
- Libraries:  
  - `SPI.h`  
  - `RF24.h` *(NRF24L01+ driver)*  
  - `TFT_eSPI.h` *(TFT display driver)*  

> **Note**: Ensure `User_Setup.h` is configured properly for your display.

---

## ⚙️ Setup Instructions

### Hardware

1. Connect NRF24L01+ to each ESP32.
2. Wire TFT display and configure `User_Setup.h`.
3. Connect analog buttons to GPIO36 using a voltage divider.
4. Add a capacitor (e.g., 10µF) across NRF24L01+'s VCC/GND for stability.

### Software

1. Install required libraries via Arduino Library Manager.
2. Update `User_Setup.h` in the TFT_eSPI library.
3. Upload `Device1_Chat.ino` to Device 1 (MY_CODE = `1234`).
4. Upload `Device2_Chat.ino` to Device 2 (MY_CODE = `5678`).
5. Open Serial Monitor at **115200 baud** with **Newline** enabled.

---

## 🕹️ Usage Guide

### Modes

#### 1. Recipient Selection Mode (`mode = 0`)
- UP/DOWN: Change digit  
- LEFT/RIGHT: Move between digits  
- MIDDLE: Confirm code & switch to typing

#### 2. Typing Mode (`mode = 1`)
- Navigate the 5x5 keyboard using direction buttons.  
- MIDDLE: Select character / Backspace (`<`)  
- PAGE: Switch keyboard pages (Letters / Numbers / Emojis)  
- UP + DOWN (together): Quick jump to Emoji page  
- Long press MIDDLE: Send message (shows "Sent")

---

## 🖼️ Display Layout

| Element              | Position (Y) | Text Size | Notes                               |
|----------------------|--------------|------------|-------------------------------------|
| Instructions         | 0            | 1          | Button guide                        |
| Recipient Code       | 20           | 3          | Highlights current digit (mode 0)   |
| Current Message      | 60           | 2          | What you're typing                  |
| Received Messages    | 90–165       | 1          | Up to 5 messages, shown in yellow   |
| "Sent" Indicator     | 220          | 2          | Visible for 2 seconds after sending |
| Keyboard             | 245–320      | 1          | Selected character in green         |

---

## 🧑‍💻 Serial Commands

| Command           | Description                                      |
|------------------|--------------------------------------------------|
| `set XXXX`        | Set recipient code (e.g., `set 5678`)           |
| `chng XXXX`       | Change recipient code (no mode reset)           |
| `<message>`       | Send message directly                           |
| `ADCprint`        | Enable ADC value monitoring                     |
| `ADCstop`         | Disable ADC value monitoring                    |

---

## 📡 Example Chat

1. On **Device 1**, enter `set 5678`  
2. On **Device 2**, enter `set 1234`  
3. Type and send a message from Serial or TFT  
4. Messages appear on TFT and Serial Monitor

---

## 🧠 Code Structure

- **Device1_Chat.ino** → `MY_CODE = 1234`, default recipient `5678`  
- **Device2_Chat.ino** → `MY_CODE = 5678`, default recipient `1234`

### Key Functions

| Function        | Description                                           |
|-----------------|-------------------------------------------------------|
| `setup()`       | Initializes Serial, TFT, SPI, RF24, ADC              |
| `loop()`        | Main logic, handles input and message handling      |
| `readButton()`  | Reads ADC pin and determines which button was pressed |
| `handleButton()`| Executes button actions                              |
| `updateDisplay()`| Renders the full screen UI                         |

---

## 🛠️ Troubleshooting

### No Communication?
- Check NRF24L01+ wiring and stable 3.3V power (add capacitor)
- Verify correct recipient codes and Serial Monitor settings

### TFT Display Blank?
- Ensure `User_Setup.h` is properly configured
- Double check all SPI connections

### Button Not Working?
- Use `ADCprint` to verify voltage levels
- Adjust resistors if thresholds are mismatched

---

## 📜 License

This project is licensed under the [MIT License](LICENSE).

---

## 🤝 Contributing

PRs and ideas welcome!  
Open an issue or submit a pull request on GitHub.

---

### 🧠 Built with ❤️ by **RDH Robotics**

> Powered by the Arduino Ecosystem, ESP32, NRF24L01+, and the amazing `RF24` and `TFT_eSPI` libraries.

