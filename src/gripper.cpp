#include "gripper.h"
#include <Arduino.h>

void Gripper::setup() {
    pinMode(pin, OUTPUT);

    servo.attach(pin);
}

void Gripper::grip() {
    servo.write(positionGrip);
    Serial.println("Gripping");
    delay(1000);
}

void Gripper::release() {
    servo.write(positionRelease);
    Serial.println("Releasing");
    delay(1000);
}