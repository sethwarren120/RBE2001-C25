#include "gripper.h"
#include <Arduino.h>

void Gripper::setup() {
    pinMode(pin, OUTPUT);

    servo.attach(pin);
}

void Gripper::grip() {
    servo.write(positionGrip);
    delay(2000);
}

void Gripper::release() {
    servo.write(positionRelease);
    delay(2000);
}