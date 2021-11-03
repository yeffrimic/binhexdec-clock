#include <NTPClient.h>
// change next line to use with another board/shield
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#define boton1 D7
#define boton2 D6
#define boton3 D5
#define boton4 D4

#define PIN D3
boolean HourOrMinutesIs = true;
WiFiUDP ntpUDP;

NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", -6 * 3600, 60000);

const char *ssid     = "Xibalba Hackerspace";
const char *password = "holi1234";

int hours = 0;
int minutes = 0;
int seconds = 0 ;
int newHours = 0;
int newMinutes = 0;
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
    //Serial.print(pos + i);
    state = in & (1 << i);
    //Serial.println(state);
    if (state) {
      matrix.setPixelColor(pos + i, colors[1]);
    } else {
      matrix.setPixelColor(pos + i, colors[0]);
    }
  }
  // Serial.println();
}

int button(int pin, int x, boolean y) {
  boolean estado = digitalRead(pin);
  Serial.println(x);
  if (estado == LOW) {
    delay(10);
    Serial.println(estado);
    if (y)  x++;
    else x--;
  }
  Serial.println(x);
  return x;
}

void setup() {
  pinMode(boton1, INPUT_PULLUP);
  pinMode(boton2, INPUT_PULLUP);
  pinMode(boton3, INPUT_PULLUP);
  pinMode(boton4, INPUT_PULLUP);
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



int x    = matrix.width();
int r = 0;
int pass = 0;

void loop() {

  if (hours > 23) {
    hours = 0;
    newHours = 0;
  }
  if ( hours < 0) {
    hours = 23;
    newHours = 0;
  }
  if (minutes > 59) {
    minutes = 0;
    newMinutes = 0;
  }
  if (minutes < 0) {
    
    minutes = 59;
    newMinutes = 0;
  }
    hours = timeClient.getHours() + newHours;
    minutes = timeClient.getMinutes() + newMinutes;
    seconds = timeClient.getSeconds();
    if (digitalRead(boton4) == LOW) {
      delay(10);
      HourOrMinutesIs = !HourOrMinutesIs;
    }
    if (HourOrMinutesIs) {
      newHours = button(boton1, newHours, true);
      newHours = button(boton2, newHours, false);
    } else {

      newMinutes = button(boton1, newMinutes, true);
      newMinutes = button(boton2, newMinutes, false);
    }
    if ( digitalRead(boton3) == LOW) {

      if (++r >= 3) {
        r = 0;
      }
    }

    char hourHex[4];
    char minuteHex[4];
    char secondsHex[4];
    sprintf(hourHex, "%X", hours);
    sprintf(minuteHex, "%X", minutes);
    sprintf(secondsHex, "%X", seconds);
    //
    Serial.println(hours);
    /* Serial.print(":");
      Serial.print(minutes);
      Serial.print(":");
      Serial.println(seconds);
      Serial.print(hourHex);
      Serial.print(":");
      Serial.print(minuteHex);
      Serial.print(":");
      Serial.println(secondsHex);*/


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
      matrix.setTextColor(colors[pass]);
    }
    matrix.show();
    timeClient.update();
    //Serial.println(timeClient.getFormattedTime());

    delay(50);
  }
