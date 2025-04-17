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
#define V_MIDDLE 0   // 0.00V (ENTER)
#define V_LEFT 1227  // 1.0V
#define V_RIGHT 184  // 0.15V
#define V_PAGE 368   // 0.3V (assumed)
#define V_EMOJI 135  // 0.11V (UP+DOWN)
#define V_NONE 4095  // 3.3V (no press)

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
const uint32_t MY_CODE = 1234; // Unique 4-digit code
char recipient_code[5] = "0000"; // Recipient's code
char message[32] = ""; // Current message
char received_message[32] = ""; // Last received message
int cursor_x = 0, cursor_y = 0; // Keyboard cursor
int mode = 0; // 0: Select recipient, 1: Type message
int message_pos = 0; // Position in message
int keyboard_page = 0; // 0: Letters, 1: Numbers/Punc, 2: Emojis

void setup() {
  // Initialize Serial
  Serial.begin(115200);
  while (!Serial) {
    delay(10);
  }
  Serial.println("Starting chat device...");

  // Initialize TFT
  tft.init();
  tft.setRotation(1); // Landscape (240x320)
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
  radio.openWritingPipe((uint8_t*)recipient_code);
  radio.openReadingPipe(1, (uint8_t*)String(MY_CODE).c_str());
  radio.startListening();

  // Initialize ADC
  analogReadResolution(12);
  pinMode(ADC_PIN, INPUT);

  // Clear display and show initial screen
  updateDisplay();
}

void loop() {
  // Debug ADC reading
  int adc_value = analogRead(ADC_PIN);
  static int last_adc = -1;
  if (adc_value != last_adc) {
    Serial.println("ADC: " + String(adc_value));
    last_adc = adc_value;
  }

  // Handle button input
  int button = readButton();
  if (button != -1) {
    Serial.println("Button: " + String(button) + ", Mode: " + String(mode) + ", Page: " + String(keyboard_page));
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
    } else if (serial_input.length() > 0 && serial_input.length() < 32) {
      strncpy(message, serial_input.c_str(), sizeof(message) - 1);
      message[sizeof(message) - 1] = '\0';
      radio.stopListening();
      radio.write(message, sizeof(message));
      Serial.println("Sent via Serial: " + String(message));
      message[0] = '\0';
      message_pos = 0;
      radio.startListening();
      updateDisplay();
    }
  }

  // Check for incoming messages
  if (radio.available()) {
    radio.read(received_message, sizeof(received_message));
    Serial.println("Received: " + String(received_message));
    updateDisplay();
  }
}

// Read button based on ADC voltage
int readButton() {
  int adc_value = analogRead(ADC_PIN);
  if (abs(adc_value - V_UP) < 100) return 0; // UP
  if (abs(adc_value - V_DOWN) < 100) return 1; // DOWN
  if (abs(adc_value - V_MIDDLE) < 100) return 2; // MIDDLE (ENTER)
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
    if (button == 2) { // MIDDLE
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
    if (button == 2) { // MIDDLE (ENTER)
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
      } else if (message_pos > 0) { // Send message
        radio.stopListening();
        radio.write(message, sizeof(message));
        Serial.println("Sent via TFT: " + String(message));
        message[0] = '\0';
        message_pos = 0;
        radio.startListening();
      }
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
  else tft.println("UP/DOWN/LEFT/RIGHT:Move MIDDLE:Select/Send");

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
  tft.setCursor(0, 60);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.println(message);

  // Received message
  tft.setCursor(0, 100);
  tft.println(received_message);

  // Keyboard (in typing mode)
  if (mode == 1) {
    tft.setTextSize(1);
    Serial.println("Rendering keyboard, Page: " + String(keyboard_page));
    for (int y = 0; y < 5; y++) {
      for (int x = 0; x < 5; x++) {
        if (keyboard[keyboard_page][y][x] == ' ') continue;
        if (x == cursor_x && y == cursor_y) {
          tft.setTextColor(TFT_BLACK, TFT_GREEN);
        } else {
          tft.setTextColor(TFT_WHITE, TFT_BLACK);
        }
        tft.setCursor(10 + x * 30, 140 + y * 15);
        tft.print(keyboard[keyboard_page][y][x]);
      }
    }
    // Page indicator
    tft.setCursor(0, 225);
    tft.setTextColor(TFT_CYAN, TFT_BLACK);
    if (keyboard_page == 0) tft.print("L");
    else if (keyboard_page == 1) tft.print("N");
    else tft.print("E");
  }
}