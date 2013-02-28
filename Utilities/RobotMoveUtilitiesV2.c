//////////////////////////////////////////////////////////////////////////////////////////////////////
///
///Program Name: RobotMoveUtilitiesV2
///Date of Creation: The beginning of time
///Creator: God(CB)
///Version#: 1
///
///Editor of Previous Version: God
///Editor of Current Version : God(Chris)
///
///Issues:
///-None
///-
///
///Currently Functioning:
///-Everything
///-
///
///Changes From Previous:
///-added multiplexor code
///-commented out the HTPBsetupIO on lines 574/581
///- commented out D_I_G_I_T_A_L = data; on line 582
///-commented out mode2 = (int)(data & 0b00000111); on line 584
///-commented out the rest of protoboard funtions
///
////////////////////////////////////////////////////////////////////////////////////////////////////
//use this include statement if you are using the regular programming laptop
//use this include statement if you are using the Coleman laptop
#include "drivers/hitechnic-irseeker-v2.h"
#include "drivers/hitechnic-protoboard.h"
#include "drivers/hitechnic-colour-v2.h"
#include "drivers/hitechnic-sensormux.h"
#include "drivers/hitechnic-eopd.h"
#include "drivers/hitechnic-gyro.h"
#include "JoystickDriver.c"

#define OUR_CPR 1440 // 2880
#define Wheel_Base 17.0
#define Wheel_Size 3.0
#define PI 3.14159
#define CIRCUMFERENCE_3 9.42477  // 2 * PI * 3 in
#define CIRCUMFERENCE_4 12.5664  // 2 * PI * 4 in

//initializes limit swithes as not pressed
int bottomLimit = 0;
int ringLimit = 0;
int ramLimit = 0;
/*
typedef struct
{
	int rightSwitch;
	int leftSwitch;
	int team;
	int mode;
} proto_data;

int D_I_G_I_T_A_L = -1;
int mode2;
proto_data proto;
*/

typedef struct
{
	long rate;
	long total;
	long dt;
	long q_err;
	long zero;
	long bias;
	long dzmin;
	long dzmax;
} gyro_data;

#define GyroSensor S4  // Gyro Sensor Port
#define GyroTimer T4   // Timer used by Gyro processing

gyro_data gyro;        // Gyro data variable
long stempA;
long stempB;
long stempC;

// function with no return value
//that stops all motors
void stop_all_motors()
{
}

// function to compute the number of clicks based on the
// distance to move for our REM bot
int compute_clicks_per_distance(float distance, float wheel_diameter)
{
	// float wheel_circumference;
	int cpr;

	// wheel_circumference = PI * wheel_diameter;
	// Lego NXT motor encoder is in 1 degree increments
	// so the number of encoder clicks per revolution is
	// 360
	cpr = OUR_CPR * (distance / CIRCUMFERENCE_3);
	return cpr;
}

////////////////////////////////////////////////////////
// Stop all TETRIX DC motors
void stop_all_motors()
{
	// Drivetrain Motors
	motor[Right1] = 0;
	motor[Right2] = 0;
	motor[Left1] = 0;
	motor[Left2] = 0;
	// Elevator Motors
	motor[elevatorA] = 0;
	motor[elevatorB] = 0;
	return;
}

////////////////////////////////////////////////////////
// Stop drivetrain TETRIX DC motors
void stop_drivetrain_motors()
{
	// Drivetrain Motors
	motor[Right1] = 0;
	motor[Right2] = 0;
	motor[Left1] = 0;
	motor[Left2] = 0;
	return;
}

////////////////////////////////////////////////////////
// Stop elevator TETRIX DC motors
void stop_elevator_motors()
{
	// Elevator Motors
	motor[elevatorA] = 0;
	motor[elevatorB] = 0;
	return;
}

////////////////////////////////////////////////////////
// function to compute the number of clicks based on the
// distance to move for our robot
int compute_clicks_per_distance(float distance)
{
	int cpr;
	cpr = OUR_CPR*(distance / CIRCUMFERENCE_4);
	return cpr;
}

