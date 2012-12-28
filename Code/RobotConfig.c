#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTServo,  none)
#pragma config(Sensor, S2,     IR,             sensorHiTechnicIRSeeker1200)
#pragma config(Sensor, S3,     Gyro,           sensorI2CHiTechnicGyro)
#pragma config(Sensor, S4,     Touch,          sensorTouch)
#pragma config(Motor,  motorA,          stopLeft,      tmotorNXT, openLoop, reversed)
#pragma config(Motor,  motorB,          stopRight,     tmotorNXT, openLoop)
#pragma config(Motor,  motorC,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  mtr_S1_C1_1,     RightMotor,    tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     LeftMotor,     tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     lifter,        tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C2_2,     lifter,        tmotorTetrix, openLoop, encoder)
#pragma config(Servo,  srvo_S1_C3_1,    ramp,                 tServoStandard)
#pragma config(Servo,  srvo_S1_C3_2,    servo2,               tServoStandard)
#pragma config(Servo,  srvo_S1_C3_3,    bamLeft,              tServoStandard)
#pragma config(Servo,  srvo_S1_C3_4,    bamRight,             tServoStandard)
#pragma config(Servo,  srvo_S1_C3_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_6,    servo6,               tServoNone)

void initializeRobot()
{
	// Place code here to initialize servos to starting positions.
	// Sensors are automatically configured and setup by ROBOTC. They may need a brief time to stabilize.
	//the servos are initialzed here so that in the event that a crate is held in autonomous it will not be dropped

	servoTarget[ramp] = 154;
	servoTarget[bamLeft] = 0;
	servoTarget[bamRight] = 254;
	servoChangeRate[ramp] = 5;

	motor[RightMotor] = 0;
	motor[LeftMotor] = 0;

	nMotorEncoder[lifter] = 0;
	nMotorEncoder[LeftMotor] = 0;
	nMotorEncoder[RightMotor] = 0;
	motor[stopLeft] = 50;
	motor[stopRight] = 50;
	wait10Msec(100);
	motor[stopLeft] = 0;
	motor[stopRight] = 0;
	nMotorEncoder[stopLeft] = 0;
	nMotorEncoder[stopRight] = 0;
	return;
}
