#define gyroAddress 0x69
int gyroADC[3];
//long gyroBuf[3];
int gyroZero[3];
int gyro_icnt;

void gyro_init(void)
{
    int x;
    // Check if gyro is connected
    disable_sensor_interrupts();
    gyro_time=0;
    if (readWhoI2C(gyroAddress) != gyroAddress)
      Serial.println("Gyro not found!");

    updateRegisterI2C(gyroAddress, 0x3E, 0x80); // send a reset to the device
    delay(10);
    updateRegisterI2C(gyroAddress, 0x15, 0x0F); // Sample rate  // 200Hz
    updateRegisterI2C(gyroAddress, 0x16, 0x18+0x06); // 10Hz low pass filter
    updateRegisterI2C(gyroAddress, 0x17, 0x31); // enable raw data ready interrupt
    updateRegisterI2C(gyroAddress, 0x3E, 0x02); // use Y gyro oscillator
    
    delay(100); //waiting for gyro to stabilize

    gyro_int_clear();
    attachInterrupt(GyroIntNum, gyro_calibrate, RISING);
    enable_sensor_interrupts();
}

void gyro_measure(void) //warning: you must call disable_sensor_interrupts() and interrupts() before attempting to call this function to be sure that interrupts are enabled but there is no I2C traffic
{
  sendByteI2C(gyroAddress, 0x1D);

  Wire.requestFrom(gyroAddress, 6);
  for (byte axis = 0; axis < 3; axis++)
    gyroADC[axis]=((Wire.read() << 8) | Wire.read());
}

void gyro_measure_temp(void)
{
  sendByteI2C(gyroAddress, 0x1B);
  Wire.requestFrom(gyroAddress, 2);
  Serial.println((((Wire.read() << 8) | Wire.read())+13200)/280+35);
}

/*
void gyro_calibrate(void) // finds gyroZero in background, sets gyro_time to non-zero when ready, to be called within interrupt
{
  static unsigned long long int findTime;
  static unsigned long int gyro_oldtime;
  static int cstep=-2;
  unsigned long int gyro_newtime=micros();
  
  digitalWrite(GyroLEDPin, HIGH);
  gyro_interrupted=1;
  if(accel_interrupted)
    digitalWrite(AccelLEDPin, LOW);
  else
    digitalWrite(StatusLEDPin, LOW);
  cstep++;
  
  disable_sensor_interrupts();
  interrupts();
  gyro_measure();
  enable_sensor_interrupts();
  
  if (cstep==-1)
  {
    for (byte i=0; i<3; i++)
      gyroADC[i]=0;
    gyro_interrupted=0;
    digitalWrite(GyroLEDPin, LOW);
    if(accel_interrupted)
      digitalWrite(AccelLEDPin, HIGH);
    else
      digitalWrite(StatusLEDPin, HIGH);
    return; //skipping for the first time
  }
  
  if(cstep==0)
  {
    for (byte i=0; i<3; i++)
      gyroBuf[i]=FINDZERO>>1;
    findTime=FINDZERO>>1;
    gyro_oldtime=gyro_newtime;
    gyro_interrupted=0;
    digitalWrite(GyroLEDPin, LOW);
    if(accel_interrupted)
      digitalWrite(AccelLEDPin, HIGH);
    else
      digitalWrite(StatusLEDPin, HIGH);
    return; //and for the second time as well
  }

  findTime+=gyro_newtime-gyro_oldtime;
  gyro_oldtime=gyro_newtime;

  if(cstep!=FINDZERO<<GYROCNTP)
  {
    gyro_interrupted=0;
    digitalWrite(GyroLEDPin, LOW);
    if(accel_interrupted)
      digitalWrite(AccelLEDPin, HIGH);
    else
      digitalWrite(StatusLEDPin, HIGH);
    return;
  }

  detachInterrupt(GyroIntNum);
  //attachInterrupt(GyroIntNum, gyro_int, RISING);

  gyroZero[0] = gyroBuf[0]/FINDZERO-((1<<GYROCNTP)>>1);
  gyroZero[1] = gyroBuf[1]/FINDZERO-((1<<GYROCNTP)>>1);
  gyroZero[2] = gyroBuf[2]/FINDZERO-((1<<GYROCNTP)>>1);
  gyro_time = findTime/FINDZERO;

  for (byte i=0; i<3; i++)
    gyroBuf[i]=0;

  cstep=-2;
  gyro_icnt=0;
  Serial.println("Gyro calibration complete");
  gyro_ready=true;
  imu_init_orientation();
  disable_sensor_interrupts();
  attachInterrupt(GyroIntNum, gyro_int, RISING);
  gyro_int_clear();

  gyro_interrupted=0;
  digitalWrite(GyroLEDPin, LOW);
  if(accel_interrupted)
    digitalWrite(AccelLEDPin, HIGH);
  else
    digitalWrite(StatusLEDPin, HIGH);
  enable_sensor_interrupts();
}
*/

