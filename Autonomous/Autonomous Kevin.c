bool isReady = false;

#include "AutoConfiguration.c"

task main()
{
	//init tasks
	initializeRobot();
	initialize_gyro();
	waitForStart();

	StartTask(process_gyro);
	StartTask(Drivetrain);
	StartTask(Arm);
}
