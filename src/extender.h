#pragma once

#include <Servo.h>

class Extender
{
private:
    Servo servo;
    int pin = 11;

    int positionOut = 180;
    int positionIn = 0;

public:
    void setup();

    void out();
    void in();
};