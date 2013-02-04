#ifndef DECLARATIONS_H
#define DECLARATIONS_H

//#include <Arduino.h>
#include "fplib.h"
#include "quaternionlib.h"

#define BattMonPin   0
#define GyroLEDPin   4
#define AccelLEDPin  13
#define StatusLEDPin 31
#define GyroIntPin   18
#define AccelIntPin  19
#define GyroIntNum   5
#define AccelIntNum  4
#define GyroIntNumAtmega  INT3 // must match *IntNum Arduino interrupt number
#define AccelIntNumAtmega INT2 // please see attachInterrupt() sources for the interrupt number assignment

const unsigned char MotorPin[]={2, 3, 5, 6};

// We use the following motor configuration: http://aeroquad.com/showwiki.php?title=Images:Quad+X.jpg

bool debug=true;

//#define FINDZERO 128
#define GYROCNTP 11
#define ACCELCNT 32

fixed M[3]={0};

const fixed gravity=0x40000000; //   1/2

fixed MotorAdjust[4]={0};
fixed MotorSpeed[4]={0};
fixed MotorAcceleration=gravity;

typedef struct Imu {
  fixed ax, ay, az;
  fixed vx, vy, vz;
  fixed x, y, z;
  fixed x1, x2, x3; //coordinates of "East"
  fixed y1, y2, y3; //coordinates of "North"
  fixed z1, z2, z3; //coordinates of "Top"
  fixed wx, wy, wz; //rotation speed in 'local' coordinats
  fixed cosa, cosb, cosc, sina, sinb, sinc;
//  fixed cosp, sinp;
//  fixed cosf, sinf;
//  fixed cost, sint;
  lfixed lcost, lsint;
  fixed tmp1, tmp2, tmp3, tmp4, tmp5;
  lfixed tmp;
  quaternion q;  //current orientation
  quaternion qd; //desired orientation
  quaternion cqs; //short-term calibration quaternion
  quaternion cql; //long-term calibration quaternion
  fixed azd; //desired vertical acceleration
};

Imu imu;

typedef int angle;

angle gyroalpha=0;
angle gyrobeta=0;
angle gyrogamma=0;

volatile unsigned long int gyro_time=0;
volatile unsigned long int accel_time=0;

volatile bool gyro_ready=false;
volatile bool accel_ready=false;

int gyro_interrupted=0;
int accel_interrupted=0;

#define FSTATE_IDLE    0
#define FSTATE_TAKEOFF 1
#define FSTATE_LAND    2
#define FSTATE_FLY     3

unsigned char flight_state=FSTATE_IDLE;

#endif
