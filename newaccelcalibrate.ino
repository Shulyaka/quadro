void printrow(fixed a[9])
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

void printrow(const char *name, fixed a[9])
{
  Serial.print(name);
  Serial.print(":");
  printrow(a);
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
    printrow("r", r);
    printrow("x", x);
    return false;
  }
  print("lcheck", s);
  return true;
}

bool lsolve(fixed a[9][9], fixed r[9], fixed x[9])  //solves a linear system using iterational Jacobi method
{
  const byte n=9;
  fixed x2[n];
  const fixed eps=1000;
  fixed norm=one;
  byte i, j, counter=0;
  
  for(i=0; i<n, i++)   //initial values:
    x2[i]=x[i];        //assuming that the provided ones are close enough to the real
  //  x2[i]=0;           //assuming that the provided ones are dummy
  
  while(norm>=eps)
  {
    printrow("x", x);
    
    for(i=0; i<n; i++)
    {
      norm=b[i];
      for(j=0; j<n; j++)
        if(i!=j)
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

  return lcheck(a, r, x);
}

//fixed df(byte i, byte j,

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

bool math_magic(fixed p[10][3], fixed x[9], fixed g=one>>1)          //finds the best calibration parameters using the least squires mathod and iterational Newton's method
{
  byte n=10;
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
    for(i=0; i<n; i++)
    {
      p2[i][0]=p[i][0]+x[2]*p[i][0]*p[i][0]+x[1]*p[i][0]+x[0];
      p2[i][1]=p[i][1]+x[5]*p[i][0]*p[i][0]+x[4]*p[i][0]+x[3];
      p2[i][2]=p[i][2]+x[8]*p[i][0]*p[i][0]+x[7]*p[i][0]+x[6];
      f[i]=-g*g+p2[i][0]*p2[i][0]+p2[i][1]*p2[i][1]+p2[i][2]*p2[i][2];
    }

    for(j=0; j<9; j++)
    {
      r[j]=0;
      for(i=0; i<n; i++)
        r[j]=r[j] - p2[i][j/3] * pow(p[i][j/3], j%3) * f[i];     //-J'*f/4
      r[j]=r[j]>>1;
    }

    for(i=0; i<9; i++)
      for(j=0; j<9; j++)                                         //J'*J/4
      {
        a[i][j]=0;
        for(k=0; k<n; k++)
          a[i][j]=a[i][j] + p2[k][i/3] * pow(p[k][i/3], i%3) * p2[k][j/3] * pow(p[k][j/3], j%3);
      }

    for(k=0; k<n; k++)                                           //f(i)*H(i)/4, i=0..n; Comment this out to get Gauss-Newton method
      for(i=0; i<3; i++)
        for(j=0; j<3; j++)
          for(l=0; l<9; l+=3)
            a[i+l][j+l]=a[i+l][j+l]+( (f[k] * pow(p[k][l/3], i+j)) << 1);
  }
  
}
