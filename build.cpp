#include "fcache.h"

double our_function( const double & x ) {
   return sin(x) * cos(x);
}

#define EPSILON 1e-3
// EPSILON 1e-4 causes the test to fail in this very coarse grid

int main() {
   FCACHE fcache;
   
   if ( fcache.build( our_function, "sin_times_cos", 0, 3.14159, 10 ) ) {
      if ( !fcache.test( 100, EPSILON ) ) {
         std::cerr << fcache.error_msg << std::endl;
      } else {
         fcache.write_c_table( "sin_times_cos_spline.h" );
      }
   } else {
      std::cerr << fcache.error_msg << std::endl;
   }

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

}
