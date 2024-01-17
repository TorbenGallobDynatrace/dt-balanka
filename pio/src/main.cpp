#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <MFRC522.h>


#define SSID "iPhone von Torben"
#define PASSWORD "JuliaUndTorben"
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
#define WELCOME_DELAY_MS 5000
#define CHAR_WIDTH 6 // Assuming each character is 6 pixels wide
#define BUTTON_PIN_1 32
#define BUTTON_PIN_2 33
#define BUTTON_PIN_3 25
#define BUTTON_PIN_4 26
#define MIN_INTERRUPT_INTERVAL 500
#define RST_PIN 15
#define SS_PIN 5
#define ANON_PLAYER "Anon"
#define ANON_PLAYER_TAG_ID "Anon"

MFRC522 mfrc522(SS_PIN, RST_PIN);

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

WiFiMulti wifiMulti;

enum AppState
{
  WELCOME,
  LOADING_PLAYERS,
  LOAD_PLAYERS_ERROR,
  REGISTER_PLAYER,
  TEAM_OVERVIEW,
  SCORE_DISPLAY,
  SUBMIT_RESULT_QUESTION,
  SUBMITTING_RESULT,
  SUBMIT_RESULT_SUCCESS,
  SUBMIT_RESULT_ERROR
};

struct Player
{
  String tag_id;
  String nickname;
};

Player allPlayers[1000];
int allPlayersCount = 0;

AppState currentState = WELCOME;

String player1 = ANON_PLAYER;
String player1TagID = ANON_PLAYER_TAG_ID;
String player2 = ANON_PLAYER;
String player2TagID = ANON_PLAYER_TAG_ID;
String player3 = ANON_PLAYER;
String player3TagID = ANON_PLAYER_TAG_ID;
String player4 = ANON_PLAYER;
String player4TagID = ANON_PLAYER_TAG_ID;

int setupPlayer = 1;

int scoreA = 0;
int scoreB = 0;

bool button1Pressed = false;
bool button2Pressed = false;
bool button3Pressed = false;
bool button4Pressed = false;

unsigned long lastButton1Press = 0;
unsigned long lastButton2Press = 0;
unsigned long lastButton3Press = 0;
unsigned long lastButton4Press = 0;

void printCenteredText(String text, int y)
{
  int textWidth = text.length() * CHAR_WIDTH; 
  int x = (SCREEN_WIDTH - textWidth) / 2;
  display.setCursor(x, y);
  display.println(text);
}

void printRegisterPlayer()
{
  display.clearDisplay();
  printCenteredText("Register", 0);
  if(setupPlayer == 1 || setupPlayer == 2) {
    printCenteredText("Team A: Player " + String(setupPlayer), 10);
  } else if(setupPlayer == 3 || setupPlayer == 4) {
    printCenteredText("Team B: Player " + String(setupPlayer), 10);
  }

  if(setupPlayer == 1) {
    if(player1 == ANON_PLAYER) {
      printCenteredText("tag >>> reader", 20);
    } else {
      printCenteredText(player1, 20);
    }
  } else if(setupPlayer == 2) {
    if(player2 == ANON_PLAYER) {
      printCenteredText("tag >>> reader", 20);
    } else {
      printCenteredText(player2, 20);
    }
  } else if(setupPlayer == 3) {
    if(player3 == ANON_PLAYER) {
      printCenteredText("tag >>> reader", 20);
    } else {
      printCenteredText(player3, 20);
    }
  } else if(setupPlayer == 4) {
    if(player4 == ANON_PLAYER) {
      printCenteredText("tag >>> reader", 20);
    } else {
      printCenteredText(player4, 20);
    }
  }

  display.display();
}

void showWelcome()
{
  display.clearDisplay();
  printCenteredText("welcome to", 0);
  printCenteredText("Dynatrace", 10);
  printCenteredText("BALANKA", 20);
  display.display();
  delay(3000);
}

