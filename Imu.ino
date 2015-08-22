void imu_updateOrientation(int alpha, int beta, int gamma)
{
  static int a=alpha<<gyrolowpass;
  static int b=beta<<gyrolowpass;
  static int c=gamma<<gyrolowpass;
  
//  quaternion qt1, qt2, qt3;
//  static unsigned int ccnt=0;

//  alpha=gyroalpha; //uncomment for gyro simulation
//  beta=gyrobeta;
//  gamma=gyrogamma;

  /* Low-pass filter start */
  a+=alpha-(a>>gyrolowpass);
  b+=beta-(b>>gyrolowpass);
  c+=gamma-(c>>gyrolowpass);
  
  alpha=a>>gyrolowpass;
  beta=b>>gyrolowpass;
  gamma=c>>gyrolowpass;
  
  gyroalpha=alpha;
  gyrobeta=beta;
  gyrogamma=gamma;
  /* Low-pass filter end */

  imu.sina=qsin(a);//lpha);
  imu.cosa=qcos(a);//lpha);
  imu.sinb=qsin(b);//eta);
  imu.cosb=qcos(b);//eta);
  imu.sinc=qsin(c);//gamma);
  imu.cosc=qcos(c);//gamma);

//if(ccnt++!=1<<GYROCNTP)
//{
  imu.angv=quaternion(imu.cosa*imu.cosb*imu.cosc, imu.sina*imu.cosb*imu.cosc, imu.sinb*imu.cosa*imu.cosc, imu.sinc*imu.cosa*imu.cosb)*imu.cqs;   // The real magic happens here
//}
//else
//{
//  ccnt=0;
//  imu.angv=quaternion(imu.cosa*imu.cosb*imu.cosc, imu.sina*imu.cosb*imu.cosc, imu.sinb*imu.cosa*imu.cosc, imu.sinc*imu.cosa*imu.cosb)*imu.cql;
//}

  if(imu.angv.w<0) //if we got this then we have an overflow. Fixing it.
    imu.angv.w=one;

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

void imu_updatePosition(const fixed acceleration[3])
{
  quaternion acc=imu.q*quaternion(acceleration[0], acceleration[1], acceleration[2])*conjugate(imu.q);
  imu.ax+=((acc.x-imu.ax)>>accellowpass);
  imu.ay+=((acc.y-imu.ay)>>accellowpass);
  imu.az+=((acc.z-gravity-imu.az)>>accellowpass);
  /*
  imu.ax=imu.x1*i+imu.x2*j+imu.x3*k;
  imu.ay=imu.y1*i+imu.y2*j+imu.y3*k;
  imu.az=imu.z1*i+imu.z2*j+imu.z3*k+gravity;
  */
  imu.vx+=imu.ax*fixed(accel_time<<8);
  imu.vy+=imu.ay*fixed(accel_time<<8);
  imu.vz+=imu.az*fixed(accel_time<<8);
  imu.x+=imu.vx*fixed(accel_time<<8);
  imu.y+=imu.vy*fixed(accel_time<<8);
  imu.z+=imu.vz*fixed(accel_time<<8);
}

void imu_init(void)
{
  attachInterrupt(AccelIntNum, dummy_int, RISING);
  attachInterrupt(GyroIntNum, dummy_int, RISING);
  accel_init();
  gyro_init();
  compass_init();
  sonar_init();
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
  while(!gyro_ready || !accel_ready || !compass_ready)
    continue;

//  for(byte i=0; i<3;i++)
//  {
//    grav[i]=(long)accelADC[i]<<18;
//    grav[i]=grav[i]+grav[i]*grav[i]*accel_square[i]+grav[i]*accel_gain[i]+accel_offset[i];
//  }
  Serial.println("Capturing position. Please don't move the quadro");
  accel_capture_wait();
  vectnorm(accel_captured);

  disable_sensor_interrupts();
  compass_measure();
  delay(100);
  compass_measure();
  enable_sensor_interrupts();

  arr[0]=one;
  arr[1]=0;
  arr[2]=0;
/*
  arr[0]=compassADC[0];
  arr[1]=compassADC[1];
  arr[2]=compassADC[2];
  vectnorm(arr);
  print("compass0", arr[0]);
  print("compass1", arr[1]);
  print("compass2", arr[2]);
*/

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
  
  desired_q.w=imu.q.w;
  desired_q.z=imu.q.z;
  desired_q.normalize();

  enable_sensor_interrupts();
}

void imu_init_calibrate_orientation(const int alpha, const int beta, const int gamma)
{
  fixed sina=qsin(alpha<<gyrolowpass);
  fixed cosa=qcos(alpha<<gyrolowpass);
  fixed sinb=qsin(beta<<gyrolowpass);
  fixed cosb=qcos(beta<<gyrolowpass);
  fixed sinc=qsin(gamma<<gyrolowpass);
  fixed cosc=qcos(gamma<<gyrolowpass);
  imu.qg=ident;
  imu.cqs=quaternion(cosa*cosb*cosc, -sina*cosb*cosc, -sinb*cosa*cosc, -sinc*cosa*cosb);
  imu.cql=imu.cqs;
}

void imu_calibrate_orientation(void)
{
//  print("qg", imu.qg);
  
  imu.cql=imu.qg;
  
  for(byte p=0; p<GYROCNTP; p++)
  {
    imu.qg=sqrt(imu.qg);
    print("qg",imu.qg);
    //print("n",norm(imu.q));
    //print("l",lnorm(imu.q));
  }

  imu.cqs*=conjugate(imu.qg);
  imu.cqs.normalize();
  
  for(byte p=0; p<GYROCNTP; p++)
  {
    imu.qg*=imu.qg;
//    print("qg",imu.qg);
  }

//  print("1",imu.q*conjugate(imu.cql));
  imu.cql=imu.cqs;//*imu.q*conjugate(imu.cql).normalize();   //tbd

  imu.qg=ident;
}

const quaternion imu_control(const quaternion &heading)
{
  fixed top[3]={control_ax, control_ay, control_az};
  quaternion h=heading;
  quaternion result;
  vectnorm(top);
  
  if(h.x!=0 || h.y!=0)
  {
    h.x=0;
    h.y=0;
    h.normalize();
  }
  
  result=sqrt(quaternion(top[2], -top[1], top[0], 0)) * h;   // (0,0,1)*top=(-top[1], top[0], 0)
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

const quaternion imu_get_orientation(void)       // to be called outside of gyro interrupt
{
  quaternion orientation;
  disable_sensor_interrupts();
  orientation=imu.q;
  enable_sensor_interrupts();
  return orientation;
}


