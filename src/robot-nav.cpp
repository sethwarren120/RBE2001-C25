/**
 * robot-nav.cpp is where you should put navigation routines.
 */

#include "robot.h"

void Robot::UpdatePose(const Twist& twist)
{
    /**
     * TODO: Add your FK algorithm to update currPose here.
     */
    float time = 0.020; // 20 ms update time

    float newTheta = currPose.theta + twist.omega * time;
    float avgTheta = (currPose.theta + newTheta) / 2;

    float newx = currPose.x + time * twist.u * cos(avgTheta);
    float newy = currPose.y + time * twist.u * sin(avgTheta);

    currPose.x = newx;
    currPose.y = newy;
    currPose.theta = newTheta;

#ifdef __NAV_DEBUG__
    // TeleplotPrint("x", currPose.x);
    // TeleplotPrint("y", currPose.y);
    TeleplotPrint("theta", currPose.theta);
    TeleplotPrintXY("pose", currPose.x, currPose.y);
#endif

}

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

/**
 * Sets a destination in the lab frame.
 */
void Robot::SetDestination(const Pose& dest)
{
    digitalWrite(LED_BUILTIN, HIGH);

    Serial.print("Setting dest to: ");
    Serial.print(dest.x);
    Serial.print(", ");
    Serial.print(dest.y);
    Serial.print('\n');

    destPose = dest;
    robotState = ROBOT_DRIVE_TO_POINT;
}

float drivekP = 10;
float turnkP = 3;

void Robot::DriveToPoint(void)
{
    if(robotState == ROBOT_DRIVE_TO_POINT)
    {
        float errHead = fmod(atan2(destPose.y - currPose.y, destPose.x - currPose.x) - currPose.theta, 2 * PI);
        errHead -= (errHead > PI) ? 2 * PI : 0;
        // errHead = 0;
        float errDist = sqrt(pow(destPose.x - currPose.x, 2) + pow(destPose.y - currPose.y, 2)) * cos(errHead);

        TeleplotPrint("errDist", errDist);

        float effortLeft = clampReal(invClamp(clampReal(errDist * drivekP, -30, 30), -10, 10) - errHead * turnkP, -30, 30);
        float effortRight = clampReal(invClamp(clampReal(errDist * drivekP, -30, 30), -10, 10) + errHead * turnkP, -30, 30);

        /**
         * TODO: Add your IK algorithm here. 
         */

         TeleplotPrint("effortLeft", effortLeft);

#ifdef __NAV_DEBUG__
        // Print useful stuff here.
#endif

        chassis.SetMotorEfforts(effortLeft, effortRight);
    }
}

bool Robot::CheckReachedDestination(void)
{
    return sqrt(pow(destPose.x - currPose.x, 2) + pow(destPose.y - currPose.y, 2)) < 2.0;
}

void Robot::HandleDestination(void)
{
    chassis.Stop();
    digitalWrite(LED_BUILTIN, LOW);
    /**
     * TODO: Stop and change state. Turn off LED.
     */
    digitalWrite(13, LOW);
}