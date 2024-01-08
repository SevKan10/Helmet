void Load_GPS()
{
  while (Serial2.available()) 
  {
    if (gps.encode(Serial2.read())) 
    {
     Lat = gps.location.lat();
     Lng = gps.location.lng(); 
    }
  }
}
