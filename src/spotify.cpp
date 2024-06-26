#include "utils.h"
#include <string.h>

AsyncWebServer auth_server(5001);
String current_track = "";
int current_progress = 0; 
int current_duration = 0;

int volume = -1;

String client_id = "fa5ef2f1b8304d1199276aa056347f5b";
String client_token = "";
String auth_link = "";
bool access_granted = false;

void getToken() {
    HTTPClient http;

    String redirect = "http%3A%2F%2F" + localIP + "%3A5001%2Fget-token";
    String url = "https://accounts.spotify.com/authorize?response_type=token&redirect_uri=" + redirect 
                + "&client_id=" + client_id 
                + "&scope=user-read-playback-state+user-read-playback-position+user-modify-playback-state";

    http.begin(url);
    int httpCode = http.GET();
    auth_link = http.getLocation();
    http.end();

    while(!access_granted) delay(500);
}

void getCurrentTrack() {
    HTTPClient http;

    http.begin("https://api.spotify.com/v1/me/player");
    http.addHeader("Authorization", "Bearer " + client_token);
    int httpCode = http.GET();
    
    JSONVar player_json = JSON.parse(http.getString());
    volume = player_json["device"]["volume_percent"];
    playing = player_json["is_playing"];
    bool shuffled_new = player_json["shuffle_state"];
    String repeat_mode = player_json["repeat_state"];
    String track_name = player_json["item"]["name"];
    String artist_name = player_json["item"]["artists"][0]["name"];
    int progress = player_json["progress_ms"];
    int duration = player_json["item"]["duration_ms"];
    if (track_name != current_track) {
        displayTrack(track_name, artist_name);
        current_track = track_name;
        current_progress = progress;
        current_duration = duration;
    } else if ((progress % 60000) / 1000 != (current_progress % 60000) / 1000) {
        current_progress = progress;
    }
    if (shuffled_new != shuffled) {
        shuffled = shuffled_new;
        displayShuffleState();
    }
    bool repeat = repeat_mode == "off" ? false : true;
    if (repeat != repeat_on) {
        repeat_on = repeat;
        displayRepeatState();
    }

    http.end();
}

void configureAuthServer() {
    auth_server.on("/", HTTP_GET, [] (AsyncWebServerRequest *request) {
        request->redirect(auth_link);
    });

    auth_server.on("/get-token", HTTP_GET, [] (AsyncWebServerRequest *request) {
        request->send(200, "text/html", get_token_script_index);
    });

    auth_server.on("/callback", HTTP_ANY, [] (AsyncWebServerRequest *request) {
        if (request->hasParam("access_token")) {
            client_token = request->getParam("access_token")->value();
            access_granted = true;
            request->send(200, "text/html", "Access granted\n");
        } else {
            request->send(400, "text/html", "Bad request\n");
        }
    });

    auth_server.onNotFound([] (AsyncWebServerRequest *request) {
        request->send(404, "text/html", "Not found");
    });
}

void spotifyAuthenticate() {
    configureAuthServer();    
    auth_server.begin();
    getToken();
    auth_server.end();

    displayText("Welcome back!", ST77XX_BLACK, ST77XX_GREEN, 5);
    delay(4000);
    getCurrentTrack();
}

void postPlayNext() {
    HTTPClient http;

    http.begin("https://api.spotify.com/v1/me/player/next");
    http.addHeader("Authorization", "Bearer " + client_token);
    http.addHeader("Content-Length", "0");
    int httpCode = http.POST("");
    http.end();
}

void postPlayPrev() {
    HTTPClient http;

    http.begin("https://api.spotify.com/v1/me/player/previous");
    http.addHeader("Authorization", "Bearer " + client_token);
    http.addHeader("Content-Length", "0");
    int httpCode = http.POST("");
    http.end();
}

void putPause() {
    HTTPClient http;

    http.begin("https://api.spotify.com/v1/me/player/pause");
    http.addHeader("Authorization", "Bearer " + client_token);
    http.addHeader("Content-Length", "0");
    int httpCode = http.PUT("");
    http.end();
}

void putResume() {
    HTTPClient http;

    http.begin("https://api.spotify.com/v1/me/player/play");
    http.addHeader("Authorization", "Bearer " + client_token);
    http.addHeader("Content-Length", "0");
    int httpCode = http.PUT("");
    http.end();
}

void putShuffle() {
    HTTPClient http;

    String query = shuffled ? "true" : "false";

    http.begin("https://api.spotify.com/v1/me/player/shuffle?state=" + query);
    http.addHeader("Authorization", "Bearer " + client_token);
    http.addHeader("Content-Length", "0");
    int httpCode = http.PUT("");
    http.end();
}

void putSetVolume() {
    HTTPClient http;

    String query = String(volume);

    http.begin("https://api.spotify.com/v1/me/player/volume?volume_percent=" + query);
    http.addHeader("Authorization", "Bearer " + client_token);
    http.addHeader("Content-Length", "0");
    int httpCode = http.PUT("");
    http.end();
}

void putRepeat() {
    HTTPClient http;

    String query = repeat_on ? "track" : "off";

    http.begin("https://api.spotify.com/v1/me/player/repeat?state=" + query);
    http.addHeader("Authorization", "Bearer " + client_token);
    http.addHeader("Content-Length", "0");
    int httpCode = http.PUT("");
    http.end();
}