////////////////////////////////////////////////////////
// function to compute the number of clicks for a
// two wheel turn for our REM bot
// the wheel_base is the distance between the wheels
// instead of the center of rotation being on of the wheels,
// the REM robot will rotate about the center of the wheel_base,
// or 1/2 the wheel base distance
//
// The wheel base is measured from the center of
// the wheel to the center of the other wheel
// Wheel_radius
// Angle to turn
int compute_clicks_for_two_wheelturn(float wheel_base_distance, float angle)
{
	// float wheel_circumference;
	float radial_distance;
	int cpr;

	// wheel_circumference = PI * wheel_diameter;

	// Using half the wheel_base as our radius, we want to turn angle degrees
	// To compute the distance we need to use the equation
	// perimeter = angle * radius
	//   where angle is in radians, and radius in inches
	//   to convert from degrees to radians use
	//     radians = (2 * Pi) / 360 * degrees
	radial_distance = (PI / 180.0) * angle * (wheel_base_distance / 2.0);
	cpr = OUR_CPR * (radial_distance / CIRCUMFERENCE_4);

	return cpr;
}

////////////////////////////////////////////////////////
// Reset the drivetrain encoders
void reset_drivetrain_encoder()
{
	// Initialize the motor encoders
	nMotorEncoder[Right2] = 0;
	nMotorEncoder[Left2] = 0;
	wait1Msec(500);
}

////////////////////////////////////////////////////////
// Reset the elevator encoder
void reset_elevator_encoder()
{
	// Initialize the elevator encoder
	nMotorEncoder[elevatorA] = 0;
	wait1Msec(500);
}

////////////////////////////////////////////////////////
// Move forward making sure encoder values are the same
void nmf(float dist, int tstop, int lspeed, int rspeed)
{
	int clicks_to_move;
	int stval;  // start time value
	int dtval;  // delta time value
	int encdiff;

	// Reset the drivetrain encoders
	reset_drivetrain_encoder();

	// Calculate the number of encoder clicks to use
	clicks_to_move = compute_clicks_per_distance(dist);

	// Start the timer to make sure we dont overshoot
	// stop the motors if we never get to our
	// distance goal so we don't burn out the motors
	//
	time1[T1] = 0;      // Start the timer at 0 msec
	stval = time1[T1];  // get number of msec
	dtval = 0;          // compute delta time

	motor[Left1] = lspeed;
	motor[Left2] = lspeed;
	motor[Right1] = rspeed;
	motor[Right2] = rspeed;
	while ((nMotorEncoder[Left2] < clicks_to_move) && (dtval < tstop))
	{
		encdiff = nMotorEncoder[Right2] - nMotorEncoder[Left2];
		// if rightencoder > leftencoder
		// slow down RightMotor
		if (encdiff > 20)
		{
			motor[Right1] = rspeed - 20;
			motor[Right2] = rspeed - 20;
		}
		else if (encdiff > 10)
		{
			motor[Right1] = rspeed - 10;
			motor[Right2] = rspeed - 10;
		}
		// if rightencoder > leftencoder
		// slow down leftmotor
		else if (encdiff < -20)
		{
			motor[Left1] = lspeed - 20;
			motor[Left2] = lspeed - 20;
		}
		else if (encdiff < -10)
		{
			motor[Left1] = lspeed - 10;
			motor[Left2] = lspeed - 10;
		}
		else
		{
			motor[Left1] = lspeed;
			motor[Left2] = lspeed;
			motor[Right1] = rspeed;
			motor[Right2] = rspeed;
		}

		dtval = time1[T1] - stval;
		wait1Msec(1);
	}
	stop_drivetrain_motors();
	return;
}

