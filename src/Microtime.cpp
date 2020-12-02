#include <sys/time.h>
#include "Microtime.hpp"

#define TEST 0 /* Set to 0 except when testing the code or timer resolution */

double get_microtime_resolution(void)
{
  double time1, time2;
  time1 = microtime();
  do 
  {
    time2 = microtime();
  } while(time1==time2);

  return time2-time1;
}

double microtime(void)
{
  struct timeval t;
  
  gettimeofday(&t, 0);
  
  return 1.0e6*t.tv_sec + (double) t.tv_usec;
}