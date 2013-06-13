const fixed orientation_distance_factor_sinpi4=orientation_distance_factor*sinpi4;
const fixed orientation_distance_factor_z_sinpi4=orientation_distance_factor_z*sinpi4;
#define Mmax (one>>3)

void motor_updateControl(void)
{
  fixed Mt;
  fixed acc;
  quaternion m=conjugate(control_q)*imu.q;
  //quaternion h=imu.q*conjugate(control_heading);
  
/*  fixed hw=imu.q.w*control_heading.w-imu.q.z*control_heading.z;
  fixed hz=imu.q.w*control_heading.z+imu.q.z*control_heading.w;

  if(abs(hw)>sinpi4)
    M[2]=hw*hz+(imu.angv.z<<6);
  else
    if(hw>0)
      M[2]=(sinpi4>>1)*hz+(imu.angv.z<<6);
    else
      M[2]=-(sinpi4>>1)*hz+(imu.angv.z<<6);
*/
//M[2]=0;
  if(abs(m.w)>sinpi4)
  {
/*
    M[0]=orientation_distance_factor*m.w*m.x-orientation_speed_factor*imu.sina;
    M[1]=orientation_distance_factor*m.w*m.y-orientation_speed_factor*imu.sinb;
    M[2]=orientation_distance_factor_z*m.w*m.z-orientation_speed_factor_z*imu.sinc;

    M[0]=orientation_distance_factor*m.w*m.x+(imu.sina<<3);
    M[1]=orientation_distance_factor*m.w*m.y+(imu.sinb<<3);
    M[2]=orientation_distance_factor_z*m.w*m.z+(imu.sinc<<3);
*/

    //m=m.w*m+(imu.angv<<6);
    //M[0]=m.x;
    //M[1]=m.y;
    //M[2]=m.z;

//  if(m.w>0)
//  {
    M[0]=(m.w*m.x>>2)+(imu.angv.x<<4);
    M[1]=(m.w*m.y>>2)+(imu.angv.y<<4);
//  }
//  else
//  {
//    M[0]=-(m.x>>1)+(imu.angv.x<<5);
//    M[1]=-(m.y>>1)+(imu.angv.y<<5);
//  }
  
    
  }
  else
  {
    if(m.w>0)
    {
      M[0]=(sinpi4>>2)*m.x+(imu.angv.x<<4);
      M[1]=(sinpi4>>2)*m.y+(imu.angv.y<<4);
      //M[2]=(sinpi4>>1)*m.z+(imu.angv.z<<5);
    }
    else
    {
      M[0]=-(sinpi4>>2)*m.x+(imu.angv.x<<4);
      M[1]=-(sinpi4>>2)*m.y+(imu.angv.y<<4);
      //M[2]=-(sinpi4>>1)*m.z+(imu.angv.z<<5);
    }
  }
  
  if(M[0]>Mmax)
    M[0]=Mmax;
  if(M[0]<-Mmax)
    M[0]=-Mmax;
  if(M[1]>Mmax)
    M[1]=Mmax;
  if(M[1]<-Mmax)
    M[1]=-Mmax;
  if(M[2]>Mmax)
    M[2]=Mmax;
  if(M[2]<-Mmax)
    M[2]=-Mmax;

  //M[2]=M[2]>>1;

  Mt=-M[0]+M[1]-M[2];
  acc=Throttle+Mt;
  if(acc<0)
  {
    if(Mt>0)
      acc=one;
    else
      acc=0;
  }
  setMotorSpeed(0, acc);

  Mt=M[0]+M[1]+M[2];
  acc=Throttle+Mt;
  if(acc<0)
  {
    if(Mt>0)
      acc=one;
    else
      acc=0;
  }
  setMotorSpeed(1, acc);

  Mt=M[0]-M[1]-M[2];
  acc=Throttle+Mt;
  if(acc<0)
  {
    if(Mt>0)
      acc=one;
    else
      acc=0;
  }
  setMotorSpeed(2, acc);

  Mt=-M[0]-M[1]+M[2];
  acc=Throttle+Mt;
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
  stopAllMotors();
}

void stopAllMotors(void)
{
  const int idleval=110;
  if(gyro_ready)
    disable_sensor_interrupts();
  analogWrite(MotorPin[0], idleval);
  analogWrite(MotorPin[1], idleval);
  analogWrite(MotorPin[2], idleval);
  analogWrite(MotorPin[3], idleval);
  MotorSpeed[0]=0;
  MotorSpeed[1]=0;
  MotorSpeed[2]=0;
  MotorSpeed[3]=0;
  if(gyro_ready)
    enable_sensor_interrupts();
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

/*
  ESC32 settings:
set PWM_MIN_PERIOD 2000
set PWM_MIN_VALUE 797
set PWM_LO_VALUE 956
set PWM_HI_VALUE 1977
set PWM_MIN_START 1037
config write

qt = [ 0.983 ( 2112752357 ),-0.010 (-21478888 ),-0.178 (-383840348 ),-0.001 (-2822913 )]
Nq = 0.999 ( 2147408780 )
qd = [ 0.999 ( 2147391761 ), 0.001 ( 2254252 ),-0.009 (-19737273 ), 0.000 ( 0 )]
mi = [ 0.985 ( 2116167243 ),-0.011 (-23669817 ),-0.169 (-364402901 ),-0.001 (-3423127 )]
Mx =-0.010 (-23308503 )
My =-0.124 (-268435456 )
Mz =-0.001 (-3441042 )
sa =-0.004 (-10614592 )
sb =-0.000 (-1560976 )
sc =-0.000 (-624384 )
ax = 0.000 ( 2108149 )
ay =-0.003 (-6535838 )
az = 0.000 ( 1965250 )
vx = 0.018 ( 39445893 )
vy = 0.001 ( 4220511 )
vz = 0.000 ( 1467807 )
 x = 0.073 ( 158285987 )
 y = 0.013 ( 27947096 )
 z = 0.001 ( 2969692 )
Motor0 = 0.420 ( 903307719 )
Motor1 = 0.395 ( 849801809 )
Motor2 = 0.648 ( 1393495483 )
Motor3 = 0.667 ( 1433382139 )
battery = 2 ( 302 )
25212

*/

