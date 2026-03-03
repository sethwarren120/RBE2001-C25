#include <Arduino.h>
#include <BlueMotor.h>

volatile long count = 0;
long countLast = 0;
unsigned time = 0;

long setpoint = 0;
bool controlLoop = false;

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
    OCR1C = constrain(effort, 0, 400);
}

void BlueMotor::moveTo(long target)  
{                              
    setpoint = target;
    controlLoop = true;
}

void BlueMotor::loop() 
{
    if (!controlLoop) return;
     
    float ks = 0;
    float kp = 0;
    float kd = 0; 
    
    long position = getPosition();
    long error = setpoint - position;
    long velocity = position - countLast;

    setEffort(ks + kp * error + kd * velocity);
}