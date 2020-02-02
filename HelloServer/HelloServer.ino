#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#ifndef STASSID
#define STASSID "ORANGE_3"
#define STAPSK  "RNJPDJUT"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

ESP8266WebServer server(80);

const int green = D7;
const int red = D8;
const int blue = D6;

void handleRoot() {
  //  digitalWrite(led, !digitalRead(led));
  server.send(200, "text/plain", "hello from esp8266!");
  //  digitalWrite(led, 0);
}

void handleNotFound() {
  //  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
    if (server.argName(i) == "r")  {
      Serial.println("r = " + server.arg(i));
      analogWrite(red, server.arg(i).toInt());
    }
    else if (server.argName(i) == "g") {
      Serial.println("g = " + server.arg(i));
      analogWrite(green, server.arg(i).toInt());
    }
    else if (server.argName(i) == "b") {
      Serial.println("b = " + server.arg(i));
      analogWrite(blue, server.arg(i).toInt());
    }
  }
  server.send(404, "text/plain", message);
  //  digitalWrite(led, 0);
}

void setup(void) {
  pinMode(red, OUTPUT);
  pinMode(green , OUTPUT);
  pinMode(blue, OUTPUT);
  //  digitalWrite(led, 0);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });
  server.on("/hamza", []() {
    server.send(502, "text/plain", "Error auth");
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
  MDNS.update();
}
