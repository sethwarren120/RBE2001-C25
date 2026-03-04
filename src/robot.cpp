#include "robot.h"
#include"elevator.h"
#include"extender.h"
#include"gripper.h"
#include <Romi32U4Buttons.h>

Romi32U4ButtonA buttonA;
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

Pose intermediate = Pose(15, -30, 0);

Pose finish = Pose(40, 0, 0);

void Robot::RobotLoop(void) 
{
    if (buttonC.isPressed()) {

        delay(300);
        
        Serial.println("a");

        extender.out();

        Serial.println("b");

        gripper.grip();

        Serial.println("c");

        elevator.setHeight(Elevator::CHASSISCLEARANCE);

        Serial.println("d");

        extender.in();

        elevator.setHeight(Elevator::LOWERSHELF);

        extender.out();

        gripper.release();

        extender.in();

        elevator.setHeight(Elevator::UPPERSHELF);

        extender.out();

        gripper.grip();

        extender.in();

        elevator.setHeight(Elevator::TOP);

        extender.out();

        gripper.release();

        extender.in();

        elevator.setHeight(Elevator::CHASSISCLEARANCE);

        DriveToPoint(intermediate);

        DriveToPoint(finish);
    }
    if (buttonA.isPressed()) {

        extender.out();
        gripper.release();
        elevator.setHeight(Elevator::GROUND);
    }
}

void Robot::EnterIdleState(void)
{
    chassis.Stop();

    Serial.println("-> IDLE");
    robotState = ROBOT_IDLE;
}