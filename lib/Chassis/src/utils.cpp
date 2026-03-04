#include "utils.h"

void TeleplotPrint(const char* var, float value)
{
    Serial.print('>');
    Serial.print(var);
    Serial.print(':');
    Serial.print(value);
    Serial.print('\n');
}

float wrapToPi(float theta)
{
    while (theta > PI)
        theta -= 2 * PI;
    while (theta < -PI)
        theta += 2 * PI;
    return theta;
}
