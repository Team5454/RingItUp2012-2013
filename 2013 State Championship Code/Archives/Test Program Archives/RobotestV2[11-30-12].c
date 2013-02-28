#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTServo,  none)
#pragma config(Sensor, S2,     Light,          sensorLightInactive)
#pragma config(Sensor, S3,     IR,             sensorHiTechnicIRSeeker1200)
#pragma config(Sensor, S4,     Gyro,           sensorI2CHiTechnicGyro)
#pragma config(Motor,  motorA,          finger,        tmotorNXT, openLoop, reversed)
#pragma config(Motor,  motorB,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  motorC,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  mtr_S1_C1_1,     LeftMotor,     tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     RightMotor,    tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     motorF,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     arm,           tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S1_C3_1,    bamLeft,              tServoStandard)
#pragma config(Servo,  srvo_S1_C3_2,    bamRight,             tServoStandard)
#pragma config(Servo,  srvo_S1_C3_3,    stopLeft,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_4,    stopRight,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "MoveCommandsV1[2012-2013].c"
int system = 0;
bool rechargable;

/////////////////////////////////////////////////////////////////////////////////////
void initializeRobot()
{
	// Place code here to sinitialize servos to starting positions.
	// Sensors are automatically configured and setup by ROBOTC. They may need a brief time to stabilize.
	//the servos are initialzed here so that in the event that a crate is held in autonomous it will not be dropped

	motor[RightMotor] = 0;
	motor[LeftMotor] = 0;

	nMotorEncoder[finger] = 0;
	nMotorEncoder[arm] = 0;
	nMotorEncoder[LeftMotor] = 0;
	nMotorEncoder[RightMotor] = 0;
	servoTarget[bamLeft] = 0;
	servoTarget[bamRight] = 255;
	ClearTimer(T1);
	while(time1[T1] < 1000)
	{
		motor[finger] = -30;
	}
	nMotorEncoder[finger] = 0;
	motor[finger] = 0;
	servoTarget[stopLeft] = 0;
	servoTarget[stopRight] = 255;
	return;
}

//////////////////////////////////////////////////////////////////////////
void selectProgram()
{
	eraseDisplay();
	// Enable Top Line NXT Display (Bluetooth, NXT Name & Battery Status)
	bNxtLCDStatusDisplay = true;
	// StopAllTasks();
	while(true)
	{
		nxtDisplayCenteredTextLine(1,"DITU SAYS WEAR");
		nxtDisplayCenteredTextLine(2,"SAFETY GLASSES");
		nxtDisplayCenteredTextLine(4, "Select Test:");
		//------------------------------------------------
		if(nNxtButtonPressed == 1)
		{
			system++;
			wait1Msec(400);
		}
		//-------------------------------------------------
		if(nNxtButtonPressed == 2)
		{
			system--;
			wait1Msec(400);
		}
		//-------------------------------------------------
		switch (system)
		{
		case 0: nxtDisplayCenteredTextLine(6, "Battery Power");break;
		case 1: nxtDisplayCenteredTextLine(6, "Drivetrain");break;
		case 2: nxtDisplayCenteredTextLine(6, "Gripper");break;
		case 3: nxtDisplayCenteredTextLine(6, "Arm");break;
		case 4: nxtDisplayCenteredTextLine(6, "BAM");break;
		case 5: nxtDisplayCenteredTextLine(6, "Mechanical Stop");break;
		case 6: nxtDisplayCenteredTextLine(6, "Gyro Sensor");break;
		case 7: nxtDisplayCenteredTextLine(6, "EOPD Sensor");break;
		case 8: nxtDisplayCenteredTextLine(6, "Color Sensor");break;
		}
		//---------------------------------------------------------------
		if(nNxtButtonPressed == 3)
		{
			eraseDisplay();
			break;
		}
		//---------------------------------------------------------------
	}
}
//////////////////////////////////////////////////////////////////////////////

