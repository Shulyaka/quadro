#include "I2C.h"

#define XAXIS 0
#define YAXIS 1
#define ZAXIS 2

#define accelAddress 0x40 // page 54 and 61 of datasheet
int accelADC[3];
long accelBuf[3];
int accelOneG;
void accel_measure(void);
void AutoOneG(void);
void accel_int(void);
void state_updatePosition(fixed i, fixed j, fixed k);

void accel_init(void)
{
    byte data;
//    int accelAddress = 0x40; // page 54 and 61 of datasheet
    
    disable_sensor_interrupts();
    // Check if accel is connected
    if (readWhoI2C(accelAddress) != 0x03) // page 52 of datasheet
      error("Accelerometer not found!");

    updateRegisterI2C(accelAddress, 0x10, 0xB6); //reset device
    delay(10);  //sleep 10 ms after reset (page 25)

    updateRegisterI2C(accelAddress, 0x0D, 0x10); //enable writing to control registers
    updateRegisterI2C(accelAddress, 0x20, 0x10, 0xF0); // set low pass filter to 10Hz (value = 0000xxxx)
    updateRegisterI2C(accelAddress, 0x35, 0x01, 0x01); // smp_skip=1
    updateRegisterI2C(accelAddress, 0x35, 0x04, 0x0E); // Set range select bits for +/-2g
    updateRegisterI2C(accelAddress, 0x21, 0x02); //new_data_int
    
    enable_sensor_interrupts();
    delay(100); //waiting for gyro to stabilize
    attachInterrupt(4, accel_int, RISING);
    disable_sensor_interrupts();
    accel_measure();
    enable_sensor_interrupts();
}

void accel_measure(void) //warning: you must call disable_sensor_interrupts() and interrupts() before attempting to call this function
{
    sendByteI2C(accelAddress, 0x02);
    Wire.requestFrom(accelAddress, 6);

    for (byte axis = 0; axis < 3; axis++)
      accelADC[axis] = (Wire.read()|(Wire.read() << 8)) >> 2;
  }

unsigned long int accel_time=0;
unsigned long int accel_oldtime=0;
void accel_int(void)
{
  digitalWrite(LampPin, 1);
  accel_time=micros()-accel_oldtime;
  accel_oldtime=micros();

  disable_sensor_interrupts();
  interrupts();
  accel_measure();
//  enable_sensor_interrupts();
  
  state_updatePosition(-(long)accelADC[1]<<18, (long)accelADC[0]<<18, -(long)accelADC[2]<<18);
  enable_sensor_interrupts();

  digitalWrite(LampPin, 0);
}

void printpoint(long *x)
{ Serial.println(x[0]);
  Serial.println(x[1]);
  Serial.println(x[2]);
}

volatile bool accel_done=true;

void accel_calibrate_int_measure_wait(void)
{
  int oldval[3];
  int delta=30;
  static int icount=0;
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
    //if(accel_done==false)
      //Serial.print("|");
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
      //Serial.println("");
    }
  }
}

