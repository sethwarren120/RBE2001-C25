#include <Servo.h>

class Gripper 
{
private:
    Servo servo;
    int pin = 4;

    int positionGrip = 90;
    int positionRelease = 0;

public:
    void setup();

    void grip();
    void release();
};