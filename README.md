# RFID Attendance System with ESP8266

This project implements an RFID logging system using an ESP8266 microcontroller. It reads RFID card data, logs it to an SD card, and sends the information to a remote server for further processing and display.

## Features

- **RFID Card Reading**: Utilizes the MFRC522 library to read RFID cards.
- **Data Logging**: Logs RFID data with timestamps to an SD card.
- **WiFi Connectivity**: Connects to a WiFi network to send data to a remote server.
- **Real-Time Clock (RTC)**: Uses the DS3231 RTC module for accurate timestamping.
- **LCD Display**: Provides user feedback on an I2C LCD.

## Hardware Requirements

- ESP8266 (NodeMCU or similar)
- MFRC522 RFID reader
- DS3231 RTC module
- I2C LCD (16x2 or similar)
- SD card module
- Breadboard and jumper wires

## Pin Definitions

| Component       | ESP8266 Pin |
|------------------|-------------|
| MFRC522 RST      | D3          |
| MFRC522 SS       | D4          |
| SD Card CS       | D8          |
| I2C SDA          | D2          |
| I2C SCL          | D1          |

## Installation

### Libraries

Make sure to install the following libraries via the Arduino Library Manager:

- `SPI`
- `MFRC522`
- `Wire`
- `LiquidCrystal_I2C`
- `SD`
- `RTClib`
- `ESP8266WiFi`
- `ArduinoJson`

### Configuration

1. **WiFi Credentials**: Update the following lines in the code with your WiFi network credentials:
   ```cpp
   const char* ssid = "your_SSID";
   const char* password = "your_PASSWORD";
   const char* server = "your_api_endpoint"; // Replace with your server endpoint
   const int port = 80;  // your_api_port
   ```

2. **Server Endpoint**: Modify the endpoint in the `sendToServer` function to point to your server:
   ```cpp
   client.println("POST /your_mongodb_endpoint HTTP/1.1"); // Replace with your MongoDB endpoint
   ```

## Usage

1. **Upload the Code**: Load the code onto your ESP8266 using the Arduino IDE.
2. **Power the Circuit**: Connect your ESP8266 and other components as per the pin definitions.
3. **Open Serial Monitor**: Use the Serial Monitor to view debug information and connection status.
4. **Scan RFID Cards**: Present an RFID card to the reader. The UID will be logged to the SD card and sent to the server.

## Troubleshooting

- **SD Card Issues**: Ensure the SD card is properly formatted (FAT32) and connected correctly.
- **WiFi Connection Problems**: Verify that the SSID and password are correct. Check the signal strength of the WiFi network.
- **RTC Not Working**: Ensure the DS3231 module is connected correctly and has a backup battery if needed.

## License

This project is licensed under the MIT License. Feel free to modify and use it for your own projects!

## Acknowledgments

- [Arduino] for the platform.
- [ESP8266 Community] for support and resources.
- [ArduinoJson] for JSON handling.

## Collaborators

- **Richard Levi Akpeh**: Firmware Developer
- **Vem Rinji Silas**: Circuit Designer and Electronics Technician
- **Oluwapelumi Kadeba**: Full Stack Developer

---

Feel free to reach out if you have any questions or need further assistance!
```