////////////////////////////////////////////////////////
// Move robot forward a specified distance
void move_forward(float dist, int tstop, int lspeed, int rspeed)
{
	int clicks_to_move;
	int stval;  // start time value
	int dtval;  // delta time value
	//int encdiff;

	reset_drivetrain_encoder();
	clicks_to_move = compute_clicks_per_distance(dist);

	// Start the timer to make sure we dont overshoot
	// stop the motors if we never get to our
	// distance goal so we don't burn out the motors
	//
	time1[T1] = 0;      // Start the timer at 0 msec
	stval = time1[T1];  // get number of msec
	dtval = 0;          // compute delta time

	motor[Left1] = lspeed;
	motor[Left2] = lspeed;
	motor[Right1] = rspeed;
	motor[Right2] = rspeed;
	while ((nMotorEncoder[Right2] < clicks_to_move) && (dtval < tstop)) {
		dtval = time1[T1] - stval;
		wait1Msec(1);
	}
	stop_drivetrain_motors();
	return;
}

////////////////////////////////////////////////////////
// Turn left using dead reckoning
void turn_left (float rangle, int tstop, int lspeed, int rspeed)
{
	int clicks_to_move;
	int stval;  // start time value
	int dtval;  // delta time value

	reset_drivetrain_encoder();
	// Now lets do a two wheel turn
	clicks_to_move = compute_clicks_for_two_wheelturn(Wheel_Base, rangle);
	// again remember depending on which way to want to
	// turn, enable the correct motor direction
	// Use that motor as the encoder test
	time1[T1] = 0;      // Start the timer at 0 msec
	stval = time1[T1];  // get number of msec
	dtval = 0;          // compute delta time
	motor[Left1] = -lspeed;
	motor[Left2] = -lspeed;
	motor[Right1] =  rspeed;
	motor[Right2] =  rspeed;
	while ((nMotorEncoder[Right2] < clicks_to_move) && (dtval < tstop)) {
		dtval = time1[T1] - stval;
		wait1Msec(1);
	}
	stop_drivetrain_motors();
	return;
}

////////////////////////////////////////////////////////
// Right turn using a gyro
void turngyro_right(float ang, int spd)
{
	ang = ang*-1;
	gyro.total = 0;

	if ((gyro.total/1000.0) > ang)
	{
		motor[Left1] = spd;
		motor[Left2] = spd;
		motor[Right1] = -spd;
		motor[Right2] = -spd;
		while ((gyro.total/1000.0) > ang)
		{
			wait1Msec(1);
		}
		stop_drivetrain_motors();
		return;
	}
	if ((gyro.total/1000.0) < ang)
	{
		motor[Left1] = -spd;
		motor[Left2] = -spd;
		motor[Right1] = spd;
		motor[Right2] = spd;
		while ((gyro.total/1000.0) < ang)
		{
			wait1Msec(1);
		}
		stop_drivetrain_motors();
		return;
	}
}

////////////////////////////////////////////////////////
// Turn left using the gyro and the right drivetrain motors
void turngyro_left_oneWheel(float ang, int spd)
{
	long startangle;
	long currentangle;
	long endangle;

	startangle = gyro.total / 1000;
	currentangle = startangle;
	endangle = startangle + ang;

	if (currentangle > endangle)
	{
		motor[Left1] = 0;
		motor[Left2] = 0;
		motor[Right1] = -spd;
		motor[Right2] = -spd;

		while (currentangle > endangle)
		{
			currentangle = gyro.total / 1000;
			wait1Msec(1);
		}
		stop_drivetrain_motors();
		return;
	}
	if (currentangle < endangle)
	{
		motor[Left1] = 0;
		motor[Left2] = 0;
		motor[Right1] = spd;
		motor[Right2] = spd;
		while (currentangle < endangle)
		{
			currentangle = gyro.total / 1000;
			wait1Msec(1);
		}
		stop_drivetrain_motors();
		return;
	}
}

////////////////////////////////////////////////////////
// Turn right using dead reckoning
void turn_right (float rangle, int tstop, int lspeed, int rspeed)
{
	int clicks_to_move;
	int stval;  // start time value
	int dtval;  // delta time value

	reset_drivetrain_encoder();
	// Now lets do a two wheel turn
	// Turn is 35 degrees
	clicks_to_move = compute_clicks_for_two_wheelturn(Wheel_Base, rangle);
	// again remember depending on which way to want to
	// turn, enable the correct motor direction
	// Use that motor as the encoder test
	time1[T1] = 0;      // Start the timer at 0 msec
	stval = time1[T1];  // get number of msec
	dtval = 0;          // compute delta time
	motor[Left1] = lspeed;
	motor[Left2] = lspeed;
	motor[Right1] = -rspeed;
	motor[Right2] = -rspeed;
	while ((nMotorEncoder[Left2] < clicks_to_move) && (dtval < tstop)) {
		dtval = time1[T1] - stval;
		wait1Msec(1);
	}
	stop_drivetrain_motors();
	return;
}

