void calibrate_orientation(void)
{
  quaternion gyroq, accq;
  fixed arr[3];
  quaternion q1, q2, tmp;
  if(!gyro_ready || !accel_ready)
  {
    Serial.println("Vaiting for sensors to be ready");
    while (!gyro_ready || !accel_ready)
      continue;
    Serial.println("Sensors ready");
  }

  gyro_orientation=ident;
  accel_orientation=ident;
  
  Serial.println("Waiting for the position 1");
  accel_capture_wait();
  gyroq=imu_get_orientation().normalize();
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
  
  Serial.println("Waiting for the position 2");
  accel_capture_wait();
  tmp=imu_get_orientation().normalize();
  vectnorm(accel_captured);
  print("gyrop2", tmp);
  gyroq=tmp*conjugate(gyroq);
  if(gyroq.w<0)
    gyroq=-gyroq;
  
  tmp=conjugate(tmp)*quaternion(one,0,0)*tmp;
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
  accq=q1*q2*conjugate(accq);
  if(accq.w<0)
    accq=-accq;

  print("accp2",q1*q2);
  
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
  
  if(gyroq.z*accq.z<0)
    Serial.println("Warning! It seems you have rotated not around Z axis!");
  
  if(gyroq.w>sinpi4 || accq.w>sinpi4)
    Serial.println("Warning! The positions are too near!");
  
}

/*
pos = [ 0.011 ( 24509610 ), 0.088 ( 189251900 ), 0.040 ( 87732620 ), 0.995 ( 2137187870 )]
gyro_orientation = [ 0.998 ( 2144943004 ), 0.020 ( 43921129 ),-0.044 (-94744204 ), 0.000 ( 0 )]
accq = [ 0.009 ( 19639479 ),-0.002 (-5473734 ), 0.006 ( 13782806 ), 0.999 ( 2147342632 )]
accel_orientation = [ 0.999 ( 2147470845 ), 0.003 ( 6891732 ), 0.001 ( 2736997 ), 0.000 ( 0 )]

pos = [ 0.058 ( 125092414 ),-0.089 (-192095390 ),-0.038 (-82606206 ),-0.993 (-2133615121 )]
gyro_orientation = [ 0.998 ( 2144922256 ), 0.019 ( 41422762 ),-0.044 (-96325955 ), 0.000 ( 0 )]
accq = [ 0.061 ( 131987297 ), 0.001 ( 2470972 ),-0.004 (-10671792 ),-0.998 (-2143395760 )]
accel_orientation = [ 0.999 ( 2147476637 ), 0.002 ( 5346019 ), 0.000 ( 1237830 ), 0.000 ( 0 )]

pos = [ 0.054 ( 117078590 ),-0.092 (-198683087 ),-0.026 (-56209000 ),-0.993 (-2134325235 )]
gyro_orientation = [ 0.998 ( 2144987352 ), 0.013 ( 28179117 ),-0.046 (-99605295 ), 0.000 ( 0 )]
accq = [ 0.058 ( 126105725 ),-0.001 (-3467510 ), 0.005 ( 12339692 ),-0.998 (-2143739508 )]
accel_orientation = [ 0.999 ( 2147474052 ),-0.002 (-6180539 ),-0.000 (-1736760 ), 0.000 ( 0 )]

gyroq = [ 0.024 ( 52175300 ),-0.092 (-199332832 ),-0.038 (-82156523 ),-0.994 (-2135996368 )]
gyro_orientation = [ 0.998 ( 2144767789 ), 0.019 ( 41142423 ),-0.046 (-99822087 ), 0.000 ( 0 )]
accq = [ 0.027 ( 58713828 ),-0.000 (-1364328 ),-0.004 (-10691804 ),-0.999 (-2146653800 )]
accel_orientation = [ 0.999 ( 2147476881 ), 0.002 ( 5347918 ),-0.000 (-682421 ), 0.000 ( 0 )]

gyroq = [ 0.130 ( 280567600 ),-0.089 (-193248678 ),-0.030 (-64892273 ),-0.986 (-2119295116 )]
gyro_orientation = [ 0.998 ( 2145015677 ), 0.015 ( 32764302 ),-0.045 (-97571836 ), 0.000 ( 0 )]
accq = [ 0.135 ( 289995111 ), 0.001 ( 2913344 ), 0.000 ( 351584 ),-0.990 (-2127811139 )]
accel_orientation = [ 0.999 ( 2147483137 ),-0.000 (-177417 ), 0.000 ( 1470138 ), 0.000 ( 0 )]

gyroq = [ 0.122 ( 263629548 ), 0.097 ( 208486229 ), 0.034 ( 74980354 ), 0.987 ( 2119692645 )]
gyro_orientation = [ 0.998 ( 2144572744 ), 0.017 ( 37827184 ),-0.048 (-105180176 ), 0.000 ( 0 )]
accq = [ 0.121 ( 260796929 ), 0.004 ( 9344466 ), 0.001 ( 3431537 ), 0.992 ( 2131565594 )]
accel_orientation = [ 0.999 ( 2147477794 ), 0.000 ( 1728567 ),-0.002 (-4707086 ), 0.000 ( 0 )]

gyroq = [ 0.056 ( 121772889 ),-0.090 (-194249548 ),-0.028 (-61259390 ),-0.993 (-2134331695 )]
gyro_orientation = [ 0.998 ( 2145054214 ), 0.014 ( 30713804 ),-0.045 (-97391479 ), 0.000 ( 0 )]
accq = [ 0.060 ( 130793569 ), 0.000 ( 1909981 ), 0.005 ( 12250709 ),-0.998 (-2143461066 )]
accel_orientation = [ 0.999 ( 2147474666 ),-0.002 (-6136773 ), 0.000 ( 956770 ), 0.000 ( 0 )]

gyroq = [ 0.052 ( 112948266 ), 0.092 ( 198402469 ), 0.039 ( 85832805 ), 0.993 ( 2133588034 )]
gyro_orientation = [ 0.998 ( 2144747304 ), 0.020 ( 43030715 ),-0.046 (-99465470 ), 0.000 ( 0 )]
accq = [ 0.050 ( 109229450 ), 0.000 ( 766860 ), 0.007 ( 15217849 ), 0.998 ( 2144649799 )]
accel_orientation = [ 0.999 ( 2147470099 ), 0.003 ( 7618834 ),-0.000 (-383929 ), 0.000 ( 0 )]

gyroq = [ 0.033 ( 71877484 ),-0.091 (-196311306 ),-0.028 (-61542244 ),-0.994 (-2136397452 )]
gyro_orientation = [ 0.998 ( 2145010096 ), 0.014 ( 30823876 ),-0.045 (-98323933 ), 0.000 ( 0 )]
accq = [ 0.037 ( 80681544 ),-0.000 (-617590 ), 0.006 ( 14656430 ),-0.999 (-2145917361 )]
accel_orientation = [ 0.999 ( 2147471104 ),-0.003 (-7333435 ),-0.000 (-309015 ), 0.000 ( 0 )]


*/


