#include "extender.h"
#include <Arduino.h>

void Extender::setup() {
    pinMode(pin, OUTPUT);

    servo.attach(pin);
}

void Extender::out() {
    servo.write(positionOut);
    delay(2000);
}

void Extender::in() {
    servo.write(positionIn);
    delay(2000);
}