void loadPlayers()
{

  HTTPClient http;

  Serial.println("[HTTP] Begin loading players...");

  http.begin("https://dt-balanka.projektvorschau.at/players.php"); 

  int httpCode = http.GET();


  if (httpCode == HTTP_CODE_OK)
  {
    Serial.println("[HTTP] Players loaded successfully.");

    String payload = http.getString();

    Serial.println("[HTTP] response payload: " + payload);

    Serial.println("[JSON] parsing response payload...");

    DynamicJsonDocument doc(102400);
    DeserializationError error = deserializeJson(doc, payload);

    if (error)
    {
      Serial.println("[JSON] parse error!");
      currentState = LOAD_PLAYERS_ERROR;
      return;
    }

    int numLoadedPlayers = doc.size();
    allPlayersCount = numLoadedPlayers;

    for (int i = 0; i < numLoadedPlayers; i++)
    {
      allPlayers[i].tag_id = doc[i]["tag_id"].as<String>();
      allPlayers[i].nickname = doc[i]["nickname"].as<String>();
    }

    Serial.println("[JSON] players parsed...");

    currentState = REGISTER_PLAYER;
  }
    else
  {
    currentState = LOAD_PLAYERS_ERROR;
    Serial.printf("[HTTP] Error loading players! Error: %s\n", http.errorToString(httpCode).c_str());
  }
}

void printLoadPlayersError()
{
  display.clearDisplay();
  printCenteredText("Error loading", 0);
  printCenteredText("players!", 10);
  printCenteredText("Please try again", 20);
  display.display();
}

void IRAM_ATTR buttonOneInterrupt()
{
  if (millis() - lastButton1Press > MIN_INTERRUPT_INTERVAL)
  {
    button1Pressed = true;
    lastButton1Press = millis();
  }
}

void IRAM_ATTR buttonTwoInterrupt()
{
  if (millis() - lastButton2Press > MIN_INTERRUPT_INTERVAL)
  {
    button2Pressed = true;
    lastButton2Press = millis();
  }
}

void IRAM_ATTR buttonThreeInterrupt()
{
  if (millis() - lastButton3Press > MIN_INTERRUPT_INTERVAL)
  {
    button3Pressed = true;
    lastButton3Press = millis();
  }
}

void IRAM_ATTR buttonFourInterrupt()
{
  if (millis() - lastButton4Press > MIN_INTERRUPT_INTERVAL)
  {
    button4Pressed = true;
    lastButton4Press = millis();
  }
}

// https://forum.arduino.cc/t/convert-mfrc522-uid-to-string-byte-array-to-string/613945
void uid_to_string(byte array[], unsigned int len, char buffer[])
{
  for (unsigned int i = 0; i < len; i++)
  {
    byte nib1 = (array[i] >> 4) & 0x0F;
    byte nib2 = (array[i] >> 0) & 0x0F;
    buffer[i * 2 + 0] = nib1 < 0xA ? '0' + nib1 : 'A' + nib1 - 0xA;
    buffer[i * 2 + 1] = nib2 < 0xA ? '0' + nib2 : 'A' + nib2 - 0xA;
  }
  buffer[len * 2] = '\0';
}

String findNicknameByTagID(const String &searchTagID)
{
  for (int i = 0; i < allPlayersCount; i++)
  {
    if (allPlayers[i].tag_id == searchTagID)
    {
      return allPlayers[i].nickname; // Found a match, return the nickname
    }
  }

  return ANON_PLAYER;
}

void RFIDReaderLoop()
{
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial())
  {
    char uid[32] = "";
    uid_to_string(mfrc522.uid.uidByte, mfrc522.uid.size, uid);

    Serial.print("UID: ");
    Serial.println(uid);

    String nickname = findNicknameByTagID(uid);

    Serial.print("Nickname: ");
    Serial.println(nickname);

    if (setupPlayer == 1)
    {
      player1 = nickname;
      player1TagID = uid;
    }
    else if (setupPlayer == 2)
    {
      player2 = nickname;
      player2TagID = uid;
    }
    else if (setupPlayer == 3)
    {
      player3 = nickname;
      player3TagID = uid;
    }
    else if (setupPlayer == 4)
    {
      player4 = nickname;
      player4TagID = uid;
    }

    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
  }
}

