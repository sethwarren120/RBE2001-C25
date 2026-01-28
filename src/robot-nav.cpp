/**
 * robot-nav.cpp is where you should put navigation routines.
 */

#include "robot.h"

void Robot::UpdatePose(const Twist& twist)
{
    /**
     * TODO: Add your FK algorithm to update currPose here.
     */

#ifdef __NAV_DEBUG__
    TeleplotPrint("x", currPose.x);
    TeleplotPrint("y", currPose.y);
    TeleplotPrint("theta", currPose.theta);
#endif

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

float drivekP = 1;
float turnkP = 1;

void Robot::DriveToPoint(void)
{
    if(robotState == ROBOT_DRIVE_TO_POINT)
    {
        float errHead = fmod(atan2(destPose.y - currPose.y, destPose.x - currPose.x) - currPose.theta, 360.0) - 180;
        float errDist = sqrt(pow(destPose.x - currPose.x, 2) + pow(destPose.y - currPose.y, 2)) * cos(errHead);

        float effortLeft = errDist * drivekP - errHead * turnkP;
        float effortRight = errDist * drivekP + errHead * turnkP;
        /**
         * TODO: Add your IK algorithm here. 
         */

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
}