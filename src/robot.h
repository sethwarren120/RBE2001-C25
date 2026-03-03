#pragma once

#include "chassis.h"

#include "elevator.h"
#include "extender.h"
#include "gripper.h"

class Robot
{
protected:
    /**
     * robotState is used to track the current task of the robot. You will add new states as 
     * the term progresses.
     */
    enum ROBOT_STATE 
    {
        ROBOT_IDLE,
        ROBOT_LIFT,
        ROBOT_GRAB,
        ROBOT_RELEASE,
        ROBOT_LOWER,
        ROBOT_TURN,
        ROBOT_DRIVE_TO_POINT,
        ROBOT_TASK_WAIT,
        ROBOT_TASK
    };
    ROBOT_STATE robotState = ROBOT_IDLE;

    /* Define the chassis*/
    Chassis chassis;

    Elevator elevator;
    Extender extender;
    Gripper gripper;

    // For managing key presses
    String keyString;

    /**
     * For tracking current pose and the destination.
     */
    Pose currPose;
    Pose destPose;
    
public:
    Robot(void) {keyString.reserve(10);}
    void InitializeRobot(void);
    void RobotLoop(void);

    void TeleplotPrintXY(const char* topic, float x, float y)
    {
        Serial.print("\n>");
        Serial.print(topic);
        Serial.print(':');
        Serial.print(x, 5);
        Serial.print(':');
        Serial.print(y, 5);
        Serial.println("|xy");
    }

protected:
    /* State changes */    
    void EnterIdleState(void);

    // /* Navigation methods.*/
    void UpdatePose(const Twist& u);
    void DriveToPoint(const Pose& dest);
    bool CheckReachedDestination(void);
    void HandleDestination(void);
};
