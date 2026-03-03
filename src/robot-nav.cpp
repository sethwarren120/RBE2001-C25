/**
 * robot-nav.cpp is where you should put navigation routines.
 */

#include "robot.h"



/**
 * Sets a destination in the lab frame.
 */

float drivekP = 15;
float turnkP = 25;

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
    while(!CheckReachedDestination())
    {
        float errHead = fmod(atan2(destPose.y - currPose.y, destPose.x - currPose.x) - currPose.theta, 2 * PI);

        #ifdef __NAV_DEBUG__
        TeleplotPrint("unboundErrHead", errHead);
        #endif

        errHead -= (errHead > PI) ? 2 * PI : 0;
        errHead += (errHead < -PI) ? 2 * PI : 0;
        // errHead = 0;
        float errDist = sqrt(pow(destPose.x - currPose.x, 2) + pow(destPose.y - currPose.y, 2));

        float effortLeft = clampReal(invClamp(clampReal(errDist * drivekP, -50, 50), -5, 5) - errHead * turnkP, -70, 70);
        float effortRight = clampReal(invClamp(clampReal(errDist * drivekP, -50, 50), -5, 5) + errHead * turnkP, -70, 70);


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
    Twist velocity;
    chassis.ChassisLoop(velocity);
    UpdatePose(velocity);
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
    TeleplotPrint("x", currPose.x);
    TeleplotPrint("y", currPose.y);
    TeleplotPrint("theta", currPose.theta);
    TeleplotPrintXY("pose", currPose.x, currPose.y);
#endif

}

void Robot::HandleDestination(void)
{
    EnterIdleState();
}