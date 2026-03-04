/**
 * robot-nav.cpp is where you should put navigation routines.
 */

#include "robot.h"
#include "utils.h"



/**
 * Sets a destination in the lab frame.
 */

float drivekP = 30;
float turnkP = 250;

float clampReal(float value, float min, float max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

float invClamp(float value, float min, float max) {
    if (value > min && value < max) {
        return (value - min > max - value) ? max : min;
    }
    return value;
}

void Robot::DriveToPoint(const Pose& dest)
{
    destPose = dest;

    while(!CheckReachedDestination())
    {
        Twist velocity;
        chassis.ChassisLoop(velocity);        
        UpdatePose(velocity);

        float errHead = fmod(atan2(destPose.y - currPose.y, destPose.x - currPose.x) - currPose.theta, 2 * PI);

        #ifdef __NAV_DEBUG__
        TeleplotPrint("unboundErrHead", errHead);
        #endif

        errHead -= (errHead > PI) ? 2 * PI : 0;
        errHead += (errHead < -PI) ? 2 * PI : 0;
        // errHead = 0;
        
        float errDist = sqrt(pow(destPose.x - currPose.x, 2) + pow(destPose.y - currPose.y, 2));

        float effortLeft = clampReal(invClamp(clampReal(errDist * drivekP, -80, 80), -5, 5) - errHead * turnkP, -90, 90);
        float effortRight = clampReal(invClamp(clampReal(errDist * drivekP, -80, 80), -5, 5) + errHead * turnkP, -90, 90);


        #ifdef __NAV_DEBUG__
        TeleplotPrint("errHead", errHead);
        TeleplotPrint("errDist", errDist);
        TeleplotPrint("effortLeft", effortLeft);
        #endif

        chassis.SetMotorEfforts(effortLeft, effortRight);

        delay(20);
    }

    chassis.Stop();
}

bool Robot::CheckReachedDestination(void)
{
    return sqrt(pow(destPose.x - currPose.x, 2) + pow(destPose.y - currPose.y, 2)) < 2.0; // error tolerance of 2 cm
    
}

void Robot::UpdatePose(const Twist& twist)
{
    float time = 0.020; // 20 ms update time

    float newTheta = currPose.theta + twist.omega * time;
    float avgTheta = (currPose.theta + newTheta) / 2;

    float newx = currPose.x + time * twist.u * cos(avgTheta);
    float newy = currPose.y + time * twist.u * sin(avgTheta);

    currPose.x = newx;
    currPose.y = newy;
    currPose.theta = newTheta;

#ifdef __NAV_DEBUG__
    Serial.print(">currPose: ");
    Serial.print(currPose.x);
    Serial.print(":");
    Serial.print(currPose.y);
    Serial.print("|xy\n");
    TeleplotPrint("currPose.x", currPose.x);
    TeleplotPrint("currPose.y", currPose.y);
    TeleplotPrint("currPose.theta", currPose.theta);
#endif

}

void Robot::HandleDestination(void)
{
    EnterIdleState();
}