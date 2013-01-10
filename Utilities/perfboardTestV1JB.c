#pragma config(Sensor, S1,     HTPB,           sensorI2CCustom9V)

//#include "HTIRS2-driver.h"
//#include "../Configuration/MoveCommands_Protoboard2013.c"

#include "../Utilities/Robotc Includes/hitechnic-protoboard.h"


////////////////////////////////////////////////////////////
//																												//
//		VICTORIA - I couldn't figure it out, but I          //
//		am positive that it is a data conversion error			//
//		I would look into bytes, because it may not					//
//		be a valid data type in robotc...										//
//																				-Joslyn					//
//																												//
////////////////////////////////////////////////////////////

task main()
{
	//StartTask(process_gyro);
	ubyte byteInput;
	int intInput;
	int switchNumber = 5;

	/*
	typedef struct
	{
		int B0;
		int B1;
		int B2;
		int B3;
		int B4;
	} proto_input;
	*/

	int B0, B1, B2, B3, B4;
  HTPBsetupIO(HTPB, 0x0);
	while(true)
	{
		//nxtDisplayTextLine(3, "Switch Pressed");

		// Fetch the state of the digital IO pins.  When not explicitly
		// configured as input or output, they will default to input.
		byteInput = HTPBreadIO(HTPB, 0x3f);
		intInput = ((int)byteInput-32);
		nxtDisplayTextLine(0, "%d", intInput);
		nxtDisplayTextLine(1, "------------------");

		byteInput = (ubyte)intInput;

		B0 = (byteInput) ;
		B1 = (byteInput) >> 1;
		B2 = (byteInput) >> 2;
		B3 = (byteInput) >> 3;
		B4 = (byteInput) >> 4;

		if (B0 == 1)
			nxtDisplayTextLine(2, "button 1 pressed");
		else
			nxtDisplayTextLine(2, "%d", (int)(B0));

		if (B1 == 1)
			nxtDisplayTextLine(3, "button 2 pressed");
		else
			nxtDisplayTextLine(3, "%d", (int)(B1));

		if (B2 == 1)
			nxtDisplayTextLine(4, "button 3 pressed");
		else
			nxtDisplayTextLine(4, "%d", (int)(B2));

		if (B3 == 1)
			nxtDisplayTextLine(5, "button 4 pressed");

		else
			nxtDisplayTextLine(5, "%d", (int)(B3));

		if (B4 == 1)
			nxtDisplayTextLine(6, "button 5 pressed");
		else
			nxtDisplayTextLine(6, "%d", (int)(B0));

		wait10Msec(10);
	}
}
