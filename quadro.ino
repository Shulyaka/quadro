//#define ACFIXED
#include <Wire.h>
#include "declarations.h"

void setup(void)
{
  pinMode(AccelLEDPin, OUTPUT);
  pinMode(GyroLEDPin, OUTPUT);
  pinMode(StatusLEDPin, OUTPUT);
  pinMode(GyroIntPin, INPUT);
  pinMode(AccelIntPin, INPUT);
  pinMode(BattMonPin, INPUT);
  pinMode(MotorPin[0], OUTPUT);
  pinMode(MotorPin[1], OUTPUT);
  pinMode(MotorPin[2], OUTPUT);
  pinMode(MotorPin[3], OUTPUT);
  Serial.begin(9600);
  motor_init();
  Serial2.begin(125200);
  Serial.print("AT+NAMEp01quadro");
  delay(2000);
  Serial.print("AT+BAUD7");
  delay(10000);
  Serial.print("+++");
  delay(1500);
  Serial.println("ATBD6");
  delay(500);
  Serial.print("ATDH");
  Serial.println(XBEE_DH);
  delay(500);
  Serial.print("ATDL");
  Serial.println(XBEE_DL);
  delay(500);
  Serial.println("ATWR");
  delay(500);
  Serial.println("ATCN");
  delay(1000);
  Serial.flush();
  Serial.begin(57600);

  Serial.println("\nInitializing... ");
  digitalWrite(StatusLEDPin, HIGH);
//  clear_cmdBuf();
  Wire.begin();
  TWBR = 12;
  imu_init();
  while(!gyro_ready || !accel_ready)
    continue;
  Serial.println("OK");

#ifdef ACCELCALIBRATE
  accel_calibrate_manual();
#endif
//accel_calibrate_manual_2();
//accel_calibrate_manual_2();
//accel_calibrate_manual_2();
//accel_calibrate_manual_2();
//accel_calibrate_manual_2();
//calibrate_orientation();
  Serial.print("Gyro time is ");
  Serial.println(gyro_time);
  Serial.print("Accel time is ");
  Serial.println(accel_time);
}

void loop(void)
{
  static unsigned int i=0;
  static fixed az=imu.az;
  static quaternion tmpq=conjugate(imu.q);
  static fixed takeoff_speed=0;
  fixed cosg;
  quaternion imu_q, imu_angv;
  fixed imu_x, imu_y, imu_z, imu_vx, imu_vy, imu_vz, imu_az;
  
  disable_sensor_interrupts();
  imu_q=imu.q;
  imu_angv=imu.angv;
  imu_x=imu.x;
  imu_y=imu.y;
  imu_z=imu.z;
  imu_vx=imu.vx;
  imu_vy=imu.vy;
  imu_vz=imu.vz;
  imu_az=imu.az;
  enable_sensor_interrupts();
//if(i==500) flight_state=FSTATE_TAKEOFF; //auto take off

  switch(flight_state)
  {
    case FSTATE_IDLE:
      az=imu.az;
      tmpq=conjugate(imu_q);
      //print("tmpq", tmpq);
      //print("angv", imu_angv);
      delay(50);
      break;
    case FSTATE_TAKEOFF:
      if((imu_az>az+0x28F5C23) || (abs((imu_q*tmpq).w)<2145336164L) || manual_takeoff) //takeoff condition: last known idle z acceleration plus 0.02 to be above noise or the real part of the mismatch quaternion is less than 0.999
      {
        if(debug) Serial.println("Flying");
        print("az",imu_az-az);
        print("w",abs((imu_q*tmpq).w)-2145336164L);
        print("tmp", imu_q*tmpq);
        print("tmpq", tmpq);
        print("imuq", imu_q);
        for(byte k=0; k<4; k++)
          MotorAdjust[k]=takeoff_speed-0x400000-gravity;
        takeoff_speed=0;
        manual_takeoff=false;
        flight_state=FSTATE_FLY;
        break;
      }
      else
        for(byte k=0; k<4; k++)
          MotorAdjust[k]=0;
      setMotorSpeed(takeoff_speed);
      if(debug) print("takeoff_speed", takeoff_speed);
      takeoff_speed=takeoff_speed+0x400000; //  1/128
      if (takeoff_speed<0)
      {
        takeoff_speed=0;
        stopAllMotors();
        flight_state=FSTATE_IDLE; //failure to launch
      }
      delayMicroseconds(accel_time); //making sure that we wait enough
      break;
    case FSTATE_LAND:
      if (MotorSpeed[0]<0 && MotorSpeed[1]<0 && MotorSpeed[2]<0 && MotorSpeed[3]<0) //landing condition
      {
        if(debug) Serial.println("On the ground");
        stopAllMotors();
        flight_state=FSTATE_IDLE;
        break;
      }
      if(MotorSpeed[0]>0)
        setMotorSpeed(0, MotorSpeed[0]-0x1000000); //   1/128
      if(MotorSpeed[1]>0)
        setMotorSpeed(1, MotorSpeed[1]-0x1000000);
      if(MotorSpeed[2]>0)
        setMotorSpeed(2, MotorSpeed[2]-0x1000000);
      if(MotorSpeed[3]>0)
        setMotorSpeed(3, MotorSpeed[3]-0x1000000);
      delay(20);
      break;
    case FSTATE_FLY:
        //print("idle",imu.q*q_idle);
        cosg=imu_q.x*imu_q.x+imu_q.y*imu_q.y;
        cosg=one-cosg-cosg;
        if(cosg<0)
        {
          az=0; //we are upside-down, no acceleration
          for(byte k=0; k<3; k++)
            if(abs(M[k])>az)
              az=abs(M[k]); //or at least minimum to get out of this state
        }
        else if(control_az>=cosg)
          az=one; //we are almost 90 degree rotated, not enough to hold altitude but do what we can with full acceleration
        else
          az=control_az%one/cosg;   //normal operation

        control_ax=horizontal_distance_factor*(desired_x-imu_x)-horizontal_speed_factor*imu_vx;
        control_ay=horizontal_distance_factor*(desired_y-imu_y)-horizontal_speed_factor*imu_vy;
        control_az=gravity+vertical_distance_factor*(desired_z-imu_z)-vertical_speed_factor*imu_vz;

        tmpq=imu_control(desired_q);
        
        disable_sensor_interrupts();  //we have to be sure that a gyro interrupt does not occur in the middle of copying
        MotorAcceleration=gravity;//control_az;//az;
        control_q=ident;//tmpq;
        enable_sensor_interrupts();
        
        // put the main flight control logic here
        break;

    default:
      error("State not used");
  }



//  if(!(i++%10))
//  {
//    if(imu.q.w==0&&imu.q.x==0&&imu.q.y==0&&imu.q.z==0) Serial.println(" "); //Just making sure the quaternion is calculated
    print_debug_info();
//  }
}

