#include "fplib.h"

typedef int angle;

void printMatrix(long long a[3][3]);
void printRow(long long a[3]);

void printFloat(float x)
{
  if(isinf(x))
    {
      Serial.print("inf");
      return;
    }
    if(isnan(x))
    {
      Serial.print("NaN");
      return;
    }
  Serial.print(x);
}

void printlnFloat(float x)
{
  printFloat(x);
  Serial.println("");
}

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

long long det(long long a11, long long a21, long long a31, long long a12, long long a22, long long a32, long long a13, long long a23, long long a33)
  {return a11*a22*a33-a13*a22*a31+a12*a23*a31-a12*a21*a33+a13*a21*a32-a11*a23*a32;}

float det(float a11, float a21, float a31, float a12, float a22, float a32, float a13, float a23, float a33)
  {return a11*a22*a33-a13*a22*a31+a12*a23*a31-a12*a21*a33+a13*a21*a32-a11*a23*a32;}

long long det(long long a[3][3])
  {return det(a[0][0],a[1][0],a[2][0],a[0][1],a[1][1],a[2][1],a[0][2],a[1][2],a[2][2]);}

float det(float a[3][3])
  {return det(a[0][0],a[1][0],a[2][0],a[0][1],a[1][1],a[2][1],a[0][2],a[1][2],a[2][2]);}

int lcheck(long long a[3][3], long long r[3], long x[3]);
int lcheck(float a[3][3], float r[3], float x[3]);

int lsolve(long long a[3][3], long long *r, long *x)
{
  long long den=det(a);
  if(den==0)
    return 1;
  x[0]=(det(r[0],    r[1],    r[2],    a[0][1], a[1][1], a[2][1], a[0][2], a[1][2], a[2][2])+(den>>1))/den;
  x[1]=(det(a[0][0], a[1][0], a[2][0], r[0],    r[1],    r[2],    a[0][2], a[1][2], a[2][2])+(den>>1))/den;
  x[2]=(det(a[0][0], a[1][0], a[2][0], a[0][1], a[1][1], a[2][1], r[0],    r[1],    r[2]   )+(den>>1))/den;
  lcheck(a, r, x);
  return 0;
}

int lsolve(float a[3][3], float *r, float *x)
{
  float den=det(a);
  if(den==0)
    return 1;
  x[0]=det(r[0],    r[1],    r[2],    a[0][1], a[1][1], a[2][1], a[0][2], a[1][2], a[2][2])/den;
  x[1]=det(a[0][0], a[1][0], a[2][0], r[0],    r[1],    r[2],    a[0][2], a[1][2], a[2][2])/den;
  x[2]=det(a[0][0], a[1][0], a[2][0], a[0][1], a[1][1], a[2][1], r[0],    r[1],    r[2]   )/den;
  lcheck(a, r, x);
  return 0;
}

int lcheck(long long a[3][3], long long r[3], long x[3])
{
  Serial.println("Check:");
  Serial.println((long)(a[0][0]*x[0]+a[0][1]*x[1]+a[0][2]*x[2]-r[0]));
  Serial.println((long)(a[1][0]*x[0]+a[1][1]*x[1]+a[1][2]*x[2]-r[1]));
  Serial.println((long)(a[2][0]*x[0]+a[2][1]*x[1]+a[2][2]*x[2]-r[2]));
}

int lcheck(float a[3][3], float r[3], float x[3])
{
  Serial.println("Check:");
  printlnFloat(a[0][0]*x[0]+a[0][1]*x[1]+a[0][2]*x[2]-r[0]);
  printlnFloat(a[1][0]*x[0]+a[1][1]*x[1]+a[1][2]*x[2]-r[1]);
  printlnFloat(a[2][0]*x[0]+a[2][1]*x[1]+a[2][2]*x[2]-r[2]);
}

