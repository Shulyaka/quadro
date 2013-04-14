volatile bool accel_capture_flag=false;
fixed accel_captured[3]=0;

#define accel_delta 100

void accel_capture_int(void)
{
  int oldval[3];
  static int icount=0;
  static int dcount=0;
  static unsigned long int accel_oldtime=0;
  accel_interrupted=true;
  digitalWrite(AccelLEDPin, HIGH);
  if(gyro_interrupted)
    digitalWrite(GyroLEDPin, LOW);
  else
    digitalWrite(StatusLEDPin, LOW);
  accel_time=micros()-accel_oldtime;
  accel_oldtime=micros();
  for (byte axis = 0; axis < 3; axis++)
    oldval[axis]=accelADC[axis];

  disable_sensor_interrupts();
  interrupts();
  accel_measure();
  
  if(gyro_ready)
    imu_updatePosition((long)accelADC[0]<<18, (long)accelADC[1]<<18, (long)accelADC[2]<<18);

  if(icount==0 && accel_capture_flag)
    for (byte axis = 0; axis < 3; axis++)
      accelBuf[axis]=0;//1<<(ACCELCNTP-1);

  if (abs(accelADC[0]-oldval[0])+abs(accelADC[1]-oldval[1])+abs(accelADC[2]-oldval[2])>accel_delta)
  {
    icount=0;
    if(accel_capture_flag)
    {
      Serial.print("|");
      if(++dcount>79)
      {
        dcount=0;
        Serial.println("");
      }
    }
    
    digitalWrite(AccelLEDPin, LOW);
    if(gyro_interrupted)
      digitalWrite(GyroLEDPin, HIGH);
    else
      digitalWrite(StatusLEDPin, HIGH);
    accel_interrupted=false;

    enable_sensor_interrupts();

    return;
  }
  
  if(accel_capture_flag)
  {
    for (byte axis = 0; axis < 3; axis++)
      accelBuf[axis]+=accelADC[axis];
    if(++icount==1<<ACCELCNTP)
    {
      accel_capture_flag=false;
      icount=0;
      dcount=0;
      for (byte axis = 0; axis < 3; axis++)
      {
        accel_captured[axis]=accelBuf[axis]<<(18-ACCELCNTP);
        accel_captured[axis]=accel_captured[axis]+accel_captured[axis]*accel_captured[axis]*accel_square[axis]+accel_captured[axis]*accel_gain[axis]+accel_offset[axis];
      }
      Serial.println("");
    }
  }
  
  digitalWrite(AccelLEDPin, LOW);
  if(gyro_interrupted)
    digitalWrite(GyroLEDPin, HIGH);
  else
    digitalWrite(StatusLEDPin, HIGH);
  accel_interrupted=false;

  enable_sensor_interrupts();
}

void accel_capture(void)
{
  accel_capture_flag=true;
  while(accel_capture_flag)
    continue;
}

void orientation_capture(void)
{
  
  
  
}

void accel_calibrate_orientation(void)
{
  fixed grav[3];
  fixed arr[3];
  quaternion q1, q2;
  if(!gyro_ready || !accel_ready)
  {
    Serial.println("Vaiting for sensors to be ready");
    while (!gyro_ready || !accel_ready)
      continue;
    Serial.println("Sensors ready");
  }
  
  attachInterrupt(AccelIntNum, accel_capture_int, RISING);
  
  accel_capture();
  
  print("ax", accel_captured[0]);
  print("ay", accel_captured[1]);
  print("az", accel_captured[2]);
  
  attachInterrupt(AccelIntNum, accel_int, RISING);
}

