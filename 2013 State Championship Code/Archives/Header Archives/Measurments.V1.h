///////////////////////////////////////////////
/// File Name: Measurments.V1.h
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

struct Distance
{
	float wheelCircumference;
	int ticksPerRotation;
	int wheelDiameter;
	int init_Angle;
	int tickConst;
	int angle;
	int inch;
	int foot;
	int tick;
	float pi;
}

//Distance Values///////////////////////////////////////
Distance units;
	units.inch = (wheelCircumference/(ticksPerRotation);
	units.wheelCircumference = pi * wheelDiamter;
	units.tick = wheelCircumference/1440;
	units.ticksPerRotation = 1440;
	units.tickConst = 114.591641;
	units.wheelDiameter = 4;
	units.pi = 3.14159265;
	units.foot = 12*inch;

//
float init_Angle = SensorValue[S1];
int Field = (fieldX,fieldY);
int fieldX = 144*inch;
int fieldY = 144*inch;

while(true)
{
float Theta = SensorValue[S1];
}

void move(int direction*, int Xvalue*)
{
	float conversionFactor = 152.7887;
	int Xspeed, Yspeed, errorX, errorY, direction;
	int Xdesired = Xvalue*conversionFactor;
	int Xcurrent = 0;
	//int Ydesired,
	//int Ycurrent;
	float proportionConstant = 3;
	float totalSpeed;
	bool count = true;

	while(count)
	{
		direction*;

		Xcurrent = abs((nMotorEncoder[RightMotor] + nMotorEncoder[LeftMotor])/2)/**conversionFactor*/;

		//calculates distance needed to go
		errorX = Xdesired - Xcurrent;
		//errorY = Ydesired - Ycurrent;

		//sets the speed of the motor
		Xspeed = proportionConstant * direction * (errorX/conversionFactor);
		if(Xspeed > 100)
			Xspeed = 100;
		//Yspeed = proportionConstant * errorY;

		motor[LeftMotor] = Xspeed;
		motor[RightMotor] = Xspeed;

		if(Xdesired <= Xcurrent)
		{
			count = false;
			motor[LeftMotor] = 0;
			motor[RightMotor] = 0;
		}
		else
			count = true;
	}
}

//End of File//
