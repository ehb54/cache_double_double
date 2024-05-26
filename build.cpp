#include "fcache.h"

double our_function( const double & x ) {
   return sin(x) * cos(x);
}

#define EPSILON 1e-3
// EPSILON 1e-4 causes the test to fail in this very coarse grid

int main() {
   FCACHE fcache;
   
   // just an example with an explicit grid
   if ( fcache.build( our_function
                      , "sin_times_cos_explicit_grid"
                      ,{
                         0
                         ,0.349066
                         ,0.698131
                         ,1.0472
                         ,1.39626
                         ,1.74533
                         ,2.09439
                         ,2.44346
                         ,2.79252
                         ,3.14159
                       }
                      ) ) {
      if ( !fcache.test( 100, EPSILON ) ) {
         std::cerr << fcache.error_msg << std::endl;
      } else {
         fcache.write_c_table( "sin_times_cos_explicit_grid.h" );
      }
   } else {
      std::cerr << fcache.error_msg << std::endl;
   }

   // 1000 point grid for timing

   if ( fcache.build( our_function, "sin_times_cos_1k", 0, 3.14159, 1000 ) ) {
      if ( !fcache.test( 100, EPSILON ) ) {
         std::cerr << fcache.error_msg << std::endl;
      } else {
         fcache.write_c_table( "sin_times_cos_spline_1k.h" );
         fcache.time( 10000000 );
      }
   } else {
      std::cerr << fcache.error_msg << std::endl;
   }

   // 50000 point grid for timing

   if ( fcache.build( our_function, "sin_times_cos_50k", 0, 3.14159, 50000 ) ) {
      if ( !fcache.test( 100, EPSILON * 1e-3 ) ) {
         std::cerr << fcache.error_msg << std::endl;
      } else {
         fcache.write_c_table( "sin_times_cos_spline_50k.h" );
         fcache.time( 10000000 );
      }
   } else {
      std::cerr << fcache.error_msg << std::endl;
   }


}
