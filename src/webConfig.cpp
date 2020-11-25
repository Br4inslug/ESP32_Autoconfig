/*
 * webConfig.cpp
 *
 *  Created on: 01.10.2017
 *      Author: Johannes
 */

#include "webConfig.h"
#include <EEPROM.h>

#define EEPROM_SIZE 512

webConfig::webConfig()
{
}

webConfig::webConfig(int pinnumber) : server(80)
{
	pinNumber = pinnumber;
}

webConfig::~webConfig()
{
	// TODO Auto-generated destructor stub
}

void webConfig::addinput(String input1)
{
	input[input1] = "";
}

void webConfig::writeToEeprom()
{
	int currentaddress = 0;

	//EEPROM.put(currentaddress, set);
	//currentaddress = currentaddress + sizeof(set);

	String value;
	String name;
	unsigned int lengthvalue, lengthname;

	std::map<String, String>::iterator it = input.begin();
	while (it != input.end())
	{
		name = it->first;
		lengthname = name.length();
		value = it->second;

		lengthvalue = value.length();

		//EEPROM.put(currentaddress, lengthname);
		//		Serial.print("Adresse1: ");
		//	Serial.println(currentaddress);
		//	Serial.print("Länge Name: ");
		//	Serial.println(lengthname);
		//currentaddress = currentaddress + sizeof(lengthname);
		//EEPROM.put(currentaddress, name);
		writeString(currentaddress, name);
		//Serial.print("Adresse2: ");
		//Serial.println(currentaddress);
		//	Serial.print("Name: ");
		//Serial.println(name);
		currentaddress = currentaddress + lengthname + 1;
		//EEPROM.put(currentaddress, lengthvalue);
		//Serial.println(currentaddress);
		//Serial.println(lengthvalue);
		//currentaddress = currentaddress + sizeof(lengthvalue);
		//EEPROM.put(currentaddress, value);
		writeString(currentaddress, value);
		//Serial.println(currentaddress);
		//Serial.println(value);
		currentaddress = currentaddress + lengthvalue + 1;

		it++;
	}

	Serial.println("Write....");

	EEPROM.commit();

	//////////////////////////////////////
	char c;
	for (int i = 0; i < 150; i++)
	{
		EEPROM.get(i, c);
		Serial.write(c);
	}
	//////////////////////////////////////
}

void webConfig::checkEeprom()
{
	//TODO hier weitermachen
	//	bool check;
	int currentaddress = 0;
	//	EEPROM.get(currentaddress, check);
	//	currentaddress = currentaddress + sizeof(check);

	String value;
	String name;

	int size = input.size();

	for (int i = 0; i < size; i++)
	{

		name = readString(currentaddress);
		currentaddress = currentaddress + name.length() + 1;
		//Serial.println(name);
		value = readString(currentaddress);
		currentaddress = currentaddress + value.length() + 1;
		//Serial.println(value);

		//input.insert({ name, value });
		input[name] = value;
	}
}

void webConfig::init()
{

	pinMode(pinNumber, INPUT);
	EEPROM.begin(EEPROM_SIZE);
	int Push_button_state = digitalRead(pinNumber);

	if (Push_button_state == HIGH)
	{
		Serial.println("Reset!");
		Serial.print("Setting AP (Access Point)…");
		const char *id = "ESP-WebConfig";
		const char *password = "123456789";
		// Remove the password parameter, if you want the AP (Access Point) to be open
		WiFi.softAP(id, password);

		IPAddress IP = WiFi.softAPIP();
		Serial.print("AP IP address: ");
		Serial.println(IP);
		server.begin();
		config();
		writeToEeprom();
	}
	else
	{
		Serial.println("Normal Operation!");

		checkEeprom();
	}
}

void webConfig::extractInputs(String header)
{

	if (header.indexOf("get") >= 0)
	{
		std::map<String, String>::iterator it = input.begin();
		while (it != input.end())
		{
			int start = header.indexOf(it->first);
			int start2 = header.indexOf("=", start);
			start2++;

			int end = header.indexOf("&", start);
			//Serial.print("END:----->");
			//Serial.print(end);
			if (end == -1)
			{
				end = header.indexOf(" ", start);
			}
			String input1 = header.substring(start2, end);
			input[it->first] = input1;
			//Serial.println(input1);

			it++;
		}
		//Serial.println();
		//Serial.println();
		set = true;
	}
}

