//int sonarADC[6]={0,0,0,0,0,0};

void sonar_init(void)
{
  
}

void sonar_measure(void)
{
  static int sonar_cur=0;
  sonarADC[sonar_cur]=analogRead(sonar_cur+1);
  
  if(++sonar_cur>6)
  {
    sonar_cur=0;
  }
}
