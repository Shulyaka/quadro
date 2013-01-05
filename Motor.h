fixed Mx, My, Mz;

void motor_updateControl(void)
{
  fixed h=one>>1; //some constants
  fixed k=one>>1;
  
  Mx=-h*state.wx-k*state.q.w*state.q.x;
  My=-h*state.wx-k*state.q.w*state.q.x;
  Mz=-h*state.wx-k*state.q.w*state.q.x;
  
  
}
