#ifdef ACFIXED
void matrixtest(void)
{
  fixed a[5][5];
  a[0][0]=one; a[0][1]=0; a[0][2]=0; a[0][3]=0; a[0][4]=0;
  a[1][0]=0; a[1][1]=one; a[1][2]=0; a[1][3]=0; a[1][4]=0;
  a[2][0]=0; a[2][1]=0; a[2][2]=one; a[2][3]=0; a[2][4]=0;
  a[3][0]=0; a[3][1]=0; a[3][2]=0; a[3][3]=one; a[3][4]=0;
  a[4][0]=0; a[4][1]=0; a[4][2]=0; a[4][3]=0; a[4][4]=one;
  printmatrix("a",a);
  print("det",det(a));
}

void printpoint(fixed a[3])
{
  print(a[0]);
  Serial.print(",");
  print(a[1]);
  Serial.print(",");
  print(a[2]);
  Serial.println("");
}

void printpoint(const char *name, fixed a[3])
{
  Serial.print(name);
  Serial.print(":");
  printpoint(a);
}

void printrow(fixed a[6])
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

void printrow(const char *name, fixed a[6])
{
  Serial.print(name);
  Serial.print(":");
  printrow(a);
}

void printmatrix(fixed a[3][3])
{
  printpoint(a[0]);
  printpoint(a[1]);
  printpoint(a[2]);
}

void printmatrix(const char *name, fixed a[3][3])
{
  Serial.print(name);
  Serial.println(":");
  printmatrix(a);
}

void printmatrix(fixed a[4][4])
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

void printmatrix(const char *name, fixed a[4][4])
{
  Serial.print(name);
  Serial.println(":");
  printmatrix(a);
}

void printmatrix(fixed a[5][5])
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

void printmatrix(const char *name, fixed a[5][5])
{
  Serial.print(name);
  Serial.println(":");
  printmatrix(a);
}

void printmatrix(fixed a[6][6])
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

void printmatrix(const char *name, fixed a[6][6])
{
  Serial.print(name);
  Serial.println(":");
  printmatrix(a);
}

fixed det(fixed a[3][3])
{ return a[0][0]*a[1][1]*a[2][2]-a[0][2]*a[1][1]*a[2][0]+a[0][1]*a[1][2]*a[2][0]-a[0][1]*a[1][0]*a[2][2]+a[0][2]*a[1][0]*a[2][1]-a[0][0]*a[1][2]*a[2][1]; }

fixed qdet(fixed a[3][3])
{ return ((a[0][0]*a[1][1]*a[2][2])>>4)-((a[0][2]*a[1][1]*a[2][0])>>4)+((a[0][1]*a[1][2]*a[2][0])>>4)-((a[0][1]*a[1][0]*a[2][2])>>4)+((a[0][2]*a[1][0]*a[2][1])>>4)-((a[0][0]*a[1][2]*a[2][1])>>4); }

fixed det(fixed a[4][4])
{
  fixed m[3][3];
  fixed d;
  m[0][0]=a[1][1]; m[0][1]=a[1][2]; m[0][2]=a[1][3];
  m[1][0]=a[2][1]; m[1][1]=a[2][2]; m[1][2]=a[2][3];
  m[2][0]=a[3][1]; m[2][1]=a[3][2]; m[2][2]=a[3][3];
  d=a[0][0]*qdet(m);
  m[0][0]=a[1][0];
  m[1][0]=a[2][0];
  m[2][0]=a[3][0];
  d=d-a[0][1]*qdet(m);
  m[0][1]=a[1][1];
  m[1][1]=a[2][1];
  m[2][1]=a[3][1];
  d=d+a[0][2]*qdet(m);
  m[0][2]=a[1][2];
  m[1][2]=a[2][2];
  m[2][2]=a[3][2];
  return d-a[0][3]*qdet(m);
}

