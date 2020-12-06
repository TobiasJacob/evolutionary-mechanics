/**
 * @file Microtime.hpp
 * @author Ashok Srinivasan
 * @brief the code of these functions is used to get the time values that will be used to calculate the Performance in time
 * 
 * @version 1.0
 * @date 2020-12-06
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#ifndef _microtime_h_
#define _microtime_h_

extern "C" {
  
  /**
   * @brief this function returns the actual time in micro-seconds
   * 
   * @return double 
   */
  double    microtime(void);

  /**
   * @brief this function returns the difference between two time instances using microtime
   * 
   * @return double 
   */
  double    get_microtime_resolution(void);
}


#endif