int findCenter(long *a, long *b, long *c, long *d, long *x)
{
  long long p[3][3];
  long long r[3];
  r[0]=(long long)b[0]*b[0]-(long long)a[0]*a[0]+(long long)b[1]*b[1]-(long long)a[1]*a[1]+(long long)b[2]*b[2]-(long long)a[2]*a[2];
  r[1]=(long long)c[0]*c[0]-(long long)a[0]*a[0]+(long long)c[1]*c[1]-(long long)a[1]*a[1]+(long long)c[2]*c[2]-(long long)a[2]*a[2];
  r[2]=(long long)d[0]*d[0]-(long long)a[0]*a[0]+(long long)d[1]*d[1]-(long long)a[1]*a[1]+(long long)d[2]*d[2]-(long long)a[2]*a[2];
  p[0][0]=b[0]-a[0]; p[0][1]=b[1]-a[1]; p[0][2]=b[2]-a[2];
  p[1][0]=c[0]-a[0]; p[1][1]=c[1]-a[1]; p[1][2]=c[2]-a[2];
  p[2][0]=d[0]-a[0]; p[2][1]=d[1]-a[1]; p[2][2]=d[2]-a[2];
  printMatrix(p);
  printRow(r);
  if (lsolve(p, r, x))
    return 1;
  x[0]>>=1;
  x[1]>>=1;
  x[2]>>=1;
  return 0;
}
/*
int findCenter(long *a, long *b, long *c, long *d, long *x)
{
  long long r[3];
  long long den=det(b[0]-a[0], c[0]-a[0], d[0]-a[0], b[1]-a[1], c[1]-a[1], d[1]-a[1], b[2]-a[2], c[2]-a[2], d[2]-a[2])<<1;
  if(den==0)
    return 1;
  r[0]=(long long)b[0]*b[0]-(long long)a[0]*a[0]+(long long)b[1]*b[1]-(long long)a[1]*a[1]+(long long)b[2]*b[2]-(long long)a[2]*a[2];
  r[1]=(long long)c[0]*c[0]-(long long)a[0]*a[0]+(long long)c[1]*c[1]-(long long)a[1]*a[1]+(long long)c[2]*c[2]-(long long)a[2]*a[2];
  r[2]=(long long)d[0]*d[0]-(long long)a[0]*a[0]+(long long)d[1]*d[1]-(long long)a[1]*a[1]+(long long)d[2]*d[2]-(long long)a[2]*a[2];
  x[0]=(det(r[0], r[1], r[2], b[1]-a[1], c[1]-a[1], d[1]-a[1], b[2]-a[2], c[2]-a[2], d[2]-a[2])+(den>>1))/den;
  x[1]=(det(b[0]-a[0], c[0]-a[0], d[0]-a[0], r[0], r[1], r[2], b[2]-a[2], c[2]-a[2], d[2]-a[2])+(den>>1))/den;
  x[2]=(det(b[0]-a[0], c[0]-a[0], d[0]-a[0], b[1]-a[1], c[1]-a[1], d[1]-a[1], r[0], r[1], r[2])+(den>>1))/den;
  return 0;
}*/

long long lpow(long long x, byte k)
{
  long long y=1;
  for (byte i=0; i<k; i++) y*=x;
  return y;
}

byte multiplicity(long long x, long long k)
{
  byte i;
  for(i=0; (x/k)*k==x; i++) x/=k;
  return i;
}

long long lcm(long long a, long long b) //to be optimized
{
  byte k,l;
  long long c=1;
  long long f;
  a=abs(a);
  b=abs(b);
  if(a==0) return b;
  if(b==0) return a;
  f=max(a,b);
  for(long long i=2; i<=f; i++)
  {
    k=multiplicity(a,i);
    l=multiplicity(b,i);
    a/=lpow(i,k);
    b/=lpow(i,l);
    c*=lpow(i,max(k,l));
  }
  return c;
}

long long lcm(long long a, long long b, long long c)
{
  return lcm(lcm(a,b),c);
}

long long gcd(long long a, long long b)
{
  if(a==0) return b;
  if(b==0) return a;
  return a*b/lcm(a,b);
}

long long gcd(long long a, long long b, long long c)
{
  return gcd(gcd(a,b),c);
}

void reduceRow(long long *a)
{
  long long m=gcd(abs(a[0]),abs(a[1]),abs(a[2]));
  a[0]/=m;
  a[1]/=m;
  a[2]/=m;
}

void printMatrix(long long a[3][3])
{
  Serial.print((long)a[0][0]); Serial.print(", "); Serial.print((long)a[0][1]); Serial.print(", ");  Serial.println((long)a[0][2]);
  Serial.print((long)a[1][0]); Serial.print(", "); Serial.print((long)a[1][1]); Serial.print(", ");  Serial.println((long)a[1][2]);
  Serial.print((long)a[2][0]); Serial.print(", "); Serial.print((long)a[2][1]); Serial.print(", ");  Serial.println((long)a[2][2]);
}

void printMatrix(float a[3][3])
{
  printFloat(a[0][0]); Serial.print(", "); printFloat(a[0][1]); Serial.print(", ");  printlnFloat(a[0][2]);
  printFloat(a[1][0]); Serial.print(", "); printFloat(a[1][1]); Serial.print(", ");  printlnFloat(a[1][2]);
  printFloat(a[2][0]); Serial.print(", "); printFloat(a[2][1]); Serial.print(", ");  printlnFloat(a[2][2]);
}

void printMatrix(long long a[3][3], char *str)
{
  Serial.println(str);
  printMatrix(a);
}

void printMatrix(float a[3][3], char *str)
{
  Serial.println(str);
  printMatrix(a);
}

void printRow(long long a[3])
{
  Serial.print((long)a[0]); Serial.print(", "); Serial.print((long)a[1]); Serial.print(", ");  Serial.println((long)a[2]);
}

void printRow(float a[3])
{
  printFloat(a[0]); Serial.print(", "); printFloat(a[1]); Serial.print(", ");  printFloat(a[2]);
}

void printRow(long long a[3], char *str)
{
  Serial.print(str); Serial.print(" ");
  printRow(a);
}

