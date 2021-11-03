#include <NTPClient.h>
// change next line to use with another board/shield
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#define BUTTON_1 D7
#define BUTTON_2 D6
#define BUTTON_3 D4
#define BUTTON_4 D5
#define MATRIX_PIN D3


Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(32, 8, MATRIX_PIN,
                            NEO_MATRIX_BOTTOM     + NEO_MATRIX_RIGHT +
                            NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,
                            NEO_GRB            + NEO_KHZ800);



boolean isHour = true;
WiFiUDP ntpUDP;

NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", -6 * 3600, 60000);

const char *ssid     = "holii";
const char *password = "estalibre";

int hours = 0;
int minutes = 0;
int seconds = 0 ;
int newHours = 0;
int newMinutes = 0;

char hourHex[4];
char minuteHex[4];
char secondsHex[4];

int x = matrix.width();
int r = 0;
int pass = 0;


const uint16_t colors[] = {
  matrix.Color(255, 0, 0), matrix.Color(0, 255, 0), matrix.Color(0, 0, 255)
};

void decToBin(unsigned int in, int pos) {

  boolean state = 0;
  for (int i = 0; i <  8; i++) {
    //Serial.print(pos + i);
    state = in & (1 << i);
    //Serial.println(state);
    if (state) {
      matrix.setPixelColor(pos + i, colors[0]);
    } else {
      matrix.setPixelColor(pos + i, 0);
    }
  }
  // Serial.println();
}

int button(int MATRIX_PIN, int x, boolean y) {
  boolean estado = digitalRead(MATRIX_PIN);
  //Serial.println(x);
  if (estado == LOW) {
    delay(30);
    //Serial.println(estado);
    if (y)  x++;
    else x--;
  }
  // Serial.println(x);
  return x;
}

void updateTime() {

  hours = timeClient.getHours() + newHours;
  minutes = timeClient.getMinutes() + newMinutes;
  seconds = timeClient.getSeconds();
}

void showDisplay() {

  switch (r) {
    case 0:
      matrix.setTextColor(colors[1]);
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
      matrix.setTextColor(colors[0]);
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
      matrix.fillScreen(0);
      decToBin(hours, 128);
      decToBin(minutes, 144);
      decToBin(seconds, 160);
      break;
  }


  if (--x < -46) {
    x = matrix.width();

  }
  matrix.show();
}

void setup() {
  pinMode(BUTTON_1, INPUT_PULLUP);
  pinMode(BUTTON_2, INPUT_PULLUP);
  pinMode(BUTTON_3, INPUT_PULLUP);
  pinMode(BUTTON_4, INPUT_PULLUP);
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
  delay(1000);
  timeClient.update();
}




void loop() {
  updateTime();
  if (digitalRead(BUTTON_4) == LOW) {
    delay(30);
    isHour = !isHour;
  }
  if (isHour) {
    newHours = button(BUTTON_1, newHours, true);
    newHours = button(BUTTON_2, newHours, false);
  } else {
    newMinutes = button(BUTTON_1, newMinutes, true);
    newMinutes = button(BUTTON_2, newMinutes, false);
  }
  if ( digitalRead(BUTTON_3) == LOW) {
    delay(30);
    if (++r >= 3) {
      r = 0;
    }
  }

  sprintf(hourHex, "%X", hours);
  sprintf(minuteHex, "%X", minutes);
  sprintf(secondsHex, "%X", seconds);

  showDisplay();

  timeClient.update();
  delay(50);
}
