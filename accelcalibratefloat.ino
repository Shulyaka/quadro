#ifndef ACFIXED

void print(float a)
{
  Serial.print(" ");
  Serial.print(a, 4);
}

void print(double a)
{
  Serial.print(" ");
  Serial.print(a, 4);
}

void print(const char *name, float a)
{
  Serial.print(name);
  Serial.print(":");
  print(a);
  Serial.println("");
}

void print(const char *name, double a)
{
  Serial.print(name);
  Serial.print(":");
  print(a);
  Serial.println("");
}

inline float pow2(float a)
{ return sq(a); }

inline float pow3(float a)
{ return a*a*a; }

inline float pow4(float a)
{ return a*a*a*a; }

void matrixtest(void)
{
  float a[5][5];
  a[0][0]=1.0; a[0][1]=0; a[0][2]=0; a[0][3]=0; a[0][4]=0;
  a[1][0]=0; a[1][1]=1.0; a[1][2]=0; a[1][3]=0; a[1][4]=0;
  a[2][0]=0; a[2][1]=0; a[2][2]=1.0; a[2][3]=0; a[2][4]=0;
  a[3][0]=0; a[3][1]=0; a[3][2]=0; a[3][3]=1.0; a[3][4]=0;
  a[4][0]=0; a[4][1]=0; a[4][2]=0; a[4][3]=0; a[4][4]=1.0;
  printmatrix("a",a);
  print("det",det(a));
}

void printpoint(float a[3])
{
  print(a[0]);
  Serial.print(",");
  print(a[1]);
  Serial.print(",");
  print(a[2]);
  Serial.println("");
}

void printpoint(const char *name, float a[3])
{
  Serial.print(name);
  Serial.print(":");
  printpoint(a);
}

void printrow(float a[6])
{
  print(a[0]);
  Serial.print(",");
  print(a[1]);
  Serial.print(",");
  print(a[2]);
  Serial.print(",");
  print(a[3]);
  Serial.print(",");
  print(a[4]);
  Serial.print(",");
  print(a[5]);
  Serial.println("");
}

void printrow(const char *name, float a[6])
{
  Serial.print(name);
  Serial.print(":");
  printrow(a);
}

void printmatrix(float a[3][3])
{
  printpoint(a[0]);
  printpoint(a[1]);
  printpoint(a[2]);
}

void printmatrix(const char *name, float a[3][3])
{
  Serial.print(name);
  Serial.println(":");
  printmatrix(a);
}

void printmatrix(float a[4][4])
{
  print(a[0][0]);
  Serial.print(",");
  print(a[0][1]);
  Serial.print(",");
  print(a[0][2]);
  Serial.print(",");
  print(a[0][3]);
  Serial.println("");
  print(a[1][0]);
  Serial.print(",");
  print(a[1][1]);
  Serial.print(",");
  print(a[1][2]);
  Serial.print(",");
  print(a[1][3]);
  Serial.println("");
  print(a[2][0]);
  Serial.print(",");
  print(a[2][1]);
  Serial.print(",");
  print(a[2][2]);
  Serial.print(",");
  print(a[2][3]);
  Serial.println("");
  print(a[3][0]);
  Serial.print(",");
  print(a[3][1]);
  Serial.print(",");
  print(a[3][2]);
  Serial.print(",");
  print(a[3][3]);
  Serial.println("");
}

void printmatrix(const char *name, float a[4][4])
{
  Serial.print(name);
  Serial.println(":");
  printmatrix(a);
}

void printmatrix(float a[5][5])
{
  const byte n=5;
  for(byte i=0; i<n; i++)
    for(byte j=0; j<n; j++)
    {
      print(a[i][j]);
      if(j!=n-1)
        Serial.print(",");
      else
        Serial.println("");
    }
}

void printmatrix(const char *name, float a[5][5])
{
  Serial.print(name);
  Serial.println(":");
  printmatrix(a);
}

