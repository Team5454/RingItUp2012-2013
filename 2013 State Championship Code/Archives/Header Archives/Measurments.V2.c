//#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  HTServo)
//#pragma config(Sensor, S2,     ,               sensorI2CCustom9V)
//#pragma config(Sensor, S3,     ,               sensorI2CCustom)
//#pragma config(Sensor, S4,     Gyro,           sensorI2CHiTechnicGyro)
//#pragma config(Motor,  motorA,           ,             tmotorNXT, openLoop, encoder)
//#pragma config(Motor,  motorB,           ,             tmotorNXT, openLoop, encoder)
//#pragma config(Motor,  motorC,           ,             tmotorNXT, openLoop, encoder)
//#pragma config(Motor,  mtr_S1_C1_1,     LeftMotor,     tmotorTetrix, openLoop, encoder)
//#pragma config(Motor,  mtr_S1_C1_2,     RightMotor,    tmotorTetrix, openLoop, reversed, encoder)
//#pragma config(Motor,  mtr_S1_C2_1,     motorF,        tmotorTetrix, openLoop, reversed, encoder)
//#pragma config(Motor,  mtr_S1_C2_2,     motorG,        tmotorTetrix, openLoop, encoder)
//#pragma config(Motor,  mtr_S1_C3_1,     motorH,        tmotorTetrix, openLoop)
//#pragma config(Motor,  mtr_S1_C3_2,     motorI,        tmotorTetrix, openLoop)
//#pragma config(Servo,  srvo_S1_C4_1,    servo1,               tServoContinuousRotation)
//#pragma config(Servo,  srvo_S1_C4_2,    servo2,               tServoContinuousRotation)
//#pragma config(Servo,  srvo_S1_C4_3,    servo3,               tServoNone)
//#pragma config(Servo,  srvo_S1_C4_4,    servo4,               tServoNone)
//#pragma config(Servo,  srvo_S1_C4_5,    servo5,               tServoNone)
//#pragma config(Servo,  srvo_S1_C4_6,    servo6,               tServoStandard)
////*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

///////////////////////////////////////////////
/// File Name: Measurments.V2.h
/// Creator: Chris Beck
/// Date of Creation: [10/17/12]
/// Last Editor: N/A
///
/// Changes: N/A
///
/// Description:
///		The Autonomous main program
///
///////////////////////////////////////////////

//struct Distance
//{
//	float wheelCircumference;
//	int ticksPerRotation;
//	int wheelDiameter;
//	int init_Angle;
//	int tickConst;
//	int angle;
//	int inch;
//	int foot;
//	int tick;
//	float pi;
//}

//Distance Values///////////////////////////////////////
//Distance units;
//	units.inch = (wheelCircumference/(ticksPerRotation);
//	units.wheelCircumference = pi * wheelDiamter;
//	units.tick = wheelCircumference/1440;
//	units.ticksPerRotation = 1440;
//	units.tickConst = 114.591641;
//	units.wheelDiameter = 4;
//	units.pi = 3.14159265;
//	units.foot = 12*inch;

////
//float init_Angle = SensorValue[S1];
//int Field = (fieldX,fieldY);
//int fieldX = 144*inch;
//int fieldY = 144*inch;


//while(true)
//{
//float Theta = SensorValue[S1];
//}

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

void move(int direction, float Xvalue, int timeout)
{
	float conversionFactor = 114.588;
	int Xspeed, errorX;
	//iny Yspeed, , errorY;
	int Xdesired = Xvalue*conversionFactor;
	int Xcurrent = 0;
	//int Ydesired,
	//int Ycurrent;
	float proportionConstant = 3;
	//float totalSpeed;
	bool count = true;

	nMotorEncoder[RightMotor] = 0;
  nMotorEncoder[LeftMotor] = 0;

  wait1Msec(50);
  ClearTimer(T1);

	while(count && (time1[T1]<timeout))
	{
		Xcurrent = abs((nMotorEncoder[RightMotor] + nMotorEncoder[LeftMotor])/2)/**conversionFactor*/;

		//calculates distance needed to go
		errorX = Xdesired - Xcurrent;
		//errorY = Ydesired - Ycurrent;

		//sets the speed of the motor
		Xspeed = proportionConstant * (direction) * (errorX/conversionFactor);
		if(Xspeed > 100)
			Xspeed = 100;
	//
		if(Xspeed < 20)
			Xspeed = 20 * (direction);
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
	count = false;
	motor[LeftMotor] = 0;
	motor[RightMotor] = 0;
	nMotorEncoder[RightMotor] = 0;
  nMotorEncoder[LeftMotor] = 0;

}

//End of File//
