////////////////////////////////////////////////////////////////////////////////////////////////
//=====================================Auto Tasks=============================================//
////////////////////////////////////////////////////////////////////////////////////////////////

//Includes//////////////////////////////////////////////////////////////////////

#include "../Utilities/RobotMoveUtilities.c"
#include "../Utilities/Robotc Includes/hitechnic-sensormux.h"

//Variables/////////////////////////////////////////////////////////////////////
int TouchVal = SensorValue[msensor_S2_2];
bool isReady = false;
bool TouchOn = false;
bool ColorOn = false;
int irSensorVal = 0;
int Row = 0;
int RowTwo = 0;
int Program = 0;

////////////////////////////////////////////////////////////////////////////////
//Select Program--------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////
void selectRow()
{
  eraseDisplay();
  // Enable Top Line NXT Display (Bluetooth, NXT Name & Battery Status)
  bNxtLCDStatusDisplay = true;
  while(true)
  {
    nxtDisplayCenteredTextLine(1,"DITU SAYS WEAR");
    nxtDisplayCenteredTextLine(2,"SAFETY GLASSES");
    nxtDisplayCenteredTextLine(4, "Select Test:");
    //------------------------------------------------
    if(nNxtButtonPressed == 1)
    {
      Program++;
      wait1Msec(400);
    }
    //-------------------------------------------------
    if(nNxtButtonPressed == 2)
    {
      Program--;
      wait1Msec(400);
    }
    //-------------------------------------------------
    switch (Program)
    {
    case 0: nxtDisplayCenteredTextLine(6, "No Auto");break;
    case 1: nxtDisplayCenteredTextLine(6, "One Auto Ring");break;
    case 2: nxtDisplayCenteredTextLine(6, "Two Auto Rings");break;
    case 3: nxtDisplayCenteredTextLine(6, "One Auto W/ Normal Rings");break;
    case 4: nxtDisplayCenteredTextLine(6, "Two Auto W/ Normal Rings");break;
    }
    //---------------------------------------------------------------
    if(nNxtButtonPressed == 3)
    {
      eraseDisplay();
      break;
    }
    //---------------------------------------------------------------
  }
}

////////////////////////////////////////////////////////////////////////////////
//Select First Row--------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////
void selectRow()
{
  eraseDisplay();
  // Enable Top Line NXT Display (Bluetooth, NXT Name & Battery Status)
  bNxtLCDStatusDisplay = true;
  while(true)
  {
    nxtDisplayCenteredTextLine(1,"DITU SAYS WEAR");
    nxtDisplayCenteredTextLine(2,"SAFETY GLASSES");
    nxtDisplayCenteredTextLine(4, "Select Test:");
    //------------------------------------------------
    if(nNxtButtonPressed == 1)
    {
      Row++;
      wait1Msec(400);
    }
    //-------------------------------------------------
    if(nNxtButtonPressed == 2)
    {
      Row--;
      wait1Msec(400);
    }
    //-------------------------------------------------
    switch (Row)
    {
    case 0: nxtDisplayCenteredTextLine(6, "No Auto");break;
    case 1: nxtDisplayCenteredTextLine(6, "Bottom Row");break;
    case 2: nxtDisplayCenteredTextLine(6, "Middle Row");break;
    case 3: nxtDisplayCenteredTextLine(6, "Top Row");break;
    }
    //---------------------------------------------------------------
    if(nNxtButtonPressed == 3)
    {																																				////////////////	You  need to return a variable
      eraseDisplay();																												////////////////	that selects the auto program
      break;																																////////////////	for later. 			-Jos :)
    }
    //---------------------------------------------------------------
  }
}

////////////////////////////////////////////////////////////////////////////////
//Select Second Row-----------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////
void selectSecondRow()
{
  eraseDisplay();
  // Enable Top Line NXT Display (Bluetooth, NXT Name & Battery Status)
  bNxtLCDStatusDisplay = true;
  while(true)
  {
    nxtDisplayCenteredTextLine(1,"DITU SAYS WEAR");
    nxtDisplayCenteredTextLine(2,"SAFETY GLASSES");
    nxtDisplayCenteredTextLine(4, "Select Test:");
    //------------------------------------------------
    if(nNxtButtonPressed == 1)
    {
      RowTwo++;
      wait1Msec(400);
    }
    //-------------------------------------------------
    if(nNxtButtonPressed == 2)
    {
      RowTwo--;
      wait1Msec(400);
    }
    //-------------------------------------------------
    switch (RowTwo)
    {
    case 0: nxtDisplayCenteredTextLine(6, "No 2nd Row");break;
    case 1: nxtDisplayCenteredTextLine(6, "Bottom Row");break;
    case 2: nxtDisplayCenteredTextLine(6, "Middle Row");break;
    case 3: nxtDisplayCenteredTextLine(6, "Top Row");break;
    }
    //---------------------------------------------------------------
    if(nNxtButtonPressed == 3)
    {
      eraseDisplay();
      break;
    }
    //---------------------------------------------------------------
  }
}

