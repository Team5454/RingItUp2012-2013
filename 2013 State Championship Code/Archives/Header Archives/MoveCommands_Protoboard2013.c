//#include "../Utilities/Robotc Includes/hitechnic-protoboard.h"
//#include "JoystickDriver.c"
#define OUR_CPR 2880 // 1440;
#define Wheel_Base 16.0
#define Wheel_Size 4.0
#define PI 3.14159
#define CIRCUMFERENCE_3 9.42477  // 2 * PI * 3 in
#define CIRCUMFERENCE_4 12.5664  // 2 * PI * 4 in

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

///////////////////// 2012 PROTOBOARD CODE ////////////////////
/*
typedef struct
{
	int rightSwitch;
	int leftSwitch;
	int team;
	int mode;
} proto_data;
*/
#define GyroSensor S2  // Gyro Sensor Port
#define GyroTimer T4   // Timer used by Gyro processing

gyro_data gyro;        // Gyro data variable
long stempA;
long stempB;
long stempC;
//int D_I_G_I_T_A_L = -1;
//int mode2;
//proto_data proto;

// function with no return value
// that stops all motors
void stop_all_motors()
{
	motor[Right1] = 0;
	motor[Right2] = 0;
	motor[Left1] = 0;
	motor[Left2] = 0;
	return;
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
	cpr = OUR_CPR*(distance / CIRCUMFERENCE_4);
	return cpr;
}

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
int compute_click_for_two_wheelturn(float wheel_base_distance, float wheel_diameter, float angle)
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

	// Lego NXT motor encoder is in 1 degree increments
	// so the number of encoder clicks per revolution is
	// 360
	cpr = OUR_CPR * (radial_distance / CIRCUMFERENCE_4);

	return cpr;
}

void reset_encoder()
{
	// Initialize the motor encoders
	nMotorEncoder[Right1] = 0;
	nMotorEncoder[Right2] = 0;
	nMotorEncoder[Left1] = 0;
	nMotorEncoder[Left2] = 0;
	wait1Msec(500);
}

// as of 12/18