void accel_calibrate() //manual accel calibration
{
  //long a[4], b[4], c[4], x[3];
  long point[4][3];  //5
  long x[3];
  int offset_x;
  int gain_x;
  int fine_offset_x;
  attachInterrupt(4, accel_calibrate_int_measure_wait, RISING);
//  Serial.println("Manual accel calibration\nThis algorithm will estimate accel zero values\nby measuring gravity in 4 different positions.\nThe more the positions differ, the better estimation.\nThe positions are not required to be exactly aligned to gravity in any way.\nPlease do not move your quadro.");
  disable_sensor_interrupts();
  updateRegisterI2C(accelAddress, 0x22, 0x00, 0x03);
  updateRegisterI2C(accelAddress, 0x29, 0x00);
  updateRegisterI2C(accelAddress, 0x2E, 0x80, 0xFC);
  Serial.print("\ntcs="); Serial.println(readRegisterI2C(accelAddress, 0x20, 0x0F));
  
  offset_x=(((int)readRegisterI2C(accelAddress, 0x38)<<4)|(readRegisterI2C(accelAddress, 0x35)>>4))-2048; //Serial.print("offset_x="); Serial.println(offset_x);
  gain_x=(readRegisterI2C(accelAddress, 0x32)>>1)-64; enable_sensor_interrupts();// Serial.print("gain_x="); Serial.println(gain_x);
  
  gain_x=0;
  disable_sensor_interrupts(); updateRegisterI2C(accelAddress, 0x32, 0x80+(gain_x<<1), 0xFE);
  gain_x=(readRegisterI2C(accelAddress, 0x32)>>1)-64; enable_sensor_interrupts(); Serial.print("\ngain_x="); Serial.println(gain_x);

/*  for(int i=-100;i<100;i+=2)
  {
    offset_x=i;
    disable_sensor_interrupts(); updateRegisterI2C(accelAddress, 0x38, 0x80+(offset_x>>4)); updateRegisterI2C(accelAddress, 0x35, (offset_x<<4)-((offset_x>>4)<<8), 0xF0);
    offset_x=(((int)readRegisterI2C(accelAddress, 0x38)<<4)|(readRegisterI2C(accelAddress, 0x35)>>4))-2048; enable_sensor_interrupts(); Serial.print(offset_x); Serial.print(", ");
    //delay(5000);
    for(accel_done=false; accel_done!=true;);
    point[1][0]=accelBuf[0]/ACCELCNT; point[1][1]=accelBuf[1]/ACCELCNT; point[1][2]=accelBuf[2]/ACCELCNT;
    Serial.println(point[1][0]);
  }
*/
/*  gain_x=20;
  disable_sensor_interrupts(); updateRegisterI2C(accelAddress, 0x32, 0x80+(gain_x<<1), 0xFE);
  gain_x=(readRegisterI2C(accelAddress, 0x32)>>1)-64; enable_sensor_interrupts(); Serial.print("\ngain_x="); Serial.println(gain_x);

  for(int i=-100;i<100;i+=2)
  {
    offset_x=i;
    disable_sensor_interrupts(); updateRegisterI2C(accelAddress, 0x38, 0x80+(offset_x>>4)); updateRegisterI2C(accelAddress, 0x35, (offset_x<<4)-((offset_x>>4)<<8), 0xF0);
    offset_x=(((int)readRegisterI2C(accelAddress, 0x38)<<4)|(readRegisterI2C(accelAddress, 0x35)>>4))-2048; enable_sensor_interrupts(); Serial.print(offset_x); Serial.print(", ");
    //delay(5000);
    for(accel_done=false; accel_done!=true;);
    point[1][0]=accelBuf[0]/ACCELCNT; point[1][1]=accelBuf[1]/ACCELCNT; point[1][2]=accelBuf[2]/ACCELCNT;
    Serial.println(point[1][0]);
  }

  gain_x=-20;
  disable_sensor_interrupts(); updateRegisterI2C(accelAddress, 0x32, 0x80+(gain_x<<1), 0xFE);
  gain_x=(readRegisterI2C(accelAddress, 0x32)>>1)-64; enable_sensor_interrupts(); Serial.print("\ngain_x="); Serial.println(gain_x);

  for(int i=-100;i<100;i+=2)
  {
    offset_x=i;
    disable_sensor_interrupts(); updateRegisterI2C(accelAddress, 0x38, 0x80+(offset_x>>4)); updateRegisterI2C(accelAddress, 0x35, (offset_x<<4)-((offset_x>>4)<<8), 0xF0);
    offset_x=(((int)readRegisterI2C(accelAddress, 0x38)<<4)|(readRegisterI2C(accelAddress, 0x35)>>4))-2048; enable_sensor_interrupts(); Serial.print(offset_x); Serial.print(", ");
    //delay(5000);
    for(accel_done=false; accel_done!=true;);
    point[1][0]=accelBuf[0]/ACCELCNT; point[1][1]=accelBuf[1]/ACCELCNT; point[1][2]=accelBuf[2]/ACCELCNT;
    Serial.println(point[1][0]);
  }*/
/*
  gain_x=50;
  disable_sensor_interrupts(); updateRegisterI2C(accelAddress, 0x32, 0x80+(gain_x<<1), 0xFE);
  gain_x=(readRegisterI2C(accelAddress, 0x32)>>1)-64; enable_sensor_interrupts(); Serial.print("\ngain_x="); Serial.println(gain_x);

  for(int i=-100;i<0;i+=2)
  {
    offset_x=i;
    disable_sensor_interrupts(); updateRegisterI2C(accelAddress, 0x38, 0x80+(offset_x>>4)); updateRegisterI2C(accelAddress, 0x35, (offset_x<<4)-((offset_x>>4)<<8), 0xF0);
    offset_x=(((int)readRegisterI2C(accelAddress, 0x38)<<4)|(readRegisterI2C(accelAddress, 0x35)>>4))-2048; enable_sensor_interrupts(); Serial.print(offset_x); Serial.print(", ");
    //delay(5000);
    for(accel_done=false; accel_done!=true;);
    point[1][0]=accelBuf[0]/ACCELCNT; point[1][1]=accelBuf[1]/ACCELCNT; point[1][2]=accelBuf[2]/ACCELCNT;
    Serial.println(point[1][0]);
  }
*/ /*
  gain_x=50;
  disable_sensor_interrupts(); updateRegisterI2C(accelAddress, 0x32, 0x80+(gain_x<<1), 0xFE);
  gain_x=(readRegisterI2C(accelAddress, 0x32)>>1)-64; enable_sensor_interrupts(); Serial.print("\ngain_x="); Serial.println(gain_x);

  for(int i=-100;i<100;i+=2)
  {
    offset_x=i;
    disable_sensor_interrupts(); updateRegisterI2C(accelAddress, 0x38, 0x80+(offset_x>>4)); updateRegisterI2C(accelAddress, 0x35, (offset_x<<4)-((offset_x>>4)<<8), 0xF0);
    offset_x=(((int)readRegisterI2C(accelAddress, 0x38)<<4)|(readRegisterI2C(accelAddress, 0x35)>>4))-2048; enable_sensor_interrupts(); Serial.print(offset_x); Serial.print(", ");
    //delay(5000);
    for(accel_done=false; accel_done!=true;);
    point[1][0]=accelBuf[0]/ACCELCNT; point[1][1]=accelBuf[1]/ACCELCNT; point[1][2]=accelBuf[2]/ACCELCNT;
    Serial.println(point[1][0]);
  }
*/
  offset_x=0;
  disable_sensor_interrupts(); updateRegisterI2C(accelAddress, 0x38, 0x80+(offset_x>>4)); updateRegisterI2C(accelAddress, 0x35, (offset_x<<4)-((offset_x>>4)<<8), 0xF0);
  offset_x=(((int)readRegisterI2C(accelAddress, 0x38)<<4)|(readRegisterI2C(accelAddress, 0x35)>>4))-2048; enable_sensor_interrupts(); Serial.print("offset_x="); Serial.println(offset_x);

/*  for(byte i=0;i<5;i++)
  {
    Serial.print("Waiting for position "); Serial.print(i+1); Serial.println("...");
    delay(5000);
    for(accel_done=false; accel_done!=true;);
    for(byte j=0;j<3;j++)
      point[i][j]=accelBuf[j]/ACCELCNT;
    printpoint(point[i]);
  }
  Serial.println("Done. Thanks. Now calculating...");
*/

point[0][0]=-2048;
point[0][1]=0;
point[0][2]=0;

point[1][0]=0;
point[1][1]=2048;
point[1][2]=0;

point[2][0]=0;
point[2][1]=0;
point[2][2]=2048;

point[3][0]=0;
point[3][1]=-2048;
point[3][2]=0;

point[4][0]=2048;
point[4][1]=0;
point[4][2]=0;

  if(findGain(point[0], point[1], point[2], point[3], point[4], x, 4096)) // find gain to be applied for the 4 points to be on a sphere
    {Serial.println("Error: Non-singular matrix. Possible reasons: programmer was drunk. Please try again.");
    return;
    }

  if(findCenter(point[0], point[1], point[2], point[3], x)) // find center of circumscribed sphere defined by four points
    {Serial.println("Error: The measured points are on the same plane. Possible reasons: measured positions are too close, sensor reading are wrong, sensor was moving during calibration sequence. Please try again.");
    return;
    }
  printpoint(x);
  
  for(int i=0;i<5;i++)
  {
    for(int j=0;j<3;j++)
      point[i][j]-=x[j];
    Serial.println("-");
    printpoint(point[i]);
    Serial.println(sqrt(point[i][0]*point[i][0]+point[i][1]*point[i][1]+point[i][2]*point[i][2]));
  }
  
  
  attachInterrupt(4, accel_int, RISING);
}

void ac(void)
{
  long int x,y,z;
  x=accelADC[0];
  y=accelADC[1];
  z=accelADC[2];
  Serial.println(sqrt(x*x + y*y + z*z));
}
