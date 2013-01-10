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
//    Hi Joslyn                                           //
//                                        -Chris          //
////////////////////////////////////////////////////////////

task main()
{
	//StartTask(process_gyro);
	ubyte byteInput;
	int intInput;
	int switchNumber = 5;
	int buttonVal = 0;

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
			buttonVal = 1;
		else
			buttonVal = 0;

		if (B1 == 1)
			buttonVal = 2;
		else
			buttonVal = 0;

		if (B2 == 1)
			buttonVal = 3;
		else
			buttonVal = 0;

		if (B3 == 1)
			buttonVal = 4;
		else
			buttonVal = 0;

		if (B4 == 1)
			buttonVal = 5;
		else
			buttonVal = 0;

		wait10Msec(10);
	}
}