void gyro_calibrate(void) // finds calibration quaternion in background, sets gyro_ready when ready, to be called within interrupt
{
  static unsigned long long int findTime;
  static unsigned long int gyro_oldtime;
  static unsigned int cstep=0;
  unsigned long int gyro_newtime=micros();
  
  digitalWrite(GyroLEDPin, HIGH);
  gyro_interrupted=1;
  if(accel_interrupted)
    digitalWrite(AccelLEDPin, LOW);
  else
    digitalWrite(StatusLEDPin, LOW);

  gyro_time=micros()-gyro_oldtime;
  gyro_oldtime=micros();

  disable_sensor_interrupts();
  interrupts();
  gyro_measure();
  enable_sensor_interrupts();

  if(cstep==0)
  {
    imu.q=ident;
    imu.cq=ident;

    findTime=1<<(GYROCNTP-1);

    if(!--gyro_interrupted)
      digitalWrite(GyroLEDPin, LOW);
    if(accel_interrupted)
      digitalWrite(AccelLEDPin, HIGH);
    else
      digitalWrite(StatusLEDPin, HIGH);

    cstep++;
    return; // skipping for the first time because gyro_time value may be wrong
  }

  imu_updateOrientation(gyroADC[0],gyroADC[1],gyroADC[2]);

  findTime+=gyro_time;

  if(cstep!=1<<GYROCNTP)
  {
    gyro_interrupted=0;
    digitalWrite(GyroLEDPin, LOW);
    if(accel_interrupted)
      digitalWrite(AccelLEDPin, HIGH);
    else
      digitalWrite(StatusLEDPin, HIGH);
    cstep++;
    return;
  }

  detachInterrupt(GyroIntNum);
  cstep=0;

  gyro_time=findTime>>GYROCNTP;

  print("q", imu.q);
  
  for(char p=0; p<GYROCNTP; p++)
  {
    imu.q=half(imu.q);
    print("q",imu.q);
  }

  imu.cq=conjugate(imu.q);

  Serial.println("Gyro calibration complete");
  print("qc",imu.cq);
  
  gyro_ready=true;
  imu_init_orientation();
  disable_sensor_interrupts();
  attachInterrupt(GyroIntNum, gyro_int, RISING);
  gyro_int_clear();

  gyro_interrupted=0;
  digitalWrite(GyroLEDPin, LOW);
  if(accel_interrupted)
    digitalWrite(AccelLEDPin, HIGH);
  else
    digitalWrite(StatusLEDPin, HIGH);
  enable_sensor_interrupts();  
}


void gyro_int_clear(void)
{
  sendByteI2C(gyroAddress, 0x1A);
  Wire.requestFrom(gyroAddress, 1);
  Wire.read();
}

void gyro_int(void)
{
  digitalWrite(GyroLEDPin, HIGH);
  gyro_interrupted++;
  if(accel_interrupted)
    digitalWrite(AccelLEDPin, LOW);
  else
    digitalWrite(StatusLEDPin, LOW);
  
  disable_sensor_interrupts();
  interrupts();
  gyro_measure();
  enable_sensor_interrupts();

  imu_updateOrientation(gyroADC[0],gyroADC[1],gyroADC[2]);
  
  if(flight_state==FSTATE_FLY)
    motor_updateControl();

  if(!--gyro_interrupted)
    digitalWrite(GyroLEDPin, LOW);
  if(accel_interrupted)
    digitalWrite(AccelLEDPin, HIGH);
  else
    digitalWrite(StatusLEDPin, HIGH);
}

