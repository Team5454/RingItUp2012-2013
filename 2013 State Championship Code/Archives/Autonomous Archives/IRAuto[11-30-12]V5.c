#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTServo,  none)
#pragma config(Sensor, S2,     IR,             sensorHiTechnicIRSeeker1200)
#pragma config(Sensor, S3,     Gyro,           sensorI2CHiTechnicGyro)
#pragma config(Sensor, S4,     Touch,          sensorTouch)
#pragma config(Motor,  motorA,          stopLeft,      tmotorNXT, openLoop, reversed)
#pragma config(Motor,  motorB,          stopRight,     tmotorNXT, openLoop)
#pragma config(Motor,  motorC,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  mtr_S1_C1_1,     RightMotor,    tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     LeftMotor,     tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     lifter,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     lifter,        tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S1_C3_1,    ramp,                 tServoStandard)
#pragma config(Servo,  srvo_S1_C3_2,    servo2,               tServoStandard)
#pragma config(Servo,  srvo_S1_C3_3,    bamLeft,              tServoStandard)
#pragma config(Servo,  srvo_S1_C3_4,    bamRight,             tServoStandard)
#pragma config(Servo,  srvo_S1_C3_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//


#include "Measurments.V2.c"
#include "MoveCommandsV1[2012-2013].c"
#include "JoystickDriver.c"

const int LIFTER_UP = 500;
const int LIFTER_DOWN = 0;

task main()
{
	waitForStart();
	initializeRobot();
	initialize_gyro();
	StartTask(process_gyro);
	//motor[finger] = 40;

	int irSensorVal;
	//int colorVal, colorVal2;
	//int eopdValLeft, eopdValRight;
	//int LightSensor1, LightSensor2;

	// move forward, turn 45 degrees, move forward
	move(1, 24, 2000);
	wait10Msec(50);
	move(1, 0, 1000);

	clearTimer(T1);

	//HTIRS2setDSPMode(InfraredSensor, DSP_1200);
	while (time1[T1] < 500)
	{
		irSensorVal = SensorValue[IR];
		nxtDisplayBigTextLine(2, "IR: %d", irSensorVal);
	}

	//////////////////////////////////////////////////////////////////

	switch(irSensorVal)
	{
	case 1:
		turngyro_left(-90.0, 35);
		wait10Msec(100);
		//start new code
		/*move(1, 9, 5000);
		wait10Msec(100);
		turngyro_left(90.0, 35);
		wait10Msec(100);
		move(1, 12, 5000);
		wait10Msec(100);
		while(LightSensor1 != true && LightSensor2 != true)
		{
			turngyro_left(-40, 15);
			wait10Msec(500);
			turngyro_left(20, 15);
			wait10Msec(500);
			move(1, 2, 5000);
			wait10Msec(500);
			turngyro_left(60, 15);
			wait10Msec(500);
			turngyro_left(-20, 15);
			wait10Msec(500);
			move(-1, 2, 5000);
			wait10Msec(500);
		}

		move(1, 8, 3000);
		wait10Msec(500);
		//end new code
		*/

		move(1, 45, 4000);
		wait10Msec(100);
		turngyro_left(83.0, 30);
		wait10Msec(100);
		move(1, 10, 1000);
		motor[RightMotor] = 50;
		wait10Msec(400);
		move(-1, 12, 2000);


		while(nMotorEncoderTarget(lifter) < LIFTER_UP)
		{
			motor[lifter] = 30;
		}
		move(1, 2, 300);

		/*
		nMotorEncoder[arm] = 0;
		wait1Msec(50);

		while(nMotorEncoder[arm] < 1600)
		{
			motor[arm] = 50;
		}
		motor[arm] = 0;
		wait1Msec(1500);
		motor[finger] = -40;
		wait1Msec(750);
		motor[finger] = 0;
		while(nMotorEncoder[arm] > 800)
		{
			motor[arm] = -50;
		}
		motor[arm] = 0;
		wait1Msec(1500);
		motor[finger] = 0;*/
		break;

		//------------------------------------------------------------------

	case 2:
		wait10Msec(100);
		move(-1, 2, 1000);
		turngyro_left(-50.0, 50);
		wait10Msec(50);

		while(nMotorEncoderTarget(lifter) < LIFTER_UP)
		{
			motor[lifter] = 30;
		}
		move(1, 2, 300);
/*
		ClearTimer(T1);
		nMotorEncoder[RightMotor] = 0;
		nMotorEncoder[LeftMotor] = 0;
		move(1, 56, 4000);
		ClearTimer(T1);
		move(-1, 12.5, 2000);
		nMotorEncoder[arm] = 0;
		wait1Msec(50);

		while(nMotorEncoder[arm] < 1600)
		{
			motor[arm] = 50;
		}
		motor[arm] = 0;
		wait1Msec(1500);
		motor[finger] = -40;
		wait1Msec(750);
		motor[finger] = 0;
		while(nMotorEncoder[arm] > 800)
		{
			motor[arm] = -50;
		}
		motor[arm] = 0;
		wait1Msec(1500);
		motor[finger] = 0;*/
		break;

		//-----------------------------------------------------

	case 3:
		move(1, 43, 2000);
		wait10Msec(100);
		turngyro_left(-100.0, 50);
		wait10Msec(100);
		move(1, 10, 1000);
		motor[LeftMotor] = 50;
		wait10Msec(400);
		move(-1, 12, 2000);

		while(nMotorEncoderTarget(lifter) < LIFTER_UP)
		{
			motor[lifter] = 30;
		}

		move(1, 2, 300);
		/*
		nMotorEncoder[arm] = 0;
		wait1Msec(50);
		while(nMotorEncoder[arm] < 1500)
		{
			motor[arm] = 50;
		}
		motor[arm] = 0;
		wait1Msec(1500);
		motor[finger] = -40;
		wait1Msec(750);
		motor[finger] = 0;
		while(nMotorEncoder[arm] > 800)
		{
			motor[arm] = -50;
		}
		motor[arm] = 0;
		wait1Msec(1500);
		motor[finger] = 0;*/
		break;
	}
}
