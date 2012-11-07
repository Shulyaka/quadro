// dthis file is deprecated from my bike project, I was too lazy to remove it
#define wheellen   66   // in sm
#define ticknum    24   // num of ticks per wheel
#define interval   1000 // standby interval, in ms
#define speedgamma 100  // speed calibration coefficient, in percent
#define accelgamma 100  // accelleration calibration coefficient, in percent

#define spdBufLen 6     // The buffer should be long enough to avoid overflow while we are reading it, because we can get incorrect values. It must be greater than 1 to get speed and greater than 2 to get accelleration
unsigned long spdBuf[spdBufLen]; // the speedBuf contains the timestamps (not speed) of last speedBufLen events in milliseconds
unsigned char spdPos=0;           // to be used inside setup(), upd_spdBuf() and getSpdBuf() only

unsigned int get_speed(void)
{
  int dt;
  int k;
  unsigned int r;
  unsigned long start=micros();
  if (spdBufLen < 2)       // will never happen
    return 0;
  if (millis() - spdBuf[spdPos] > interval) // not moving
    return 0;
  dt = (get_spdBufDif(0,spdBufLen-1,spdPos)) / spdBufLen;  //this is way too simple, we need a really complex math here! Avoiding divisions if possible
  if (dt <= 0)             // invalid values in speed buffer, possibly we didn't move after startup
    return 0;              // returning 0 km/h anyway.
  k=(int)((long)36*(long)wheellen*(long)speedgamma/(long)ticknum); //this is constant, will be calculated only once during the compilation
  r=k/dt;
  if(debug)
  {
    Serial.print(micros());
    Serial.print(" is the current time, function entered at ");
    Serial.println(start);
  }
  return r;
}

int get_accel(void)
{
  int dt1,dt2;
  int k;
  unsigned char pos=spdPos;
  if (spdBufLen < 3)       //will never happen
    return 0;
  if (millis() - spdBuf[pos] > interval) // not moving
    return 0;    // note that we are not able to detect peak deceleration if the whell is suddenly stopped, but we can assume that it's already zero after [interval] ms.
                 // But if this happens in real life, the system will probably be already dead.
  dt1=get_spdBufDif(0,1,pos);        // still too simple...
  dt2=get_spdBufDif(1,2,pos);
  if (dt1 <= 0 || dt2 <= 0)// invalid values in speed buffer, possibly we didn't move after startup
    return 0;              // returning 0 m/s^2 anyway.  
  k=10000*wheellen*accelgamma/ticknum; //this is constant, will be calculated only once during the compilation
  return k*(dt2-dt1)/(dt1*dt2*(dt1+dt2));
}
/*
inline unsigned long get_spdBuf(unsigned char pos)
{
  unsigned char i;
  if (pos>spdBufLen-1)
    return 0;
  if(spdPos<pos)
    i=spdBufLen-pos+spdPos;
  else
    i=spdPos-pos;
  return spdBuf[i];
//  return (spdPos<pos) ? spdBuf[spdBufLen-pos+spdPos] : spdBuf[spdPos-pos];        // appeared to be less optimized. Strange
}
*/

unsigned long get_spdBufDif(unsigned char pos1, unsigned char pos2, unsigned char pos) //pos is sinonym for spdPos, we need it because spdPos may change in an interrupt
{
  unsigned char i,j;

  if (pos2>spdBufLen-1 || pos1>=pos2)
    return 0;
  if(pos<pos1)
    i=spdBufLen-pos1+pos;
  else
    i=pos-pos1;

  if(pos<pos2)
    j=spdBufLen-pos2+pos;
  else
    j=pos-pos2;

//  return (pos<pos2) ? spdBuf[i]-spdBuf[spdBufLen-pos2+pos] : spdBuf[i]-spdBuf[pos-pos2];        // appeared to be less optimized. Strange
  return spdBuf[i]-spdBuf[j];  // note that we don't care about overflow when substracting
}

void upd_spdBuf(void)
{
  if (++spdPos==spdBufLen)
    spdPos=0;
  spdBuf[spdPos]=millis();
  if(debug==true) // when we find the Truth
  {
    Serial.print("upd_spdBuf: ");
    Serial.println(spdBuf[spdPos]);
  }
}

void clear_spdBuf(void)
{
  unsigned char i;
  for (i=0; i<spdBufLen; spdBuf[i++]=0);
}