void printRow(float a[3], char *str)
{
  Serial.print(str); Serial.print(" ");
  printRow(a);
}

void printRow(long a[3])
{
  Serial.print(a[0]); Serial.print(", "); Serial.print(a[1]); Serial.print(", ");  Serial.println(a[2]);
}

void printRow(long a[3], char *str)
{
  Serial.print(str); Serial.print(" ");
  printRow(a);
}

void printValue(float x)
{
  printlnFloat(x);
}

void printValue(float x, char *str)
{
  Serial.print(str); Serial.print(" ");
  printValue(x);
}

int fss(long long **a, long long *res)
{
  byte k=3;
  long long t[3];
  long long m,m0,m1;
  for(byte i=2;i!=255;i--) //find a row starting on a non-zero elemint
    if(a[i][0]!=0)
      k=i;
  if(k==3) //if not found,
  {
    res[0]=1;
    res[1]=0;
    res[2]=0;
    return 0;
  }
  if(k!=0) //if found,
    for(byte i=0; i<3; i++) //shifting the rows
    {
      t[i]=a[0][i];
      a[0][i]=a[k][i];
      a[k][i]=t[i];
    }
  for(byte i=0;i<3;i++)  //inverting if needed
    if(a[i][0]<0)
      for(byte j=0;j<3;j++)
        a[i][j]=-a[i][j];
  for(byte i=0;i<3;i++)  //reducing the matrix if needed
    reduceRow(a[i]);
//printMatrix(a,"Shifting rows:");
  if(a[1][0]!=0) //processing second row
  {
    m=lcm(a[0][0],a[1][0]);
    a[1][1]=a[1][1]*(m/a[1][0])-a[0][1]*(m/a[0][0]);
    a[1][2]=a[1][2]*(m/a[1][0])-a[0][2]*(m/a[0][0]);
    a[1][0]=0;
    reduceRow(a[1]);
  }
//  printMatrix(a,"Processing second row:");
  if(a[2][0]!=0) //processing third row
  {
    m=lcm(a[0][0],a[2][0]);
    a[2][1]=a[2][1]*(m/a[2][0])-a[0][1]*(m/a[0][0]);
    a[2][2]=a[2][2]*(m/a[2][0])-a[0][2]*(m/a[0][0]);
    a[2][0]=0;
    reduceRow(a[2]);
  }
//printMatrix(a,"Third row:");
  k=0; //the same for the second column
  for(byte i=2;i!=0;i--)
    if(a[i][1]!=0)
      k=i;
  if(k==0) //if not found,
  {
    res[0]=-a[0][1];
    res[1]=a[0][0];
    res[2]=0;
    reduceRow(res);
    return 0;
  }
  if(k!=1) //if found,
    for(byte i=1; i<3; i++) //shifting the rows
    {
      t[i]=a[0][i];
      a[0][i]=a[k][i];
      a[k][i]=t[i];
    }
//printMatrix(a,"Second column:");
  for(byte i=1;i<3;i++)  //inverting if needed
    if(a[i][1]<0)
      for(byte j=1;j<3;j++)
        a[i][j]=-a[i][j];
//printMatrix(a,"Shifting rows:");
  if(a[2][1]!=0) //processing third row again
  {
    m=lcm(a[1][1],a[2][1]);
    a[2][2]=a[2][2]*(m/a[2][1])-a[1][2]*(m/a[1][1]);
    a[2][1]=0;
    reduceRow(a[2]);
  }
//printMatrix(a,"Third row again:");
  if(a[2][2]!=0)
    return 1; //matrix is non-singular

  if(a[0][1]!=0) //reverse pass
  {
    m=lcm(a[1][1],a[0][1]);
    a[0][2]=a[0][2]*(m/a[0][1])-a[1][2]*(m/a[1][1]);
    a[0][0]*=m/a[0][1];
    a[0][1]=0;
    reduceRow(a[0]);
  }
//printMatrix(a,"Reverse pass:");  
  if(a[0][2]==0)
  {
    res[0]=0;
    res[1]=-a[1][2];
    res[2]=a[1][1];
    reduceRow(res);
    return 0;
  }
  
  if(a[1][2]==0)
  {
    res[0]=-a[0][2];
    res[1]=0;
    res[2]=a[0][0];
    reduceRow(res);
    return 0;
  }

  for(byte i=0;i<2;i++)  //inverting if needed
    if(a[i][2]<0)
      for(byte j=0;j<3;j++)
        a[i][j]=-a[i][j];

  m=lcm(a[0][2],a[1][2]);
  a[0][0]*=m/a[0][2];
  a[0][2]=m;
  a[1][1]*=m/a[1][2];
  a[1][2]=m;
  
  res[0]=a[1][1]*a[1][2];
  res[1]=a[0][0]*a[0][2];
  res[2]=-a[0][0]*a[1][1];
  reduceRow(res);
  
  return 0;
}

