/*
 * WiFiMQTT.h
 *
 *  Created on: 08.01.2017
 *      Author: Johannes
 */
#include "Arduino.h"
//#include <ESP8266WiFi.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <map>

#ifndef WiFiMQTT_H_
#define WiFiMQTT_H_

class WiFiMQTT
{
public:
	WiFiMQTT();
	WiFiMQTT(int i);
	WiFiMQTT(String SSId, String PSk, String MQTT_BROKEr, String ClientNamE);
	virtual ~WiFiMQTT();
	void setup_wifi();
	void setup_MQTT(MQTT_CALLBACK_SIGNATURE);
	void setup(MQTT_CALLBACK_SIGNATURE);
	void callback1(char *topic, byte *payload, unsigned int length);

	void reconnect();
	void loop();
	void addTopic(String topic);
	void publish(const char *topic, const char *payload);
	String getValueFromTopic(String topic);
	void setConfig(String SSId, String PSk, String MQTT_BROKEr, String ClientNamE);

private:
	String SSID;
	String PSK;
	String MQTT_BROKER;
	String ClientName;

	WiFiClient espClient;
	PubSubClient client;
	std::map<String, String> topics1;
	int numberTopics = 0;
};

#endif /* WiFiMQTT */