void printmatrix(float a[6][6])
{
  const byte n=6;
  for(byte i=0; i<n; i++)
    for(byte j=0; j<n; j++)
    {
      print(a[i][j]);
      if(j!=n-1)
        Serial.print(",");
      else
        Serial.println("");
    }
}

void printmatrix(const char *name, float a[6][6])
{
  Serial.print(name);
  Serial.println(":");
  printmatrix(a);
}

float det(float a[3][3])
{ return a[0][0]*a[1][1]*a[2][2]-a[0][2]*a[1][1]*a[2][0]+a[0][1]*a[1][2]*a[2][0]-a[0][1]*a[1][0]*a[2][2]+a[0][2]*a[1][0]*a[2][1]-a[0][0]*a[1][2]*a[2][1]; }

float det(float a[4][4])
{
  float m[3][3];
  float d;
  m[0][0]=a[1][1]; m[0][1]=a[1][2]; m[0][2]=a[1][3];
  m[1][0]=a[2][1]; m[1][1]=a[2][2]; m[1][2]=a[2][3];
  m[2][0]=a[3][1]; m[2][1]=a[3][2]; m[2][2]=a[3][3];
  d=a[0][0]*det(m);
  m[0][0]=a[1][0];
  m[1][0]=a[2][0];
  m[2][0]=a[3][0];
  d=d-a[0][1]*det(m);
  m[0][1]=a[1][1];
  m[1][1]=a[2][1];
  m[2][1]=a[3][1];
  d=d+a[0][2]*det(m);
  m[0][2]=a[1][2];
  m[1][2]=a[2][2];
  m[2][2]=a[3][2];
  return d-a[0][3]*det(m);
}

float det(float a[5][5])
{
  float m[4][4];
  float d;
  m[0][0]=a[1][1]; m[0][1]=a[1][2]; m[0][2]=a[1][3]; m[0][3]=a[1][4];
  m[1][0]=a[2][1]; m[1][1]=a[2][2]; m[1][2]=a[2][3]; m[1][3]=a[2][4];
  m[2][0]=a[3][1]; m[2][1]=a[3][2]; m[2][2]=a[3][3]; m[2][3]=a[3][4];
  m[3][0]=a[4][1]; m[3][1]=a[4][2]; m[3][2]=a[4][3]; m[3][3]=a[4][4];
  d=a[0][0]*det(m);
  m[0][0]=a[1][0];
  m[1][0]=a[2][0];
  m[2][0]=a[3][0];
  m[3][0]=a[4][0];
  d=d-a[0][1]*det(m);
  m[0][1]=a[1][1];
  m[1][1]=a[2][1];
  m[2][1]=a[3][1];
  m[3][1]=a[4][1];
  d=d+a[0][2]*det(m);
  m[0][2]=a[1][2];
  m[1][2]=a[2][2];
  m[2][2]=a[3][2];
  m[3][2]=a[4][2];
  d=d-a[0][3]*det(m);
  m[0][3]=a[1][3];
  m[1][3]=a[2][3];
  m[2][3]=a[3][3];
  m[3][3]=a[4][3];
  return d+a[0][4]*det(m);
}

float det(float a[6][6])
{
  const byte n=6-1;
  float m[n][n];
  float d=0;
  byte i, j;
  
  for(i=0; i<n; i++)
    for(j=0; j<n; j++)
      m[i][j]=a[i+1][j+1];

  for(j=0; j<n; j++)
  {
    if(j==(j>>1)<<1)
      d=d+a[0][j]*det(m);
    else
      d=d-a[0][j]*det(m);
      
    for(i=0; i<n; i++)
      m[i][j]=a[i+1][j];
  }

  return (n==(n>>1)<<1)? d+a[0][n]*det(m) : d-a[0][n]*det(m);
}

bool lsolve(float a[3][3], float r[3], float x[3])
{
  float m[3][3];
  float d=det(a);
  byte i, j;
  if(d==0)
  {
    x[0]=1.0;
    x[1]=1.0;
    x[2]=1.0;
    return false;
  }

  for(i=0; i<3; i++)
    for(j=1; j<3; j++)
      m[i][j]=a[i][j];

  for(j=0; j<3; j++)
  {
    for(i=0; i<3; i++)
      m[i][j]=r[i];

    x[j]=det(m)/d;

    if(j!=2)
      for(i=0; i<3; i++)
        m[i][j]=a[i][j];
  }

  return true;
}