float f1(float p[3][3], float *g)
{
  float tx=(1+g[0])*(1+g[0]);
  float ty=(1+g[1])*(1+g[1]);
  float tz=(1+g[2])*(1+g[2]);
  return -(p[0][0]*tx+p[0][1]*ty+p[0][2]*tz)*(p[0][1]*ty+p[0][2]*tz)/(tx*tx) + p[1][0]*tx*(p[1][0]*tx+p[1][1]*ty+p[1][2]*tz)/(ty*ty) + p[2][0]*tx*(p[2][0]*tx+p[2][1]*ty+p[2][2]*tz)/(tz*tz);
}

float f2(float p[3][3], float *g)
{
  float tx=(1+g[0])*(1+g[0]);
  float ty=(1+g[1])*(1+g[1]);
  float tz=(1+g[2])*(1+g[2]);
  return p[0][1]*ty*(p[0][0]*tx+p[0][1]*ty+p[0][2]*tz)/(tx*tx) - (p[1][0]*tx+p[1][1]*ty+p[1][2]*tz)*(p[1][0]*tx+p[1][2]*tz)/(ty*ty) + p[2][1]*ty*(p[2][0]*tx+p[2][1]*ty+p[2][2]*tz)/(tz*tz);
}

float f3(float p[3][3], float *g, float *a, float one_g)
{
  long long tx=(1+g[0])*(1+g[0]);
  long long ty=(1+g[1])*(1+g[1]);
  long long tz=(1+g[2])*(1+g[2]);
  return (p[0][0]*tx+p[0][1]*ty+p[0][2]*tz)*(p[0][0]*tx+p[0][1]*ty+p[0][2]*tz)/(tx*4) + (p[1][0]*tx+p[1][1]*ty+p[1][2]*tz)*(p[1][0]*tx+p[1][1]*ty+p[1][2]*tz)/(ty*4) + (p[2][0]*tx+p[2][1]*ty+p[2][2]*tz)*(p[2][0]*tx+p[2][1]*ty+p[2][2]*tz)/(tz*4) \
         - (p[0][0]*tx+p[0][1]*ty+p[0][2]*tz)*a[0]/2 - (p[1][0]*tx+p[1][1]*ty+p[1][2]*tz)*a[1]/2 - (p[2][0]*tx+p[2][1]*ty+p[2][2]*tz)*a[2]/2 + tx*a[0]*a[0] + ty*a[1]*a[1] + tz*a[2]*a[2] - one_g*one_g;
}

float f1d1(float p[3][3], float *g)
{
  float tx=(1+g[0])*(1+g[0]);
  float ty=(1+g[1])*(1+g[1]);
  float tz=(1+g[2])*(1+g[2]);
  return ((p[0][1]*ty+p[0][2]*tz)*(p[0][0]*tx+2*p[0][1]*ty+2*p[0][2]*tz)/(tx*tx*tx) + p[1][0]*(2*p[1][0]*tx+p[1][1]*ty+p[1][2]*tz)/(ty*ty) + p[2][0]*(2*p[2][0]*tx+p[2][1]*ty+p[2][2]*tz)/(tz*tz))*(1+g[0])*2;
}

float f1d2(float p[3][3], float *g)
{
  float tx=(1+g[0])*(1+g[0]);
  float ty=(1+g[1])*(1+g[1]);
  float tz=(1+g[2])*(1+g[2]);
  return (-p[0][1]*(p[0][0]*tx+2*p[0][1]*ty+2*p[0][2]*tz)/(tx*tx) - p[1][0]*tx*(p[1][0]*tx+2*p[1][1]*ty+p[1][2]*tz)/(ty*ty*ty) + p[2][0]*p[2][1]*tx/(tz*tz))*(1+g[1])*2;
}

float f1d3(float p[3][3], float *g)
{
  float tx=(1+g[0])*(1+g[0]);
  float ty=(1+g[1])*(1+g[1]);
  float tz=(1+g[2])*(1+g[2]);
  return (-p[0][2]*(p[0][0]*tx+2*p[0][1]*ty+2*p[0][2]*tz)/(tx*tx) + p[1][0]*p[1][2]*tx/(ty*ty) - p[2][0]*tx*(p[2][0]*tx+2*p[2][1]*ty+p[2][2]*tz)/(tz*tz*tz))*(1+g[2])*2;
}

float f2d1(float p[3][3], float *g)
{
  float tx=(1+g[0])*(1+g[0]);
  float ty=(1+g[1])*(1+g[1]);
  float tz=(1+g[2])*(1+g[2]);
  return (-p[0][1]*(p[0][0]*tx+2*p[0][1]*ty+2*p[0][2]*tz)/(tx*tx*tx) - p[1][0]*(2*p[1][0]*tx+p[1][1]*ty+2*p[1][2]*tz)/(ty*ty) + p[2][0]*p[2][1]*ty/(tz*tz))*(1+g[0])*2;
}

