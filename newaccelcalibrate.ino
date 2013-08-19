void print9row(fixed a[9])
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
  Serial.print(",");
  print(a[6]);
  Serial.print(",");
  print(a[7]);
  Serial.print(",");
  print(a[8]);
  Serial.println("");
}

void print9row(const char *name, fixed a[9])
{
  Serial.print(name);
  Serial.print(":");
  print9row(a);
}

void printmatrix(fixed a[9][9])
{
  const byte n=9;
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

void printmatrix(const char *name, fixed a[9][9])
{
  Serial.print(name);
  Serial.println(":");
  printmatrix(a);
}

bool lcheck(fixed a[9][9], fixed r[9], fixed x[9])
{
  const byte n=9;
  fixed delta=2147484L;
  fixed s=0, e;
  for(byte i=0; i<n; i++)
  {
    e=-r[i];
    for(byte j=0; j<9; j++)
      e=e+a[i][j]*x[j];
    s=s+sq(e);
  }
  
  if(s>delta || s<0)
  {
    print("lcheck failed", s);
    printmatrix("matrix", a);
    print9row("r", r);
    print9row("x", x);
    return false;
  }
  print("lcheck", s);
  return true;
}

bool lsolve(fixed a[9][9], fixed r[9], fixed x[9])  //solves a linear system using iterational Gauss-Seidel method
{
  const byte n=9;
  fixed x2[n];
  const fixed eps=1000;
  fixed norm=one;
  byte i, j, counter=0;
  
  for(i=0; i<n; i++)   //initial values:
  //  x2[i]=x[i];        //assuming that the provided ones are close enough to the real
    x2[i]=0;           //assuming that the provided ones are dummy
  
  printmatrix("a", a);
  print9row("r", r);
  
  while(norm>=eps)
  {
    print9row("lsolve x", x);
    
    for(i=0; i<n; i++)
    {
      norm=r[i];
      for(j=0; j<i-1; j++)
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
    
    if(++counter==0)
    {
      error("Unable to solve the system");
      return false;
    }
  }
  
  print9row("solved! x", x);

  return lcheck(a, r, x);
}

void print(float a)
{
  Serial.print(" ");
  if(isnan(a))
    Serial.print("NaN");
  else if(isinf(a))
    Serial.print("inf");
  else
    Serial.print(a, 4);
}

void print(const char *name, float a)
{
  Serial.print(name);
  Serial.print(":");
  print(a);
  Serial.println("");
}

void print9row(float a[9])
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
  Serial.print(",");
  print(a[6]);
  Serial.print(",");
  print(a[7]);
  Serial.print(",");
  print(a[8]);
  Serial.println("");
}

void print9row(const char *name, float a[9])
{
  Serial.print(name);
  Serial.print(":");
  print9row(a);
}

void printmatrix(float a[9][9])
{
  const byte n=9;
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

void printmatrix(const char *name, float a[9][9])
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
//  Serial.println("det6");
  
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

float det(float a[7][7])
{
  const byte n=7-1;
  float m[n][n];
  float d=0;
  byte i, j;
//  Serial.println("det7");

  
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

float det(float a[8][8])
{
  const byte n=8-1;
  float m[n][n];
  float d=0;
  byte i, j;
//  Serial.println("det8");
  
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

float det(float a[9][9])
{
  const byte n=9-1;
  float m[n][n];
  float d=0;
  byte i, j;
  Serial.println("det9");
  
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

bool lsolve_float(fixed a[9][9], fixed r[9], fixed x[9])
{
  const byte n=9;
  float a2[n][n];
  float r2[n];
  float x2[n];
  
  for(byte i=0; i<n; i++)
  {
    r2[i]=(r[i]==one)?1.0:(float)(r[i].value)/2147483648.0;
    x2[i]=(x[i]==one)?1.0:(float)(x[i].value)/2147483648.0;
    for(byte j=0; j<n; j++)
      a2[i][j]=(a[i][j]==one)?1.0:(float)(a[i][j].value)/2147483648.0;
  }
  
  if(!lsolve(a2, r2, x2))
    return false;
  
  for(byte i=0; i<n; i++)
  {
    if(x2[i]>=1.0)
    {
      error("x>one");
      x[i]=one;
    }
    if(x2[i]<=-1.0)
    {
      error("x<-one");
      x[i]=-one;
    }
    x[i]=(long)(x2[i]*2147483648.0);
  }
  
  return true;
}

bool lcheck(float a[9][9], float r[9], float x[9])
{
  const byte n=9;
  float delta=0.0001;
  float s=0, e;
  for(byte i=0; i<n; i++)
  {
    e=-r[i];
    for(byte j=0; j<n; j++)
      e=e+a[i][j]*x[j];
    s=s+e*e;
  }
  
  if(abs(s)>delta)
  {
    print("lcheck  failed", abs(s));
    return false;
  }
//  print("lcheck", s);
  return true;
}

bool swap_rows(float a[9][9], float b[9], byte i, byte j)
{
  const byte n=9;
  float t;
  for(byte k=0; k<n; k++)
  {
    t=a[i][k];
    a[i][k]=a[j][k];
    a[j][k]=t;
  }
  
  t=b[i];
  b[i]=b[j];
  b[j]=t;
  
  return true;
}

bool is_diag_dom(float a[9][9])
{
  const byte n=9;
  float tmp;
  
  for(byte i=0; i<n; i++)
  {
    tmp=0;
    for(byte j=0; j<n; j++)
      if(i!=j)
        tmp+=abs(a[i][j]);
    
    if(tmp>abs(a[i][i]))
    {
      Serial.println("Matrix is not diagonal dominant");
      print("line", i);
      print("sum", tmp);
      return false;
    }
  }
  
  Serial.println("Matrix is diagonal dominant");
  return true;
}

bool rearrange(float a[9][9], float r[9])
{
  const byte n=9;
  byte i, j, k;
  float tmp;
  
  is_diag_dom(a);
  
  print("det", det(a));
  
  for(i=0; i<n-1; i++)
  {
    k=i;
    tmp=abs(a[i][i]);
    for(j=i+1; j<n; j++)
      if(abs(a[i][j])>tmp)
      {
        tmp=abs(a[i][j]);
        k=j;
      }
    if(k!=i)
      swap_rows(a,r,i,k);
  }
  
  return is_diag_dom(a);
}

bool lsolve(float a[9][9], float r[9], float x[9])  //solves a linear system using iterational Gauss-Seidel method;  Guaranteed only for diagonal-dominant or symmetric positive-definite matrix
{
  const byte n=9;
  float x2[n];
  const float eps=0.000000001;
  float norm=1.0;
  byte i, j;
  int counter=4096;
  
  for(i=0; i<n; i++)   //initial values:
  //  x2[i]=x[i];        //assuming that the provided ones are close enough to the real
    x2[i]=0;           //assuming that the provided ones are dummy
  
  printmatrix("a", a);
  print9row("r", r);

//  rearrange(a, r);
  
//  print("det", det(a));
  
//  printmatrix("system rearranged", a);
//  print9row("r", r);

  while(norm>=eps)
  {
    print9row("lsolve x", x2);
    
    for(i=0; i<n; i++)
    {
      norm=r[i];
      for(j=0; j<i; j++)
        norm-=a[i][j]*x2[j];
      for(j=i+1; j<n; j++)
        norm-=a[i][j]*x[j];
      x2[i]=norm/a[i][i];
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
      error("Unable to solve the system! Falling back to det");
      return lsolve_det(a,r,x);
    }
  }
  
  print9row("solved! x", x);

  return true;//lcheck(a, r, x);
}

bool lsolve2(fixed a[9][9], fixed r[9], fixed x[9])  //solves a linear system using iterational Gauss-Seidel method;  Guaranteed only for diagonal-dominant or symmetric positive-definite matrix
{
  const byte n=9;
  fixed x2[n];
  const fixed eps=10;
  fixed norm=one;
  byte i, j;
  int counter=4096;
  
  for(i=0; i<n; i++)   //initial values:
  //  x2[i]=x[i];        //assuming that the provided ones are close enough to the real
    x2[i]=0;           //assuming that the provided ones are dummy
  
  printmatrix("a", a);
  print9row("r", r);

//  rearrange(a, r);
  
//  print("det", det(a));
  
//  printmatrix("system rearranged", a);
//  print9row("r", r);

  while(norm>=eps)
  {
    print9row("lsolve x", x2);
    
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
      error("Unable to solve the system! Falling back to float");
      return lsolve_float(a,r,x);
    }
  }
  
  print9row("solved! x", x);

  return true;//lcheck(a, r, x);
}


bool lsolve_det(float a[9][9], float r[9], float x[9])
{
  const byte n=9;
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
  
  print9row("solved_det! x", x);
  
  return lcheck(a, r, x);
}

fixed pow(fixed a, byte i)
{
  switch(i)
  {
    case 0:
      return one;
    case 1:
      return a;
    case 2:
      return a*a;
    case 3:
      return a*a*a;
    case 4:
      return sq(a*a);
    default:
      error("Unsupported power");
  }

  return 0;
}

float pow(float a, byte i)
{
  switch(i)
  {
    case 0:
      return 1.0;
    case 1:
      return a;
    case 2:
      return a*a;
    case 3:
      return a*a*a;
    case 4:
      return (a*a)*(a*a);
    default:
      error("Unsupported power");
  }

  return 0;
}

bool math_magic(fixed p[20][3], fixed x[9])          //finds the best calibration parameters using the least squires mathod and iterational Newton's method
{
  byte n=20;
  fixed x2[9];
  fixed r[9];
  fixed a[9][9];
  fixed p2[n][3];
  fixed f[n];
  const fixed eps=1000;
  fixed norm=one;
  byte i, j, k, l, counter=0;
  
  for (i=0; i<9; i++)  //initial values:
  //  x2[i]=x[i];        //assuming that the provided ones are close enough to the real
    x2[i]=0;           //assuming that the provided ones are dummy
  
  while(norm>=eps)
  {
    print9row("math_magic x", x);
    
    for(i=0; i<n; i++)
    {
      p2[i][0]=p[i][0]+x[2]*p[i][0]*p[i][0]+x[1]*p[i][0]+x[0];
      p2[i][1]=p[i][1]+x[5]*p[i][0]*p[i][0]+x[4]*p[i][0]+x[3];
      p2[i][2]=p[i][2]+x[8]*p[i][0]*p[i][0]+x[7]*p[i][0]+x[6];
      f[i]=-gravity*gravity+p2[i][0]*p2[i][0]+p2[i][1]*p2[i][1]+p2[i][2]*p2[i][2];
    }

    for(j=0; j<9; j++)
    {
      r[j]=0;
      for(i=0; i<n; i++)
        r[j]=r[j] - ((p2[i][j/3] * pow(p[i][j/3], j%3) * f[i]));//>>3);     //-J'*f/16
    }

    for(i=0; i<9; i++)
      for(j=0; j<9; j++)
      {
        a[i][j]=0;
        for(k=0; k<n; k++)
          a[i][j]=a[i][j] + ((p2[k][i/3] * pow(p[k][i/3], i%3) * p2[k][j/3] * pow(p[k][j/3], j%3))>>1);    //J'*J/16
      }

    for(k=0; k<n; k++)                                           //Comment this out to get Gauss-Newton method
      for(i=0; i<3; i++)
        for(j=0; j<3; j++)
          for(l=0; l<9; l+=3)
            a[i+l][j+l]=a[i+l][j+l]+( (f[k] * pow(p[k][l/3], i+j)) >> 2);    //f(i)*H(i)/16, i=0..n

    disable_sensor_interrupts(); //for speed
    if(!lsolve2(a, r, x2))
    {
      enable_sensor_interrupts();
      return false;
    }
    enable_sensor_interrupts();
    
    norm=abs(x2[0]);
    x[0]=x[0]+(x2[0]>>2);
    for(i=1; i<n; i++)
    {
      if(abs(x2[i])>norm)
        norm=abs(x2[i]);
      x[i]=x[i]+(x2[i]>>2);
    }
    
    if(++counter==0)
    {
      error("Unable to do the magic");
      return false;
    }
  }
  
  print9row("math_magic result", x);

  return true;
}

bool math_magic(float p[20][3], float x[9])          //finds the best calibration parameters using the least squires mathod and iterational Newton's method
{
  byte n=20;
  float x2[9];
  float r[9];
  float a[9][9];
  float p2[n][3];
  float f[n];
  const float eps=0.000001;
  float norm=1.0;
  byte i, j, k, l, counter=0;
  
  for (i=0; i<9; i++)  //initial values:
  //  x2[i]=x[i];        //assuming that the provided ones are close enough to the real
    x2[i]=0;           //assuming that the provided ones are dummy
  
  //x[1]=0.25;
  
  while(norm>=eps)
  {
    print9row("math_magic x", x);
    
    for(i=0; i<n; i++)
    {
      p2[i][0]=p[i][0]+x[2]*p[i][0]*p[i][0]+x[1]*p[i][0]+x[0];
      p2[i][1]=p[i][1]+x[5]*p[i][0]*p[i][0]+x[4]*p[i][0]+x[3];
      p2[i][2]=p[i][2]+x[8]*p[i][0]*p[i][0]+x[7]*p[i][0]+x[6];
      f[i]=-0.25+p2[i][0]*p2[i][0]+p2[i][1]*p2[i][1]+p2[i][2]*p2[i][2];
    }

    for(j=0; j<9; j++)
    {
      r[j]=0;
      for(i=0; i<n; i++)
        r[j]=r[j] - ((p2[i][j/3] * pow(p[i][j/3], j%3) * f[i]) / 8.0);     //-J'*f/16
    }

    for(i=0; i<9; i++)
      for(j=0; j<9; j++)
      {
        a[i][j]=0;
        for(k=0; k<n; k++)
          a[i][j]=a[i][j] + ((p2[k][i/3] * pow(p[k][i/3], i%3) * p2[k][j/3] * pow(p[k][j/3], j%3)) / 4.0);    //J'*J/16
      }

    for(k=0; k<n; k++)                                           //Comment this out to get Gauss-Newton method
      for(i=0; i<3; i++)
        for(j=0; j<3; j++)
          for(l=0; l<9; l+=3)
            a[i+l][j+l]=a[i+l][j+l]+( (f[k] * pow(p[k][l/3], i+j)) / 8.0);    //f(i)*H(i)/16, i=0..n

    disable_sensor_interrupts(); //for speed
    if(!lsolve(a, r, x2))
    {
      enable_sensor_interrupts();
      return false;
    }
    enable_sensor_interrupts();
    
    norm=abs(x2[0]);
    x[0]+=x2[0];
    for(i=1; i<n; i++)
    {
      if(abs(x2[i])>norm)
        norm=abs(x2[i]);
      x[i]+=x2[i];
    }
    
    if(++counter==0)
    {
      error("Unable to do the magic");
      return false;
    }
  }
  
  print9row("math_magic result", x);

  return true;
}

bool math_magic_float(fixed p[20][3], fixed x[9])
{
  const byte n=9;
  const byte m=20;
  float p2[m][3];
  float x2[n];
  
  for(byte i=0; i<n; i++)
    x2[i]=(x[i]==one)?1.0:(float)(x[i].value)/2147483648.0;
  
  for(byte i=0; i<m; i++)
    for(byte j=0; j<3; j++)
      p2[i][j]=(p[i][j]==one)?1.0:(float)(p[i][j].value)/2147483648.0;
  
  if(!math_magic(p2, x2))
    return false;
  
  for(byte i=0; i<n; i++)
  {
    if(x2[i]>=1.0)
    {
      error("x>one");
      x[i]=one;
    }
    if(x2[i]<=-1.0)
    {
      error("x<-one");
      x[i]=-one;
    }
    x[i]=(long)(x2[i]*2147483648.0);
  }
  
  return true;
  
}

bool accel_calibrate_manual_3() //manual accel calibration
{
  const byte n=20;
  fixed point[n][3];
  fixed k[9];
  
//  Serial.println("Manual accel calibration\nThis algorithm will estimate accel zero values\nby measuring gravity in 6 different positions.\nThe more the positions differ, the better estimation.\nThe positions are not required to be exactly aligned to gravity in any way.\nPlease do not move your quadro.");

point[0][0]=0x38E38E38;
point[0][1]=0;
point[0][2]=0;

point[1][0]=0;
point[1][1]=gravity;
point[1][2]=0;

point[2][0]=0;
point[2][1]=0;
point[2][2]=gravity;

point[3][0]=-0x38E38E38;
point[3][1]=0;
point[3][2]=0;

point[4][0]=0;
point[4][1]=-gravity;
point[4][2]=0;

point[5][0]=0;
point[5][1]=0;
point[5][2]=-gravity;

point[6][0]=0x2839FD28;
point[6][1]=0x2D413CCD;
point[6][2]=0;

point[7][0]=0x2839FD28;
point[7][1]=0;
point[7][2]=-0x2D413CCD;

point[8][0]=0;
point[8][1]=0x2D413CCD;
point[8][2]=0x2D413CCD;

point[9][0]=-0x2839FD28;
point[9][1]=-0x2D413CCD;
point[9][2]=0;


point[10][0]=0x2839FD28;
point[10][1]=-0x2D413CCD;
point[10][2]=0;

point[11][0]=-0x2839FD28;
point[11][1]=0x2D413CCD;
point[11][2]=0;

point[12][0]=-0x2839FD28;
point[12][1]=0;
point[12][2]=0x2D413CCD;

point[13][0]=0x2839FD28;
point[13][1]=0;
point[13][2]=0x2D413CCD;

point[14][0]=-0x2839FD28;
point[14][1]=0;
point[14][2]=-0x2D413CCD;

point[15][0]=0;
point[15][1]=-0x2D413CCD;
point[15][2]=-0x2D413CCD;

point[16][0]=0;
point[16][1]=0x2D413CCD;
point[16][2]=-0x2D413CCD;

point[17][0]=0;
point[17][1]=-0x2D413CCD;
point[17][2]=0x2D413CCD;

point[18][0]=0x20D845D1;
point[18][1]=0x24F34E8B;
point[18][2]=0x24F34E8B;

point[19][0]=-0x20D845D1;
point[19][1]=-0x24F34E8B;
point[19][2]=-0x24F34E8B;


  for(byte i=0; i<3; i++)
  {
    accel_square[i]=0;
    accel_gain[i]=0;
    accel_offset[i]=0;
  }

/*  for(byte i=0;i<n;i++)
  {
    serialEvent();
    serialEvent2();
    Serial.print("Waiting for position "); Serial.print(i+1); Serial.println("...");
    delay(500);
    accel_capture_wait();
    for(byte j=0;j<3;j++)
      point[i][j]=accel_captured[j];
    printpoint(point[i]);
    print("sqrt", sqrt(point[i][0]%point[i][0]+point[i][1]%point[i][1]+point[i][2]%point[i][2]));
    print("sq-g", sqrt(point[i][0]%point[i][0]+point[i][1]%point[i][1]+point[i][2]%point[i][2]) - gravity);
    accel_cum=accel_cum+(sqrt(point[i][0]%point[i][0]+point[i][1]%point[i][1]+point[i][2]%point[i][2])-gravity);
    print("acum",accel_cum);
  }
  Serial.println("Done. Thanks. Now calculating...");
*/

  for(byte i=0; i<n; i++)
  {
    printpoint("point", point[i]);
    print("sqrt", sqrt(point[i][0]%point[i][0]+point[i][1]%point[i][1]+point[i][2]%point[i][2]));
  }

  if(!math_magic(point, k)) // find gain to be applied for the n points to be on a sphere
    {error("Cannot find calibration parameters. Possible reasons: programmer was drunk. Please try again.");
    delay(5000);
    return false;
    }

  print9row("k",k);

  for(byte i=0; i<3; i++)
  {
    accel_offset[i]=k[i];
    accel_gain[i]=k[i+3];
    accel_square[i]=k[i+6];
  }

  for(byte i=0; i<n; i++)
    for(byte j=0; j<3; j++)
      point[i][j]=point[i][j]+accel_square[j]*sq(point[i][j])+accel_gain[j]*point[i][j]+accel_offset[j];

  for(byte i=0; i<n; i++)
  {
    printpoint("point", point[i]);
    print("sqrt", sqrt(point[i][0]%point[i][0]+point[i][1]%point[i][1]+point[i][2]%point[i][2]));
  }

  delay(1500);
  
  return true;
}

