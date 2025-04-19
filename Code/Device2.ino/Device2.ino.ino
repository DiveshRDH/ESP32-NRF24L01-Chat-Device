#include <SPI.h>
#include <RF24.h>
#include <TFT_eSPI.h>

// NRF24L01+ pins
#define CE_PIN 5
#define CSN_PIN 17

// ADC pin for buttons
#define ADC_PIN 36 // VP pin (ADC1_CHANNEL_0)

// Button voltage thresholds (in ADC units, 0-4095 for 0-3.3V)
#define V_UP 1843    // 1.5V
#define V_DOWN 737   // 0.6V
#define V_MIDDLE 0   // 0.00V
#define V_LEFT 1227  // 1.0V
#define V_RIGHT 184  // 0.15V
#define V_PAGE 368   // 0.3V
#define V_EMOJI 135  // 0.11V (UP+DOWN)
#define V_NONE 4095  // 3.3V (no press)

// Long press threshold (ms)
#define LONG_PRESS 500

// Keyboard pages (5x5 grid)
const char keyboard[3][5][5] = {
  // Page 0: Letters (UK QWERTY subset)
  {
    {'Q', 'W', 'E', 'R', 'T'},
    {'Y', 'U', 'I', 'O', 'P'},
    {'A', 'S', 'D', 'F', 'G'},
    {'H', 'J', 'K', 'L', ' '},
    {'Z', 'X', 'C', '<', ' '} // '<' is backspace
  },
  // Page 1: Numbers/Punctuation
  {
    {'1', '2', '3', '4', '5'},
    {'6', '7', '8', '9', '0'},
    {'@', '#', '£', '.', ','},
    {'!', '?', ';', ':', '-'},
    {'_', '(', ')', '<', ' '}
  },
  // Page 2: Emojis
  {
    {':', ')', ' ', ' ', ' '}, // :)
    {':', '(', ' ', ' ', ' '}, // :(
    {':', 'D', ' ', ' ', ' '}, // :D
    {'<', '3', ' ', ' ', ' '}, // <3
    {'^', '^', ' ', ' ', ' '}  // ^^
  }
};

// Objects
RF24 radio(CE_PIN, CSN_PIN);
TFT_eSPI tft = TFT_eSPI();

// Device-specific variables
const uint32_t MY_CODE = 5678; // Unique code for Device 2
char recipient_code[5] = "1234"; // Default to Device 1
char message[32] = ""; // Current message
char received_messages[5][32] = {""}; // Store up to 5 received messages
int received_count = 0; // Number of received messages
int cursor_x = 0, cursor_y = 0; // Keyboard cursor
int mode = 0; // 0: Select recipient, 1: Type message
int message_pos = 0; // Position in message
int keyboard_page = 0; // 0: Letters, 1: Numbers/Punc, 2: Emojis
unsigned long button_press_time = 0; // For long press detection
bool middle_pressed = false; // Track MIDDLE button state
bool adc_print = false; // ADC printing flag
int last_adc = -1; // Last ADC value
unsigned long sent_display_time = 0; // Time when "Sent" was displayed

void setup() {
  // Initialize Serial
  Serial.begin(115200);
  while (!Serial) {
    delay(10);
  }
  Serial.println("Starting Device 2...");

  // Initialize TFT
  tft.init();
  tft.setRotation(0); // Landscape (240x320)
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(0, 0);
  tft.println("Initializing...");

  // Initialize SPI for NRF24L01+ (VSPI)
  SPI.begin(18, 19, 23, 17);

  // Initialize NRF24L01+
  if (!radio.begin()) {
    Serial.println("NRF24L01+ not detected!");
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.setCursor(0, 20);
    tft.println("NRF24L01+ NOT DETECTED");
    while (1);
  }
  Serial.println("NRF24L01+ initialized!");
  radio.setPALevel(RF24_PA_LOW);
  radio.setDataRate(RF24_250KBPS);
  radio.setRetries(15, 15); // Max retries for stable communication
  radio.openWritingPipe((uint8_t*)"1234"); // Default to Device 1
  radio.openReadingPipe(1, (uint8_t*)"5678"); // Device 2's address
  radio.startListening();

  // Initialize ADC
  analogReadResolution(12);
  pinMode(ADC_PIN, INPUT);

  // Clear display and show initial screen
  updateDisplay();
}

