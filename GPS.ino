void gps_init(void)  // a blocking function that waits until the fix is established, Not required.
{
  while(!gps_ready)
    serialEvent3();
}

void gps_int(void)
{
  unsigned long fix_age;
  static fixed lat_zero=0, lon_zero=0;
  gps.get_position(&gps_lat_abs.value, &gps_lon_abs.value, &fix_age);
  gps.get_datetime(&gps_date, &gps_time, NULL);

  gps_lat=gps_lat_abs-lat_zero;   // Warning! This should be rewritten if you use it on a North or South Pole!
  gps_lon=gps_lon_abs-lon_zero;
  
  print("latitude",gps_lat_abs);
  
  if(!gps_ready && fix_age != TinyGPS::GPS_INVALID_AGE && gps_date!=TinyGPS::GPS_INVALID_DATE && gps_time!=TinyGPS::GPS_INVALID_TIME && gps_lat_abs!=TinyGPS::GPS_INVALID_ANGLE && gps_lon_abs!=TinyGPS::GPS_INVALID_ANGLE)
  {
    lat_zero=gps_lat_abs;
    lon_zero=gps_lon_abs;
    gps_lat=0;
    gps_lon=0;
    gps_ready=true;
  Serial.println("GPS fix established");
  Serial.print("Date: ");
  Serial.println(gps_date);
  Serial.print("Time: ");
  Serial.println(gps_time);
  print("latitude",gps_lat_abs);
  print("longitude",gps_lon_abs);    
  }
}
