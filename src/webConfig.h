/*
 * webConfig.h
 *
 *  Created on: 01.10.2017
 *      Author: Johannes
 */
#include <Arduino.h>
//#include <ESP8266WiFi.h>
#include <map>

#include <WiFi.h>

#ifndef webConfig_H_
#define webConfig_H_


class webConfig
{
public:
	webConfig();
	webConfig(int pinnumber);
	virtual ~webConfig();
	void init();
	void config();
	void addinput(String input);
	void extractInputs(String header);
	void printInputs();
	void printInput();
	void writeToEeprom();
	void checkEeprom();
	String getInputValue(String name);
	void writeString(int address, String data);
	String readString(int address);


private:
	
	WiFiServer server;
	String header;
	std::map<String, String> input;
	bool set = false;
	 int pinNumber ;
	 int resetcounter;
};

#endif /* webConfig */
