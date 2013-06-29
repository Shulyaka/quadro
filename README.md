quadro
================

Arduino Quadrocopter IMU Control System

To run this code you will need an Arduino Mega with AeroQuad Shield 2.0 (not in stock anymore) with
two additional wires for accel and gyro interrupts, so I guess you have no choice but to port the
code to your board having in mind that the code is heavily relies on interrupts, and only ITG-3200
gyro and BMA180 accel are currently supported. However you will find a lot of useful ideas in the code
such as fixed-point math library, advanced accel calibration algorythm (not finished yet, sorry) and
quaternion-based IMU implementation.
Requires TinyGPS Library (http://arduiniana.org/libraries/tinygps/).

The development is in not complete yet.
