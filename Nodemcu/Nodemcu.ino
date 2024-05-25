#include <ESP8266WiFi.h>          // Include the ESP8266 Wi-Fi library
#include <ESP8266HTTPClient.h>    // Include the ESP8266 HTTP client library
#include <EEPROM.h>               // Include the EEPROM library for non-volatile storage

// Pin definitions for UART communication
// A9  -> RX(node)
// A10 -> TX(node)

// Wi-Fi credentials
const char* ssid = "n";            // Wi-Fi SSID
const char* password = "77777777"; // Wi-Fi password

// URLs for different server endpoints
const char* serverUrl1 = "http://fota2024wt.atwebpages.com/read_and_erase.php"; // URL to read and erase data
const char* serverUrl2 = "http://fota2024wt.atwebpages.com/uploads/App_1.hex";  // URL to download App 1
const char* serverUrl3 = "http://fota2024wt.atwebpages.com/uploads/App_2.hex";  // URL to download App 2

// EEPROM configuration
const int EEPROM_SIZE = 512;       // EEPROM size

// GPIO pin definitions
const int buttonPin = D5;          // Button pin
const int resetpin = D7;           // Reset pin for App 1
const int resetpin2 = D4;          // Reset pin for App 2

// Control flags
bool ctrldataSent = false;         // Flag to track if control data has been sent
bool hexdataSent = false;          // Flag to track if firmware has been sent before

char global_chr = 0;               // Global character to track current command

// Function prototypes
void TakeAction(void);

void setup() 
{
  // Initialize GPIO pins
  pinMode(buttonPin, INPUT_PULLUP);    // Configure the button pin as input with pull-up resistor
  pinMode(resetpin, OUTPUT);           // Configure reset pin for App 1 as output
  pinMode(resetpin2, OUTPUT);          // Configure reset pin for App 2 as output
  digitalWrite(resetpin, HIGH);        // Set reset pin for App 1 high

  // Initialize serial communication
  Serial.begin(9600);                  // Begin serial communication at 9600 baud
  Serial1.begin(9600);                 // Begin secondary serial communication at 9600 baud
  delay(100);                          // Delay to ensure communication setup

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(100);                        // Wait until connected to Wi-Fi
  }

  // If control data hasn't been sent, fetch and send it
  if (!ctrldataSent) 
  {
    HTTPClient http;
    WiFiClient client;
    http.begin(client, serverUrl1);    // Initialize HTTP client with URL

    int httpCode = http.GET();         // Send GET request
    if (httpCode == HTTP_CODE_OK) 
    {
      String payload = http.getString(); // Get response payload
      if (!isEEPROMDataSame(payload))    // Check if new data is different from EEPROM data
      {
        for (char c : payload) 
        {
          global_chr = c;                // Update global character
        }
        saveToEEPROM(payload);           // Save new data to EEPROM
        ctrldataSent = true;             // Set control data sent flag
      } 
      else 
      {
        Serial.println("Data in EEPROM is the same as the new data. Not sending again.");
      }
    } 
    else 
    {
      Serial.print("Failed to download file. HTTP error code: ");
      Serial.println(httpCode);          // Print HTTP error code
    }

    http.end();                          // End HTTP client
  }
}

bool isEEPROMDataSame(String newData) 
{
  char storedData[EEPROM_SIZE]; // Buffer to hold data read from EEPROM
  EEPROM.begin(EEPROM_SIZE);    // Initialize EEPROM
  for (int i = 0; i < EEPROM_SIZE; ++i) 
  {
    storedData[i] = EEPROM.read(i); // Read data from EEPROM
  }
  EEPROM.end();                    // End EEPROM

  // Compare the stored data with the new data
  String storedDataStr(storedData);
  return storedDataStr == newData; // Return true if data is the same
}

void saveToEEPROM(String data) 
{
  // Write data to EEPROM
  EEPROM.begin(EEPROM_SIZE);      // Initialize EEPROM
  for (int i = 0; i < data.length(); ++i) 
  {
    EEPROM.write(i, data[i]);     // Write data to EEPROM
  }
  EEPROM.commit();                // Commit changes to EEPROM
  EEPROM.end();                   // End EEPROM
}

