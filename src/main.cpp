#include "utils.h"

AsyncWebServer server(100);


void setup() {
    Serial.begin(115200);
    Serial.println("Start");

    // init components
    initSensor();
    initButtons();
    initDisplay();

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
        putShuffle();
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

    if (encoder_rotated) {
        Serial.println("Encoder rotated");
        encoder_rotated = false;
        putSetVolume();
    }
}
