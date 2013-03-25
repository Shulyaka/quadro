void motor_updateControl(void)
{
  fixed Mt;
  fixed acc;
  quaternion m=imu.q*conjugate(control_q);

  if(abs(m.w)>sinpi4)
  {
    M[0]=orientation_distance_factor*m.w*m.x-orientation_speed_factor*imu.sina;
    M[1]=orientation_distance_factor*m.w*m.y-orientation_speed_factor*imu.sinb;
    M[2]=orientation_distance_factor*m.w*m.z-orientation_speed_factor*imu.sinc;
  }
  else
  {
    if(m.w>0)
    {
      M[0]=orientation_distance_factor_sinpi4*m.x-orientation_speed_factor*imu.sina;
      M[1]=orientation_distance_factor_sinpi4*m.y-orientation_speed_factor*imu.sinb;
      M[2]=orientation_distance_factor_sinpi4*m.z-orientation_speed_factor*imu.sinc;
    }
    else
    {
      M[0]=-orientation_distance_factor_sinpi4*m.x-orientation_speed_factor*imu.sina;
      M[1]=-orientation_distance_factor_sinpi4*m.y-orientation_speed_factor*imu.sinb;
      M[2]=-orientation_distance_factor_sinpi4*m.z-orientation_speed_factor*imu.sinc;
    }
  }

  Mt=-M[0]+M[1]-M[2];
  acc=MotorAcceleration+Mt;
  if(acc<0)
  {
    if(Mt>0)
      acc=one;
    else
      acc=0;
  }
  setMotorSpeed(0, acc);

  Mt=M[0]+M[1]+M[2];
  acc=MotorAcceleration+Mt;
  if(acc<0)
  {
    if(Mt>0)
      acc=one;
    else
      acc=0;
  }
  setMotorSpeed(1, acc);

  Mt=M[0]-M[1]-M[2];
  acc=MotorAcceleration+Mt;
  if(acc<0)
  {
    if(Mt>0)
      acc=one;
    else
      acc=0;
  }
  setMotorSpeed(2, acc);

  Mt=-M[0]-M[1]+M[2];
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
