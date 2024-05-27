#include "utils.h"

int timer_count = 0;
int last_set_volume = 0;

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
    if (auto_volume) {
        int new_val = analogRead(SENSOR);
        volume = map(new_val, 960, 1045, 100, 0);
        if (abs(volume - last_set_volume) > 10) {
            last_set_volume = volume;
            putSetVolume();
        }
    }

    if (repeat_pressed) {
        repeat_pressed = false;
        putRepeat();
        displayRepeatState();
    }

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
        last_set_volume = volume;
        displayVolumeMode();
    }

    if (shuffle_pressed) {
        shuffle_pressed = false;
        putShuffle();
        displayShuffleState();
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
            displayShuffleState();
            displayRepeatState();
            displayVolumeMode();
            timer_count = 0;
        }
    }
}
