void motor_updateControl(void)
{
  fixed h=one>>1; //some constants
  fixed k=one>>1;
  
  Mx=k*state.q.w*state.q.x-h*state.wx;
  My=k*state.q.w*state.q.y-h*state.wy;
  Mz=k*state.q.w*state.q.z-h*state.wz;
  
  
}

