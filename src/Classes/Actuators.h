#define ESPLED_PIN 2

#define WATERBOMB_PIN 12
//#define RELE2_PIN 13

class Actuators
{
private:
    /* data */
public:
    Actuators()
    {
        pinMode(ESPLED_PIN, OUTPUT);
        pinMode(WATERBOMB_PIN, OUTPUT);
    }
    ~Actuators() {}

    void OnOff_Espled(String act)
    {
        if(act == "on")
            digitalWrite(ESPLED_PIN, HIGH);
        else if(act == "off")
            digitalWrite(ESPLED_PIN, LOW);
    }

    void OnOff_WaterPump(String act)
    {
        if(act == "on")
            digitalWrite(WATERBOMB_PIN, HIGH);
        else if(act == "off")
            digitalWrite(WATERBOMB_PIN, LOW);
    }
};