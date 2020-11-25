#include <Arduino.h>

#include "webConfig.h"

#include "WiFiMQTT.h"

webConfig webConfig1(15);

WiFiMQTT wifimqtt;

void callback(char *topic, byte *payload, unsigned int length)
{
  wifimqtt.callback1(topic, payload, length);
  
}

void setup()
{
  Serial.begin(115200);

  webConfig1.addinput("Password");
  webConfig1.addinput("SSID");
  webConfig1.addinput("MQTT-Server");
  webConfig1.addinput("ESP-Name");

  webConfig1.init();

  //webConfig1.printInputs();
  //Debug
  wifimqtt.setConfig(webConfig1.getInputValue("SSID"), webConfig1.getInputValue("Password"), webConfig1.getInputValue("MQTT-Server"), webConfig1.getInputValue("ESP-Name"));
  wifimqtt.setup(callback);
}

void loop()
{
  //webConfig1.printInputs();
  wifimqtt.loop();
  // delay(1000);
  //webConfig1.checkEeprom();
  // String value=webConfig1.getInputValue("SSID");
  //Serial.println(value);
}
