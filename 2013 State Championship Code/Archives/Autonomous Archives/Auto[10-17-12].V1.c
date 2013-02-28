///////////////////////////////////////////////
/// File Name: Auto.V1.c
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
int leftMotorSpd = 0;
int rightMotorSpd = 0;

struct mPos
{
	float X;
	float Y;
	float Theta;
}

void Position(float *x, float *y, float *theta)
{

}

void calSpeed(mPos temp)
{
	float errorX = desired.X - current.X;
	float errorY = desired.Y - current.Y;
	float errorTheta = desired.Theta - current.Theta;
	float Sx = Kp(errorX);
	float Sy = Kp(errorY);
	float Stheta = Kp*(errorTheta);
	float St = sqrt((cos*Theta*Sx)+(sin*Theta*Sy));
	int MoutR = St;
	int MoutL = St;
	int motorR = -Stheta;
	int motorL = Stheta;
}

///////////////////////////////////////////////
task main()
{
	mPos current;
		current.X;
		current.Y;
		current.Theta;

	mPos desired;
		desired.X;
		desired.Y;
		desired.Theta;

	int Kp = 1;
//////////////////////////////////////////////
	while(desired != current)
		{
			updateCurrent();
			calSpeed(desired);
			leftMotorSpd;
			rightMotorSpd;
		}
	leftMotorSpd = 0;
	rightMotorSpd = 0;


}

//End of File//
