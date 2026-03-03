#include <Servo.h>

class Extender
{
private:
    Servo servo;
    int pin;

    int positionOut = 50;
    int positionIn = 10;

public:
    void setup();

    void out();
    void in();
};