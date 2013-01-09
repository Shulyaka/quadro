#include <Wire.h>
#include "declarations.h"

void setup(void)
{
  Serial.begin(9600);
  Serial.print("AT+NAMEp01quadro");
  delay(2000);
  Serial.print("AT+BAUD8");
  delay(2000);
  Serial.flush();
  Serial.begin(115200);
  Serial.println("\nInitializing... ");
  clear_cmdBuf();
  pinMode(LampPin, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(31, OUTPUT);
  pinMode(18, INPUT);
  pinMode(19, INPUT);
  pinMode(Motor0Pin, OUTPUT);
  pinMode(Motor1Pin, OUTPUT);
  pinMode(Motor2Pin, OUTPUT);
  pinMode(Motor3Pin, OUTPUT);
  Wire.begin();
  TWBR = 12;
  imu_init();
  motor_init();

  Serial.println("OK");

//accel_calibrate_manual();

  while(gyro_time==0 || accel_time==0)
    continue;
  Serial.print("Gyro time is ");
  Serial.println(gyro_time);
  Serial.print("Accel time is ");
  Serial.println(accel_time);
}

void loop(void)
{
  static unsigned int i=0;
  static unsigned int j=0;
  fixed t1,t2,t3,t4,t5;
  fixed ax, ay, az;
  lfixed tm;
//  angle f,p,t;
  quaternion qt;
  static fixed az_idle=imu.az;
  static signed char takeoff_speed=0;
  
  delay(10);

  check_cmd(); //to be rewritten using serialEvent() //or may be not...

  switch(flight_state)
  {
    case FSTATE_IDLE:
      az_idle=imu.az;
      break;
    case FSTATE_TAKEOFF:
      if(imu.az>az_idle+42949673ULL) //takeoff condition: last known idle z acceleration plus 0.02 to be above noise
      {
        if(debug) Serial.println("Flying");
        Motor0Zero=takeoff_speed;
        Motor1Zero=takeoff_speed;
        Motor2Zero=takeoff_speed;
        Motor3Zero=takeoff_speed;
        takeoff_speed=0;
        flight_state=FSTATE_FLY;
        break;
      }
      motor0(takeoff_speed);
      motor1(takeoff_speed);
      motor2(takeoff_speed);
      motor3(takeoff_speed);
      if(debug)Serial.println(takeoff_speed);
      takeoff_speed++;
      if (takeoff_speed<0)
      {
        takeoff_speed=0;
        flight_state=FSTATE_IDLE;
      }
      delayMicroseconds(accel_time); //making sure that we wait enough
      break;
    case FSTATE_FLY:
        // put the flight control logic here
        break;

    default:
      error("State not used");
  }



  if(!(i++%10))
  {
    j++;

    ax=imu.ax;
    ay=imu.ay;
    az=imu.az;
    t1=imu.tmp1;
    t2=imu.tmp2;
    t3=imu.tmp3;
    t4=imu.tmp4;
    t5=imu.tmp5;
    tm=imu.tmp;
    qt=imu.q;
//    f=getangle(qt.x);

  if(qt.w==0&&qt.x==0&&qt.y==0&&qt.z==0) Serial.println(" "); //Just making sure the quaternion is calculated

    Serial.println("----------------");
    print("qt",qt);
    print("Nq",norm(qt));
    print("qd",imu.qd);
    print("mi",qt*conjugate(imu.qd));
    print("Mx",Mx);
    print("My",My);
    print("Mz",Mz);
//    print("t1",t1);
//    print("t2",t2);
//    print("t3",t3);
//    print("t4",t4);
//    print("t5",t5);
    
//    print(" s",t1%t1+t2%t2+t3%t3);
//    print("ss",lsqrt(t1%t1+t2%t2+t3%t3));
//    print("v1",lvectlen(t1,t2,t3));
//    print("v2",vectlen(t1,t2,t3));
//    print("tm",tm);
//    print("tt",tofixed(tm));
//    print("tn",t1%cc+t2%sc);
//  print("z1",imu.z1);
//  print("z2",imu.z2);
//  print("z3",imu.z3);
  print("ax",ax);
  print("ay",ay);
  print("az",az);
/*  print("vx",imu.vx);
  print("vy",imu.vy);
  print("vz",imu.vz);
  print(" x",imu.x);
  print(" y",imu.y);
  print(" z",imu.z);
*/
  cmd_gyro();
  Serial.println(accel_time);
  }
}

void error (const char *msg)
{
  Serial.print("Error: ");
  Serial.println(msg);
}

void print(fixed val)  //to be rewritten using fixed-point math
{
  if(val!=one)
  {
    if(val>=0) Serial.print(" ");
    Serial.print((double)val.value/65536/32768);
    Serial.print(" (");
    if(val>=0) Serial.print(" ");
    Serial.print(val.value);
    Serial.print(" )");
  }
  else
    Serial.print(" 1.00 ( one )");
}

void print(const char *name, fixed val)
{
  Serial.print(name);
  Serial.print(" = ");
  if(val!=one)
  {
    if(val>=0) Serial.print(" ");
    Serial.print((double)val.value/65536/32768);
    Serial.print(" (");
    if(val>=0) Serial.print(" ");
    Serial.print(val.value);
    Serial.println(" )");
    //Serial.println(val.value,BIN);
  }
  else
    Serial.println(" 1.00 ( one )");
}
void print(const char *name, angle val)
{
  Serial.print(name);
  Serial.print(" = ");
  if(val>=0) Serial.print(" ");
  Serial.print((double)val/115);
  Serial.print(" (");
  if(val>=0) Serial.print(" ");
  Serial.print(val);
  Serial.println(" )");
}

void print(const char *name, lfixed val)
{
  unsigned long long a=val.value;
  Serial.print(name);
  Serial.print(" = ");
  if(val>=0) Serial.print(" ");
    Serial.print((double)val.value/65536/32768/65536/32768);
    Serial.print(" (");
    for(byte i=63;i!=255;i--)
    {
      Serial.print((byte)(a>>i),BIN);
      a=a-((a>>i)<<i);
    }
    Serial.println(")");
}

void print(const char *name, quaternion val)
{
  Serial.print(name);
  Serial.print(" = [");
  print(val.w);
  Serial.print(",");
  print(val.x);
  Serial.print(",");
  print(val.y);
  Serial.print(",");
  print(val.z);
  Serial.println("]");
}

void dummy_int(void)
  {return;}

void disable_sensor_interrupts()
{
  EIMSK &= ~(1 << INT3); //gyro
  EIMSK &= ~(1 << INT2); //accel
}

void enable_sensor_interrupts() //warning: only use when all interrupt vectors are set correctly
{
  EIMSK |= (1 << INT3); //gyro
  EIMSK |= (1 << INT2); //accel
}

