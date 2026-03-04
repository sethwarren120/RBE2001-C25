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

    /* ------------------- Control tuning for driving to pose ------------------- */
    const float TURN_ONLY_THRESHOLD = 0.f;

    const float KP_DIST = 1; // TODO: tune to work better
    // const float KP_DIST = 2; // a previous value that kinda worked
    const float KI_DIST = 0.05;

    const float KP_THETA = 100;
    const float KI_THETA = 0.0; // TODO: tune to work better
    // const float KI_THETA = 0.2; // a previous value that kinda didn't work

    const int16_t MAX_EFFORT = 400;

    /* ----------------------------- PID integrators ---------------------------- */
    float errorDistIntegral = 0;
    float errorThetaIntegral = 0;
    
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
