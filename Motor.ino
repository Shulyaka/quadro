void motor_updateControl(void)
{
  fixed h=one>>1; //some constants
  fixed k=one>>1;
  fixed k2=k*sinpi4;
  quaternion m=state.q*conjugate(state.qd);

  if(abs(m.w)>sinpi4)
  {
    Mx=k*m.w*m.x-h*state.sina;
    My=k*m.w*m.y-h*state.sinb;
    Mz=k*m.w*m.z-h*state.sinc;
  }
  else
  {
    if(m.w>0)
    {
      Mx=k2*m.x-h*state.sina;
      My=k2*m.y-h*state.sinb;
      Mz=k2*m.z-h*state.sinc;
    }
    else
    {
      Mx=-k2*m.x-h*state.sina;
      My=-k2*m.y-h*state.sinb;
      Mz=-k2*m.z-h*state.sinc;
    }
  }


}

void motor0(unsigned char v)
{analogWrite(Motor0Pin, v);}

void motor1(unsigned char v)
{analogWrite(Motor1Pin, v);}

void motor2(unsigned char v)
{analogWrite(Motor2Pin, v);}

void motor3(unsigned char v)
{analogWrite(Motor3Pin, v);}

