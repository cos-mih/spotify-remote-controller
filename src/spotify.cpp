#include "utils.h"
#include <string.h>

String client_id = "fa5ef2f1b8304d1199276aa056347f5b";
String client_token = "";
String auth_link = "";
bool access_granted = false;

void getToken(String ip) {
    HTTPClient http;

    String redirect = "http%3A%2F%2F" + ip + "%3A100%2Fcallback%2F";
    String url = "https://accounts.spotify.com/authorize?response_type=token&redirect_uri=" + redirect 
                + "&client_id=" + client_id 
                + "&scope=user-read-playback-state+user-read-playback-position+user-modify-playback-state";

    http.begin(url);
    int httpCode = http.GET();

    auth_link = http.getLocation();
    Serial.print("Get token request code: ");
    Serial.println(httpCode);
    Serial.println(auth_link);

    http.end();

    while(!access_granted) delay(500);

    Serial.print("Access token: ");
    Serial.println(client_token);
}

void configureAuthServer() {
    server.on("/", HTTP_ANY, [] (AsyncWebServerRequest *request) {
        request->redirect(auth_link);
    });

    server.on("/callback/", HTTP_ANY, [] (AsyncWebServerRequest *request) {
        if (request->hasParam("access_token")) {
            client_token = request->getParam("access_token")->value();
            access_granted = true;
            request->send(200, "text/html", "Access granted\n");
        }
        request->send(400, "text/html", "Bad request\n");
    });

    server.onNotFound([] (AsyncWebServerRequest *request) {
        request->send(404, "text/html", "Not found");
    });
}

void spotifyAuthenticate(String ip) {
    server.reset();
    configureAuthServer();    
    server.begin();
    getToken(ip);
    server.end();
}

void postPlayNext() {
    HTTPClient http;

    http.begin("https://api.spotify.com/v1/me/player/next");
    http.addHeader("Authorization", "Bearer " + client_token);
    http.addHeader("Content-Length", "0");
    int httpCode = http.POST("");

    Serial.print("Skip next request code: ");
    Serial.println(httpCode);

    http.end();
}

void postPlayPrev() {
    HTTPClient http;

    http.begin("https://api.spotify.com/v1/me/player/previous");
    http.addHeader("Authorization", "Bearer " + client_token);
    http.addHeader("Content-Length", "0");
    int httpCode = http.POST("");

    Serial.print("Skip previous request code: ");
    Serial.println(httpCode);

    http.end();
}

void putPause() {
    HTTPClient http;

    http.begin("https://api.spotify.com/v1/me/player/pause");
    http.addHeader("Authorization", "Bearer " + client_token);
    http.addHeader("Content-Length", "0");
    int httpCode = http.PUT("");

    Serial.print("Pause request code: ");
    Serial.println(httpCode);

    http.end();
}

void putResume() {
    HTTPClient http;

    http.begin("https://api.spotify.com/v1/me/player/play");
    http.addHeader("Authorization", "Bearer " + client_token);
    http.addHeader("Content-Length", "0");
    int httpCode = http.PUT("");

    Serial.print("Resume request code: ");
    Serial.println(httpCode);

    http.end();
}

void putShuffle(bool state) {
    HTTPClient http;

    String query = state ? "true" : "false";

    http.begin("https://api.spotify.com/v1/me/player/shuffle?state=" + query);
    http.addHeader("Authorization", "Bearer " + client_token);
    http.addHeader("Content-Length", "0");
    int httpCode = http.PUT("");

    Serial.print("Shuffle request code: ");
    Serial.println(httpCode);

    http.end();
}