task main()
{
	initializeRobot();
	////////////////////////////////////////////////////////////////////////////
	while(true)
	{
		selectProgram();

		switch(system)
		{
		case 0:
			while(true)
			{
				//----------------------------
				if(nNxtButtonPressed == 1)
				{
					rechargable = false;
					wait1Msec(400);
				}
				//----------------------------
				if(nNxtButtonPressed == 2)
				{
					rechargable = true;
					wait1Msec(400);
				}
				//------------------------------------------------------
				switch (rechargable)
				{
				case false: nxtDisplayCenteredTextLine(6, "Batteries");
					bNxtRechargable = false;
					break;
				case true: nxtDisplayCenteredTextLine(6, "Rechargable");
					bNxtRechargable = true;
					break;
				}
				//------------------------------------------------------
				if(nNxtButtonPressed == 3)
				{
					eraseDisplay();
					break;
				}
			//--------------------------------------------------------
			}
			ClearTimer(T1);
			while(time1(T1) < 4000)
			{
				string in = nImmediateBatteryLevel;
				string ex = externalBattery;
				nxtDisplayCenteredTextLine(5, in);
				nxtDisplayCenteredTextLine(7, ex);
			}
			break;

			//Drivetrain Test//////////////////////////////////////////////////////////
		case 1:
			motor[LeftMotor] = 50;
			wait10Msec(200);
			motor[LeftMotor] = 0;
			//-----------------------
			motor[LeftMotor] = -50;
			wait10Msec(200);
			motor[LeftMotor] = 0;
			//-----------------------
			motor[RightMotor] = 50;
			wait10Msec(200);
			motor[RightMotor] = 0;
			//-----------------------
			motor[RightMotor] = -50;
			wait10Msec(200);
			motor[RightMotor] = 0;
			//-----------------------
			motor[LeftMotor] = 50;
			motor[RightMotor] = 50;
			wait10Msec(200);
			motor[LeftMotor] = 0;
			motor[RightMotor] = 0;
			//-----------------------
			motor[LeftMotor] = 50;
			motor[RightMotor] = 50;
			wait10Msec(200);
			motor[LeftMotor] = 0;
			motor[RightMotor] = 0;
			//-----------------------
			break;

			//Gripper Test//////////////////////////////////////////////////////////
		case 2:
			nMotorEncoderTarget[finger] = 90;
			ClearTimer(T1);
			while ((nMotorEncoder[finger] < 90) && (time1[T1] < 500))
			{
				motor[finger] = 30;
			}
			wait10Msec(100);
			//-------------------------------------------------------
			nMotorEncoderTarget[finger] = 0;
			ClearTimer(T1);
			while ((nMotorEncoder[finger] > 0) && (time1[T1] < 500))
			{
				motor[finger] = -30;
			}
			motor[finger] = 0;

			break;

			//Arm Test////////////////////////////////////////////////////////////////
		case 3:
			//-----------------------------
			motor[arm] = 20;
			wait10Msec(300);
			//-----------------------------
			motor[arm] = -20;
			wait10Msec(300);
			//-----------------------------
			break;
			//BAM/////////////////////////////////////////////////////////////////////
		case 4:
			servoTarget[bamLeft] = 255;
			servoTarget[bamRight] = 0;
			wait10Msec(200);
			//------------------------------
			servoTarget[bamLeft] = 0;
			servoTarget[bamRight] = 255;
			wait10Msec(200);
			//------------------------------
			servoTarget[bamLeft] = 127;
			servoTarget[bamRight] = 127;
			wait10Msec(200);
			break;
			//Mechanical Stop/////////////////////////////////////////////////////////
		case 5:
			servoTarget[stopLeft] = 255;
			servoTarget[stopRight] = 0;
			wait10Msec(200);
			//------------------------------
			servoTarget[stopLeft] = 0;
			servoTarget[stopRight] = 255;
			wait10Msec(200);
			//------------------------------
			servoTarget[stopLeft] = 127;
			servoTarget[stopRight] = 127;
			wait10Msec(200);
			break;
			//Gyro/////////////////////////////////////////////////////////////////////
		case 6:
			turngyro_right(90, 30);
			wait10Msec(200);
			//----------------------------
			turngyro_right(-90, 30);
			wait10Msec(200);
			//----------------------------
			turngyro_right(45, 30);
			wait10Msec(200);
			//----------------------------
			turngyro_right(-45, 30);
			wait10Msec(200);
			break;
		}
	}
}