float f2d2(float p[3][3], float *g)
{
  float tx=(1+g[0])*(1+g[0]);
  float ty=(1+g[1])*(1+g[1]);
  float tz=(1+g[2])*(1+g[2]);
  return (p[0][1]*(p[0][0]*tx+2*p[0][1]*ty+p[0][2]*tz)/(tx*tx) + (2*p[1][0]*tx+p[1][1]*ty+2*p[1][2]*tz)*(p[1][0]*tx+p[1][2]*tz)/(ty*ty*ty) + p[2][1]*(p[2][0]*tx+2*p[2][1]*ty+p[2][2]*tz)/(tz*tz))*(1+g[1])*2;
}

float f2d3(float p[3][3], float *g)
{
  float tx=(1+g[0])*(1+g[0]);
  float ty=(1+g[1])*(1+g[1]);
  float tz=(1+g[2])*(1+g[2]);
  return (p[0][1]*p[0][2]*ty/(tx*tx) - p[1][2]*(2*p[1][0]*tx+p[1][1]*ty+p[1][2]*tz)/(ty*ty) - p[2][1]*ty*(2*p[2][0]*tx+2*p[2][1]*ty+p[2][2]*tz)/(tz*tz*tz))*(1+g[2])*2;
}

float f3d1(float p[3][3], float *g, float *a)
{
  float tx=(1+g[0])*(1+g[0]);
  float ty=(1+g[1])*(1+g[1]);
  float tz=(1+g[2])*(1+g[2]);
  return (-(p[0][0]*tx+p[0][1]*ty+p[0][2]*tz)*(p[0][0]*tx+p[0][1]*ty+p[0][2]*tz)/(4*tx*tx) + p[0][0]*(p[0][0]*tx+p[0][1]*ty+p[0][2]*tz)/(2*tx) + p[1][0]*(p[1][0]*tx+p[1][1]*ty+p[1][2]*tz)/(2*ty) + p[2][0]*(p[2][0]*tx+p[2][1]*ty+p[2][2]*tz)/(2*tz) - (p[0][0]*a[0]+p[1][0]*a[1]+p[2][0]*a[2])/2 + a[0]*a[0])*(1+g[0])*2;
}

float f3d2(float p[3][3], float *g, float *a)
{
  float tx=(1+g[0])*(1+g[0]);
  float ty=(1+g[1])*(1+g[1]);
  float tz=(1+g[2])*(1+g[2]);
  return (-(p[1][0]*tx+p[1][1]*ty+p[1][2]*tz)*(p[1][0]*tx+p[1][1]*ty+p[1][2]*tz)/(4*ty*ty) + p[0][1]*(p[0][0]*tx+p[0][1]*ty+p[0][2]*tz)/(2*tx) + p[1][1]*(p[1][0]*tx+p[1][1]*ty+p[1][2]*tz)/(2*ty) + p[2][1]*(p[2][0]*tx+p[2][1]*ty+p[2][2]*tz)/(2*tz) - (p[0][1]*a[0]+p[1][1]*a[1]+p[2][1]*a[2])/2 + a[1]*a[1])*(1+g[1])*2;
}

float f3d3(float p[3][3], float *g, float *a)
{
  float tx=(1+g[0])*(1+g[0]);
  float ty=(1+g[1])*(1+g[1]);
  float tz=(1+g[2])*(1+g[2]);
  return (-(p[2][0]*tx+p[2][1]*ty+p[2][2]*tz)*(p[2][0]*tx+p[2][1]*ty+p[2][2]*tz)/(4*ty*ty) + p[0][2]*(p[0][0]*tx+p[0][1]*ty+p[0][2]*tz)/(2*tx) + p[1][2]*(p[1][0]*tx+p[1][1]*ty+p[1][2]*tz)/(2*ty) + p[2][2]*(p[2][0]*tx+p[2][1]*ty+p[2][2]*tz)/(2*tz) - (p[0][2]*a[0]+p[1][2]*a[1]+p[2][2]*a[2])/2 + a[2]*a[2])*(1+g[2])*2;
}

