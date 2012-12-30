bool speed = true;

task Drive()
{
	while(true)
	{
		getJoystickSettings(joystick);
		int jLeft = -(int)joystick.joy1_y1;
		int jRight = (int)joystick.joy1_y2;
		if(joy1Btn(5))
		{
			speed = true;
		}
		//------------------------------
		if(joy1Btn(7))
		{
			speed = false;
		}
		//-------------------------------
		if (speed)
		{
			if (abs(jLeft) < 10)            ///< core out the noise for near zero settings
				motor[LeftMotor] = 0;         ///< sets the left motor to 0% power
			else
				motor[LeftMotor] = jLeft;     ///< set motors to joystick settings
			if (abs(jRight) < 10)           ///< core out the noise for near zero settings
				motor[RightMotor] = 0;        ///< sets the right motor to 0% power
			else
				motor[RightMotor] = jRight;   ///< sets motors to joystick settings
		}
		//--------------------------------
		else
		{
			if (abs(jLeft) < 10)              ///< core out the noise for near zero settings
				motor[LeftMotor] = 0;           ///< sets the left motor to 0% power
			else
				motor[LeftMotor] = (jLeft/3);   ///< set motors to joystick settings
			if (abs(jRight) < 10)             ///< core out the noise for near zero settings
				motor[RightMotor] = 0;          ///< sets the right motor to 0% power
			else
				motor[RightMotor] = (jRight/3); ///< sets motors to joystick settings
		}
	}
}

task Arm()
{
	while(true)
	{
		int touchVal;
		touchVal = SensorValue(Touch);

		int lifterEncoderVal;
		lifterEncoderVal = nMotorEncoder[lifter];

		nxtDisplayCenteredBigTextLine(2, "%d", touchVal);
		nxtDisplayCenteredBigTextLine(5, "%d", lifterEncoderVal);

		//down
		if(joy2Btn(7) && (touchVal == 0))
		{
			motor[lifter] = -20;
		}
		//up
		else if(joy2Btn(5))
		{
			motor[lifter] = 70;
		}
		//Idle
		else
		{
			motor[lifter] = 0;
		}


	}
}

task RAM()
{
	while(true)
	{
		//////Mechanical Stop/////////////////////////////////////////////////////////////////////////
		//Down////
		if(joy2Btn(2))
		{
			nMotorEncoderTarget[stopLeft] = 90;
			nMotorEncoderTarget[stopRight] = 90;
			ClearTimer(T1);
			while ((nMotorEncoder[stopLeft] < 90) && (time1[T1] < 500))
			{
				motor[stopLeft] = -30;
				motor[stopRight] = -30;
			}
		}

		//Up////
		if(joy2Btn(4))
		{
			nMotorEncoderTarget[stopLeft] = 0;
			nMotorEncoderTarget[stopRight] = 0;
			ClearTimer(T1);
			while ((nMotorEncoder[stopLeft] > 0) && (time1[T1] < 500))
			{
				motor[stopLeft] = 30;
				motor[stopRight] = 30;
			}
			motor[stopLeft] = 0;
			motor[stopRight] = 0;
		}
	}
}

task Incline()
{
	while(true)
	{
		if(joy2Btn(10))
		{
			servoTarget[ramp] = 1;
		}
	}
}
