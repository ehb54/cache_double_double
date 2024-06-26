#include "fcache.h"

FCACHE::FCACHE() {
   f = 0;
}

std::vector < double > FCACHE::uniform_grid(
                                            double              x_min
                                            ,double              x_max
                                            ,unsigned int        points
                                            ) {
#define ROUNDING_TOLERANCE 1e-6
   
   std::vector < double > grid;

   double delta_x = ( x_max - x_min ) / ( (double) points - 1 );

   for ( double x = x_min; x <= x_max + ROUNDING_TOLERANCE; x += delta_x ) {
      grid.push_back( x );
   }

   return grid;
}

bool FCACHE::build(
                   double               (*f)(const double &)
                   ,const std::string & name
                   ,double              x_min
                   ,double              x_max
                   ,unsigned int        points
                   ) {

   if ( x_min >= x_max ) {
      error_msg = "build() x_min must be less than x_max";
      return false;
   }

   return build( f, name, uniform_grid( x_min, x_max, points ) );
}

bool FCACHE::build(
                   double                          (*f)(const double &)
                   ,const std::string            & name
                   ,const std::vector < double > & grid
                   ) {

   this->f = f;
   this->name = name;
   
   if ( grid.size() < 2 ) {
      error_msg = "build() requires 2 points";
      return false;
   }

   x_v = grid;

   std::sort( x_v.begin(), x_v.end() );
   
   double x_min = x_v.front();
   double x_max = x_v.back();
   
   if ( x_min >= x_max ) {
      error_msg = "build() x_min must be less than x_max";
      return false;
   }

   y_v.clear();
   y2_v.clear();

   for ( auto const & x : x_v ) {
      y_v.push_back( (*f)( x ) );
   }

   natural_spline( x_v, y_v, y2_v );
   return true;
}

bool FCACHE::compare(
                     const double & x
                     ,double & diff
                     ) {

   if ( !f ) {
      error_msg = "no f defined";
      return false;
   }

   double result;

   if ( !apply_natural_spline( x, result ) ) {
      return false;
   }

   diff = fabs( f(x) - result );
   return true;
}

bool FCACHE::time(
                  unsigned int        points
                  ) {
   if ( !f ) {
      error_msg = "no f defined";
      return false;
   }

   double x_min   = x_v.front();
   double x_max   = x_v.back();
   
   return time( uniform_grid( x_v.front(), x_v.back(), points ) );
}

bool FCACHE::time(
                  const std::vector < double >  & grid
                  ) {
   if ( !f ) {
      error_msg = "no f defined";
      return false;
   }

   if ( grid.size() < 2 ) {
      error_msg = "test() requires 2 points";
      return false;
   }

   // compute via f
   {

      // require some result to prevent optimizing compiler from bypassing
      double result = 0;

      clock_t start_time = clock();

      for ( auto const & x : grid ) {
         result += f(x);
      }
         
      clock_t used_time = clock() - start_time;

      // force result in output to prevent optimizing compiler from bypassing
      printf( "compute via f: %.2fms (%d)\n", used_time * 1e3 / CLOCKS_PER_SEC, (int)result );
   }
   
   // compute via spline
   {

      // require some result to prevent optimizing compiler from bypassing
      double result = 0;
      double fx;

      clock_t start_time = clock();

      for ( auto const & x : grid ) {
         apply_natural_spline( x, fx );
         result += fx; 
      }
         
      clock_t used_time = clock() - start_time;

      // force result in output to prevent optimizing compiler from bypassing
      printf( "compute via apply_natural_spline (%lu grid points): %.2fms (%d)\n", x_v.size(), used_time * 1e3 / CLOCKS_PER_SEC, (int)result );
   }
   
   // compute via fast_natural_spline
   {

      // require some result to prevent optimizing compiler from bypassing
      double result = 0;

      clock_t start_time = clock();

      for ( auto const & x : grid ) {
         result +=  fast_natural_spline( x );
      }
         
      clock_t used_time = clock() - start_time;

      // force result in output to prevent optimizing compiler from bypassing
      printf( "compute via fast_natural_spline (%lu grid points): %.2fms (%d)\n", x_v.size(), used_time * 1e3 / CLOCKS_PER_SEC, (int)result );
   }
   
   return true;
}

bool FCACHE::test(
                  unsigned int        points
                  ,double             epsilon
                  ) {
   if ( !f ) {
      error_msg = "no f defined";
      return false;
   }

   if ( points < 2 ) {
      error_msg = "test() requires 2 points";
      return false;
   }
   
   double x_min   = x_v.front();
   double x_max   = x_v.back();
   
   std::vector < double > grid = uniform_grid( x_v.front(), x_v.back(), points );
   return test( grid, epsilon );
}

bool FCACHE::test(
                  const std::vector < double >  & grid
                  ,double             epsilon
                  ) {
   if ( !f ) {
      error_msg = "no f defined";
      return false;
   }

   if ( grid.size() < 2 ) {
      error_msg = "test() requires 2 points";
      return false;
   }

   double diff;
   for ( auto const & x : grid ) {
      if ( !compare( x, diff ) ) {
         return false;
      }
      if ( diff > epsilon ) {
         double result;
         apply_natural_spline( x, result );
         std::stringstream msg;
         msg
            << std::setprecision (17)
            << "test() failed : at x = "
            << x
            << " :  | "
            << f(x)
            << " - "
            << result
            << " | > "
            << epsilon
            ;
         error_msg = msg.str();
         return false;
      }
   }

   return true;
}

