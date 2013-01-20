#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  HTServo)
#pragma config(Sensor, S4,     HTPB,           sensorI2CCustom9V)
#pragma config(Motor,  motorA,           ,             tmotorNXT, openLoop, encoder)
#pragma config(Motor,  motorB,           ,             tmotorNXT, openLoop, encoder)
#pragma config(Motor,  motorC,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  mtr_S1_C1_1,     Right1,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,     Right2,        tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C2_1,     Left1,         tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C2_2,     Left2,         tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_1,     elevatorA,     tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C3_2,     elevatorB,     tmotorTetrix, openLoop, reversed)
#pragma config(Servo,  srvo_S1_C4_1,    WAMRightServo,        tServoContinuousRotation)
#pragma config(Servo,  srvo_S1_C4_2,    WAMLeftServo,         tServoContinuousRotation)
#pragma config(Servo,  srvo_S1_C4_3,    RampServo,            tServoStandard)
#pragma config(Servo,  srvo_S1_C4_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "JoystickDriver.c"
#include "C:\Users\ditu\Documents\GitHub\RingItUp2012-2013\Utilities\Robotc Includes\hitechnic-protoboard.h"

/*/////////////////////////////////////////
GENERAL NOTES GOING FORWARD FROM VICTORIA: some things to think about
-since the top limit swithes are not wired yet, I have not tested them, but there is code that I just wrote that should change a global if all of the swithes are pressed.
-code needs to be implimented that utilizes the top limit switch so that the arm will not go up if it is pressed
-code needs to be written to have the arm go down to pick up the rings before it goes the the desired preset height. There is some code I wrote to start from
but it has not been tested at all
-use the PickUpRing function I just wrote as a basis to start from after you get the go to position code working
////////////////////////////////////////////*/

//initializes swithes as not pressed
int bottomLimit = 0;
int ringLimit = 0;

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
	servoChangeRate[servo1] = 10;   //Set how fast servo moves.  ROBOTC default is 10
	servoChangeRate[servo2] = 10;
	// Servo range:  0 - 255
	servo[WAMRightServo] = 127;	// WAM Right Side
	servo[WAMLeftServo] = 127;	// WAM Left Side
	servo[RampServo] = 127;			// Ramp

	//Initialize Sensors
	HTPBsetupIO(HTPB, 0x0); //initializes the protoboard
	nMotorEncoder[elevatorA] = 0; //resets the elevator encoder

	return;
}

///////////////////////////////////////////////////////////////////////////////////////////////
//converts desired height to raise/lower the arm to to encoder ticks.
int inchesToTicks(float inches)
{
	int ticks;
	ticks=(int)((inches)*232.91); // 232.91 clicks per inch.
	return ticks;
}

///////////////////////////////////////////////////////////////////////////////////////////////
//This is where you define the heights for the presets. Currently there is only one preset height for testing, but add other cases for the other levels.
//Only changes the value of the variable "value" that is given to the function when a button is pressed.
int readJoystick(int value)
{
	float marginHeight = 1.25+1+2-.25;  // (45 deg fitting + 1 inch field uncertainty + 2 inch clearance - starting height of the gripper. Units are inches.
	float pegHeight1 = 18.0+marginHeight;  // Peg #1 - Bottom row of pegs.  Units are inches.
	float pegHeight2 = 31.5+marginHeight;  // Peg #2 - Middle row of pegs.  Units are inches.
	float pegHeight3 = 45.0+marginHeight;  // Peg #3 - Top row of pegs.  Units are inches.  Subtracted 4 inches for testing only so that we don't damage the arm.
	if (joy2Btn(1))  // Peg Height #1 (lowest peg)
	{
		value = inchesToTicks(pegHeight1);
		nxtDisplayString(6, "btn 1 pressed");
	}

	if (joy2Btn(2))  // Peg Height #2 (middle peg)
	{
		value = inchesToTicks(pegHeight2);
		nxtDisplayString(6, "btn 2 pressed");
	}

	if (joy2Btn(3)) // Peg Height #3 (top peg)
	{
		value = inchesToTicks(pegHeight3);
		nxtDisplayString(6, "btn 3 pressed");
	}

	return value;
}

///////////////////////////////////////////////////////////////////////////////////////////////
//checks to see if you want to score rings autonomously or not. Greg wanted the option to have the robot go ahead and lower the arm to score the ring for you if you press a buttion
// Not Currently Used
bool isAuto(bool value)
{
	if (joy2Btn(9))
		value = true;
	else
		value = false;
	return value;
}

