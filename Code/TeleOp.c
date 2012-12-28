#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTServo,  none)
#pragma config(Sensor, S2,     IR,             sensorHiTechnicIRSeeker1200)
#pragma config(Sensor, S3,     Gyro,           sensorI2CHiTechnicGyro)
#pragma config(Sensor, S4,     Touch,          sensorTouch)
#pragma config(Motor,  motorA,          stopLeft,      tmotorNXT, openLoop, reversed)
#pragma config(Motor,  motorB,          stopRight,     tmotorNXT, openLoop)
#pragma config(Motor,  motorC,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  mtr_S1_C1_1,     RightMotor,    tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     LeftMotor,     tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     lifter,        tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C2_2,     lifter,        tmotorTetrix, openLoop, encoder)
#pragma config(Servo,  srvo_S1_C3_1,    ramp,                 tServoStandard)
#pragma config(Servo,  srvo_S1_C3_2,    servo2,               tServoStandard)
#pragma config(Servo,  srvo_S1_C3_3,    bamLeft,              tServoStandard)
#pragma config(Servo,  srvo_S1_C3_4,    bamRight,             tServoStandard)
#pragma config(Servo,  srvo_S1_C3_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_6,    servo6,               tServoNone)

///////////////////////////////////////////////
/// File Name: TeleOp.c
/// Creator: Chris Beck, Joslyn
/// Last Editor: Chris Beck
///
/// Changes: Code cut up into header files to simplify it.
///	Multitasking added.
///
/// Description:
///		The teleoperated main program
///
///////////////////////////////////////////////
#include "JoystickDriver.c"
#include "TeleOpTasks.c"
#include "RobotConfig.c"

////////////////////////////////////////////////////////////////////////////////////////

task main()
{
	initializeRobot();
	waitForStart();

	StartTask(Drive);
	StartTask(Arm);
	StartTask(RAM);
	StartTask(Ramp);
}



//End of File//
