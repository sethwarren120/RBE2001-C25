#include "BlueMotor.h"

class Elevator 
{
private:
    BlueMotor motor;

    int groundHeight = 0;
    int clearHeight = 30;
    int lowerHeight = 60;
    int upperHeight = 90;
    int topHeight = 120;

    int positions[5] = { groundHeight, clearHeight, lowerHeight, upperHeight, topHeight };

public:
    void setup();

    void setHeight(int pos);
};