void testing()
{
	const int ENTER_TEST = 4;
	const int EXIT_TEST = 4;
	//const string ENTER_TEST_SOUND = "Woops.rso";
	//const string EXIT_TEST_SOUND = "Woops.rso";
	//const string LEFT_MOTOR_SOUND = "Woops.rso";
	//const string RIGHT_MOTOR_SOUND = "Woops.rso";
	bool startTesting = false;
	bool done = false;

	for(int i=0;i<10;++i)
	{
		getJoystickSettings(joystick);


		//enter test is 4
		if (joy1Btn(ENTER_TEST))
		{
			startTesting = true;
			break;
		}
		wait10Msec(1);
	}


	if(startTesting)
	{
		// plays Woops when testing starts
		//PlaySoundFile(ENTER_TEST_SOUND);
		nxtDisplayCenteredTextLine(2, "entering test mode");
		wait1Msec(3000);

		// done becomes true when the exit test button is pressed
		while (!done)
		{

			//PlaySoundFile(LEFT_MOTOR_SOUND);
			motor[Left1] = 50;
			motor[Left2] = 50;
			wait10Msec(100);

			//PlaySoundFile(RIGHT_MOTOR_SOUND);
			motor[Right1] = 50;
			motor[Right2] = 50;
			wait10Msec(100);


			if (joy1Btn(EXIT_TEST))
			{
				done = true;

			}

		}
	}
}
// Move forward making sure encoder values are the same
void nmf(float dist, int tstop, int lspeed, int rspeed)
{
	int clicks_to_move;
	int stval;  // start time value
	int dtval;  // delta time value
	int encdiff;

	reset_encoder();
	// Move away from the wall
	clicks_to_move = compute_clicks_per_distance(dist, Wheel_Size);

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
	while ((nMotorEncoder[Left1] < clicks_to_move) && (dtval < tstop))
	{
		encdiff = nMotorEncoder[Right1] - nMotorEncoder[Left1];
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
	stop_all_motors();
}

void move_forward(float dist, int tstop, int lspeed, int rspeed)
{
	int clicks_to_move;
	int stval;  // start time value
	int dtval;  // delta time value
	//int encdiff;

	reset_encoder();
	// Move away from the wall
	clicks_to_move = compute_clicks_per_distance(dist, Wheel_Size);

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
	while ((nMotorEncoder[Right1] < clicks_to_move) && (dtval < tstop)) {
		dtval = time1[T1] - stval;
		wait1Msec(1);
	}
	stop_all_motors();
}

void turn_left (float rangle, int tstop, int lspeed, int rspeed)
{
	int clicks_to_move;
	int stval;  // start time value
	int dtval;  // delta time value

	reset_encoder();
	// Now lets do a two wheel turn
	clicks_to_move = compute_click_for_two_wheelturn(Wheel_Base, Wheel_Size, rangle);
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
	while ((nMotorEncoder[Right1] < clicks_to_move) && (dtval < tstop)) {
		dtval = time1[T1] - stval;
		wait1Msec(1);
	}
	stop_all_motors();
}



void turngyro_right(float ang, int spd)
{
	gyro.total = 0;

	if ((gyro.total/1000.0) > ang)
	{
		motor[Left1] = -spd;
		motor[Left2] = -spd;
		motor[Right1] = spd;
		motor[Right2] = spd;
		while ((gyro.total/1000.0) > ang)
		{
			wait1Msec(1);
		}
		stop_all_motors();
	}
	if ((gyro.total/1000.0) < ang)
	{
		motor[Left1] = spd;
		motor[Left2] = spd;
		motor[Right1] = -spd;
		motor[Right2] = -spd;
		while ((gyro.total/1000.0) < ang)
		{
			wait1Msec(1);
		}
		stop_all_motors();
	}
}

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
		motor[Right1] = spd;
		motor[Right2] = spd;

		while (currentangle > endangle)
		{
			currentangle = gyro.total / 1000;
			wait1Msec(1);
		}
		stop_all_motors();
	}
	if (currentangle < endangle)
	{
		motor[Left1] = 0;
		motor[Left2] = 0;
		motor[Right1] = -spd;
		motor[Right2] = -spd;
		while (currentangle < endangle)
		{
			currentangle = gyro.total / 1000;
			wait1Msec(1);
		}
		stop_all_motors();
	}
}

void turn_right (float rangle, int tstop, int lspeed, int rspeed)
{
	int clicks_to_move;
	int stval;  // start time value
	int dtval;  // delta time value

	reset_encoder();
	// Now lets do a two wheel turn
	// Turn is 35 degrees
	clicks_to_move = compute_click_for_two_wheelturn(Wheel_Base, Wheel_Size, rangle);
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
	while ((nMotorEncoder[Left1] < clicks_to_move) && (dtval < tstop)) {
		dtval = time1[T1] - stval;
		wait1Msec(1);
	}
	stop_all_motors();
}

void turngyro_left(float ang, int spd)
{
	gyro.total = 0.0;

	if ((gyro.total/1000.0) < ang)
	{
		motor[Left1] = spd;
		motor[Left2] = spd;
		motor[Right1] = -spd;
		motor[Right2] = -spd;
		while ((gyro.total/1000.0) < ang)
		{
			wait1Msec(1);
		}
		stop_all_motors();
	}
	if ((gyro.total/1000.0) > ang)
	{
		motor[Left1] = -spd;
		motor[Left2] = -spd;
		motor[Right1] = spd;
		motor[Right2] = spd;
		while ((gyro.total/1000.0) > ang)
		{
			wait1Msec(1);
		}
		stop_all_motors();
	}
}

void turngyro_right_oneWheel(float ang, int spd)
{
	long startangle;
	long currentangle;
	long endangle;

	startangle = gyro.total / 1000;
	currentangle = startangle;
	endangle = startangle + ang;

	if (currentangle < endangle)
	{
		motor[Left1] = spd;
		motor[Left2] = spd;
		motor[Right1] = 0;
		motor[Right2] = 0;
		while (currentangle < endangle)
		{
			currentangle = gyro.total / 1000;
			wait1Msec(1);
		}
		stop_all_motors();
	}
	if ((gyro.total/1000.0) > endangle)
	{
		motor[Left1] = -spd;
		motor[Left2] = -spd;
		motor[Right1] = 0;
		motor[Right2] = 0;
		while (currentangle > endangle)
		{
			currentangle = gyro.total / 1000;
			wait1Msec(1);
		}
		stop_all_motors();
	}
}

