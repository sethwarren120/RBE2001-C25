#include "elevator.h"
#include <Arduino.h>

void Elevator::setup() {
    motor.setup();
}

void Elevator::setHeight(int pos) {
    motor.moveTo(positions[pos]);
}