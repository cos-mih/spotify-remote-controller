#include "utils.h"

unsigned long button_press_time = 0;  
unsigned long last_button_press_time = 0;

bool repeat_pressed = false;
bool repeat_on = false;

bool next_pressed = false;
bool prev_pressed = false;

bool vol_mod_pressed = false;
bool auto_volume = false;

bool shuffle_pressed = false;
bool shuffled = false;

bool play_pause_pressed = false;
bool playing = true;

bool encoder_rotated = false;

bool timer_off = false;

Adafruit_ST7789 display = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);
hw_timer_t *timer = NULL;

void IRAM_ATTR timer_isr() {
    timer_off = true;
}

bool debounce() {
    button_press_time = millis();
    if (button_press_time - last_button_press_time > 500) {
        last_button_press_time = button_press_time;
        return true;
    }
    return false;
}

void IRAM_ATTR repeat_isr() {
    if (debounce()) {
        repeat_pressed = true;
        repeat_on = !repeat_on;
    }
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
    configureButton(REPEAT, repeat_isr);
    configureButton(PLAY_PREV, play_prev_isr);
    configureButton(PLAY_NEXT, play_next_isr);
    configureButton(VOL_MODE, vol_mod_isr);
    configureButton(SHUFFLE, shuffle_isr);
    initRotaryEncoder();
}

void initSensor() {
    analogReadResolution(12);
    pinMode(SENSOR, INPUT);
}

void initTimer() {
    timer = timerBegin(0, 80, true);
    timerAttachInterrupt(timer, timer_isr, true);
    timerAlarmWrite(timer, 1000000, true);
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

void displayTrack(String track_name, String artist_name) {
    display.fillScreen(ST77XX_BLACK);
    display.setCursor(0, 20);
    display.setTextSize(4);
    display.setTextColor(ST77XX_GREEN);
    display.println(track_name);
    display.setTextSize(3);
    display.setTextColor(ST77XX_WHITE);
    display.println(artist_name);
}

void displayProgress() {
    display.setCursor(0, 200);
    int m = current_progress / 60000;
    int s = (current_progress % 60000) / 1000;
    
    String min = m >= 10 ? String(m) : "0" + String(m);
    String sec = s >= 10 ? String(s) : "0" + String(s);

    display.fillRect(0, 220, 240, 2, ST77XX_BLACK);
    int progress = (current_progress * 240) / current_duration;
    display.fillRect(0, 220, progress, 2, ST77XX_CYAN);

    display.setTextSize(2);
    display.setTextColor(ST77XX_GREEN, ST77XX_BLACK);
    display.println(min + ":" + sec);
}

void displayShuffleState() {
    display.setCursor(180, 200);
    display.setTextSize(2);
    if (shuffled) {
        display.setTextColor(ST77XX_YELLOW);
        display.print("SHUFF");
    } else {
        display.setTextColor(ST77XX_BLACK);
        display.print("SHUFF");
    }
}

void displayRepeatState() {
    if (repeat_on) {
        display.fillCircle(110, 205, 5, ST77XX_ORANGE);
    } else {
        display.fillCircle(110, 205, 5, ST77XX_BLACK);
    }   
}

void displayVolumeMode() {
    if (auto_volume) {
        display.fillCircle(125, 205, 5, ST77XX_GREEN);
    } else {
        display.fillCircle(125, 205, 5, ST77XX_BLACK);
    }  
}
