#define accelAddress 0x40 // page 54 and 61 of datasheet
#define accel_capture_delta 200
int accelADC[3];
long accelBuf[3];
volatile bool accel_done=true;
volatile bool accel_capture_flag=false;
fixed accel_captured[3]=0;
quaternion accel_captured_imu=ident;

void accel_init(void)
{
    disable_sensor_interrupts();
    accel_time=0;
    // Check if accel is connected
    if (readWhoI2C(accelAddress) != 0x03) // page 52 of datasheet
      error("Accelerometer not found!");

    updateRegisterI2C(accelAddress, 0x10, 0xB6); //reset device
    delay(10);  //sleep 10 ms after reset (page 25)

    updateRegisterI2C(accelAddress, 0x0D, 0x10); //enable writing to control registers (ee_w=1)
    updateRegisterI2C(accelAddress, 0x20, 0x10, 0xF0); // set low pass filter to 10Hz (bw=1)
    updateRegisterI2C(accelAddress, 0x35, 0x01, 0x01); // smp_skip=1
    updateRegisterI2C(accelAddress, 0x35, 0x04, 0x0E); // Set range select bits for +/-2g
    updateRegisterI2C(accelAddress, 0x21, 0x02); //new_data_int
    updateRegisterI2C(accelAddress, 0x0D, 0x00); //disable writing to control registers (ee_w=0)
    
    delay(100); //waiting for accel to stabilize
    accel_clear_int();
    attachInterrupt(AccelIntNum, accel_calibrate, RISING);
    //accel_measure();
    enable_sensor_interrupts();
}

void accel_calibrate()
{  //does not calibrate currently
  Serial.println("Accel calibration complete");
  accel_ready=true;
  if(gyro_ready)
  {
    imu_init_orientation();
    imu_init_position();
  }
  attachInterrupt(AccelIntNum, accel_int, RISING);
}

void accel_measure(void) //warning: you must call disable_sensor_interrupts() and interrupts() before attempting to call this function
{
    sendByteI2C(accelAddress, 0x02);
    Wire.requestFrom(accelAddress, 6);

    accelADC[0] = readReverseWordI2C() >> 2;
    accelADC[1] = readReverseWordI2C() >> 2;
    accelADC[2] = readReverseWordI2C() >> 2;
  }

void accel_capture_wait(void)
{
  accel_capture_flag=true;
  while(accel_capture_flag)
    continue;
}

void accel_clear_int(void)
{
  accel_measure();
}

void accel_update_eeprom(void)
{
  updateRegisterI2C(accelAddress, 0x0D, 0x10, 0x10); //ee_w=1

  for(byte i=0x40; i<0x5B; i+=2)
  {
    delay(100);
    updateRegisterI2C(accelAddress, i, 0x00);
  }

  delay(1000);

  accel_init();
}

/*  // The below is the old accel interrupt without position capturing support:
void accel_int(void)
{
  static unsigned long int accel_oldtime=0;
  accel_interrupted=true;
  digitalWrite(AccelLEDPin, HIGH);
  if(gyro_interrupted)
    digitalWrite(GyroLEDPin, LOW);
  else
    digitalWrite(StatusLEDPin, LOW);
  accel_time=micros()-accel_oldtime;
  accel_oldtime=micros();

  disable_sensor_interrupts();
  interrupts();
  accel_measure();
  
  if(gyro_ready)
    imu_updatePosition((long)accelADC[0]<<18, (long)accelADC[1]<<18, (long)accelADC[2]<<18);
  
  digitalWrite(AccelLEDPin, LOW);
  if(gyro_interrupted)
    digitalWrite(GyroLEDPin, HIGH);
  else
    digitalWrite(StatusLEDPin, HIGH);
  accel_interrupted=false;

  enable_sensor_interrupts();
}
*/

void accel_int(void)
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

  if(accel_capture_flag)
    for (byte axis = 0; axis < 3; axis++)
      oldval[axis]=accelADC[axis];

  disable_sensor_interrupts();
  interrupts();
  accel_measure();
  
  if(gyro_ready)
    imu_updatePosition((long)accelADC[0]<<18, (long)accelADC[1]<<18, (long)accelADC[2]<<18);

  if(accel_capture_flag)
  {
    if(icount==0)
    {
      for (byte axis = 0; axis < 3; axis++)
        accelBuf[axis]=0;//1<<(ACCELCNTP-1);
      accel_captured_imu=imu.q;
    }

    if(abs(accelADC[0]-oldval[0])+abs(accelADC[1]-oldval[1])+abs(accelADC[2]-oldval[2])>accel_capture_delta)
    {
      icount=0;
      Serial.print("|");
      //Serial.println(accelADC[0]-oldval[0]);
      //Serial.println(accelADC[1]-oldval[1]);
      //Serial.println(accelADC[2]-oldval[2]);
      if(++dcount>79)
      {
        dcount=0;
        Serial.println("");
      }
    
      digitalWrite(AccelLEDPin, LOW);
      if(gyro_interrupted)
        digitalWrite(GyroLEDPin, HIGH);
      else
        digitalWrite(StatusLEDPin, HIGH);
      accel_interrupted=false;

      noInterrupts();
      enable_sensor_interrupts();

      return;
    }
  
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

  noInterrupts();
  enable_sensor_interrupts();
}

void printpoint(long *x)
{ Serial.println(x[0]);
  Serial.println(x[1]);
  Serial.println(x[2]);
}

