#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  HTServo)
#pragma config(Sensor, S2,     ,               sensorI2CCustom9V)
#pragma config(Sensor, S3,     ,               sensorI2CCustom)
#pragma config(Sensor, S4,     Gyro,           sensorI2CHiTechnicGyro)
#pragma config(Motor,  motorA,           ,             tmotorNXT, openLoop, encoder)
#pragma config(Motor,  motorB,           ,             tmotorNXT, openLoop, encoder)
#pragma config(Motor,  motorC,           ,             tmotorNXT, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C1_1,     LeftMotor,     tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     RightMotor,    tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     motorF,        tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C2_2,     motorG,        tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C3_1,     motorH,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_2,     motorI,        tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S1_C4_1,    servo1,               tServoContinuousRotation)
#pragma config(Servo,  srvo_S1_C4_2,    servo2,               tServoContinuousRotation)
#pragma config(Servo,  srvo_S1_C4_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_6,    servo6,               tServoStandard)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//
#include "Measurments.V2.c"

task main()
{
	initializeRobot();
	move(1, 22.5);
	wait10Msec(500);
	move(-1,22.5);


}
