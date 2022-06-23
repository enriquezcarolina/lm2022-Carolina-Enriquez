
#include <stdlib.h>
#include <stdio.h>
#include <iostream>


uint16_t sgn[]= 
{
  #include "sgn.h"
};




int main()
{
for(int i=0; i<64;i++)
   {
    printf("%d,",sgn[i]);
   }

   
   return 0;

}

   




