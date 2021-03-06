#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  HTServo)
#pragma config(Motor,  motorA,          Gripper,       tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  motorB,          BAM,           tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C1_1,     rightDrive,    tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,     leftDrive,     tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_1,     jointOne,      tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     jointTwo,      tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_1,     motorH,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_2,     motorI,        tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S1_C4_1,    servo1,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_2,    servo2,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

task main()
{
  //intialize all motors.
  motor[rightDrive] = 0;
  motor[leftDrive] = 0;
  motor[jointOne] = 0;
  motor[jointTwo] = 0;
  motor[BAM] = 0;
  motor[Gripper] = 0;

  // move left motor forward.
  motor[leftDrive] = 40;
  wait1Msec(2000);

  // move left motor backwards
  motor[leftDrive] = -40;
  wait1Msec(2000);
  motor[leftDrive] = 0;
  wait1Msec(1000);

  //move right motor forward
  motor[rightDrive] = 40;
  wait1Msec(2000);

  //move right motor backwards
  motor[rightDrive] = -40;
  wait1Msec(2000);
  motor[rightDrive] = 0;
  wait1Msec(1000);

  //move jointone up
  motor[jointOne] = 100;
  wait1Msec(2000);

  //move jointone down
  motor[jointOne] = -100;
  wait1Msec(2000);

  //move jointtwo up
  motor[jointTwo] = 100;
  wait1Msec(2000);

  //move jointtwo down
  motor[jointTwo] = -100;
  wait1Msec(2000);

   //move BAM forward
   servoTarget[BAM] = 180;
   wait1Msec(2000);

   //move BAM backwards
   servoTarget[BAM] = -180;
	 wait1Msec(2000);

	 //open gripper
	 motor[Gripper] = 60;
	 wait1Msec(2000);

	 //close gripper
	 motor[Gripper] = -60;
   wait1Msec(2000);
}