bool lsolve(float a[6][6], float r[6], float x[6])
{
  const byte n=6;
  float m[n][n];
  float d=det(a);
  byte i, j;
  if(d==0)
  {
    for(i=0; i<n; i++)
      x[i]=1.0;
    return false;
  }

  for(i=0; i<n; i++)
    for(j=1; j<n; j++)
      m[i][j]=a[i][j];

  for(j=0; j<n; j++)
  {
    for(i=0; i<n; i++)
      m[i][j]=r[i];

    x[j]=det(m)/d;

    if(j!=n-1)
      for(i=0; i<n; i++)
        m[i][j]=a[i][j];
  }

  return lcheck(a, r, x);
}

bool lcheck(float a[6][6], float r[6], float x[6])
{
  float delta=0.01;
  float s=0, e;
  for(byte i=0; i<6; i++)
  {
    e=-r[i];
    for(byte j=0; j<6; j++)
      e=e+a[i][j]*x[j];
    s=s+e*e;
  }
  
  if(s>delta)
  {
    print("lcheck failed", s);
    printmatrix("matrix", a);
    printrow("r", r);
    printrow("x", x);
    return false;
  }
  print("lcheck", s);
  return true;
}

bool center(float a[3], float b[3], float c[3], float d[3], float k[6], float x[3])
{
  float m[3][3];
  float r[3];
  float t;
  t=b[0]-a[0];
  m[0][0]=k[0]*(sq(b[0])-sq(a[0]))+t+k[3]*t;
  t=c[0]-a[0];
  m[1][0]=k[0]*(sq(c[0])-sq(a[0]))+t+k[3]*t;
  t=d[0]-a[0];
  m[2][0]=k[0]*(sq(d[0])-sq(a[0]))+t+k[3]*t;

  t=b[1]-a[1];
  m[0][1]=k[1]*(b[1]*b[1]-a[1]*a[1])+t+k[4]*t;
  t=c[1]-a[1];
  m[1][1]=k[1]*(c[1]*c[1]-a[1]*a[1])+t+k[4]*t;
  t=d[1]-a[1];
  m[2][1]=k[1]*(d[1]*d[1]-a[1]*a[1])+t+k[4]*t;

  t=b[2]-a[2];
  m[0][2]=k[2]*(b[2]*b[2]-a[2]*a[2])+t+k[5]*t;
  t=c[2]-a[2];
  m[1][2]=k[2]*(c[2]*c[2]-a[2]*a[2])+t+k[5]*t;
  t=d[2]-a[2];
  m[2][2]=k[2]*(d[2]*d[2]-a[2]*a[2])+t+k[5]*t;

  t=b[0]*b[0]-a[0]*a[0];
  r[0]=     k[0]*k[0]*(pow4(b[0])-pow4(a[0]))+t+k[3]*k[3]*t;
  t=b[1]*b[1]-a[1]*a[1];
  r[0]=r[0]+k[1]*k[1]*(pow4(b[1])-pow4(a[1]))+t+k[4]*k[4]*t;
  t=b[2]*b[2]-a[2]*a[2];
  r[0]=r[0]+k[2]*k[2]*(pow4(b[2])-pow4(a[2]))+t+k[5]*k[5]*t;
  r[0]=r[0]/2.0;
  t=b[0]*b[0]*b[0]-a[0]*a[0]*a[0];
  r[0]=r[0]+k[0]*(t+k[3]*t)+k[3]*(b[0]*b[0]-a[0]*a[0]);
  t=b[1]*b[1]*b[1]-a[1]*a[1]*a[1];
  r[0]=r[0]+k[1]*(t+k[4]*t)+k[4]*(b[1]*b[1]-a[1]*a[1]);
  t=b[2]*b[2]*b[2]-a[2]*a[2]*a[2];
  r[0]=r[0]+k[2]*(t+k[5]*t)+k[5]*(b[2]*b[2]-a[2]*a[2]);
  
  t=c[0]*c[0]-a[0]*a[0];
  r[1]=     k[0]*k[0]*(c[0]*c[0]*c[0]*c[0]-a[0]*a[0]*a[0]*a[0])+t+k[3]*k[3]*t;
  t=c[1]*c[1]-a[1]*a[1];
  r[1]=r[1]+k[1]*k[1]*(c[1]*c[1]*c[1]*c[1]-a[1]*a[1]*a[1]*a[1])+t+k[4]*k[4]*t;
  t=c[2]*c[2]-a[2]*a[2];
  r[1]=r[1]+k[2]*k[2]*(c[2]*c[2]*c[2]*c[2]-a[2]*a[2]*a[2]*a[2])+t+k[5]*k[5]*t;
  r[1]=r[1]/2.0;
  t=c[0]*c[0]*c[0]-a[0]*a[0]*a[0];
  r[1]=r[1]+k[0]*(t+k[3]*t)+k[3]*(c[0]*c[0]-a[0]*a[0]);
  t=c[1]*c[1]*c[1]-a[1]*a[1]*a[1];
  r[1]=r[1]+k[1]*(t+k[4]*t)+k[4]*(c[1]*c[1]-a[1]*a[1]);
  t=c[2]*c[2]*c[2]-a[2]*a[2]*a[2];
  r[1]=r[1]+k[2]*(t+k[5]*t)+k[5]*(c[2]*c[2]-a[2]*a[2]);

  t=d[0]*d[0]-a[0]*a[0];
  r[2]=     k[0]*k[0]*(d[0]*d[0]*d[0]*d[0]-a[0]*a[0]*a[0]*a[0])+t+k[3]*k[3]*t;
  t=d[1]*d[1]-a[1]*a[1];
  r[2]=r[2]+k[1]*k[1]*(d[1]*d[1]*d[1]*d[1]-a[1]*a[1]*a[1]*a[1])+t+k[4]*k[4]*t;
  t=d[2]*d[2]-a[2]*a[2];
  r[2]=r[2]+k[2]*k[2]*(d[2]*d[2]*d[2]*d[2]-a[2]*a[2]*a[2]*a[2])+t+k[5]*k[5]*t;
  r[2]=r[2]/2.0;
  t=d[0]*d[0]*d[0]-a[0]*a[0]*a[0];
  r[2]=r[2]+k[0]*(t+k[3]*t)+k[3]*(d[0]*d[0]-a[0]*a[0]);
  t=d[1]*d[1]*d[1]-a[1]*a[1]*a[1];
  r[2]=r[2]+k[1]*(t+k[4]*t)+k[4]*(d[1]*d[1]-a[1]*a[1]);
  t=d[2]*d[2]*d[2]-a[2]*a[2]*a[2];
  r[2]=r[2]+k[2]*(t+k[5]*t)+k[5]*(d[2]*d[2]-a[2]*a[2]);
  
  return lsolve(m, r, x);
}

