#include "fcache_use.h"

FCACHE_USE::FCACHE_USE(
                       std::vector < double >   & x_v
                       ,std::vector < double >   & y_v
                       ,std::vector < double >   & y2_v
                       ) {
   this->x_v  = & x_v;
   this->y_v  = & y_v;
   this->y2_v = & y2_v;
   size       = x_v.size();
}

double FCACHE_USE::value( const double & x ) { 

   unsigned int klo = 0;
   unsigned int khi = size - 1;

   while ( khi - klo > 1) {
      unsigned int k = ( khi + klo ) >> 1;
      if ( (*x_v)[ k ] > x ) {
         khi = k;
      } else {
         klo = k;
      }
   }

   if ( khi == klo ) {
      error_msg = "apply_natural_spline error finding point";
      return -DBL_MAX;
   }

   double h = (*x_v)[ khi ] - (*x_v)[ klo ];

   if ( h <= 0e0 ) {
      error_msg = "apply_natural_spline zero or negative interval";
      return -DBL_MAX;
   }

   double a = ( (*x_v)[ khi ] - x ) / h;
   double b = ( x - (*x_v)[ klo ] ) / h;

   return
      a * (*y_v)[ klo ] +
      b * (*y_v)[ khi ] + ( ( a * a * a - a ) * (*y2_v)[ klo ] + 
                            ( b * b * b - b ) * (*y2_v)[ khi ] ) * ( h * h ) / 6e0;
}

