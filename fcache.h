#ifndef FCACHE_H
#define FCACHE_H

#include <iostream>
#include <fstream>
#include <iomanip>
#include <math.h>
#include <vector>
#include <algorithm>
#include <time.h>

class FCACHE
{
 public:

   FCACHE();
   // ~FCACHE();
   std::string error_msg;

   bool build(
              double                          (*f)(const double &)
              ,const std::string            & name
              ,const std::vector < double > & grid
              );
   
   bool build(
              double               (*f)(const double &)
              ,const std::string & name
              ,double              x_min
              ,double              x_max
              ,unsigned int        points
              );
   
   bool compare(
                const double & x
                ,double & diff
                 ); 

   bool write_c_table( std::string fname );

   bool apply_natural_spline(
                             const std::vector < double >  & x_v
                             ,const std::vector < double > & y_v
                             ,const std::vector < double > & y2_v
                             ,const double                 & x
                             ,double                       & y
                             );

   bool test(
             const std::vector < double >  & x_v
             ,double             epsilon
             );

   bool test(
             unsigned int        points
             ,double             epsilon
             );

   std::vector < double > uniform_grid(
                                       double              x_min
                                       ,double              x_max
                                       ,unsigned int        points
                                       );

   bool time( unsigned int points );
   bool time( const std::vector < double >  & x_v );

 private:

   double                   (*f)(const double &);
   
   std::vector < double >   x_v;
   std::vector < double >   y_v;
   std::vector < double >   y2_v;

   std::string name;

   void natural_spline(
                       const std::vector < double >  & x_v
                       ,const std::vector < double > & y_v
                       ,std::vector < double >       & y2_v
                       );

   bool apply_natural_spline(
                             const double            & x
                             ,double                  & y
                             );

};
   
#endif // FCACHE

