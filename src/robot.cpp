#include "robot.h"
#include <servo32u4.h>
#include <Romi32U4Buttons.h>

Romi32U4ButtonC buttonC;

int taskTimer = 0;

void Robot::InitializeRobot(void)
{
    chassis.InititalizeChassis();

}

void Robot::EnterIdleState(void)
{
    chassis.Stop();

    Serial.println("-> IDLE");
    robotState = ROBOT_IDLE;
}

void Robot::RobotLoop(void) 
{
        Twist velocity;
        if(chassis.ChassisLoop(velocity)) {
            UpdatePose(velocity);
                
            if (robotState == ROBOT_TASK_WAIT) {
                taskTimer--;
                if (taskTimer <= 0) {
                    SetDestination(pose);
                }
            }
            else if(buttonA.isPressed()) {
                taskTimer = 30;
                robotState = ROBOT_TASK_WAIT;
                pose = Pose(poses[poseIndex][0], poses[poseIndex][1], 0);
            }
            if(robotState == ROBOT_DRIVE_TO_POINT) {

                // spin to face the point to minimize turning while driving
                if(!spinned) {
                    float xErr = destPose.x - currPose.x;
                    float yErr = destPose.y - currPose.y;
                    float errHead = atan2(yErr, xErr) - currPose.theta;
                    errHead = fmod(errHead, 2*PI);
                    errHead -= (errHead > PI) ? 2 * PI : 0;

                    Pose tempPose = Pose(0, 0, errHead);
                    SetDestination(tempPose);
                    Spin();
                    if(CheckSpin()) {
                        HandleDestination();
                        spinned = true;
                    }
                }
                // drive to point with hopefully minimal turning
                else {
                    SetDestination(pose);
                    DriveToPoint();
                    if(CheckReachedDestination()) {
                        HandleDestination();
                        // if at destination, update to the next pose in the list

                        poseIndex++;
                        pose = Pose(poses[poseIndex][0], poses[poseIndex][1], 0);

                        if (poseIndex == sizeof(poses) / sizeof(poses[0])) {
                            EnterIdleState();
                        }
                    }
                }
            }
        }
    }