void handleTeamOverviewButtonInput() 
{
  if(button1Pressed) {
    Serial.println("Button 1 pressed. nothing to do");
    button1Pressed = false;
  }
  if(button2Pressed) {
    Serial.println("Button 2 pressed. Go back to register players");
    currentState = REGISTER_PLAYER;
    button2Pressed = false;
  }
  if(button3Pressed) {
    Serial.println("Button 3 pressed. Go back to register players");
    currentState = REGISTER_PLAYER;
    button3Pressed = false;
  }
  if(button4Pressed) {
    Serial.println("Button 4 pressed. Start game - show Score screen");
    currentState = SCORE_DISPLAY;
    button4Pressed = false;
  }
}

void printScoreDisplay()
{
  display.clearDisplay();
  printCenteredText(player1 + "&" + player2, 0);
  printCenteredText(String(scoreA) + " : " + String(scoreB), 10);
  printCenteredText(player3 + "&" + player4, 20);
  display.display();
}

void handleScoreScreenButtonInput()
{
  if(button1Pressed) {
    Serial.println("Button 1 pressed. nothing to do");
    button1Pressed = false;
  }
  if(button2Pressed) {
    Serial.println("Button 2 pressed. Add point for team A");
    scoreA++;
    button2Pressed = false;
  }
  if(button3Pressed) {
    Serial.println("Button 3 pressed. Add point for team B");
    scoreB++;
    button3Pressed = false;
  }
  if(button4Pressed) {
    Serial.println("Button 4 pressed. Show submit result question");
    currentState = SUBMIT_RESULT_QUESTION;
    button4Pressed = false;
  }
}

void resetGame()
{
  showWelcome();
  currentState = LOADING_PLAYERS;
  player1 = ANON_PLAYER;
  player1TagID = ANON_PLAYER_TAG_ID;
  player2 = ANON_PLAYER;
  player2TagID = ANON_PLAYER_TAG_ID;
  player3 = ANON_PLAYER;
  player3TagID = ANON_PLAYER_TAG_ID;
  player4 = ANON_PLAYER;
  player4TagID = ANON_PLAYER_TAG_ID;
  scoreA = 0;
  scoreB = 0;
  setupPlayer = 1;
}

void printTeamOverview()
{
  display.clearDisplay();
  printCenteredText("TEAMS OK?", 0);
  printCenteredText(player1 + " & " + player2, 10);
  printCenteredText(player3 + " & " + player4, 20);
  display.display();
}

  void setup()
  {
    Serial.begin(115200);

    Wire.begin(21, 22);

    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
    {
      Serial.println("Screen Error: SSD1306 allocation failed");
      for (;;)
        ; // Don't proceed, loop forever
    }

    // set display settings
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);

    // initialize buttons
    pinMode(BUTTON_PIN_1, INPUT_PULLUP);
    pinMode(BUTTON_PIN_2, INPUT_PULLUP);
    pinMode(BUTTON_PIN_3, INPUT_PULLUP);
    pinMode(BUTTON_PIN_4, INPUT_PULLUP);

    attachInterrupt(digitalPinToInterrupt(BUTTON_PIN_1), buttonOneInterrupt, FALLING);
    attachInterrupt(digitalPinToInterrupt(BUTTON_PIN_2), buttonTwoInterrupt, FALLING);
    attachInterrupt(digitalPinToInterrupt(BUTTON_PIN_3), buttonThreeInterrupt, FALLING);
    attachInterrupt(digitalPinToInterrupt(BUTTON_PIN_4), buttonFourInterrupt, FALLING);

    // initialize RFID reader
    SPI.begin();
    mfrc522.PCD_Init();

    // setup WiFi
    wifiMulti.addAP(SSID, PASSWORD);

    // show welcome screen, reset game
    resetGame();
  }

  void printSubmitResultQuestion()
  {
    display.clearDisplay();
    printCenteredText("Submit result?", 0);
    printCenteredText("A: " + String(scoreA) + ", B: " + String(scoreB), 10);
    display.display();
  }

  void handleResultQuestionButtonInput()
  {
    if(button1Pressed) {
      Serial.println("Button 1 pressed. nothing to do");
      button1Pressed = false;
    }
    if(button2Pressed) {
      Serial.println("Button 2 pressed. Back to score screen");
      currentState = SCORE_DISPLAY;
      button2Pressed = false;
    }
    if(button3Pressed) {
      Serial.println("Button 3 pressed. Back to score screen");
      currentState = SCORE_DISPLAY;
      button3Pressed = false;
    }
    if(button4Pressed) {
      Serial.println("Button 4 pressed. Submit result");
      currentState = SUBMITTING_RESULT;
      button4Pressed = false;
    }
  }

  void handleRegisterPlayerButtonInput()
  {
    if (button1Pressed)
    {
      Serial.println("Button 1 pressed. nothing to do");
      button1Pressed = false;
    }
    if (button2Pressed)
    {
      Serial.println("Button 2 pressed. Switch to previous player");
      if (setupPlayer > 1)
      {
        setupPlayer--;
      } else {
      setupPlayer = 4;
    }
    button2Pressed = false;
  }
  if (button3Pressed)
  {
    Serial.println("Button 3 pressed. Switch to next player");
    if(setupPlayer < 4) {
      setupPlayer++;
    } else {
      setupPlayer = 1;
    }
    button3Pressed = false;
  }
  if (button4Pressed)
  {
    Serial.println("Button 4 pressed. Show Team configuration");
    currentState = TEAM_OVERVIEW;
    button4Pressed = false;
  }
}

