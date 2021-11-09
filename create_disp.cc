#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>

#include "vfd_interface.h"

static const int EXIT_VALUE = 666;

int main( int argc, char* argv[] ) {

   uint8_t data[22];
   int grid;
   char val[8] = {0};

   VfdInterface vfd;
   bool init = vfd.Init();
   if (!init) {
      printf( "FAIL!\n" );
      return 1;
   }

   // Mode: show all grids activated
   if( argc == 2 ) {
      if( !strcmp( argv[1], "--all-grids") ) {
         memset(data,255,22);
         vfd.updateDisplay( data, 0, 22 );
      }
      else if( !strcmp( argv[1], "--clear") ) {
         memset(data,0,22);
         vfd.updateDisplay( data, 0, 22 );
      }
      else if( !strcmp( argv[1], "--help") || !strcmp( argv[1], "-h" ) ) {
         printf( "Usage:\n   --all-grids: show all grids activated\n"
                 "   --clear: deactivate all grids\n"
                 "   --help: show this text\n\n"
                 "   No parameters: test individual grids interactively\n\n" );
      }
   }
   else {
   printf( "=== DISPLAY TEST ===\nExit entering grid=%d\n", EXIT_VALUE );

   while(1) {

      memset(data,0,22);

      printf( "Grid? [0-21, %d end] ", EXIT_VALUE );
      scanf( "%d", &grid );

      if(grid == EXIT_VALUE)
         break;
      else if( grid < 0 || grid > 21 )
         continue;

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

      vfd.updateDisplay( data, 0, 22 );

      printf( "Current keys [%08X]\n", vfd.readKeys() );
   }}

   return 0;
}

