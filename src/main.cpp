#include "utils.h"


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
        getCurrentTrack();
    }

    if (prev_pressed) {
        prev_pressed = false;
        postPlayPrev();
        getCurrentTrack();
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
}
