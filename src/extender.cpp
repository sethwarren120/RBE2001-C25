#include "extender.h"
#include <Arduino.h>

void Extender::setup() {
    pinMode(pin, OUTPUT);

    servo.attach(pin);
}

void Extender::out() {
    servo.write(positionOut);
    Serial.println("Moving out");
    delay(1000);
}

void Extender::in() {
    servo.write(positionIn);
    Serial.println("Moving in");
    delay(1000);
}