void loop() {
  // Read ADC and handle printing
  int adc_value = analogRead(ADC_PIN);
  if (adc_print && adc_value != last_adc) {
    Serial.println("ADC: " + String(adc_value));
    last_adc = adc_value;
  }

  // Handle button input
  int button = readButton();
  if (button == 2) { // MIDDLE button
    if (!middle_pressed) {
      middle_pressed = true;
      button_press_time = millis();
    }
  } else if (middle_pressed && button == -1) {
    unsigned long press_duration = millis() - button_press_time;
    middle_pressed = false;
    if (press_duration < LONG_PRESS) {
      handleButton(2); // Short press
    } else {
      handleButton(7); // Long press (send)
    }
    updateDisplay();
    delay(200); // Debounce
  } else if (button != -1 && button != 2) {
    handleButton(button);
    updateDisplay();
    delay(200); // Debounce
  }

  // Handle Serial input
  if (Serial.available()) {
    String serial_input = Serial.readStringUntil('\n');
    serial_input.trim();
    if (serial_input.startsWith("set ")) {
      String code = serial_input.substring(4);
      if (code.length() == 4 && code.toInt() >= 0 && code.toInt() <= 9999) {
        strncpy(recipient_code, code.c_str(), 5);
        mode = 1; // Switch to typing mode
        keyboard_page = 0;
        radio.stopListening();
        radio.openWritingPipe((uint8_t*)recipient_code);
        Serial.println("Recipient set to: " + String(recipient_code));
        updateDisplay();
      } else {
        Serial.println("Invalid code. Use 'set XXXX' (4 digits)");
      }
    } else if (serial_input.startsWith("chng ")) {
      String code = serial_input.substring(5);
      if (code.length() == 4 && code.toInt() >= 0 && code.toInt() <= 9999) {
        strncpy(recipient_code, code.c_str(), 5);
        radio.stopListening();
        radio.openWritingPipe((uint8_t*)recipient_code);
        radio.startListening();
        Serial.println("Recipient changed to: " + String(recipient_code));
        updateDisplay();
      } else {
        Serial.println("Invalid code. Use 'chng XXXX' (4 digits)");
      }
    } else if (serial_input == "ADCprint") {
      adc_print = true;
      Serial.println("ADC printing enabled");
    } else if (serial_input == "ADCstop") {
      adc_print = false;
      Serial.println("ADC printing disabled");
    } else if (serial_input.length() > 0 && serial_input.length() < 32) {
      strncpy(message, serial_input.c_str(), sizeof(message) - 1);
      message[sizeof(message) - 1] = '\0';
      radio.stopListening();
      if (radio.write(message, sizeof(message))) {
        Serial.println("Sent via Serial: " + String(message));
        sent_display_time = millis();
        updateDisplay();
      } else {
        Serial.println("Failed to send message");
      }
      message[0] = '\0';
      message_pos = 0;
      radio.startListening();
    }
  }

  // Check for incoming messages
  if (radio.available()) {
    char new_message[32] = "";
    radio.read(new_message, sizeof(new_message));
    Serial.println("Received: " + String(new_message));
    // Shift message history
    if (received_count < 5) {
      strncpy(received_messages[received_count], new_message, sizeof(received_messages[0]));
      received_count++;
    } else {
      for (int i = 0; i < 4; i++) {
        strncpy(received_messages[i], received_messages[i + 1], sizeof(received_messages[0]));
      }
      strncpy(received_messages[4], new_message, sizeof(received_messages[0]));
    }
    updateDisplay();
  }

  // Clear "Sent" message after 2 seconds
  if (sent_display_time && millis() - sent_display_time > 2000) {
    sent_display_time = 0;
    updateDisplay();
  }
}

// Read button based on ADC voltage
int readButton() {
  int adc_value = analogRead(ADC_PIN);
  if (abs(adc_value - V_UP) < 100) return 0; // UP
  if (abs(adc_value - V_DOWN) < 100) return 1; // DOWN
  if (abs(adc_value - V_MIDDLE) < 100) return 2; // MIDDLE
  if (abs(adc_value - V_LEFT) < 100) return 3; // LEFT
  if (abs(adc_value - V_RIGHT) < 100) return 4; // RIGHT
  if (abs(adc_value - V_PAGE) < 100) return 5; // PAGE
  if (abs(adc_value - V_EMOJI) < 100) return 6; // UP+DOWN
  return -1; // No press
}

