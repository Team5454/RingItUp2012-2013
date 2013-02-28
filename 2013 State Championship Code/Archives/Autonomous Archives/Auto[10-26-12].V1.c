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
int Kp = 1;

struct mPos{
	float X;
	float Y;

	float Theta;
}

void Position(float *x, float *y, float *theta){
	float posX = *x;
	float posY = *y;
	float posTheta = *theta;
	mPos *desired;
	desired->X = posX;
	desired->Y = posY;
	desired->Theta = posTheta;
}

void calSpeed(mPos *desired, mPos *current){
	float errorX = desired->X - current->X;
	float errorY = desired->Y - current->Y;
	float errorTheta = desired->Theta - current->Theta;
	float Sx = Kp*(errorX);
	float Sy = Kp*(errorY);
	float Stheta = Kp*(errorTheta);
	float St = sqrt((cos(errorTheta*Sx))+(sin(errorTheta*Sy)));
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

//////////////////////////////////////////////
	while(desired != current)
		{
			updateCurrent();
			calSpeed(desired, current);
			leftMotorSpd = MoutL;
			rightMotorSpd = MoutR;
		}
	leftMotorSpd = 0;
	rightMotorSpd = 0;
}

//End of File//