////////////////////////////////////////////////////////
// Turn left using a gyro
void turngyro_left(float ang, int spd)
{
	gyro.total = 0.0;

	if ((gyro.total/1000.0) < ang)
	{
		motor[Left1] = -spd;
		motor[Left2] = -spd;
		motor[Right1] = spd;
		motor[Right2] = spd;
		while ((gyro.total/1000.0) < ang)
		{
			wait1Msec(1);
		}
		stop_drivetrain_motors();
		return;
	}
	if ((gyro.total/1000.0) > ang)
	{
		motor[Left1] = spd;
		motor[Left2] = spd;
		motor[Right1] = -spd;
		motor[Right2] = -spd;
		while ((gyro.total/1000.0) > ang)
		{
			wait1Msec(1);
		}
		stop_drivetrain_motors();
		return;
	}
}

////////////////////////////////////////////////////////
// Turn right using a gyro and the left drivetrain
void turngyro_right_oneWheel(float ang, int spd)
{
	long startangle;
	long currentangle;
	long endangle;

	ang = ang*-1;
	startangle = gyro.total / 1000;
	currentangle = startangle;
	endangle = startangle + ang;

	if (currentangle < endangle)
	{
		motor[Left1] = -spd;
		motor[Left2] = -spd;
		motor[Right1] = 0;
		motor[Right2] = 0;
		while (currentangle < endangle)
		{
			currentangle = gyro.total / 1000;
			wait1Msec(1);
		}
		stop_drivetrain_motors();
		return;
	}
	if ((gyro.total/1000.0) > endangle)
	{
		motor[Left1] = spd;
		motor[Left2] = spd;
		motor[Right1] = 0;
		motor[Right2] = 0;
		while (currentangle > endangle)
		{
			currentangle = gyro.total / 1000;
			wait1Msec(1);
		}
		stop_drivetrain_motors();
		return;
	}
}

////////////////////////////////////////////////////////
// Move backwards
void move_backwards(float dist, int tstop, int lspeed, int rspeed)
{
	int clicks_to_move;
	int stval;  // start time value
	int dtval;  // delta time value

	reset_drivetrain_encoder();

	// Move to the goal
	// remember going backwards
	clicks_to_move = -compute_clicks_per_distance(dist);
	time1[T1] = 0;      // Start the timer at 0 msec
	stval = time1[T1];  // get number of msec
	dtval = 0;          // compute delta time
	motor[Left1] = -lspeed;
	motor[Left2] = -lspeed;

	motor[Right1] = -rspeed;
	motor[Right2] = -rspeed;
	while ((nMotorEncoder[Right2] > clicks_to_move) && (dtval < tstop)) {
		dtval = time1[T1] - stval;
		wait1Msec(1);
	}
	stop_drivetrain_motors();
	return;
}

////////////////////////////////////////////////////////
// Backup left turn
void back_turn_left(float rangle, int tstop, int lspeed, int rspeed)
{
	int clicks_to_move;
	int stval;  // start time value
	int dtval;  // delta time value

	reset_drivetrain_encoder();

	// Now lets do a two wheel turn
	// remember going backwards
	clicks_to_move = compute_clicks_for_two_wheelturn(Wheel_Base, rangle);
	time1[T1] = 0;      // Start the timer at 0 msec
	stval = time1[T1];  // get number of msec
	dtval = 0;          // compute delta time
	// again remember depending on which way to want to
	// turn, enable the correct motor direction
	motor[Right1] = -rspeed;
	motor[Right2] = -rspeed;
	motor[Left1] = lspeed;
	motor[Left2] = lspeed;
	while ((nMotorEncoder[Left2] < clicks_to_move) && (dtval < tstop)) {
		dtval = time1[T1] - stval;
		wait1Msec(1);
	}
	stop_drivetrain_motors();
	return;
}

