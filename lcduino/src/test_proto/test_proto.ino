#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

// =====================================================
// TFT
// =====================================================

#define TFT_CS        D0
#define TFT_DC        D2
#define TFT_RST       -1
#define PIN_PWR_LCD   D4
#define PIN_BUZZER    D8
#define PIN_BUTTON    D3

// SPI matériel ESP8266 :
// SCK  = D5
// MISO = D6
// MOSI = D7

Adafruit_ILI9341 tft(TFT_CS, TFT_DC, TFT_RST);


// =====================================================
// WIFI
// =====================================================

const char* WIFI_NAME = "EcranNodeMCU";
const char* WIFI_PASSWORD = "12345678";

ESP8266WebServer server(80);


// =====================================================
// ETAT ACTUEL
// =====================================================

enum DisplayMode {
  MODE_CLEAR,
  MODE_TEXT,
  MODE_CAT,
  MODE_DOG
};

DisplayMode currentMode = MODE_CLEAR;
String currentText = "";


// =====================================================
// COULEURS
// =====================================================

#define CAT_ORANGE   0xFD20
#define DOG_BROWN    0xA145
#define DOG_LIGHT    0xFBE0
#define PINK         0xF81F

#define HEADER_COLOR ILI9341_BLUE
#define HEADER_TEXT  ILI9341_WHITE
#define BODY_COLOR   ILI9341_WHITE
#define BODY_BG      ILI9341_BLACK


// =====================================================
// PAGE WEB
// =====================================================

const char PAGE[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="fr">
<head>

<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1">

<title>Ecran NodeMCU</title>

<style>

body {
  font-family: Arial, sans-serif;
  background: #eeeeee;
  margin: 0;
  padding: 20px;
}

.container {
  max-width: 600px;
  margin: auto;
  background: white;
  padding: 20px;
  border-radius: 12px;
}

textarea {
  width: 100%;
  height: 150px;
  box-sizing: border-box;
  font-size: 18px;
  padding: 10px;
}

button {
  font-size: 18px;
  padding: 12px 20px;
  margin: 6px 4px 6px 0;
  border: none;
  border-radius: 8px;
}

.send {
  background: #2196F3;
  color: white;
}

.cat {
  background: #ff9800;
  color: white;
}

.dog {
  background: #4CAF50;
  color: white;
}

.clear {
  background: #f44336;
  color: white;
}

.status {
  margin-top: 20px;
  padding: 15px;
  background: #eeeeee;
  border-radius: 8px;
}

pre {
  white-space: pre-wrap;
  background: white;
  padding: 10px;
  border-radius: 6px;
}

</style>

</head>

<body>

<div class="container">

<h2>Ecran TFT NodeMCU</h2>

<textarea
id="message"
placeholder="Texte à afficher (4 lignes maximum)..."></textarea>

<br>

<button class="send" onclick="sendText()">Envoyer</button>
<button class="cat" onclick="showCat()">Cat</button>
<button class="dog" onclick="showDog()">Chien</button>
<button class="clear" onclick="clearScreen()">Clear</button>

<div class="status">

<h3>Affichage actuel</h3>

<p>
Type :
<strong id="mode">---</strong>
</p>

<div id="textArea">
Texte :
<pre id="displayText"></pre>
</div>

</div>

</div>


<script>

async function sendText() {

  const text =
    document.getElementById("message").value;

  const body =
    new URLSearchParams();

  body.append("text", text);

  await fetch("/send", {
    method: "POST",
    headers: {
      "Content-Type":
      "application/x-www-form-urlencoded"
    },
    body: body
  });

  updateStatus();
}


async function showCat() {

  await fetch("/cat", {
    method: "POST"
  });

  updateStatus();
}


async function showDog() {

  await fetch("/dog", {
    method: "POST"
  });

  updateStatus();
}


async function clearScreen() {

  await fetch("/clear", {
    method: "POST"
  });

  updateStatus();
}


async function updateStatus() {

  try {

    const response =
      await fetch("/status");

    const data =
      await response.json();

    const mode =
      document.getElementById("mode");

    const textArea =
      document.getElementById("textArea");

    const displayText =
      document.getElementById("displayText");


    if(data.mode === "text") {

      mode.textContent = "TEXTE";
      textArea.style.display = "block";
      displayText.textContent = data.text;

    } else if(data.mode === "cat") {

      mode.textContent = "CHAT";
      textArea.style.display = "none";

    } else if(data.mode === "dog") {

      mode.textContent = "CHIEN";
      textArea.style.display = "none";

    } else {

      mode.textContent = "VIDE";
      textArea.style.display = "none";
    }

  } catch(e) {

    console.log(e);
  }
}


setInterval(updateStatus, 1000);
updateStatus();

</script>

</body>
</html>
)rawliteral";


