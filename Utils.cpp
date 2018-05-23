#include "Utils.h"

uint getRandNumberG(uint low, uint high )
{
   float prob = (float) qrand() / (RAND_MAX+1);
   double a = low + prob*(high-low);
   return uint(a);
}
