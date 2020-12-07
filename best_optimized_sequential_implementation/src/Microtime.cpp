#include <sys/time.h>
#include "Microtime.hpp"

#define TEST 0 // Set to 0 except when testing the code or timer resolution

double get_microtime_resolution(void)
{
  double time1, time2; //the two time instances will be saved in these two rispectively 
  time1 = microtime(); //gets the first instance of time in micro-seconds
  do 
  {
    time2 = microtime(); //gets the second instance of time in micro-seconds
  } while(time1==time2); //this loop will make sure that the times are different from each other

  return time2-time1; //returns the difference between the two times, that is the actual time passed between the two istants
}

double microtime(void)
{
  struct timeval t; 
  
  gettimeofday(&t, 0); //gets the actual time of the day
  
  return 1.0e6*t.tv_sec + (double) t.tv_usec;
}