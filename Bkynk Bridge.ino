#include <ArduinoJson.h>
DynamicJsonBuffer jsonBuffer(1024);
const char* command;

enum status_code
{
	ready=0,connected,blynk_ready,error=-1
};

status_code status = ready;

void setup() {
	// Initialize serial port
	Serial.begin(9600);
	while (!Serial) continue;
}

void loop() {

	// reply only when you receive data:
	if (Serial.available() > 0) {
		JsonObject& root = jsonBuffer.parseObject(Serial);
		status = error;
		if (root.success()) {
			command = root["command"];
			if (strcmp(command, "status") == 0) {
				status = ready;
			}
		}
		Serial.println(status);
	}
}