bool waitForOK() 
{
  const unsigned long timeout = 6000; // Timeout period
  unsigned long startTime = millis(); // Start time for timeout

  String response = "";               // Response buffer

  while (millis() - startTime < timeout) 
  {
    if (Serial.available()) 
    {
      char c = Serial.read();         // Read character from serial
      response += c;
      if (response.endsWith("ok")) 
      {
        return true;                  // Return true if "OK" received
      }
    }
  }
  Serial.println("Error: Timeout waiting for 'OK' response");
  return false;                       // Return false if timeout reached
}

void loop() 
{
  ctrldataSent = false;               // Reset control data sent flag
  hexdataSent = false;                // Reset hex data sent flag

  if (!ctrldataSent) 
  {
    HTTPClient http;
    WiFiClient client;
    http.begin(client, serverUrl1);   // Initialize HTTP client with URL

    int httpCode = http.GET();        // Send GET request
    if (httpCode == HTTP_CODE_OK) 
    {
      String payload = http.getString(); // Get response payload
      if (!isEEPROMDataSame(payload))    // Check if new data is different from EEPROM data
      {
        for (char c : payload) 
        {
          if(global_chr != c)
          {
            digitalWrite(resetpin,LOW); // Reset device to prepare for new command
            delay(700);
            digitalWrite(resetpin,HIGH);
            delay(100);
            Serial.write('S');          // Send 'S' command to indicate start
            global_chr = c;
            Serial.write(c);            // Send character
            delay(500);
            TakeAction();               // Take appropriate action based on command
          }
        }
        saveToEEPROM(payload);           // Save new data to EEPROM
        ctrldataSent = true;             // Set control data sent flag
      } 
      else 
      {
        Serial.println("Data in EEPROM is the same as the new data. Not sending again.");
      }
    } 
    else 
    {
      Serial.print("Failed to download file. HTTP error code: ");
      Serial.println(httpCode);          // Print HTTP error code
    }

    http.end();                          // End HTTP client
  }
}

void TakeAction(void)
{
  if(global_chr == 'A')
  {
    if (!hexdataSent) 
    {
      HTTPClient http;
      WiFiClient client;
      http.begin(client, serverUrl2);    // Initialize HTTP client with App 1 URL

      int httpCode = http.GET();         // Send GET request
      if (httpCode == HTTP_CODE_OK) 
      {
        String payload = http.getString(); // Get response payload
        if (!isEEPROMDataSame(payload))    // Check if new data is different from EEPROM data
        {
          for (char ch : payload) 
          {
            Serial.write(ch);             // Send character
            if (ch == '\n') 
            {
              if (!waitForOK())           // Wait for "OK" response
              {
                Serial.println("Failed to receive 'OK' response. Terminating...");
                return;                   // Terminate if "OK" not received
              }
            }
          }
          saveToEEPROM(payload);           // Save new data to EEPROM
          hexdataSent = true;              // Set hex data sent flag
        } 
        else 
        {
          Serial.println("Data in EEPROM is the same as the new data. Not sending again.");
        }
      } 
      else 
      {
        Serial.print("Failed to download file. HTTP error code: ");
        Serial.println(httpCode);          // Print HTTP error code
      }

      http.end();                          // End HTTP client
    }
  }
  else if(global_chr == 'B')
  {
    if (!hexdataSent) 
    {
      HTTPClient http;
      WiFiClient client;
      http.begin(client, serverUrl3);    // Initialize HTTP client with App 2 URL

      int httpCode = http.GET();         // Send GET request
      if (httpCode == HTTP_CODE_OK) 
      {
        String payload = http.getString(); // Get response payload
        if (!isEEPROMDataSame(payload))    // Check if new data is different from EEPROM data
        {
          for (char ch : payload) 
          {
            Serial.write(ch);             // Send character
            if (ch == '\n') 
            {
              if (!waitForOK())           // Wait for "OK" response
              {
                Serial.println("Failed to receive 'OK' response. Terminating...");
                return;                   // Terminate if "OK" not received
              }
            }
          }
          saveToEEPROM(payload);           // Save new data to EEPROM
          hexdataSent = true;              // Set hex data sent flag
        } 
        else 
        {
          Serial.println("Data in EEPROM is the same as the new data. Not sending again.");
        }
      } 
      else 
      {
        Serial.print("Failed to download file. HTTP error code: ");
        Serial.println(httpCode);          // Print HTTP error code
      }

      http.end();                          // End HTTP client
    }
  }
}