float L(float a[6][3], float k[6])
{
  float x0[3];
  float x4[3];
  float x5[3];
  center (a[0], a[1], a[2], a[3], k, x0);
  center (a[4], a[1], a[2], a[3], k, x4);
  center (a[5], a[1], a[2], a[3], k, x5);
  return sq(x4[0]-x0[0]) + sq(x4[1]-x0[1]) + sq(x4[2]-x0[2]) + sq(x5[0]-x0[0]) + sq(x5[1]-x0[1]) + sq(x5[2]-x0[2]);
}

float L(float a[6][3], float k[6], float h0, float h1, float h2, float h3, float h4, float h5)
{
  float kh[6]={
  k[0]+h0,
  k[1]+h1,
  k[2]+h2,
  k[3]+h3,
  k[4]+h4,
  k[5]+h5};
  return L(a,kh);
}

float Fg(float a[6][3], float k[6])
{
  float x[3];
  center (a[0], a[1], a[2], a[3], k, x);
  return sq(k[0]*sq(a[0][0])+k[3]*a[0][0]+a[0][0]-x[0]) + sq(k[1]*sq(a[0][1])+k[4]*a[0][1]+a[0][1]-x[1]) + sq(k[2]*sq(a[0][2])+k[5]*a[0][2]+a[0][2]-x[2]) - 0.25;
}

