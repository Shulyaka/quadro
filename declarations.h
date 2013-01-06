#ifndef DECLARATIONS_H
#define DECLARATIONS_H

//#include <Arduino.h>
#include "fplib.h"
#include "quaternionlib.h"

#define LampPin    13
#define AudioPin   12
#define LightPin   A0
#define TempPin    A1
#define PreasPin   A2
#define HumidPin   A3

bool debug=false;

#define FINDZERO 128
#define GYROCNTP 5
#define ACCELCNT 32

fixed Mx, My, Mz;

typedef struct State {
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
  quaternion q;
};

State state;

typedef int angle;

angle gyroalpha=0;
angle gyrobeta=0;
angle gyrogamma=0;

volatile unsigned long int gyro_time=0;
volatile unsigned long int accel_time=0;

#endif
