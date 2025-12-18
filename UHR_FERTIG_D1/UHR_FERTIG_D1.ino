//Important this programm is made for three hexagonal shapes, which need so much power, that 
// a external Powersupply is necessary
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <NTPClient.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include "web_html.h"
#include "style_css.h"
#include <WiFiManager.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFiMulti.h>
#include "WebSocketsServer.h"
ESP8266WebServer server(80);
ESP8266WiFiMulti wifiMulti;
#include <Adafruit_NeoPixel.h>
#define PIN        D1
#define NUMPIXELS 111 // Popular NeoPixel ring size
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
//WLAN Zugangsdaten
const char* ssid1="";
const char* pw1="";
const char* ssid2="";
const char* pw2="";
const char* ssid3="";
const char* pw3="";
//Variablen
int Helligkeit=5;
int Frequenz =10;
int Muster = 0;
unsigned long millis_alt = 0;
int iSpalte = 0;
int iReihe = 1;
long Intervall = 100;
int oldSekunde = 0;
unsigned long erste_Zuschaltung = 1000;
unsigned long zweite_Zuschaltung = 2000;
unsigned long dritte_Zuschaltung = 3000;
int Mode = 0;
int Mode_alt = 0;
int Blinker = 0;
#define ANZAHL_MODI 10
String sModeText[ANZAHL_MODI] = {
  {"Alle"},
  {"<-->"},
  {">--<"},
  {"-->"},
  {"<--"},
  {"^--v"},
  {"v--^"},
  {"--v"},
  {"^--"},
  {"Uhr"}
};

//Zeitverschiebung UTC <-> MEZ (Winterzeit) = 3600 Sekunden (1 Stunde)
//Zeitverschiebung UTC <-> MEZ (Sommerzeit) = 7200 Sekunden (2 Stunden)
const long utcOffsetInSeconds = 3600;


WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

int iRed1 = 125, iGreen1 = 125, iBlue1 = 125;
int iRed2 = 125, iGreen2 = 125, iBlue2 = 125;
int iRed3 = 125, iGreen3 = 125, iBlue3 = 125;
int iRedg =125 , iGreeng =125 , iBlueg =125 ;
int Hauptknopfwert;
bool bColorSet = false;

byte bOben_Unten[39][4] = {
  {21, 21, 21, 21},
  {58, 58, 58, 58},
  {95, 95, 95, 95},
  {14, 14, 27, 27},
  {51, 51, 64, 64},
  {88, 88, 101, 101},
  {8, 8, 20, 32},
  {45, 45, 57, 69},
  {82, 82, 94, 106 },
  {3, 13, 26, 36},
  {40, 50, 63, 73},
  {77, 87, 100, 110},
  {7, 7, 19, 31 },
  {44, 44, 56, 68},
  {81, 81, 93, 105},
  {2, 12, 25, 35},
  {39, 49, 62, 72},
  {76, 86, 99, 109},
  {6, 6, 18, 30 },
  {43, 43, 55, 67},
  {80, 80, 92, 104},
  {1, 11, 24, 34},
  {38, 48, 61, 71},
  {75, 85, 98, 108},
  {5, 5, 17, 29 },
  {42, 42, 54, 66},
  {79, 79, 91, 103},
  {0, 10, 23, 33},
  {37, 47, 60, 70},
  {74, 84, 97, 107},
  {4, 4, 16, 28 },
  {41, 41, 53, 65},
  {78, 78, 90, 102},
  {9, 9, 22, 22},
  {46, 46, 59, 59},
  {83, 83, 96, 96},
  {15, 15, 15, 15},
  {52, 52, 52, 52},
  {89, 89, 89, 89},
};
byte bLinks_Rechts[21][7] = {
  {33, 34, 35, 36, 36, 36, 36},
  {28, 29, 30, 31, 32, 32, 32},
  {22, 23, 24, 25, 26, 27, 27},
  {15, 16, 17, 18, 19, 20, 21},
  {9, 10, 11, 12, 13, 14, 14},
  {4, 5, 6, 7, 8, 8, 8},
  {0, 1, 2, 3, 3, 3, 3},
  {70, 71, 72, 73, 73, 73, 73},
  {65, 66, 67, 68, 69, 69, 69},
  {59, 60, 61, 62, 63, 64, 64},
  {52, 53, 54, 55, 56, 57, 58},
  {46, 47, 48, 49, 50, 51, 51},
  {41, 42, 43, 44, 45, 45, 45},
  {37, 38, 39, 40, 40, 40, 40},
  {107, 108, 109, 110, 110, 110, 111},
  {102, 103, 104, 105, 106, 106, 106},
  {96, 97, 98, 99, 100, 101, 101},
  {90, 90, 91, 92, 93, 94, 95},
  {83, 84, 85, 86, 87, 88, 89},
  {78, 79, 80, 81, 82, 82, 82},
  {74, 74, 75, 76, 76, 77, 77},
};

