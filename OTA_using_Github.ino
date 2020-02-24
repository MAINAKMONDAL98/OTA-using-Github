#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
String payload,cstr;

const String FirmwareVer={"1.1"};//change the latest version
//#define URL_fw_Version "https://raw.githubusercontent.com/programmer131/otaFiles/master/version.txt"
//#define URL_fw_Bin "https://raw.githubusercontent.com/programmer131/otaFiles/master/firmware.bin"
#define URL_fw_Version "https://raw.githubusercontent.com/MAINAKMONDAL98/OTA-using-Github/master/version.txt"
#define URL_fw_Bin "https://raw.githubusercontent.com/MAINAKMONDAL98/OTA-using-Github/master/OTA_USING_GITHUB_BLINK.bin"
//#define URL_fw_Version "http://cade-make.000webhostapp.com/version.txt"
//#define URL_fw_Bin "http://cade-make.000webhostapp.com/firmware.bin"
HTTPClient http;
 
const char* ssid = "ding";
const char* password = "12345678";
 
void FirmwareUpdate()
{
  http.begin(URL_fw_Version,"CC AA 48 48 66 46 0E 91 53 2C 9C 7C 23 2A B1 74 4D 29 9D 33");     // check version URL
  delay(100);
  int httpCode = http.GET();            // get data from version file
  delay(100);
  
  if (httpCode == HTTP_CODE_OK)         // if version received
  {
    payload = http.getString();  // save received version
    Serial.println(payload);
  }
  else
  {
    Serial.print("error in downloading version file:");
    Serial.println(httpCode);

  }
  
  http.end();
  if (httpCode == HTTP_CODE_OK)         // if version received
  {
  if(payload==cstr) 
  {   
     Serial.println("Device already on latest firmware version"); 
  }
  else
  {  cstr=payload;
     Serial.println(cstr);
     Serial.println("New firmware detected");
     WiFiClient client;

    // The line below is optional. It can be used to blink the LED on the board during flashing
    // The LED will be on during download of one buffer of data from the network. The LED will
    // be off during writing that buffer to flash
    // On a good connection the LED should flash regularly. On a bad connection the LED will be
    // on much longer than it will be off. Other pins than LED_BUILTIN may be used. The second
    // value is used to put the LED on. If the LED is on with HIGH, that value should be passed
    ESPhttpUpdate.setLedPin(LED_BUILTIN, LOW);


    t_httpUpdate_return ret = ESPhttpUpdate.update(URL_fw_Bin,"","CC AA 48 48 66 46 0E 91 53 2C 9C 7C 23 2A B1 74 4D 29 9D 33");//if the binary is http, then you dont have finger print key
    
    switch (ret) {
      case HTTP_UPDATE_FAILED:
        Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s\n", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
        cstr="00";
        break;

      case HTTP_UPDATE_NO_UPDATES:
        Serial.println("HTTP_UPDATE_NO_UPDATES");
        break;

      case HTTP_UPDATE_OK:
        Serial.println("HTTP_UPDATE_OK");
        break;
    } 
  }
 }  
}

unsigned long previousMillis = 0;        // will store last time LED was updated
const long interval = 30000;

 void repeatedCall(){
    unsigned long currentMillis = millis();
    if ((currentMillis - previousMillis) >= interval) 
    {
      // save the last time you blinked the LED
      previousMillis = currentMillis;
      FirmwareUpdate();
    }
 }

  
void setup()
{
  Serial.begin(115200);
  Serial.println("");
  Serial.println("Start");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print("O");
  }                                   
  Serial.println("Connected to WiFi");
  pinMode(LED_BUILTIN, OUTPUT);
}
void loop()
{
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(5000);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(5000);                       // wait for a second
  repeatedCall();    
}
