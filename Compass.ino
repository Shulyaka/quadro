#define compassAddress 0x1E
int compassADC[3];

void compass_init(void)
{
  disable_sensor_interrupts();
  
  if (readWhoI2C(compassAddress) != 0x10)
      error("Magnetometer not found!");
  
  updateRegisterI2C(compassAddress, 0x01, 0x20); // Gain
  delay(20);
  updateRegisterI2C(compassAddress, 0x02, 0x01); // start single conversion
  delay(100); //waiting for compass to stabilize
  
  compass_ready=true;
  
  enable_sensor_interrupts();
}

void compass_measure(void) //warning: you must call disable_sensor_interrupts() and interrupts() before attempting to call this function
{
  sendByteI2C(compassAddress, 0x03);
  Wire.requestFrom(compassAddress, 6);
  
  compassADC[0]=readWordI2C();
  compassADC[1]=readWordI2C();
  compassADC[2]=readWordI2C();
  
  updateRegisterI2C(compassAddress, 0x02, 0x01); // start single conversion
}
