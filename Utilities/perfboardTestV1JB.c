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
		intInput = ((int)byteInput);
		nxtDisplayTextLine(0, "%d", intInput);
		nxtDisplayTextLine(1, "------------------");

		/*
		proto.B0 = (int)(input & 00010000) >> ;
		proto.B1 = (int)(input & 00100000) >> ;
		proto.B2 = (int)(input & 00100000) >> ;
		proto.B3 = (int)(input & 00100000) >> ;
		proto.B4 = (int)(input & 00100000) >> ;
		*/

		byteInput = (ubyte)intInput;

		B0 = (byteInput);
		B1 = (byteInput << 5) >> 6;
		B2 = (byteInput) >> 2;
		B3 = (byteInput) >> 3;
		B4 = (byteInput) >> 4;


		nxtDisplayTextLine(2, "%d", (int)(B0));
		nxtDisplayTextLine(3, "%d", (int)(B1));
		nxtDisplayTextLine(4, "%d", (int)(B2));
		nxtDisplayTextLine(5, "%d", (int)(B3));
		nxtDisplayTextLine(6, "%d", (int)(B4));

		wait10Msec(10);
	}
}
