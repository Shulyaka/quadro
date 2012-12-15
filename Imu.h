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
  fixed cospn, sinpn;
  fixed cosfn, sinfn;
  fixed costn, sintn;
  fixed t1, t2, t3, t4;
  lfixed lt1, lt2;
  fixed norm;
  fixed nm1,nm2,nm3;
  lfixed nm;
  quaternion qr;
  fixed sasb,sacb,casb,cacb;
//  unsigned fixed ggg;
  unsigned long dn;

  alpha=gyroalpha; //uncomment for gyro simulation
  beta=gyrobeta;
  gamma=gyrogamma;


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
  
  qr=quaternion(sacb*state.cosc+casb*state.sinc, casb*state.cosc-sacb*state.sinc, sasb*state.cosc+cacb*state.sinc, cacb*state.cosc-sasb*state.sinc); //qx*qy*qz
  state.q=qr*state.q;

state.x1=t1*state.cosp+t4;
state.x2=t2*state.cosp-t3;
state.x3=-state.cost*state.sinp;
state.y1=t3*state.cosp-t2;
state.y2=t4*state.cosp+t1;
state.y3=-state.sint*state.sinp;
state.z1=state.cosf*state.sinp;
state.z2=state.sinf*state.sinp;
state.z3=state.cosp;
}

const fixed gravity=0x40000000;

void state_updatePosition(fixed i, fixed j, fixed k)
{
  state.ax=state.x1*i+state.x2*j+state.x3*k;
  state.ay=state.y1*i+state.y2*j+state.y3*k;
  state.az=state.z1*i+state.z2*j+state.z3*k+gravity;
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
  fixed x,y,z;
  while(accel_time==0)
    continue;

  disable_sensor_interrupts();

  x=(long)accelADC[1]<<18;
  y=-(long)accelADC[0]<<18;
  z=(long)accelADC[2]<<18;
  
  state.cosp=tolfixed(z)/vectlen(x,y,z);
  state.sinp=sinbycos(state.cosp);
  
  state.cosf=tolfixed(x)/vectlen(x,y);
  state.sinf=tolfixed(y)/vectlen(x,y);
  state.cost=state.cosf; //to be rewritten based on magneto estimations
  state.sint=state.sinf;

/*  state.cosp=hcos(0);
  state.sinp=hsin(0);
  state.cosf=hcos(0);
  state.sinf=hsin(0);
  state.cost=hcos(0);
  state.sint=hsin(0);
*/
  enable_sensor_interrupts();
}

