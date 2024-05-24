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

#define SENSOR 36

#define POWER 32
#define PLAY_NEXT 33
#define PLAY_PREV 25
#define VOL_MODE 26
#define SHUFFLE 27

#define ROT_ENC_CLK 14
#define ROT_ENC_DT 12
#define ROT_ENC_SW 13

#define TFT_CS 14
#define TFT_MOSI 23
#define TFT_SCLK 18
#define TFT_DC 4
#define TFT_RST 2

extern bool on_pressed;
extern bool next_pressed;
extern bool prev_pressed;

extern bool vol_mod_pressed;
extern bool auto_volume;

extern bool shuffle_pressed;
extern bool shuffled;

extern bool play_pause_pressed;
extern bool playing;

extern bool encoder_rotated;
extern int volume;
extern bool timer_off;

extern String network_login_index_html;
extern String get_token_script_index;

extern Adafruit_ST7789 display;
extern hw_timer_t *timer;
extern String localIP;
extern String current_track;
extern int current_progress;
extern int current_duration;

void initDisplay();
void initSensor();
void initButtons();
void initTimer();

void initGetTokenIndex();

void WiFiConnect();

void getCurrentTrack();
void spotifyAuthenticate();
void postPlayNext();
void postPlayPrev();
void putPause();
void putResume();
void putShuffle();
void putSetVolume();

void displayText(String, uint16_t, uint16_t, int);
void displayTrack(String, String);
void setupTrackDisplay();
void displayProgress();

#endif
