#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include "DHT.h"

// Initialize Wifi connection to the router
char ssid[] = "DSC";     // یوزر نیم مودم یا روتر وای فای
char password[] = "1qazxsw2"; // پسوورد روتر یا مودم وای فای

// Initialize Telegram BOT

#define BOTtoken "526698318:AAHVZsMwPFaOmRk-MpujqqyYDba1sWvaxDo"  // توکن ربات تلگرام
#define DHTPIN D7     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)

DHT dht(DHTPIN, DHTTYPE);
WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

int Bot_mtbs = 1000;
long Bot_lasttime;
bool Start = false;

const int relay1 = D1;
const int relay2 = D2;
const int relay3 = D3;
const int relay4 = D4;
int relaystatus = 0;

void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));


  for (int i = 0; i < numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    String text = bot.messages[i].text;

    String from_name = bot.messages[i].from_name;
    if (from_name == "") from_name = "Guest";


    if (text == "/HELLO_SHZIOT_SeverRoom") {
      String keyboardJson = "[[\"Server Room Temp\", \"Server Room Humid\"],[\"Relay One OFF\", \"Relay One ON\"],[\"Relay Two OFF\", \"Relay Two ON\"],[\"Relay Three OFF\", \"Relay Three ON\"],[\"Relay Four OFF\", \"Relay Four ON\"],[\"INFO\", \"About\"]]";
      bot.sendMessageWithReplyKeyboard(chat_id, "HI Body, NOW You Can Control Server Room", "", keyboardJson, true);
      bot.sendChatAction(chat_id, "typing");
      delay(10);
      bot.sendMessage(chat_id, "PLEASE USE KEYBOARD");
    }

    if (text == "Relay One OFF") {
      digitalWrite(relay1, LOW);   // turn the LED on (HIGH is the voltage level)
      relaystatus = 1;
      bot.sendMessage(chat_id, "Relay One is OFF", "");
    }

    if (text == "Relay One ON") {
      digitalWrite(relay1, HIGH);    // turn the LED off (LOW is the voltage level)
      relaystatus = HIGH;
      bot.sendMessage(chat_id, "Relay One is ON", "");
    }

    if (text == "Relay Two ON") {
      digitalWrite(relay2, HIGH);   // turn the LED on (HIGH is the voltage level)
      relaystatus = 1;
      bot.sendMessage(chat_id, "Relay Two is ON", "");
    }

    if (text == "Relay Two OFF") {
      digitalWrite(relay2, LOW);    // turn the LED off (LOW is the voltage level)
      relaystatus = HIGH;
      bot.sendMessage(chat_id, "Relay Two is OFF", "");
    }

    if (text == "Relay Three OFF") {
      digitalWrite(relay3, LOW);   // turn the LED on (HIGH is the voltage level)
      relaystatus = 1;
      bot.sendMessage(chat_id, "Room 3 is on", "");
    }

    if (text == "Relay Three ON") {
      digitalWrite(relay3, HIGH);    // turn the LED off (LOW is the voltage level)
      relaystatus = HIGH;
      bot.sendMessage(chat_id, "Room 3 turned OFF", "");
    }


    if (text == "Relay Four OFF") {
      digitalWrite(relay4, LOW);   // turn the LED on (HIGH is the voltage level)
      relaystatus = 1;
      bot.sendMessage(chat_id, "Room 4 is on", "");
    }

    if (text == "Relay Four ON") {
      digitalWrite(relay4, HIGH);    // turn the LED off (LOW is the voltage level)
      relaystatus = HIGH;
      bot.sendMessage(chat_id, "Room 4 turned OFF", "");
    }

    if (text == "Server Room Temp") {
      float t = dht.readTemperature();
      bot.sendMessage(chat_id, "Tempture is : " + String(t)+" *c", "");
    }

    if (text == "Server Room Humid") {
     float h = dht.readHumidity();
      bot.sendMessage(chat_id, "Humidity is : " + String(h)+"%", "");
    }



    if (text == "About" ) {
      bot.sendChatAction(chat_id, "typing");
      delay(100);
      bot.sendMessage(chat_id, "This System Monitors Tempture & Humidity Of Server Room And you can Control Four Relays");
    }
    if (text == "INFO" ) {
      bot.sendChatAction(chat_id, "typing");
      delay(100);
      bot.sendMessage(chat_id, "This System designed By Hamed Rahimi At SHZIOT.ir /");
    }

    // You can't use own message, just choose from one of bellow

    //typing for text messages
    //upload_photo for photos
    //record_video or upload_video for videos
    //record_audio or upload_audio for audio files
    //upload_document for general files
    //find_location for location data

    //more info here - https://core.telegram.org/bots/api#sendchataction


    
    if (text == "/start") {
      String welcome = "Hello " + from_name + ".\n";
      welcome += "Welcome to SHZIOT Server Room System .\n " ;
      welcome += "press /HELLO_SHZIOT_SeverRoom To Start Bot .\n\n";
      bot.sendMessage(chat_id, welcome);
    }
  }
}


void setup() {
  Serial.begin(115200);
  Serial.println("DHT22 test!");
  dht.begin();
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  // attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  digitalWrite(relay1, LOW); // initialize pin as off
  pinMode(D1, OUTPUT); // initialize digital ledPin as an output.
  delay(10);
  digitalWrite(relay2, LOW); // initialize pin as off

  pinMode(D2, OUTPUT); // initialize digital ledPin as an output.
  delay(10);
  digitalWrite(relay3, LOW); // initialize pin as off

  pinMode(D3, OUTPUT); // initialize digital ledPin as an output.
  delay(10);
  digitalWrite(relay4, LOW); // initialize pin as off

  pinMode(D4, OUTPUT); // initialize digital ledPin as an output.
  delay(10);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {





  if (millis() > Bot_lasttime + Bot_mtbs)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while (numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }

    Bot_lasttime = millis();
  }




  /*

    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
   
    

    // check if returns are valid, if they are NaN (not a number) then something went wrong!
    if (isnan(t) || isnan(h)) {
      Serial.println("Failed to read from DHT");
    }
    else {
      Serial.print("Humidity: ");
      Serial.print(h);
      Serial.print(" %\t");
      Serial.print("Temperature: ");
      Serial.print(t);
      Serial.println(" *C");
    }
    delay(1000);
  */
}
