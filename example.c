
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "eph_manager.h" /* remove this line for use with solsys version 2 */
#include "novas.h"


int main (void)
{

   const short int year = 2008;
   const short int month = 4;
   const short int day = 24;
   const short int leap_secs = 33;
   const short int accuracy = 0;
   short int error = 0;
   short int de_num = 0;
   
   const double hour = 10.605;
   const double ut1_utc = -0.387845;
   
   const double latitude = 42.0;
   const double longitude = -70;
   const double height = 0.0;
   const double temperature = 10.0;
   const double pressure = 1010.0;
      
	double jd_beg, jd_end,
	rat, dect, dist,zd, az, rar, decr;

   on_surface geo_loc;
   
   cat_entry dummy_star;
   
   object moon;
   
   
/*
   Make structures of type 'on_surface' and 'observer-on-surface' containing 
   the observer's position and weather (latitude, longitude, height, 
   temperature, and atmospheric pressure).
*/
   
   make_on_surface (latitude,longitude,height,temperature,pressure, &geo_loc);

   make_cat_entry ("DUMMY","xxx",0,0.0,0.0,0.0,0.0,0.0,0.0, 
      &dummy_star);
  
   if ((error = make_object (0,11,"Moon",&dummy_star, &moon)) != 0)
   {
      printf ("Error %d from make_object (Moon)\n", error);
      return (error);
   }

/*
   Open the JPL binary ephemeris file, here named "JPLEPH".
   Remove this block for use with solsys version 2.
*/

   if ((error = ephem_open ("JPLEPH", &jd_beg,&jd_end,&de_num)) != 0)
   {
		printf ("Error reading JPL ephemeris file.\n");
      return (error);
   }

   double jd_utc = julian_date (year,month,day,hour);
   double jd_tt = jd_utc + ((double) leap_secs + 32.184) / 86400.0;
   double jd_ut1 = jd_utc + ut1_utc / 86400.0;
   double delta_t = 32.184 + leap_secs - ut1_utc;

   printf("jd_utc=%f\r\n",jd_utc);
   
   if ((error = topo_planet (jd_tt,&moon,delta_t,&geo_loc,accuracy,
      &rat,&dect,&dist)) != 0)
   {
      printf ("Error %d from topo_planet.", error);
      return (error);
   }

   printf ("Moon geocentric and topocentric positions:\n");
   printf ("%15.10f        %15.10f        %15.12f\n", rat, dect, dist);

/*
   Position of the Moon in local horizon coordinates.  (Polar motion 
   ignored here.)
*/
   
   equ2hor (jd_ut1,delta_t,accuracy,0.0,0.0,&geo_loc,rat,dect,1,
      &zd,&az,&rar,&decr);

   printf ("Moon zenith distance and azimuth:\n");
   printf ("%15.10f        %15.10f\n", zd, az);
   printf ("\n");

   ephem_close();
   return (0);
}
