#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  HTServo)
#pragma config(Sensor, S4,     HTPB,           sensorI2CCustom9V)
#pragma config(Motor,  motorA,          RamRight,      tmotorNXT, openLoop)
#pragma config(Motor,  motorB,          RamLeft,       tmotorNXT, openLoop)
#pragma config(Motor,  motorC,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  mtr_S1_C1_1,     RightMotor,    tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,     RightMotor,    tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     LeftMotor,     tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     LeftMotor,     tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C3_1,     elevatorA,     tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C3_2,     elevatorB,     tmotorTetrix, openLoop, reversed)
#pragma config(Servo,  srvo_S1_C4_1,    BamContR,             tServoContinuousRotation)
#pragma config(Servo,  srvo_S1_C4_2,    BamContL,             tServoContinuousRotation)
#pragma config(Servo,  srvo_S1_C4_3,    ramp,                 tServoStandard)
#pragma config(Servo,  srvo_S1_C4_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

///////////////////////////////////////////////
/// File Name: TeleOp.c
/// Creator: Chris Beck, Joslyn Beckert
/// Last Editor: Joslyn
///
/// Changes: Code cut up into header files to simplify it.
///	Multitasking added.
///
/// Description:
///		The teleoperated main program
///
///////////////////////////////////////////////
#include "JoystickDriver.c"
#include "../Configuration/TeleOpTasks.c"
#include "../Configuration/RobotConfig.c"
//#include "../Utilities/RobotMoveUtilities.c"
#include "../Utilities/Robotc Includes/hitechnic-sensormux.h"
#include "../Utilities/Robotc Includes/hitechnic-protoboard.h"

////////////////////////////////////////////////////////////////////////////////////////

task main()
{
	initializeRobot();
	waitForStart();

	StartTask(Drive);
	StartTask(Lifter);
	StartTask(RAM);
	StartTask(BAM);
	StartTask(DeployRamp);

	while(true)
	{
		getJoystickSettings(joystick);
		nxtDisplayCenteredTextLine(1, "Happily Running :)");
}
}


//End of File//
