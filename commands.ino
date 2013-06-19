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
  if(flight_state!=FSTATE_IDLE)
  {
    error("The zero command is disabled in non-idle mode");
    return;
  }
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
//  imu.q=ident;
//  imu.qg=conjugate(gyro_orientation);
//  control_az=gravity;
  imu_init_orientation();
}

void cmd_takeoff(void)
{
  quaternion imu_q=imu_get_orientation();
  Serial.println("Taking off");
  manual_takeoff=false;
  desired_z=0xF0000L;
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
  accel_offset[0]=(long)x*10000L;
  print("accel_offset[0]", accel_offset[0]);
  imu_init_orientation();
}

void cmd_usr2(void)
{
  desired_z=desired_z-0x100000L;
  Serial.println("Descending");
}

void cmd_usr3(void)
{
  Serial.println("Manual takeoff");
  manual_takeoff=true;
}

void cmd_usr4(int x)
{
  accel_offset[1]=(long)x*10000L;
  print("accel_offset[1]", accel_offset[1]);
  imu_init_orientation();
}

void cmd_usr5(void)
{
  desired_z=desired_z+0x100000L;
  Serial.println("Ascending");
}

void cmd_incr(void)
{
  orientation_distance_factor=orientation_distance_factor<<1;
  print("orientation_distance_factor", orientation_distance_factor);
}

void cmd_decr(void)
{
  orientation_distance_factor=orientation_distance_factor>>1;
  print("orientation_distance_factor", orientation_distance_factor);
}

int cmd_RU(fixed *x) //Range Update
{
  if(*x < 0)
  {
    print("RA", *x);
    error("range A < 0");
    return -1;
  }

  sonara=*x;

  Serial.print("range A = ");
  Serial.print(sonara.value>>15);
  Serial.print(" [");
  print(sonara);
  Serial.println("]");
  
  return 0;
}

int cmd_WU(fixed *x) //Range Update
{
  if(*x < 0)
  {
    print("RB", *x);
    error("range B < 0");
    return -1;
  }

  sonarb=*x;

  Serial.print("range B = ");
  Serial.print(sonarb.value>>15);
  Serial.print(" [");
  print(sonarb);
  Serial.println("]");

  return 0;
}

int cmd_LU(fixed *x, fixed *y) //Landing Update
{
  fixed a=*x;
  fixed b=*y;
  print("Landing pad X", a);
  print("Landing pad Y", b);
  return 0;
}


