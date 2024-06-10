#include <Wire.h>
#include <SPI.h>
#include <LoRa.h>

#define SLAVE_ADDRESS 0x08
#define LORA_SS 10    // Pin connected to LoRa module's NSS
#define LORA_RST 9    // Pin connected to LoRa module's RESET
#define LORA_DIO0 2   // Pin connected to LoRa module's DIO0

void setup() {
  Wire.begin(); // Initialize I2C as master
  Serial.begin(9600);
  while (!Serial);

  Serial.println("LoRa Receiver");
  
  // Setup LoRa transceiver module
  if (!LoRa.begin(868E6)) { // Initialize LoRa at 868 MHz
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

void loop() {
  // Try to parse packet
  String receivedMessage = "";

  // Check if there's a packet available to read
  if (LoRa.parsePacket()) {
    while (LoRa.available()) {
      // Read the packet, decode from binary to ASCII and display
      char incoming = (char)LoRa.read();
      Serial.print(incoming);
      receivedMessage += incoming;
    }
    // Print a new line after each received packet
    Serial.println();

    // Send the received message over I2C
    Wire.beginTransmission(SLAVE_ADDRESS); // Start I2C transmission to slave
    Wire.write(receivedMessage.c_str()); // Send the received message
    Wire.endTransmission(); // End transmission
    
    //Serial.println("Message sent over I2C: " + receivedMessage);
  }

  
}
