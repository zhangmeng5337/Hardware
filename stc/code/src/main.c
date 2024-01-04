#include "tm7780.h"
#include "app.h"

void main(void)
{
 hardware_init();
  while(1)
  {
     app_proc();
  }
}