////////////////////////////////////////////////////////////////////////////////
//Line Following (Color Sensor)-----------------------------------------------//
////////////////////////////////////////////////////////////////////////////////
void Color_FollowLine()
{
  switch(irSensorVal)
  {
//Case 1///////////////////////////////////////////////////
    case 1;
    while(ColorOn == true && TouchVal == 0)
    {
      while(ColorOn == true && TouchVal == 0)
      {
        if(sensorValue[msensor_S2_1] < 15)
        {
          motor[mtr_S1_C1_2] = 50;
          motor[mtr_S1_C1_1] = 50;
        }
        //------------------------------------------
        if(SensorValue[msensor_S2_1] < 15)
        {
          motor[mtr_S1_C1_2] = -30;
          motor[mtr_S1_C1_1] = 30;
        }
        //------------------------------------------
        if(SensorValue[msensor_S2_1] >= 15)
        {
          motor[mtr_S1_C1_2] = 30;
          motor[mtr_S1_C1_1]= 30;
        }
        //------------------------------------------
        if(SensorValue[msensor_S2_1] < 15)
        {
          motor[mtr_S1_C1_2] = 30;
          motor[mtr_S1_C1_1] = -30;
        }
        //------------------------------------------
        if(SensorValue[msensor_S2_1] >= 15)
        {
          motor[mtr_S1_C1_2] = 30;
          motor[mtr_S1_C1_1]= 30;
        }
      }
      //--------------------------------------------
      ClearTimer(T1);
      while(time1[T1] < 1250)
      {
        motor[mtr_S1_C1_2] = -30;
        motor[mtr_S1_C1_1] = 30;
      }
    }
    break;
//Case 2///////////////////////////////////////////////////
  case 2:
    while(ColorOn == true && TouchVal == 0)
    {
      if(sensorValue[msensor_S2_1] < 15)
      {
        motor[mtr_S1_C1_2] = 50;
        motor[mtr_S1_C1_1] = 50;
      }
      //------------------------------------------
      if(SensorValue[msensor_S2_1] < 15)
      {
        motor[mtr_S1_C1_2] = -30;
        motor[mtr_S1_C1_1] = 30;
      }
      //------------------------------------------
      if(SensorValue[msensor_S2_1] >= 15)
      {
        motor[mtr_S1_C1_2] = 30;
        motor[mtr_S1_C1_1]= 30;
      }
      //------------------------------------------
      if(SensorValue[msensor_S2_1] < 15)
      {
        motor[mtr_S1_C1_2] = 30;
        motor[mtr_S1_C1_1] = -30;
      }
      //------------------------------------------
      if(SensorValue[msensor_S2_1] >= 15)
      {																																	///////////////	This is repeated from above. -Jos
        motor[mtr_S1_C1_2] = 30;
        motor[mtr_S1_C1_1]= 30;
      }
    }
    //--------------------------------------------
    ClearTimer(T1);
    while(time1[T1] < 1250)
    {
      motor[mtr_S1_C1_2] = 30;
      motor[mtr_S1_C1_1] = 30;
    }
    break;
//Case 3///////////////////////////////////////////////////
  case 3:
    while(ColorOn == true && TouchVal == 0)
    {
      if(sensorValue[msensor_S2_1] < 15)
      {
        motor[mtr_S1_C1_2] = 50;
        motor[mtr_S1_C1_1] = 50;
      }
      //------------------------------------------
      if(SensorValue[msensor_S2_1] < 15)
      {
        motor[mtr_S1_C1_2] = -30;
        motor[mtr_S1_C1_1] = 30;
      }
      //------------------------------------------
      if(SensorValue[msensor_S2_1] >= 15)
      {
        motor[mtr_S1_C1_2] = 30;
        motor[mtr_S1_C1_1]= 30;
      }
      //------------------------------------------
      if(SensorValue[msensor_S2_1] < 15)
      {
        motor[mtr_S1_C1_2] = 30;
        motor[mtr_S1_C1_1] = -30;
      }
      //------------------------------------------
      if(SensorValue[msensor_S2_1] >= 15)
      {
        motor[mtr_S1_C1_2] = 30;
        motor[mtr_S1_C1_1]= 30;
      }
    }
    //--------------------------------------------
    ClearTimer(T1);
    while(time1[T1] < 1250)
    {
      motor[mtr_S1_C1_2] = 30;
      motor[mtr_S1_C1_1] = -30;
    }
    break;
  }
}

