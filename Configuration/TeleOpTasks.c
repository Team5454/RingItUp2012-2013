#include "../Utilities/RobotMoveUtilities.c"
#include "../Utilities/Robotc Includes/hitechnic-sensormux.h"
#include "JoystickDriver.c"
#include "../Utilities/Robotc Includes/hitechnic-protoboard.h"
int limit = 0;

//Variables/////////////////////////////////////////////////////////////////////
const tMUXSensor Color = msensor_S3_1;
const tMUXSensor TouchSensor = msensor_S3_2;
const tMUXSensor IR = msensor_S3_3;


/////////////////////////////////////////////////////////////////////////////////
///////////////////// NEEDS TO BE WRITTEN BEFORE TOURNAMENT /////////////////////
/////////////////////////////////////////////////////////////////////////////////

//values are wrong
/*int readJoystick(int value)
{
if (joy2Btn()) // 1 height
value = (int)1440*18/(2*PI);
else if (joy2Btn()) // 2 height
value = (int)1440*32/(2*PI);
else if (joy2Btn()) // 3 height
value = (int)1440*46/(2*PI);
return value;
}*/

task BAM()
{
	// not sure how to dispense rings yet
	int value = 130;
	if(6)
	{

		//Move BAM apart
		while(nMotorEncoder[BAM] < 130)
		{
			servo[BamContL] = 40;
			servo[BamContR] = 40;
		}
	}

	bool BamTogether;
	//Move BAM together
	if(8)
	{
		BamTogether = true;
		while(BamTogether)
		{
			servo[BamContL] = -40;
			servo[BamContR] = -40;
		}

		servo[BamContL] = 40;
		servo[BamContR] = 40;
		wait10Msec(500);
	}

	BamTogether = false;

	servo[BamContL] = 0;
	servo[BamContR] = 0;

	servoTarget[BAM] = value;
	return;
}

///////////////////////////////////////////////////////////////
/////////////////////////// PRESETS ///////////////////////////
///////////////////////////////////////////////////////////////

/*int elevatorGoToHeight(int Xdesired)
{
int Xcurrent;
int errorX;
int Xspeed;
//int lifterEncoderVal;
int touchValTop = 0;
int touchValBottom = 0;
bool automode;
int elevatorDirection = 1;
int proportionConstant = 1; // figure this out!!!!
automode = true;
Xcurrent = nMotorEncoder[motorA];

while(((Xcurrent > Xdesired -40) || (Xcurrent < Xdesired +40)) && touchValBottom == 0)
{
Xdesired = readJoystick(Xdesired);  //reads controller to see if fickle user wants to go somewhere else instead

//reads limit switches
ProcessProto();

//reads xcurrent
Xcurrent = nMotorEncoder[lifter];

//calculates what the distance you still need to go and determines the adjusted speed
errorX = Xdesired - Xcurrent;

//back up if overshoot
if (Xcurrent > Xdesired)
elevatorDirection = elevatorDirection * -1;

//////////////////////////////////////////////////////////
//////////           SETS THE SPEED             //////////
//////////////////////////////////////////////////////////
Xspeed = proportionConstant * errorX * elevatorDirection;
//////////////////////////////////////////////////////////

//corrects speed for distances over 100
if(abs(Xspeed) > 75)
Xspeed = 100*elevatorDirection;

//sets the speed of the motors
motor[lifter] = Xspeed;

//stop button
if(joy2Btn() == 1)
{
motor[lifter] = 0;
break;
}
}
motor[lifter] = 0;
return 0;
}
*/

//////////////////////////////////////////////////////////////////////////////
////////////////////////// DRIVE TRAIN CODE IS DONE //////////////////////////
//////////////////////////////////////////////////////////////////////////////

task Drive()
{
	bool speed = true;

	while(true)
	{
		while(true)
		{
			int jLeft = -(int)joystick.joy1_y1;
			int jRight = (int)joystick.joy1_y2;

			if (abs(jLeft) < 10)
			{///< core out the noise for near zero settings
				motor[LeftMotor] = 0;
			}
			else
			{
				motor[LeftMotor] = (jLeft/126)*100;     ///< set motors to joystick settings
			}
			if (abs(jRight) < 10)  {         ///< core out the noise for near zero settings
				motor[RightMotor] = 0;        ///< sets the right motor to 0% power
			}
			else
			{
				motor[RightMotor] = (jRight/126)*100;   ///< sets motors to joystick settings
			}
		}

	}

	/*
	task Lifter()
	{

	////////////////////////////// RANDOM JUNK //////////////////////////////


	int Xdesired; // the encoder value of height we want to obtain
	int Xcurrent;
	//int lifterEncoderVal;
	int touchValTop;
	int touchValBottom;
	bool automode;
	int elevatorDirection = 1;
	int home = (int)1440*18/(2*PI);
	int pickUpRings = -5;
	int value;
	bool resetEncoders = true;

	automode = true;
	//int joystickCurrent;

	while (true)
	{
	//where is it at????
	//go to bottom and start there
	//hit limit switch and reset the encoders at that point
	resetEncodersAtBeginning(resetEncoders);

	//sets the value of xcurrent and xdesired
	Xcurrent = nMotorEncoder[lifter];
	Xdesired = readJoystick(Xcurrent);

	bool mode = isAuto(value);

	///////////////////////////////////////////////////
	/////       ACTUAL TELE-OP MOVING STUFF       /////
	///////////////////////////////////////////////////

	if (Xdesired != Xcurrent)
	{
	//go down until ring is picked up
	//go up until color sensor can be used to reset encoder

	elevatorGoToHeight(Xdesired);

	//if 1 stage
	if (mode)
	{
	dispenseRings();
	elevatorGoToHeight(home);
	mode = 0;
	}
	}

	//read limit switch


	if(joy1Btn(5))
	motor[lifter] = 50;
	else if(joy1Btn(7))
	motor[lifter] = -50;
	else
	motor[lifter] = 0;


	//dispense rings
	if (joy2Btn() == 1)
	dispenseRings();

	//stop button
	if(joy2Btn() == 1)
	{
	motor[lifter] = 0;
	break;
	}


	}
	*/

}

