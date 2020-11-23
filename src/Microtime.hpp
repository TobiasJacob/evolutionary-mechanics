#ifndef _microtime_h_
#define _microtime_h_

extern "C" {
  double    microtime(void); /* Time in micro-seconds */
  double    get_microtime_resolution(void); /* Timer resolution in micro-seconds */
}


#endif
