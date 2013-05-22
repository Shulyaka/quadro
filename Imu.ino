void imu_updateOrientation(int alpha, int beta, int gamma)
{
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

//  sasb=imu.sina*imu.sinb;
//  sacb=imu.sina*imu.cosb;
//  casb=imu.cosa*imu.sinb;
//  cacb=imu.cosa*imu.cosb;

//if(ccnt++!=1<<GYROCNTP)
//{
  //imu.angv=quaternion(cacb*imu.cosc-sasb*imu.sinc, sacb*imu.cosc+casb*imu.sinc, casb*imu.cosc-sacb*imu.sinc, sasb*imu.cosc+cacb*imu.sinc)*imu.cqs; //qx*qy*qz
  imu.angv=quaternion(imu.cosa*imu.cosb*imu.cosc, imu.sina*imu.cosb*imu.cosc, imu.sinb*imu.cosa*imu.cosc, imu.sinc*imu.cosa*imu.cosb)*imu.cqs;
//}
//else
//{
//  ccnt=0;
  //imu.angv=quaternion(cacb*imu.cosc-sasb*imu.sinc, sacb*imu.cosc+casb*imu.sinc, casb*imu.cosc-sacb*imu.sinc, sasb*imu.cosc+cacb*imu.sinc)*imu.cql; //same, but with long-term calibration quaternion
//  imu.angv=quaternion(cacb*imu.cosc, imu.sina*(imu.cosb*imu.cosc), imu.sinb*(imu.cosa*imu.cosc), imu.sinc*cacb)*imu.cql;
//}

  imu.qg=imu.qg*imu.angv;
  imu.q=imu.qg*gyro_orientation;

  if(imu.q.w<0)
  {
    imu.qg=-imu.qg;
    imu.q=-imu.q;
  }

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
  quaternion acc=imu.q*quaternion(i+i*i*accel_square[0]+i*accel_gain[0]+accel_offset[0], j+j*j*accel_square[1]+j*accel_gain[1]+accel_offset[1], k+k*k*accel_square[2]+k*accel_gain[2]+accel_offset[2])*conjugate(imu.q);
  imu.ax=acc.x;
  imu.ay=acc.y;
  imu.az=acc.z-gravity;
  /*
  imu.ax=imu.x1*i+imu.x2*j+imu.x3*k;
  imu.ay=imu.y1*i+imu.y2*j+imu.y3*k;
  imu.az=imu.z1*i+imu.z2*j+imu.z3*k+gravity;
  */
  imu.vx=imu.vx+imu.ax*(accel_time<<8);
  imu.vy=imu.vy+imu.ay*(accel_time<<8);
  imu.vz=imu.vz+imu.az*(accel_time<<8);
  imu.x=imu.x+imu.vx*(accel_time<<8);
  imu.y=imu.y+imu.vy*(accel_time<<8);
  imu.z=imu.z+imu.vz*(accel_time<<8);
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
}

void imu_init_orientation(void)
{
//  fixed grav[3];
  fixed arr[3];
  quaternion q1, q2;
  imu.heading=ident;
  while(!accel_ready)
    continue;

//  for(byte i=0; i<3;i++)
//  {
//    grav[i]=(long)accelADC[i]<<18;
//    grav[i]=grav[i]+grav[i]*grav[i]*accel_square[i]+grav[i]*accel_gain[i]+accel_offset[i];
//  }
  Serial.println("Capturing position. Please don't move the quadro");
  accel_capture_wait();
  vectnorm(accel_captured);

  arr[0]=one; // replace with real magneto data;
  arr[1]=0;
  arr[2]=0;
  vectnorm(arr);

  q2=sqrt(quaternion(accel_captured[2], accel_captured[1], -accel_captured[0], 0));  //  accel_captured*(0,0,1)=(accel_captured[1], -accel_captured[0], 0)

  q1=q2*quaternion(arr[0], arr[1], arr[2])*conjugate(q2);

  arr[0]=q1.x;
  arr[1]=q1.y;
  arr[2]=0;
  vectnorm(arr);

  q1=sqrt(quaternion(arr[0], 0, 0, -arr[1]));   //  arr*(1,0,0)=(0, 0, -arr[1])

  disable_sensor_interrupts();
  
  imu.q=q1*q2;
  imu.q.normalize();
  if(imu.q.w<0)
    imu.q=-imu.q;

  imu.qg=imu.q*conjugate(gyro_orientation);
  imu.qg.normalize();
  if(imu.qg.w<0)
    imu.qg=-imu.qg;

  enable_sensor_interrupts();
}

void imu_init_calibrate_orientation(void)
{
    imu.qg=ident;
    imu.cqs=ident;
    imu.cql=ident;
}

void imu_calibrate_orientation(void)
{
  print("qg", imu.qg);
  
  imu.cql=imu.qg;
  
  for(char p=0; p<GYROCNTP; p++)
  {
    imu.qg=sqrt(imu.qg);
    print("qg",imu.qg);
    //print("n",norm(imu.q));
    //print("l",lnorm(imu.q));
  }

  imu.cqs=imu.cqs*conjugate(imu.qg);
  imu.cqs.normalize();
  
  for(char p=0; p<GYROCNTP; p++)
  {
    imu.qg=imu.qg*imu.qg;
    print("qg",imu.qg);
  }

//  print("1",imu.q*conjugate(imu.cql));
  imu.cql=imu.cqs;//*imu.q*conjugate(imu.cql).normalize();   //tbd
  
  imu.qg=ident;
}

quaternion imu_control(quaternion heading)
{
  fixed top[3]={control_ax, control_ay, control_az};
  quaternion result;
  vectnorm(top);
  
  if(heading.x!=0 || heading.y!=0)
  {
    heading.x=0;
    heading.y=0;
    heading.normalize();
  }
  
  result=sqrt(quaternion(top[2], -top[1], top[0], 0)) * heading;   // (0,0,1)*top=(-top[1], top[0], 0)
/*  if(abs(result.w)<sinpi4)
  {
    Serial.println("w<sinpi4==========================================================================================");
    print("control_q", result);
    print("heading", heading);
    print("top[0]", top[0]);
    print("top[1]", top[1]);
    print("top[2]", top[2]);
    print("control_ax", control_ax);
    print("control_ay", control_ay);
    print("control_az", control_az);
    print("vectlen", vectlen(control_ax, control_ay, control_az));
  }
*/
  return result;
}

quaternion imu_get_orientation(void)       // to be called outside of gyro interrupt
{
  quaternion orientation;
  disable_sensor_interrupts();
  orientation=imu.q;
  enable_sensor_interrupts();
  return orientation;
}


