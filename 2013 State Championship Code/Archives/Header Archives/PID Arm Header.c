typedef struct
{
	long KI;
	long KP;
	long KD;
} elevatorPID;

void stop_all_motors()
{
	motor[frontElevator] = 0;
	motor[backElevator] = 0;
	return;
}

void reset_encoder()
{
	// Initialize the motor encoders
	nMotorEncoder[LeftMotor] = 0;
	nMotorEncoder[RightMotor] = 0;
	//wait1Msec(500);
	wait1Msec(1000);

}


void elevatorGoTo(int position, int tstop, bool switchPressed)
{

	int Xspeed, errorX, derivX;
	int integralX = 0;
	int Xdesired;
	int Xcurrent = 0;
	int proportionConstant = 1; ///////////////////////////////////////FIGURE THIS OUT
	bool count = true;

	int stval;  // start time value
	int dtval;  // delta time value

	// Start the timer to make sure we dont overshoot
	// stop the motors if we never get to our
	// distance goal so we don't burn out the motors
	//
	time1[T1] = 0;      // Start the timer at 0 msec
	stval = time1[T1];  // get number of msec
	dtval = 0;          // compute delta time

	switch (position) {
	case 0: //go to very bottom (home!)
		Xdesired = -200;
		break;
	case 1:
		Xdesired = (int)1440*18/(2*PI); //bottom rung
		break;
	case 2:
		Xdesired = (int)1440*32/(2*PI); //middle rung
		break;
	case 3:
		Xdesired = (int)1440*46/(2*PI); //top rung
		break;
	}

	while (count && dtval < tstop)
	{
		Xcurrent = nMotorEncoder[frontElevator];
		errorX = Xdesired - Xcurrent;
		Xspeed = proportionConstant * errorX;

		if(Xspeed > 100)
			Xspeed = 100;

		motor[backElevator] = Xspeed*-1;
		motor[frontElevator] = Xspeed;
		dtval = time1[T1] - stval;
		wait1Msec(1);

		if(Xdesired <= Xcurrent)
		{
			count = false;
			stop_all_motors();
		}
		else if (switchPressed)
		{
			stop_all_motors;
			reset_encoder;
		}
		else
			count = true;
	}

}
