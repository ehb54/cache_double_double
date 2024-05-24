## procedure

 * given a function : double our_function( const double & )
 * define its usage limits (minimum & maximum x )
 * build phase: compute its values on a grid, apply natural spline and create c tables for inclusion 
   * class FCACHE, see build.cpp for use
 * use phase: use a spline on the tables to get the value
   * class FCACHE_USE, see example.cpp for use

## install & test
 * clone, ./make.sh

## thoughts
 * EPSILON is a tradeoff, smaller tolerance will require more points
 * for some functions an adaptive grid might save data space
 * for my spherical harmonic usage, the resulting .h file was 24M
