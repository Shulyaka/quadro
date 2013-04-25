void calibrate_orientation(void)
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


