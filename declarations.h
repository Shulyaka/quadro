#ifndef QUADRO_H
#define QUADRO_H

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

typedef int angle;

//#include <Arduino.h>
#include "fplib.h"
#include "quaternionlib.h"

fixed Mx, My, Mz;
int accelADC[3];

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

void dummy_int(void)
  {return;}

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
angle gyroalpha=0;
angle gyrobeta=0;
angle gyrogamma=0;

#endif
