#pragma once    

#include "BlueMotor.h"

class Elevator 
{
private:
    BlueMotor motor;

    /* ------------------------------- Real values ------------------------------ */
    float groundHeight = 0.0f;
    float clearHeight = 10.5f;
    float lowerHeight = 18.0f;
    float upperHeight = 22.0f;
    float topHeight = 27.0f;

    /* ------------------------------- Test values ------------------------------ */
    // float groundHeight = 0.0f;
    // float clearHeight = 1.0f;
    // float lowerHeight = 2.0f;
    // float upperHeight = 3.0f;
    // float topHeight = 4.0f;

    float positions[5] = { groundHeight, clearHeight, lowerHeight, upperHeight, topHeight };

    // Base height 2.5

public:
    void setup();

    void setHeight(int pos);

    enum ElevatorPositions {
        GROUND,
        CHASSISCLEARANCE,
        LOWERSHELF,
        UPPERSHELF,
        TOP
    };
};