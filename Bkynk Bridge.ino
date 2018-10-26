#include <ArduinoJson.h>
DynamicJsonBuffer jsonBuffer(1024);
const char* command;

enum status_code
{
	ready = 0, connected, blynk_ready, error = -1
};


int parsecommand(const char * command) {
	if (!strcmp(command, "status")) {
		return 1;
	}
	return 0;
}

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

			if (parsecommand(root["command"])){
				status = ready;
			}

			//if (!strcmp(root["command"], "status")) {
			//	status = ready;
			//}
		}
		Serial.println(status);
	}
}