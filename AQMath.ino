//Algebra

int findMedian(int *data, int arraySize)
{
  int temp;
  boolean done = 0;
  //byte i;
  
   // Sorts numbers from lowest to highest
  while (done != 1) {        
    done = 1;
    for (byte i = 0; i<(arraySize-1); i++) {
      if (data[i] > data[i+1]) {     // numbers are out of order - swap
        temp = data[i+1];
        data[i+1] = data[i];
        data[i] = temp;
        done = 0;
      }
    }
  }
  
  return data[arraySize/2]; // return the median value
}

long findMedian2(long *data, int arraySize)
{
  long temp;
  boolean done = 0;
  //byte i;
  
   // Sorts numbers from lowest to highest
  while (done != 1) {        
    done = 1;
    for (byte i = 0; i<(arraySize-1); i++) {
      if (data[i] > data[i+1]) {     // numbers are out of order - swap
        temp = data[i+1];
        data[i+1] = data[i];
        data[i] = temp;
        done = 0;
      }
    }
  }
  
  return data[arraySize/2]; // return the median value
}

long findAverage(long *data, int arraySize)
{
  long c=0;
  for(byte i=0;i<arraySize;i++)
    c+=data[i];
  return c/arraySize;
}

int findAverage2(int *data, int arraySize)
{
  long c=0;
  for(byte i=0;i<arraySize;i++)
    c+=data[i];
  return c/arraySize;
}


//Linear Algebra

bool lcheck(fixed a[9][9], fixed r[9], fixed x[9])
{
  const byte n=9;
  fixed delta=10000L;
  fixed s=0, e;
  for(byte i=0; i<n; i++)
  {
    e=-r[i];
    for(byte j=0; j<9; j++)
      e=e+a[i][j]*x[j];
    s=s+sq(e);
  }
  
  if(s>delta || s<0)
    return false;

  return true;
}

bool lsolve(fixed a[9][9], fixed r[9], fixed x[9])  //solves a linear system using iterational Gauss-Seidel method
{
  const byte n=9;
  fixed x2[n];
  const fixed eps=1;
  fixed norm=one;
  byte i, j;
  int counter=5000;
  
  for(i=0; i<n; i++)   //initial values:
  //  x2[i]=x[i];        //assuming that the provided ones are close enough to the real, or
    x2[i]=0;           //assuming that the provided ones are dummy
  
  while(norm>=eps)
  {
    for(i=0; i<n; i++)
    {
      norm=r[i];
      for(j=0; j<i; j++)
        norm=norm-a[i][j]*x2[j];
      for(j=i+1; j<n; j++)
        norm=norm-a[i][j]*x[j];
      x2[i]=norm%one/a[i][i];
    }
    
    norm=abs(x[0]-x2[0]);
    x[0]=x2[0];
    for(i=1; i<n; i++)
    {
      if(abs(x[i]-x2[i])>norm)
        norm=abs(x[i]-x2[i]);
      x[i]=x2[i];
    }
    
    if(--counter==0)
    {
      return false;
    }
  }

  return lcheck(a, r, x);
}


//Trigonometry

//const fixed sinpi4 = 1518500250L;

angle asin(fixed sinx)
{
  fixed s2=sinx*sinx;
  fixed k=1686788L;
  return ((sinx*k).value + (sinx*s2*k).value/6 + ((s2*s2*sinx*k).value*3)/40)/256;
}

angle getangle(fixed sinx, fixed cosx)
{
  if(sinx>sinpi4)
    return 10350-asin(cosx);
  if(sinx<-sinpi4)
    return -10350+asin(cosx);
  if(cosx>0)
    return asin(sinx);
  if(sinx>0)
    return 20700-asin(sinx);
  else
    return -20700+asin(sinx);
}

//inline long lsinbylcos(long x)
//{return lsqrt(0x4000000000000000ULL-(unsigned long long)x*(unsigned long long)x);}

//inline long lcosbylsin(long x)
//{return lsinbylcos(x);}

fixed vectlen(fixed a, fixed b, fixed c)
{return sqrt(a%a+b%b+c%c);}

fixed vectlen(fixed a, fixed b)
{return sqrt(a%a+b%b);}

lfixed lvectlen(fixed a, fixed b, fixed c)
{return lsqrt(a%a+b%b+c%c);}

lfixed lvectlen(fixed a, fixed b)
{return lsqrt(a%a+b%b);}

inline fixed sinbycos(fixed& x)
{return (x==one) ? 0 : sqrt(0x4000000000000000ULL-x%x);}

inline fixed cosbysin(fixed& x)
{return sinbycos(x);}

int vectnorm(fixed x[3])
{
  fixed l=vectlen(x[0], x[1], x[2]);
//  print("vectlen",l);
  if(l==0)
    return -1;
  if(l>0)
  {
    x[0]=x[0]%one/l;
    x[1]=x[1]%one/l;
    x[2]=x[2]%one/l;
  }
  else
  {
    l=l+l*l+l*l*l; //should be enough
    x[0]=x[0]+x[0]*l;
    x[1]=x[1]+x[1]*l;
    x[2]=x[2]+x[2]*l;
  }
  return 0;
}

int lvectnorm(fixed x[3])
{
  lfixed l=lvectlen(x[0], x[1], x[2]);
  if(l==0)
    return -1;

  x[0]=x[0]%one/l;
  x[1]=x[1]%one/l;
  x[2]=x[2]%one/l;
  
  return 0;
}

fixed hsin(int x) //third approximation
{
  fixed a=(11198484186LL*gyro_time*x+0x80000000)>>32;
  fixed a2=a*a;
  fixed a3=a2*a;
  return a - (a3.value+3)/6 + ((a3*a2).value+60)/120;
}

fixed hcos(int x)
{
  fixed a=(11198484186LL*gyro_time*x+0x80000000)>>32;
  fixed a2=a*a;
  return one - ((a2.value+1)>>1) + ((a2*a2).value+12)/24;
}

fixed htan(int x)
{
  fixed a=(11198484186LL*gyro_time*x+0x80000000)>>32;
  fixed a2=a*a;
  fixed a3=a2*a;
  return a + (a3.value+1)/3 + (((a3*a2)<<1).value+7)/15;
}

fixed qsin(int x) //returns sin of half of the angle
{
  fixed a=((5599242093LL>>gyrolowpass)*gyro_time*x+0x80000000)>>32;
  fixed a2=a*a;
  fixed a3=a2*a;
  return a - (a3.value+3)/6 + ((a3*a2).value+60)/120;
}

fixed qcos(int x)
{
  fixed a=((5599242093LL>>gyrolowpass)*gyro_time*x+0x80000000)>>32;
  fixed a2=a*a;
  return one - ((a2.value+1)>>1) + ((a2*a2).value+12)/24;
}