byte bZifferEiner[10][11] = {
  {0, 1, 2, 3, 4, 8, 10, 11, 12, 13, 13},   // 0
  {4, 5, 6, 7, 8, 10, 10, 10, 10, 10, 10},
  {0, 1, 3, 4, 6, 8, 10, 12, 13, 13, 13},
  {0, 1, 2, 3, 4, 6, 8, 10, 13, 13, 13},
  {0, 1, 2, 3, 6, 10, 11, 11, 11, 11, 11},
  {0, 2, 3, 4, 6, 8, 10, 11, 13, 13, 13},
  {0, 2, 3, 4, 6, 8, 10, 11, 12, 13, 13},
  {0, 1, 2, 3, 4, 10, 11, 11, 11, 11, 11},
  {0, 1, 2, 3, 4, 6, 8, 10, 11, 12, 13},
  {0, 1, 2, 3, 4, 6, 8, 10, 11, 13, 13},
};

byte bZifferZehner[6][10] = {
  {23, 24, 25, 26, 28, 32, 33, 34, 35, 36},
  {28, 29, 30, 31, 32, 33, 33, 33, 33, 33},
  {23, 24, 26, 28, 30, 32, 33, 35, 36, 36},
  {23, 24, 25, 26, 28, 30, 32, 33, 36, 36},
  {23, 24, 25, 26, 30, 33, 34, 34, 34, 34},
  {23, 25, 26, 28, 30, 32, 33, 34, 36, 36},
};
void dispEiner(int dispNr, int dispZiffer) {
  for (int i = 0; i < 11; i++) {
    switch (dispNr) {
      case 0:
        //pixels.setPixelColor(bZifferEiner[dispZiffer][i] + dispNr * 37, 240 - 4 * timeClient.getSeconds(), 0, 4 * timeClient.getSeconds());
        pixels.setPixelColor(bZifferEiner[dispZiffer][i] + dispNr * 37, pixels.Color(iRed1, iGreen1, iBlue1));
        break;
      case 1:
        //pixels.setPixelColor(bZifferEiner[dispZiffer][i] + dispNr * 37, 240 - 4 * timeClient.getSeconds(), 0, 4 * timeClient.getSeconds());
        pixels.setPixelColor(bZifferEiner[dispZiffer][i] + dispNr * 37, pixels.Color(iRed2, iGreen2, iBlue2));
        break;
      case 2:
        //pixels.setPixelColor(bZifferEiner[dispZiffer][i] + dispNr * 37, 240 - 4 * timeClient.getSeconds(), 0, 4 * timeClient.getSeconds());
        pixels.setPixelColor(bZifferEiner[dispZiffer][i] + dispNr * 37, pixels.Color(iRed3, iGreen3, iBlue3));
        break;
    }
  }
}

