#include "robot.h"
#include <Romi32U4Buttons.h>

Romi32U4ButtonA buttonA;
Romi32U4ButtonB buttonB;
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

Pose pose;

void Robot::RobotLoop(void) 
{
        /**
         * Run the chassis loop, which handles low-level control.
         */
        Twist velocity;
        if(chassis.ChassisLoop(velocity)) {
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
                    SetDestination(pose);
                }
            }
            else if(buttonA.isPressed()) {
                taskTimer = 30;
                robotState = ROBOT_TASK_WAIT;
                pose = Pose(60, 0, 0);
            }
            else if(buttonB.isPressed()) {
                taskTimer = 30;
                robotState = ROBOT_TASK_WAIT;
                pose = Pose(0, 0, -2 * PI);
            }
            else if (buttonC.isPressed()) {
                taskTimer = 30;
                robotState = ROBOT_TASK_WAIT;
                pose = Pose(0, 0, 2 * PI);
            }
            if(robotState == ROBOT_DRIVE_TO_POINT) {

                Spin();
                if(CheckSpin()) {
                    HandleDestination();
                }
                // SetDestination(pose);
                // DriveToPoint();
                // if(CheckReachedDestination()) HandleDestination();
            }
        }
    }

