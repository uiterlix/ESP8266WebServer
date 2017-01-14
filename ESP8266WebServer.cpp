#include <DHT.h>
#include <DHT_U.h>
#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "HTMLWriter.h"
#include "WiFiCredentials.h"

const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;

const int bluePin = 13;
const int redPin = 15;
const int greenPin = 12;

#define DHTTYPE DHT11
#define DHTPIN  2
#define LEDPIN 4

// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(80);
DHT dht(DHTPIN, DHTTYPE);

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, 3600);

float temp_f;  // Values read from sensor
float humid_f;
long lastPolled = 0;

void setLedToRed() {
	analogWrite(bluePin, 0);
	analogWrite(redPin, 600);
	analogWrite(greenPin, 0);
}

void setLedToGreen() {
	analogWrite(bluePin, 0);
	analogWrite(redPin, 0);
	analogWrite(greenPin, 600);
}

void setLedToBlue() {
	analogWrite(bluePin, 600);
	analogWrite(redPin, 0);
	analogWrite(greenPin, 0);
}

void connectWiFi() {
	// Connect to WiFi network
	setLedToRed();
	Serial.print("Connecting to ");
	Serial.println(ssid);
	timeClient.end();
	WiFi.disconnect();
	WiFi.begin(ssid, password);
	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}
	Serial.println("");
	Serial.println("WiFi connected");
	// Start the server
	server.begin();
	Serial.println("Server started");
	// Print the IP address
	Serial.println(WiFi.localIP());
	timeClient.begin();
	setLedToGreen();
}

void setup() {
	Serial.begin(115200);
	delay(10);

	// prepare GPIO3
	pinMode(LEDPIN, OUTPUT);
	digitalWrite(LEDPIN, 0);

	// Connect to WiFi network
	connectWiFi();
	dht.begin();

}

void loop() {
	// Check if WiFi is still connected
	if (WiFi.status() != WL_CONNECTED) {
		Serial.println("WiFi was disconnected. Attempt to reconnect.");
		connectWiFi();
	}

	// Check if a client has connected
	WiFiClient client = server.available();
	if (!client) {
		return;
	}

	// Wait until the client sends some data
	Serial.println("new client");
	setLedToBlue();
	timeClient.update();

	for (int i = 0; i < 100; i++) {
		if (client.available())
			break;
		delay(10);
	}

	if (!client.available()) {
		// terminate this client, nothing we can do with it
		Serial.println("Client timed out... Stopping client.");
		client.stop();
		setLedToGreen();
		return;
	}

	// Read the first line of the request
	String req = client.readStringUntil('\r');
	Serial.println(req);
	client.flush();

	// Match the request
	int val;
	String s;
	if (req.indexOf("/gpio/0") != -1) {
		val = 0;
		digitalWrite(LEDPIN, val);
		s =
				"HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\nGPIO is now 0</html>";
	} else if (req.indexOf("/gpio/1") != -1) {
		val = 1;
		digitalWrite(LEDPIN, val);
		s =
				"HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\nGPIO is now 1</html>";
	} else if (req.indexOf("/ ") != -1) {
		// read with raw sample data.
		long now = millis();
		if (now - lastPolled > 3000) {
			// read temperature every 3 seconds max.
			temp_f = dht.readTemperature();
			humid_f = dht.readHumidity();
			lastPolled = millis();
		}
		String formattedTime = timeClient.getFormattedTime();
		char time[100];
		formattedTime.toCharArray(time, 100);
		HtmlWriter writer;
		writer.startHtml()
			.startStyle()
				.text("div.content {")
				.text("font-family: monospace;")
				.text("font-size: 20px;")
				.text("text-align: center;")
				.text("margin-top: 20px; }")
			.endStyle()
			.startBody()
				.startDiv("content")
					.text("Tijd: ")
					.text(time)
					.br()
					.text("Temperatuur: ")
					.text(temp_f)
					.br()
					.text("Luchtvochtigheid: ")
					.text(humid_f)
				.endDiv()
			.endBody()
		.endHtml();
		s = String(writer.build());
	} else {
		// send 404
		Serial.println("404");
		client.print("HTTP/1.1 404 OK\n");
		client.flush();
		setLedToGreen();
		return;
	}

	client.flush();

	// Send the response to the client
	client.print(s);
	delay(1);
	Serial.println("Client disconnected");
	setLedToGreen();
	// The client will actually be disconnected
	// when the function returns and 'client' object is detroyed
}

