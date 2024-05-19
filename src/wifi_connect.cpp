#include "utils.h"

const char* config_ssid = "ESP-Spotify-Controller";
const char* config_password = "ESP-Password";

String ssid = "";
String password = "";
bool configured = false;
bool connected = false;

const char *index_html = R"(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Network Login Page</title>
    <style>
        body {
            display: flex;
            justify-content: center;
            align-items: center;
            height: 100vh;
            background-color: #def4fc;
            margin: 0;
        }
        .login-container {
            background: #ffffff;
            padding: 20vh;
            box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);
            border-radius: 8px;
            width: 50vw;
            text-align: left
        }
        
        .login-container input[type="text"],
        .login-container input[type="password"] {
            width: 95%;
            padding: 10px;
            margin: 10px 0;
            border: 1px solid #ccc;
            border-radius: 4px;
        }
        .login-container button {
            width: 40%;
            padding: 10px;
            border: none;
            border-radius: 4px;
            background-color: #de14b5;
            color: white;
            font-size: 16px;
            cursor: pointer;
            margin-top: 10px;
        }
        .login-container button:hover {
            background-color: #ab138c;
        }
    </style>
</head>
<body>
    <div class="login-container">
        <h1>Network Login</h1>
        <form action=/post>
            SSID: <input type="text" placeholder="SSID" name="SSID" required>
            PASSWORD: <input type="password" placeholder="Password" name="PASSWORD" required>
            <button type="submit">Login</button>
        </form>
    </div>
</body>
</html>
)";


void configureServer() {
    server.on("/", HTTP_GET, [] (AsyncWebServerRequest *request) {
        request->send(200, "text/html", index_html);
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

String WiFiConnect() {
    WiFi.mode(WIFI_AP);
    WiFi.softAP(config_ssid, config_password);
    displayText(WiFi.softAPIP().toString() + "\nPort: 100", ST77XX_MAGENTA, ST77XX_CYAN, 3);

    configureServer();

    server.begin();
    while (!configured) {
        delay(500);
    }

    while(!connected) {
        WiFi.mode(WIFI_STA);
        WiFi.begin(ssid, password);
        Serial.println("Connecting");
        int tries = 0;

        while(WiFi.status() != WL_CONNECTED && tries < 20){
            Serial.print(".");
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
        displayText("Connected!\nLocal IP:\n" + WiFi.localIP().toString(), ST77XX_MAGENTA, ST77XX_CYAN, 3);
    }
    server.end();

    return WiFi.localIP().toString();
}