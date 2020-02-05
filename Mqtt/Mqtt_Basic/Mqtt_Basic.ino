#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.

const char* ssid = "";
const char* password = "";

const char* mqtt_server = "";

void callback(char* topic, byte* payload, unsigned int length);


WiFiClient espClient;
PubSubClient client(mqtt_server, 1883 , callback, espClient);


void setup_wifi() {
    delay(10);
    // We start by connecting to a WiFi network
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

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
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    for (int i = 0; i < length; i++) {
        Serial.print((char)payload[i]);
    }
    Serial.println();

}

void setup() {
    pinMode(BUILTIN_LED, OUTPUT);  // Initialize the BUILTIN_LED pin as an output
	
    Serial.begin(115200);
	
    setup_wifi();
	
    if (client.connect("arduinoClient", "testUser", "testPass")) {
        client.publish("outTopic", "hello world");
        client.subscribe("inTopic");
    }
	
}

void loop() {
	
    client.loop();

}