// =====================================================
// JSON ESCAPE
// =====================================================

String jsonEscape(const String &input) {

  String result;

  result.reserve(input.length() + 20);

  for (unsigned int i = 0; i < input.length(); i++) {

    char c = input[i];

    if (c == '"') {
      result += "\\\"";
    }
    else if (c == '\\') {
      result += "\\\\";
    }
    else if (c == '\n') {
      result += "\\n";
    }
    else if (c == '\r') {
    }
    else if (c == '\t') {
      result += "\\t";
    }
    else {
      result += c;
    }
  }

  return result;
}


// =====================================================
// CHAT
// =====================================================

void drawCat() {

  tft.fillScreen(ILI9341_CYAN);

  // oreilles
  tft.fillTriangle(
    70, 80,
    105, 20,
    125, 88,
    CAT_ORANGE
  );

  tft.fillTriangle(
    195, 88,
    215, 20,
    250, 80,
    CAT_ORANGE
  );

  // intérieur oreilles
  tft.fillTriangle(
    87, 70,
    105, 38,
    115, 75,
    PINK
  );

  tft.fillTriangle(
    205, 75,
    215, 38,
    233, 70,
    PINK
  );

  // tête
  tft.fillCircle(
    160,
    105,
    75,
    CAT_ORANGE
  );

  // yeux
  tft.fillCircle(
    130,
    95,
    22,
    ILI9341_WHITE
  );

  tft.fillCircle(
    190,
    95,
    22,
    ILI9341_WHITE
  );

  tft.fillCircle(
    130,
    95,
    11,
    ILI9341_GREEN
  );

  tft.fillCircle(
    190,
    95,
    11,
    ILI9341_GREEN
  );

  tft.fillCircle(
    130,
    95,
    5,
    ILI9341_BLACK
  );

  tft.fillCircle(
    190,
    95,
    5,
    ILI9341_BLACK
  );

  // museau
  tft.fillCircle(
    140,
    135,
    24,
    ILI9341_WHITE
  );

  tft.fillCircle(
    180,
    135,
    24,
    ILI9341_WHITE
  );

  // nez
  tft.fillTriangle(
    150, 123,
    170, 123,
    160, 138,
    PINK
  );

  // bouche
  tft.drawLine(
    160, 138,
    160, 150,
    ILI9341_BLACK
  );

  tft.drawLine(
    160, 150,
    148, 158,
    ILI9341_BLACK
  );

  tft.drawLine(
    160, 150,
    172, 158,
    ILI9341_BLACK
  );

  // moustaches
  tft.drawLine(
    140, 135,
    65, 120,
    ILI9341_BLACK
  );

  tft.drawLine(
    140, 142,
    60, 142,
    ILI9341_BLACK
  );

  tft.drawLine(
    140, 149,
    70, 165,
    ILI9341_BLACK
  );

  tft.drawLine(
    180, 135,
    255, 120,
    ILI9341_BLACK
  );

  tft.drawLine(
    180, 142,
    260, 142,
    ILI9341_BLACK
  );

  tft.drawLine(
    180, 149,
    250, 165,
    ILI9341_BLACK
  );

  // corps
  tft.fillRoundRect(
    105,
    170,
    110,
    70,
    30,
    CAT_ORANGE
  );

  // ventre
  tft.fillRoundRect(
    135,
    185,
    50,
    55,
    20,
    ILI9341_WHITE
  );

  // pattes
  tft.fillCircle(
    120,
    220,
    20,
    CAT_ORANGE
  );

  tft.fillCircle(
    200,
    220,
    20,
    CAT_ORANGE
  );
}


// =====================================================
// CHIEN
// =====================================================

