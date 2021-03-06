
bool speed = false;

task Drive()
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

task Lifter()
{
  int lifterEncoderVal;
  int touchValTop;
  int touchValBottom;
  bool automode;
  int elevatorDirection;
  automode = true; // if automoode is true, elevator moves to preset heights on its own

  while (true)
  {
    //touchValBottom = SensorValue[TouchBottom];
    //touchValTop = SensorValue[TouchTop];

    lifterEncoderVal = nMotorEncoder[lifter];

    nxtDisplayCenteredBigTextLine(5, "%d", lifterEncoderVal);

    //down
    if(joy2Btn(7) && (touchValBottom == 0))
    {
      motor[lifter] = -20;
    }
    //up
    else if(joy2Btn(5) && (touchValTop == 0))
    {
      motor[lifter] = 70;
    }
    //Idle
    else
    {
      motor[lifter] = 0;
    }

    if (automode)
    {
      int Xspeed, errorX, derivX;
      int integralX = 0;
      int Xdesired;
      int Xcurrent = 0;
      int proportionConstant = 1; ///////////////////////////////////////FIGURE THIS OUT
      bool count = true;

      int stval;  // start time value
      int dtval;  // delta time value

      // Start the timer to make sure we dont overshoot
      // stop the motors if we never get to our
      // distance goal so we don't burn out the motors
      //
      time1[T1] = 0;      // Start the timer at 0 msec
      stval = time1[T1];  // get number of msec
      dtval = 0;          // compute delta time

      switch (position) {
      case 0: //go to very bottom (home!)
        Xdesired = -200;
        break;
      case 1:
        Xdesired = (int)1440*18/(2*PI); //bottom rung
        break;
      case 2:
        Xdesired = (int)1440*32/(2*PI); //middle rung
        break;
      case 3:
        Xdesired = (int)1440*46/(2*PI); //top rung
        break;
      }

      Xcurrent = nMotorEncoder[lifter];

      if (Xcurrent > Xdesired)
        elevatorDirection = elevatorDirection * -1;

      while (count && dtval < tstop && automode)
      {
        if (joy2Btn())//put in random button
          automode = false;


        Xcurrent = nMotorEncoder[lifter];
        errorX = Xdesired - Xcurrent;

        Xspeed = proportionConstant * errorX * elevatorDirection;

        if(abs(Xspeed) > 100)
          Xspeed = 100*elevatorDirection;

        motor[lifter] = Xspeed;
        dtval = time1[T1] - stval;
        wait1Msec(1);

        if((Xdesired <= Xcurrent && elevatorDirection == 1) || !automode)
        {
          count = false;
          stop_all_motors();
          automode = true;
        }
        else if (switchPressed || !automode)
        {
          stop_all_motors;
          reset_encoder;
          automode = true;
        }
        else if (elevatorDirection == -1 && touchValBottom == 1)
        {
          stop_all_motors;
          reset_encoder;
          automode = true;
        }
        else if (elevatorDirection == 1 && touchValTop == 1)
        {
          stop_all_motors;
          reset_encoder;
          automode = true;
        }
        else
          count = true;
      }
    }
  }
}

task RAM()
{
  //////Mechanical Stop/////////////////////////////////////////////////////////////////////////
  //Down////
  if(joy2Btn(2))
  {
    nMotorEncoderTarget[RamLeft] = 90;
    nMotorEncoderTarget[RamRight] = 90;
    ClearTimer(T1);
    while ((nMotorEncoder[RamLeft] < 90) && (time1[T1] < 500))
    {
      motor[RamLeft] = -30;
      motor[RamRight] = -30;
    }
  }

  //Up////
  if(joy2Btn(4))
  {
    nMotorEncoderTarget[RamLeft] = 0;
    nMotorEncoderTarget[RamRight] = 0;
    ClearTimer(T1);
    while ((nMotorEncoder[RamLeft] > 0) && (time1[T1] < 500))
    {
      motor[RamLeft] = 30;
      motor[RamRight] = 30;
    }
    motor[RamLeft] = 0;
    motor[RamRight] = 0;
  }
}

task Ramp()
{
  if(joy2Btn(10))
  {
    servoTarget[ramp] = 1;
  }
}
