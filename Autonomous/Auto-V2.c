#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  HTServo)
#pragma config(Sensor, S2,     Gyro,           sensorI2CHiTechnicGyro)
#pragma config(Sensor, S3,     colorSensorVal, sensorI2CHiTechnicColor)
#pragma config(Sensor, S4,     HTPB,           sensorI2CCustom9V)
#pragma config(Motor,  motorA,          RAMright,      tmotorNXT, openLoop, encoder)
#pragma config(Motor,  motorB,          RAMleft,       tmotorNXT, openLoop, encoder)
#pragma config(Motor,  motorC,          RampServo,             tmotorNXT, openLoop)
#pragma config(Motor,  mtr_S1_C1_1,     Right1,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,     Right2,        tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     Left1,         tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C2_2,     Left2,         tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C3_1,     elevatorA,     tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C3_2,     elevatorB,     tmotorTetrix, openLoop, reversed)
#pragma config(Servo,  srvo_S1_C4_1,    WAMrightServo,        tServoStandard)
#pragma config(Servo,  srvo_S1_C4_2,    WAMleftServo,         tServoStandard)
#pragma config(Servo,  srvo_S1_C4_3,    colorSensorServo,            tServoStandard)
#pragma config(Servo,  srvo_S1_C4_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "JoystickDriver.c"
#include "..\Utilities\Robotc Includes\hitechnic-protoboard.h"
#include "..\Utilities\AutoMoveCommands-V1.0.c"

//initializes limit swithes as not pressed
int bottomLimit = 0;
int ringLimit = 0;
int ramLimit = 0;

// Servo Starting Positions.  These values are also used to restrict servo motion during tele-op operation.
int colorServoDefault = 157;
int colorServoDown = 229;
int WAMrightServoStart = 125;	// WAM Left Servo Start Point
int WAMleftServoStart = 103;	// WAM Left Servo Start Point

//Gives the initial values for the autonomous selection program
int Row = 0;	// Row to score the first ring
int AutoActions = 0;	// User selected autonomous actions
int delayAutoStart = 0;	// User selected autonomous delay selection
int delayAutoStartValue = 0;	// User selected autonomous delay in seconds
int robotStartLocation = 0;	// User selected robot start location

// Initialize robot starting location
bool leftWall=false;
bool corner=false;
bool rightWall=false;

int pegHeightCmd = 0;	// Initialize the selected row

//Fail safe variable
bool tripped;

// The following initialization code runs before the robot receives the start match command from the Field Control System
void initializeRobot()
{
	//Stop DC motors
	motor[Right1] = 0;
	motor[Right2] = 0;
	motor[Left1] = 0;
	motor[Left2] = 0;

	//Stop NXT motors
	motor[motorA] = 0;
	motor[motorB] = 0;
	motor[motorC] = 0;

	//Initialize Servos to starting positions
	servoChangeRate[WAMrightServo] = 10;   //Set how fast servo moves.  ROBOTC default is 10
	servoChangeRate[WAMleftServo] = 10;
	// Servo range:  0 - 255
	servo[WAMrightServo] = WAMrightServoStart;	// WAM Right Side
	servo[WAMleftServo] = WAMleftServoStart;	// WAM Left Side
	servo[colorSensorServo] = colorServoDown;

	//zeros gyros
	initialize_gyro();
	StartTask(process_gyro);
	wait1Msec(1000);

	//Initialize Sensors
	HTPBsetupIO(HTPB, 0x0); //initializes the protoboard
	nMotorEncoder[elevatorA] = 0; //resets the elevator encoder

	return;
}