float Fg(float a[6][3], float k[6], float h0, float h1, float h2, float h3, float h4, float h5)
{
  float kh[6]=
  {
    k[0]+h0,
    k[1]+h1,
    k[2]+h2,
    k[3]+h3,
    k[4]+h4,
    k[5]+h5
  };
  return Fg(a,kh);
}

bool math_magic(float a[6][3], float k2[3], float k1[3], float k0[3]) //please contact me on shulyaka at gmail if you are interested in the math behind it. I'm going to publish a documentation with all the equations some day...
{
  float k[6]={0};
  float m[6][6];
  float r[6];
  float kn[6]={1.0};
  const float h2=0.00005;
  const float h1=0.00005;
  const float h2_2=h2*2.0;
  const float h1_2=h1*2.0;
  const float sqh2=sq(h2);
  const float sqh1=sq(h1);
  const float h1h2=h1*h2;
  float L0;
  byte i;

for(byte j=0; j<32; j++)
{
  L0=L(a,k);
  
  r[0]=(L(a,k,h2,0,0,0,0,0)-L0)/h2;   //d L / d k[0]
  r[1]=(L(a,k,0,h2,0,0,0,0)-L0)/h2;
  r[2]=(L(a,k,0,0,h2,0,0,0)-L0)/h2;
  r[3]=(L(a,k,0,0,0,h1,0,0)-L0)/h1;
  r[4]=(L(a,k,0,0,0,0,h1,0)-L0)/h1;
  r[5]=Fg(a,k);

  m[0][0]=(L(a,k,h2_2,0,0,0,0,0)-(L(a,k,h2,0,0,0,0,0)*2.0)+L0)/sqh2;    //d2L / d k[0]^2
  m[0][1]=(L(a,k,h2,h2,0,0,0,0)-L(a,k,h2,0,0,0,0,0)-L(a,k,0,h2,0,0,0,0)+L0)/sqh2;
  m[0][2]=(L(a,k,h2,0,h2,0,0,0)-L(a,k,h2,0,0,0,0,0)-L(a,k,0,0,h2,0,0,0)+L0)/sqh2;
  m[0][3]=(L(a,k,h2,0,0,h1,0,0)-L(a,k,h2,0,0,0,0,0)-L(a,k,0,0,0,h1,0,0)+L0)/h1h2;
  m[0][4]=(L(a,k,h2,0,0,0,h1,0)-L(a,k,h2,0,0,0,0,0)-L(a,k,0,0,0,0,h1,0)+L0)/h1h2;
  m[0][5]=(L(a,k,h2,0,0,0,0,h1)-L(a,k,h2,0,0,0,0,0)-L(a,k,0,0,0,0,0,h1)+L0)/h1h2;

  m[1][0]=m[0][1];
  m[1][1]=(L(a,k,0,h2_2,0,0,0,0)-(L(a,k,0,h2,0,0,0,0)*2.0)+L0)/sqh2;
  m[1][2]=(L(a,k,0,h2,h2,0,0,0)-L(a,k,0,h2,0,0,0,0)-L(a,k,0,0,h2,0,0,0)+L0)/sqh2;
  m[1][3]=(L(a,k,0,h2,0,h1,0,0)-L(a,k,0,h2,0,0,0,0)-L(a,k,0,0,0,h1,0,0)+L0)/h1h2;
  m[1][4]=(L(a,k,0,h2,0,0,h1,0)-L(a,k,0,h2,0,0,0,0)-L(a,k,0,0,0,0,h1,0)+L0)/h1h2;
  m[1][5]=(L(a,k,0,h2,0,0,0,h1)-L(a,k,0,h2,0,0,0,0)-L(a,k,0,0,0,0,0,h1)+L0)/h1h2;
  
  m[2][0]=m[0][2];
  m[2][1]=m[1][2];
  m[2][2]=(L(a,k,0,0,h2_2,0,0,0)-(L(a,k,0,0,h2,0,0,0)*2.0)+L0)/sqh2;
  m[2][3]=(L(a,k,0,0,h2,h1,0,0)-L(a,k,0,0,h2,0,0,0)-L(a,k,0,0,0,h1,0,0)+L0)/h1h2;
  m[2][4]=(L(a,k,0,0,h2,0,h1,0)-L(a,k,0,0,h2,0,0,0)-L(a,k,0,0,0,0,h1,0)+L0)/h1h2;
  m[2][5]=(L(a,k,0,0,h2,0,0,h1)-L(a,k,0,0,h2,0,0,0)-L(a,k,0,0,0,0,0,h1)+L0)/h1h2;

  m[3][0]=m[0][3];
  m[3][1]=m[1][3];
  m[3][2]=m[2][3];
  m[3][3]=(L(a,k,0,0,0,h1_2,0,0)-(L(a,k,0,0,0,h1,0,0)*2.0)+L0)/sqh1;
  m[3][4]=(L(a,k,0,0,0,h1,h1,0)-L(a,k,0,0,0,h1,0,0)-L(a,k,0,0,0,0,h1,0)+L0)/sqh1;
  m[3][5]=(L(a,k,0,0,0,h1,0,h1)-L(a,k,0,0,0,h1,0,0)-L(a,k,0,0,0,0,0,h1)+L0)/sqh1;

  m[4][0]=m[0][4];
  m[4][1]=m[1][4];
  m[4][2]=m[2][4];
  m[4][3]=m[3][4];
  m[4][4]=(L(a,k,0,0,0,0,h1_2,0)-(L(a,k,0,0,0,0,h1,0)*2.0)+L0)/sqh1;
  m[4][5]=(L(a,k,0,0,0,0,h1,h1)-L(a,k,0,0,0,0,h1,0)-L(a,k,0,0,0,0,0,h1)+L0)/sqh1;

  m[5][0]=(Fg(a,k,h2,0,0,0,0,0)-r[5])/h2;
  m[5][1]=(Fg(a,k,0,h2,0,0,0,0)-r[5])/h2;
  m[5][2]=(Fg(a,k,0,0,h2,0,0,0)-r[5])/h2;
  m[5][3]=(Fg(a,k,0,0,0,h1,0,0)-r[5])/h1;
  m[5][4]=(Fg(a,k,0,0,0,0,h1,0)-r[5])/h1;
  m[5][5]=(Fg(a,k,0,0,0,0,0,h1)-r[5])/h1;
  
  if((L0<h1/5) && (r[5]<h1/5))
    break;

  lsolve(m,r,kn);
  printmatrix("matrix", m);
  printrow("r", r);
  if(isnan(kn[0]))
  {
    Serial.println("Could not find the calibration values");
    return false;
  }
  Serial.print(j);
  Serial.print(": ");
  for(i=0; i<6; i++)
  {
    k[i]=k[i]-kn[i];
    print(k[i]);
  }
  Serial.println(";");
  print("L0",L0);
  print("Fg",r[5]);
}

  center(a[0], a[1], a[2], a[3], k, k0);
  k2[0]=k[0];
  k2[1]=k[1];
  k2[2]=k[2];
  k1[0]=k[3];
  k1[1]=k[4];
  k1[2]=k[5];
  k0[0]=-k0[0];
  k0[1]=-k0[1];
  k0[2]=-k0[2];
  return true;
}