void print_debug_info(void)
{
    fixed ax, ay, az;
    fixed t1,t2,t3,t4,t5;
    lfixed tm;
//    angle f,p,t;
    quaternion qt;
    quaternion heading;
    int battery=analogRead(BattMonPin);
    
    static unsigned int j=0;
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
    qt=imu_get_orientation();
//    f=getangle(qt.x);

    heading=qt;
    heading.x=0;
    heading.y=0;
    heading.normalize();
    
    Serial.println("----------------");
    print("imu.q", qt);
    print("heading", heading);
    print("pitch", qt*conjugate(heading));
//    print("Nq",norm(qt));
//    print("qd",control_q);
//    print("mi",qt*conjugate(control_q));
    print("Mx",M[0]);
    print("My",M[1]);
    print("Mz",M[2]);
//    print("t1",t1);
//    print("t2",t2);
//    print("t3",t3);
//    print("t4",t4);
//    print("t5",t5);
    print("angv.x",imu.angv.x);
    print("angv.y",imu.angv.y);
    print("angv.z",imu.angv.z);
    
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
//  print("ax",ax);
//  print("ay",ay);
//  print("az",az);
//  print("vx",imu.vx);
//  print("vy",imu.vy);
//  print("vz",imu.vz);
//  print(" x",imu.x);
//  print(" y",imu.y);
//  print(" z",imu.z);

//  print("MotorAcceleration",MotorAcceleration);

//  print("MotorAdjust0", MotorAdjust[0]);
//  print("MotorAdjust1", MotorAdjust[1]);
//  print("MotorAdjust2", MotorAdjust[2]);
//  print("MotorAdjust3", MotorAdjust[3]);

  print("Motor0", MotorSpeed[0]);
  print("Motor1", MotorSpeed[1]);
  print("Motor2", MotorSpeed[2]);
  print("Motor3", MotorSpeed[3]);
  print("battery", battery);
  if(battery < 690)
    Serial.println("Warning! Battery is too low!");
  // 304:  3.95V
  // 815: 11.86V
  // 809: 11.80V
  //0.01456

//  cmd_gyro();
//  Serial.println(accel_time);  
}

void error (const char *msg)
{
  Serial.print("Error: ");
  Serial.println(msg);
}

void print(fixed val)
{
  int v;
  if(val==one)
    Serial.print(" 1.00 ( one )");
  else
  {
    if(val==-one)
      Serial.print("-1.00 (");
    else
    {
      if(val>=0)
        Serial.print(" 0.");
      else
        Serial.print("-0.");
      v=abs(val.value)/0x20C49C;
      if(v<10)
        Serial.print("00");
      else if(v<100)
        Serial.print("0");
      Serial.print(v);
    }
    Serial.print(" (");
    if(val>=0) Serial.print(" ");
    Serial.print(val.value);
    Serial.print(" )");
  }
}

void print(const char *name, fixed val)
{
  Serial.print(name);
  Serial.print(" =");
  print(val);
  Serial.println("");
}
void print(const char *name, angle val)
{
  Serial.print(name);
  Serial.print(" =");
  if(val>=0) Serial.print(" ");
  Serial.print(val/115);
  Serial.print(" (");
  if(val>=0) Serial.print(" ");
  Serial.print(val);
  Serial.println(" )");
}

void print(const char *name, lfixed val)
{
  unsigned long long a=val.value;
  int v;
  Serial.print(name);
  Serial.print(" =");
  if(val>=0) Serial.print(" ");
  Serial.print((int)(val.value/0x4000000000000000LL));
  Serial.print(".");
  v=abs(val.value)/0x10624DD2F1A9FCLL - (abs(val.value)/0x4000000000000000LL)*1000;
  if(v<10)
    Serial.print("00");
  else if(v<100)
    Serial.print("0");
  Serial.print(v);
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

void disable_sensor_interrupts() //we call this to prevent sudden I2C traffic caused by sensor interrupts in the middle of transfer but we do not want to disable interrupts completely because I2C functionality relies on them
{
  EIMSK &= ~(1 << GyroIntNumAtmega);
  EIMSK &= ~(1 << AccelIntNumAtmega);
}

void enable_sensor_interrupts() //warning: only use when all interrupt vectors are set correctly
{
  EIMSK |= (1 << GyroIntNumAtmega);
  EIMSK |= (1 << AccelIntNumAtmega);
}

