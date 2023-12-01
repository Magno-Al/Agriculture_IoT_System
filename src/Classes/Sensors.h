#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

// DHT11 - air humidity and temperature sensor 
#define DHT_PIN 4
#define DHT_TYPE DHT11
// HCSR04 - distance ultrassonic sensor (water tank level)
#define HCSR04_TRIG_PIN 5
#define HCSR04_ECHO_PIN 18
// SOLO HUMIDITY - solo humidity sensor
#define SOLO_HUMIDITY_PIN 32
// LDR - ambient hight/low luminosity sensor 
#define LDR_PIN 15

DHT dht(DHT_PIN, DHT_TYPE);

class Sensors
{
private:
    /* data */
public:
    Sensors()
    {
        dht.begin();
    
        pinMode(HCSR04_TRIG_PIN, OUTPUT); 
        pinMode(HCSR04_ECHO_PIN, INPUT);
        pinMode(SOLO_HUMIDITY_PIN, INPUT);
        pinMode(LDR_PIN, INPUT);
    }

    ~Sensors() {}

    std::string ReadAirHumidity()
    {
        float h = dht.readHumidity();

        if (isnan(h)) 
        {
            Serial.println(F("Failed to read from DHT sensor!"));
            return "NA";
        }
        //Serial.println(h);
        return std::to_string(h);
    }

    std::string ReadTemperature()
    {
        float t = dht.readTemperature();

        if (isnan(t)) 
        {
            Serial.println(F("Failed to read from DHT sensor!"));
            return "NA";
        }
        //Serial.println(t);
        return std::to_string(t);
    }

    std::string ReadWaterLevel()
    {
        digitalWrite(HCSR04_TRIG_PIN, LOW);
        delayMicroseconds(2);

        digitalWrite(HCSR04_TRIG_PIN, HIGH);
        delayMicroseconds(10);
        digitalWrite(HCSR04_TRIG_PIN, LOW);

        long duration = pulseIn(HCSR04_ECHO_PIN, HIGH);

        if (isnan(duration)) 
        {
            Serial.println(F("Failed to read water level from HCSR04 sensor!"));
            return "NA";
        }
        float distanceCm = duration * 0.034 / 2;

        //Serial.println(distanceCm);
        return std::to_string(distanceCm);
    }

    std::string ReadSoloHumidity()
    {
        int h = analogRead(SOLO_HUMIDITY_PIN);

        if (isnan(h)) 
        {
            Serial.println(F("Failed to read from Solo Humidity sensor!"));
            return "NA";
        }
        //Serial.println(h);
        return std::to_string(h);
    }

    std::string ReadLuminosity()
    {
        int h = digitalRead(LDR_PIN);

        if (isnan(h)) 
        {
            Serial.println(F("Failed to read from Luminosity sensor!"));
            return "NA";
        }
        //Serial.println(h);
        return std::to_string(h);
    }
};