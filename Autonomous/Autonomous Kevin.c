bool isReady = false;

task main()
{
	initializeRobot();
	initialize_gyro();
	waitForStart();

	//init tasks
	StartTask(process_gyro);
	StartTask(Drive);
	StartTask(Arm);

	while(true)
	{
		wait10Msec(10000);
	}
}

task Drive()
{
	int irSensorVal;

	move(1, 24, 2000); //Move foward out of start
	wait10Msec(50);    //Pause
	irSensorVal = SensorValue[IR]; // Get IR Sensor Value

	nxtDisplayBigTextLine(2, "IR: %d", irSensorVal); //Display IR

	switch(irSensorVal)
	{
	case 1:
		LeftColumn();
		break;
	case 2:
		CenterColumn();
		break;
	case 3:
		RightColumn();
		break;
	}
	isReady = true;
}
task Arm()
{
	WaitForDrive();
	DropArm(0);
}

void WaitForDrive()
{
	while(isReady == false)
	{
		wait10Msec(500);
	}
}

void DropArm(int height)
{

}

void LeftColumn()
{
	turngyro_left(-90.0, 35);
	wait10Msec(100);
	move(1, 45, 4000);
	wait10Msec(100);
	turngyro_left(83.0, 30);
	wait10Msec(100);
	move(1, 10, 1000);
	motor[RightMotor] = 50;
	wait10Msec(400);
	move(-1, 12, 2000);

}
void RightColumn()
{
	move(1, 43, 2000);
	wait10Msec(100);
	turngyro_left(-100.0, 50);
	wait10Msec(100);
	move(1, 10, 1000);
	motor[LeftMotor] = 50;
	wait10Msec(400);
	move(-1, 12, 2000);
}
void CenterColumn()
{
	wait10Msec(100);
	move(-1, 2, 1000);
	turngyro_left(-50.0, 50);
	wait10Msec(50);
}
