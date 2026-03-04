#pragma once

#include <Servo.h>

class Extender
{
private:
    Servo servo;
    int pin = 12;

    int positionOut = 0;
    int positionIn = 180;

public:
    void setup();

    void out();
    void in();
};