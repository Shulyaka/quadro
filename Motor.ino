void motor_updateControl(void)
{
  fixed h=one>>1; //some constants
  fixed k=one>>1;
  fixed k2=k*sinpi4;
  quaternion m=imu.q*conjugate(imu.qd);

  if(abs(m.w)>sinpi4)
  {
    Mx=k*m.w*m.x-h*imu.sina;
    My=k*m.w*m.y-h*imu.sinb;
    Mz=k*m.w*m.z-h*imu.sinc;
  }
  else
  {
    if(m.w>0)
    {
      Mx=k2*m.x-h*imu.sina;
      My=k2*m.y-h*imu.sinb;
      Mz=k2*m.z-h*imu.sinc;
    }
    else
    {
      Mx=-k2*m.x-h*imu.sina;
      My=-k2*m.y-h*imu.sinb;
      Mz=-k2*m.z-h*imu.sinc;
    }
  }

  setMotorSpeed(0, MotorAcceleration);
  setMotorSpeed(1, MotorAcceleration);
  setMotorSpeed(2, MotorAcceleration);
  setMotorSpeed(3, MotorAcceleration);
}

void motor_init(void)
{
  setMotorSpeed(0);
}

void setMotorSpeed(unsigned char number, fixed rpm)
{
  const int baserange=120;
  int v=baserange+128;

  MotorSpeed[number]=rpm;

  if (rpm<-MotorAdjust[number])
  {
    error("Motor rpm < 0");
    rpm=-MotorAdjust[number];
  }
  
  if(rpm!=one)
  {
    rpm=rpm+MotorAdjust[number];
    if(rpm<0)
      if(MotorAdjust[number]<0)
        v=baserange;
      else
        v=baserange+128;
    else
      v=(rpm.value>>24)+baserange; //our range is 120-248
  }
  
  analogWrite(MotorPin[number], v);
}

void setMotorSpeed(fixed rpm)
{
  setMotorSpeed(0, rpm);
  setMotorSpeed(1, rpm);
  setMotorSpeed(2, rpm);
  setMotorSpeed(3, rpm);
}