////////////////////////////////////////////////////////////////////////////////
//Select Robot Start Location (Wall) -----------------------------------------//
////////////////////////////////////////////////////////////////////////////////
void selectStartLocation()
{
	eraseDisplay();
	// Enable Top Line NXT Display (Bluetooth, NXT Name & Battery Status)
	//bNxtLCDStatusDisplay = true;
	while(true)
	{
		nxtDisplayCenteredTextLine(1,"DITU SAYS WEAR");
		nxtDisplayCenteredTextLine(2,"SAFETY GLASSES");
		nxtDisplayCenteredTextLine(4,"Start Location:");
		//------------------------------------------------
		if(nNxtButtonPressed == 1)	// NXT Right Triangle Button
		{
			robotStartLocation++;
			wait1Msec(400);
		}
		//-------------------------------------------------
		if(nNxtButtonPressed == 2)	// NXT Left Triangle Button
		{
			robotStartLocation--;
			wait1Msec(400);
		}
		// Keep AutoActions within a range of valid values
		if (robotStartLocation > 2) robotStartLocation = 2;
		if (robotStartLocation < 0) robotStartLocation = 0;
		//-------------------------------------------------
		switch (robotStartLocation)
		{
		case 0: nxtDisplayCenteredTextLine(6, "Left Wall");break;
		case 1: nxtDisplayCenteredTextLine(6, "Corner");break;
		case 2: nxtDisplayCenteredTextLine(6, "Right Wall");break;
		default: nxtDisplayCenteredTextLine(6, "Error");break;
		}
		//---------------------------------------------------------------
		if(nNxtButtonPressed == 3)	// NXT Orange Button
		{
			wait1Msec(400);
			break;
		}
		//---------------------------------------------------------------
	}
}

////////////////////////////////////////////////////////////////////////////////
//Select Autonomous Actions --------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////
void selectAutoActions()
{
	eraseDisplay();
	// Enable Top Line NXT Display (Bluetooth, NXT Name & Battery Status)
	//bNxtLCDStatusDisplay = true;
	while(true)
	{
		nxtDisplayCenteredTextLine(1,"DITU SAYS WEAR");
		nxtDisplayCenteredTextLine(2,"SAFETY GLASSES");
		nxtDisplayCenteredTextLine(4,"Select Column:");
		//------------------------------------------------
		if(nNxtButtonPressed == 1)	// NXT Right Triangle Button
		{
			AutoActions++;
			wait1Msec(400);
		}
		//-------------------------------------------------
		if(nNxtButtonPressed == 2)	// NXT Left Triangle Button
		{
			AutoActions--;
			wait1Msec(400);
		}
		// Keep AutoActions within a range of valid values
		if (AutoActions > 5) AutoActions = 5;
		if (AutoActions < 0) AutoActions = 0;
		//-------------------------------------------------
		switch (AutoActions)
		{
		case 0: nxtDisplayCenteredTextLine(6, "No Auto");break;
		case 1: nxtDisplayCenteredTextLine(6, "IR Beacon");break;
		case 2: nxtDisplayCenteredTextLine(6, "Left Column");break;
		case 3: nxtDisplayCenteredTextLine(6, "Center Column");break;
		case 4: nxtDisplayCenteredTextLine(6, "Right Column");break;
		case 5: nxtDisplayCenteredTextLine(6, "Play Defense");break;
		default: nxtDisplayCenteredTextLine(6, "Error");break;
		}
		//---------------------------------------------------------------
		if(nNxtButtonPressed == 3)	// NXT Orange Button
		{
			wait1Msec(400);
			break;
		}
		//---------------------------------------------------------------
	}
}

////////////////////////////////////////////////////////////////////////////////
//Select Row------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////
void selectRow()
{
	eraseDisplay();
	// Enable Top Line NXT Display (Bluetooth, NXT Name & Battery Status)
	//bNxtLCDStatusDisplay = true;
	while(true)
	{
		nxtDisplayCenteredTextLine(1,"DITU SAYS WEAR");
		nxtDisplayCenteredTextLine(2,"SAFETY GLASSES");
		nxtDisplayCenteredTextLine(4,"Peg Row:");
		//------------------------------------------------
		if(nNxtButtonPressed == 1)	// NXT Right Triangle Button
		{
			Row++;
			wait1Msec(400);
		}
		//-------------------------------------------------
		if(nNxtButtonPressed == 2)	// NXT Left Triangle Button
		{
			Row--;
			wait1Msec(400);
		}
		//---------------------------------------------------------------
		// Keep Row within a range of valid values
		if (Row > 2) Row = 2;
		if (Row < 0) Row = 0;
		switch (Row)
		{
		case 0: nxtDisplayCenteredTextLine(6, "Bottom Row");break;
		case 1: nxtDisplayCenteredTextLine(6, "Middle Row");break;
		case 2: nxtDisplayCenteredTextLine(6, "Top Row");break;
		default: nxtDisplayCenteredTextLine(6, "Error");break;
		}
		//---------------------------------------------------------------
		if(nNxtButtonPressed == 3)	// NXT Orange Button
		{																																				////////////////	You  need to return a variable
			wait1Msec(400);																										////////////////	that selects the auto program
			break;																																////////////////	for later. 			-Jos :)
		}
		//---------------------------------------------------------------
	}
}

