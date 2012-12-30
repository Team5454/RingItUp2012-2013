#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTServo,  none)
#pragma config(Sensor, S2,     HTSMUX,         sensorI2CCustom)
#pragma config(Sensor, S3,     Gyro,           sensorI2CHiTechnicGyro)
#pragma config(Sensor, S4,     IR,          sensorTouch)
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

//////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "../Configuration/AutoTasks.c"
#include "../Configuration/RobotConfig.c"

const tMUXSensor Color = msensor_S2_1;
const tMUXSensor TouchSensor = msensor_S2_2;

task main()
{
  //Initialize Tasks
  initializeRobot();
  initialize_gyro();
  selectProgram();
  switch(
  {
  case 0:
    break;
  case 1:
    selectRow();
    break;
  case 2:
    selectRow();
    selectSecondRow();
    break;
  }
  waitForStart();

  StartTask(process_gyro);
  StartTask(Drivetrain);
  StartTask(Arm);
}
