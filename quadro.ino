#include <Wire.h>

void clear_cmdBuf(void);
void error(const char *);
void check_cmd(void);  // to be used inside loop() constantly.
void print_cmdBuf(void);
unsigned char parse_cmd(int *);
void cmd_ping(void);
void cmd_speed(void);
void cmd_accel(void);
void cmd_lamp(int);
void cmd_audio(int);
void cmd_temp(void);
void cmd_preas(void);
void cmd_humid(void);
void cmd_debug(void);

unsigned int get_speed(void); // returns current speed in km/h multiplied by 100. We avoid floating-point calculations on our CPU.
int get_accel(void); // returns current horizontal acceleration in m/s^2 multiplied by 100
//inline unsigned long get_spdBuf(unsigned char); // helper function to get timestamp from speedBuf
unsigned long get_spdBufDif(unsigned char, unsigned char,unsigned char); // helper function to get difference between two timestamps from speedBuf
void upd_spdBuf(void); // interrupt function
void clear_spdBuf(void);

//#include "mysin.h"
volatile unsigned long int gyro_time=0;
volatile unsigned long int accel_time=0;
#include "AQMath.h"
#include "quadro.h"
#include "Accel.h"
#include "Gyro.h"
#include "Imu.h"
#include "Motor.h"

//static int mysin[1293];
//int mycos[1293];
//int mytan[1293];

int interrupted;

