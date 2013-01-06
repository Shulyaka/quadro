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

#endif
