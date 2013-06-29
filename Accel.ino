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
  //vectnorm(accel_captured);
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


#ifdef ACCELCALIBRATE

void accel_calibrate_int_measure_wait(void)
{
  int oldval[3];
  int delta=30;
  static int icount=0;
  static int dcount=0;
  for (byte axis = 0; axis < 3; axis++)
    oldval[axis]=accelADC[axis];
  disable_sensor_interrupts();
  interrupts();
  accel_measure();
  enable_sensor_interrupts();

  if(icount==0 && accel_done==false)
    for (byte axis = 0; axis < 3; axis++)
      accelBuf[axis]=ACCELCNT>>1;

  if (abs(accelADC[0]-oldval[0])+abs(accelADC[1]-oldval[1])+abs(accelADC[2]-oldval[2])>delta)
  {
    icount=0;
    if(accel_done==false)
    {
      Serial.print("|");
      if(++dcount>79)
      {
        dcount=0;
        Serial.println("");
      }
    }
    return;
  }

  if(!accel_done)
  {
    for (byte axis = 0; axis < 3; axis++)
      accelBuf[axis]+=accelADC[axis];
    if(++icount==ACCELCNT)
    {
      accel_done=true;
      icount=0;
      dcount=0;
      Serial.println("");
    }
  }
}