////////////////////////////////////////////////////////
// Backup right turn
void back_turn_right (float rangle, int tstop, int lspeed, int rspeed)
{
	int clicks_to_move;
	int stval;  // start time value
	int dtval;  // delta time value

	reset_drivetrain_encoder();

	// Now lets do a two wheel turn
	// remember going backwards
	clicks_to_move = compute_clicks_for_two_wheelturn(Wheel_Base, rangle);
	time1[T1] = 0;      // Start the timer at 0 msec
	stval = time1[T1];  // get number of msec
	dtval = 0;          // compute delta time
	// again remember depending on which way to want to
	// turn, enable the correct motor direction
	motor[Right1] = rspeed;
	motor[Right2] = rspeed;
	motor[Left1] = -lspeed;
	motor[Left2] = -lspeed;
	while ((nMotorEncoder[Right2] < clicks_to_move) && (dtval < tstop)) {
		dtval = time1[T1] - stval;
		wait1Msec(1);
	}
	stop_drivetrain_motors();
	return;
}
////////////////////////////////////////////////////////
// Initialize the gyro
void initialize_gyro()
{
	long stempI;
	long dzmin;
	long dzmax;

	dzmin = 999999;
	dzmax = 0;

	if (SensorType[GyroSensor] != sensorRawValue) {
		SetSensorType(GyroSensor, sensorRawValue);
		wait1Msec(100);
	}

	// Measure gyro bias - 20 samples every 5 msec = 100 msec;
	stempA = 0;
	for (stempI = 1; stempI < 40; stempI++)
	{
		stempB = SensorValue[GyroSensor];
		// compute Deadzone
		if (stempB > dzmax)
			dzmax = stempB;
		if (stempB < dzmin)
			dzmin = stempB;

		stempB -= stempA;
		stempB /= stempI;

		stempA += stempB;
		wait1Msec(5);
	}

	// Deadzone calculation
	// Find differnce between average and min
	// and max, whichever is greater use that
	// difference * 1.25
	// Use the average (stempA) +/- the above
	// for deadband
	if (abs(stempA - dzmax) > abs(stempA - dzmin))
	{
		stempI = abs(stempA - dzmax) * 1.25;
	}
	else
	{
		stempI = abs(stempA - dzmin) * 1.25;
	}
	gyro.dzmin = stempA - stempI;
	gyro.dzmax = stempA + stempI;

	// Initialize gyro parameters
	gyro.total = 0;
	gyro.dt    = 0;
	gyro.q_err = 0;
	gyro.zero  = 0;
	gyro.bias  = stempA * 1000;

	gyro.rate  = SensorValue[GyroSensor];   // calculate the t0 rate so the first
	// Deadzone computation
	if ((gyro.rate > gyro.dzmax) ||
		(gyro.rate < gyro.dzmin))
	{
		gyro.rate *= 1000;                      // average is good
		gyro.rate -= gyro.bias;
	}
	else
	{
		gyro.rate = 0;
	}

	time1[GyroTimer] = 0;                   // Use a timer to calculate dt
}

