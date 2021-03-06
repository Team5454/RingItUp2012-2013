#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTServo,  none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     Light,          sensorLightInactive)
#pragma config(Sensor, S3,     IR,             sensorHiTechnicIRSeeker1200)
#pragma config(Sensor, S4,     Gyro,           sensorI2CHiTechnicGyro)
#pragma config(Motor,  motorA,          stopLeft,      tmotorNXT, openLoop, reversed)
#pragma config(Motor,  motorB,          stopRight,     tmotorNXT, openLoop)
#pragma config(Motor,  motorC,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  mtr_S1_C1_1,     RightMotor,    tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     LeftMotor,     tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     motorF,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     arm,           tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S1_C3_1,    servo1,               tServoStandard)
#pragma config(Servo,  srvo_S1_C3_2,    servo2,               tServoStandard)
#pragma config(Servo,  srvo_S1_C3_3,    bamLeft,              tServoStandard)
#pragma config(Servo,  srvo_S1_C3_4,    bamRight,             tServoStandard)
#pragma config(Servo,  srvo_S1_C3_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

///////////////////////////////////////////////
/// File Name: Teleop.V2.c
/// Creator: Chris Beck, Joslyn, Sou
/// Date of Creation: [12/07/12]
/// Last Editor: Chris Beck
///
/// Changes: Modifications to logic to enable correct functioning of code and corrections of errors.
///
/// Description:
///		The teleoperated main program
///
///////////////////////////////////////////////
#include "JoystickDriver.c"

void initializeRobot()
{
	// Place code here to sinitialize servos to starting positions.
	// Sensors are automatically configured and setup by ROBOTC. They may need a brief time to stabilize.
	//the servos are initialzed here so that in the event that a crate is held in autonomous it will not be dropped

	motor[RightMotor] = 0;
	motor[LeftMotor] = 0;

	nMotorEncoder[arm] = 0;
	nMotorEncoder[LeftMotor] = 0;
	nMotorEncoder[RightMotor] = 0;
	servoTarget[bamLeft] = 0;
	servoTarget[bamRight] = 254;
	motor[stopLeft] = 0;
	motor[stopRight] = 0;
	return;
}
bool speed = true;
bool go = false;
////////////////////////////////////////////////////////////////////////////////////////
task main()
{
	waitForStart();
	initializeRobot();

	while(true)
	{
		getJoystickSettings(joystick);
		int jLeft = (int)joystick.joy1_y1;
		int jRight = -(int)joystick.joy1_y2;


		//Driver Controls//////////////////////////////////////////////////////////////////////////

		//////Drivetrain///////////////////////////////////////////////////////////////////////
		//Speed Control
		if(joy1Btn(5))
		{
			speed = true;
		}
		if(joy1Btn(7))
		{
			speed = false;
		}

		if (speed)
		{
			if (abs(jLeft) < 10)            ///< core out the noise for near zero settings
				motor[LeftMotor] = 0;         ///< sets the left motor to 0% power
			else
				motor[LeftMotor] = jLeft;     ///< set motors to joystick settings
			if (abs(jRight) < 10)           ///< core out the noise for near zero settings
				motor[RightMotor] = 0;        ///< sets the right motor to 0% power
			else
				motor[RightMotor] = jRight;   ///< sets motors to joystick settings
		}

		else
		{
			if (abs(jLeft) < 10)              ///< core out the noise for near zero settings
				motor[LeftMotor] = 0;           ///< sets the left motor to 0% power
			else
				motor[LeftMotor] = (jLeft/3);   ///< set motors to joystick settings
			if (abs(jRight) < 10)             ///< core out the noise for near zero settings
				motor[RightMotor] = 0;          ///< sets the right motor to 0% power
			else
				motor[RightMotor] = (jRight/3); ///< sets motors to joystick settings
		}

		//////BAM////////////////////////////////////////////////////////////////////////////////
		//up
		if(joy1Btn(8))
		{
			servoTarget[bamLeft] = 255;
			servoTarget[bamRight] = 0;
		}
		//down
		else if(joy1Btn(6))
		{
			servoTarget[bamLeft] = 0;
			servoTarget[bamRight] = 255;
		}
		else
		{
			servoTarget[bamLeft] = 127;
			servoTarget[bamRight] = 127;
		}


		//Manipulator Controls/////////////////////////////////////////////////////////////////////

		//////Arm//////////////////////////////////////////////////////////////////////////////
		//up
		if(joy2Btn(7))
		{
			motor[arm] = 20;
		}
		//down
		else if(joy2Btn(5))
		{
			motor[arm] = -20;
		}
		else
		{
			motor[arm] = 0;
		}


		//////Fingers///////////////////////////////////////////////////////////////////////////////
		//if(joy2Btn(6))
		//{
		//	nMotorEncoderTarget[finger] = 90;
		//	ClearTimer(T1);
		//	while ((nMotorEncoder[finger] < 90) && (time1[T1] < 500))
		//	{
		//		motor[finger] = 30;
		//	}
		//}

		//if(joy2Btn(8))
		//{
		//	nMotorEncoderTarget[finger] = 0;
		//	ClearTimer(T1);
		//	while ((nMotorEncoder[finger] > 0) && (time1[T1] < 500))
		//	{
		//		motor[finger] = -30;
		//	}
		//	motor[finger] = 0;
		//}

		//////Mechanical Stop/////////////////////////////////////////////////////////////////////////
		//Down//////////////////////
		if(joy2Btn(2))
		{
			//while(!(joy2Btn(4)))
			//{
			motor[stopLeft] = -50;
			motor[stopRight] = 50;
			//}
		}
		//Up/////////////////////
		if(joy2Btn(4))
		{
			motor[stopLeft] = 50;
			motor[stopRight] = -50;
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////////


//End of File//
