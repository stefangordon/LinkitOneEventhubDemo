#include <LWiFi.h>
#include <LWiFiClient.h>
#include <LGPS.h>

// WIFI
#define WIFI_AP "woot"
#define WIFI_PWD "w00tw00t"

// EVENT HUB
#define DEVICEID "l1device" //How do we identify this device
#define HOST "l1demo.azurewebsites.net" //The hostname of our gateway
#define SAS "SharedAccessSignature sr=https%3a%2f%2fl1demo.servicebus.windows.net%2fhub%2fpublishers%2fl1device%2fmessages&sig=pnSv2hfbmviD9OOW2t3tU4F2BAKxB6ln2WriR9qCMmQ%3d&se=1466746623&skn=l1device" // Our Shared access signature for this device

// GLOBAL VARIABLES
int LoopCounter;
LWiFiClient Client;
gpsSentenceInfoStruct GpsInfo;

// The setup function runs once during boot
void setup() {
  LoopCounter = 0;

  // Initialize serial, wifi and GPS
  Serial.begin(9600);
  LWiFi.begin();
  LGPS.powerOn();

  // Connect to WPA WIFI, swap with one of the commented methods for WEP or Unencrypted
  // LWiFi.connect(WIFI_AP);  if the AP is not encrypted
  // LWiFi.connectWEP(WIFI_AP, WIFI_PWD);  if the AP uses WEP encryption
  if(LWiFi.connectWPA(WIFI_AP, WIFI_PWD) < 0)
  {
    Serial.println("Failed to connect to WIFI");
    return;
  }
  Serial.println("WIFI Connected OK");

}

// Loop methods is called continously
// We simulate a 'fast loop' and a 'slow loop'
// using a counter as a simpler alternative to
// a timer interrupt.
void loop() {
  // Fast Loop (100ms)  
  // Prints any HTTP responses to serial output
  int v;
   while(Client.available())
   {
     v = Client.read();
     if(v < 0)
     break;
     Serial.print((char)v);
   }
   delay(100);

   // Slow Loop (5000ms)
   // Posts data to Eventhub
   LoopCounter++;
   if (LoopCounter > 50)
   {
      LoopCounter = 0;
      PostGPS();
      PostADC();
   }
}

// Post a GPS JSON message to EventHub
void PostGPS()
{
  LGPS.getData(&GpsInfo);
  String gpsResult = GetGPSResult((char*)GpsInfo.GPGGA);

  PostData(String(HOST), String(ENDPOINT), gpsResult, String(SAS));
}

// Post an Analog reading from the ADC to EventHub
void PostADC()
{
  PostData(String(HOST), String(ENDPOINT), GetADCResult(), String(SAS));
}

// Posts a message to EventHub
// This method talks to our Azure Gateway because the current public
// firmware does not support HTTPS (TLS/SSL).
// If using an SDK with SSL support you can directly call Eventhub instead
int PostData(String host, String endpoint, String data, String sas)
{
  Serial.print("Connecting to " + host);
  if(!Client.connect(host.c_str(), 80))
  {
    Serial.println("Failed to connect to " + host);
    return -1;
  }

  Serial.println("Transmitting HTTP Post");

  Client.println("POST /data/" + deviceID + " HTTP/1.1");
  Client.println("Host: " + host);
  Client.println("Content-Type: application/x-www-form-urlencoded");
  Client.println("Authorization: " + sas);
  Client.println("Content-Length: " + String(data.length()));
  Client.println();
  Client.println(data); 

  return 0;
}

/* ******* GPS Methods ********* */

const char *nextToken(const char* src, char* buf){
   int i = 0;
   while(src[i] != 0 && src[i] != ',')
     i++;

    if(buf){
       strncpy(buf, src, i);
       buf[i] = 0;
 }
 if(src[i])
 i++;
 return src+i;
}

String GetGPSResult(const char* str)
{
  char latitude[20];
  char longitude[20];
  char buf[20];
  const char* p = str;

  p = nextToken(p, 0); // GGA
  p = nextToken(p, 0); // Time
  p = nextToken(p, latitude); // Latitude
  p = nextToken(p, 0); // N
  p = nextToken(p, longitude); // Longitude
  p = nextToken(p, 0); // E
  p = nextToken(p, buf); // fix quality

  String successResult = "={ \"messageType\": \"GPS\", \"deviceID\": \"{did}\", \"fixed\": true, \"latitude\": {lat}, \"longitude\": {lon}, \"satellites\": {sat}}";
  String errorResult = "={ \"messageType\": \"GPS\", \"deviceID\": \"{did}\", \"fixed\": false}";

  if(buf[0] == '1')
  {
    successResult.replace("{did}", DEVICEID);    
    p = nextToken(p, buf); // number of satellites
    successResult.replace("{sat}", buf);
    successResult.replace("{lat}", latitude);
    successResult.replace("{lon}", longitude);

    return successResult;
  }
  else
  {
    errorResult.replace("{did}", DEVICEID);  
    return errorResult;
  }
}

/* ******* Analog Read Methods ********* */

// Assumes a voltage between 0 and 5v on A0/GPIO0
String GetADCResult()
{
  String successResult = "={ \"messageType\": \"ADC\", \"deviceID\": \"{did}\", \"value\": {val} }";

  successResult.replace("{did}", DEVICEID);    
  successResult.replace("{val}", String(analogRead(A0)));

  return successResult; 
}