bool FCACHE::apply_natural_spline(
                                  const double    & x
                                  ,double         & y
                                  ) {
   return apply_natural_spline( x_v, y_v, y2_v, x, y );
}

bool FCACHE::apply_natural_spline(
                                  const std::vector < double >  & x_v
                                  ,const std::vector < double > & y_v
                                  ,const std::vector < double > & y2_v
                                  ,const double                 & x
                                  ,double                       & y
                                  ) {
   unsigned int klo = 0;
   unsigned int khi = x_v.size() - 1;

   while ( khi - klo > 1) {
      unsigned int k = ( khi + klo ) >> 1;
      if ( x_v[ k ] > x ) {
         khi = k;
      } else {
         klo = k;
      }
   }

   if ( khi == klo ) {
      error_msg = "apply_natural_spline error finding point";
      return false;
   }

   double h = x_v[ khi ] - x_v[ klo ];

   if ( h <= 0e0 ) {
      error_msg = "apply_natural_spline zero or negative interval";
      return false;
   }

   double a = ( x_v[ khi ] - x ) / h;
   double b = ( x - x_v[ klo ] ) / h;

   y = 
      a * y_v[ klo ] +
      b * y_v[ khi ] + ( ( a * a * a - a ) * y2_v[ klo ] + 
                         ( b * b * b - b ) * y2_v[ khi ] ) * ( h * h ) / 6e0;

   return true;
}

double FCACHE::fast_natural_spline( const double & x ) {
   // no bounds checking, could crash
   
   static double one_over_six = 1e0/6e0;
   
   unsigned int klo = 0;
   unsigned int khi = x_v.size() - 1;

   while ( khi - klo > 1 ) {
      unsigned int k = ( khi + klo ) >> 1;
      if ( x_v[ k ] > x ) {
         khi = k;
      } else {
         klo = k;
      }
   }

   double h = x_v[ khi ] - x_v[ klo ];
   double one_over_h = 1e0 / h;

   double a = ( x_v[ khi ] - x ) * one_over_h;
   double b = ( x - x_v[ klo ] ) * one_over_h;

   return
      a * y_v[ klo ] +
      b * y_v[ khi ] + ( ( a * a * a - a ) * y2_v[ klo ] + 
                         ( b * b * b - b ) * y2_v[ khi ] ) * ( h * h ) * one_over_six;

}

bool FCACHE::write_c_table( std::string fname ) {

   std::ofstream ofs( fname.c_str(), std::ios::out );

   std::cout << ">" << fname << std::endl;
   ofs << std::setprecision (17);

   // x_v
   {
      ofs
         << "    std::vector < double > "
         << name
         << "_x_v = { " 
         << std::endl;

      bool first = true;
      for ( const auto & x : x_v ) {
         ofs
            << "        "
            << ( first ? "" : "," )
            << x
            << std::endl
            ;
         first = false;
      }
      ofs
         << "    };"
         << std::endl
         << std::endl
         ;
   }

   // y_v
   {
      ofs
         << "    std::vector < double > "
         << name
         << "_y_v = { " 
         << std::endl
         ;

      bool first = true;
      for ( const auto & y : y_v ) {
         ofs
            << "        "
            << ( first ? "" : "," )
            << y
            << std::endl
            ;
         first = false;
      }
      ofs
         << "    };"
         << std::endl
         << std::endl
         ;
   }

   // y2_v
   {
      ofs
         << "    std::vector < double > "
         << name
         << "_y2_v = { " 
         << std::endl
         ;

      bool first = true;
      for ( const auto & y2 : y2_v ) {
         ofs
            << "        "
            << ( first ? "" : "," )
            << y2
            << std::endl
            ;
         first = false;
      }
      ofs
         << "    };"
         << std::endl
         << std::endl
         ;
   }

   ofs.close();
}

void FCACHE::natural_spline(
                            const std::vector < double >  & x
                            ,const std::vector < double > & y
                            ,std::vector < double >       & y2
                            ) {
   double  p;
   double  qn;
   double  sig;
   double  un;
   std::vector < double > u(x.size());
   
   size_t x_size = x.size();

   y2.resize(x_size);
   
   y2[ 0 ] = u [ 0 ] = 0e0;
   
   for ( size_t i = 1; i < x_size - 1; ++i )  {
      sig     = ( x[ i ] - x[ i - 1 ] ) / ( x[ i + 1 ] - x[ i - 1 ] );
      p       = sig * y2[ i - 1 ] + 2e0;
      y2[ i ] = ( sig -1e0 ) / p;
      u [ i ] = ( y[ i + 1 ] - y[ i ] ) / ( x[ i + 1 ] - x[ i ]) - ( y[ i ] - y[ i - 1 ] ) / ( x[ i ] - x[ i - 1 ]);
      u [ i ] = ( 6e0 * u[ i ] / ( x[ i + 1 ] - x[ i - 1 ] ) - sig * u[ i - 1 ] ) / p;
   }
   
   qn = un = 0e0;
   
   y2[ x_size - 1 ] = 0e0;
   
   for ( size_t k = x_size - 2; k >= 1 ; --k ) {
      y2[ k ] = y2[ k ] * y2[ k + 1 ] + u[ k ];
   }
}
