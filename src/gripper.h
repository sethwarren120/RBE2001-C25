#pragma once

#pragma once

#include <Servo.h>

class Gripper 
{
private:
    Servo servo;
    int pin = 4;

    int positionGrip = 0;
    int positionRelease = 130;

public:
    void setup();

    void grip();
    void release();
};