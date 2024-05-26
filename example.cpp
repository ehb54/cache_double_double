#include "fcache_use.h"
#include <iostream>

#include "sin_times_cos_spline_50k.h"

int main() {
   FCACHE_USE sin_times_cos( sin_times_cos_50k_x_v, sin_times_cos_50k_y_v, sin_times_cos_50k_y2_v );

   for ( double x = 0; x < 3.1415; x += .2 ) {
      std::cout
         << "sin_times_cos( "
         << x
         << " ) = "
         << sin_times_cos.value( x )
         << std::endl
         ;
   }
}