void accel_calibrate_manual() //manual accel calibration
{
  //long a[4], b[4], c[4], x[3];
  long point[7][3];
  long x[3]={0,0,0};
  int offset_x, offset_y, offset_z;
  int gain[3];
  int fine_offset_x;
  int n=0;
//  Serial.println("Manual accel calibration\nThis algorithm will estimate accel zero values\nby measuring gravity in 4 different positions.\nThe more the positions differ, the better estimation.\nThe positions are not required to be exactly aligned to gravity in any way.\nPlease do not move your quadro.");
  disable_sensor_interrupts();
  updateRegisterI2C(accelAddress, 0x0D, 0x10); //ee_w=1
  updateRegisterI2C(accelAddress, 0x22, 0x01, 0x03); //offset_finetuning
  updateRegisterI2C(accelAddress, 0x29, 0xF2); //fine_offset_x
  updateRegisterI2C(accelAddress, 0x26, 0x24, 0xFE); //fine_offset_y
  updateRegisterI2C(accelAddress, 0x25, 0x00, 0x0F); //fine_offset_z (MSB)
  updateRegisterI2C(accelAddress, 0x23, 0x00, 0x07); //fine_offset_z (LSB)
  //updateRegisterI2C(accelAddress, 0x2E, 0x80, 0xFC);
  Serial.print("\ntcs="); Serial.println(readRegisterI2C(accelAddress, 0x20, 0x0F));
  
  offset_x=(((int)readRegisterI2C(accelAddress, 0x38)<<4)|(readRegisterI2C(accelAddress, 0x35)>>4))-2048; Serial.print("offset_x="); Serial.println(offset_x);
  offset_y=(((int)readRegisterI2C(accelAddress, 0x39)<<4)|readRegisterI2C(accelAddress, 0x36, 0x0F))-2048; Serial.print("offset_y="); Serial.println(offset_y);
  offset_z=(((int)readRegisterI2C(accelAddress, 0x3A)<<4)|(readRegisterI2C(accelAddress, 0x36)>>4))-2048; Serial.print("offset_z="); Serial.println(offset_z);
  gain[0]=(readRegisterI2C(accelAddress, 0x32)>>1)-64; Serial.print("\ngain_x="); Serial.println(gain[0]);
  gain[1]=(readRegisterI2C(accelAddress, 0x33)>>1)-64; Serial.print("gain_y="); Serial.println(gain[1]);
  gain[2]=(readRegisterI2C(accelAddress, 0x34)>>1)-64; Serial.print("gain_z="); Serial.println(gain[2]);
  enable_sensor_interrupts();
  
  gain[0]=21;
  disable_sensor_interrupts(); updateRegisterI2C(accelAddress, 0x32, 0x80+(gain[0]<<1), 0xFE);
  gain[0]=(readRegisterI2C(accelAddress, 0x32)>>1)-64; enable_sensor_interrupts(); Serial.print("\ngain_x="); Serial.println(gain[0]);

  gain[1]=20;
  disable_sensor_interrupts(); updateRegisterI2C(accelAddress, 0x33, 0x80+(gain[1]<<1), 0xFE);
  gain[1]=(readRegisterI2C(accelAddress, 0x33)>>1)-64; enable_sensor_interrupts(); Serial.print("gain_y="); Serial.println(gain[1]);

  gain[2]=16;
  disable_sensor_interrupts(); updateRegisterI2C(accelAddress, 0x34, 0x80+(gain[2]<<1), 0xFE);
  gain[2]=(readRegisterI2C(accelAddress, 0x34)>>1)-64; enable_sensor_interrupts(); Serial.print("gain_z="); Serial.println(gain[2]);

  offset_x=-55;
  disable_sensor_interrupts(); updateRegisterI2C(accelAddress, 0x38, 0x80+(offset_x>>4)); updateRegisterI2C(accelAddress, 0x35, (offset_x<<4)-((offset_x>>4)<<8), 0xF0);
  offset_x=(((int)readRegisterI2C(accelAddress, 0x38)<<4)|(readRegisterI2C(accelAddress, 0x35)>>4))-2048; enable_sensor_interrupts(); Serial.print("offset_x="); Serial.println(offset_x);

  offset_y=-508;
  disable_sensor_interrupts(); updateRegisterI2C(accelAddress, 0x39, 0x80+(offset_y>>4)); updateRegisterI2C(accelAddress, 0x36, offset_y-((offset_y>>4)<<4), 0x0F);
  offset_y=(((int)readRegisterI2C(accelAddress, 0x39)<<4)|readRegisterI2C(accelAddress, 0x36, 0x0F))-2048; enable_sensor_interrupts(); Serial.print("offset_y="); Serial.println(offset_y);

  offset_z=-45;
  disable_sensor_interrupts(); updateRegisterI2C(accelAddress, 0x3A, 0x80+(offset_z>>4)); updateRegisterI2C(accelAddress, 0x36, (offset_z<<4)-((offset_z>>4)<<8), 0xF0);
  offset_z=(((int)readRegisterI2C(accelAddress, 0x3A)<<4)|(readRegisterI2C(accelAddress, 0x36)>>4))-2048; enable_sensor_interrupts(); Serial.print("offset_z="); Serial.println(offset_z);
//accel_update_eeprom();
  attachInterrupt(4, accel_calibrate_int_measure_wait, RISING);

//  for(int i=-100;i<100;i+=2)
//  {
//    offset_x=i;
//    disable_sensor_interrupts(); updateRegisterI2C(accelAddress, 0x38, 0x80+(offset_x>>4)); updateRegisterI2C(accelAddress, 0x35, (offset_x<<4)-((offset_x>>4)<<8), 0xF0);
//    offset_x=(((int)readRegisterI2C(accelAddress, 0x38)<<4)|(readRegisterI2C(accelAddress, 0x35)>>4))-2048; enable_sensor_interrupts(); Serial.print(offset_x); Serial.print(", ");
//    //delay(5000);
//    for(accel_done=false; accel_done!=true;);
//    point[1][0]=accelBuf[0]/ACCELCNT; point[1][1]=accelBuf[1]/ACCELCNT; point[1][2]=accelBuf[2]/ACCELCNT;
//    Serial.println(point[1][0]);
//  }
//
//  gain[0]=20;
//  disable_sensor_interrupts(); updateRegisterI2C(accelAddress, 0x32, 0x80+(gain[0]<<1), 0xFE);
//  gain[0]=(readRegisterI2C(accelAddress, 0x32)>>1)-64; enable_sensor_interrupts(); Serial.print("\ngain[0]="); Serial.println(gain[0]);
//
//  for(int i=-100;i<100;i+=2)
//  {
//    offset_x=i;
//    disable_sensor_interrupts(); updateRegisterI2C(accelAddress, 0x38, 0x80+(offset_x>>4)); updateRegisterI2C(accelAddress, 0x35, (offset_x<<4)-((offset_x>>4)<<8), 0xF0);
//    offset_x=(((int)readRegisterI2C(accelAddress, 0x38)<<4)|(readRegisterI2C(accelAddress, 0x35)>>4))-2048; enable_sensor_interrupts(); Serial.print(offset_x); Serial.print(", ");
//    //delay(5000);
//    for(accel_done=false; accel_done!=true;);
//    point[1][0]=accelBuf[0]/ACCELCNT; point[1][1]=accelBuf[1]/ACCELCNT; point[1][2]=accelBuf[2]/ACCELCNT;
//    Serial.println(point[1][0]);
//  }
//

//  gain[0]=50;
//  disable_sensor_interrupts(); updateRegisterI2C(accelAddress, 0x32, 0x80+(gain[0]<<1), 0xFE);
//  gain[0]=(readRegisterI2C(accelAddress, 0x32)>>1)-64; enable_sensor_interrupts(); Serial.print("\ngain[0]="); Serial.println(gain[0]);
//
//  for(int i=-100;i<0;i+=2)
//  {
//    offset_x=i;
//    disable_sensor_interrupts(); updateRegisterI2C(accelAddress, 0x38, 0x80+(offset_x>>4)); updateRegisterI2C(accelAddress, 0x35, (offset_x<<4)-((offset_x>>4)<<8), 0xF0);
//    offset_x=(((int)readRegisterI2C(accelAddress, 0x38)<<4)|(readRegisterI2C(accelAddress, 0x35)>>4))-2048; enable_sensor_interrupts(); Serial.print(offset_x); Serial.print(", ");
//    //delay(5000);
//    for(accel_done=false; accel_done!=true;);
//    point[1][0]=accelBuf[0]/ACCELCNT; point[1][1]=accelBuf[1]/ACCELCNT; point[1][2]=accelBuf[2]/ACCELCNT;
//    Serial.println(point[1][0]);
//  }
//

//  offset_x=0;
//  disable_sensor_interrupts(); updateRegisterI2C(accelAddress, 0x38, 0x80+(offset_x>>4)); updateRegisterI2C(accelAddress, 0x35, (offset_x<<4)-((offset_x>>4)<<8), 0xF0);
//  offset_x=(((int)readRegisterI2C(accelAddress, 0x38)<<4)|(readRegisterI2C(accelAddress, 0x35)>>4))-2048; enable_sensor_interrupts(); Serial.print("offset_x="); Serial.println(offset_x);
//
//  offset_y=0;
//  disable_sensor_interrupts(); updateRegisterI2C(accelAddress, 0x39, 0x80+(offset_y>>4)); updateRegisterI2C(accelAddress, 0x36, offset_y-((offset_y>>4)<<4), 0x0F);
//  offset_y=(((int)readRegisterI2C(accelAddress, 0x39)<<4)|readRegisterI2C(accelAddress, 0x36, 0x0F))-2048; enable_sensor_interrupts(); Serial.print("offset_y="); Serial.println(offset_y);
//
//  offset_z=0;
//  disable_sensor_interrupts(); updateRegisterI2C(accelAddress, 0x3A, 0x80+(offset_z>>4)); updateRegisterI2C(accelAddress, 0x36, (offset_z<<4)-((offset_z>>4)<<8), 0xF0);
//  offset_z=(((int)readRegisterI2C(accelAddress, 0x3A)<<4)|(readRegisterI2C(accelAddress, 0x36)>>4))-2048; enable_sensor_interrupts(); Serial.print("offset_z="); Serial.println(offset_z);


point[0][0]=-4096;
point[0][1]=0;
point[0][2]=0;

point[1][0]=0;
point[1][1]=3996;
point[1][2]=0;

point[2][0]=0;
point[2][1]=0;
point[2][2]=4096;

point[3][0]=0;
point[3][1]=-3996;//4096;
point[3][2]=0;

point[4][0]=2364;
point[4][1]=2307;//2364;
point[4][2]=-2365;

point[5][0]=2364;
point[5][1]=2307;//2364;
point[5][2]=2365;
//0,5,0
// 4096   4096   4096
//4096^2 4096^2 4096^2
//   0    4096     0

//3996^2   0    3996^2

  for(byte i=0;i<7;i++)
  {
    Serial.print("Waiting for position "); Serial.print(i+1); Serial.println("...");
    delay(5000);
    for(accel_done=false; accel_done!=true;);
    for(byte j=0;j<3;j++)
      point[i][j]=accelBuf[j]/ACCELCNT;
    printpoint(point[i]);
    Serial.println(sqrt(point[i][0]*point[i][0]+point[i][1]*point[i][1]+point[i][2]*point[i][2]));
  }
  Serial.println("Done. Thanks. Now calculating...");

//  if(findGain(point[0], point[1], point[2], point[3], point[4], point[5], x, 4096)) // find gain to be applied for the 5 points to be on a sphere
//    {Serial.println("Error: Non-singular matrix. Possible reasons: programmer was drunk. Please try again.");
//    return;
//    }
//  while(findGain(point[0], point[1], point[2], point[3], point[4], point[5], x, 4096)!=0)
//  {
//    if(n++>5)
//    {
//      Serial.println("Something is wrong... Please try again later");
//      return;
//    }
//    Serial.println("Additional position needed");
//    for(byte i=0; i<5; i++)
//      for(byte j=0; j<3; j++)
//        point[i][j]=point[i+1][j];
//    for(accel_done=false; accel_done!=true;);
//    for(byte j=0;j<3;j++)
//      point[5][j]=accelBuf[j]/ACCELCNT;
//    printpoint(point[5]);
//  }
//printRow(x,"gain:");
//
//findGain(point[2], point[1], point[0], point[3], point[4], point[5], x, 4096);
//printRow(x,"gain1:");
//findGain(point[3], point[1], point[2], point[0], point[4], point[5], x, 4096);
//printRow(x,"gain2:");
//findGain(point[4], point[1], point[2], point[3], point[0], point[5], x, 4096);
//printRow(x,"gain3:");
//findGain(point[5], point[1], point[2], point[3], point[4], point[0], x, 4096);
//printRow(x,"gain4:");
//findGain(point[0], point[1], point[3], point[4], point[2], point[5], x, 4096);
//printRow(x,"gain5:");
//
//findGain(point[0], point[1], point[2], point[3], point[4], point[5], x, 4096);
//
//for(byte i=0; i<7; i++)
//  for(byte j=0; j<3; j++)
//    point[i][j]=point[i][j]>0 ? (point[i][j]*(200+x[j])+100)/200 : (point[i][j]*(200+x[j])-100)/200;

printRow(point[0],"point[0]:");
printRow(point[1],"point[1]:");
printRow(point[2],"point[2]:");
printRow(point[3],"point[3]:");
printRow(point[4],"point[4]:");
printRow(point[5],"point[5]:");
printRow(point[6],"point[6]:");

  if(findCenter(point[0], point[1], point[2], point[3], x)) // find center of circumscribed sphere defined by four points
    {Serial.println("Error: The measured points are on the same plane. Possible reasons: measured positions are too close, sensor reading are wrong, sensor was moving during calibration sequence. Please try again.");
    return;
    }
  printRow(x,"center1:");
  
  if(findCenter(point[4], point[1], point[2], point[3], x)) // find center of circumscribed sphere defined by four points
    {Serial.println("Error: The measured points are on the same plane. Possible reasons: measured positions are too close, sensor reading are wrong, sensor was moving during calibration sequence. Please try again.");
    return;
    }
  printRow(x,"center2:");

  if(findCenter(point[5], point[1], point[2], point[3], x)) // find center of circumscribed sphere defined by four points
    {Serial.println("Error: The measured points are on the same plane. Possible reasons: measured positions are too close, sensor reading are wrong, sensor was moving during calibration sequence. Please try again.");
    return;
    }
  printRow(x,"center3:");

  if(findCenter(point[6], point[1], point[2], point[3], x)) // find center of circumscribed sphere defined by four points
    {Serial.println("Error: The measured points are on the same plane. Possible reasons: measured positions are too close, sensor reading are wrong, sensor was moving during calibration sequence. Please try again.");
    return;
    }
  printRow(x,"center4:");
findCenter(point[0], point[1], point[2], point[3], x);
  for(int i=0;i<7;i++)
  {
    for(int j=0;j<3;j++)
      point[i][j]-=x[j];
    Serial.println("-");
    printRow(point[i],"point:");
    Serial.println(sqrt(point[i][0]*point[i][0]+point[i][1]*point[i][1]+point[i][2]*point[i][2]));
  }
  //delay(1500);
  attachInterrupt(4, accel_int, RISING); //not necessery if we call accel_init
  //accel_update_eeprom(); //this will call accel_init and soft reset the sensor
}

#endif

