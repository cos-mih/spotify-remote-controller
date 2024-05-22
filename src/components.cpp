#include "utils.h"

unsigned long button_press_time = 0;  
unsigned long last_button_press_time = 0;

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
hw_timer_t *timer = NULL;

void IRAM_ATTR timer_isr() {
    Serial.println("hei");
}

bool debounce() {
    button_press_time = millis();
    if (button_press_time - last_button_press_time > 1000) {
        last_button_press_time = button_press_time;
        return true;
    }
    return false;
}

void IRAM_ATTR play_next_isr() {
    if (debounce()) {
        next_pressed = true;
    }
}

void IRAM_ATTR play_prev_isr() {
    if (debounce()) {
        prev_pressed = true;
    }
}

void IRAM_ATTR vol_mod_isr() {
    if (debounce()) {
        vol_mod_pressed = true;
        auto_volume = !auto_volume;
    }
}

void IRAM_ATTR shuffle_isr() {
    if (debounce()) {
        shuffle_pressed = true;
        shuffled = !shuffled;
    }
}

void IRAM_ATTR play_pause_isr() {
    if (debounce()) {
        play_pause_pressed = true;
        playing = !playing;
    }
}

void IRAM_ATTR encoder_isr() {
    if (debounce()) {
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

void initTimer() {
    timer = timerBegin(0, 80, true);
    timerAttachInterrupt(timer, timer_isr, true);
    timerAlarmWrite(timer, 4000000, true);
    timerAlarmEnable(timer); 
}

void initDisplay() {
    display.init(240, 240, SPI_MODE3);
    display.setRotation(2);
    display.setTextWrap(true);
}

void displayText(String message, uint16_t bgColor, uint16_t txtColor, int txtSize) {
    display.fillScreen(bgColor);
    display.setCursor(0, 30);
    display.setTextColor(txtColor);
    display.setTextSize(txtSize);
    display.println(message);
}

void setupTrackDisplay() {
    display.setTextColor(ST77XX_GREEN);
}

void displayTrack(String track_name, String artist_name) {
    display.fillScreen(ST77XX_BLACK);
    display.setCursor(0, 30);
    display.setTextSize(4);
    display.println(track_name);
    display.setTextSize(3);
    display.println(artist_name);
}
