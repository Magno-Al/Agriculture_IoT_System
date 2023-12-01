#include <WiFi.h>

class WifiHandler
{
private:
    const char* SSID;
    const char* Password;
public:
    WifiHandler(const char* ssid, const char* password) : SSID(ssid), Password(password) {}
    ~WifiHandler() {}

    boolean IsConnected()
    {
        if (WiFi.status() == WL_CONNECTED)
            return 1;
        else
            return 0;
    }

    void Connect(void)
    {
        if (IsConnected())
            return;

        WiFi.begin(SSID, Password);

        Serial.println("Trying to connect to WiFi:");
        Serial.println(SSID);

        while (WiFi.status() != WL_CONNECTED)
        {
            delay(300);
            Serial.print(".");
        }

        Serial.println("Connected!");
    }
};