void printSubmitResultSuccess()
{
  display.clearDisplay();
  printCenteredText("Result submitted", 0);
  printCenteredText("successfully!", 10);
  display.display();
}

void handleResultScreenButtonInput() {
  // reset state if any button pressed and show welcome screen and then register players
  if(button1Pressed || button2Pressed || button3Pressed || button4Pressed) {
    Serial.println("Button pressed. Resetting state");
    resetGame();
  }
}

void submitResult()
{
  HTTPClient http;

  Serial.println("[HTTP] Begin submitting result...");

  http.begin("http://dt-balanka.projektvorschau.at/results.php");

  http.addHeader("Content-Type", "application/json");

  String payload = "{ \"tap1\": \"" + player1TagID + "\", \"tap2\": \"" + player2TagID + "\", \"tbp1\": \"" + player3TagID + "\", \"tbp2\": \"" + player4TagID + "\", \"score_a\": " + String(scoreA) + ", \"score_b\": " + String(scoreB) + " }";

  Serial.println("[HTTP] payload: " + payload);

  int httpCode = http.POST(payload);

  if (httpCode == HTTP_CODE_OK)
  {
    Serial.println("[HTTP] Result submitted successfully.");
    currentState = SUBMIT_RESULT_SUCCESS;
  }
  else
  {
    currentState = SUBMIT_RESULT_ERROR;
    Serial.printf("[HTTP] Error submitting result! Error: %s\n", http.errorToString(httpCode).c_str());
  }
}

void printSubmitResultError()
{
  display.clearDisplay();
  printCenteredText("Error submitting", 0);
  printCenteredText("result!", 10);
  display.display();
}

    void
    loop()
{
  switch(currentState) {
    case WELCOME:
      resetGame();
      break;
    case LOADING_PLAYERS:
      if (wifiMulti.run() == WL_CONNECTED)
      {
        loadPlayers();
      }
      break;
    case LOAD_PLAYERS_ERROR:
      printLoadPlayersError();
      delay(1000);
      break;
    case REGISTER_PLAYER:
      printRegisterPlayer();
      handleRegisterPlayerButtonInput();
      RFIDReaderLoop();
      break;
    case TEAM_OVERVIEW:
      printTeamOverview();
      handleTeamOverviewButtonInput();
      break;
    case SCORE_DISPLAY:
      printScoreDisplay();
      handleScoreScreenButtonInput();
      break;
    case SUBMIT_RESULT_QUESTION:
      printSubmitResultQuestion();
      handleResultQuestionButtonInput();
      break;
    case SUBMITTING_RESULT:
      submitResult();
      break;
    case SUBMIT_RESULT_SUCCESS:
      printSubmitResultSuccess();
      handleResultScreenButtonInput();
      break;
    case SUBMIT_RESULT_ERROR:
      printSubmitResultError();
      handleResultScreenButtonInput();
      break;
    }
}