void webConfig::printInputs()
{

	std::map<String, String>::iterator it = input.begin();
	while (it != input.end())
	{

		Serial.print(it->first);
		Serial.print(" - ");
		Serial.println(it->second);

		it++;
	}
	Serial.println();
}

void webConfig::printInput()
{

	Serial.println(input.find("Password")->second);
}

String webConfig::getInputValue(String name)
{
	String value;
	value = input.find(name)->second;

	return value;
}

void webConfig::config()
{

	set = false;
	while (!set)
	{

		WiFiClient client = server.available(); // Listen for incoming clients

		if (client)
		{								   // If a new client connects,
			Serial.println("New Client."); // print a message out in the serial port
			String currentLine = "";	   // make a String to hold incoming data from the client
			while (client.connected())
			{ // loop while the client's connected
				if (client.available())
				{							// if there's bytes to read from the client,
					char c = client.read(); // read a byte, then
					Serial.write(c);		// print it out the serial monitor
					header += c;
					if (c == '\n')
					{ // if the byte is a newline character
						// if the current line is blank, you got two newline characters in a row.
						// that's the end of the client HTTP request, so send a response:
						if (currentLine.length() == 0)
						{
							// HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
							// and a content-type so the client knows what's coming, then a blank line:
							client.println("HTTP/1.1 200 OK");
							client.println("Content-type:text/html");
							client.println("Connection: close");
							client.println();

							extractInputs(header);

							//	Serial.print(header);

							String index_html1 = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<style> 
input[type=button], input[type=submit], input[type=reset] {
  background-color: #4CAF50;
  border: none;
  color: white;
  padding: 10px 32px;
  text-decoration: none;
  margin: 4px 2px;
  cursor: pointer;
  font-family: Verdana;
  font-size: 40px;
  border-radius: 12px;
}
input[type=text]{
  background-color:#ffad33;
  border: none;
  color: white;
  padding: 5px 5px;
  text-decoration: none;
  margin: 4px 2px;
  cursor: pointer;
  font-size: 40px;
}
body {
  background-color: darkgrey;
  color: black;
  font-family: Verdana;
   text-align: center;
  font-size: 40px;
}
p {
  font-family: Verdana;
}
</style>
</head>
<body>

<p>WebConfig</p>

 <form action="/get">
   

)rawliteral";

							String index_html2;
							std::map<String, String>::iterator it = input.begin();
							while (it != input.end())
							{
								// Accessing KEY from element pointed by it.
								String name = it->first;
								index_html2 += name;
								index_html2 += R"rawliteral(: <br><input type="text" name=")rawliteral";

								index_html2 += name;
								index_html2 += R"rawliteral("><br>)rawliteral";

								// Accessing VALUE from element pointed by it.

								// Increment the Iterator to point to next entry
								it++;
							}

							String index_html3 = R"rawliteral(  <br>  <input type="submit" value="Submit">
  </form>

</body>
</html>)rawliteral";

							client.println(index_html1);
							client.println(index_html2);
							client.println(index_html3);
							client.println();
							// Break out of the while loop
							break;
						}
						else
						{ // if you got a newline, then clear currentLine
							currentLine = "";
						}
					}
					else if (c != '\r')
					{					  // if you got anything else but a carriage return character,
						currentLine += c; // add it to the end of the currentLine
					}
				}
			}
			// Clear the header variable
			header = "";
			// Close the connection
			client.stop();
			Serial.println("Client disconnected.");
			Serial.println("");
		}
	}
}

void webConfig::writeString(int address, String data)
{
	int stringSize = data.length();
	for (int i = 0; i < stringSize; i++)
	{
		EEPROM.write(address + i, data[i]);
	}
	EEPROM.write(address + stringSize, '\0'); //Add termination null character
}

String webConfig::readString(int address)
{
	char data[100]; //Max 100 Bytes
	int len = 0;
	unsigned char k;
	k = EEPROM.read(address);
	while (k != '\0' && len < 100) //Read until null character
	{
		k = EEPROM.read(address + len);
		data[len] = k;
		len++;
	}
	data[len] = '\0';
	return String(data);
}