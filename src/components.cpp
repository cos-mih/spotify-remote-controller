#include "utils.h"

bool on_pressed = false;
bool next_pressed = false;
bool prev_pressed = false;

bool vol_mod_pressed = false;
bool auto_volume = false;

bool shuffle_pressed = false;
bool shuffled = false;

bool play_pause_pressed = false;
bool playing = true;

bool encoder_rotated = false;

Adafruit_ST7789 display = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

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

void IRAM_ATTR encoder_isr() {
    encoder_rotated = true;

    if (digitalRead(ROT_ENC_DT)) {
        volume += 5;
        if (volume > 100) {
            volume = 100;
        }
    } else {
        volume -= 5;
        if (volume < 0) {
            volume = 0;
        }
    }
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

void initRotaryEncoder() {
    pinMode(ROT_ENC_DT, INPUT);
    pinMode(ROT_ENC_CLK, INPUT);
    attachInterrupt(ROT_ENC_CLK, encoder_isr, FALLING);
    configureButton(ROT_ENC_SW, play_pause_isr);
}

void initButtons() {
    configureButton(PLAY_PREV, play_prev_isr);
    configureButton(PLAY_NEXT, play_next_isr);
    configureButton(VOL_MODE, vol_mod_isr);
    configureButton(SHUFFLE, shuffle_isr);
    initRotaryEncoder();
}

void initSensor() {
    analogReadResolution(16);
    pinMode(SENSOR, INPUT);
}

void initDisplay() {
    display.init(240, 240, SPI_MODE3);
    display.setRotation(2);
}
