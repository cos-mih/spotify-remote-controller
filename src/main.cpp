#include "utils.h"

#define SENSOR 36

#define POWER 32
#define PLAY_NEXT 33
#define PLAY_PREV 25
#define VOL_MODE 26
#define SHUFFLE 27

#define ROT_ENC_SW 13

#define TFT_CS 14
#define TFT_MOSI 23
#define TFT_SCLK 18
#define TFT_DC 4
#define TFT_RST 2

bool on_pressed = false;
bool next_pressed = false;
bool prev_pressed = false;

bool vol_mod_pressed = false;
bool auto_volume = false;

bool shuffle_pressed = false;
bool shuffled = false;

bool play_pause_pressed = false;
bool playing = true;

Adafruit_ST7789 display = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);
AsyncWebServer server(100);


void IRAM_ATTR play_next_isr() {
    next_pressed = true;
}

void IRAM_ATTR play_prev_isr() {
    prev_pressed = true;
}

void IRAM_ATTR vol_mod_isr() {
    vol_mod_pressed = true;
    auto_volume = !auto_volume;
}

void IRAM_ATTR shuffle_isr() {
    shuffle_pressed = true;
    shuffled = !shuffled;
}

void IRAM_ATTR play_pause_isr() {
    play_pause_pressed = true;
    playing = !playing;
}

void displayText(String message, uint16_t bgColor, uint16_t txtColor, int txtSize) {
    display.setTextWrap(true);
    display.fillScreen(bgColor);
    display.setCursor(0, 30);
    display.setTextColor(txtColor);
    display.setTextSize(txtSize);
    display.println(message);
}

void configureButton(uint8_t pin, void (*isr_func) ()) {
    pinMode(pin, INPUT_PULLUP);
    attachInterrupt(pin, isr_func, FALLING);
}

void initButtons() {
    configureButton(PLAY_PREV, play_prev_isr);
    configureButton(PLAY_NEXT, play_next_isr);
    configureButton(VOL_MODE, vol_mod_isr);
    configureButton(SHUFFLE, shuffle_isr);
    configureButton(ROT_ENC_SW, play_pause_isr);
}

void setup() {
    Serial.begin(115200);
    Serial.println("Start");

    // init noise sensor pin
    analogReadResolution(16);
    pinMode(SENSOR, INPUT);

    // init buttons
    initButtons();

    // init tft displat
    display.init(240, 240, SPI_MODE3);

    // connect to WiFi network
    String localIP = WiFiConnect();

    // spotify auth
    spotifyAuthenticate(localIP);

    displayText("Welcome back!", ST77XX_BLACK, ST77XX_YELLOW, 5);
    delay(4000);
}

void loop() {
 
    if (next_pressed) {
        Serial.println("Next button pressed");
        next_pressed = false;
        postPlayNext();
    }

    if (prev_pressed) {
        Serial.println("Prev button pressed");
        prev_pressed = false;
        postPlayPrev();
    }

    if (vol_mod_pressed) {
        Serial.println("Volume mode button pressed");
        vol_mod_pressed = false;

    }

    if (shuffle_pressed) {
        Serial.println("Shuffle button pressed");
        shuffle_pressed = false;
        putShuffle(shuffled);
    }

    if (play_pause_pressed) {
        Serial.println("Play-pause button pressed");
        play_pause_pressed = false;
        if (playing) {
            putResume();
        } else {
            putPause();
        }
    }
}