int findGain(long *a, long *b, long *c, long *d, long *e, long *x, long one_g) //please contact me on shulyaka at gmail if you are interested in the math behind it. I'm going to publish a documentation with all the equations some day...
{
//  long long matrix[9];
//  long long *p[3]={&matrix[0], &matrix[3], &matrix[6]};
  float p[3][3];
  float p1[3][3];
  float m[3][3];
  float r[3];
  float dena;
  float dene;
  float res[3]={0,0,0};
  float at[3]={a[0],a[1],a[2]};
  char string[10]={0};
  //long long Ax, Ay, Az, Bx, By, Bz, Cx, Cy, Cz;
//  long long k=1;

  dena=det((float)(b[0]-a[0]), c[0]-a[0], d[0]-a[0], b[1]-a[1], c[1]-a[1], d[1]-a[1], b[2]-a[2], c[2]-a[2], d[2]-a[2]);
  dene=det((float)(b[0]-e[0]), c[0]-e[0], d[0]-e[0], b[1]-e[1], c[1]-e[1], d[1]-e[1], b[2]-e[2], c[2]-e[2], d[2]-e[2]);
  Serial.println(dena);
  Serial.println(dene);
  p[0][0]=det((float)(b[0]*b[0]-a[0]*a[0]), c[0]*c[0]-a[0]*a[0], d[0]*d[0]-a[0]*a[0], b[1]-a[1], c[1]-a[1], d[1]-a[1], b[2]-a[2], c[2]-a[2], d[2]-a[2])/dena-det((float)(b[0]*b[0]-e[0]*e[0]), c[0]*c[0]-e[0]*e[0], d[0]*d[0]-e[0]*e[0], b[1]-e[1], c[1]-e[1], d[1]-e[1], b[2]-e[2], c[2]-e[2], d[2]-e[2])/dene;
  p[0][1]=det((float)(b[1]*b[1]-a[1]*a[1]), c[1]*c[1]-a[1]*a[1], d[1]*d[1]-a[1]*a[1], b[1]-a[1], c[1]-a[1], d[1]-a[1], b[2]-a[2], c[2]-a[2], d[2]-a[2])/dena-det((float)(b[1]*b[1]-e[1]*e[1]), c[1]*c[1]-e[1]*e[1], d[1]*d[1]-e[1]*e[1], b[1]-e[1], c[1]-e[1], d[1]-e[1], b[2]-e[2], c[2]-e[2], d[2]-e[2])/dene;
  p[0][2]=det((float)(b[2]*b[2]-a[2]*a[2]), c[2]*c[2]-a[2]*a[2], d[2]*d[2]-a[2]*a[2], b[1]-a[1], c[1]-a[1], d[1]-a[1], b[2]-a[2], c[2]-a[2], d[2]-a[2])/dena-det((float)(b[2]*b[2]-e[2]*e[2]), c[2]*c[2]-e[2]*e[2], d[2]*d[2]-e[2]*e[2], b[1]-e[1], c[1]-e[1], d[1]-e[1], b[2]-e[2], c[2]-e[2], d[2]-e[2])/dene;
  p[1][0]=det((float)(b[0]-a[0]), c[0]-a[0], d[0]-a[0], b[0]*b[0]-a[0]*a[0], c[0]*c[0]-a[0]*a[0], d[0]*d[0]-a[0]*a[0], b[2]-a[2], c[2]-a[2], d[2]-a[2])/dena-det((float)(b[0]-e[0]), c[0]-e[0], d[0]-e[0], b[0]*b[0]-e[0]*e[0], c[0]*c[0]-e[0]*e[0], d[0]*d[0]-e[0]*e[0], b[2]-e[2], c[2]-e[2], d[2]-e[2])/dene;
  p[1][1]=det((float)(b[0]-a[0]), c[0]-a[0], d[0]-a[0], b[1]*b[1]-a[1]*a[1], c[1]*c[1]-a[1]*a[1], d[1]*d[1]-a[1]*a[1], b[2]-a[2], c[2]-a[2], d[2]-a[2])/dena-det((float)(b[0]-e[0]), c[0]-e[0], d[0]-e[0], b[1]*b[1]-e[1]*e[1], c[1]*c[1]-e[1]*e[1], d[1]*d[1]-e[1]*e[1], b[2]-e[2], c[2]-e[2], d[2]-e[2])/dene;
  p[1][2]=det((float)(b[0]-a[0]), c[0]-a[0], d[0]-a[0], b[2]*b[2]-a[2]*a[2], c[2]*c[2]-a[2]*a[2], d[2]*d[2]-a[2]*a[2], b[2]-a[2], c[2]-a[2], d[2]-a[2])/dena-det((float)(b[0]-e[0]), c[0]-e[0], d[0]-e[0], b[2]*b[2]-e[2]*e[2], c[2]*c[2]-e[2]*e[2], d[2]*d[2]-e[2]*e[2], b[2]-e[2], c[2]-e[2], d[2]-e[2])/dene;
  p[2][0]=det((float)(b[0]-a[0]), c[0]-a[0], d[0]-a[0], b[1]-a[1], c[1]-a[1], d[1]-a[1], b[0]*b[0]-a[0]*a[0], c[0]*c[0]-a[0]*a[0], d[0]*d[0]-a[0]*a[0])/dena-det((float)(b[0]-e[0]), c[0]-e[0], d[0]-e[0], b[1]-e[1], c[1]-e[1], d[1]-e[1], b[0]*b[0]-e[0]*e[0], c[0]*c[0]-e[0]*e[0], d[0]*d[0]-e[0]*e[0])/dene;
  p[2][1]=det((float)(b[0]-a[0]), c[0]-a[0], d[0]-a[0], b[1]-a[1], c[1]-a[1], d[1]-a[1], b[1]*b[1]-a[1]*a[1], c[1]*c[1]-a[1]*a[1], d[1]*d[1]-a[1]*a[1])/dena-det((float)(b[0]-e[0]), c[0]-e[0], d[0]-e[0], b[1]-e[1], c[1]-e[1], d[1]-e[1], b[1]*b[1]-e[1]*e[1], c[1]*c[1]-e[1]*e[1], d[1]*d[1]-e[1]*e[1])/dene;
  p[2][2]=det((float)(b[0]-a[0]), c[0]-a[0], d[0]-a[0], b[1]-a[1], c[1]-a[1], d[1]-a[1], b[2]*b[2]-a[2]*a[2], c[2]*c[2]-a[2]*a[2], d[2]*d[2]-a[2]*a[2])/dena-det((float)(b[0]-e[0]), c[0]-e[0], d[0]-e[0], b[1]-e[1], c[1]-e[1], d[1]-e[1], b[2]*b[2]-e[2]*e[2], c[2]*c[2]-e[2]*e[2], d[2]*d[2]-e[2]*e[2])/dene;

  p1[0][0]=det((float)(b[0]*b[0]-a[0]*a[0]), c[0]*c[0]-a[0]*a[0], d[0]*d[0]-a[0]*a[0], b[1]-a[1], c[1]-a[1], d[1]-a[1], b[2]-a[2], c[2]-a[2], d[2]-a[2])/dena;
  p1[0][1]=det((float)(b[1]*b[1]-a[1]*a[1]), c[1]*c[1]-a[1]*a[1], d[1]*d[1]-a[1]*a[1], b[1]-a[1], c[1]-a[1], d[1]-a[1], b[2]-a[2], c[2]-a[2], d[2]-a[2])/dena;
  p1[0][2]=det((float)(b[2]*b[2]-a[2]*a[2]), c[2]*c[2]-a[2]*a[2], d[2]*d[2]-a[2]*a[2], b[1]-a[1], c[1]-a[1], d[1]-a[1], b[2]-a[2], c[2]-a[2], d[2]-a[2])/dena;
  p1[1][0]=det((float)(b[0]-a[0]), c[0]-a[0], d[0]-a[0], b[0]*b[0]-a[0]*a[0], c[0]*c[0]-a[0]*a[0], d[0]*d[0]-a[0]*a[0], b[2]-a[2], c[2]-a[2], d[2]-a[2])/dena;
  p1[1][1]=det((float)(b[0]-a[0]), c[0]-a[0], d[0]-a[0], b[1]*b[1]-a[1]*a[1], c[1]*c[1]-a[1]*a[1], d[1]*d[1]-a[1]*a[1], b[2]-a[2], c[2]-a[2], d[2]-a[2])/dena;
  p1[1][2]=det((float)(b[0]-a[0]), c[0]-a[0], d[0]-a[0], b[2]*b[2]-a[2]*a[2], c[2]*c[2]-a[2]*a[2], d[2]*d[2]-a[2]*a[2], b[2]-a[2], c[2]-a[2], d[2]-a[2])/dena;
  p1[2][0]=det((float)(b[0]-a[0]), c[0]-a[0], d[0]-a[0], b[1]-a[1], c[1]-a[1], d[1]-a[1], b[0]*b[0]-a[0]*a[0], c[0]*c[0]-a[0]*a[0], d[0]*d[0]-a[0]*a[0])/dena;
  p1[2][1]=det((float)(b[0]-a[0]), c[0]-a[0], d[0]-a[0], b[1]-a[1], c[1]-a[1], d[1]-a[1], b[1]*b[1]-a[1]*a[1], c[1]*c[1]-a[1]*a[1], d[1]*d[1]-a[1]*a[1])/dena;
  p1[2][2]=det((float)(b[0]-a[0]), c[0]-a[0], d[0]-a[0], b[1]-a[1], c[1]-a[1], d[1]-a[1], b[2]*b[2]-a[2]*a[2], c[2]*c[2]-a[2]*a[2], d[2]*d[2]-a[2]*a[2])/dena;

  printMatrix(p, "p:");
  printMatrix(p1, "p1:");
  
  for(int i=0;i<10;i++)
  {
    m[0][0]=f1d1(p, res); m[0][1]=f1d2(p, res); m[0][2]=f1d3(p, res);
    m[1][0]=f2d1(p, res); m[1][1]=f2d2(p, res); m[1][2]=f2d3(p, res);
    m[2][0]=f3d1(p1, res, at); m[2][1]=f3d2(p1, res, at); m[2][2]=f3d3(p1, res, at);
    r[0]=m[0][0]*res[0]+m[0][1]*res[1]+m[0][2]*res[2]-f1(p, res);
    r[1]=m[1][0]*res[0]+m[1][1]*res[1]+m[1][2]*res[2]-f2(p, res);
    r[2]=m[2][0]*res[0]+m[2][1]*res[1]+m[2][2]*res[2]-f3(p1, res, at, one_g);
    
    printValue(f3(p1, res, at, one_g), "f3:");
    printMatrix(m, "m:");
    printRow(r, "r:");
    if(lsolve(m, r, res)!=0)
      return 1;
    printRow(res, "res:");
  }


  //Serial.println(fss(p,res));
  //printMatrix(p);

  //theoretically we could easily calculate offset here, but we have to stop at this point and do it with separate measurements due to unpredictable nature of BMA180 when it comes to gain/offset relations
  return 0;
}

