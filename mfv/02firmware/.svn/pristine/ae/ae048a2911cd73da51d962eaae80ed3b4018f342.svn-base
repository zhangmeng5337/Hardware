#include <stdint.h>
#include <string.h>
#include "types.h"
#include "libSearch.h"

/**
	@brief Bin search function, x input direction is negative
*/
float bin_search(float x, lincurve_element *table, uint8_t max_elements, int xdirection, float def)
{
	uint8_t i=0;
	uint8_t found = 0;
	float x1,x2;
	float y1,y2;
	float m=0.0F,k=0.0F;
	
  if (xdirection == X_DIRECTION_DEC) { // decreasing x direction
    // special case where setpoint is greater than the last calibration pont
    if (x > table[0].x) {
      x1 = table[0].x;
      x2 = table[1].x;
      y1 = table[0].y;
      y2 = table[1].y;
      m = (y2 - y1)/(x2 - x1);
      k = y2 - m * x2;
      return m * x + k;		
    } else if (x <= table[max_elements-1].x) { 
      x1 = table[max_elements-2].x;
      x2 = table[max_elements-1].x;
      y1 = table[max_elements-2].y;
      y2 = table[max_elements-1].y;
      m = (y2 - y1)/(x2 - x1);
      k = y2 - m * x2;
      return m * x + k;
    } else {		
      while (i<max_elements-1) {
        x1 = table[i].x;
        x2 = table[i+1].x;
        y1 = table[i].y;
        y2 = table[i+1].y;
        if ((x <= table[i].x) && (x > table[i+1].x )){
          found = 1;
          break;
        } else {
          i++;
        }
      }
      // check if we found our window
      if (found) {
        m = (y2 - y1)/(x2 - x1);
        k = y2 - m * x2;      
      } else {
        return def;
      }	
    }  
  } else if (xdirection == X_DIRECTION_INC) { // increasing x direction
    if (x < table[0].x) {
      x1 = table[0].x;
      x2 = table[1].x;
      y1 = table[0].y;
      y2 = table[1].y;
      m = (y2 - y1)/(x2 - x1);
      k = y2 - m * x2;
      return m * x + k;		
    } else if (x >= table[max_elements-1].x) { 
      x1 = table[max_elements-2].x;
      x2 = table[max_elements-1].x;
      y1 = table[max_elements-2].y;
      y2 = table[max_elements-1].y;
      m = (y2 - y1)/(x2 - x1);
      k = y2 - m * x2;
      return m * x + k;
    } else {
      while (i<max_elements-1) {
        x1 = table[i].x;
        x2 = table[i+1].x;
        y1 = table[i].y;
        y2 = table[i+1].y;
        if ((x >= table[i].x) && (x < table[i+1].x )){
          found = 1;
          break;
        } else {
          i++;
        }
      }
      // check if we found our window
      if (found) {
        m = (y2 - y1)/(x2 - x1);
        k = y2 - m * x2;      
      } else {
        return def;
      }	    
    }
  }  
  // calculate the interpolated value
  return m * x + k;
}

/**
	@brief
		Bin search function
*/
float dbin_search(float x, lincurve_dual_element *table, uint8_t max_elements, uint8_t dir, float def)
{
	uint8_t i=0;
	uint8_t found = 0;
	float x1,x2;
	float y1,y2;
	float m=0.0F,k=0.0F;
    
	// special case where setpoint is greater than the last calibration pont
	if (x < table[0].x) {
		return table[0].y[dir];
		
	} else if (x >= table[max_elements-1].x) { 
		return table[max_elements-1].y[dir];
		
	} else {
		
		while (i<max_elements-1) {
			if ((x >= table[i].x) && (x < table[i+1].x )){
				found = 1;
                x1 = table[i].x;
                x2 = table[i+1].x;
                y1 = table[i].y[dir];
                y2 = table[i+1].y[dir];
				break;
			} else {
				i++;
			}
		}
		
		if (found) {
			m = (y2 - y1)/(x2 - x1);
			k = y2 - m * x2;		
		} else {
			return def;
		}
		
		return m * x + k;
	}
}
