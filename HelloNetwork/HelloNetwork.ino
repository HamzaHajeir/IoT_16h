#include <ESP8266WiFi.h>



const char* ssid = "YourNetworkSSID";
const char* pass = "YourNetworkPASS";


void setup(){
	
	// put your setup code here, to run once:
	Serial.begin(115200);

	// We start by connecting to a WiFi network

	Serial.println();
	Serial.println();
	Serial.print("Connecting to ");
	Serial.println(ssid);


	WiFi.mode(WIFI_STA);
	WiFi.begin(ssid, pass);

	WiFi.waitForConnectResult();

	if(WiFi.status() == WL_CONNECTED ){
	  Serial.println("");
	  Serial.println("WiFi connected");
	  Serial.println("IP address: ");
	  Serial.println(WiFi.localIP());
	}


	
}