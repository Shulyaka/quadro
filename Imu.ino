void imu_updateOrientation(int alpha, int beta, int gamma)
{
//  quaternion qr;
  fixed sasb,sacb,casb,cacb;
//  quaternion qt1, qt2, qt3;
  static unsigned int ccnt=0;

//  alpha=gyroalpha; //uncomment for gyro simulation
//  beta=gyrobeta;
//  gamma=gyrogamma;


  imu.sina=qsin(alpha);
  imu.cosa=qcos(alpha);
  imu.sinb=qsin(beta);
  imu.cosb=qcos(beta);
  imu.sinc=qsin(gamma);
  imu.cosc=qcos(gamma);

  imu.wx=alpha;
  imu.wy=beta;
  imu.wz=gamma;

  sasb=imu.sina*imu.sinb;
  sacb=imu.sina*imu.cosb;
  casb=imu.cosa*imu.sinb;
  cacb=imu.cosa*imu.cosb;
//  qr=quaternion(cacb*imu.cosc-sasb*imu.sinc, sacb*imu.cosc+casb*imu.sinc, casb*imu.cosc-sacb*imu.sinc, sasb*imu.cosc+cacb*imu.sinc);
if(ccnt++!=1<<GYROCNTP)
{
  imu.q=imu.q*quaternion(cacb*imu.cosc-sasb*imu.sinc, sacb*imu.cosc+casb*imu.sinc, casb*imu.cosc-sacb*imu.sinc, sasb*imu.cosc+cacb*imu.sinc)*imu.cqs; //qx*qy*qz
}
else
{
  ccnt=0;
  imu.q=imu.q*quaternion(cacb*imu.cosc-sasb*imu.sinc, sacb*imu.cosc+casb*imu.sinc, casb*imu.cosc-sacb*imu.sinc, sasb*imu.cosc+cacb*imu.sinc)*imu.cql; //same, but with long-term calibration quaternion
//  if(gyro_ready)
//  {
//    print("q", imu.q);
//    imu.q=ident;
//  }
}

//  if(imu.q.w<0)
//    imu.q=-imu.q;

//imu.tmp1=qr.x;
//imu.tmp2=qr.y;
//imu.tmp3=qr.z;

/*qt1=imu.q*quaternion(imu.q.x, imu.q.w, imu.q.z, -imu.q.y);
qt2=imu.q*quaternion(imu.q.y, -imu.q.z, imu.q.w, imu.q.x);
qt3=imu.q*quaternion(imu.q.z, imu.q.y, -imu.q.x, imu.q.w);

imu.x1=qt1.x;
imu.x2=qt1.y;
imu.x3=qt1.z;
imu.y1=qt2.x;
imu.y2=qt2.y;
imu.y3=qt2.z;
imu.z1=qt3.x;
imu.z2=qt3.y;
imu.z3=qt3.z;

imu.tmp1=qt1.w;
imu.tmp2=qt2.w;
imu.tmp3=qt3.w;
*/
}

void imu_updatePosition(fixed i, fixed j, fixed k)
{
  quaternion acc=imu.q*quaternion(i,j,k)*conjugate(imu.q);
  imu.ax=acc.x;
  imu.ay=acc.y;
  imu.az=acc.z-gravity;
  /*
  imu.ax=imu.x1*i+imu.x2*j+imu.x3*k;
  imu.ay=imu.y1*i+imu.y2*j+imu.y3*k;
  imu.az=imu.z1*i+imu.z2*j+imu.z3*k+gravity;
  */
  imu.vx=imu.vx+imu.ax*(accel_time<<9);
  imu.vy=imu.vy+imu.ay*(accel_time<<9);
  imu.vz=imu.vz+imu.az*(accel_time<<9);
  imu.x=imu.x+imu.vx*(accel_time<<9);
  imu.y=imu.y+imu.vy*(accel_time<<9);
  imu.z=imu.z+imu.vz*(accel_time<<9);
}

void imu_init(void)
{
  attachInterrupt(AccelIntNum, dummy_int, RISING);
  attachInterrupt(GyroIntNum, dummy_int, RISING);
  accel_init();
  gyro_init();
}

void imu_init_position(void)
{
  imu.ax=0;
  imu.ay=0;
  imu.az=0;
  imu.vx=0;
  imu.vy=0;
  imu.vz=0;
  imu.x=0;
  imu.y=0;
  imu.z=0;
  imu.azd=gravity;
}

void imu_init_orientation(void)
{
  fixed grav[3];
  fixed nort[3];
  quaternion q1, q2;
  while(!accel_ready)
    continue;

  disable_sensor_interrupts();

  for(byte i=0; i<3;i++)
    grav[i]=(long)accelADC[i]<<18;

  nort[0]=one; // replace with real magneto data;
  nort[1]=0;
  nort[2]=0;

//  q2=quaternion((grav[2]>>1)+(one>>1), grav[1]>>1, -grav[0]>>1, 0);   //  grav*(0,0,1)=(grav[1], -grav[0], 0)
//  q2.normalize();
  q2=sqrt(quaternion(grav[2], grav[1], -grav[0], 0));  //  grav*(0,0,1)=(grav[1], -grav[0], 0)

  q1=q2*quaternion(nort[0], nort[1], nort[2])*conjugate(q2);

  nort[0]=q1.x;
  nort[1]=q1.y;
  nort[2]=0;
  vectnorm(nort);

//  q1=quaternion((nort[0]>>1)+(one>>1), 0, 0, -nort[1]>>1);   //  nort*(1,0,0)=(0, 0, -nort[1]);
//  q1.normalize();
  q1=sqrt(quaternion(nort[0], 0, 0, -nort[1]));   //  nort*(1,0,0)=(0, 0, -nort[1]);
  
  imu.q=q1*q2;
  if(imu.q.w<0)
    imu.q=-imu.q;
  imu.qd=ident;

  enable_sensor_interrupts();
}

void imu_init_calibrate_orientation(void)
{
    imu.q=ident;
    imu.cqs=ident;
    imu.cql=ident;
}

void imu_calibrate_orientation(void)
{
  print("q", imu.q);
  
  imu.cql=imu.q;
  
  for(char p=0; p<GYROCNTP; p++)
  {
    imu.q=sqrt(imu.q);
    print("q",imu.q);
  }

  imu.cqs=imu.cqs*conjugate(imu.q);

  for(char p=0; p<GYROCNTP; p++)
  {
    imu.q=imu.q*imu.q;
    print("q",imu.q);
  }

//  print("1",imu.q*conjugate(imu.cql));
  imu.cql=imu.cqs;//*imu.q*conjugate(imu.cql);   //tbd
  
  imu.q=ident;
}