void dispZehner(int dispNr, int dispZiffer) {
  switch (dispNr) {
    case 0:
      for (int i = 0; i < 10; i++) {
        pixels.setPixelColor(bZifferZehner[dispZiffer][i] + dispNr * 37, pixels.Color(iRed1, iGreen1, iBlue1));
      }
      break;
    case 1:
      for (int i = 0; i < 10; i++) {
        pixels.setPixelColor(bZifferZehner[dispZiffer][i] + dispNr * 37, pixels.Color(iRed2, iGreen2, iBlue2));
      }
      break;
    case 2:
      for (int i = 0; i < 10; i++) {
        pixels.setPixelColor(bZifferZehner[dispZiffer][i] + dispNr * 37, pixels.Color(iRed3, iGreen3, iBlue3));
      }
      break;
  }
}


WebSocketsServer webSocket = WebSocketsServer(81);
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  switch (type) {
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n", num);
      break;
    case WStype_CONNECTED: {
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);

        // send message to client
        webSocket.sendTXT(num, "Connected");
        char dataString[20] = {0};
        sprintf(dataString, "#1%02X%02X%02X", iRed1, iGreen1, iBlue1);
        webSocket.sendTXT(num, dataString);
        sprintf(dataString, "#2%02X%02X%02X", iRed2, iGreen2, iBlue2);
        webSocket.sendTXT(num, dataString);
        sprintf(dataString, "#3%02X%02X%02X", iRed3, iGreen3, iBlue3);
        webSocket.sendTXT(num, dataString);
        sprintf(dataString, "#g%02X%02X%02X", iRedg, iGreeng, iBlueg);
        webSocket.sendTXT(num, dataString);
                sprintf(dataString, "!H%03d", Helligkeit);
        webSocket.sendTXT(num, dataString);
                sprintf(dataString, "!F%03d",Frequenz);
        webSocket.sendTXT(num, dataString);
        sprintf(dataString, "%s", sModeText[Mode]);
        webSocket.sendTXT(num, dataString);

      }
      break;
    case WStype_TEXT:
      //Serial.printf("[%u] get Text: %s\n", num, payload);
      if (payload[0] == '#') {
        if (payload[1] == 'g') {
          yield();
          // we get RGB data
          // decode rgb data
          uint32_t rgbg = (uint32_t) strtol((const char *) &payload[2], NULL, 16);

          iRedg = (rgbg >> 16) & 0xFF;
          iGreeng = (rgbg >> 8) & 0xFF;
          iBlueg = (rgbg >> 0) & 0xFF;
          iRed1 = iRedg;
          iGreen1 = iGreeng;
          iBlue1 = iBlueg;
          iRed2 = iRedg;
          iGreen2 = iGreeng;
          iBlue2 = iBlueg;
          iRed3 = iRedg;
          iGreen3 = iGreeng;
          iBlue3 = iBlueg;
          bColorSet = true;

          for (uint8_t i = 0; i < WEBSOCKETS_SERVER_CLIENT_MAX; i++) {
            if (i != num) {
              webSocket.sendTXT(i, payload);
            }
          }
        }
        if (payload[1] == '1') {
          yield();
          // we get RGB data

          // decode rgb data
          uint32_t rgb1 = (uint32_t) strtol((const char *) &payload[2], NULL, 16);

          iRed1 = (rgb1 >> 16) & 0xFF;
          iGreen1 = (rgb1 >> 8) & 0xFF;
          iBlue1 = (rgb1 >> 0) & 0xFF;
          bColorSet = true;

          for (uint8_t i = 0; i < WEBSOCKETS_SERVER_CLIENT_MAX; i++) {
            if (i != num) {
              webSocket.sendTXT(i, payload);
            }
          }
        }
        if (payload[1] == '2') {
          yield();
          // we get RGB data

          // decode rgb data
          uint32_t rgb2 = (uint32_t) strtol((const char *) &payload[2], NULL, 16);

          iRed2 = (rgb2 >> 16) & 0xFF;
          iGreen2 = (rgb2 >> 8) & 0xFF;
          iBlue2 = (rgb2 >> 0) & 0xFF;
          bColorSet = true;

          for (uint8_t i = 0; i < WEBSOCKETS_SERVER_CLIENT_MAX; i++) {
            if (i != num) {
              webSocket.sendTXT(i, payload);
            }
          }
        }

        if (payload[1] == '3') {
          yield();
          // we get RGB data

          // decode rgb data
          uint32_t rgb3 = (uint32_t) strtol((const char *) &payload[2], NULL, 16);

          iRed3 = (rgb3 >> 16) & 0xFF;
          iGreen3 = (rgb3 >> 8) & 0xFF;
          iBlue3 = (rgb3 >> 0) & 0xFF;
          bColorSet = true;

          for (uint8_t i = 0; i < WEBSOCKETS_SERVER_CLIENT_MAX; i++) {
            if (i != num) {
              webSocket.sendTXT(i, payload);
            }
          }
        }
      }      if (payload[0] == '!' && payload[1] == 'F') {
        uint32_t FQZ = (uint32_t) strtol((const char *) &payload[2], NULL, 10);
        Intervall = 1000 / (int) FQZ;
        Frequenz=FQZ;

        for (uint8_t i = 0; i < WEBSOCKETS_SERVER_CLIENT_MAX; i++) {
          if (i != num) {
            webSocket.sendTXT(i, payload);
          }
        }
      }
      if (payload[0] == '!' && payload[1] == 'B') {
        if (Blinker == 0) {
          Blinker = 1;
         webSocket.sendTXT(num, "!BFrequenzOn");
        }
        else {
          Blinker = 0;
          webSocket.sendTXT(num, "!BFrequenzOff");
        }
      }

      if (payload[0] == '!' && payload[1] == 'p') {

        if (Muster == 0) {
          Muster = 1;
        }
        else {
          Muster = 0;
        }
      }


      if (payload[0] == '!' && payload[1] == 'M')
      {
        Mode_alt = Mode;
        Mode++;
        pixels.clear();
        if (Mode == ANZAHL_MODI) {
          Mode = 0;
        }

        for (uint8_t i = 0; i < WEBSOCKETS_SERVER_CLIENT_MAX; i++) {
          //if (i != num) {
          char dataString[20] = {0};
          sprintf(dataString, "!M%s", sModeText[Mode]);
          webSocket.sendTXT(i, dataString);
          sprintf(dataString, "$%s", sModeText[Mode_alt]);
          webSocket.sendTXT(i, dataString);
          // }
        }
      }
      if(payload[0] == '!' && payload[1] == 'H') {
      uint32_t Brightness = (uint32_t) strtol((const char *) &payload[2], NULL, 10);
       pixels.setBrightness(Brightness);
       Serial.println(Brightness);
       Helligkeit=Brightness;
                for (uint8_t i = 0; i < WEBSOCKETS_SERVER_CLIENT_MAX; i++) {
            if (i != num) {
              webSocket.sendTXT(i, payload);
            }
          }
      }
      break;
  }
}
void setup() {


  Serial.begin(115200);


  server.begin();
  pixels.begin();
  pixels.setBrightness(5);

  Serial.begin(115200);
wifiMulti.addAP(ssid2, pw2);
wifiMulti.addAP(ssid3, pw3);
WiFi.begin(ssid1, pw1);
  WiFi.setSleepMode(WIFI_NONE_SLEEP);

  millis_alt = millis();
  while (wifiMulti.run() != WL_CONNECTED) {
    Serial.print("");
   // while (WiFi.status() != WL_CONNECTED) {
    if (millis() > erste_Zuschaltung) {
      WiFi.begin(ssid2, pw2);
    }
  
  
   if (millis() > zweite_Zuschaltung) {
        WiFi.begin(ssid3, pw3);
    }
    if (millis() > dritte_Zuschaltung) {
   WiFiManager wm;
   bool res;
   res = wm.autoConnect("AutoConnectAP", "password"); // password protected ap
  if (!res) 
  Serial.println("Failed to connect");
       // ESP.restart();
    }
    else {
      //if you get here you have connected to the WiFi
      Serial.println("connected...yeey :)");

    }
   WiFi.mode(WIFI_AP); // explicitly set mode, esp defaults to STA+AP
    WiFi.begin("AutoConnectAP", "password");
      WiFi.setSleepMode(WIFI_NONE_SLEEP);
  }


  Serial.println("");
  Serial.print("Connected to ");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  server.on("/", [] {
    server.send(200, "text/html", INDEX_HTML);
  });

  server.on("/style.css", [] {
    server.send(200, "text/css", STYLE_CSS);
  });
  // start webSocket server
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  if (MDNS.begin("uhrprojekt")) {
    Serial.println("MDNS responder started");
    timeClient.begin();
      WiFi.setSleepMode(WIFI_NONE_SLEEP);
  }
}

