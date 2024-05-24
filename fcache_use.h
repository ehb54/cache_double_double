#ifndef FCACHE_USE_H
#define FCACHE_USE_H

#include <vector>
#include <string>
#include <values.h>

class FCACHE_USE
{
 public:

   FCACHE_USE(
              std::vector < double >    & x_v
              ,std::vector < double >   & y_v
              ,std::vector < double >   & y2_v
              );

   // ~FCACHE_USE();

   double value( const double & x );
   std::string error_msg;

 private:

   std::vector < double > * x_v;
   std::vector < double > * y_v;
   std::vector < double > * y2_v;

   size_t size;
};
   
#endif // FCACHE_USE

