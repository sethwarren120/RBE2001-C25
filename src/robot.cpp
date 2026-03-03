#include "robot.h"
#include"elevator.h"
#include"extender.h"
#include"gripper.h"
#include <Romi32U4Buttons.h>

Romi32U4ButtonC buttonC;


void Robot::InitializeRobot(void)
{
    chassis.InititalizeChassis();
    elevator.setup();
    extender.setup();
    gripper.setup();

    extender.out();
    gripper.release();
}

Pose intermediate = Pose(10, -10, 0);

Pose finish = Pose(20, 0, 0);

void Robot::RobotLoop(void) 
{
    if (buttonC.isPressed()) {
        delay(3000);

        extender.out();

        gripper.grip();

        elevator.setHeight(1);

        extender.in();

        elevator.setHeight(2);

        extender.out();

        gripper.release();

        extender.in();

        elevator.setHeight(3);

        extender.out();

        gripper.grip();

        extender.in();

        elevator.setHeight(4);

        extender.out();

        gripper.release();

        extender.in();

        elevator.setHeight(1);

        DriveToPoint(intermediate);

        DriveToPoint(finish);
    }
}

void Robot::EnterIdleState(void)
{
    chassis.Stop();

    Serial.println("-> IDLE");
    robotState = ROBOT_IDLE;
}