////////////////////////////////////////////////////////////////////////////////
//Select Autonomous Start Delay Time------------------------------------------//
////////////////////////////////////////////////////////////////////////////////
void selectAutoStartDelay()
{
	eraseDisplay();
	// Enable Top Line NXT Display (Bluetooth, NXT Name & Battery Status)
	//bNxtLCDStatusDisplay = true;
	while(true)
	{
		nxtDisplayCenteredTextLine(1,"DITU SAYS WEAR");
		nxtDisplayCenteredTextLine(2,"SAFETY GLASSES");
		nxtDisplayCenteredTextLine(4,"Start Delay:");
		//------------------------------------------------
		if(nNxtButtonPressed == 1)	// NXT Right Triangle Button
		{
			delayAutoStart++;
			wait1Msec(400);
		}
		//-------------------------------------------------
		if(nNxtButtonPressed == 2)	// NXT Left Triangle Button
		{
			delayAutoStart--;
			wait1Msec(400);
		}
		//-------------------------------------------------
		// Keep delayAutoStart within a range of valid values
		if (delayAutoStart > 4) delayAutoStart = 4;
		if (delayAutoStart < 0) delayAutoStart = 0;
		switch (delayAutoStart)
		{
		case 0: nxtDisplayCenteredTextLine(6, "0 sec");break;
		case 1: nxtDisplayCenteredTextLine(6, "1 sec");break;
		case 2: nxtDisplayCenteredTextLine(6, "5 sec");break;
		case 3: nxtDisplayCenteredTextLine(6, "10 sec");break;
		case 4: nxtDisplayCenteredTextLine(6, "15 sec");break;
		default: nxtDisplayCenteredTextLine(6, "Error");break;

		}
		//---------------------------------------------------------------
		if(nNxtButtonPressed == 3)	// NXT Orange Button
		{																																				////////////////	You  need to return a variable
			wait1Msec(400);																											////////////////	that selects the auto program
			break;																																////////////////	for later. 			-Jos :)
		}
		//---------------------------------------------------------------
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////
//converts desired height in inches to raise/lower the arm to encoder ticks.
int inchesToTicks(float inches)
{
	int ticks;
	ticks=(int)((inches)*232.91); // 232.91 clicks per inch.
	return ticks;
}

///////////////////////////////////////////////////////////////////////////////////////////////
//This is where you define the heights for the presets.
//Only changes the value of the variable "value" that is given to the function when a button is pressed.
int selectLocation(int value)
{
	int adjustment = 5;
	int heightChange, pegHeightTicks, XcurrentLocal;
	// Define the height for preset arm locations
	float marginHeight = 1.25+1+2-.25;  // (45 deg fitting + 1 inch field uncertainty + 2 inch clearance - starting height of the gripper. Units are inches.
	float pegHeight1 = 17.5+marginHeight+1;  // Peg #1 - Bottom row of pegs.  Units are inches.
	float pegHeight2 = 31.0+marginHeight+1;  // Peg #2 - Middle row of pegs.  Units are inches.
	float pegHeight3 = 43.25+marginHeight+1.5;  // Peg #3 - Top row of pegs.  Units are inches.  Subtracted 4 inches for testing only so that we don't damage the arm.
	float armDriveHeight = 4.0+1; // Height to allow the robot to drive around with up to two rings in the crown
	// Define the arm travel distance to auto score a ring after the arm is in the correct height above a peg
	float autoScoreHeight = 6.0+adjustment; // Lower arm this distance to score a ring
	if (value==1)  // Peg Height #1 (lowest peg)
	{
		pegHeightTicks = inchesToTicks(pegHeight1);
		//nxtDisplayString(6, "btn 2 pressed");
	}

	if (value==2)  // Peg Height #2 (middle peg)
	{
		pegHeightTicks = inchesToTicks(pegHeight2);
		//nxtDisplayString(6, "btn 3 pressed");
	}

	if (value==3) // Peg Height #3 (top peg)
	{
		pegHeightTicks = inchesToTicks(pegHeight3);
		//nxtDisplayString(6, "btn 4 pressed");
	}

	if(value==4)	// Auto score a ring (short distance down arm command)
	{
		heightChange = inchesToTicks(autoScoreHeight);
		XcurrentLocal = nMotorEncoder[elevatorA];
		pegHeightTicks = XcurrentLocal - heightChange;
		//nxtDisplayString(6, "btn 9 pressed");
	}

	if(value==5)	// Height to allow the robot to drive around with up to two rings in the crown
	{
		pegHeightTicks = inchesToTicks(armDriveHeight);
		//nxtDisplayString(6, "btn 9 pressed");
	}

	return pegHeightTicks;
}

///////////////////////////////////////////////////////////////////////////////////////////////
//This is the (P)roportional loop to have the arm go to the desired location
int elevatorGoToHeight(int Xdesired) //Xdesired is in encoder ticks
{
	int XcurrentLocal;
	int errorX;
	int Xspeed;
	float proportionConstant = 0.2; // Proportional constant value.

	// Get current encoder value
	XcurrentLocal = nMotorEncoder[elevatorA];

	while(Xdesired != XcurrentLocal) //only run the loop when you are not at the correct position.
	{
		XcurrentLocal = nMotorEncoder[elevatorA];

		//nxtDisplayTextLine(3, "AutoMode");
		//nxtDisplayTextLine(4, "Xcur %d", XcurrentLocal);
		//nxtDisplayTextLine(5, "Xdes %d", Xdesired);

		//kicks out of the loop when the arm is close enough to the disired spot
		if ((XcurrentLocal < (Xdesired+150)) && (XcurrentLocal > (Xdesired-150)))
		{
			motor[elevatorA] = 0;
			motor[elevatorB] = 0;
			break;
		}

		//calculates how far from desired to determine how fast and what direction to go.
		errorX = Xdesired - XcurrentLocal;
		// Set the motor speed
		Xspeed = (int) (proportionConstant * errorX);

		if (Xspeed > 0)  //set min and max speed limits when going up
		{
			if (Xspeed > 90)
				Xspeed = 90;
			if (Xspeed < 40)
				Xspeed = 40;
		}
		if (Xspeed < 0)  //set the min and max speed limits when going down
		{
			if (Xspeed < -55)
				Xspeed = -55;
			if (Xspeed > -15)
				Xspeed = -15;
		}

		//sets the speed of the motors to go to the position
		motor[elevatorA] = Xspeed;
		motor[elevatorB] = Xspeed;

		//nxtDisplayTextLine(7, "speed %d", Xspeed);

	}

	//turn off motors after leaving the while loop
	motor[elevatorA] = 0;
	motor[elevatorB] = 0;

	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////
//This is the (P)roportional loop to have the arm go to the desired location
bool elevatorGoToHeightFailSafe(int Xdesired, int timeLimit) //Xdesired is in encoder ticks
{
	int XcurrentLocal;
	int errorX;
	int Xspeed;
	float proportionConstant = 0.2; // Proportional constant value.
	bool tripped = false;

	// Get current encoder value
	XcurrentLocal = nMotorEncoder[elevatorA];

	ClearTimer(T1);
	while(Xdesired != XcurrentLocal) //only run the loop when you are not at the correct position.
	{
		tripped = false;

		if(time1[T1] >= timeLimit)
		{
			tripped = true;
			motor[elevatorA] = 0;
			motor[elevatorB] = 0;
			return tripped;
		}

		XcurrentLocal = nMotorEncoder[elevatorA];

		//nxtDisplayTextLine(3, "AutoMode");
		//nxtDisplayTextLine(4, "Xcur %d", XcurrentLocal);
		//nxtDisplayTextLine(5, "Xdes %d", Xdesired);

		//kicks out of the loop when the arm is close enough to the disired spot
		if ((XcurrentLocal < (Xdesired+150)) && (XcurrentLocal > (Xdesired-150)))
		{
			motor[elevatorA] = 0;
			motor[elevatorB] = 0;
			break;
		}

		//calculates how far from desired to determine how fast and what direction to go.
		errorX = Xdesired - XcurrentLocal;
		// Set the motor speed
		Xspeed = (int) (proportionConstant * errorX);

		if (Xspeed > 0)  //set min and max speed limits when going up
		{
			if (Xspeed > 90)
				Xspeed = 90;
			if (Xspeed < 40)
				Xspeed = 40;
		}
		if (Xspeed < 0)  //set the min and max speed limits when going down
		{
			if (Xspeed < -55)
				Xspeed = -55;
			if (Xspeed > -15)
				Xspeed = -15;
		}

		//sets the speed of the motors to go to the position
		motor[elevatorA] = Xspeed;
		motor[elevatorB] = Xspeed;

		//nxtDisplayTextLine(7, "speed %d", Xspeed);
		tripped = false;
	}

	//turn off motors after leaving the while loop
	motor[elevatorA] = 0;
	motor[elevatorB] = 0;

	return tripped;
}

//to have the arm go down an pick up the ring. UNTESTED and not currently used
/*void PickUpRing()
{
//go down until the bottom limit switch or gripper limit switch are pressed. Currently, it goes one speed, but you could make use of the p loop if/when that is working
while (bottomLimit == 0 && ringLimit == 0)
{
motor[elevatorA] = -55;
motor[elevatorB] = -55;
}

motor[elevatorA] = 0;
motor[elevatorB] = 0;

}
*/

// code to look at the limit switches. It might want to be it's own task and have the limit switches be globals so we can shut off the motors easily from anywhere in the code
// Note: swithes are "1" when pressed and "0" when not pressed
void ProcessProto()
{
	ubyte byteInput;
	int B3, B4;
	HTPBsetupIO(HTPB, 0x0);

	byteInput = HTPBreadIO (HTPB, 0x3f); //fetch the state of all the digital IO pins. They default as inputs.

	// the "& 0b__" clears the other values except the one identified in the binary string so the only "1" is for the desired position
	// the ">>" is a bit shift so that when say only the second swithc is switched, then the binary would be like "00010" but with the ">>1" opperator
	// it becomes "0001". this makes it so that the value of each of the variables is a 0 or 1 regardless of what other switches are pressed.

	//B0 input
	bottomLimit = ((byteInput)& 0b001); //currently, this is the bottom limit switch
	//B1 input
	ringLimit = ((byteInput) & 0b010) >> 1; //currently, is the switch fror the gripper
	//B2 input
	ramLimit = ((byteInput) & 0b100)>> 2;
	//B3 input
	B3 = ((byteInput) & 0b1000) >> 3;
	//B4 input
	B4 = ((byteInput) & 0b10000)>> 4;

	//for debugging purpose, to make sure the two switches are working electrically
	//nxtDisplayTextLine(0, "%d", (int) bottomLimit);
	//nxtDisplayTextLine(1, "%d", (int) ringLimit);
	//	wait10Msec(10);
}

//RAM Down
void RAM_down()
{
	motor[RAMright] = 60;
	motor[RAMleft] = 60;
	wait1Msec(500);
	motor[RAMright] = 10;
	motor[RAMleft] = 10;
}

//RAM Up
void RAM_up()
{
	motor[RAMright] = -75;
	motor[RAMleft] = -75;
	wait1Msec(500);
	motor[RAMright] = 0;
	motor[RAMleft] = 0;
}

//////////////////////////////////////////////////////////////////////////////
//////////													TASK MAIN												//////////
//////////////////////////////////////////////////////////////////////////////

task main()
{
	int elevatorHeightTicks;

	// Set the following variables to false to hide the standard NXT LCD information
	bDisplayDiagnostics = false;
	bNxtLCDStatusDisplay = false;

	////////// INITIALIZATIONS //////////
	initializeRobot();	// Execute robot initialization routine

	///////////////  Variables to be used later  /////////////////
	//int maxArmHeightTicks = inchesToTicks(maxArmHeight);
	//int minRingPickupHeightTicks = inchesToTicks(minRingPickupHeight);
	//int WAMservoStep = 3;	//Amount to inc1rement the WAM servo position
	//int WAMservoStep12 = 17;	// Move a servo 15 degrees
	//float maxArmHeight = 45.25;	// Maximum Safe Arm Height used during manual control of the arm
	//float minRingPickupHeight = 8.0;

	// User selected Autonomous information
	{
		selectStartLocation();	// Get the starting location of the robot
		selectAutoActions(); 		// Get Autononmous actions
		switch(AutoActions)
		{
		case 0:	// No Autonomous
			break;
		case 1:	// IR Beacon
			selectRow();  // runs the select row function
			selectAutoStartDelay();	// select the autonomous start delay
			break;
		case 2:	// Left Colmnn
			selectRow();        // runs the select row function
			selectAutoStartDelay();	// select the autonomous start delay
			break;
		case 3:	// Center Colmnn
			selectRow();        // runs the select row function
			selectAutoStartDelay();	// select the autonomous start delay
			break;
		case 4:	// Right Colmnn
			selectRow();        // runs the select row function
			selectAutoStartDelay();	// select the autonomous start delay
			break;
		case 5:	// Play Defense
			selectAutoStartDelay();	// select the autonomous start delay
			break;
		default:	// Not a valid selection
			eraseDisplay();
			nxtDisplayCenteredTextLine(1,"DITU SAYS");
			nxtDisplayCenteredTextLine(2,"INPUT ERROR");
			nxtDisplayCenteredTextLine(4,"TRY AGAIN");
			wait1Msec(6000);
			break;
		}
	}
	eraseDisplay();
	// Set the following variables to false to hide the standard NXT LCD information
	// Show the default FTC Display Information
	bDisplayDiagnostics = true;
	bNxtLCDStatusDisplay = true;
	//bDisplayDiagnostics = false;
	//bNxtLCDStatusDisplay = false;

	// Determine the autonomous start delay based on delayAutoStart
	switch(delayAutoStart)
	{
	case 0:	// delay start = 0 seconds
		delayAutoStartValue = 0;
		break;
	case 1:	// delay start = 1 second
		delayAutoStartValue = 1000;
		break;
	case 2:	// delay start = 5 seconds
		delayAutoStartValue = 5000;
		break;
	case 3:	// delay start = 10 seconds
		delayAutoStartValue = 10000;
		break;
	case 5:	// delay start = 15 seconds
		delayAutoStartValue = 15000;
		break;
	default:	// delay start = 0 seconds
		delayAutoStartValue = 0;
		break;
	}

	// Process robot starting location selection
	switch(robotStartLocation)
	{
	case 0:	// Left Wall
		leftWall = true;
		break;
	case 1:	// Corner
		corner = true;
		break;
	case 2:	// Right Wall
		rightWall = true;
		break;
	default:	// Not a valid starting location
		break;
	}

	// Process robot starting location selection
	switch(Row)
	{
	case 0:	// Bottom Row
		pegHeightCmd = 1;
		break;
	case 1:	// Middle Row
		pegHeightCmd = 2;
		break;
	case 2:	// Top Row
		pegHeightCmd = 3;
		break;
	default:	// Not a valid starting location
		break;
	}

	waitForStart();	// Wait for the signal to start from the Field Control System

	//reads the protoboard to see which switches are pressed. Since they are globals, nothing is returned. We may want to make this it's own task eventually
	ProcessProto();

	switch(AutoActions)
	{
	case 0:	// No Autonomous
		break;
	case 1:	// Score Ring on the column designated by the IR Beacon
		wait1Msec(delayAutoStartValue);

		// INSERT CODE TO IDENTIFY THE COLUMN CONTAINING THE IR BEACON HERE

		elevatorHeightTicks = selectLocation(5);	// Determine the number of encoder ticks to raise the elevator to the drive height
		elevatorGoToHeight(elevatorHeightTicks);	// Raise elevator to the commanded height
		wait1Msec(500);

		// Insert robot move commands here

		// Now that the robot is in the scoring location, raise the elevator to the commanded row
		elevatorHeightTicks = selectLocation(pegHeightCmd);	// Determine the number of encoder ticks based on the commanded elevator height
		elevatorGoToHeight(elevatorHeightTicks);	// Raise elevator to the commanded height to score a ring
		wait1Msec(500);
		elevatorHeightTicks = selectLocation(4);	// Determine the number of encoder ticks to lower the elevator and score the ring
		elevatorGoToHeight(elevatorHeightTicks);	// Score Ring
		break;
	case 2:	// Score Ring on Left Column
		wait1Msec(delayAutoStartValue);
		elevatorHeightTicks = selectLocation(5);	// Determine the number of encoder ticks to raise the elevator to the drive height
		elevatorGoToHeight(elevatorHeightTicks);	// Raise elevator to the commanded height
		wait1Msec(500);

		// Insert robot move commands here

		// Now that the robot is in the scoring location, raise the elevator to the commanded row
		elevatorHeightTicks = selectLocation(pegHeightCmd);	// Determine the number of encoder ticks based on the commanded elevator height
		elevatorGoToHeight(elevatorHeightTicks);	// Raise elevator to the commanded height to score a ring
		wait1Msec(500);
		elevatorHeightTicks = selectLocation(4);	// Determine the number of encoder ticks to lower the elevator and score the ring
		elevatorGoToHeight(elevatorHeightTicks);	// Score Ring
		break;
	case 3:	// Score Ring on Center Column
		wait1Msec(delayAutoStartValue);
		elevatorHeightTicks = selectLocation(5);	// Determine the number of encoder ticks to raise the elevator to the drive height
		tripped = elevatorGoToHeightFailSafe(elevatorHeightTicks, 	10000);	// Raise elevator to the commanded height
		if (tripped) //If the elevator trips the fail safe, break
			break;

		wait1Msec(500);

		move_forward(60-5, 5000, 90, 90);

		/////The alignment from the wall that could work.../////
		//move_forward(24, 4000, 80, 80);
		//turngyro_left(45, 60, 60);
		//move_forward(4, 2000, 80, 80);

		RAM_down();

		// Drive forward until the RAM limit switch is activated or time expires
		ClearTimer(T1);
		while(ramLimit==0 && time1[T1] < 2500)
		{
			// Try to follow the white navigation tape
			if(SensorValue(colorSensorVal) >= 15)
			{
				// The color sensor sees the black platform, command the robot to drift to the right
				motor[Left1] = 35;
				motor[Left2] = 35;
				motor[Right1] = 15;
				motor[Right2] = 15;
			}
			else
			{	// The color sensor sees the white navigation tape, command the robot to drift to the left
				motor[Left1] = 15;
				motor[Left2] = 15;
				motor[Right1] = 35;
				motor[Right2] = 35;
			}
			ProcessProto();
		}
		ClearTimer(T1);
		while(time1[T1] < 200)
		{
			motor[Left1] = -25;
			motor[Left2] = -25;
			motor[Right1] = 25;
			motor[Right2] = 25;
			motor[RAMright] = 95;
			motor[RAMleft] = 95;
		}

		ClearTimer(T1);
		while(time1[T1] < 200)
		{
			motor[Left1] = 20;
			motor[Left2] = 20;
			motor[Right1] = -20;
			motor[Right2] = -20;
			motor[RAMright] = 95;
			motor[RAMleft] = 95;
		}

		ClearTimer(T1);
		while(time1[T1] < 200)
		{
			motor[Left1] = 25;
			motor[Left2] = 25;
			motor[Right1] = 25;
			motor[Right2] = 25;
			motor[RAMright] = 95;
			motor[RAMleft] = 95;
		}

		/*ClearTimer(T1);
		while(time1[T1] < 200)
		{
			motor[Left1] = -20;
			motor[Left2] = -20;
			motor[Right1] = -20;
			motor[Right2] = -20;
			motor[RAMright] = 95;
			motor[RAMleft] = 95;
		}*/

		motor[RAMright] = 0;
		motor[RAMleft] = 0;
		motor[Left1] = 0;
		motor[Left2] = 0;
		motor[Right1] = 0;
		motor[Right2] = 0;

		// Now that the robot is in the scoring location, raise the elevator to the commanded row
		elevatorHeightTicks = selectLocation(pegHeightCmd);	// Determine the number of encoder ticks based on the commanded elevator height
		elevatorGoToHeightFailSafe(elevatorHeightTicks, 7000);	// Raise elevator to the commanded height to score a ring
		if (tripped) //If the elevator trips the fail safe, break
			break;
		wait1Msec(500);
		elevatorHeightTicks = selectLocation(4);	// Determine the number of encoder ticks to lower the elevator and score the ring
		elevatorGoToHeightFailSafe(elevatorHeightTicks, 7000);	// Score Ring
		if (tripped) //If the elevator trips the fail safe, break
			break;
		break;
	case 4:	// Score Ring on Right Column
		wait1Msec(delayAutoStartValue);
		elevatorHeightTicks = selectLocation(5);	// Determine the number of encoder ticks to raise the elevator to the drive height
		elevatorGoToHeight(elevatorHeightTicks);	// Raise elevator to the commanded height
		wait1Msec(500);

		// Insert robot move commands here
		//asdfjkl;

		wait1Msec(500);

		move_forward(12, 5000, 90, 90);
		turngyro_right(45, 100);
		move_forward(20, 7000, 80, 80);
		//RAM_down();

		motor[Right1] = 0;
		motor[Right2] = 0;
		motor[Left1] = 0;
		motor[Left2] = 0;

		/*
		// Drive forward until the RAM limit switch is activated or time expires
		ClearTimer(T1);
		while(ramLimit==0 && time1[T1] < 2500)
		{
			// Try to follow the white navigation tape
			if(SensorValue(colorSensorVal) >= 15)
			{
				// The color sensor sees the black platform, command the robot to drift to the right
				motor[Left1] = 35;
				motor[Left2] = 35;
				motor[Right1] = 15;
				motor[Right2] = 15;
			}
			else
			{	// The color sensor sees the white navigation tape, command the robot to drift to the left
				motor[Left1] = 15;
				motor[Left2] = 15;
				motor[Right1] = 35;
				motor[Right2] = 35;
			}
			ProcessProto();
		}
		ClearTimer(T1);
		while(time1[T1] < 1500)
		{
			motor[Left1] = -25;
			motor[Left2] = -25;
			motor[Right1] = 25;
			motor[Right2] = 25;
			motor[RAMright] = 95;
			motor[RAMleft] = 95;
		}

		ClearTimer(T1);
		while(time1[T1] < 1500)
		{
			motor[Left1] = 20;
			motor[Left2] = 20;
			motor[Right1] = -20;
			motor[Right2] = -20;
			motor[RAMright] = 95;
			motor[RAMleft] = 95;
		}

		ClearTimer(T1);
		while(time1[T1] < 1500)
		{
			motor[Left1] = 25;
			motor[Left2] = 25;
			motor[Right1] = 25;
			motor[Right2] = 25;
			motor[RAMright] = 95;
			motor[RAMleft] = 95;
		}

		/*ClearTimer(T1);
		while(time1[T1] < 200)
		{
			motor[Left1] = -20;
			motor[Left2] = -20;
			motor[Right1] = -20;
			motor[Right2] = -20;
			motor[RAMright] = 95;
			motor[RAMleft] = 95;
		}*/

		/*
		motor[RAMright] = 0;
		motor[RAMleft] = 0;
		motor[Left1] = 0;
		motor[Left2] = 0;
		motor[Right1] = 0;
		motor[Right2] = 0;

		// Now that the robot is in the scoring location, raise the elevator to the commanded row
		elevatorHeightTicks = selectLocation(pegHeightCmd);	// Determine the number of encoder ticks based on the commanded elevator height
		elevatorGoToHeightFailSafe(elevatorHeightTicks, 7000);	// Raise elevator to the commanded height to score a ring
		if (tripped) //If the elevator trips the fail safe, break
			break;
		wait1Msec(500);
		elevatorHeightTicks = selectLocation(4);	// Determine the number of encoder ticks to lower the elevator and score the ring
		elevatorGoToHeightFailSafe(elevatorHeightTicks, 7000);	// Score Ring
		if (tripped) //If the elevator trips the fail safe, break
			break;
		break;
		// Now that the robot is in the scoring location, raise the elevator to the commanded row
		/*elevatorHeightTicks = selectLocation(pegHeightCmd);	// Determine the number of encoder ticks based on the commanded elevator height
		elevatorGoToHeight(elevatorHeightTicks);	// Raise elevator to the commanded height to score a ring
		wait1Msec(500);
		elevatorHeightTicks = selectLocation(4);	// Determine the number of encoder ticks to lower the elevator and score the ring
		elevatorGoToHeight(elevatorHeightTicks);	// Score Ring
		break;*/
	case 5:	// Play Defense
		wait1Msec(delayAutoStartValue);
		elevatorHeightTicks = selectLocation(5);	// Determine the number of encoder ticks to raise the elevator to the drive height
		elevatorGoToHeight(elevatorHeightTicks);	// Raise elevator to the commanded height
		//wait1Msec(500);

		move_backwards(84.0, 10000, 100, 100);

		break;
	default:	// Not a valid autonomous action
		break;
	}

	servo[colorSensorServo] = colorServoDefault;
	wait10Msec(200);
	// All Done, time to stop all tasks
	StopAllTasks();
}	// End Main Bracket
