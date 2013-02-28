void initializeRobot()
{
	// Place code here to initialize servos to starting positions.
	// Sensors are automatically configured and setup by ROBOTC. They may need a brief time to stabilize.
	//the servos are initialzed here so that in the event that a crate is held in autonomous it will not be dropped

	//servoChangeRate[ramp] = 5;

	motor[RightMotor] = 0;
	motor[LeftMotor] = 0;

	nMotorEncoder[elevatorA] = 0;
	nMotorEncoder[LeftMotor] = 0;
	nMotorEncoder[RightMotor] = 0;
	servoTarget[ramp] = 154;


	//motor[stopLeft] = 50;
	//motor[stopRight] = 50;
	//wait10Msec(100);
	//motor[stopLeft] = 0;
	//motor[stopRight] = 0;
	//nMotorEncoder[stopLeft] = 0;
	//nMotorEncoder[stopRight] = 0;

	return;
}
