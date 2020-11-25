/*
 * Blinker.cpp
 *
 *  Created on: 08.01.2017
 *      Author: Johannes
 */

#include "WiFiMQTT.h"

WiFiMQTT::WiFiMQTT(): client(espClient)
{
	WiFiClient espClient;
	PubSubClient client(espClient);
	// TODO Auto-generated constructor stub
}
WiFiMQTT::WiFiMQTT(String SSId, String PSk, String MQTT_BROKEr, String ClientNamE) : client(espClient)
{
	SSID = SSId;
	PSK = PSk;
	MQTT_BROKER = MQTT_BROKEr;
	ClientName = ClientNamE;
}


WiFiMQTT::~WiFiMQTT()
{
	// TODO Auto-generated destructor stub
}

void WiFiMQTT::setConfig(String SSId, String PSk, String MQTT_BROKEr, String ClientNamE) 
{
	SSID = SSId;
	PSK = PSk;
	MQTT_BROKER = MQTT_BROKEr;
	ClientName = ClientNamE;

		
}

 void  WiFiMQTT::callback1(char *topic, byte *payload, unsigned int length)
{
	Serial.print("Message arrived [");
	Serial.print(topic);
	Serial.print("] ");
	String spayload;

	for (unsigned int i = 0; i < length; i++)
	{

		Serial.print((char)payload[i]);

		spayload.concat((char)payload[i]);
	}
	topics1[topic] = spayload;
	Serial.println();
	Serial.println(topic);
	Serial.println(spayload);
}

void WiFiMQTT::setup_wifi()
{
	long start = millis();
	delay(10);
	Serial.println();
	Serial.print("Connecting to ");
	Serial.println(SSID);

	WiFi.begin(SSID.c_str(), PSK.c_str());

	while (WiFi.status() != WL_CONNECTED)
	{
		yield();
		delay(50);
		Serial.print(".");
		if (millis() >= start + 5000)
		{
			//	system_restart(); //for ESP32
			ESP.restart(); //ESP.restart(); //for esp8266
		}
	}

	Serial.println("");
	Serial.println("WiFi connected");
	Serial.println("IP address: ");
	Serial.println(WiFi.localIP());
}

void WiFiMQTT::setup(MQTT_CALLBACK_SIGNATURE)
{
	setup_wifi();
	client.setServer(MQTT_BROKER.c_str(), 1883);

	client.setCallback(callback);
}

void WiFiMQTT::reconnect()
{
	long start = millis();
	while (!client.connected())
	{
		Serial.print("Reconnecting...");
		delay(100);
		if (!client.connect(ClientName.c_str()))
		{
			Serial.print("failed, rc=");
			Serial.print(client.state());
			Serial.println(" retrying in 5 seconds");
		}
		if (millis() >= start + 10000)
		{
			//system_restart(); //for ESP32
			ESP.restart(); //for esp8266
		}
	}

	std::map<String, String>::iterator it = topics1.begin();

	while (it != topics1.end())
	{

		String word = it->first;

		int n = word.length();
		char char_array[n + 1];

		strcpy(char_array, word.c_str());

		client.subscribe(char_array);

		String count = it->second;

		it++;
	}
}

void WiFiMQTT::publish(const char *topic, const char *payload)
{
	client.publish(topic, payload);
}

void WiFiMQTT::addTopic(String topic)
{
	topics1[topic] = "";
}

void WiFiMQTT::loop()
{
	if (!client.connected())
	{
		reconnect();
	}
	client.loop();
}

String WiFiMQTT::getValueFromTopic(String topic)
{
	std::map<String, String>::iterator it;
	it = topics1.find(topic);
	return it->second;
}


