#include "../Configuration/RobotPragma.h"

///////////////////////////////////////////////
/// File Name: Teleop.V4.c
/// Creator: Chris Beck, Joslyn, Sou
/// Date of Creation: [12/10/12]
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
	// Place code here to initialize servos to starting positions.
	// Sensors are automatically configured and setup by ROBOTC. They may need a brief time to stabilize.
	//the servos are initialzed here so that in the event that a crate is held in autonomous it will not be dropped

	servoTarget[ramp] = 154;
	servoTarget[bamLeft] = 0;
	servoTarget[bamRight] = 254;
	servoChangeRate[ramp] = 5;

	motor[RightMotor] = 0;
	motor[LeftMotor] = 0;

	nMotorEncoder[lifter] = 0;
	nMotorEncoder[LeftMotor] = 0;
	nMotorEncoder[RightMotor] = 0;
	motor[stopLeft] = 50;
	motor[stopRight] = 50;
	wait10Msec(100);
	motor[stopLeft] = 0;
	motor[stopRight] = 0;
	nMotorEncoder[stopLeft] = 0;
	nMotorEncoder[stopRight] = 0;
	return;
}
bool speed = true;
////////////////////////////////////////////////////////////////////////////////////////

task main()
{
	initializeRobot();
	waitForStart();

	while(true)
	{
		getJoystickSettings(joystick);
		int jLeft = -(int)joystick.joy1_y1;
		int jRight = (int)joystick.joy1_y2;


		//Driver Controls//////////////////////////////////////////////////////////////////////////

		//////Drivetrain///////////////////////////////////////////////////////////////////////
		//Speed Control//
		if(joy1Btn(5))
		{
			speed = true;
		}
		//------------------------------
		if(joy1Btn(7))
		{
			speed = false;
		}
		//-------------------------------
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
		//--------------------------------
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

		//Manipulator Controls/////////////////////////////////////////////////////////////////////

		//////lifter///////////////////////////////////////////////////////////////////////////////

		int touchVal;
		touchVal = SensorValue(Touch);

		int lifterEncoderVal;
		lifterEncoderVal = nMotorEncoder[lifter];

		nxtDisplayCenteredBigTextLine(2, "%d", touchVal);
		nxtDisplayCenteredBigTextLine(5, "%d", lifterEncoderVal);

				//down
		if(joy2Btn(7) && (touchVal == 0))
		{
			motor[lifter] = -20;
		}
		//up
		else if(joy2Btn(5))
		{
			motor[lifter] = 70;
		}
		//Idle
		else
		{
			motor[lifter] = 0;
		}


		//OVERRIDE
		//down
		if(joy2Btn(4))
		{
			motor[lifter] = 50;
		}

		/*
		float encoderVal;
		nMotorEncoder[lifter] = encoderVal;
		nxtDisplayBigTextLine(2, "encoder val: %lf", encoderVal);
		*/
		/*

		//////Mechanical Stop/////////////////////////////////////////////////////////////////////////
		//Down////
		if(joy2Btn(2))
		{
		nMotorEncoderTarget[stopLeft] = 90;
		nMotorEncoderTarget[stopRight] = 90;
		ClearTimer(T1);
		while ((nMotorEncoder[stopLeft] < 90) && (time1[T1] < 500))
		{
		motor[stopLeft] = -30;
		motor[stopRight] = -30;
		}
		}

		//Up////
		if(joy2Btn(4))
		{
		nMotorEncoderTarget[stopLeft] = 0;
		nMotorEncoderTarget[stopRight] = 0;
		ClearTimer(T1);
		while ((nMotorEncoder[stopLeft] > 0) && (time1[T1] < 500))
		{
		motor[stopLeft] = 30;
		motor[stopRight] = 30;
		}
		motor[stopLeft] = 0;
		motor[stopRight] = 0;
		}

		*/

		//Ramp/////////////////////////////////////////////////////////////////////////
		if(joy2Btn(10))
		{
				servoTarget[ramp] = 1;
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////////


//End of File//