const fixed sinpi4 = 1518500250L;


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

/*   I used the following code to generate the below lookup table:
void printSqrtTable(void)
{
  Serial.print("const byte l_sqrt[192]={		// input: x (64-255), output: sqrt((x-64)<<8)\n");
  for(i=64; i<256; i++)
  {
    Serial.print((unsigned byte)sqrt(i<<8));
    if(i!=255)
      Serial.print(",\n");
    else
      Serial.print("};\n");
  }
}
*/
const byte l_sqrt[192]={		// input: x (64-255), output: sqrt((x-64)<<8)
128,128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,144,144,145,146,147,148,149,150,150,151,152,153,154,155,155,
156,157,158,159,160,160,161,162,163,163,164,165,166,167,167,168,169,170,170,171,172,173,173,174,175,176,176,177,178,178,179,180,
181,181,182,183,183,184,185,185,186,187,187,188,189,189,190,191,192,192,193,193,194,195,195,196,197,197,198,199,199,200,201,201,
202,203,203,204,204,205,206,206,207,208,208,209,209,210,211,211,212,212,213,214,214,215,215,216,217,217,218,218,219,219,220,221,
221,222,222,223,224,224,225,225,226,226,227,227,228,229,229,230,230,231,231,232,232,233,234,234,235,235,236,236,237,237,238,238,
239,240,240,241,241,242,242,243,243,244,244,245,245,246,246,247,247,248,248,249,249,250,250,251,251,252,252,253,253,254,254,255};

