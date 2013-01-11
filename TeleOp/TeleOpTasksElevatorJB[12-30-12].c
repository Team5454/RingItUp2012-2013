
#include "perfboardTestV1JB.c"

bool resetEncodersAtBeginning(bool resetEncoders)
{
	//resets encoders at the beginning



	if(resetEncoders)
	{
		motor[lifter] = -10;
		if(SensorValue[TouchTop] == 1) //may be wrong sensorvalue output
		{
			motor[lifter] = 0;
			nMotorEncoder[lifter] = 0;
			resetEncoders = false;
		}
	}
}

//values are wrong
int readJoystick(int value)
{
	if (joy2Btn()) // 1 height
		value = (int)1440*18/(2*PI);
	else if (joy2Btn()) // 2 height
		value = (int)1440*32/(2*PI);
	else if (joy2Btn()) // 3 height
		value = (int)1440*46/(2*PI);
	return value;
}

//checks to see if you want to score rings in one or two stage
bool isAuto(bool value)
{
	if (joy2Btn())
		value = true;
	else
		value = false;
	return value;
}

int elevatorGoToHeight(int Xdesired)
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
	Xcurrent = nMotorEncoder[lifter];

	while(((Xcurrent > Xdesired -40) || (Xcurrent < Xdesired +40)) && touchValBottom == 0)
	{
		Xdesired = readJoystick(Xdesired);  //reads controller to see if fickle user wants to go somewhere else instead

		//reads limit switches


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

void dispenseRings()
{
  // not sure how to dispense rings yet
	int value = 130;
	servoTarget[gripper] = value;
	return;
}

task Lifter()
{
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
}
