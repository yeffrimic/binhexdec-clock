#include <NTPClient.h>
// change next line to use with another board/shield
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#define PIN D3

const char *ssid     = "Xibalba Hackerspace";
const char *password = "holi1234";

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(32, 8, PIN,
                            NEO_MATRIX_BOTTOM     + NEO_MATRIX_RIGHT +
                            NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,
                            NEO_GRB            + NEO_KHZ800);


const uint16_t colors[] = {
  matrix.Color(255, 0, 0), matrix.Color(0, 255, 0), matrix.Color(0, 0, 255)
};


WiFiUDP ntpUDP;

// By default 'pool.ntp.org' is used with 60 seconds update interval and
// no offset
//NTPClient timeClient(ntpUDP);

// You can specify the time server pool and the offset, (in seconds)
// additionally you can specify the update interval (in milliseconds).
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", -6 * 3600, 60000);

void setup() {
  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(10);
  matrix.setTextColor(colors[0]);
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }

  timeClient.begin();
}



int x    = matrix.width();
int pass = 0;

void loop() {

  int hours = timeClient.getHours();
  int minutes = timeClient.getMinutes();
  int seconds = timeClient.getSeconds();
  char hourHex[4];
  char minuteHex[4];
  char secondsHex[4];
  sprintf(hourHex, "%X", hours);
  sprintf(minuteHex, "%X", minutes);
  sprintf(secondsHex, "%X", seconds);
  //
  Serial.print(hours);
  Serial.print(":");
  Serial.print(minutes);
  Serial.print(":");
  Serial.println(seconds);
  Serial.print(hourHex);
  Serial.print(":");
  Serial.print(minuteHex);
  Serial.print(":");
  Serial.println(secondsHex);

  matrix.fillScreen(0);
  matrix.setCursor(x, 0);
  matrix.print(F(""));
  matrix.print(hourHex);
  matrix.print(":");
  matrix.print(minuteHex);
  matrix.print(":");
  matrix.print(secondsHex);
  if (--x < -46) {
    x = matrix.width();
    if (++pass >= 3) pass = 0;
    matrix.setTextColor(colors[pass]);
  }
  matrix.show();
  timeClient.update();
  //Serial.println(timeClient.getFormattedTime());

  delay(50);
}
