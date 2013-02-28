#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  HTServo)
#pragma config(Sensor, S2,     ,               sensorI2CHiTechnicGyro)
#pragma config(Sensor, S4,     HTPB,           sensorI2CCustom9V)
#pragma config(Motor,  motorA,          RAMright,      tmotorNXT, openLoop, encoder)
#pragma config(Motor,  motorB,          RAMleft,       tmotorNXT, openLoop, encoder)
#pragma config(Motor,  motorC,          Ramp,          tmotorNXT, openLoop)
#pragma config(Motor,  mtr_S1_C1_1,     Right1,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,     Right2,        tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C2_1,     Left1,         tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C2_2,     Left2,         tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_1,     elevatorA,     tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C3_2,     elevatorB,     tmotorTetrix, openLoop, reversed)
#pragma config(Servo,  srvo_S1_C4_1,    WAMrightServo,        tServoStandard)
#pragma config(Servo,  srvo_S1_C4_2,    WAMleftServo,         tServoStandard)
#pragma config(Servo,  srvo_S1_C4_3,    colorSensorServo,     tServoStandard)
#pragma config(Servo,  srvo_S1_C4_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//
#include "..\..\Utilities\RobotMoveUtilitiesV2.c"

// Servo Starting Positions.  These values are also used to restrict servo motion during tele-op operation.
int WAMrightServoStart = 140-15;	// WAM Left Servo Start Point
int WAMleftServoStart = 115-12;	// WAM Left Servo Start Point

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
	servoChangeRate[colorSensorServo] = 10;
	// Servo range:  0 - 255
	servo[WAMrightServo] = WAMrightServoStart;	// WAM Right Side
	servo[WAMleftServo] = WAMleftServoStart;	// WAM Left Side
	//servo[RampServo] = 127;			// Ramp
	servo[colorSensorServo] = 157;
	//Initialize Sensors
	HTPBsetupIO(HTPB, 0x0); //initializes the protoboard
	nMotorEncoder[elevatorA] = 0; //resets the elevator encoder

	return;
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
int readJoystick(int value)
{
	int heightChange;
	// Define the height for preset arm locations
	float marginHeight = 1.25+1+2-.25;  // (45 deg fitting + 1 inch field uncertainty + 2 inch clearance - starting height of the gripper. Units are inches.
	float pegHeight1 = 17.5+marginHeight+1;  // Peg #1 - Bottom row of pegs.  Units are inches.
	float pegHeight2 = 31.0+marginHeight+1;  // Peg #2 - Middle row of pegs.  Units are inches.
	float pegHeight3 = 43.25+marginHeight+1.5;  // Peg #3 - Top row of pegs.  Units are inches.  Subtracted 4 inches for testing only so that we don't damage the arm.
	float armDriveHeight = 4.0+1; // Height to allow the robot to drive around with up to two rings in the crown
	// Define the arm travel distance to auto score a ring after the arm is in the correct height above a peg
	float autoScoreHeight = 6.0; // Lower arm this distance to score a ring
	if (joy2Btn(2))  // Peg Height #1 (lowest peg)
	{
		value = inchesToTicks(pegHeight1);
		//nxtDisplayString(6, "btn 2 pressed");
	}

	if (joy2Btn(3))  // Peg Height #2 (middle peg)
	{
		value = inchesToTicks(pegHeight2);
		//nxtDisplayString(6, "btn 3 pressed");
	}

	if (joy2Btn(4)) // Peg Height #3 (top peg)
	{
		value = inchesToTicks(pegHeight3);
		//nxtDisplayString(6, "btn 4 pressed");
	}

	if(joy1Btn(3))	// Auto score a ring (short distance down arm command)
	{
		heightChange = inchesToTicks(autoScoreHeight);
		value = value - heightChange;
		//nxtDisplayString(6, "btn 9 pressed");
	}

	if(joy1Btn(1) || joy2Btn(1))	// Height to allow the robot to drive around with up to two rings in the crown
	{
		value = inchesToTicks(armDriveHeight);
		//nxtDisplayString(6, "btn 9 pressed");
	}

	return value;
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

		//stop button to cancel going to present loctation
		getJoystickSettings(joystick);
		if( joy2Btn(11) == 1 || joy2Btn(12) == 1  )	// Stop elevator motors if either of the joystick buttons are pressed
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
task main()
{
	initializeRobot();	// Execute robot initialization routine

	int Xcurrent, XcurrentTemp, Xdesired;

	float maxArmHeight = 46.75;	// Maximum Safe Arm Height used during manual control of the arm
	int maxArmHeightTicks = inchesToTicks(maxArmHeight);

	float minRingPickupHeight = 8.0;
	int minRingPickupHeightTicks = inchesToTicks(minRingPickupHeight);

	int WAMservoStep = 3;	//Amount to increment the WAM servo position
	int WAMservoStep12 = 17;	// Move a servo 12 degrees

	waitForStart();	// Wait for the signal to start from the Field Control System

	while(true)
	{
		getJoystickSettings(joystick);
		//clears the screen of automatic NXT diagnostic garbage
		//bDisplayDiagnostics = false;
		//bNxtLCDStatusDisplay = false;

		//reads the protoboard to see which switches are pressed. Since they are globals, nothing is returned. We may want to make this it's own task eventually
		ProcessProto();

		//DC motor controls
		//Left drive motor

		// Allow robot to move only when the bottom limit switch is not pressed
		if( bottomLimit !=1 )
		{
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
			} else {
			// The gripper is touching the floor, don't allow the robot to move until the elevator has raised the arm.
			motor[Right1] = 0;
			motor[Right2] = 0;
			motor[Left1] = 0;
			motor[Left2] = 0;

		}

		//reads the protoboard to see which switches are pressed. Since they are globals, nothing is returned. We may want to make this it's own task eventually
		ProcessProto();

		nxtDisplayCenteredTextLine(5, "Bottom Lim %d", bottomLimit);
		nxtDisplayCenteredTextLine(6, "Ring   Lim %d", ringLimit);
		nxtDisplayCenteredTextLine(7, "Ram    Lim %d", ramLimit);

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
				// Stop arm from going higher if it is already at the maximum safe height
				Xcurrent = nMotorEncoder[elevatorA];
				if( Xcurrent < maxArmHeightTicks) {
					motor[elevatorA] = 85;
					motor[elevatorB] = 85;
					} else {
					motor[elevatorA] = 0;
					motor[elevatorB] = 0;
				}
			}
			else if(joy2Btn(7))  // Arm Down
			{
				// Allow arm to move down even if it is holding a ring, but only if it is high enough.
				Xcurrent = nMotorEncoder[elevatorA];
				if( ringLimit != 1 || Xcurrent > minRingPickupHeightTicks) {
					motor[elevatorA] = -55;
					motor[elevatorB] = -55;
					} else {
					motor[elevatorA] = 0;
					motor[elevatorB] = 0;
				}
			}
			else
			{
				motor[elevatorA] = 0;	// Stop Arm Motion
				motor[elevatorB] = 0;
			}

			// uses opportunity to reset the encoder if it is all the way at the bottom to help agianst compiling error
			if (bottomLimit == 1) nMotorEncoder[elevatorA] = 0;
		}

		/////////////////////////////////////////   RAM   /////////////////////////////////////////
		//RAM Down
		if(joy1Btn(2) || joy2Btn(8))
		{
			motor[RAMright] = 60;
			motor[RAMleft] = 60;
			wait1Msec(500);
			motor[RAMright] = 3;
			motor[RAMleft] = 3;
		}

		//RAM Up
		if(joy1Btn(4) || joy2Btn(6))
		{
			motor[RAMright] = -75;
			motor[RAMleft] = -75;
			wait1Msec(500);
			motor[RAMright] = 0;
			motor[RAMleft] = 0;
		}

		/////////////////////////////////////////   WAM   /////////////////////////////////////////

		// Right WAM
		if(joy1Btn(6))  //Button 6 Is Pressed Open Right WAM Arm
		{
			int WAMrightServoCount = ServoValue[WAMrightServo];
			if(WAMrightServoCount <= 255-5 - WAMservoStep) WAMrightServoCount = WAMrightServoCount + WAMservoStep;    //Update servo command if servo position is not near the limit
				servo[WAMrightServo] = WAMrightServoCount;  //Raise Arm
		}
		if(joy1Btn(8))  //Button 8 Is Pressed Close Right WAM Arm to pull rings off of the dispenser
		{
			int WAMrightServoCount = ServoValue[WAMrightServo];
			if(WAMrightServoCount >= WAMrightServoStart + WAMservoStep) WAMrightServoCount = WAMrightServoCount - WAMservoStep;   //Update servo command if servo position is not near the limit
				servo[WAMrightServo] = WAMrightServoCount;  //Lower Arm
		}
		// Left WAM
		if(joy1Btn(5))  //Button 5 Is Pressed Open Left WAM Arm
		{
			//Update servo command if servo position is not near the limit
			int WAMleftServoCount = ServoValue[WAMleftServo];
			if(WAMleftServoCount >=  WAMservoStep){
				WAMleftServoCount = WAMleftServoCount - WAMservoStep;
				servo[WAMleftServo] = WAMleftServoCount;  //Raise Arm
			}
		}

		if(joy1Btn(10))  //Button 5 Is Pressed Open Left and Right WAM Arms an additional 12 degrees
		{
			int WAMrightServoCount = ServoValue[WAMrightServo];
			int WAMleftServoCount = ServoValue[WAMleftServo];
			if(WAMleftServoCount >=  WAMservoStep) WAMleftServoCount = WAMleftServoCount - WAMservoStep12;   //Update servo command if servo position is not near the limit
				if(WAMrightServoCount <= 255-5 - WAMservoStep) WAMrightServoCount = WAMrightServoCount + WAMservoStep12;    //Update servo command if servo position is not near the limit
				servo[WAMrightServo] = WAMrightServoCount;  //Raise Arm
			servo[WAMleftServo] = WAMleftServoCount;  //Raise Arm
			wait1Msec(1000);

		}
		if(joy1Btn(7))  //Button 7 Is Pressed Close Left WAM Arm to pull rings off of the dispenser
		{
			int WAMleftServoCount = ServoValue[WAMleftServo];
			if(WAMleftServoCount <= WAMleftServoStart - WAMservoStep) WAMleftServoCount = WAMleftServoCount + WAMservoStep;
			servo[WAMleftServo] = WAMleftServoCount;  //Lower Arm
		}

		/////////////////////////////////////////   RAMP   /////////////////////////////////////////
		//servo open to release the ramp
		if(joy2Btn(10))
		{
			ClearTimer(T1);
			while(time1[T1] < 500)
			{
				motor[Ramp] = 40;
			}
		}
		else
		{
			motor[Ramp] = 0;
		}

		//servo close if needed to wiggle the string off
		if(joy2Btn(9))
		{
			ClearTimer(T1);
			while(time1[T1] < 500)
			{
				motor[Ramp] = -40;
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////
		// The following code engages the arm control code when needed
		Xcurrent = nMotorEncoder[elevatorA];
		XcurrentTemp = Xcurrent;
		Xdesired = readJoystick(XcurrentTemp); //returns how many encoder counts need to go to if button pressed. If no button pressed, Xdesired stays the same

		if (Xdesired != Xcurrent) //if the user has changed  the Xdesired fromt he current encoder position by pressing the preset button, then it does the p loop function to get to theat height
		{
			elevatorGoToHeight(Xdesired);
		}
	}
}
