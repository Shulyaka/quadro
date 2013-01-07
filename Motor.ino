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

void motor_init(void)
{
  motor0(0);
  motor1(0);
  motor2(0);
  motor3(0);
}

void motor0(signed char v) //our range is 120-247
{if(v>=0)analogWrite(Motor0Pin, 120+v);}

void motor1(signed char v)
{if(v>=0)analogWrite(Motor1Pin, 120+v);}

void motor2(signed char v)
{if(v>=0)analogWrite(Motor2Pin, 120+v);}

void motor3(signed char v)
{if(v>=0)analogWrite(Motor3Pin, 120+v);}

