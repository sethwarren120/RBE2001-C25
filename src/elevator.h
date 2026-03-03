#include "BlueMotor.h"

class Elevator 
{
private:
    BlueMotor motor;

    float groundHeight = 0.0f;
    float clearHeight = 9.0f;
    float lowerHeight = 17.3f;
    float upperHeight = 21.6;
    float topHeight = 25.9;

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