void gyro_calibrate_orientation(void)
{
  quaternion pos;
  fixed top[3];
  
  if(!gyro_ready || !accel_ready)
  {
    Serial.println("Vaiting for sensors to be ready");
    while (!gyro_ready || !accel_ready)
      continue;
    Serial.println("Sensors ready");
  }

  attachInterrupt(AccelIntNum, accel_capture_int, RISING);

  gyro_orientation=ident;
  
  Serial.println("Waiting for the position 1");
  accel_capture();
  pos=imu_get_orientation();
  Serial.println("Waiting for the position 2");
  accel_capture();
  pos=imu_get_orientation()*conjugate(pos);

  top[0]=pos.x;
  top[1]=pos.y;
  top[2]=pos.z;

  if(top[2]<0)
  {
    top[0]=-top[0];
    top[1]=-top[1];
    top[2]=-top[2];
  }

  vectnorm(top);
  
  gyro_orientation=sqrt(quaternion(top[2], top[1], -top[0], 0));
  
  print("pos",pos);
  print("gyro_orientation", gyro_orientation);
  
  attachInterrupt(AccelIntNum, accel_int, RISING);
}

void calibrate_orientation(void)
{
  quaternion pos;
  fixed arr[3];
  quaternion q1, q2;
  quaternion accq;
  quaternion tmp;
  if(!gyro_ready || !accel_ready)
  {
    Serial.println("Vaiting for sensors to be ready");
    while (!gyro_ready || !accel_ready)
      continue;
    Serial.println("Sensors ready");
  }

  attachInterrupt(AccelIntNum, accel_capture_int, RISING);

  gyro_orientation=ident;
  
  Serial.println("Waiting for the position 1");
  accel_capture();
  pos=imu_get_orientation();
  
  vectnorm(accel_captured);
  
  tmp=conjugate(pos)*quaternion(one,0,0)*pos;
  arr[0]=tmp.x;
  arr[1]=tmp.y;
  arr[2]=tmp.z;
  vectnorm(arr);

  print("arr.x", arr[0]);
  print("arr.y", arr[1]);
  print("arr.z", arr[2]);

  q2=sqrt(quaternion(accel_captured[2], accel_captured[1], -accel_captured[0], 0));
print("q2",q2);
  q1=q2*quaternion(arr[0], arr[1], arr[2])*conjugate(q2);

  arr[0]=q1.x;
  arr[1]=q1.y;
  arr[2]=0;
  vectnorm(arr);

  q1=sqrt(quaternion(arr[0], 0, 0, -arr[1]));
print("q1",q1);
  accq=q1*q2;
  
  print("accp1",accq);
  
  Serial.println("Waiting for the position 2");
  accel_capture();
  tmp=imu_get_orientation();
  pos=tmp*conjugate(pos);

  vectnorm(accel_captured);
  
  tmp=conjugate(tmp)*quaternion(one,0,0)*tmp;
  arr[0]=tmp.x;
  arr[1]=tmp.y;
  arr[2]=tmp.z;
  vectnorm(arr);

  print("arr.x", arr[0]);
  print("arr.y", arr[1]);
  print("arr.z", arr[2]);
  
  q2=sqrt(quaternion(accel_captured[2], accel_captured[1], -accel_captured[0], 0));

  q1=q2*quaternion(arr[0], arr[1], arr[2])*conjugate(q2);

  arr[0]=q1.x;
  arr[1]=q1.y;
  arr[2]=0;
  vectnorm(arr);

  q1=sqrt(quaternion(arr[0], 0, 0, -arr[1]));

  accq=q1*q2*conjugate(accq);

  print("accp1",q1*q2);
  
  arr[0]=pos.x;
  arr[1]=pos.y;
  arr[2]=pos.z;

  if(arr[2]<0)
  {
    arr[0]=-arr[0];
    arr[1]=-arr[1];
    arr[2]=-arr[2];
  }

  vectnorm(arr);
  
  gyro_orientation=sqrt(quaternion(arr[2], arr[1], -arr[0], 0));
  
  print("pos",pos);
  print("gyro_orientation", gyro_orientation);
  
  arr[0]=accq.x;
  arr[1]=accq.y;
  arr[2]=accq.z;

  if(arr[2]<0)
  {
    arr[0]=-arr[0];
    arr[1]=-arr[1];
    arr[2]=-arr[2];
  }

  vectnorm(arr);
  
  accel_orientation=sqrt(quaternion(arr[2], arr[1], -arr[0], 0));
  
  print("accq",accq);
  print("accel_orientation", accel_orientation);
  
  
  attachInterrupt(AccelIntNum, accel_int, RISING);
  
  
  
}

