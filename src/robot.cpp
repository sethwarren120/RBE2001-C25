#include "robot.h"
#include <Romi32U4Buttons.h>

Romi32U4ButtonC buttonC;

int taskTimer = 0;

void Robot::InitializeRobot(void)
{
    chassis.InititalizeChassis();

    /**
     * TODO: Set pin 13 HIGH when navigating and LOW when destination is reached.
     * Need to set as OUTPUT here.
     */
    pinMode(13, OUTPUT);
}

void Robot::EnterIdleState(void)
{
    chassis.Stop();

    Serial.println("-> IDLE");
    robotState = ROBOT_IDLE;
}


float poses[][2] = {{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}};

/**
 * The main loop for your robot. Process both synchronous events (motor control),
 * and asynchronous events (distance readings, etc.).
*/
void Robot::RobotLoop(void) 
{
        /**
         * Run the chassis loop, which handles low-level control.
         */
        Twist velocity;
        if(chassis.ChassisLoop(velocity))
        {
            // We do FK regardless of state
            UpdatePose(velocity);
                
            /**
             * Here, we break with tradition and only call these functions if we're in the 
             * DRIVE_TO_POINT state. CheckReachedDestination() is expensive, so we don't want
             * to do all the maths when we don't need to.
             * 
             * While we're at it, we'll toss DriveToPoint() in, as well.
             */ 
            if (robotState == ROBOT_TASK_WAIT) {
            taskTimer--;
            if (taskTimer <= 0) {
                SetDestination(Pose(0,0,2 * PI));
            }
        }
        else if (buttonC.isPressed()) {
            taskTimer = 30;
            robotState = ROBOT_TASK_WAIT;
        }
        if(robotState == ROBOT_DRIVE_TO_POINT) {
            // for(int i = 0; i < sizeof(poses); i++) {
            //     Pose pose;

            //     if(i == sizeof(poses) - 1) {
            //         pose = Pose(poses[i][0], poses[i][1], 0);
            //     }

            //     else {
            //         pose = Pose(poses[i][0], poses[i][1], atan2(poses[i+1][1] - poses[i][1], poses[i+1][0] - poses[i][0]));
            //     }
                Spin();
                if(CheckSpin()) {
                    HandleDestination();
                }
                //SetDestination(pose);
                //DriveToPoint();
                //if(CheckReachedDestination()) HandleDestination();
            }
        }
    }

