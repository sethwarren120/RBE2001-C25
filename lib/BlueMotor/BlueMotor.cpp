#include <Arduino.h>
#include <BlueMotor.h>

volatile long count = 0;
long countLast = 0;
unsigned time = 0;
  
float ks = 0;
float kg = 0;
float kp = 5;
float kd = 0; 
long maxError = 1;

bool prevEncA = false;

const int ENCA = 6;
const int ENCB = 5;

BlueMotor::BlueMotor()
{
}

void BlueMotor::setup()
{
    pinMode(PWMOutPin, OUTPUT);
    pinMode(AIN2, OUTPUT);
    pinMode(AIN1, OUTPUT);
    pinMode(ENCA, INPUT);
    pinMode(ENCB, INPUT);
    TCCR1A = 0xA8; //0b10101000; //gcl: added OCR1C for adding a third PWM on pin 11
    TCCR1B = 0x11; //0b00010001;
    ICR1 = 400;
    OCR1C = 0;

    attachInterrupt(digitalPinToInterrupt(ENCA), isr, CHANGE);
    reset();
}

long BlueMotor::getPosition()
{
    long tempCount = 0;
    noInterrupts();
    tempCount = count;
    interrupts();
    return tempCount;
}

void BlueMotor::reset()
{
    noInterrupts();
    count = 0;
    interrupts();
}


void BlueMotor::isr()
{
    bool encA = digitalRead(ENCA);
    bool encB = digitalRead(ENCB);

    if (encA != prevEncA) 
        (encB == encA) ? count-- : count++;
    else 
        (encA == encB) ? count++ : count--;

    prevEncA = encA;
}

float countsToCm(long counts) {
    // 270 counts per revolution
    float rotations = counts / 270.0f;
    // Gear Reduction
    rotations *= 18.0f / 34.0f;
    // Rotations * gear circumference (2 * PI * radius)
    return rotations * 2.0 * PI * 1.0;
}

void BlueMotor::setEffort(int effort)
{
    if (effort < 0)
    {
        setEffort(-effort, true);
    }
    else
    {
        setEffort(effort, false);
    }
}

void BlueMotor::setEffort(int effort, bool clockwise)
{
    if (clockwise)
    {
        digitalWrite(AIN1, HIGH);
        digitalWrite(AIN2, LOW);
    }
    else
    {
        digitalWrite(AIN1, LOW);
        digitalWrite(AIN2, HIGH);
    }
    // True range is 0 to 400, set to 200 for safety
    // CAN CHANGE THIS IF NEEDED
    OCR1C = constrain(effort, 0, 200);
}

void BlueMotor::moveTo(float target)  
{                     
    while(abs(target - countsToCm(getPosition())) > maxError) {
        float position = countsToCm(getPosition());
        float error = target - position;
        float velocity = position - countLast * 50.0f; // Cm/s

        float direction = error / abs(error);

        // Combination of PID and feedforward control
        // Feedforward kg and ks are meant to remove the effects of gravity and
        // friction from the pd controller

        setEffort(kg + ks * direction + kp * error + kd * velocity);

        delay(20);
    }

    setEffort(0);
}