unsigned long usqrt(lfixed x)
{
  char i;
  unsigned long a=0;
  if(x.value<=0) return 0;
  for(i=2; x.value>>i!=0; i+=2); i-=8;
  if(i<0)
    a=((unsigned long)l_sqrt[(x.value<<(-i))-64])>>(4+((-i)>>1));
  else if (i<8)
    a=((unsigned long)l_sqrt[(x.value>>i)-64])>>(4-(i>>1));
  else
    a=((unsigned long)l_sqrt[(x.value>>i)-64])<<((i>>1)-4);
  a=(x.value/a+a)>>1;
  return (x.value/a+a)>>1;
}

fixed sqrt(lfixed x)
{
  char i;
  unsigned long a=0;
  if(x.value<=0) return 0;
  for(i=2; x.value>>i!=0; i+=2); i-=8;
  if(i<0)
    a=((unsigned long)l_sqrt[(x.value<<(-i))-64])>>(4+((-i)>>1));
  else if (i<8)
    a=((unsigned long)l_sqrt[(x.value>>i)-64])>>(4-(i>>1));
  else
    a=((unsigned long)l_sqrt[(x.value>>i)-64])<<((i>>1)-4);
  a=(x.value/a+a+1)>>1;
//  a=(x.value/a+a+1)>>1;
  return (x.value/a+a+1)>>1;
}

lfixed lsqrt(lfixed x)
{
  char i;
  long long temp=0;
  lfixed a=0;
  if(x<=0) return 0;
  for(i=2; x>>i!=0; i+=2); i-=8;
  if(i<0)
    a=((unsigned long long)l_sqrt[(x.value<<(-i))-64])<<(27-((-i)>>1));
//  else if (i<8)
//    a=((unsigned long long)l_sqrt[(x.value>>i)-64])>>(4-(i>>1));
  else
    a=((unsigned long long)l_sqrt[(x.value>>i)-64])<<((i>>1)+27);
  a=(ldiv(x,a)+a+1)>>1;
  a=(ldiv(x,a)+a+1)>>1;
  return (ldiv(x,a)+a+1)>>1;
//  return a;
}

fixed vectlen(fixed a, fixed b, fixed c)
{return sqrt(a%a+b%b+c%c);}

lfixed lvectlen(fixed a, fixed b, fixed c)
{return lsqrt(a%a+b%b+c%c);}

inline fixed sinbycos(fixed& x)
{return (x==one) ? 0 : sqrt(0x4000000000000000ULL-x%x);}

inline fixed cosbysin(fixed& x)
{return sinbycos(x);}

/*angle acos(fixed& x)
{return asin(cosbysin(x));} //to be rewritten
*/

/*inline long mult(long x, long y)
{return (x>0) ? ((long long)y*((unsigned long)x<<1))>>32 : -(((long long)y*((unsigned long)(-x)<<1))>>32);}

inline long mult(long x, long y, long z)
{return mult(mult(x,y),z);}

inline long mult(long x, long y, long z, long w)
{return mult(mult(x,y),mult(z,w));}

long sin(angle x)
{return lsin(x);}
*/

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

