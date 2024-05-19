#ifndef UTILS_H
#define UTILS_H

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Arduino_JSON.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>

extern AsyncWebServer server;
extern Adafruit_ST7789 display;


String WiFiConnect();

void spotifyAuthenticate(String ip);
void postPlayNext();
void postPlayPrev();
void putPause();
void putResume();
void putShuffle(bool);

void displayText(String, uint16_t, uint16_t, int);

#endif
