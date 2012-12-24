typedef struct State {
  fixed ax, ay, az;
  fixed vx, vy, vz;
  fixed x, y, z;
  fixed x1, x2, x3; //coordinates of "East"
  fixed y1, y2, y3; //coordinates of "North"
  fixed z1, z2, z3; //coordinates of "Top"
  fixed cosa, cosb, cosc, sina, sinb, sinc, tana, tanb;
  fixed cosp, sinp;
  fixed cosf, sinf;
  fixed cost, sint;
  lfixed lcost, lsint;
  fixed tmp1, tmp2, tmp3, tmp4, tmp5;
  lfixed tmp;
  quaternion q;
};

State state;
angle gyroalpha=0;
angle gyrobeta=0;
angle gyrogamma=0;

void state_updateOrientation(int alpha, int beta, int gamma)
{
/*  fixed cospn, sinpn;
  fixed cosfn, sinfn;
  fixed costn, sintn;
  fixed t1, t2, t3, t4;
  lfixed lt1, lt2;
  fixed norm;
  fixed nm1,nm2,nm3;
  lfixed nm;*/
//  quaternion qr;
  fixed sasb,sacb,casb,cacb;
//  quaternion qt1, qt2, qt3;

//  alpha=gyroalpha; //uncomment for gyro simulation
//  beta=gyrobeta;
//  gamma=gyrogamma;


  state.sina=qsin(alpha);
  state.cosa=qcos(alpha);
  state.sinb=qsin(beta);
  state.cosb=qcos(beta);
  state.sinc=qsin(gamma);
  state.cosc=qcos(gamma);

  sasb=state.sina*state.sinb;
  sacb=state.sina*state.cosb;
  casb=state.cosa*state.sinb;
  cacb=state.cosa*state.cosb;
//  qr=quaternion(sacb*state.cosc+casb*state.sinc, casb*state.cosc-sacb*state.sinc, sasb*state.cosc+cacb*state.sinc, cacb*state.cosc-sasb*state.sinc);
  state.q=state.q*quaternion(sacb*state.cosc+casb*state.sinc, casb*state.cosc-sacb*state.sinc, sasb*state.cosc+cacb*state.sinc, cacb*state.cosc-sasb*state.sinc); //qx*qy*qz

//state.tmp1=qr.x;
//state.tmp2=qr.y;
//state.tmp3=qr.z;

/*qt1=state.q*quaternion(state.q.w, state.q.z, -state.q.y, state.q.x);
qt2=state.q*quaternion(-state.q.z, state.q.w, state.q.x, state.q.y);
qt3=state.q*quaternion(state.q.y, -state.q.x, state.q.w, state.q.z);

state.x1=qt1.x;
state.x2=qt1.y;
state.x3=qt1.z;
state.y1=qt2.x;
state.y2=qt2.y;
state.y3=qt2.z;
state.z1=qt3.x;
state.z2=qt3.y;
state.z3=qt3.z;

state.tmp1=qt1.w;
state.tmp2=qt2.w;
state.tmp3=qt3.w;
*/
}

const fixed gravity=0x40000000;

void state_updatePosition(fixed i, fixed j, fixed k)
{
  quaternion acc=state.q*quaternion(i,j,k,0)*conjugate(state.q);
  state.ax=acc.x;
  state.ay=acc.y;
  state.az=acc.z-gravity;
  /*
  state.ax=state.x1*i+state.x2*j+state.x3*k;
  state.ay=state.y1*i+state.y2*j+state.y3*k;
  state.az=state.z1*i+state.z2*j+state.z3*k+gravity;
  */
  state.vx=state.vx+state.ax*(accel_time<<9);
  state.vy=state.vy+state.ay*(accel_time<<9);
  state.vz=state.vz+state.az*(accel_time<<9);
  state.x=state.x+state.vx*(accel_time<<9);
  state.y=state.y+state.vy*(accel_time<<9);
  state.z=state.z+state.vz*(accel_time<<9);
}

void state_init(void)
{
  attachInterrupt(4, dummy_int, RISING);
  attachInterrupt(5, dummy_int, RISING);
  accel_init();
  gyro_init();
}

void state_init_accel(void)
{
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

void state_init_gyro(void)
{
  fixed grav[3];
  fixed nort[3];
  fixed t[3];
  quaternion q1, q2;
  while(accel_time==0)
    continue;

  disable_sensor_interrupts();

  for(byte i=0; i<3;i++)
    grav[i]=(long)accelADC[i]<<18;

  nort[0]=one; // put magneto data here;
  nort[1]=0;
  nort[2]=0;

  q2=quaternion(grav[1]>>1, -grav[0]>>1, 0, (grav[2]>>1)+(one>>1));   //  grav*(0,0,1)=(grav[1], -grav[0], 0)
  q2.normalize();
  
  q1=q2*quaternion(nort[0],nort[1],nort[2],0)*conjugate(q2);

  nort[0]=q1.x;
  nort[1]=q1.y;
  nort[2]=0;
  vectnorm(nort);

  q1=quaternion(0, 0, -nort[1]>>1, (nort[0]>>1)+(one>>1));   //  nort*(1,0,0)=(0, 0, -nort[1]);
  q1.normalize();
  
  state.q=q1*q2;

  enable_sensor_interrupts();
}

