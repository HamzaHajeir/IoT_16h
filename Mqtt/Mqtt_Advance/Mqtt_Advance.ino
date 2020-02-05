#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.

const char* ssid = "";
const char* password = "";


const char* mqtt_server = "";

void callback(char* topic, byte* payload, unsigned int length);

WiFiClient espClient;

PubSubClient client(mqtt_server, 1883 , callback, espClient);

unsigned long lastMsg = 0;

#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

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

    // Switch on the LED if an 1 was received as first character
    if ((char)payload[0] == '1') {
        digitalWrite(BUILTIN_LED, LOW);  // Turn the LED on (Note that LOW is the voltage level
                                         // but actually the LED is on; this is because
                                         // it is active low on the ESP-01)
    } else {
        digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
    }
}

void reconnect() {
    // Loop until we're reconnected
    while (!client.connected()) {
        Serial.print("Attempting MQTT connection...");

        // Attempt to connect
        if (client.connect("arduinoClient", "testUser", "testPass")) {
            Serial.println("connected");
			
            // Once connected, publish an announcement...
            client.publish("outTopic", "hello world");
			
            // ... and resubscribe
            client.subscribe("inTopic");
			
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
    pinMode(BUILTIN_LED, OUTPUT);  // Initialize the BUILTIN_LED pin as an output
	
    Serial.begin(115200);
	
    setup_wifi();
	
    if (client.connect("arduinoClient", "testUser", "testPass")) {
        client.publish("outTopic", "hello world");
        client.subscribe("inTopic");
    }
	
}

void loop() {
    if (!client.connected()) {
        reconnect();
    }
	
    client.loop();

    unsigned long now = millis();
    if (now - lastMsg > 2000) {
        lastMsg = now;
        ++value;
        snprintf(msg, MSG_BUFFER_SIZE, "hello world #%ld", value);
		
        Serial.print("Publish message: ");
        Serial.println(msg);
        client.publish("outTopic", msg);
    }
}