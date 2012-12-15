#ifndef QUADRO_H
#define QUADRO_H

#define LampPin    13
#define AudioPin   12
#define LightPin   A0
#define TempPin    A1
#define PreasPin   A2
#define HumidPin   A3

bool debug=false;

#define FINDZERO 128
#define GYROCNTP 5
#define ACCELCNT 32

void disable_sensor_interrupts()
{
  EIMSK &= ~(1 << INT3); //gyro
  EIMSK &= ~(1 << INT2); //accel
}

void enable_sensor_interrupts() //warning: only use when all interrupt vectors are set correctly
{
  EIMSK |= (1 << INT3); //gyro
  EIMSK |= (1 << INT2); //accel
}

void dummy_int(void)
  {return;}

void print(fixed val)
{
  if(val!=one)
  {
    if(val>=0) Serial.print(" ");
    Serial.print((double)val.value/65536/32768);
    Serial.print(" (");
    if(val>=0) Serial.print(" ");
    Serial.print(val.value);
    Serial.println(" )");
  }
  else
    Serial.print(" 1.00 ( one )");
}

void print(const char *name, fixed val)
{
  Serial.print(name);
  Serial.print(" = ");
  if(val!=one)
  {
    if(val>=0) Serial.print(" ");
    Serial.print((double)val.value/65536/32768);
    Serial.print(" (");
    if(val>=0) Serial.print(" ");
    Serial.print(val.value);
    Serial.println(" )");
    //Serial.println(val.value,BIN);
  }
  else
    Serial.println(" 1.00 ( one )");
}
void print(const char *name, angle val)
{
  Serial.print(name);
  Serial.print(" = ");
  if(val>=0) Serial.print(" ");
  Serial.print((double)val/115);
  Serial.print(" (");
  if(val>=0) Serial.print(" ");
  Serial.print(val);
  Serial.println(" )");
}

void print(const char *name, lfixed val)
{
  unsigned long long a=val.value;
  Serial.print(name);
  Serial.print(" = ");
  if(val>=0) Serial.print(" ");
    Serial.print((double)val.value/65536/32768/65536/32768);
    Serial.print(" (");
    for(byte i=63;i!=255;i--)
    {
      Serial.print((byte)(a>>i),BIN);
      a=a-((a>>i)<<i);
    }
    Serial.println(")");
}

void print(const char *name, quaternion val)
{
  Serial.print(name);
  Serial.print(" = [");
  print(val.x);
  Serial.print(",");
  print(val.y);
  Serial.print(",");
  print(val.z);
  Serial.print(",");
  print(val.w);
  Serial.println("]");
}

#endif
