#include <Wire.h>
#include "Audio.h"

#define I2S_DOUT      25
#define I2S_BCLK      27
#define I2S_LRC       26

Audio audio;

// Buffer to store received data
String receivedData = "";

void setup() {
  // Initialize Serial communication for debugging (USB Serial Monitor)
  Serial.begin(115200);

  // Initialize I2C communication (ESP32 acts as Slave)
  Wire.begin(8);  // Set the ESP32's I2C address to 8
  Wire.onReceive(receiveEvent);  // Register receive event handler

  // Connect to Wi-Fi
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  WiFi.begin("Dhruvsiphone", "88048123");

  while (WiFi.status() != WL_CONNECTED) {
    delay(1500);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi!");

  // Initialize audio
  audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
  audio.setVolume(100);  // Set volume to maximum

  Serial.println("ESP32 I2C Slave Ready to Receive Data!");
}

void loop() {
  // Process the received data
  if (receivedData.length() > 0) {
    Serial.print("Received: ");
    Serial.println(receivedData);

    // Convert the received text to speech
    audio.connecttospeech(receivedData.c_str(), "en");  // Convert text to speech

    // Clear the received data buffer
    receivedData = "";
  }

  // Handle audio playback
  audio.loop();
}

// I2C receive event handler
void receiveEvent(int numBytes) {
  receivedData = "";  // Clear the buffer
  while (Wire.available()) {
    char c = Wire.read();  // Read each byte
    receivedData += c;     // Append to the string
  }
}

// Audio info callback
void audio_info(const char *info) {
  Serial.print("audio_info: ");
  Serial.println(info);
}