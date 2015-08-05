bool manual_calibration(void)
{
  return accel_calibrate_manual() && calibrate_orientation();
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

#define ACSQUARE
#ifndef ACSQUARE
fixed f(fixed newpoint[3])
{
  return sqrt(lsq(newpoint[0])+lsq(newpoint[1])+lsq(newpoint[2]))-gravity;
}

fixed df(fixed point[3], fixed newpoint[3], byte i, byte l)
{
  return (pow(point[i/l], i%l) % newpoint[i/l]) / (f(newpoint)+gravity);
}
#else
fixed f(fixed newpoint[3])
{
  return sq(newpoint[0])+sq(newpoint[1])+sq(newpoint[2])-sq(gravity);
}

fixed df(fixed point[3], fixed newpoint[3], byte i, byte l)
{
  return (pow(point[i/l], i%l) * newpoint[i/l])<<1;
}
#endif

bool math_magic(fixed point[20][3], byte n, fixed x[9])          //finds best calibration parameters using the least squares method and iterational Newton's method
{
  const byte l=3;
  fixed F[3*l][3*l];
  fixed r[3*l];
  fixed dx[3*l];
  fixed norm=one;
  const fixed eps=1;
  byte i, j, k, counter=0;
  const fixed d=(1UL<<31)/n;
  fixed newpoint[20][3];

  for(i=0; i<3*l; i++)
    x[i]=0;

  while(norm>eps)
  {
    for(i=0; i<n; i++)
      for(j=0; j<3; j++)
      {
        newpoint[i][j]=point[i][j];
        for(k=0; k<l; k++)
          newpoint[i][j]=newpoint[i][j]+x[k]*pow(point[i][j], k);
      }

    for(i=0; i<3*l; i++)
      for(j=0; j<3*l; j++)
      {
        F[i][j]=0;
        for(k=0; k<n; k++)
        {
          //printf("df=%g\n", df(point[k], i));
          F[i][j]=F[i][j]+df(point[k], newpoint[k], i, l)*df(point[k], newpoint[k], j, l)*d;
        }
      }

    for(i=0; i<3*l; i++)
    {
      r[i]=0;
      for(k=0; k<n; k++)
        r[i]=r[i]-df(point[k], newpoint[k], i, l)*f(newpoint[k])*d;
    }

    if(!lsolve(F, r, dx))
    {
      printf("Unable to solve\n");
      return false;
    }

    norm=0;
    for(i=0; i<3*l; i++)
    {
      norm=norm+sq(dx[i]);
      x[i]=x[i]+dx[i];
    }

    if(++counter==0)
    {
      error("Unable to do the magic");
      return false;
    }

    Serial.print("math_magic "); Serial.print(counter); printrow(" pass", x);
  }

  return true;
}

bool accel_calibrate_manual(void)
{
  const byte n=12;
  fixed point[20][3];
  fixed k[9];
  
//  Serial.println("Manual accel calibration\nThis algorithm will estimate accel zero values\nby measuring gravity in several different positions.\nThe more the positions differ, the better estimation.\nThe positions are not required to be exactly aligned to gravity in any way.\nPlease do not move your quadro while position is being measured.");
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
  }
*/
  Serial.println("Done. Thanks. Now calculating...");

  for(byte i=0;i<n;i++)
  {
    Serial.print("point["); Serial.print(i); Serial.print("][0]="); Serial.print(point[i][0].value); Serial.print(";\n");
    Serial.print("point["); Serial.print(i); Serial.print("][1]="); Serial.print(point[i][1].value); Serial.print(";\n");
    Serial.print("point["); Serial.print(i); Serial.print("][2]="); Serial.print(point[i][2].value); Serial.print(";\n\n");
  }

  for(byte i=0; i<n; i++)
  {
    printpoint("point", point[i]);
    print("sqrt", sqrt(lsq(point[i][0])+lsq(point[i][1])+lsq(point[i][2])));
  }
  
  disable_sensor_interrupts(); //for speed
  if(!math_magic(point, n, k)) // find gain to be applied for the n points to be on a sphere
  {
    enable_sensor_interrupts();
    error("Cannot find calibration parameters. Possible reasons: programmer was drunk. Please try again.");
    delay(5000);
    return false;
  }
  enable_sensor_interrupts();

  printrow("math_magic result", k);

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


// cross accel and gyro axis misallignment compensation

bool calibrate_orientation(void)
{
  quaternion gyroq, accq, cmbq;
  fixed arr[3];
  quaternion q1, q2, tmp;
  if(!gyro_ready || !accel_ready)
  {
    Serial.println("Vaiting for sensors to be ready");
    while (!gyro_ready || !accel_ready)
      continue;
    Serial.println("Sensors ready");
  }

//  gyro_orientation=ident;
//  accel_orientation=ident;
  
  Serial.println("Waiting for the position 1");
  accel_capture_wait();
  gyroq=imu_get_orientation();
  gyroq.normalize();
  vectnorm(accel_captured);
  print("gyrop1", gyroq);
  
  tmp=conjugate(gyroq)*quaternion(one,0,0)*gyroq;
  arr[0]=tmp.x;
  arr[1]=tmp.y;
  arr[2]=tmp.z;

  q2=sqrt(quaternion(accel_captured[2], accel_captured[1], -accel_captured[0], 0));
  q1=q2*quaternion(arr[0], arr[1], arr[2])*conjugate(q2);

  arr[0]=q1.x;
  arr[1]=q1.y;
  arr[2]=0;
  vectnorm(arr);

  q1=sqrt(quaternion(arr[0], 0, 0, -arr[1]));
  accq=q1*q2;
  
  print("accp1",accq);
  
  cmbq=gyroq*conjugate(accq);
  
  print("cmbp1", cmbq);
  
  print("conjugate(gyrop1)*accp1", conjugate(gyroq)*accq);
  
  Serial.println("Waiting for the position 2");
  accel_capture_wait();
  tmp=accel_captured_imu;//imu_get_orientation();
  tmp.normalize();
  vectnorm(accel_captured);
  print("gyrop2", tmp);
  gyroq=conjugate(gyroq)*tmp;
  if(gyroq.w<0)
    gyroq=-gyroq;
  
  tmp=conjugate(tmp)*quaternion(one,0,0)*tmp;
  arr[0]=tmp.x;
  arr[1]=tmp.y;
  arr[2]=tmp.z;
  
  print("arr[0]", arr[0]);
  print("arr[1]", arr[1]);
  print("arr[2]", arr[2]);
  
  q2=sqrt(quaternion(accel_captured[2], accel_captured[1], -accel_captured[0], 0));
  q1=q2*quaternion(arr[0], arr[1], arr[2])*conjugate(q2);

  arr[0]=q1.x;
  arr[1]=q1.y;
  arr[2]=0;
  vectnorm(arr);

  q1=sqrt(quaternion(arr[0], 0, 0, -arr[1]));
  accq=conjugate(accq)*q1*q2;
  if(accq.w<0)
    accq=-accq;

  print("accp2",q1*q2);

  tmp=accel_captured_imu;
  tmp.normalize();  
  cmbq=conjugate(cmbq)*(tmp*conjugate(accq));
  
  print("cmbp2", tmp*conjugate(accq));
  
  print("conjugate(gyrop2)*accp2", conjugate(tmp)*(q1*q2));
  
  print("sqrt", sqrt(conjugate(tmp)*(q1*q2)));
  
  print("g_o*sqrt",gyro_orientation*sqrt(conjugate(tmp)*(q1*q2)));
  
  arr[0]=(conjugate(tmp)*(q1*q2)).x;
  arr[1]=(conjugate(tmp)*(q1*q2)).y;
  arr[2]=(conjugate(tmp)*(q1*q2)).z;

  if(arr[2]<0)
  {
    arr[0]=-arr[0];
    arr[1]=-arr[1];
    arr[2]=-arr[2];
  }

  vectnorm(arr);
  print("arr[0]", arr[0]);
  print("arr[1]", arr[1]);
  print("arr[2]", arr[2]);

  
  arr[0]=gyroq.x;
  arr[1]=gyroq.y;
  arr[2]=gyroq.z;

  if(arr[2]<0)
  {
    arr[0]=-arr[0];
    arr[1]=-arr[1];
    arr[2]=-arr[2];
  }

  vectnorm(arr);
  
  gyro_orientation=sqrt(quaternion(arr[2], arr[1], -arr[0], 0));
  
  print("gyroq",gyroq);
  print("gyro_orientation", gyro_orientation);
  
  arr[0]=accq.x;
  arr[1]=accq.y;
  arr[2]=accq.z;

  if(arr[2]<0)
  {
    arr[0]=-arr[0];
    arr[1]=-arr[1];
    arr[2]=-arr[2];
  }

  vectnorm(arr);
  
  accel_orientation=sqrt(quaternion(arr[2], arr[1], -arr[0], 0));
  
  print("accq",accq);
  print("accel_orientation", accel_orientation);
  
  print("g*-a", gyro_orientation*conjugate(accel_orientation));
  
  arr[0]=cmbq.x;
  arr[1]=cmbq.y;
  arr[2]=cmbq.z;

  if(arr[2]<0)
  {
    arr[0]=-arr[0];
    arr[1]=-arr[1];
    arr[2]=-arr[2];
  }

  vectnorm(arr);
  
  print("cmbq", cmbq);
  
  cmbq=sqrt(quaternion(arr[2], arr[1], -arr[0], 0));
  
  print("combined_orientation", cmbq);
  
  if(gyroq.z*accq.z<0)
    Serial.println("Warning! It seems you have rotated not around Z axis!");
  
  if(gyroq.w>sinpi4 || accq.w>sinpi4)
    Serial.println("Warning! The positions are too near!");
  
  gyro_orientation=ident;

  return true;
}

/*

conjugate(gyrop2)*accp2 = [ 0.999 ( 2146874001 ), 0.005 ( 12116299 ),-0.022 (-48046977 ),-0.005 (-12756628 )]
sqrt = [ 0.999 ( 2147331230 ), 0.002 ( 6058579 ),-0.011 (-24025194 ),-0.002 (-6378767 )]


conjugate(gyrop2)*accp2 = [ 0.999 ( 2146658204 ),-0.024 (-52185602 ),-0.012 (-27816917 ),-0.003 (-6888770 )]
sqrt = [ 0.999 ( 2147277277 ),-0.012 (-26095309 ),-0.006 (-13909796 ),-0.001 (-3444716 )]

conjugate(gyrop2)*accp2 = [ 0.999 ( 2146678769 ),-0.025 (-53726937 ),-0.010 (-23269881 ),-0.002 (-5311203 )]
sqrt = [ 0.999 ( 2147282419 ),-0.012 (-26865986 ),-0.005 (-11636031 ),-0.001 (-2655851 )]

conjugate(gyrop2)*accp2 = [ 0.999 ( 2146654910 ),-0.024 (-52863175 ),-0.012 (-26907346 ),-0.002 (-6339983 )]
sqrt = [ 0.999 ( 2147276453 ),-0.012 (-26434138 ),-0.006 (-13454971 ),-0.001 (-3170298 )]

conjugate(gyrop2)*accp2 = [ 0.999 ( 2146724214 ),-0.023 (-50826609 ),-0.011 (-25374331 ),-0.002 (-5828998 )]
sqrt = [ 0.999 ( 2147293780 ),-0.011 (-25415552 ),-0.005 (-12688288 ),-0.001 (-2914757 )]


conjugate(gyrop2)*accp2 = [ 0.999 ( 2147471984 ), 0.003 ( 7025767 ), 0.000 ( 858539 ), 0.000 ( 16571 )]
sqrt = [ 0.999 ( 2147480732 ), 0.001 ( 3512888 ), 0.000 ( 429270 ), 0.000 ( 8285 )]
g_o*sqrt = [ 0.999 ( 2147319844 ),-0.010 (-22582089 ),-0.006 (-13486084 ),-0.001 (-3418891 )]

conjugate(gyrop2)*accp2 = [ 0.999 ( 2147483145 ),-0.000 (-609221 ), 0.000 ( 1293938 ), 0.000 ( 345300 )]
sqrt = [ 0.999 ( 2147483522 ),-0.000 (-304611 ), 0.000 ( 646969 ), 0.000 ( 172650 )]
g_o*sqrt = [ 0.999 ( 2147277918 ),-0.012 (-26399971 ),-0.006 (-13260304 ),-0.001 (-3281918 )]

conjugate(gyrop2)*accp2 = [ 0.999 ( 2147481568 ),-0.000 (-1186495 ),-0.001 (-2660501 ),-0.000 (-674092 )]
sqrt = [ 0.999 ( 2147483128 ),-0.000 (-593248 ),-0.000 (-1330251 ),-0.000 (-337046 )]
g_o*sqrt = [ 0.999 ( 2147260393 ),-0.012 (-26688444 ),-0.007 (-15243061 ),-0.001 (-3769408 )]

conjugate(gyrop2)*accp2 = [ 0.999 ( 2147479717 ),-0.001 (-2948463 ), 0.001 ( 2760017 ), 0.000 ( 756719 )]
sqrt = [ 0.999 ( 2147482665 ),-0.000 (-1474233 ), 0.000 ( 1380009 ), 0.000 ( 378359 )]
g_o*sqrt = [ 0.999 ( 2147267926 ),-0.012 (-27569627 ),-0.005 (-12522952 ),-0.001 (-3092711 )]



*/


