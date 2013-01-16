void motor_updateControl(void)
{
  fixed h=one>>1; //some constants
  fixed k=one>>1;
  fixed k2=k*sinpi4;
  fixed Mt;
  fixed acc;
  quaternion m=imu.q*conjugate(imu.qd);

  if(abs(m.w)>sinpi4)
  {
    M[0]=k*m.w*m.x-h*imu.sina;
    M[1]=k*m.w*m.y-h*imu.sinb;
    M[2]=k*m.w*m.z-h*imu.sinc;
  }
  else
  {
    if(m.w>0)
    {
      M[0]=k2*m.x-h*imu.sina;
      M[1]=k2*m.y-h*imu.sinb;
      M[2]=k2*m.z-h*imu.sinc;
    }
    else
    {
      M[0]=-k2*m.x-h*imu.sina;
      M[1]=-k2*m.y-h*imu.sinb;
      M[2]=-k2*m.z-h*imu.sinc;
    }
  }

  Mt=-M[0]+M[1]+M[2];
  acc=MotorAcceleration+Mt;
  if(acc<0)
  {
    if(Mt>0)
      acc=one;
    else
      acc=0;
  }
  setMotorSpeed(0, acc);

  Mt=M[0]+M[1]-M[2];
  acc=MotorAcceleration+Mt;
  if(acc<0)
  {
    if(Mt>0)
      acc=one;
    else
      acc=0;
  }
  setMotorSpeed(1, acc);

  Mt=M[0]-M[1]+M[2];
  acc=MotorAcceleration+Mt;
  if(acc<0)
  {
    if(Mt>0)
      acc=one;
    else
      acc=0;
  }
  setMotorSpeed(2, acc);

  Mt=-M[0]-M[1]-M[2];
  acc=MotorAcceleration+Mt;
  if(acc<0)
  {
    if(Mt>0)
      acc=one;
    else
      acc=0;
  }
  setMotorSpeed(3, acc);

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
//    error("Motor rpm < 0");
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