void accel_calibrate_int_measure_wait(void)
{
  int oldval[3];
  int delta=30;
  static int icount=0;
  static int dcount=0;
  for (byte axis = 0; axis < 3; axis++)
    oldval[axis]=accelADC[axis];
  disable_sensor_interrupts();
  interrupts();
  accel_measure();
  enable_sensor_interrupts();

  if(icount==0 && accel_done==false)
    for (byte axis = 0; axis < 3; axis++)
      accelBuf[axis]=ACCELCNT>>1;

  if (abs(accelADC[0]-oldval[0])+abs(accelADC[1]-oldval[1])+abs(accelADC[2]-oldval[2])>delta)
  {
    icount=0;
    if(accel_done==false)
    {
      Serial.print("|");
      if(++dcount>79)
      {
        dcount=0;
        Serial.println("");
      }
    }
    return;
  }

  if(!accel_done)
  {
    for (byte axis = 0; axis < 3; axis++)
      accelBuf[axis]+=accelADC[axis];
    if(++icount==ACCELCNT)
    {
      accel_done=true;
      icount=0;
      dcount=0;
      Serial.println("");
    }
  }
}

void manualcalibrate(void)
{
  float point[3];
  float k0[3]={0};
  float k1[3]={0};
  float k2[3]={0};
  byte i=0;
  
  while(true)
  {
    Serial.print("Waiting for position "); Serial.print(++i); Serial.println("...");
    delay(5000);
    for(accel_done=false; accel_done!=true;);
    for(byte j=0;j<3;j++)
      point[j]=(float)(accelBuf[j]/ACCELCNT)/8192.0;
    printpoint(point);
    print("sqrt", sqrt(sq(point[0])+sq(point[1])+sq(point[2])));
    for(byte j=0; j<3; j++)
      point[j]+=k2[j]*sq(point[j])+k1[j]*point[j]+k0[j];
    printpoint(point);
    print("sqrt", sqrt(sq(point[0])+sq(point[1])+sq(point[2])));
  }
}

