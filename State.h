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
  fixed tmp1, tmp2, tmp3;
  lfixed tmp;
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
//  unsigned fixed ggg;
//  int alpha,beta,gamma;
  unsigned long dn;
/*  
  state.sina=hsin(gyroADC[0]);
  state.cosa=hcos(gyroADC[0]);
  state.tana=htan(gyroADC[0]);
  state.sinb=hsin(gyroADC[1]);
  state.cosb=hcos(gyroADC[1]);
  state.tanb=htan(gyroADC[1]);
*/

/*  alpha=gyroalpha; //uncomment for gyro simulation
  beta=gyrobeta;
  gamma=gyrogamma;
*/

  state.sina=hsin(alpha);
  state.cosa=hcos(alpha);
  state.tana=htan(alpha);
  state.sinb=hsin(beta);
  state.cosb=hcos(beta);
  state.tanb=htan(beta);
  state.sinc=hsin(gamma);
  state.cosc=hcos(gamma);

/*state.cosf=hcos(0); //uncomment for step-debugging
state.sinf=hsin(0);
state.cosp=cos((angle)0*115);
state.sinp=sin((angle)0*115);
state.cost=2147483633;//cos((angle)0*115);
state.sint=-250032;//sin((angle)0*115);
*/
  //please contact me on shulyaka at gmail if you are really really interested in theese formulas --Denis
  t1=state.sina*state.sinb;
  t1=(t1*t1)>>1;
  t2=(t1*t1)>>1; //should be enough
  t3=state.cosa*state.cosb;
  norm=(t3+t3*(t1+t2+t2+t2));

  t4=state.sinp*norm;
  t1=state.sinf*t4;
  t2=state.cosf*t4; //inversed
  t3=state.cosp*norm;

  nm1=t1+t3*state.tanb;
  nm2=t2+t3*state.tana; //inversed
  nm3=tofixed(t1%state.tana-t2%state.tanb);
  nm=lvectlen(nm1,nm2,nm3);
  nm1=nm1%one/nm;
  nm2=nm2%one/nm; //inversed
  nm3=nm3%one/nm;
//  state.tmp1=nm1;
//  state.tmp2=nm2;
//  state.tmp3=nm3;

  if (state.cosp > sinpi4) //0-45 dg, sin is more accurate here
    {sinpn=tofixed(nm);
    cospn=sinbycos(sinpn);
    }
  else if (state.cosp > -sinpi4) //45-135 dg, using cos
    {cospn=t3-tofixed(t2%state.tana+t1%state.tanb);
    sinpn=sinbycos(cospn);
    }
  else //135-180 dg, using sin
    {sinpn=tofixed(nm);
    cospn=-sinbycos(sinpn);
    }

  if(nm!=0)
    {
    t1=state.tanb*norm*state.sina;
    t2=tofixed(state.tana*norm%state.sina+state.cosa%norm); //inversed
    t3=state.tanb*norm*state.cosa; //inversed
    
    cosfn=tofixed(nm1%t1+nm2%t2-nm3%t3);
    sinfn=tofixed(nm1%state.cosa+nm3%state.sina);

    t1=state.cosp*state.cosf; //inversed
    t2=state.cosp*state.sinf; //inversed
    t3=state.sinp;
    
    costn=tofixed(nm1%state.sinf+nm2%state.cosf);
    sintn=tofixed(nm2%t2+nm3%t3-nm1%t1);
    }
  else // undefined
    {cosfn=state.cosf;
    sinfn=state.sinf;
    
    costn=state.cosc;
    sintn=state.sinc;
    }

//state.tmp1=sintn;
//state.tmp2=costn;
//state.tmp3=0;

t1=tofixed(state.cost%costn+state.sint%sintn);
//t2=tofixed(state.sint%costn-state.cost%sintn);
//lt1=state.lcost*costn+state.lsint*sintn;
//lt2=state.lsint*costn-state.lcost*sintn;

////dn=usqrt(t1%t1+t2%t2); //one more correction step
////t1=(t1%one).value/dn;
////t2=(t2%one).value/dn;

//state.lcost=lt1;
//state.lsint=lt2;

//state.cost=tofixed(lt1);
//state.sint=tofixed(lt2);
state.cost=t1;
if(state.sint*costn>state.cost*sintn)
  state.sint=sinbycos(t1);
else
  state.sint=-sinbycos(t1);

state.cosf=tofixed(cosfn%state.cosc+sinfn%state.sinc);
state.sinf=tofixed(sinfn%state.cosc-cosfn%state.sinc);

state.cosp=cospn;
state.sinp=sinpn;

t1=state.cost*state.cosf;
t2=state.cost*state.sinf;
t3=state.sint*state.cosf;
t4=state.sint*state.sinf;
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
  //state.ax=6589.01463*atan((float)accelADC[0]/(float)accelADC[2]); //115LBS/° //to be rewritten (I hate floats!)
  //state.ay=6589.01463*atan((float)accelADC[1]/(float)accelADC[2]);
  state.ax=0;
  state.ay=0;
  state.az=0;
  state.vx=0;
  state.vy=0;
  state.vz=0;
  state.x=0;
  state.y=0;
  state.z=0;
  state.cosp=hcos(0); //to be rewritten based on accel estimations
  state.cosf=hcos(0);
  state.cost=hcos(0);
  state.sinp=hsin(0);
  state.sinf=hsin(0);
  state.sint=hsin(0);
}
