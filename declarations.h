#ifndef DECLARATIONS_H
#define DECLARATIONS_H
                                                   //TODO:  to rename all this mess to something more logical
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
#define XBEE_DH "13A200"
#define XBEE_DL "40983FF3"

//#define XBEE_DL "0000FFFF"
//#define XBEE_DH "00000000"

const unsigned char MotorPin[]={2, 3, 5, 6};

// We use the following motor configuration: http://aeroquad.com/showwiki.php?title=Images:Quad+X.jpg

bool debug=true;

//#define FINDZERO 128
#define GYROCNTP 10L  //this value should not be too high because we calibrate the sensor using the shortest arc
#define GYROSTEPS 3L //the number of calibration iterations
#define ACCELCNT 4096
#define ACCELCNTP 9 //to replace the above one

fixed M[3]={0};

const fixed gravity=0x40000000; //   1/2
const fixed sinpi4 = 1518500250L;

fixed MotorAdjust[4]={0};
fixed MotorSpeed[4]={0};
fixed MotorAcceleration=gravity;

//defines current instantaneous imu parameters
typedef struct {
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
  quaternion qg;  //actual orientation in the gyro axis
  quaternion angv; //angual velocity
//  quaternion qd; //desired temporary orientation
  quaternion cqs; //short-term calibration quaternion
  quaternion cql; //long-term calibration quaternion
  quaternion heading; //quaternion representing the nearest horizontal orientation
} Imu;

Imu imu;

//defines current desired waypoint
quaternion desired_q=ident; //only heading is currently supported
fixed desired_x=0;
fixed desired_y=0;
fixed desired_z=0xF0000L;
fixed desired_vx=0; //not supported yet
fixed desired_vy=0; //not supported yet
fixed desired_vz=0; //not supported yet

fixed sonara=0;
fixed sonarb=0;

//defines the acceleration needed to achieve the waypoint state
fixed control_ax=0;
fixed control_ay=0;
fixed control_az=gravity;
quaternion control_q=ident;
quaternion control_heading=ident;

//the following values adjust control parameters, subject to calibrate:
const fixed orientation_distance_factor=one>>1;
const fixed orientation_speed_factor=one>>1;
const fixed orientation_distance_factor_z=one>>1;
const fixed orientation_speed_factor_z=one>>1;

const fixed vertical_distance_factor=one;//>>1;
const fixed vertical_speed_factor=0;//one>>1;
const fixed horizontal_distance_factor=one;//one>>1;
const fixed horizontal_speed_factor=0;//one>>1;

//sensor calibration parameters             -- TODO: to store them in EEPROM
quaternion gyro_orientation=quaternion(2147277277, -26095309, -13909796, -3444716);//ident;   //gyro misalignment compensation               //const
quaternion accel_orientation=ident;   //accel misalignment compensation
/*
fixed accel_offset[3]={-2000000, -9900000, -2000000};
fixed accel_gain[3]={-6000000, -1800000, 1000000};
*/
/*fixed accel_offset[3]={2150000, -750000, 1950000};//1200000};
fixed accel_gain[3]={-3420000, -2850000, -5200000};
*/
fixed accel_offset[3]={-100000000, 10000000, 1950000};
fixed accel_gain[3]={-3420000, -2850000, -5200000};
fixed accel_square[3]={0, 0, 0};


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

bool manual_takeoff=false;

#endif