////////////////////////////////////////////////////////////////////////////////
//Task Stop and Align (Touch Sensor)------------------------------------------//
////////////////////////////////////////////////////////////////////////////////
void Touch_LineUp()
{
  while(TouchOn == true)
  {
    while(TouchVal == 0)
    {
      motor[mtr_S1_C1_1] = 50;
      motor[mtr_S1_C1_2] = 50;
    }
    while(TouchVal == 1)
    {
      motor[mtr_S1_C1_1] = 0;
      motor[mtr_S1_C1_2] = 0;
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
//Wait For Drive--------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////
void WaitForDrive()
{
  while(isReady == false)
  {
    wait1Msec(1000/60);
  }
}

////////////////////////////////////////////////////////////////////////////////
//Rows------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////
void TopRow()
{

}
//-----------------------
void MiddleRow()
{

}
//--------------------------
void BottomRow()
{

}

////////////////////////////////////////////////////////////////////////////////
//Drop Arm--------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////
void DropArm()
{
  switch(Row)
  {
  case 1:
    BottomRow();
    break;
    //-------------------------------------
  case 2:
    MiddleRow();
    break;
    //--------------------------------------
  case 3:
    TopRow();
    break;
  }
}

////////////////////////////////////////////////////////////////////////////////
//Columns---------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////
void LeftColumn()
{
  turngyro(-90.0, 35);
  wait10Msec(100);
  move(1, 35, 4000);
  wait10Msec(100);
  turngyro(45.0, 30);
  wait10Msec(100);
  move(1, 5, 1000);
}
//-----------------------
void RightColumn()
{
  move(1, 30, 2000);
  wait10Msec(100);
  turngyro(-55.0, 50);
  wait10Msec(100);
  move(1, 5, 1000);
}
//--------------------------
void CenterColumn()
{
  wait10Msec(100);
  move(-1, 2, 1000);
  turngyro(-50.0, 50);
  wait10Msec(100);
  move(1, 15, 1000);
}

////////////////////////////////////////////////////////////////////////////////
//Tasks-----------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////

//Task Drivetrain///////////////////////////////////////////////////////////////
task Drivetrain()
{
  while(true)
  {
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
}
//Task Arm/////////////////////////////////////////////////////
task Arm()
{
  while(true)
  {
    WaitForDrive();
    Color_FollowLine();
    DropArm();
  }
}

////////////////////////////////////////////////////////////////////////////////
//Greg's Idea-----------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//Return From Columns---------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////
void Return_LeftColumn()
{
  move(-1, 5, 1000);
  wait10Msec(100);
  turngyro(-45.0, 30);
  wait10Msec(100);
  move(-1, 35, 4000);
  wait10Msec(100);
  turngyro(90.0, 35);
}
//-----------------------
void Return_RightColumn()
{
  move(-1, 5, 1000);
  wait10Msec(100);
  turngyro(55.0, 50);
  wait10Msec(100);
  move(-1, 30, 2000);
}
//--------------------------
void Return_CenterColumn()
{
  move(-1, 15, 1000);
  wait10Msec(100);
  turngyro(50.0, 50);
  wait10Msec(100);
  move(1, 2, 1000);
}
////////////////////////////////////////////////////////////////////////////////
//Grab Rings------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////
void GoToRing()
{
  turngyro(90, 30, 1500);
  wait10Msec();
  move(1, 12, 1500);
  wait10Msec();
}
//-----------------------
void ReturnToStart()
{
  move(-1, 12, 1500);
  wait10Msec();
  turngyro(-90, 30, 1500);
  wait10Msec();
}
//-----------------------
void GrabRing()
{
}

////////////////////////////////////////////////////////////////////////////////
//Collect Other Auto Ring-----------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////
task GrabAutoRing()
{
  goToRing();
  ReturnToStart();
  GrabRing();
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

////////////////////////////////////////////////////////////////////////////////
//Tasks-----------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////
task DoubleScore()
{
  WaitForDrive();

}