void loop() {
  timeClient.update();
    MDNS.update();
  server.handleClient();
  webSocket.loop();

  if (Mode == 0) {
    if (oldSekunde != timeClient.getSeconds() || bColorSet) {
      bColorSet = false;
      oldSekunde = timeClient.getSeconds();
      pixels.clear();
      //Serial.print(daysOfTheWeek[timeClient.getDay()]);
      //Serial.print(", ");
      //Serial.println(timeClient.getFormattedTime());
      byte SekEiner = timeClient.getSeconds() % 10;
      byte SekZehner = (timeClient.getSeconds() / 10) % 10;
      byte MinEiner = timeClient.getMinutes() % 10;
      byte MinZehner = (timeClient.getMinutes() / 10) % 10;
      byte StuEiner = timeClient.getHours() % 10;
      byte StuZehner = (timeClient.getHours() / 10) % 10;
      dispEiner(0, StuEiner);
      dispEiner(1, MinEiner);
      dispEiner(2, SekEiner);
      dispZehner(0, StuZehner);
      dispZehner(1, MinZehner);
      dispZehner(2, SekZehner );
      yield();
      pixels.show();
    }
  }

  if (Mode == 1) {
    if (millis() - millis_alt > Intervall ) {
      if (Blinker == 1) {
        pixels.clear();
        pixels.show();
      }
    }
    if (millis() - millis_alt > Intervall*2) {
      millis_alt = millis();
      pixels.fill(pixels.Color(iRed3, iGreen3, iBlue3), 74, 37);
      pixels.fill(pixels.Color(iRed2, iGreen2, iBlue2), 37, 37);
      pixels.fill(pixels.Color(iRed1, iGreen1, iBlue1), 0, 37);
      pixels.show();
    }
  }

  if (Mode == 2 ) {

    //  void Links_Rechts(int dispNr, int k) {
    //for (int i = 0; i < 11; i++) {
    if (millis() - millis_alt > Intervall) {
      millis_alt = millis();
      iSpalte++;
      if (iSpalte > 10) {
        iSpalte = 0;
        pixels.clear();
      } if (Muster == 1) {
        pixels.clear();
      }
      for (int k = 0; k < 7; k++) {

        if (bLinks_Rechts[10 + iSpalte][k] > 73) {
          pixels.setPixelColor(bLinks_Rechts[10 + iSpalte][k] , pixels.Color(iRed3, iGreen3, iBlue3));
        }
        if (bLinks_Rechts[10 + iSpalte][k] < 74) {
          pixels.setPixelColor(bLinks_Rechts[10 + iSpalte][k] , pixels.Color(iRed2, iGreen2, iBlue2));
        }
        if (bLinks_Rechts[10 - iSpalte][k] > 36 ) {
          //Links-->rechts}
          pixels.setPixelColor((bLinks_Rechts[10 - iSpalte][k]), pixels.Color(iRed2, iGreen2, iBlue2));
        }
        if (bLinks_Rechts[10 - iSpalte][k] < 37 ) {
          //Links-->rechts}
          pixels.setPixelColor((bLinks_Rechts[10 - iSpalte][k]), pixels.Color(iRed1, iGreen1, iBlue1));
        }
      }
      pixels.show();

    }
  }
  if (Mode == 3) {

    //  void Links_Rechts(int dispNr, int k) {
    //for (int i = 0; i < 11; i++) {
    if (millis() - millis_alt > Intervall) {
      millis_alt = millis();
      iSpalte++;
      if (iSpalte > 10) {
        iSpalte = 0;
        pixels.clear();
      } if (Muster == 1) {
        pixels.clear();
      }
      for (int k = 0; k < 7; k++) {

        if (bLinks_Rechts[20 - iSpalte][k] > 73) {
          pixels.setPixelColor(bLinks_Rechts[20 - iSpalte][k] , pixels.Color(iRed3, iGreen3, iBlue3));
        }
        if (bLinks_Rechts[20 - iSpalte][k] > 36 && bLinks_Rechts[20 - iSpalte][k] < 74) {
          pixels.setPixelColor(bLinks_Rechts[20 - iSpalte][k] , pixels.Color(iRed2, iGreen2, iBlue2));
        }

        if (bLinks_Rechts[iSpalte][k] > 36 && bLinks_Rechts[iSpalte][k] < 74) {
          //Links-->rechts}
          pixels.setPixelColor((bLinks_Rechts[iSpalte][k]), pixels.Color(iRed2, iGreen2, iBlue2));
        }
        if (bLinks_Rechts[iSpalte][k] < 37 ) {
          //Links-->rechts}
          pixels.setPixelColor((bLinks_Rechts[iSpalte][k]), pixels.Color(iRed1, iGreen1, iBlue1));
        }
      }
    } pixels.show();
  }

  if (Mode == 4) {

    if (millis() - millis_alt > Intervall) {
      millis_alt = millis();
      iSpalte++;
      if (iSpalte > 20) {
        iSpalte = 0;
        pixels.clear();
      } if (Muster == 1) {
        pixels.clear();
      }
      for (int k = 0; k < 7; k++) {
        if (bLinks_Rechts[iSpalte][k] > 73) {
          //Links-->rechts}
          pixels.setPixelColor((bLinks_Rechts[iSpalte][k]), pixels.Color(iRed3, iGreen3, iBlue3));
        }
        if (bLinks_Rechts[iSpalte][k] > 36 && bLinks_Rechts[iSpalte][k] < 74) {
          //Links-->rechts}
          pixels.setPixelColor((bLinks_Rechts[iSpalte][k]), pixels.Color(iRed2, iGreen2, iBlue2));
        }
        if ( bLinks_Rechts[iSpalte][k] < 37 ) {
          //Links-->rechts}
          pixels.setPixelColor((bLinks_Rechts[ iSpalte][k]), pixels.Color(iRed1, iGreen1, iBlue1));
        }
      } pixels.show();
    }
  }

  if (Mode == 5) {

    if (millis() - millis_alt > Intervall) {
      millis_alt = millis();
      iSpalte++;
      if (iSpalte > 20) {
        iSpalte = 0;
        pixels.clear();
      } if (Muster == 1) {
        pixels.clear();
      }
      //     void Links_Rechts(int dispNr, int k) {
      // for (int i = 0; i < 21; i++) {
      for (int k = 0; k < 7; k++) {
        if (bLinks_Rechts[20 - iSpalte][k] > 73) {
          pixels.setPixelColor(bLinks_Rechts[20 - iSpalte][k] , pixels.Color(iRed3, iGreen3, iBlue3));
        }
        if (bLinks_Rechts[20 - iSpalte][k] > 36 && bLinks_Rechts[20 - iSpalte][k] < 74) {
          pixels.setPixelColor(bLinks_Rechts[20 - iSpalte][k] , pixels.Color(iRed2, iGreen2, iBlue2));
        }
        if (bLinks_Rechts[20 - iSpalte][k] < 37 ) {
          pixels.setPixelColor(bLinks_Rechts[20 - iSpalte][k] , pixels.Color(iRed1, iGreen1, iBlue1));
        }
      } pixels.show();
    }
  }
  if (Mode == 6) {
    if (millis() - millis_alt > Intervall) {
      millis_alt = millis();
      iReihe = iReihe + 3;
      if (iReihe > 19) {
        iReihe = 1;
        pixels.clear();
      } if (Muster == 1) {
        pixels.clear();
      }
      for (int k = 0; k < 4; k++) {
        pixels.setPixelColor((bOben_Unten[19 + iReihe ][k]), pixels.Color(iRed3, iGreen3, iBlue3));
        pixels.setPixelColor((bOben_Unten[18 + iReihe][k]), pixels.Color(iRed2, iGreen2, iBlue2));
        pixels.setPixelColor((bOben_Unten[20 + iReihe ][k]), pixels.Color(iRed1, iGreen1, iBlue1));
        pixels.setPixelColor((bOben_Unten[18 - iReihe ][k]), pixels.Color(iRed3, iGreen3, iBlue3));
        pixels.setPixelColor((bOben_Unten[20 - iReihe][k]), pixels.Color(iRed2, iGreen2, iBlue2));
        pixels.setPixelColor((bOben_Unten[19 - iReihe ][k]), pixels.Color(iRed1, iGreen1, iBlue1));
      } pixels.show();
    }
  }
  if (Mode == 7) {
    if (millis() - millis_alt > Intervall) {
      millis_alt = millis();
      iReihe = iReihe + 3;
      if (iReihe > 19) {
        iReihe = 1;
        pixels.clear();
      } if (Muster == 1) {
        pixels.clear();
      }
      for (int k = 0; k < 4; k++) {
        pixels.setPixelColor((bOben_Unten[39 - iReihe ][k]), pixels.Color(iRed3, iGreen3, iBlue3));
        pixels.setPixelColor((bOben_Unten[38 - iReihe][k]), pixels.Color(iRed2, iGreen2, iBlue2));
        pixels.setPixelColor((bOben_Unten[40 - iReihe ][k]), pixels.Color(iRed1, iGreen1, iBlue1));
        pixels.setPixelColor((bOben_Unten[iReihe + 1 ][k]), pixels.Color(iRed3, iGreen3, iBlue3));
        pixels.setPixelColor((bOben_Unten[iReihe ][k]), pixels.Color(iRed2, iGreen2, iBlue2));
        pixels.setPixelColor((bOben_Unten[iReihe - 1][k]), pixels.Color(iRed1, iGreen1, iBlue1));
      } pixels.show();
    }
  }
  if (Mode == 8) {
    if (millis() - millis_alt > Intervall) {
      millis_alt = millis();
      iReihe = iReihe - 3;
      if (iReihe < 1) {
        iReihe = 38;
        pixels.clear();
      } if (Muster == 1) {
        pixels.clear();
      }
      for (int k = 0; k < 4; k++) {
        pixels.setPixelColor((bOben_Unten[iReihe ][k]), pixels.Color(iRed3, iGreen3, iBlue3));
        pixels.setPixelColor((bOben_Unten[iReihe - 1][k]), pixels.Color(iRed2, iGreen2, iBlue2));
        pixels.setPixelColor((bOben_Unten[iReihe + 1][k]), pixels.Color(iRed1, iGreen1, iBlue1));

      }
      pixels.show();
    }
  }
  if (Mode == 9) {

    if (millis() - millis_alt > Intervall) {
      millis_alt = millis();
      iReihe = iReihe + 3;
      if (iReihe > 39) {
        iReihe = 1;
        pixels.clear();
      } if (Muster == 1) {
        pixels.clear();
      }
      for (int k = 0; k < 4; k++) {
        pixels.setPixelColor((bOben_Unten[iReihe + 1][k]), pixels.Color(iRed3, iGreen3, iBlue3));
        pixels.setPixelColor((bOben_Unten[iReihe][k]), pixels.Color(iRed2, iGreen2, iBlue2));
        pixels.setPixelColor((bOben_Unten[iReihe - 1][k]), pixels.Color(iRed1, iGreen1, iBlue1));
      } pixels.show();
    }
  }
  MDNS.update();
  server.handleClient();
  webSocket.loop();
}