void drawDog() {

  tft.fillScreen(ILI9341_CYAN);

  // =========================
  // OREILLES
  // =========================

  // gauche
  tft.fillRoundRect(
    55,
    45,
    55,
    105,
    25,
    DOG_BROWN
  );

  // droite
  tft.fillRoundRect(
    210,
    45,
    55,
    105,
    25,
    DOG_BROWN
  );


  // =========================
  // TETE
  // =========================

  tft.fillCircle(
    160,
    100,
    78,
    DOG_LIGHT
  );


  // =========================
  // YEUX
  // =========================

  tft.fillCircle(
    130,
    92,
    20,
    ILI9341_WHITE
  );

  tft.fillCircle(
    190,
    92,
    20,
    ILI9341_WHITE
  );

  tft.fillCircle(
    130,
    92,
    9,
    ILI9341_BLACK
  );

  tft.fillCircle(
    190,
    92,
    9,
    ILI9341_BLACK
  );

  // reflets
  tft.fillCircle(
    126,
    88,
    3,
    ILI9341_WHITE
  );

  tft.fillCircle(
    186,
    88,
    3,
    ILI9341_WHITE
  );


  // =========================
  // MUSEAU
  // =========================

  tft.fillRoundRect(
    120,
    120,
    80,
    48,
    22,
    ILI9341_WHITE
  );


  // =========================
  // NEZ
  // =========================

  tft.fillTriangle(
    145, 125,
    175, 125,
    160, 143,
    ILI9341_BLACK
  );


  // =========================
  // BOUCHE
  // =========================

  tft.drawLine(
    160, 143,
    160, 153,
    ILI9341_BLACK
  );

  tft.drawLine(
    160, 153,
    148, 160,
    ILI9341_BLACK
  );

  tft.drawLine(
    160, 153,
    172, 160,
    ILI9341_BLACK
  );


  // =========================
  // LANGUE
  // =========================

  tft.fillCircle(
    160,
    165,
    10,
    PINK
  );

  // masque moitié haute
  tft.fillRect(
    150,
    155,
    20,
    10,
    ILI9341_WHITE
  );


  // =========================
  // CORPS
  // =========================

  tft.fillRoundRect(
    105,
    165,
    110,
    75,
    32,
    DOG_LIGHT
  );


  // =========================
  // POITRINE
  // =========================

  tft.fillRoundRect(
    140,
    180,
    40,
    60,
    20,
    ILI9341_WHITE
  );


  // =========================
  // PATTES
  // =========================

  tft.fillCircle(
    120,
    220,
    22,
    DOG_LIGHT
  );

  tft.fillCircle(
    200,
    220,
    22,
    DOG_LIGHT
  );


  // petites griffes
  tft.drawLine(
    114, 220,
    114, 232,
    ILI9341_BLACK
  );

  tft.drawLine(
    124, 220,
    124, 232,
    ILI9341_BLACK
  );

  tft.drawLine(
    194, 220,
    194, 232,
    ILI9341_BLACK
  );

  tft.drawLine(
    204, 220,
    204, 232,
    ILI9341_BLACK
  );


/* // =========================
  // QUEUE
  // =========================

  tft.drawCircle(
    230,
    190,
    35,
    DOG_BROWN
  );

  tft.drawCircle(
    230,
    190,
    34,
    DOG_BROWN
  );

  tft.drawCircle(
    230,
    190,
    33,
    DOG_BROWN
  );*/
}


// =====================================================
// BANDEAU MESSAGE
// =====================================================

void drawMessageHeader() {

  const int HEADER_HEIGHT = 60;

  tft.fillRect(
    0,
    0,
    320,
    HEADER_HEIGHT,
    HEADER_COLOR
  );

  tft.drawFastHLine(
    0,
    HEADER_HEIGHT - 1,
    320,
    ILI9341_WHITE
  );

  tft.setTextColor(
    HEADER_TEXT
  );

  tft.setTextSize(4);
  tft.setTextWrap(false);

  const char* title = "Message";

  int16_t x1;
  int16_t y1;

  uint16_t textWidth;
  uint16_t textHeight;

  tft.getTextBounds(
    title,
    0,
    0,
    &x1,
    &y1,
    &textWidth,
    &textHeight
  );

  int titleX =
    (320 - textWidth) / 2;

  int titleY =
    (HEADER_HEIGHT - textHeight) / 2;

  titleY += 2;

  tft.setCursor(
    titleX,
    titleY
  );

  tft.print(title);
}


// =====================================================
// AFFICHAGE TEXTE
// =====================================================

void drawText() {

  tft.fillScreen(
    BODY_BG
  );

  drawMessageHeader();

  tft.setTextColor(
    BODY_COLOR
  );

  tft.setTextSize(3);
  tft.setTextWrap(false);

  const int TEXT_X = 10;
  const int TEXT_Y = 82;
  const int LINE_HEIGHT = 40;
  const int MAX_LINES = 4;

  int y = TEXT_Y;
  int lineNumber = 0;

  String line = "";

  for (unsigned int i = 0;
       i <= currentText.length();
       i++) {

    char c;

    if (i == currentText.length()) {

      c = '\n';

    } else {

      c = currentText[i];
    }


    if (c == '\r') {
      continue;
    }


    if (c == '\n') {

      if (lineNumber < MAX_LINES) {

        tft.setCursor(
          TEXT_X,
          y
        );

        tft.print(line);

        y += LINE_HEIGHT;

        lineNumber++;
      }

      line = "";

      if (lineNumber >= MAX_LINES) {
        break;
      }

    } else {

      line += c;
    }
  }
}


