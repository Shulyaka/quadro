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
  //print("lcheck", s);
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
  //  x2[i]=x[i];        //assuming that the provided ones are close enough to the real
    x2[i]=0;           //assuming that the provided ones are dummy
  
  //printmatrix("a", a);
  //print9row("r", r);
  
  while(norm>=eps)
  {
    //print9row("lsolve x", x);
    
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
    //print("norm", norm);
    
    if(--counter==0)
    {
      error("Unable to solve the system");
      return false;
    }
  }
  
  //print("counter", counter);
  //print9row("solved! x", x);

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

#define ACSQUARE
#ifndef ACSQUARE
fixed f(fixed point[3], fixed x[9])
{
  fixed newpoint[3] = { point[0]+x[0]+x[1]*point[0]+x[2]*sq(point[0]),
                        point[1]+x[3]+x[4]*point[1]+x[5]*sq(point[1]),
                        point[2]+x[6]+x[7]*point[2]+x[8]*sq(point[2])};

  return sqrt(lsq(newpoint[0])+lsq(newpoint[1])+lsq(newpoint[2]))-gravity;
}

fixed df(fixed point[3], fixed x[9], byte i)
{
  fixed newpoint[3] = { point[0]+x[0]+x[1]*point[0]+x[2]*sq(point[0]),
                        point[1]+x[3]+x[4]*point[1]+x[5]*sq(point[1]),
                        point[2]+x[6]+x[7]*point[2]+x[8]*sq(point[2])};

  return (pow(point[i/3], i%3) % newpoint[i/3]) / (f(point, x)+gravity);
}
#else
fixed f(fixed point[3], fixed x[9])
{
  fixed newpoint[3] = { point[0]+x[0]+x[1]*point[0]+x[2]*sq(point[0]),
                        point[1]+x[3]+x[4]*point[1]+x[5]*sq(point[1]),
                        point[2]+x[6]+x[7]*point[2]+x[8]*sq(point[2])};

  return sq(newpoint[0])+sq(newpoint[1])+sq(newpoint[2])-sq(gravity);
}

fixed df(fixed point[3], fixed x[9], byte i)
{
  fixed newpoint[3] = { point[0]+x[0]+x[1]*point[0]+x[2]*sq(point[0]),
                        point[1]+x[3]+x[4]*point[1]+x[5]*sq(point[1]),
                        point[2]+x[6]+x[7]*point[2]+x[8]*sq(point[2])};

  return (pow(point[i/3], i%3) * newpoint[i/3])<<1;
}
#endif

bool math_magic(fixed point[12][3], fixed x[9])          //finds the best calibration parameters using the least squires method and iterational Newton's method
{
  const byte n=12;
  fixed F[9][9];
  fixed r[9];
  fixed dx[9]={0};
  fixed norm=one;
  const fixed eps=1;
  byte i, j, k, counter=0;
  const fixed d=(1UL<<31)/n;

  for(i=0; i<9; i++)
    x[i]=0;

  while(norm>eps)
  {
    for(i=0; i<9; i++)
      for(j=0; j<9; j++)
      {
        F[i][j]=0;
        for(k=0; k<n; k++)
        {
          //printf("df=%g\n", df(point[k], x, i));
          F[i][j]=F[i][j]+df(point[k], x, i)*df(point[k], x, j)*d;
        }
      }

    for(i=0; i<9; i++)
    {
      r[i]=0;
      for(k=0; k<n; k++)
        r[i]=r[i]-df(point[k], x, i)*f(point[k], x)*d;
    }

    disable_sensor_interrupts(); //for speed
    if(!lsolve(F, r, dx))
    {
      enable_sensor_interrupts();
      printf("Unable to solve\n");
      return false;
    }
    enable_sensor_interrupts();

    norm=0;
    for(i=0; i<9; i++)
    {
      norm=norm+sq(dx[i]);
      x[i]=x[i]+dx[i];
    }

    if(++counter==0)
    {
      error("Unable to do the magic");
      return false;
    }

    Serial.print("math_magic "); Serial.print(counter); print9row(" pass", x);
  }

  print9row("math_magic result", x);

  return true;
}

