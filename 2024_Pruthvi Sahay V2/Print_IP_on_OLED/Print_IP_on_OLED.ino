#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>

// Define I2C pins
#define I2C_SDA 15
#define I2C_SCL 14
TwoWire I2Cbus = TwoWire(0);

// Display defines
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &I2Cbus, OLED_RESET);

// WiFi credentials
const char* ssid = "Oneplus8";        // Replace with your WiFi SSID
const char* password = "12345678"; // Replace with your WiFi Password

// Function to display text on OLED
void displayText(String text) {
  display.clearDisplay();
  display.setCursor(0, 10);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.print(text);
  display.display();
}

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);

  // Initialize I2C with defined SDA and SCL pins
  I2Cbus.begin(I2C_SDA, I2C_SCL, 100000);
  
  // Initialize OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println("SSD1306 OLED display failed to initialize");
    while (true); // Stop the program here if display initialization fails
  }

  // Display initialization message
  displayText("Connecting to WiFi...");
  delay(1000);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // WiFi connected
  Serial.println();
  Serial.print("Connected to WiFi. IP Address: ");
  Serial.println(WiFi.localIP());

  // Display IP address on OLED
  displayText("IP Address: ");
  displayText(WiFi.localIP().toString()); // Print the IP address to OLED
  delay(5000); // Display IP for 5 seconds
}

void loop() {
  // You can add additional functionality here if needed
}
