void cmd_ping(void)
{
  Serial.println("Pong.");
}

int cmd_ping2(void)
{
  Serial2.println("Pong.");
  return 0;
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
}

void cmd_lamp(int state)
{
  digitalWrite(AccelLEDPin, state);
  Serial.print("The lamp is in state ");
  Serial.print(state);
  Serial.print(" now.\n");
}

void cmd_zero(void)
{
  gyroalpha=0;
  gyrobeta=0;
  gyrogamma=0;
  imu.ax=0;
  imu.ay=0;
  imu.az=0;
  imu.vx=0;
  imu.vy=0;
  imu.vz=0;
  imu.x=0;
  imu.y=0;
  imu.z=0;
  imu.q=ident;
  imu.qg=conjugate(gyro_orientation);
  control_az=gravity;
}

void cmd_takeoff(void)
{
  quaternion imu_q=imu_get_orientation();
  Serial.println("Taking off");
  manual_takeoff=false;
  if(abs((imu_q*conjugate(imu_q)).w)<2145336164L)
  {
    error("Flying condition is true. Takeoff disabled for safety reasons. Please check your quaternions!");
  }
  else
    flight_state=FSTATE_TAKEOFF;
}

void cmd_land(void)
{
  Serial.println("Landing");
  flight_state=FSTATE_LAND;
}

void cmd_emerg(void)
{
  Serial.println("Emergency landing!\nMight be dangerous and damaging!\nMayday Mayday Mayday!\nAaaaaaa!");
  flight_state=FSTATE_IDLE;
  stopAllMotors();
}

void cmd_debug(int flag)
{
  debug=flag;
  Serial.print("The debug flag is ");
  Serial.print(debug);
  Serial.print(" now.\n");
}

void cmd_seta(int a)
{
  gyroalpha=a;
  Serial.print("Gyro alpha is ");
  Serial.print(a);
  Serial.print(".\n");
}

void cmd_setb(int b)
{
  gyrobeta=b;
  Serial.print("Gyro beta is ");
  Serial.print(b);
  Serial.print(".\n");
}

void cmd_setc(int c)
{
  gyrogamma=c;
  Serial.print("Gyro gamma is ");
  Serial.print(c);
  Serial.print(".\n");
}

void cmd_usr1(int x)
{
  analogWrite(MotorPin[0], x);
}

void cmd_usr2(void)
{
  desired_z=desired_z-0x2000000;
  Serial.println("Descending");
}

void cmd_usr3(void)
{
  Serial.println("Manual takeoff");
  manual_takeoff=true;
}

int cmd_RU(fixed *x) //Range Update
{
  fixed z=*x;
  print("range A", z);
  return 0;
}

int cmd_LU(fixed *x, fixed *y) //Landing Update
{
  fixed a=*x;
  fixed b=*y;
  print("Landing plate X", a);
  print("Landing plate Y", b);
  return 0;
}


