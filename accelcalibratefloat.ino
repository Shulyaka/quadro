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

void math_nonmagic(float a[3][3], float k2[3], float k1[3], float k0[3])
{
  int t=1;
  float k[3];
  float m[3][3];
  float r[3]={-0.5+sqrt(sq(a[0][0])+sq(a[0][1])+sq(a[0][2])), -a[1][t], 0.5-sqrt(sq(a[2][0])+sq(a[2][1])+sq(a[2][2]))};

  m[0][0]=1; m[0][1]=-sqrt(sq(a[0][0])+sq(a[0][1])+sq(a[0][2])); m[0][2]=sq(a[0][0])+sq(a[0][1])+sq(a[0][2]);
  m[1][0]=1; m[1][1]=a[1][t]; m[1][2]=sq(a[1][t]);
  m[2][0]=1; m[2][1]=sqrt(sq(a[2][0])+sq(a[2][1])+sq(a[2][2])); m[2][2]=sq(a[2][0])+sq(a[2][1])+sq(a[2][2]);
  
  lsolve(m, r, k);
  
  k2[0]=0;
  k2[1]=0;
  k2[2]=0;
  k1[0]=0;
  k1[1]=0;
  k1[2]=0;
  k0[0]=0;
  k0[1]=0;
  k0[2]=0;
  
  k0[t]=k[0];
  k1[t]=k[1];
  k2[t]=k[2];
}

void math_nonmagic_linear(float a[3][3], float k2[3], float k1[3], float k0[3])
{
  int t=1;

  float a1=sqrt(sq(a[0][0])+sq(a[0][1])+sq(a[0][2]));
  float a2=sqrt(sq(a[2][0])+sq(a[2][1])+sq(a[2][2]));
  
  k2[0]=0;
  k2[1]=0;
  k2[2]=0;
  k1[0]=0;
  k1[1]=0;
  k1[2]=0;
  k0[0]=0;
  k0[1]=0;
  k0[2]=0;
  
  k0[t]=a1/(a2+a1)-0.5;
  k1[t]=1.0/(a2+a1)-1.0;
  k2[t]=0;
}

long long accelQBuf[3]={0};

void accel_calibrate_int_measure_wait(void)
{
  int oldval[3];
  int delta=120;
  static int icount=0;
  static int dcount=0;
  quaternion acc;
  fixed i, j, k;
  
  accel_interrupted=true;
  digitalWrite(AccelLEDPin, HIGH);
  if(gyro_interrupted)
    digitalWrite(GyroLEDPin, LOW);
  else
    digitalWrite(StatusLEDPin, LOW);
    
  for (byte axis = 0; axis < 3; axis++)
    oldval[axis]=accelADC[axis];
  disable_sensor_interrupts();
  interrupts();
  accel_measure();
  enable_sensor_interrupts();

  if(icount==0 && accel_done==false)
    for (byte axis = 0; axis < 3; axis++)
    {
      accelBuf[axis]=0;
      accelQBuf[axis]=0;
    }

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
    digitalWrite(AccelLEDPin, LOW);
    if(gyro_interrupted)
      digitalWrite(GyroLEDPin, HIGH);
    else
      digitalWrite(StatusLEDPin, HIGH);
    accel_interrupted=false;
    return;
  }

  if(!accel_done)
  {
    for (byte axis = 0; axis < 3; axis++)
      accelBuf[axis]+=accelADC[axis];
      
    i=(long)accelADC[0]<<18;
    j=(long)accelADC[1]<<18;
    k=(long)accelADC[2]<<18;
        
    quaternion acc=imu.q*quaternion(i+i*i*accel_square[0]+i*accel_gain[0]+accel_offset[0], j+j*j*accel_square[1]+j*accel_gain[1]+accel_offset[1], k+k*k*accel_square[2]+k*accel_gain[2]+accel_offset[2])*conjugate(imu.q);
    accelQBuf[0]+=acc.x.value;
    accelQBuf[0]+=acc.y.value;
    accelQBuf[0]+=(acc.z-gravity).value;
    
    if(++icount==ACCELCNT)
    {
      accel_done=true;
      icount=0;
      dcount=0;
      Serial.println("");
    }
  }
  
  digitalWrite(AccelLEDPin, LOW);
  if(gyro_interrupted)
    digitalWrite(GyroLEDPin, HIGH);
  else
    digitalWrite(StatusLEDPin, HIGH);
  accel_interrupted=false;
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
      point[j]=(float)accelBuf[j]/(8192.0*ACCELCNT);
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
  float point[3][3]={0};
  float k2[3];
  float k1[3];
  float k0[3];
  float kt[6]={0};
  float pt[3]={0};
  int n=0;
