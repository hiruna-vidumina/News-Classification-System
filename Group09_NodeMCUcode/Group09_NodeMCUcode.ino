#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266LLMNR.h>
#include <ArduinoJson.h>


// Update these with values suitable for your network.
#ifndef STASSID
#define STASSID "Hiruna Vidumina"
#define STAPSK  "hiruvidu"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;
const char* mqtt_server = "Broker.mqttdashboard.com";

WiFiClient espClient;
PubSubClient client(espClient);
ESP8266WebServer server(80);

const char* id[5];
const char* name[5];
const char* author[5];
const char* title[5];
const char* description[5];
const char* url[5];
const char* publishedAt[5];
const char* urlToImage[5];

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  randomSeed(micros());
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  char str[length+1];
  int i = 0;
  for (int i = 0; i < length; i++) {
    str[i] = (char)payload[i];
    Serial.print(str[i]);
    }
  str[i] = 0;
  Serial.println();
  StaticJsonDocument <32768> doc;
  deserializeJson(doc,payload);
  JsonArray arr = doc.as<JsonArray>();
  int j = 0;
  for (JsonObject repo : arr) {
    id[j] = (const char*)repo["source"]["id"];
    name[j] = (const char*)repo["source"]["name"];
    author[j] = (const char*)repo["author"];
    title[j] = (const char*)repo["title"];
    description[j] = (const char*)repo["description"];
    url[j] = (const char*)repo["url"];
    publishedAt[j] = (const char*)repo["publishedAt"];
    urlToImage[j] = (const char*)repo["urlToImage"];
    j++;
    }
  for (int k = 0; k < 5; k++) { 
    if (name[k] == NULL)
            name[k] = "Source is not given";
    if (author[k] == NULL)
            author[k] = "Anonymous";
    if (title[k] == NULL)
            title[k] = "No Title";
    if (description[k] == NULL)
            description[k] = " ";
    if (url[k] == NULL)
            url[k] = " ";
    if (urlToImage[k] == NULL)
            urlToImage[k] = " ";
    Serial.println("");
    Serial.println(name[k]);
    Serial.println(author[k]);
    Serial.println(title[k]);
    Serial.println(description[k]);
    Serial.println(url[k]);
    Serial.println(urlToImage[k]);
    }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // ... and resubscribe
      client.subscribe("incommingTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  // Start LLMNR responder
  LLMNR.begin("esp8266");
  Serial.println("LLMNR responder started");
  // Start HTTP server
  server.on("/", [] (){
    String page = "";
    page += "<!DOCTYPE html>\n";
    page += "<html>\n";
    page += "<body>\n";
    page += "<h1 style=\"font-size:300%;text-align:center;\">" + (String)title[0] + "</h1>\n";
    page += "<h2>" + (String)name[0] + " - " + (String)author[0] + "</h2>\n";
    page += "<p style=\"font-size:100%;\">" + (String)description[0] + "</p>\n";
    page += "<p><a href=\"" + (String)url[0] + "\">Click here for more details!</a></p>\n";
    page += "<img src=\"" + (String)urlToImage[0] + "\" width=\"500\" height=\"400\">\n";
    page += "<h1 style=\"font-size:300%;text-align:center;\">" + (String)title[1] + "</h1>\n";
    page += "<h2>" + (String)name[1] + " - " + (String)author[1] + "</h2>\n";
    page += "<p style=\"font-size:100%;\">" + (String)description[1] + "</p>\n";
    page += "<p><a href=\"" + (String)url[1] + "\">Click here for more details!</a></p>\n";
    page += "<img src=\"" + (String)urlToImage[1] + "\" width=\"500\" height=\"400\">\n";
    page += "<h1 style=\"font-size:300%;text-align:center;\">" + (String)title[2] + "</h1>\n";
    page += "<h2>" + (String)name[2] + " - " + (String)author[2] + "</h2>\n";
    page += "<p style=\"font-size:100%;\">" + (String)description[2] + "</p>\n";
    page += "<p><a href=\"" + (String)url[2] + "\">Click here for more details!</a></p>\n";
    page += "<img src=\"" + (String)urlToImage[2] + "\" width=\"500\" height=\"400\">\n";
    page += "<h1 style=\"font-size:300%;text-align:center;\">" + (String)title[3] + "</h1>\n";
    page += "<h2>" + (String)name[3] + " - " + (String)author[3] + "</h2>\n";
    page += "<p style=\"font-size:100%;\">" + (String)description[3] + "</p>\n";
    page += "<p><a href=\"" + (String)url[3] + "\">Click here for more details!</a></p>\n";
    page += "<img src=\"" + (String)urlToImage[3] + "\" width=\"500\" height=\"400\">\n";
    page += "<h1 style=\"font-size:300%;text-align:center;\">" + (String)title[4] + "</h1>\n";
    page += "<h2>" + (String)name[4] + " - " + (String)author[4] + "</h2>\n";
    page += "<p style=\"font-size:100%;\">" + (String)description[4] + "</p>\n";
    page += "<p><a href=\"" + (String)url[4] + "\">Click here for more details!</a></p>\n";
    page += "<img src=\"" + (String)urlToImage[4] + "\" width=\"500\" height=\"400\">\n";
    page += "</body>\n";
    page += "</html>\n";
    page += "";
    server.send(200, "text/html", page);
  });
  server.begin();
  Serial.println("HTTP server started");

}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  server.handleClient();
  delay(5000);
}