// =====================================================
// CLEAR
// =====================================================

void clearDisplay() {

  tft.fillScreen(
    ILI9341_BLACK
  );
}


// =====================================================
// HTTP
// =====================================================

void handleRoot() {

  server.send_P(
    200,
    "text/html",
    PAGE
  );
}


void handleSend() {

  if (!server.hasArg("text")) {

    server.send(
      400,
      "text/plain",
      "Texte manquant"
    );

    return;
  }

  currentText =
    server.arg("text");

  currentMode =
    MODE_TEXT;

  drawText();

  server.send(
    200,
    "text/plain",
    "OK"
  );
}


void handleCat() {

  currentText = "";

  currentMode =
    MODE_CAT;

  drawCat();

  server.send(
    200,
    "text/plain",
    "OK"
  );
}


void handleDog() {

  currentText = "";

  currentMode =
    MODE_DOG;

  drawDog();

  server.send(
    200,
    "text/plain",
    "OK"
  );
}


void handleClear() {

  currentText = "";

  currentMode =
    MODE_CLEAR;

  clearDisplay();

  server.send(
    200,
    "text/plain",
    "OK"
  );
}


void handleStatus() {

  String json;

  json.reserve(
    currentText.length() + 80
  );

  json = "{";


  if (currentMode == MODE_TEXT) {

    json += "\"mode\":\"text\",";

  } else if (currentMode == MODE_CAT) {

    json += "\"mode\":\"cat\",";

  } else if (currentMode == MODE_DOG) {

    json += "\"mode\":\"dog\",";

  } else {

    json += "\"mode\":\"clear\",";
  }


  json += "\"text\":\"";

  json += jsonEscape(
    currentText
  );

  json += "\"";

  json += "}";


  server.send(
    200,
    "application/json",
    json
  );
}


// =====================================================
// SETUP
// =====================================================

void setup() {

  Serial.begin(115200);

  delay(100);

  pinMode(PIN_PWR_LCD,OUTPUT);
  pinMode(PIN_BUZZER,OUTPUT);
  pinMode(PIN_BUTTON,INPUT_PULLUP);

  digitalWrite(PIN_PWR_LCD,HIGH);
  tone(PIN_BUZZER,440);
  delay(500);
  noTone(PIN_BUZZER);

  SPI.begin();

  tft.begin();

  // paysage 320 x 240
  tft.setRotation(1);

  clearDisplay();


  // ===================================================
  // POINT D'ACCES WIFI
  // ===================================================

  WiFi.mode(
    WIFI_AP
  );


  IPAddress localIP(
    192,
    168,
    0,
    1
  );

  IPAddress gateway(
    192,
    168,
    0,
    1
  );

  IPAddress subnet(
    255,
    255,
    255,
    0
  );


  WiFi.softAPConfig(
    localIP,
    gateway,
    subnet
  );


  WiFi.softAP(
    WIFI_NAME,
    WIFI_PASSWORD
  );


  Serial.println();
  Serial.println("==========================");
  Serial.println("POINT D'ACCES WIFI");

  Serial.print("SSID : ");
  Serial.println(WIFI_NAME);

  Serial.print("Mot de passe : ");
  Serial.println(WIFI_PASSWORD);

  Serial.print("Adresse : http://");
  Serial.println(WiFi.softAPIP());

  Serial.println("==========================");


  // ===================================================
  // ROUTES
  // ===================================================

  server.on(
    "/",
    HTTP_GET,
    handleRoot
  );

  server.on(
    "/send",
    HTTP_POST,
    handleSend
  );

  server.on(
    "/cat",
    HTTP_POST,
    handleCat
  );

  server.on(
    "/dog",
    HTTP_POST,
    handleDog
  );

  server.on(
    "/clear",
    HTTP_POST,
    handleClear
  );

  server.on(
    "/status",
    HTTP_GET,
    handleStatus
  );


  server.onNotFound([]() {

    server.send(
      404,
      "text/plain",
      "404"
    );

  });


  server.begin();

  Serial.println(
    "Serveur HTTP demarre"
  );

  drawCat();
}


// =====================================================
// LOOP
// =====================================================

void loop() {

  server.handleClient();

  digitalWrite(PIN_PWR_LCD,!digitalRead(PIN_BUTTON));

  yield();
}