//  float f_accel_offset[3]={0.0, 0.0, -0.015};
//  float f_accel_gain[3]={0.0, 0.0, 0.0005};
//  float f_accel_square[3]={0.0, 0.0, 0.06};
  quaternion q;

//  Serial.println("Manual accel calibration\nThis algorithm will estimate accel zero values\nby measuring gravity in 6 different positions.\nThe more the positions differ, the better estimation.\nThe positions are not required to be exactly aligned to gravity in any way.\nPlease do not move your quadro.");
//matrixtest(); return;
//  attachInterrupt(4, accel_calibrate_int_measure_wait, RISING);

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

accel_square[1]=0;
accel_gain[1]=0;
accel_offset[1]=0;

  for(byte i=0;i<3;i++)
  {
    Serial.print("Waiting for position "); Serial.print(i+1); Serial.println("...");
    delay(5000);
    accel_capture_wait();
    for(byte j=0;j<3;j++)
      point[i][j]=accel_captured[j]==one?1.0:((float)accel_captured[j].value)/0x80000000L;
    printpoint(point[i]);
    print("sqrt", sqrt(sq(point[i][0])+sq(point[i][1])+sq(point[i][2])));
    if(i==1)
    {
      q=accel_captured_imu*quaternion(accel_captured[0], accel_captured[1], accel_captured[2])*conjugate(accel_captured_imu);
      point[1][0]=q.x==one?1.0:((float)q.x.value)/0x80000000L;
      point[1][1]=q.y==one?1.0:((float)q.y.value)/0x80000000L;
      point[1][2]=q.z==one?1.0:((float)q.z.value)/0x80000000L;
      printpoint("adjusted",point[1]);
      print("sqrt", sqrt(sq(point[1][0])+sq(point[1][1])+sq(point[1][2])));
    }
  }

  Serial.println("Waiting for position 0...");
  delay(5000);
  accel_capture_wait();
  for(byte j=0;j<3;j++)
    pt[j]=accel_captured[j]==one?1.0:((float)accel_captured[j].value)/0x80000000L;
  printpoint(pt);
  print("sqrt", sqrt(sq(pt[0])+sq(pt[1])+sq(pt[2])));


  Serial.println("Done. Thanks. Now calculating...");

/*  if(!math_magic(point, k2, k1, k0)) // find gain to be applied for the 5 points to be on a sphere
    {Serial.println("Error: Non-singular matrix. Possible reasons: programmer was drunk. Please try again.");
    return;
    }
*/
math_nonmagic(point, k2, k1, k0);

printpoint("k2",k2);
printpoint("k1",k1);
printpoint("k0",k0);

for(byte i=0; i<3; i++)
  for(byte j=0; j<3; j++)
    point[i][j]+=k2[j]*sq(point[i][j])+k1[j]*point[i][j]+k0[j];
for(byte j=0; j<3; j++)
  pt[j]+=k2[j]*sq(pt[j])+k1[j]*pt[j]+k0[j];

for(byte i=0; i<3; i++)
{
  printpoint("point", point[i]);
  print("sqrt", sqrt(sq(point[i][0])+sq(point[i][1])+sq(point[i][2])));
}

printpoint("point", pt);
print("sqrt", sqrt(sq(pt[0])+sq(pt[1])+sq(pt[2])));

