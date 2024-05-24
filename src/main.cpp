#include "utils.h"

int timer_count = 0;

void setup() {
    Serial.begin(115200);
    Serial.println("Start");

    initDisplay();

    // connect to WiFi network
    WiFiConnect();

    // spotify auth
    initGetTokenIndex();
    spotifyAuthenticate();

    // init components
    initSensor();
    initButtons();
    initTimer();
}

void loop() {
    if (next_pressed) {
        next_pressed = false;
        postPlayNext();
    }

    if (prev_pressed) {
        prev_pressed = false;
        postPlayPrev();
    }

    if (vol_mod_pressed) {
        vol_mod_pressed = false;

    }

    if (shuffle_pressed) {
        shuffle_pressed = false;
        putShuffle();
    }

    if (play_pause_pressed) {
        play_pause_pressed = false;
        if (playing) {
            putResume();
        } else {
            putPause();
        }
    }

    if (encoder_rotated) {
        encoder_rotated = false;
        putSetVolume();
    }

    if (timer_off) {
        timer_off = false;
        timer_count++;
        if (playing) {
            current_progress += 1000;
        }
        displayProgress();
        if (timer_count == 4) {
            getCurrentTrack();
            timer_count = 0;
        }
    }
}
