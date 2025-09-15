#include <SoftwareSerial.h>

// Alarm receiver's phone number with country code
const String PHONE = "+918780468884";

// GSM Module RX pin to Arduino 3
// GSM Module TX pin to Arduino 2
#define rxPin 2
#define txPin 3
SoftwareSerial sim800(rxPin, txPin);

// The pin that the PIR sensor is attached to
int pir_sensor = 11;

bool stopCall = false; // Flag to stop the call if "Ok" is received

void setup() {
  pinMode(pir_sensor, INPUT);    // Initialize sensor as an input
  Serial.begin(115200);          // Initialize serial communication

  sim800.begin(9600);            // Initialize GSM module communication
  Serial.println("SIM800L software serial initialized");

  // Check GSM module connection
  sim800.println("AT");
  delay(1000);
  if (sim800.available()) {
    Serial.println(sim800.readString());
  }
  
  // Set SMS mode to text
  sim800.println("AT+CMGF=1");
  delay(1000);
  Serial.println("Set SMS to text mode");

  // Enable new message notification
  sim800.println("AT+CNMI=1,2,0,0,0");
  delay(1000);
  Serial.println("Enabled new message notifications");
}

void loop() {
  // Read sensor value
  int val = digitalRead(pir_sensor);  
  if (val == HIGH) {                  // Check if motion is detected
    Serial.println("Motion detected!");

    // Send SMS alert
    sendSMS("Jay Sir, Animals detected on your farmland: nature's harmony at work!");
    
    // Wait for 1 minute (60000 ms) for "Ok" response
    stopCall = false; // Reset the stopCall flag
    waitForOKResponse();
    
    // Proceed with call if "Ok" is not received
    if (!stopCall) {
      Serial.println("Preparing to call...");
      makeCall();
    }
  }

  // Check for incoming SMS
  checkIncomingSMS();
}

// Function to send an SMS
void sendSMS(String message) {
  Serial.println("Sending SMS...");
  sim800.println("AT+CMGS=\"" + PHONE + "\""); // Specify the recipient
  delay(1000);
  sim800.println(message);                     // Message content
  delay(1000);
  sim800.write(26);                            // Ctrl+Z to send the SMS
  delay(5000);                                 // Wait for the SMS to send
  Serial.println("SMS sent.");
}

// Function to check incoming SMS
void checkIncomingSMS() {
  if (sim800.available()) {
    String response = sim800.readString();
    Serial.println("Received: " + response);

    // Check if the message contains "Ok"
    if (response.indexOf("Ok") >= 0) {
      Serial.println("Received 'Ok', stopping call.");
      stopCall = true;
    }
  }
}

// Function to wait for "Ok" response for 1 minute
void waitForOKResponse() {
  unsigned long startTime = millis();
  while (millis() - startTime < 30000) { // Wait for 1 minute
    checkIncomingSMS(); // Continuously check for the "Ok" SMS
    if (stopCall) {
      return; // Exit if "Ok" is received
    }
  }
  Serial.println("No 'Ok' received, proceeding with call.");
}

// Function to make a call
void makeCall() {
  sim800.println("ATD" + PHONE + ";");
  delay(1000);

  unsigned long callStart = millis();
  while (millis() - callStart < 20000) { // Keep the call active for 20 seconds
    checkIncomingSMS(); // Continuously check for the "Ok" SMS
    if (stopCall) {
      hangUpCall(); // Hang up if "Ok" is received
      return;
    }
  }

  hangUpCall(); // Hang up after 20 seconds if no "Ok" is received
}

// Function to hang up the call
void hangUpCall() {
  Serial.println("Hanging up the call...");
  sim800.println("ATH");
  delay(1000);
}