task Lifter()
{

	int encoder;

	getJoystickSettings(joystick);
	bDisplayDiagnostics = false;
	bNxtLCDStatusDisplay = false;
	ProcessProto();

	//nxtDisplayCenteredTextLine(2, "Desired = %d", scaledDesiredHeight);
	nxtDisplayCenteredTextLine(4, "Limit = %d", limit);

	if(limit == 1)
	{
		if(joy1Btn(5))
		{
			motor[elevatorA] = 85;
			motor[elevatorB] = 85;
		}
		else
		{
			motor[elevatorA] = 0;
			motor[elevatorB] = 0;
		}
	}
	else
	{
		if(joy1Btn(5))
		{
			motor[elevatorA] = 85;
			motor[elevatorB] = 85;
		}
		else if(joy1Btn(7))
		{
			motor[elevatorA] = -55;
			motor[elevatorB] = -55;
		}
		else if(joy1Btn(1))
			nMotorEncoder[elevatorA] = 0;
		else
		{
			motor[elevatorA] = 0;
			motor[elevatorB] = 0;
		}
	}

	encoder = nMotorEncoder[elevatorA]*-1;

	/*if(joy1Btn(2))
	{
	while(encoder <= scaledDesiredHeight)
	{
	encoder = nMotorEncoder[elevatorA]*-1;
	motor[elevatorA] = 50;
	motor[elevatorB] = 50;
	}
	motor[elevatorA] = 0;
	motor[elevatorB] = 0;
	}*/

	nxtDisplayCenteredTextLine(3, "Encoder = %d", encoder);


	//while(true)
	//{
	//	HTPBsetupIO(HTPB, 0x0);
	//	nMotorEncoder[elevatorA] = 0;
	//	int encoder;
	//	int desiredHeight = 20; //desired height to go to in inches
	//	int scaledDesiredHeight = (int)(desiredHeight - 2.7282)/.0043; //converts inches to encoder counts for test rig
	//	//StartTask(ProcessProto);

	//	while(true)
	//	{
	//		getJoystickSettings(joystick);
	//		bDisplayDiagnostics = false;
	//		bNxtLCDStatusDisplay = false;
	//		ProcessProto();

	//		nxtDisplayCenteredTextLine(2, "Desired = %d", scaledDesiredHeight);
	//		nxtDisplayCenteredTextLine(4, "Limit = %d", limit);

	//		if(limit == 1)
	//		{
	//			if(joy1Btn(5))
	//			{
	//				motor[elevatorA] = 75;
	//				motor[elevatorB] = 75;
	//			}
	//			else
	//			{
	//				motor[elevatorA] = 0;
	//				motor[elevatorB] = 0;
	//			}
	//		}
	//		else
	//		{
	//			if(joy1Btn(5))
	//			{
	//				motor[elevatorA] = 75;
	//				motor[elevatorB] = 75;
	//			}
	//			else if(joy1Btn(7))
	//			{
	//				motor[elevatorA] = -55;
	//				motor[elevatorB] = -55;
	//			}
	//			else if(joy1Btn(1))
	//				nMotorEncoder[elevatorA] = 0;
	//			else
	//			{
	//				motor[elevatorA] = 0;
	//				motor[elevatorB] = 0;
	//			}
	//		}

	//		encoder = nMotorEncoder[elevatorA]*-1;

	//		if(joy1Btn(2))
	//		{
	//			while(encoder <= scaledDesiredHeight)
	//			{
	//				encoder = nMotorEncoder[elevatorA]*-1;
	//				motor[elevatorA] = 50;
	//				motor[elevatorB] = 50;
	//			}
	//			motor[elevatorA] = 0;
	//			motor[elevatorB] = 0;
	//		}

	//		nxtDisplayCenteredTextLine(3, "Encoder = %d", encoder);

}


/*
////////////////
SECOND MEET CODE
////////////////

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
*/



task RAM()
{
	while(true)
	{
		//////Mechanical Stop/////////////////////////////////////////////////////////////////////////
		//Down////
		if(joy2Btn(2))
		{
			nMotorEncoderTarget[RamLeft] = 90;
			nMotorEncoderTarget[RamRight] = 90;
			ClearTimer(T1);
			while ((nMotorEncoder[RamLeft] < 90) && (time1[T1] < 500))
			{
				motor[RamLeft] = -30;
				motor[RamRight] = -30;
			}
		}

		//Up////
		if(joy2Btn(4))
		{
			nMotorEncoderTarget[RamLeft] = 0;
			nMotorEncoderTarget[RamRight] = 0;
			ClearTimer(T1);
			while ((nMotorEncoder[RamLeft] > 0) && (time1[T1] < 500))
			{
				motor[RamLeft] = 30;
				motor[RamRight] = 30;
			}
			motor[RamLeft] = 0;
			motor[RamRight] = 0;
		}
	}
}

task DeployRamp()
{
	while(true)
	{
		if(joy2Btn(10))
		{
			servoTarget[ramp] = 1;
		}
	}
}
