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
  Wire.begin();
  TWBR = 12;
  state_init();

  Serial.println("OK");

//accel_calibrate_manual();

  while(gyro_time==0 || accel_time==0)
    continue;
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
  
  delay(10);

  check_cmd(); //to be rewritten using serialEvent()

  if(!(i++%10))
  {
    j++;

    ax=state.ax;
    ay=state.ay;
    az=state.az;
    t1=state.tmp1;
    t2=state.tmp2;
    t3=state.tmp3;
    t4=state.tmp4;
    t5=state.tmp5;
    tm=state.tmp;
    qt=state.q;
//    f=getangle(qt.x);

  if(qt.x==0&&qt.y==0&&qt.z==0&&qt.w==0) Serial.println(" "); //Just making sure the quaternion is calculated

    Serial.println("----------------");
    print("qt",qt);
    print("Nq",norm(qt));
    print("Mx",Mx);
    print("My",My);
    print("Mx",Mz);
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
//  print("z1",state.z1);
//  print("z2",state.z2);
//  print("z3",state.z3);
  print("ax",ax);
  print("ay",ay);
  print("az",az);
/*  print("vx",state.vx);
  print("vy",state.vy);
  print("vz",state.vz);
  print(" x",state.x);
  print(" y",state.y);
  print(" z",state.z);
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

void print(fixed val)
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
  print(val.x);
  Serial.print(",");
  print(val.y);
  Serial.print(",");
  print(val.z);
  Serial.print(",");
  print(val.w);
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

