#include <Arduino.h>
#include <PubSubClient.h>
#include "Classes/Actuators.h"
#include "Classes/Sensors.h"
#include "Classes/WifiHandler.h"

// ================================================================
// WiFi setup:
const char *SSID = "Beto iPhone";
const char *PASSWORD = "beto12345";
WifiHandler WIFI(SSID, PASSWORD);
WiFiClient wifiClient;
// ================================================================
// Broker setup:
PubSubClient MQTT(wifiClient);
const char *MQTT_ID = "agrisysesp32id1";
const char *BROKER_MQTT = "test.mosquitto.org";
const uint16_t BROKER_PORT = 1883;
// ================================================================
// Subscribe topics:
const char *ST_ESP_LED = "AgriSys_Actuator_EspLed";
const char *ST_WATER_PUMP = "AgriSys_Actuator_WaterPump";
// ================================================================
// Publish topics:
const char *PT_AIR_TEMPERATURE = "AgriSys_Sensor_AirTemperature";
const char *PT_AIR_HUMIDITY = "AgriSys_Sensor_AirHumidity";
const char *PT_WATER_LEVEL = "AgriSys_Sensor_WaterLevel";
const char *PT_SOLO_HUMIDITY = "AgriSys_Sensor_SoloHumidity";
// ================================================================
// Objects:
Sensors sensors;
Actuators actuators;
// ================================================================
// Functions Declaration:
void Monitor();
void BrokerKeepAlive();
void MqttCallback(char *topic, byte *payload, unsigned int length);
// ================================================================

void setup() 
{
  Serial.begin(115200);

  actuators.OnOff_Espled("on");
  delay(2000);

  MQTT.setServer(BROKER_MQTT, BROKER_PORT);
  MQTT.setCallback(MqttCallback);
  // Actuators reset
  actuators.OnOff_Espled("off");
  actuators.OnOff_WaterPump("off");
}

void loop() 
{
  BrokerKeepAlive();

  Monitor();

  // Publish sensor datas to MQTT:
  MQTT.publish(PT_AIR_TEMPERATURE, sensors.ReadTemperature().c_str());
  MQTT.publish(PT_AIR_HUMIDITY, sensors.ReadAirHumidity().c_str());
  MQTT.publish(PT_WATER_LEVEL, sensors.ReadWaterLevel().c_str());  
  MQTT.publish(PT_SOLO_HUMIDITY, sensors.ReadSoloHumidity().c_str());

  delay(2000);

}

// ================================================================
// Functions:
void Monitor()
{
  Serial.println(sensors.ReadSoloHumidityInt());
  //Solo umido
  if (/*sensors.ReadSoloHumidityInt() > 0 &&*/ sensors.ReadSoloHumidityInt() < 2000)
  {
    Serial.println(" Status: Solo umido");
    //actuators.OnOff_WaterPump("off");
  }
 
  //Solo com umidade moderada
  if (sensors.ReadSoloHumidityInt() > 2000 && sensors.ReadSoloHumidityInt() < 3000)
  {
    Serial.println(" Status: Umidade moderada");
    //actuators.OnOff_WaterPump("off");
  }
 
  //Solo seco
  if (sensors.ReadSoloHumidityInt() > 3000 /*&& sensors.ReadSoloHumidityInt() < 1024*/)
  {
    Serial.println(" Status: Solo seco");
    //actuators.OnOff_WaterPump("on");
  }
}

void BrokerKeepAlive()
{
  while (!WIFI.IsConnected())
  {
    WIFI.Connect();
  }

  while (!MQTT.connected())
  {
    Serial.print("Connecting to MQTT Broker: ");
    Serial.println(BROKER_MQTT);

    if (MQTT.connect(MQTT_ID))
    {
      Serial.println("Connected!");
      // Add topics here.
      MQTT.subscribe(ST_ESP_LED);
      MQTT.subscribe(ST_WATER_PUMP);
    }
    else
    {
      Serial.println("Fail in connection. Trying again in 2s...");
      delay(2000);
    }
  }

  MQTT.loop();
}

void MqttCallback(char *topic, byte *payload, unsigned int length)
{
  std::string requestedTopic = std::string(topic);
  String msg;
  for (int i = 0; i < length; i++)
  {
    char c = (char)payload[i];
    msg += c;
  }

  Serial.println(topic);
  Serial.println(msg);

  if (requestedTopic == ST_ESP_LED)
  {
    actuators.OnOff_Espled(msg);
  }
  else if (requestedTopic == ST_WATER_PUMP)
  {
    actuators.OnOff_WaterPump(msg);
  }
}
// ================================================================