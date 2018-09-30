#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>

#include "pt6312.h"


int main( int argc, char* argv[] ) {

   uint8_t data[22];
   int grid;
   char val[8] = {0};
   init_pt6312();

   printf( "=== DISPLAY TEST ===\n\n" );

   while(1) {

      memset(data,0,22);

      printf( "Grid? [1-22, 0:end] " );
      scanf( "%d", &grid );

      if( grid < 0 || grid > 22 )
        continue;
      else if(!grid)
        break;

      grid--;

      printf( "Value (16bit) ? " );
      scanf( "%6s", &val );

      uint16_t num;
      if( !strncmp(val,"0x",2) )
         sscanf( &val[2], "%X", &num );
      else
         num = atoi(val);

      // Little endian: first LOW byte!
      data[grid*2+1] = (uint8_t)( (num & 0xFF00) >> 8 );
      data[grid*2] = (uint8_t)(num & 0xFF);

      updateDisplay( data, 0, 22 );
   }

   close_pt6312();

   return 0;
}