void accel_calibrate_manual_2() //manual accel calibration
{
  float point[6][3]={0};
  float k2[3];
  float k1[3];
  float k0[3];
  float kt[6]={0};
  float pt[3]={0};
  int n=0;
//  Serial.println("Manual accel calibration\nThis algorithm will estimate accel zero values\nby measuring gravity in 6 different positions.\nThe more the positions differ, the better estimation.\nThe positions are not required to be exactly aligned to gravity in any way.\nPlease do not move your quadro.");
//matrixtest(); return;
  attachInterrupt(4, accel_calibrate_int_measure_wait, RISING);

//  manualcalibrate();

/*
point[0][0]=-4096;
point[0][1]=0;
point[0][2]=0;

point[1][0]=0;
point[1][1]=3996;
point[1][2]=0;

point[2][0]=0;
point[2][1]=0;
point[2][2]=4096;

point[3][0]=0;
point[3][1]=-3996;//4096;
point[3][2]=0;

point[4][0]=2364;
point[4][1]=2307;//2364;
point[4][2]=-2365;

point[5][0]=2364;
point[5][1]=2307;//2364;
point[5][2]=2365;
//0,5,0
// 4096   4096   4096
//4096^2 4096^2 4096^2
//   0    4096     0

//3996^2   0    3996^2
*/

  for(byte i=0;i<6;i++)
  {
    Serial.print("Waiting for position "); Serial.print(i+1); Serial.println("...");
    delay(5000);
    for(byte k=0; k<20; k++)
    {
      Serial.println(k);
      for(accel_done=false; accel_done!=true;);
      for(byte j=0;j<3;j++)
        point[i][j]+=(float)(accelBuf[j]/ACCELCNT)/8192.0;
    }
    for(byte j=0;j<3;j++)
      point[i][j]/=20.0;
    printpoint(point[i]);
    print("sqrt", sqrt(sq(point[i][0])+sq(point[i][1])+sq(point[i][2])));

  }

  Serial.println("Waiting for position 7...");
  delay(5000);
  for(byte k=0; k<20; k++)
  {
    Serial.println(k);
    for(accel_done=false; accel_done!=true;);
    for(byte j=0;j<3;j++)
      pt[j]+=(float)(accelBuf[j]/ACCELCNT)/8192.0;
  }
  for(byte j=0;j<3;j++)
    pt[j]/=20.0;
  printpoint(pt);
  print("sqrt", sqrt(sq(pt[0])+sq(pt[1])+sq(pt[2])));

  Serial.println("Done. Thanks. Now calculating...");

  if(!math_magic(point, k2, k1, k0)) // find gain to be applied for the 5 points to be on a sphere
    {Serial.println("Error: Non-singular matrix. Possible reasons: programmer was drunk. Please try again.");
    return;
    }

printpoint("k2",k2);
printpoint("k1",k1);
printpoint("k0",k0);

for(byte i=0; i<6; i++)
  for(byte j=0; j<3; j++)
    point[i][j]=k2[j]*sq(point[i][j])+(1+k1[j])*point[i][j]+k0[j];
for(byte j=0; j<3; j++)
  pt[j]=k2[j]*sq(pt[j])+(1+k1[j])*pt[j]+k0[j];

for(byte i=0; i<6; i++)
{
  printpoint("point", point[i]);
  print("sqrt", sqrt(sq(point[i][0])+sq(point[i][1])+sq(point[i][2])));
}

printpoint("point", pt);
print("sqrt", sqrt(sq(pt[0])+sq(pt[1])+sq(pt[2])));


center(point[0], point[1], point[2], point[3], kt, k0);
printpoint("center0", k0);

center(point[0], point[1], point[2], point[4], kt, k0);
printpoint("center4", k0);

center(point[0], point[1], point[2], point[5], kt, k0);
printpoint("center5", k0);



  delay(1500);
  attachInterrupt(4, accel_int, RISING); //not necessery if we call accel_init
}