void setup(void)
{
  unsigned long time1,time2;
  int x;
//  fixed a=26042;
//  fixed b=one;
  Serial.begin(9600);
  Serial.print("AT+NAMEp01quadro");
  delay(2000);
  Serial.print("AT+BAUD8");
  delay(2000);
  Serial.flush();
  Serial.begin(115200);
  Serial.println("\nInitializing... ");
  clear_cmdBuf();
  clear_spdBuf();
  pinMode(LampPin, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(31, OUTPUT);
  pinMode(18, INPUT);
  pinMode(19, INPUT);
  Wire.begin();
  TWBR = 12;
  state_init();
  interrupted=0;
//  attachInterrupt(5, intpt, RISING);
//  gyro_clear_int();

  Serial.println("OK");
  lfixed a=0x2000000000000000ULL;
  print("a",a);
  print("a*a",a*a);
//  Serial.print("gyro_time=");
//  Serial.println(gyro_time);
/*  lfixed a=56931263897486104ULL;
  lfixed b=569312638974861037ULL; //0.12345
  lfixed c=9223372035423120042LL;
  print("c",c);
  print("div1",a/b);
  print("div2",ldiv(a,b));
  print("sqrt0",sqrt(b));
  a=lsqrt(b);
  print("sqrt1",a);
//  Serial.print("a=");Serial.print((unsigned long)(a.value>>32),BIN);Serial.print(" ");Serial.println((unsigned long)(a.value-((a.value>>32)<<32)),BIN);
*/

//Serial.println((long)lpow(3,3));
//Serial.println((long)lcm(32,1));
//Serial.println((long)lcm(32,3));
//Serial.println((long)lcm(32,18));
//long long matrix[9]={ 2, 0, 0, \
              //        0, 0, 1, \
                //      1, 0, 1 };
//long long *ma[3]={&matrix[0], &matrix[3], &matrix[6]};
//long long res[3];
//printMatrix(ma);
//Serial.println(fss(ma,res));
//printMatrix(ma);
//Serial.println((long)res[0]);
//Serial.println((long)res[1]);
//Serial.println((long)res[2]);

//accel_calibrate_manual();
/*  disable_sensor_interrupts();
  
  updateRegisterI2C(accelAddress, 0x22, 0x03, 0x03);

  updateRegisterI2C(accelAddress, 0x0E, 0x80, 0x80);
  while (readRegisterI2C(accelAddress, 0x0E, 0x80))
    delay(10);
  Serial.println("Done");
  updateRegisterI2C(accelAddress, 0x0E, 0x40, 0x40);
  while (readRegisterI2C(accelAddress, 0x0E, 0x40))
    delay(10);
  Serial.println("Done\nPlease place the board vertically");
  delay(5000);
  Serial.println("Thanks");
  updateRegisterI2C(accelAddress, 0x0E, 0x20, 0x20);
  while (readRegisterI2C(accelAddress, 0x0E, 0x20))
    delay(10);
  Serial.println("Done");

  enable_sensor_interrupts();
  */
  while(gyro_time==0 || accel_time==0)
    continue;
}

unsigned int i=0;
int j=16328;
unsigned int icnt=0;
void loop(void)
{
  unsigned long time;
  double y;
  long x;
  //long long p;
  int z;
  fixed t1,t2,t3,t4,t5;
  fixed ax, ay, az;
  lfixed tm;
//  angle f,p,t;
  quaternion qt;
  long a,b;
  long long c;
  long *d;
  
  delay(10);
  //upd_state();
  check_cmd(); //to be rewritten using serialEvent()

  //TODO: An interrupt watchdog would be good 
  if(interrupted==2)
  {
    error("CPU Overload!");
    //gyro_clear_int();
  }

  if(!(i++%10))
  {
    //z=gyroADC[2];
    //time=micros();
    ////y=cos(z*0.001214142);
    //y=acos(gyroADC[0]*0.0001);
    //Serial.print(micros()-time);
    //Serial.print(" acos ");
    //Serial.println(state.tana);
    //x=lcos(z);
//    time=micros();
//    x=lsin(j);
//    Serial.print(micros()-time);
//    Serial.print(" lsin ");
    //Serial.print(state.tana); Serial.print(" tan "); Serial.println(state.tanb);
    //Serial.println(state.cosp);
    /*a=356948084;
    b=521226057;
    c=(long long)a*(long long)b;
    d=(long *)&c;
    Serial.println(*d,BIN);
    d=(long *)&c+1;
    Serial.println(*d,BIN);*/
//    Serial.print(gyroADC[0]);Serial.print(" - ");Serial.print(gyroADC[1]);Serial.print(" - ");Serial.println(gyroADC[2]);
    

    //Serial.print(j);Serial.print(":  ");Serial.println(lsin(j++));
//    Serial.print(j);Serial.print(":  ");Serial.print((double)j/115);Serial.print(" - ");Serial.print(((double)lsin(j))/65536/32768);Serial.print("  ");Serial.print(lsin(j));Serial.print(" / ");Serial.println((long)(sin((double)j*3.1415926535/20700)*65536*32768)-lsin(j));
//    if(j!=lacos(x)){Serial.print(j);Serial.print(":  ");Serial.print((double)j/115);Serial.print(" - ");Serial.print(x);Serial.print(" / ");Serial.println(lacos(x));
//    }else Serial.println(j);
    j++;
//    print("j",j);
//    print("cos j",cos((angle)j));
//    print("acos cos j",acos(cos((angle)j)));
//    if(j==100)
//    {
//      //digitalWrite(LampPin, 1);
//      cmd_setb(0);
//      state_init();
//    }
    //else
    //  Serial.println(j);
    //Serial.print(state.ax/1000000);  Serial.print("        ");  Serial.print(state.ay/1000000);  Serial.print("        ");  Serial.println(state.az/1000000);
    //ac();
    //prtval("mult1",mult(lcos(state.f),lsin(state.p)));
    //prtval("tana ",state.tana);
    //prtval("add  ",mult(mult(lcos(state.f),lsin(state.p)),state.tana));
    //prtval("cosp ",state.cosp);

    ax=state.ax;
    ay=state.ay;
    az=state.az;
    t1=state.tmp1;
    t2=state.tmp2;
    t3=state.tmp3;
    t4=state.tmp4;
    t5=state.tmp5;
    tm=state.tmp;
    //p=acos(cp);
    qt=state.q;
//    p=getangle(sp,cp);

if(qt.x==0&&qt.y==0&&qt.z==0&&qt.w==0) Serial.println(" "); //Just making sure quaternion is calculated

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

void intpt(void)
{
  digitalWrite(LampPin, 1);
  if(interrupted)
    {
      interrupted=2;
      return;
    }
  interrupted=1;
  icnt++;
  interrupts();

//  upd_state();

  interrupted=0;
  digitalWrite(LampPin, 0);
}

void error (const char *msg)
{
  Serial.print("Error: ");
  Serial.println(msg);
}

