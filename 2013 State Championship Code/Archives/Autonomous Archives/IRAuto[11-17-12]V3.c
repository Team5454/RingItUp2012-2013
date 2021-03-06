#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTServo,  none)
#pragma config(Sensor, S2,     Light,          sensorLightInactive)
#pragma config(Sensor, S3,     IR,             sensorHiTechnicIRSeeker1200)
#pragma config(Sensor, S4,     Gyro,           sensorI2CHiTechnicGyro)
#pragma config(Motor,  motorA,          finger,        tmotorNXT, openLoop, reversed)
#pragma config(Motor,  motorB,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  motorC,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  mtr_S1_C1_1,     LeftMotor,     tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     RightMotor,    tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     motorF,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     arm,           tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S1_C3_1,    bamLeft,              tServoStandard)
#pragma config(Servo,  srvo_S1_C3_2,    bamRight,             tServoStandard)
#pragma config(Servo,  srvo_S1_C3_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "Measurments.V2.c"
#include "MoveCommandsV1[2012-2013].c"
#include "JoystickDriver.c"

task main()
{
		waitForStart();
	 	initializeRobot();
	 	initialize_gyro();
		StartTask(process_gyro);
	  motor[finger] = 40;

	  int irSensorVal;

	  // move forward, turn 45 degrees, move forward
	  move(1, 24, 2000);
	  wait10Msec(50);
	  move(1, 0, 1000);

		clearTimer(T1);

    //HTIRS2setDSPMode(InfraredSensor, DSP_1200);
    while (time1[T1] < 500)
   	{
   	    irSensorVal = SensorValue[IR];
        nxtDisplayBigTextLine(2, "IR: %d", irSensorVal);
  	}

//////////////////////////////////////////////////////////////////

  switch(irSensorVal)
  {
  	case 1:
  			turngyro_left(-90.0, 35);
  			wait10Msec(100);
  			move(1, 45, 4000);
  			wait10Msec(100);
  			turngyro_left(83.0, 30);
  			wait10Msec(100);
  			//move(1, 0.5, 2000);
  			nMotorEncoder[arm] = 0;
 				wait1Msec(50);

  			while(nMotorEncoder[arm] < 1600)
  				{
 	  				motor[arm] = 50;
  				}
				motor[arm] = 0;
  			wait1Msec(1500);
  			motor[finger] = -40;
  			wait1Msec(750);
  			motor[finger] = 0;
  			while(nMotorEncoder[arm] > 800)
  				{
 	  				motor[arm] = -50;
  				}
				motor[arm] = 0;
				wait1Msec(1500);
				motor[finger] = 0;
  			break;

//------------------------------------------------------------------

  	case 2:
  		wait10Msec(100);
  		move(-1, 2, 1000);
  		turngyro_left(-50.0, 50);
  		wait10Msec(50);
  		ClearTimer(T1);
  		nMotorEncoder[RightMotor] = 0;
  		nMotorEncoder[LeftMotor] = 0;
			move(1, 56, 4000);
    	ClearTimer(T1);
 		  move(-1, 12.5, 2000);
  		nMotorEncoder[arm] = 0;
 			wait1Msec(50);

  		while(nMotorEncoder[arm] < 1600)
  			{
 	  			motor[arm] = 50;
  			}
			motor[arm] = 0;
  		wait1Msec(1500);
  		motor[finger] = -40;
  		wait1Msec(750);
  		motor[finger] = 0;
  			while(nMotorEncoder[arm] > 800)
  				{
 	  				motor[arm] = -50;
  				}
				motor[arm] = 0;
				wait1Msec(1500);
				motor[finger] = 0;
  		break;

//-----------------------------------------------------

  	case 3:
  			move(1, 43, 2000);
  			wait10Msec(100);
  			turngyro_left(-100.0, 50);
  			wait10Msec(100);
  			move(1, 2.5, 1000);
  			nMotorEncoder[arm] = 0;
 				wait1Msec(50);
  			while(nMotorEncoder[arm] < 1500)
  				{
 	  				motor[arm] = 50;
  				}
				motor[arm] = 0;
  			wait1Msec(1500);
  			motor[finger] = -40;
  			wait1Msec(750);
  			motor[finger] = 0;
  			while(nMotorEncoder[arm] > 800)
  				{
 	  				motor[arm] = -50;
  				}
				motor[arm] = 0;
				wait1Msec(1500);
				motor[finger] = 0;
  			break;
  }
}