fixed det(fixed a[5][5])
{
  fixed m[4][4];
  fixed d;
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

fixed det(fixed a[6][6])
{
  const byte n=6-1;
  fixed m[n][n];
  fixed d=0;
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

bool lsolve(fixed a[3][3], fixed r[3], fixed x[3])
{
  fixed m[3][3];
  fixed d=det(a);
  byte i, j;
  if(d==0)
  {
    x[0]=one;
    x[1]=one;
    x[2]=one;
    return false;
  }

  for(i=0; i<3; i++)
    for(j=1; j<3; j++)
      m[i][j]=a[i][j];

  for(j=0; j<3; j++)
  {
    for(i=0; i<3; i++)
      m[i][j]=r[i];

    x[j]=det(m)%one/d;

    if(j!=2)
      for(i=0; i<3; i++)
        m[i][j]=a[i][j];
  }

  return true;
}

bool lsolve(fixed a[6][6], fixed r[6], fixed x[6])
{
  const byte n=6;
  fixed m[n][n];
  fixed d=det(a);
  byte i, j;
  print("det a",d);
  if(d==0)
  {
    for(i=0; i<n; i++)
      x[i]=one;
    return false;
  }

  for(i=0; i<n; i++)
    for(j=1; j<n; j++)
      m[i][j]=a[i][j];

  for(j=0; j<n; j++)
  {
    for(i=0; i<n; i++)
      m[i][j]=r[i];

    print("det x",det(m));
    x[j]=det(m)%one/d;

    if(j!=n-1)
      for(i=0; i<n; i++)
        m[i][j]=a[i][j];
  }

  return lcheck(a, r, x);
}

bool lcheck(fixed a[6][6], fixed r[6], fixed x[6])
{
  fixed delta=2147484L;
  fixed s=0, e;
  for(byte i=0; i<6; i++)
  {
    e=-r[i];
    for(byte j=0; j<6; j++)
      e=e+a[i][j]*x[j];
    s=s+sq(e);
  }
  
  if(s>delta || s<0)
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

bool center(fixed a[3], fixed b[3], fixed c[3], fixed d[3], fixed k[6], fixed x[3])
{
  fixed m[3][3];
  fixed r[3];
  fixed t;
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
  r[0]=r[0]>>1;
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
  r[1]=r[1]>>1;
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
  r[2]=r[2]>>1;
  t=d[0]*d[0]*d[0]-a[0]*a[0]*a[0];
  r[2]=r[2]+k[0]*(t+k[3]*t)+k[3]*(d[0]*d[0]-a[0]*a[0]);
  t=d[1]*d[1]*d[1]-a[1]*a[1]*a[1];
  r[2]=r[2]+k[1]*(t+k[4]*t)+k[4]*(d[1]*d[1]-a[1]*a[1]);
  t=d[2]*d[2]*d[2]-a[2]*a[2]*a[2];
  r[2]=r[2]+k[2]*(t+k[5]*t)+k[5]*(d[2]*d[2]-a[2]*a[2]);
  
  return lsolve(m, r, x);
}

fixed L(fixed a[6][3], fixed k[6])
{
  fixed x0[3];
  fixed x4[3];
  fixed x5[3];
  center (a[0], a[1], a[2], a[3], k, x0);
  center (a[4], a[1], a[2], a[3], k, x4);
  center (a[5], a[1], a[2], a[3], k, x5);
  return pow2(x4[0]-x0[0]) + pow2(x4[1]-x0[1]) + pow2(x4[2]-x0[2]) + pow2(x5[0]-x0[0]) + pow2(x5[1]-x0[1]) + pow2(x5[2]-x0[2]);
}

fixed L(fixed a[6][3], fixed k[6], fixed h0, fixed h1, fixed h2, fixed h3, fixed h4, fixed h5)
{
  fixed kh[6]={
  k[0]+h0,
  k[1]+h1,
  k[2]+h2,
  k[3]+h3,
  k[4]+h4,
  k[5]+h5};
  return L(a,kh);
}

fixed Fg(fixed a[6][3], fixed k[6])
{
  fixed x[3];
  center (a[0], a[1], a[2], a[3], k, x);
  return pow2(k[0]*sq(a[0][0])+k[3]*a[0][0]+a[0][0]-x[0]) + pow2(k[1]*sq(a[0][1])+k[4]*a[0][1]+a[0][1]-x[1]) + pow2(k[2]*sq(a[0][2])+k[5]*a[0][2]+a[0][2]-x[2]) - sq(gravity);
}

fixed Fg(fixed a[6][3], fixed k[6], fixed h0, fixed h1, fixed h2, fixed h3, fixed h4, fixed h5)
{
  fixed kh[6]={
  k[0]+h0,
  k[1]+h1,
  k[2]+h2,
  k[3]+h3,
  k[4]+h4,
  k[5]+h5};
  return Fg(a,kh);
}

bool math_magic(fixed a[6][3], fixed k2[3], fixed k1[3], fixed k0[3]) //please contact me on shulyaka at gmail if you are interested in the math behind it. I'm going to publish a documentation with all the equations some day...
{
  fixed k[6]={0};
  fixed m[6][6];
  fixed r[6];
  fixed kn[6]={one};
  const fixed h2=2147483L; //=0.001
  const fixed h1=2147483L; //=0.001
  const fixed h2_2=h2<<1;
  const fixed h1_2=h1<<1;
  const fixed sqh2=sq(h2);
  const fixed sqh1=sq(h1);
  const fixed h1h2=h1*h2;
  fixed L0;
  byte i;

for(byte j=0; j<10; j++)
{
  L0=L(a,k);
  
  r[0]=(L(a,k,h2,0,0,0,0,0)-L0)%one/h2;   //d L / d k[0]
  r[1]=(L(a,k,0,h2,0,0,0,0)-L0)%one/h2;
  r[2]=(L(a,k,0,0,h2,0,0,0)-L0)%one/h2;
  r[3]=(L(a,k,0,0,0,h1,0,0)-L0)%one/h1;
  r[4]=(L(a,k,0,0,0,0,h1,0)-L0)%one/h1;
  r[5]=Fg(a,k);

  m[0][0]=(L(a,k,h2_2,0,0,0,0,0)-(L(a,k,h2,0,0,0,0,0)<<1)+L0)%one/sqh2;    //d2L / d k[0]^2
  m[0][1]=(L(a,k,h2,h2,0,0,0,0)-L(a,k,h2,0,0,0,0,0)-L(a,k,0,h2,0,0,0,0)+L0)%one/sqh2;
  m[0][2]=(L(a,k,h2,0,h2,0,0,0)-L(a,k,h2,0,0,0,0,0)-L(a,k,0,0,h2,0,0,0)+L0)%one/sqh2;
  m[0][3]=(L(a,k,h2,0,0,h1,0,0)-L(a,k,h2,0,0,0,0,0)-L(a,k,0,0,0,h1,0,0)+L0)%one/h1h2;
  m[0][4]=(L(a,k,h2,0,0,0,h1,0)-L(a,k,h2,0,0,0,0,0)-L(a,k,0,0,0,0,h1,0)+L0)%one/h1h2;
  m[0][5]=(L(a,k,h2,0,0,0,0,h1)-L(a,k,h2,0,0,0,0,0)-L(a,k,0,0,0,0,0,h1)+L0)%one/h1h2;

  m[1][0]=m[0][1];
  m[1][1]=(L(a,k,0,h2_2,0,0,0,0)-(L(a,k,0,h2,0,0,0,0)<<1)+L0)%one/sqh2;
  m[1][2]=(L(a,k,0,h2,h2,0,0,0)-L(a,k,0,h2,0,0,0,0)-L(a,k,0,0,h2,0,0,0)+L0)%one/sqh2;
  m[1][3]=(L(a,k,0,h2,0,h1,0,0)-L(a,k,0,h2,0,0,0,0)-L(a,k,0,0,0,h1,0,0)+L0)%one/h1h2;
  m[1][4]=(L(a,k,0,h2,0,0,h1,0)-L(a,k,0,h2,0,0,0,0)-L(a,k,0,0,0,0,h1,0)+L0)%one/h1h2;
  m[1][5]=(L(a,k,0,h2,0,0,0,h1)-L(a,k,0,h2,0,0,0,0)-L(a,k,0,0,0,0,0,h1)+L0)%one/h1h2;
  
  m[2][0]=m[0][2];
  m[2][1]=m[1][2];
  m[2][2]=(L(a,k,0,0,h2_2,0,0,0)-(L(a,k,0,0,h2,0,0,0)<<1)+L0)%one/sqh2;
  m[2][3]=(L(a,k,0,0,h2,h1,0,0)-L(a,k,0,0,h2,0,0,0)-L(a,k,0,0,0,h1,0,0)+L0)%one/h1h2;
  m[2][4]=(L(a,k,0,0,h2,0,h1,0)-L(a,k,0,0,h2,0,0,0)-L(a,k,0,0,0,0,h1,0)+L0)%one/h1h2;
  m[2][5]=(L(a,k,0,0,h2,0,0,h1)-L(a,k,0,0,h2,0,0,0)-L(a,k,0,0,0,0,0,h1)+L0)%one/h1h2;

  m[3][0]=m[0][3];
  m[3][1]=m[1][3];
  m[3][2]=m[2][3];
  m[3][3]=(L(a,k,0,0,0,h1_2,0,0)-(L(a,k,0,0,0,h1,0,0)<<1)+L0)%one/sqh1;
  m[3][4]=(L(a,k,0,0,0,h1,h1,0)-L(a,k,0,0,0,h1,0,0)-L(a,k,0,0,0,0,h1,0)+L0)%one/sqh1;
  m[3][5]=(L(a,k,0,0,0,h1,0,h1)-L(a,k,0,0,0,h1,0,0)-L(a,k,0,0,0,0,0,h1)+L0)%one/sqh1;

  m[4][0]=m[0][4];
  m[4][1]=m[1][4];
  m[4][2]=m[2][4];
  m[4][3]=m[3][4];
  m[4][4]=(L(a,k,0,0,0,0,h1_2,0)-(L(a,k,0,0,0,0,h1,0)<<1)+L0)%one/sqh1;
  m[4][5]=(L(a,k,0,0,0,0,h1,h1)-L(a,k,0,0,0,0,h1,0)-L(a,k,0,0,0,0,0,h1)+L0)%one/sqh1;

  m[5][0]=(Fg(a,k,h2,0,0,0,0,0)-r[5])%one/h2;
  m[5][1]=(Fg(a,k,0,h2,0,0,0,0)-r[5])%one/h2;
  m[5][2]=(Fg(a,k,0,0,h2,0,0,0)-r[5])%one/h2;
  m[5][3]=(Fg(a,k,0,0,0,h1,0,0)-r[5])%one/h1;
  m[5][4]=(Fg(a,k,0,0,0,0,h1,0)-r[5])%one/h1;
  m[5][5]=(Fg(a,k,0,0,0,0,0,h1)-r[5])%one/h1;
  
  lsolve(m,r,kn);
  
  Serial.print(j);
  Serial.print(": ");
  for(i=0; i<6; i++)
  {
    k[i]=k[i]-kn[i];
    print(k[i]);
  }
  Serial.println(";");
  print("L0",L0);
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
  int delta=50;
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

fixed accel_cum=0;

void manualcalibrate(void)
{
  fixed point[3];
//  fixed k0[3]={0};
//  fixed k1[3]={0};
//  fixed k2[3]={0};
  byte i=0;
  
  while(true)
  {
    Serial.print("Waiting for position "); Serial.print(++i); Serial.println("...");
    delay(5000);
    for(accel_done=false; accel_done!=true;);
    for(byte j=0;j<3;j++)
      point[j]=accelBuf[j]<<8;
    printpoint(point);
    print("sqrt", sqrt(point[0]%point[0]+point[1]%point[1]+point[2]%point[2]));
    for(byte j=0; j<3; j++)
      point[j]=point[j]+accel_square[j]*point[j]*point[j]+accel_gain[j]*point[j]+accel_offset[j];
    printpoint(point);
    print("sqrt", sqrt(point[0]%point[0]+point[1]%point[1]+point[2]%point[2]));
  }
}

void accel_calibrate_manual_2() //manual accel calibration
{
  fixed point[6][3];
  fixed k2[3];
  fixed k1[3];
  fixed k0[3];
  int n=0;
//  Serial.println("Manual accel calibration\nThis algorithm will estimate accel zero values\nby measuring gravity in 6 different positions.\nThe more the positions differ, the better estimation.\nThe positions are not required to be exactly aligned to gravity in any way.\nPlease do not move your quadro.");
//matrixtest(); return;
  attachInterrupt(4, accel_calibrate_int_measure_wait, RISING);

  manualcalibrate();
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
    i=0;
    serialEvent();
    serialEvent2();
    Serial.print("Waiting for position "); Serial.print(i+1); Serial.println("...");
    delay(500);
    for(accel_done=false; accel_done!=true;);
    for(byte j=0;j<3;j++)
    {
      point[i][j]=(accelBuf[j]/ACCELCNT)<<18;
      point[i][j]=point[i][j]+point[i][j]*accel_gain[j]+accel_offset[j];
    }
    printpoint(point[i]);
    print("sqrt", sqrt(point[i][0]%point[i][0]+point[i][1]%point[i][1]+point[i][2]%point[i][2]));
    print("sq-g", sqrt(point[i][0]%point[i][0]+point[i][1]%point[i][1]+point[i][2]%point[i][2]) - gravity);
    accel_cum=accel_cum+(sqrt(point[i][0]%point[i][0]+point[i][1]%point[i][1]+point[i][2]%point[i][2])-gravity);
    print("acum",accel_cum);
  }
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
    point[i][j]=point[i][j]+k2[j]*sq(point[i][j])+k1[j]*point[i][j]+k0[j];

for(byte i=0; i<6; i++)
{
  printpoint("point", point[i]);
  print("sqrt", sqrt(point[i][0]%point[i][0]+point[i][1]%point[i][1]+point[i][2]%point[i][2]));
}

  delay(1500);
  attachInterrupt(4, accel_int, RISING); //not necessery if we call accel_init
}
#endif
