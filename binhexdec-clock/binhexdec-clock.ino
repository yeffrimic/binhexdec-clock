#include <NTPClient.h>
// change next line to use with another board/shield
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#define PIN D3

WiFiUDP ntpUDP;

NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", -6 * 3600, 60000);

const char *ssid     = "holii";
const char *password = "estalibre";

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(32, 8, PIN,
                            NEO_MATRIX_BOTTOM     + NEO_MATRIX_RIGHT +
                            NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,
                            NEO_GRB            + NEO_KHZ800);


const uint16_t colors[] = {
  matrix.Color(255, 0, 0), matrix.Color(0, 255, 0), matrix.Color(0, 0, 255)
};

void decToBin(unsigned int in, int pos) {

  boolean state = 0;
  for (int i = 0; i <  8; i++) {
    Serial.print(pos + i);
    state = in & (1 << i);
    Serial.println(state);
    if (state) {
      matrix.setPixelColor(pos + i, colors[1]);
    } else {
      matrix.setPixelColor(pos + i, colors[0]);
    }
  }
  Serial.println();
}

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
int r = 0;
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


  switch (r) {
    case 0:
      matrix.fillScreen(0);
      matrix.setCursor(x, 0);
      matrix.print(F(""));
      matrix.print(hours);
      matrix.print(":");
      matrix.print(minutes);
      matrix.print(":");
      matrix.print(seconds);
      break;
    case 1:
      matrix.fillScreen(0);
      matrix.setCursor(x, 0);
      matrix.print(F(""));
      matrix.print(hourHex);
      matrix.print(":");
      matrix.print(minuteHex);
      matrix.print(":");
      matrix.print(secondsHex);
      break;
    case 2:

      decToBin(hours, 128);
      decToBin(minutes, 144);
      decToBin(seconds, 160);
      break;

  }


  if (--x < -46) {
    x = matrix.width();
    if (++pass >= 3) {
      pass = 0;
      //r++;
      //if (r > 2) r = 0;
    }
    matrix.setTextColor(colors[pass]);
  }
  matrix.show();
  timeClient.update();
  //Serial.println(timeClient.getFormattedTime());

  delay(50);
}