void move_backwards(float dist, int tstop, int lspeed, int rspeed)
{
	int clicks_to_move;
	int stval;  // start time value
	int dtval;  // delta time value

	reset_encoder();

	// Move to the goal
	// remember going backwards
	clicks_to_move = -compute_clicks_per_distance(dist, Wheel_Size);
	time1[T1] = 0;      // Start the timer at 0 msec
	stval = time1[T1];  // get number of msec
	dtval = 0;          // compute delta time
	motor[Left1] = -lspeed;
	motor[Left2] = -lspeed;

	motor[Right1] = -rspeed;
	motor[Right2] = -rspeed;
	while ((nMotorEncoder[Left1] > clicks_to_move) && (dtval < tstop)) {
		dtval = time1[T1] - stval;
		wait1Msec(1);
	}
	stop_all_motors();
}

void back_turn_left(float rangle, int tstop, int lspeed, int rspeed)
{
	int clicks_to_move;
	int stval;  // start time value
	int dtval;  // delta time value

	reset_encoder();

	// Now lets do a two wheel turn
	// remember going backwards
	clicks_to_move = compute_click_for_two_wheelturn(Wheel_Base, Wheel_Size, rangle);
	time1[T1] = 0;      // Start the timer at 0 msec
	stval = time1[T1];  // get number of msec
	dtval = 0;          // compute delta time
	// again remember depending on which way to want to
	// turn, enable the correct motor direction
	motor[Right1] = rspeed;
	motor[Right2] = rspeed;
	motor[Left1] = -lspeed;
	motor[Left2] = -lspeed;
	while ((nMotorEncoder[Right1] < clicks_to_move) && (dtval < tstop)) {
		dtval = time1[T1] - stval;
		wait1Msec(1);
	}
	stop_all_motors();
}

void back_turn_right (float rangle, int tstop, int lspeed, int rspeed)
{
	int clicks_to_move;
	int stval;  // start time value
	int dtval;  // delta time value

	reset_encoder();

	// Now lets do a two wheel turn
	// remember going backwards
	clicks_to_move = compute_click_for_two_wheelturn(Wheel_Base, Wheel_Size, rangle);
	time1[T1] = 0;      // Start the timer at 0 msec
	stval = time1[T1];  // get number of msec
	dtval = 0;          // compute delta time
	// again remember depending on which way to want to
	// turn, enable the correct motor direction
	motor[Right1] = -rspeed;
	motor[Right2] = -rspeed;
	motor[Left1] = lspeed;
	motor[Left2] = lspeed;
	while ((nMotorEncoder[Left1] < clicks_to_move) && (dtval < tstop)) {
		dtval = time1[T1] - stval;
		wait1Msec(1);
	}
	stop_all_motors();
}
/*	CONFLICTS
void initializeRobot()
{
	// Place code here to sinitialize servos to starting positions.
	// Sensors are automatically configured and setup by ROBOTC. They may need a brief time to stabilize.
	motor[Right1] = 0;
	motor[Right2] = 0;
	motor[Left1] = 0;
	motor[Left2] = 0;

	return;
}*/

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
/*
void initialize_proto()
{
	// The data to be written: 0x3F = 111111 binary,
	// makes all digital ports outputs.
	HTPBsetupIO(HTPB, 0x0);
}
*/
///////// 2012 PROTOBOARD CODE /////////////
/*
task process_proto
{
	while (true)
	{
		int data = (int) HTPBreadIO  (HTPB, 0xff);
		D_I_G_I_T_A_L = data;
		mode2 = (int)(data & 0b00000111);
		proto.leftSwitch = (int)(data & 0b00010000) >> 4;
		proto.rightSwitch = (int)((data<<2)>>7);
		proto.team = (int)((data<<4)>>7) & 1;
		proto.mode = (int)(data & 0b00000111);
		wait1Msec(100);

	}
}*/
