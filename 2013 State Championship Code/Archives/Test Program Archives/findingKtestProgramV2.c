#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  HTServo)
#pragma config(Sensor, S2,     ,               sensorI2CCustom9V)
#pragma config(Sensor, S3,     ,               sensorI2CCustom)
#pragma config(Sensor, S4,     Gyro,           sensorI2CHiTechnicGyro)
#pragma config(Motor,  motorA,           ,             tmotorNXT, openLoop, encoder)
#pragma config(Motor,  motorB,           ,             tmotorNXT, openLoop, encoder)
#pragma config(Motor,  motorC,           ,             tmotorNXT, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C1_1,     LeftMotor,     tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     RightMotor,    tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     motorF,        tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C2_2,     motorG,        tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C3_1,     motorH,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_2,     motorI,        tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S1_C4_1,    servo1,               tServoContinuousRotation)
#pragma config(Servo,  srvo_S1_C4_2,    servo2,               tServoContinuousRotation)
#pragma config(Servo,  srvo_S1_C4_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_6,    servo6,               tServoStandard)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "JoystickDriver.c"

void initializeRobot()
{
  // Place code here to sinitialize servos to starting positions.
  // Sensors are automatically configured and setup by ROBOTC. They may need a brief time to stabilize.
  //the servos are initialzed here so that in the event that a crate is held in autonomous it will not be dropped

  motor[RightMotor] = 0;
  motor[LeftMotor] = 0;

  nMotorEncoder[RightMotor] = 0;
  nMotorEncoder[LeftMotor] = 0;

  return;
}

task main()
{
	initializeRobot();
	waitForStart();

	float conversionFactor = 152.7887;
	int Xspeed, Yspeed, errorX, errorY, direction;
	int Xdesired = 22.5*conversionFactor;
	int Xcurrent = 0;
	//int Ydesired,
	//int Ycurrent;
	float proportionConstant = 3;
	float totalSpeed;
	bool count = true;

	while(count)
	{
		direction = -1;

		Xcurrent = abs((nMotorEncoder[RightMotor] + nMotorEncoder[LeftMotor])/2)/**conversionFactor*/;

		//calculates distance needed to go
		errorX = Xdesired - Xcurrent;
		//errorY = Ydesired - Ycurrent;

		//sets the speed of the motor
		Xspeed = proportionConstant * direction * (errorX/conversionFactor);
		if(Xspeed > 100)
			Xspeed = 100;
		//Yspeed = proportionConstant * errorY;

		motor[LeftMotor] = Xspeed;
		motor[RightMotor] = Xspeed;

		if(Xdesired <= Xcurrent)
		{
			count = false;
			motor[LeftMotor] = 0;
			motor[RightMotor] = 0;
			nMotorEncoder[RightMotor] = 0;
  		nMotorEncoder[LeftMotor] = 0;
		}
		else
			count = true;
	}
}
