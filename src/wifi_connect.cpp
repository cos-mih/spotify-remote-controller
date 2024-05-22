#include "utils.h"
#include <ESPmDNS.h>

AsyncWebServer server(5000);
String localIP = "";

const char* config_ssid = "ESP-Spotify-Controller";
const char* config_password = "ESP-Password";

String ssid = "";
String password = "";
bool configured = false;
bool connected = false;


void configureServer() {
    server.on("/", HTTP_GET, [] (AsyncWebServerRequest *request) {
        request->send(200, "text/html", network_login_index_html);
    });

    server.on("/post", HTTP_GET, [] (AsyncWebServerRequest *request) {
        ssid = request->getParam("SSID")->value();
        password = request->getParam("PASSWORD")->value();
        request->send(200, "text/html", "Config done\n");
        configured = true;
    });

    server.onNotFound([] (AsyncWebServerRequest *request) {
        request->send(404, "text/html", "Not found");
    });
}

void WiFiConnect() {
    WiFi.mode(WIFI_AP);
    WiFi.softAP(config_ssid, config_password);
    displayText(WiFi.softAPIP().toString() + "\nPort: 5000", ST77XX_MAGENTA, ST77XX_CYAN, 3);

    configureServer();

    server.begin();
    while (!configured) {
        delay(500);
    }

    while(!connected) {
        WiFi.mode(WIFI_STA);
        WiFi.begin(ssid, password);
        displayText("Connecting", ST77XX_MAGENTA, ST77XX_CYAN, 3);
        int tries = 0;

        while(WiFi.status() != WL_CONNECTED && tries < 20){
            display.print(".");
            delay(1000);
            tries++;
        }

        if (tries == 20) {
            configured = false;
            WiFi.mode(WIFI_AP);
            WiFi.softAP(config_ssid, config_password);
            displayText(WiFi.softAPIP().toString(), ST77XX_MAGENTA, ST77XX_CYAN, 3);
            while (!configured) {
                delay(500);
            }
            continue;
        }

        connected = true;
        localIP = WiFi.localIP().toString();
        displayText("Connected!\nLocal IP:\n" + localIP + "\n\nConnect to\nport: 5001" ,ST77XX_MAGENTA, ST77XX_CYAN, 3);
    }
    server.end();
}
