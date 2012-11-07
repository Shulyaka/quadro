#include "quadro.h"

void cmd_ping(void)
{
  Serial.println("Pong.");
}

void cmd_gyro(void)
{
  Serial.print(gyroADC[0]);
  Serial.print("     ");
  Serial.print(gyroADC[1]);
  Serial.print("     ");
  Serial.println(gyroADC[2]);
}

void cmd_accel(void)
{
  Serial.print(accelADC[0]);
  Serial.print("     ");
  Serial.print(accelADC[1]);
  Serial.print("     ");
  Serial.println(accelADC[2]);
//  accel_measure();
}

void cmd_lamp(int state)
{
  digitalWrite(LampPin, state);
  Serial.print("The lamp is in state ");
  Serial.print(state);
  Serial.print(" now.\n");
}

void cmd_zero(void)
{
  state.cosp=hcos((angle)0);
  state.sinp=hsin((angle)0);
  state.cosf=hcos((angle)0);
  state.sinf=hsin((angle)0);
  state.cost=hcos((angle)0);
  state.sint=hsin((angle)0);
  gyroalpha=0;
  gyrobeta=0;
  gyrogamma=0;
  state.ax=0;
  state.ay=0;
  state.az=0;
  state.vx=0;
  state.vy=0;
  state.vz=0;
  state.x=0;
  state.y=0;
  state.z=0;
}

void cmd_temp(void)
{
  Serial.print("The temperature is ");
  Serial.print(analogRead(TempPin));
  Serial.print(" degrees.\n");
}

void cmd_preas(void)
{
  Serial.print("The atmosphere preassure is ");
  Serial.print(analogRead(PreasPin));
  Serial.print(" pascal.\n");
}

void cmd_humid(void)
{
  Serial.print("The atmosphere humidity is ");
  Serial.print(analogRead(HumidPin));
  Serial.print(" mm.\n");
}

void cmd_debug(int state)
{
  debug=state;
  Serial.print("The debug flag is ");
  Serial.print(state);
  Serial.print(" now.\n");
}

void cmd_seta(int a)
{
  gyroalpha=a;
  Serial.print("Gyro alpha is ");
  Serial.print(a);
  Serial.print(".\n");
  disable_sensor_interrupts();
}

void cmd_setb(int b)
{
  gyrobeta=b;
  Serial.print("Gyro beta is ");
  Serial.print(b);
  Serial.print(".\n");
  enable_sensor_interrupts();
}

void cmd_setc(int c)
{
  gyrogamma=c;
  Serial.print("Gyro gamma is ");
  Serial.print(c);
  Serial.print(".\n");
}