// Handle button presses
void handleButton(int button) {
  if (mode == 0) { // Select recipient code
    if (button == 0 && recipient_code[cursor_x] < '9') recipient_code[cursor_x]++;
    if (button == 1 && recipient_code[cursor_x] > '0') recipient_code[cursor_x]--;
    if (button == 3 && cursor_x > 0) cursor_x--;
    if (button == 4 && cursor_x < 3) cursor_x++;
    if (button == 2) { // MIDDLE short press
      mode = 1; // Switch to typing mode
      cursor_x = 0;
      cursor_y = 0;
      keyboard_page = 0;
      radio.stopListening();
      radio.openWritingPipe((uint8_t*)recipient_code);
      Serial.println("Switched to typing mode");
    }
  } else if (mode == 1) { // Type message
    if (button == 0 && cursor_y > 0) cursor_y--;
    if (button == 1 && cursor_y < 4) cursor_y++;
    if (button == 3 && cursor_x > 0) cursor_x--;
    if (button == 4 && cursor_x < 4) cursor_x++;
    if (button == 5) { // PAGE
      keyboard_page = (keyboard_page + 1) % 3;
      cursor_x = 0;
      cursor_y = 0;
    }
    if (button == 6) { // UP+DOWN for emoji page
      keyboard_page = 2;
      cursor_x = 0;
      cursor_y = 0;
    }
    if (button == 2) { // MIDDLE short press
      char selected = keyboard[keyboard_page][cursor_y][cursor_x];
      if (selected == '<' && message_pos > 0) {
        message[message_pos - 1] = '\0'; // Backspace
        message_pos--;
      } else if (selected != ' ' && message_pos < 31) {
        message[message_pos] = selected;
        message_pos++;
        message[message_pos] = '\0';
      } else if (selected == ' ' && message_pos < 31) {
        message[message_pos] = ' '; // Space
        message_pos++;
        message[message_pos] = '\0';
      }
    }
    if (button == 7 && message_pos > 0) { // MIDDLE long press
      radio.stopListening();
      if (radio.write(message, sizeof(message))) {
        Serial.println("Sent via TFT: " + String(message));
        sent_display_time = millis();
      } else {
        Serial.println("Failed to send message");
      }
      message[0] = '\0';
      message_pos = 0;
      radio.startListening();
    }
  }
}

// Update TFT display
void updateDisplay() {
  tft.fillScreen(TFT_BLACK);

  // Instructions
  tft.setTextSize(1);
  tft.setCursor(0, 0);
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  if (mode == 0) tft.println("UP/DOWN:Digit MIDDLE:Confirm");
  else tft.println("UP/DOWN/LEFT/RIGHT:Move MIDDLE:Select Hold:SEND");

  // Recipient code
  tft.setTextSize(3);
  tft.setCursor(0, 20);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.print("To: ");
  if (mode == 0) {
    for (int i = 0; i < 4; i++) {
      if (i == cursor_x) tft.setTextColor(TFT_GREEN, TFT_BLACK);
      else tft.setTextColor(TFT_WHITE, TFT_BLACK);
      tft.print(recipient_code[i]);
    }
  } else {
    tft.print(recipient_code);
  }

  // Current message
  tft.setTextSize(2);
  tft.setCursor(0, 60);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.println(message);

  // Received messages (smaller text)
  tft.setTextSize(1);
  int y_pos = 90;
  for (int i = 0; i < received_count; i++) {
    tft.setCursor(0, y_pos);
    tft.setTextColor(TFT_YELLOW, TFT_BLACK);
    tft.println(received_messages[i]);
    y_pos += 15; // Adjust spacing for smaller text
  }

  // "Sent" message
  if (sent_display_time) {
    tft.setTextSize(2);
    tft.setCursor(0, 220);
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.println("Sent");
  }

  // Keyboard (in typing mode)
  if (mode == 1) {
    tft.setTextSize(1);
    for (int y = 0; y < 5; y++) {
      for (int x = 0; x < 5; x++) {
        if (keyboard[keyboard_page][y][x] == ' ') continue;
        if (x == cursor_x && y == cursor_y) {
          tft.setTextColor(TFT_BLACK, TFT_GREEN);
        } else if (keyboard[keyboard_page][y][x] == '<') {
          tft.setTextColor(TFT_RED, TFT_BLACK);
        } else {
          tft.setTextColor(TFT_WHITE, TFT_BLACK);
        }
        tft.setCursor(10 + x * 30, 245 + y * 15);
        tft.print(keyboard[keyboard_page][y][x]);
      }
    }
    // Page indicator
    tft.setCursor(0, 320 - 10);
    tft.setTextColor(TFT_CYAN, TFT_BLACK);
    if (keyboard_page == 0) tft.print("L");
    else if (keyboard_page == 1) tft.print("N");
    else tft.print("E");
  }
}