///////////////////////////////////////////////////////////////////////////////////////////////
//This is the P loop to have the arm go to the desired location
int elevatorGoToHeight(int Xdesired) //Xdesired is in encoder ticks
{
	int XcurrentLocal;
	int errorX;
	int Xspeed;
	float proportionConstant = .2; // Proportional constant value.

	// Get current encoder value
	XcurrentLocal = nMotorEncoder[elevatorA];

	while(Xdesired != XcurrentLocal) //only run the loop when you are not at the correct position.
	{
		XcurrentLocal = nMotorEncoder[elevatorA];
		Xdesired = readJoystick(Xdesired);  //reads controller to see if fickle user wants to change to another preset level. if no button is being pressed, Xdesired stays the same

		nxtDisplayTextLine(3, "AutoMode");
		nxtDisplayTextLine(4, "Xcur %d", XcurrentLocal);
		nxtDisplayTextLine(5, "Xdes %d", Xdesired);

		//kicks out of the loop when you are close enough to the disired spot
		if ((XcurrentLocal < (Xdesired+150)) && (XcurrentLocal > (Xdesired-150)))
		{
			motor[elevatorA] = 0;
			motor[elevatorB] = 0;
			break;
		}

		//calculates how far from desired to determine how fast and what direction to go.
		errorX = Xdesired - XcurrentLocal;

		//////////////////////////////////////////////////////////
		//////////           SETS THE SPEED             //////////
		//////////////////////////////////////////////////////////
		Xspeed = (int) (proportionConstant * errorX);
		//////////////////////////////////////////////////////////

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

		nxtDisplayTextLine(7, "speed %d", Xspeed);

		//stop button to cancel going to present loctation
		if(joy2Btn(8) == 1)
		{
			motor[elevatorA] = 0;
			motor[elevatorB] = 0;
			break;
		}
	}

	//turn off motors after leaving the while loop
	motor[elevatorA] = 0;
	motor[elevatorB] = 0;

	return 0;
}

//to have the arm go down an pick up the ring. UNTESTED!!
void PickUpRing()
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

// code to look at the limit switches. It might want to be it's own task and have the limit switches be globals so we can shut off the motors easily when anywhere in the code
// Note: swithes are "1" when pressed and "0" when not pressed
void ProcessProto()
{
	ubyte byteInput;
	int B2, B3, B4;
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
	B2 = ((byteInput) & 0b100)>> 2;
	//B3 input
	B3 = ((byteInput) & 0b1000) >> 3;
	//B4 input
	B4 = ((byteInput) & 0b10000)>> 4;

	//for debugging purpose, to make sure the two switches are working electrically
	nxtDisplayTextLine(0, "%d", (int) bottomLimit);
	nxtDisplayTextLine(1, "%d", (int) ringLimit);
//	wait10Msec(10);
}

task main()
{
	initializeRobot();	// Execute robot initialization routine

	int Xcurrent, XcurrentTemp, Xdesired;

	waitForStart();

	while(true)
	{
		getJoystickSettings(joystick);
		//clears the screen of automatic NXT diagnostic garbage
		bDisplayDiagnostics = false;
		bNxtLCDStatusDisplay = false;

		//DC motor controls
		//Left drive motor
		if(abs(joystick.joy1_y1) <= 15)   //Joystick deadband
		{
			motor[Left1] = 0;
			motor[Left2] = 0;
		}
		else
		{
			motor[Left1] = joystick.joy1_y1*25/32;  //Assign DriveMtrD (Motor 1) to the Left Y-Axis; scale joystick to motor (100/128)
			motor[Left2] = joystick.joy1_y1*25/32;  //Assign DriveMtrD (Motor 1) to the Left Y-Axis; scale joystick to motor (100/128)

		}
		//Right drive motor
		if(abs(joystick.joy1_y2) <= 15)   //Joystick deadband
		{
			motor[Right1] = 0;
			motor[Right2] = 0;
		}
		else
		{
			motor[Right1] = joystick.joy1_y2*25/32;  //Assign DriveMtrE (Motor 2) to the Right Y-Axis; scale joystick to motor (100/128)
			motor[Right2] = joystick.joy1_y2*25/32;  //Assign DriveMtrE (Motor 2) to the Right Y-Axis; scale joystick to motor (100/128)
		}

		//reads the protoboard to see which switches are pressed. Since they are globals, nothing is returned. We may want to make this it's own task eventually
		ProcessProto();

		//only allows the elevator to go up if the bottom limit switch is pressed
		if(bottomLimit == 1)
		{
			if(joy2Btn(5))
			{
				motor[elevatorA] = 85;
				motor[elevatorB] = 85;
			}
			else
			{
				motor[elevatorA] = 0;
				motor[elevatorB] = 0;
			}

			// uses opportunity to reset the encoder if it is all the way at the bottom to help agianst compiling error
			if (bottomLimit == 1) nMotorEncoder[elevatorA] = 0;

		}
		else //normal control if the bottom switch is not pressed
		{
			if(joy2Btn(5))  //  Arm Up
			{
				motor[elevatorA] = 85;
				motor[elevatorB] = 85;
			}
			else if(joy2Btn(7))  // Arm Down
			{
				motor[elevatorA] = -55;
				motor[elevatorB] = -55;
			}
			else
			{
				motor[elevatorA] = 0;	// Stop Arm Motion
				motor[elevatorB] = 0;
			}

			// uses opportunity to reset the encoder if it is all the way at the bottom to help agianst compiling error
			if (bottomLimit == 1) nMotorEncoder[elevatorA] = 0;
		}

		// Place RAM code here

		// Place WAM code here

		// Place code to auto score a ring here

		// Place code to Pick-up a ring here

		Xcurrent = nMotorEncoder[elevatorA];
		XcurrentTemp = Xcurrent;
		Xdesired = readJoystick(XcurrentTemp); //returns how many encoder counts need to go to if button pressed. If no button pressed, Xdesired stays the same

		if (Xdesired != Xcurrent) //if the user has changed  the Xdesired fromt he current encoder position by pressing the preset button, then it does the p loop function to get to theat height
		{
			//PickUpRing(); // this funciton will use the p loop function to go until either the gripper hits the ground to pick up a singe ring or the limit switch in the arm is pressed to pick up two
			elevatorGoToHeight(Xdesired);

		}
	}
}
