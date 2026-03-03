#pragma once

class BlueMotor
{
public:
    BlueMotor();
    void setEffort(int effort);
    void moveTo(long position);
    void loop();
    long getPosition();
    void reset();
    void setup();

private:
    void setEffort(int effort, bool clockwise);
    static void isr();
    const int tolerance = 3;
    const int PWMOutPin = 12;
    const int AIN2 = 30;
    const int AIN1 = 13;
};