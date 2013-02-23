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
    updateRegisterI2C(gyroAddress, 0x15, 0x04); // Sample rate  // 200Hz     //0F
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

  if(cstep<=1<<GYROCNTP)
  {
    gyro_time=micros()-gyro_oldtime;
    gyro_oldtime=micros();
  }

  disable_sensor_interrupts();
  interrupts();
  gyro_measure();
  enable_sensor_interrupts();

  if(cstep==0)
  {
    imu_init_calibrate_orientation();

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

  if(cstep<=1<<GYROCNTP)
    findTime+=gyro_time;

  if(cstep==1<<GYROCNTP)
  {
    detachInterrupt(GyroIntNum);
    gyro_time=findTime>>GYROCNTP;
    imu_calibrate_orientation();
    Serial.println("Gyro 1st pass calibration complete");
    print("cqs",imu.cqs);
    print("cql",imu.cql);

    disable_sensor_interrupts();
    attachInterrupt(GyroIntNum, gyro_calibrate, RISING);
    gyro_int_clear();

    gyro_interrupted=0;
    digitalWrite(GyroLEDPin, LOW);
    if(accel_interrupted)
      digitalWrite(AccelLEDPin, HIGH);
    else
      digitalWrite(StatusLEDPin, HIGH);
    cstep++;
    noInterrupts();
    enable_sensor_interrupts();
    return;
  }

  if(cstep==2<<GYROCNTP)
  {
    detachInterrupt(GyroIntNum);

    imu_calibrate_orientation();
    Serial.println("Gyro 2nd pass calibration complete");
    print("cqs",imu.cqs);
    print("cql",imu.cql);

    disable_sensor_interrupts();
    attachInterrupt(GyroIntNum, gyro_calibrate, RISING);
    gyro_int_clear();

    gyro_interrupted=0;
    digitalWrite(GyroLEDPin, LOW);
    if(accel_interrupted)
      digitalWrite(AccelLEDPin, HIGH);
    else
      digitalWrite(StatusLEDPin, HIGH);
    cstep++;
    noInterrupts();
    enable_sensor_interrupts();
    return;
  }

  if(cstep==3<<GYROCNTP)
  {
    detachInterrupt(GyroIntNum);

    imu_calibrate_orientation();
    Serial.println("Gyro 3rd pass calibration complete");
    print("cqs",imu.cqs);
    print("cql",imu.cql);

    disable_sensor_interrupts();
    attachInterrupt(GyroIntNum, gyro_calibrate, RISING);
    gyro_int_clear();

    gyro_interrupted=0;
    digitalWrite(GyroLEDPin, LOW);
    if(accel_interrupted)
      digitalWrite(AccelLEDPin, HIGH);
    else
      digitalWrite(StatusLEDPin, HIGH);
    cstep++;
    noInterrupts();
    enable_sensor_interrupts();
    return;
  }

  if(cstep==4<<GYROCNTP)
  {
    detachInterrupt(GyroIntNum);

    imu_calibrate_orientation();

    Serial.println("Gyro 4th pass calibration complete");
    print("cqs",imu.cqs);
    print("cql",imu.cql);
  
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
    cstep=0;
    noInterrupts();
    enable_sensor_interrupts();
    return;
  }

  gyro_interrupted=0;
  digitalWrite(GyroLEDPin, LOW);
  if(accel_interrupted)
    digitalWrite(AccelLEDPin, HIGH);
  else
    digitalWrite(StatusLEDPin, HIGH);
  cstep++;
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