/*
center(point[0], point[1], point[2], point[3], kt, k0);
printpoint("center0", k0);

center(point[0], point[1], point[2], point[4], kt, k0);
printpoint("center4", k0);

center(point[0], point[1], point[2], point[5], kt, k0);
printpoint("center5", k0);
*/

for(byte i=0; i<3; i++)
{
  k2[i]*=2147483648.0;
  k1[i]*=2147483648.0;
  k0[i]*=2147483648.0;
}

printpoint("k2",k2);
printpoint("k1",k1);
printpoint("k0",k0);


  delay(1500);
  attachInterrupt(4, accel_int, RISING); //not necessery if we call accel_init
}

/*
results:

adjusted: 0.0164, 0.0051, 0.4990
k2: 0.0000, 0.0397, 0.0000
k1: 0.0000, 0.0001, 0.0000
k0: 0.0000, -0.0051, 0.0000

adjusted: 0.0169, 0.0023, 0.4998
k2: 0.0000, 0.0289, 0.0000
k1: 0.0000, -0.0001, 0.0000
k0: 0.0000, -0.0023, 0.0000

adjusted: 0.0175, 0.0050, 0.4999
k2: 0.0000, 0.0393, 0.0000
k1: 0.0000, 0.0004, 0.0000
k0: 0.0000, -0.0050, 0.0000

adjusted: 0.0011, -0.0024, 0.5001
k2: 0.0000, 0.0094, 0.0000
k1: 0.0000, -0.0000, 0.0000
k0: 0.0000, 0.0024, 0.0000

adjusted: 0.0057, -0.0240, 0.4993
k2: 0.0000, -0.1151, 0.0000
k1: 0.0000, 0.0006, 0.0000
k0: 0.0000, 0.0241, 0.0000


adjusted: 0.0201, 0.0062, 0.5001
k2: 0.0000, 0.0428, 0.0000
k1: 0.0000, 0.0003, 0.0000
k0: 0.0000, -0.0062, 0.0000

adjusted: 0.0119, -0.0216, 0.4992
k2: 0.0000, -0.1059, 0.0000
k1: 0.0000, 0.0011, 0.0000
k0: 0.0000, 0.0217, 0.0000


---------   linear
k1: 0.0000, -0.0000, 0.0000
k0: 0.0000, 0.0034, 0.0000
k1: 0.0000, -56320.0000, 0.0000
k0: 0.0000, 7283584.0000, 0.0000

k1: 0.0000, -0.0006, 0.0000
k0: 0.0000, 0.0046, 0.0000
k1: 0.0000, -1217408.0000, 0.0000
k0: 0.0000, 9906560.0000, 0.0000

k1: 0.0000, -0.0002, 0.0000
k0: 0.0000, 0.0033, 0.0000
k1: 0.0000, -524672.0000, 0.0000
k0: 0.0000, 7107968.0000, 0.0000

k1: 0.0000, 0.0002, 0.0000
k0: 0.0000, 0.0036, 0.0000
k1: 0.0000, 531968.0000, 0.0000
k0: 0.0000, 7764864.0000, 0.0000


k1: 0.0000, -0.0010, 0.0000
k0: 0.0000, -0.0015, 0.0000
k1: 0.0000, -2181504.0000, 0.0000
k0: 0.0000, -3170176.0000, 0.0000

k1: 0.0000, -0.0009, 0.0000
k0: 0.0000, -0.0012, 0.0000
k1: 0.0000, -1870208.0000, 0.0000
k0: 0.0000, -2665856.0000, 0.0000

k1: 0.0000, -0.0003, 0.0000
k0: 0.0000, -0.0005, 0.0000
k1: 0.0000, -632704.0000, 0.0000
k0: 0.0000, -1156416.0000, 0.0000

k1: 0.0000, -0.0011, 0.0000
k0: 0.0000, -0.0002, 0.0000
k1: 0.0000, -2261888.0000, 0.0000
k0: 0.0000, -406464.0000, 0.0000

k1: 0.0000, -0.0013, 0.0000
k0: 0.0000, -0.0003, 0.0000
k1: 0.0000, -2817664.0000, 0.0000
k0: 0.0000, -706368.0000, 0.0000

k1: 0.0000, -0.0011, 0.0000
k0: 0.0000, -0.0001, 0.0000
k1: 0.0000, -2268800.0000, 0.0000
k0: 0.0000, -224000.0000, 0.0000

k1: 0.0000, -0.0013, 0.0000
k0: 0.0000, -0.0004, 0.0000
k1: 0.0000, -2844288.0000, 0.0000
k0: 0.0000, -752896.0000, 0.0000

k1: 0.0000, -0.0014, 0.0000
k0: 0.0000, -0.0003, 0.0000
k1: 0.0000, -3057920.0000, 0.0000
k0: 0.0000, -707712.0000, 0.0000

k1: 0.0000, -0.0009, 0.0000
k0: 0.0000, -0.0002, 0.0000
k1: 0.0000, -1952768.0000, 0.0000
k0: 0.0000, -483136.0000, 0.0000


--------------------------------------------- X
k1: -0.0018, 0.0000, 0.0000
k0: 0.0011, 0.0000, 0.0000
k1: -3912192.0000, 0.0000, 0.0000
k0: 2341760.0000, 0.0000, 0.0000

k1: -0.0015, 0.0000, 0.0000
k0: 0.0010, 0.0000, 0.0000
k1: -3279232.0000, 0.0000, 0.0000
k0: 2166912.0000, 0.0000, 0.0000

k1: -0.0016, 0.0000, 0.0000
k0: 0.0010, 0.0000, 0.0000
k1: -3541760.0000, 0.0000, 0.0000
k0: 2226944.0000, 0.0000, 0.0000

k1: -0.0019, 0.0000, 0.0000
k0: 0.0010, 0.0000, 0.0000
k1: -4078464.0000, 0.0000, 0.0000
k0: 2152320.0000, 0.0000, 0.0000

k1: -0.0017, 0.0000, 0.0000
k0: 0.0010, 0.0000, 0.0000

k1: -0.0016, 0.0000, 0.0000
k0: 0.0010, 0.0000, 0.0000
k1: -3408128.0000, 0.0000, 0.0000
k0: 2061440.0000, 0.0000, 0.0000

k1: -0.0016, 0.0000, 0.0000
k0: 0.0010, 0.0000, 0.0000
k1: -3425408.0000, 0.0000, 0.0000
k0: 2138752.0000, 0.0000, 0.0000

-------------------------------------------- Z
k1: 0.0000, 0.0000, 0.0006
k0: 0.0000, 0.0000, 0.0015
k1: 0.0000, 0.0000, 1315072.0000
k0: 0.0000, 0.0000, 3218176.0000

k1: 0.0000, 0.0000, -0.0005
k0: 0.0000, 0.0000, 0.0008
k1: 0.0000, 0.0000, -1091328.0000
k0: 0.0000, 0.0000, 1631360.0000

k1: 0.0000, 0.0000, -0.0015
k0: 0.0000, 0.0000, -0.0004
k1: 0.0000, 0.0000, -3117184.0000
k0: 0.0000, 0.0000, -907840.0000

k1: 0.0000, 0.0000, -0.0022
k0: 0.0000, 0.0000, -0.0005
k1: 0.0000, 0.0000, -4795136.0000
k0: 0.0000, 0.0000, -1083776.0000

k1: 0.0000, 0.0000, -0.0016
k0: 0.0000, 0.0000, -0.0002
k1: 0.0000, 0.0000, -3526016.0000
k0: 0.0000, 0.0000, -478592.0000

k1: 0.0000, 0.0000, -0.0007
k0: 0.0000, 0.0000, 0.0002
k1: 0.0000, 0.0000, -1490944.0000
k0: 0.0000, 0.0000, 411904.0000

k1: 0.0000, 0.0000, 0.0008
k0: 0.0000, 0.0000, 0.0003
k1: 0.0000, 0.0000, 1680896.0000
k0: 0.0000, 0.0000, 664960.0000

k1: 0.0000, 0.0000, -0.0005
k0: 0.0000, 0.0000, 0.0007
k1: 0.0000, 0.0000, -1016832.0000
k0: 0.0000, 0.0000, 1544960.0000

k1: 0.0000, 0.0000, -0.0017
k0: 0.0000, 0.0000, 0.0001
k1: 0.0000, 0.0000, -3552512.0000
k0: 0.0000, 0.0000, 302592.0000

k1: 0.0000, 0.0000, 0.0002
k0: 0.0000, 0.0000, 0.0000
k1: 0.0000, 0.0000, 425216.0000
k0: 0.0000, 0.0000, 104192.0000

k1: 0.0000, 0.0000, -0.0015
k0: 0.0000, 0.0000, -0.0005
k1: 0.0000, 0.0000, -3318528.0000
k0: 0.0000, 0.0000, -1136960.0000

k1: 0.0000, 0.0000, -0.0022
k0: 0.0000, 0.0000, 0.0009
k1: 0.0000, 0.0000, -4815744.0000
k0: 0.0000, 0.0000, 1840384.0000

k1: 0.0000, 0.0000, -0.0008
k0: 0.0000, 0.0000, 0.0004
k1: 0.0000, 0.0000, -1681920.0000
k0: 0.0000, 0.0000, 853248.0000

k1: 0.0000, 0.0000, -0.0022
k0: 0.0000, 0.0000, 0.0010
k1: 0.0000, 0.0000, -4622592.0000
k0: 0.0000, 0.0000, 2206080.0000

k1: 0.0000, 0.0000, -0.0012
k0: 0.0000, 0.0000, 0.0001
k1: 0.0000, 0.0000, -2527872.0000
k0: 0.0000, 0.0000, 315136.0000

k1: 0.0000, 0.0000, -0.0053
k0: 0.0000, 0.0000, 0.0003
k1: 0.0000, 0.0000, -11410944.0000
k0: 0.0000, 0.0000, 729216.0000

k1: 0.0000, 0.0000, -0.0024
k0: 0.0000, 0.0000, 0.0001
k1: 0.0000, 0.0000, -5154688.0000
k0: 0.0000, 0.0000, 269056.0000

k1: 0.0000, 0.0000, -0.0022
k0: 0.0000, 0.0000, -0.0001
k1: 0.0000, 0.0000, -4802560.0000
k0: 0.0000, 0.0000, -266560.0000

k1: 0.0000, 0.0000, -0.0010
k0: 0.0000, 0.0000, 0.0003
k1: 0.0000, 0.0000, -2119680.0000
k0: 0.0000, 0.0000, 693248.0000

k1: 0.0000, 0.0000, -0.0007
k0: 0.0000, 0.0000, 0.0006
k1: 0.0000, 0.0000, -1527808.0000
k0: 0.0000, 0.0000, 1209984.0000

k1: 0.0000, 0.0000, 0.0006
k0: 0.0000, 0.0000, 0.0019
k1: 0.0000, 0.0000, 1315072.0000
k0: 0.0000, 0.0000, 3986816.0000

k1: 0.0000, 0.0000, -0.0003
k0: 0.0000, 0.0000, 0.0010
k1: 0.0000, 0.0000, -695808.0000
k0: 0.0000, 0.0000, 2204800.0000

k1: 0.0000, 0.0000, -0.0026
k0: 0.0000, 0.0000, 0.0001
k1: 0.0000, 0.0000, -5523072.0000
k0: 0.0000, 0.0000, 297472.0000

k1: 0.0000, 0.0000, 0.0023
k0: 0.0000, 0.0000, 0.0013
k1: 0.0000, 0.0000, 4892160.0000
k0: 0.0000, 0.0000, 2819840.0000

k1: 0.0000, 0.0000, -0.0011
k0: 0.0000, 0.0000, 0.0004
k1: 0.0000, 0.0000, -2296192.0000
k0: 0.0000, 0.0000, 791040.0000

k1: 0.0000, 0.0000, -0.0024
k0: 0.0000, 0.0000, 0.0005
k1: 0.0000, 0.0000, -5139456.0000
k0: 0.0000, 0.0000, 1018368.0000

----------------------------------------------------- Y square
k2: 0.0000, 0.0126, 0.0000
k1: 0.0000, -0.0004, 0.0000
k0: 0.0000, -0.0043, 0.0000
k2: 0.0000, 26974274.0000, 0.0000
k1: 0.0000, -913277.9375, 0.0000
k0: 0.0000, -9212034.0000, 0.0000

k2: 0.0000, 0.0534, 0.0000
k1: 0.0000, -0.0008, 0.0000
k0: 0.0000, -0.0146, 0.0000
k2: 0.0000, 114666872.0000, 0.0000
k1: 0.0000, -1700769.0000, 0.0000
k0: 0.0000, -31368992.0000, 0.0000

k2: 0.0000, 0.0502, 0.0000
k1: 0.0000, -0.0009, 0.0000
k0: 0.0000, -0.0139, 0.0000
k2: 0.0000, 107732256.0000, 0.0000
k1: 0.0000, -2012332.7500, 0.0000
k0: 0.0000, -29765144.0000, 0.0000
--
k2: 0.0000, 0.0313, 0.0000
k1: 0.0000, -0.0008, 0.0000
k0: 0.0000, -0.0092, 0.0000
k2: 0.0000, 67142912.0000, 0.0000
k1: 0.0000, -1673573.5000, 0.0000
k0: 0.0000, -19725634.0000, 0.0000

k2: 0.0000, 0.0531, 0.0000
k1: 0.0000, -0.0007, 0.0000
k0: 0.0000, -0.0143, 0.0000
k2: 0.0000, 113939456.0000, 0.0000
k1: 0.0000, -1525280.6250, 0.0000
k0: 0.0000, -30794546.0000, 0.0000

k2: 0.0000, 0.0407, 0.0000
k1: 0.0000, -0.0008, 0.0000
k0: 0.0000, -0.0114, 0.0000
k2: 0.0000, 87304928.0000, 0.0000
k1: 0.0000, -1628543.7500, 0.0000
k0: 0.0000, -24418236.0000, 0.0000

k2: 0.0000, 0.0466, 0.0000
k1: 0.0000, 0.0014, 0.0000
k0: 0.0000, -0.0115, 0.0000
k2: 0.0000, 100053352.0000, 0.0000
k1: 0.0000, 2958213.2500, 0.0000
k0: 0.0000, -24664118.0000, 0.0000

k2: 0.0000, 0.0488, 0.0000
k1: 0.0000, -0.0010, 0.0000
k0: 0.0000, -0.0134, 0.0000
k2: 0.0000, 104742912.0000, 0.0000
k1: 0.0000, -2217992.2500, 0.0000
k0: 0.0000, -28691026.0000, 0.0000

k2: 0.0000, 0.0252, 0.0000
k1: 0.0000, -0.0010, 0.0000
k0: 0.0000, -0.0075, 0.0000
k2: 0.0000, 54177164.0000, 0.0000
k1: 0.0000, -2044078.5000, 0.0000
k0: 0.0000, -16099790.0000, 0.0000

k2: 0.0000, 0.0511, 0.0000
k1: 0.0000, -0.0011, 0.0000
k0: 0.0000, -0.0139, 0.0000
k2: 0.0000, 109668408.0000, 0.0000
k1: 0.0000, -2319549.0000, 0.0000
k0: 0.0000, -29875890.0000, 0.0000

k2: 0.0000, 0.0357, 0.0000
k1: 0.0000, -0.0007, 0.0000
k0: 0.0000, -0.0100, 0.0000
k2: 0.0000, 76638136.0000, 0.0000
k1: 0.0000, -1571347.5000, 0.0000
k0: 0.0000, -21402364.0000, 0.0000

k2: 0.0000, 0.0392, 0.0000
k1: 0.0000, -0.0004, 0.0000
k0: 0.0000, -0.0111, 0.0000
k2: 0.0000, 84190040.0000, 0.0000
k1: 0.0000, -945695.1250, 0.0000
k0: 0.0000, -23806080.0000, 0.0000



*/

#endif