bool accel_calibrate_manual_3() //manual accel calibration
{
  const byte n=12;
  fixed point[n][3];
  fixed k[9];
  
  Serial.println("Manual accel calibration\nThis algorithm will estimate accel zero values\nby measuring gravity in 6 different positions.\nThe more the positions differ, the better estimation.\nThe positions are not required to be exactly aligned to gravity in any way.\nPlease do not move your quadro.");
/*
point[0][0]=0x3BF51698;
point[0][1]=-0x0930C4C4;
point[0][2]=-0x16D78C44;

point[1][0]=-0x3157470F;
point[1][1]=-0x27D7CB2E;
point[1][2]=0x0B533D95;

point[2][0]=0x1D771866;
point[2][1]=-0x3727CBF7;
point[2][2]=-0xC2D948E;

point[3][0]=-0x06AC62CA;
point[3][1]=0x3EF05A71;
point[3][2]=-0x0348D55C;

point[4][0]=0x33E85FD1;
point[4][1]=0x1CAD9275;
point[4][2]=-0x190DA5DB;

point[5][0]=-0x1194F26B;
point[5][1]=0x2EA0F4D8;
point[5][2]=0x269014B6;

point[6][0]=-0x2E2DF938;
point[6][1]=-0x2757B41C;
point[6][2]=-0x14F6273A;

point[7][0]=0x1C576CCF;
point[7][1]=0x38D10F52;
point[7][2]=0x0556308F;

point[8][0]=-0x405015C3;
point[8][1]=-0x09C2798D;
point[8][2]=-0x04AE2619;

point[9][0]=0x19C193B4;
point[9][1]=0x32E71CDB;
point[9][2]=0x1BAFAF86;

*/

point[0][0]=-62404608;
point[0][1]=86130176;
point[0][2]=-1073431552;

point[1][0]=15958528;
point[1][1]=-26644992;
point[1][2]=1073015808;

point[2][0]=-3159552;
point[2][1]=774172160;
point[2][2]=733757440;

point[3][0]=-418952192;
point[3][1]=613997056;
point[3][2]=768361984;

point[4][0]=-613409280;
point[4][1]=230033920;
point[4][2]=854765568;

point[5][0]=-666322432;
point[5][1]=-376044032;
point[5][2]=760500736;

point[6][0]=-378470400;
point[6][1]=-669841408;
point[6][2]=759221760;

point[7][0]=315152384;
point[7][1]=-695069696;
point[7][2]=764981760;

point[8][0]=551900672;
point[8][1]=-643232256;
point[8][2]=-657971712;

point[9][0]=-597839872;
point[9][1]=-308036608;
point[9][2]=-833975808;

point[10][0]=-465549824;
point[10][1]=433178112;
point[10][2]=-871903744;

point[11][0]=-33232384;
point[11][1]=773597696;
point[11][2]=-755563520;


/*
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
*/

  for(byte i=0; i<3; i++)
  {
    accel_offset[i]=0;
    accel_gain[i]=0;
    accel_square[i]=0;
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
    print("sqrt", sqrt(lsq(point[i][0])+lsq(point[i][1])+lsq(point[i][2])));
    print("sq-g", sqrt(lsq(point[i][0])+lsq(point[i][1])+lsq(point[i][2])) - gravity);
    accel_cum=accel_cum+(sqrt(lsq(point[i][0])+lsq(point[i][1])+lsq(point[i][2]))-gravity);
    print("acum",accel_cum);
  }
*/
  Serial.println("Done. Thanks. Now calculating...");

  for(byte i=0;i<n;i++)
  {
    Serial.print("point["); Serial.print(i); Serial.print("][0]="); Serial.print(point[i][0].value); Serial.println(";");
    Serial.print("point["); Serial.print(i); Serial.print("][1]="); Serial.print(point[i][1].value); Serial.println(";");
    Serial.print("point["); Serial.print(i); Serial.print("][2]="); Serial.print(point[i][2].value); Serial.println(";\n");
  }

  for(byte i=0; i<n; i++)
  {
    printpoint("point", point[i]);
    print("sqrt", sqrt(lsq(point[i][0])+lsq(point[i][1])+lsq(point[i][2])));
  }

  if(!math_magic(point, k)) // find gain to be applied for the n points to be on a sphere
    {error("Cannot find calibration parameters. Possible reasons: programmer was drunk. Please try again.");
    delay(5000);
    return false;
    }

  print9row("k",k);

  for(byte i=0; i<3; i++)
  {
    accel_offset[i]=k[i*3];
    accel_gain[i]=k[i*3+1];
    accel_square[i]=k[i*3+2];
  }

  for(byte i=0; i<n; i++)
    for(byte j=0; j<3; j++)
      point[i][j]=point[i][j]+accel_square[j]*sq(point[i][j])+accel_gain[j]*point[i][j]+accel_offset[j];

  for(byte i=0; i<n; i++)
  {
    printpoint("point", point[i]);
    print("sqrt", sqrt(lsq(point[i][0])+lsq(point[i][1])+lsq(point[i][2])));
  }

  delay(1500);
  
  return true;
}

