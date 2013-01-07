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
  state.q.w=one;
  state.q.x=0;
  state.q.y=0;
  state.q.z=0;
}

void cmd_takeoff(void)
{
  Serial.println("Taking off");
  for(byte i=0; i<100; i++)
  {
    motor0(i);
    motor1(i);
    motor2(i);
    motor3(i);
    delay(50);
  }
}

void cmd_land(void)
{
  Serial.println("Landing");
  for(byte i=100; i!=255; i--)
  {
    motor0(i);
    motor1(i);
    motor2(i);
    motor3(i);
    delay(20);
  }
}

void cmd_emerg(void)
{
  Serial.println("Emergency landing!\nMight be dangerous and damaging!\nAaaaaaa!");
  motor0(0);
  motor1(0);
  motor2(0);
  motor3(0);
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

