#include <Servo.h>

class Gripper 
{
private:
    Servo servo;
    int pin;

    int positionGrip;
    int positionRelease;

public:
    void setup();

    void grip();
    void release();
};