#include <ArduinoJson.hpp>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#define BLYNK_PRINT Serial

enum status_code
{
	error = -1,
	ready = 1, 
	wifi_connecting,
	wifi_connected,
	wifi_disconnected,
	blynk_ready
};

DynamicJsonBuffer jsonBuffer(1024);
const char* command;
status_code status = error;

void status_print(int status) {
	switch (status) {
	case ready: 	Serial.println("ready");
		break;
	case wifi_connecting: 	Serial.println("WIFI connecting");
		break;
	case wifi_connected: 	Serial.println("WIFI connected");
		break;
	case wifi_disconnected: 	Serial.println("WIFI disconnected");
		break;
	case blynk_ready: 	Serial.println("blynk Ready");
		break;
	default: 	Serial.println("error");
		break;
	}
}

int parsecommand(JsonObject& jsonObj) {

	const char* command = jsonObj["command"];
		if (command != "") {
			if (strcmp(command, "status") == 0) {
				status = ready;
			}
			else if (strcmp(command, "connect") == 0) {
				//const char* SSID = jsonObj["SSID"];
				//const char* Password = jsonObj["Password"];
				//const char* auth_token = jsonObj["AuthToken"];
				const char* ssid = "";
				const char* pass = "";
				const char* auth = "f4057864ca204b55ae987c03a82cf7d7";
				if (ssid && pass && auth) {
					status = wifi_connecting;
					status_print(status);
					Blynk.begin(auth, ssid, pass);
					status_print(status);
				}
				else
				{
					status = error;
				}
			}
			else if (!strcmp(command, "disconnect")) {
				WiFi.disconnect();
				status = wifi_disconnected;
			}
				status_print(status);
				return 1; //no commnad
		}
		else {
			return 0; //no commnad
		}
}

void setup() {
	// Initialize serial port
	Serial.begin(9600);
	while (!Serial) continue;
	Serial.println("boot");
	char ssid[] = "";
	char pass[] = "";
	char auth[] = "f4057864ca204b55ae987c03a82cf7d7";
	Blynk.begin(auth, ssid, pass);
}

void loop() {
	// reply only when you receive data:
	if (Serial.available() > 0) {
		JsonObject& root = jsonBuffer.parseObject(Serial);
		status = error;
		if (root.success()) {
			if (parsecommand(root)) {
				Serial.flush();
				status = ready;
			}
		}
	}
}