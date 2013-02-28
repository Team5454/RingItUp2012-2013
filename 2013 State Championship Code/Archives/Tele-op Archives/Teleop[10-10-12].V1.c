
///////////////////////////////////////////////
/// File Name: Teleop.V1.c
/// Creator: Chris Beck
/// Last Editor: N/A
///
/// Changes: N/A
///
/// Description:
///		The teleoperated main program
///
///////////////////////////////////////////////

#include JoystickDriver.c

task main()
{

		getJoystickSettings(joystick);
		motor[lUpper] = joystick.joy1_y1;
		motor[lLower] = joystick.joy1_y1;

		motor[rUpper] = joystick.joy1_y2;
		motor[rLower] = joystick.joy1_y2;


		SensorValue[lightsensor]; // not this competition

}
