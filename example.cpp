#include "fcache_use.h"
#include <iostream>

std::vector < double > sin_times_cos_x_v = { 
   0
   ,0.34906555555555552
   ,0.69813111111111104
   ,1.0471966666666666
   ,1.3962622222222221
   ,1.7453277777777776
   ,2.0943933333333331
   ,2.4434588888888884
   ,2.7925244444444441
   ,3.1415899999999999
};

std::vector < double > sin_times_cos_y_v = { 
   0
   ,0.32139357898013426
   ,0.49240377410775443
   ,0.43301314415650738
   ,0.17101117991069092
   ,-0.17100868635167571
   ,-0.43301181735957778
   ,-0.4924042348949339
   ,-0.32139561174432957
   ,-2.6535897933433872e-06
};

std::vector < double > sin_times_cos_y2_v = { 
   0
   ,-1.3385910025208023
   ,-2.0508414114396776
   ,-1.8034818921290863
   ,-0.71225449659775397
   ,0.7122440693212837
   ,1.803476574604626
   ,2.0508425381851336
   ,1.3386024300263943
   ,0
};

int main() {
   FCACHE_USE sin_times_cos( sin_times_cos_x_v, sin_times_cos_y_v, sin_times_cos_y2_v );

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