/*
results:

k2: 0.0194, -0.0282, -0.0094
k1: 0.0017, -0.0176, -0.0178
k0: 0.0064, 0.0062, 0.0037

k2: 0.0849, 0.0050, 0.1154
k1: -0.0271, -0.0119, -0.0193
k0: -0.0274, -0.0106, -0.0293

k2: -0.0017, -0.0111, 0.0060
k1: -0.0052, -0.0170, -0.0187
k0: 0.0062, 0.0033, -0.0012

k2: 0.0032, 0.0533, 0.0080
k1: -0.0547, -0.0143, -0.0173
k0: -0.0200, -0.0089, -0.0032

k2: -0.0439, 0.0606, -0.0507
k1: -0.0284, -0.0225, -0.0158
k0: 0.0041, -0.0047, 0.0115
------
k2: 0.0042, -0.0083, 0.0082
k1: 0.0018, -0.0052, -0.0053
k0: 0.0033, 0.0017, -0.0012

k2: -0.0523, 0.3354, 0.0922
k1: -0.2839, -0.0437, -0.0159
k0: -0.1236, -0.0683, -0.0394

k2: 0.0754, 0.0168, 0.0021
k1: 0.0296, -0.0067, -0.0048
k0: -0.0023, -0.0045, -0.0023

k2: 0.0007, 0.0091, 0.0061
k1: -0.0050, -0.0057, -0.0052
k0: 0.0010, -0.0002, -0.0025

k2: 0.0198, -0.0042, 0.0086
k1: 0.0112, -0.0092, -0.0057
k0: 0.0005, -0.0003, -0.0026

k2: -0.0441, 0.0423, -0.0374
k1: -0.0306, -0.0059, -0.0016
k0: -0.0019, -0.0045, 0.0074

k2: 0.1103, -0.0066, 0.0160
k1: 0.0257, -0.0064, -0.0036
k0: -0.0085, -0.0030, -0.0048

k2: 0.0019, 0.0109, -0.0023
k1: 0.0049, -0.0064, -0.0021
k0: 0.0019, -0.0059, -0.0015

k2: 0.5410, 0.1290, -0.0720
k1: -0.0322, -0.0448, -0.0299
k0: -0.1392, -0.0340, -0.0093

k2: 0.0070, -0.0083, -0.0062
k1: 0.0133, 0.0033, -0.0205
k0: 0.0040, -0.0057, -0.0073



*/

#endif