////////////////////// 2012 Protoboard Code //////////////////////
// Task Process Gyro
task process_gyro
{
	while (true)
	{
		stempA = SensorValue[GyroSensor];
		// Deadzone computation
		if ((stempA > gyro.dzmax) ||
			(stempA < gyro.dzmin))
		{
			// Update the gyro heading
			stempA *= 1000;                      // average is good
			stempA -= gyro.bias;
		}
		else
		{
			stempA = 0;
		}

		gyro.dt = time1[GyroTimer];
		time1[GyroTimer] = 0;

		stempB = gyro.rate;            // retrieve last value
		gyro.rate = stempA;            // save the new value

		// Verify stempA <= stempB
		if (stempB < stempA)
		{
			stempC = stempA;        // swap A <-> B
			stempA = stempB;
			stempB = stempC;
		}

		// Find change in angle by integration (area under curve...
		// well actually the trapezoid since we only have 2 points)
		stempC = stempB - stempA;      // (1/2 (B-A) + A ) * dt
		stempC /= 2;
		stempC += stempA;

		stempC *= gyro.dt;             // * dt (msec)
		stempC += gyro.q_err;          // add back the quantization error from
		stempA = stempC;               // the last calculation
		stempA /= 1000;                // convert msec to sec

		stempB = stempA;               // calculate the remainder
		stempB *= 1000;
		stempC -= stempB;

		gyro.q_err = stempC;           // the remainder is a quantization error that
		// we should keep for the next calculation

		stempB = gyro.total;           // update the integrated angle value
		stempB += stempA;
		gyro.total = stempB;

		stempB /= 1000;                // convert from 1/1000th degree to degrees

		// only process the gyro every 10 msec
		while (time1[GyroTimer] < 10)
		{
			wait1Msec(1);
		}
	}
}

//Move Command//////////////////////////////////////////////////////////////////////////////////////
void move(int direction, float Xvalue, int timeout)
{
	float conversionFactor = 114.588;
	int Xspeed, errorX;
	//iny Yspeed, , errorY;
	int Xdesired = Xvalue*conversionFactor;
	int Xcurrent = 0;
	//int Ydesired,
	//int Ycurrent;
	float proportionConstant = 3;
	//float totalSpeed;
	bool count = true;

	motor[Right1] = 0;
	motor[Right2] = 0;
	motor[Left1] = 0;
	motor[Left2] = 0;

	wait1Msec(50);
	ClearTimer(T1);

	while(count && (time1[T1]<timeout))
	{
		Xcurrent = abs((nMotorEncoder[Right2] + nMotorEncoder[Left2])/2)/**conversionFactor*/;

		//calculates distance needed to go
		errorX = Xdesired - Xcurrent;
		//errorY = Ydesired - Ycurrent;

		//sets the speed of the motor
		Xspeed = proportionConstant * (direction) * (errorX/conversionFactor);
		if(Xspeed > 100)
			Xspeed = 100;
		//
		if(Xspeed < 20)
			Xspeed = 20 * (direction);
		//Yspeed = proportionConstant * errorY;

		motor[Left1] = Xspeed;
		motor[Left2] = Xspeed;
		motor[Right1] = Xspeed;
		motor[Right2] = Xspeed;

		if(Xdesired <= Xcurrent)
		{
			count = false;
			motor[Right1] = 0;
			motor[Right2] = 0;
			motor[Left1] = 0;
			motor[Left2] = 0;
			nMotorEncoder[Right2] = 0;
			nMotorEncoder[Left2] = 0;

		}
		else
			count = true;
	}
	count = false;
	motor[Right1] = 0;
	motor[Right2] = 0;
	motor[Left1] = 0;
	motor[Left2] = 0;
	nMotorEncoder[Right2] = 0;
	nMotorEncoder[Left2] = 0;
}

void initialize_proto()
{
	// The data to be written: 0x3F = 111111 binary,
	// makes all digital ports outputs.
	//HTPBsetupIO(HTPB, 0x0);
}

//task process_proto
//{
//	while (true)
//	{
//		int data = (int) HTPBreadIO  (HTPB, 0xff);
//		D_I_G_I_T_A_L = data;
//		mode2 = (int)(data & 0b00000111);
//		proto.leftSwitch = (int)(data & 0b00010000) >> 4;
//		proto.rightSwitch = (int)((data<<2)>>7);
//		proto.team = (int)((data<<4)>>7) & 1;
//		proto.mode = (int)(data & 0b00000111);
//		wait1Msec(100);

//	}
//}

// code to look at the limit switches. It might want to be it's own task and have the limit switches be globals so we can shut off the motors easily from anywhere in the code
// Note: swithes are "1" when pressed and "0" when not pressed
void ProcessProto()
{
	ubyte byteInput;
	int B3, B4;
	//HTPBsetupIO(HTPB, 0x0);

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


//End of File//
