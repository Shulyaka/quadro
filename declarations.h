#ifndef DECLARATIONS_H
#define DECLARATIONS_H

//#include <Arduino.h>
#include "fplib.h"
#include "quaternionlib.h"

#define LampPin    13 //to be removed
//#define AudioPin   12
//#define LightPin   A0
//#define TempPin    A1
//#define PreasPin   A2
//#define HumidPin   A3

bool debug=true;

#define FINDZERO 128
#define GYROCNTP 5
#define ACCELCNT 32

const unsigned char MotorPin[]={2, 3, 5, 6};

fixed Mx=0, My=0, Mz=0;

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
  quaternion qn; //next desired orientation
  fixed azd; //desired vertical acceleration
};

Imu imu;

typedef int angle;

angle gyroalpha=0;
angle gyrobeta=0;
angle gyrogamma=0;

volatile unsigned long int gyro_time=0;
volatile unsigned long int accel_time=0;

#define FSTATE_IDLE    0
#define FSTATE_TAKEOFF 1
#define FSTATE_LAND    2
#define FSTATE_FLY     3

unsigned char flight_state=FSTATE_IDLE;

#endif
