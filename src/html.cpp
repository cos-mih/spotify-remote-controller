#include "utils.h"

String get_token_script_index = "";


String network_login_index_html = R"(
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

void initGetTokenIndex() {
    get_token_script_index = R"(
<!DOCTYPE html>
<html>
<head>
    <title>Spotify Auth</title>
</head>
<body>
    <script>
        const fragment = window.location.hash;
        const token = fragment.substring(fragment.indexOf('=') + 1, fragment.indexOf('&'));

        if (token) {
            fetch('http://)" + localIP + R"(:5001/callback?access_token=' + token)
                .then(response => response.text())
                .then(data => {
                    document.body.innerHTML = `<p>Access granted!</p>`;
                })
                .catch(error => {
                    console.error('Error:', error);
                    document.body.innerHTML = `<p>Error sending token: ${error}</p>`;
                });
        } else {
            document.body.innerHTML = '<p>No access token found</p>';
        }
    </script>
</body>
</html>
    )";
}
