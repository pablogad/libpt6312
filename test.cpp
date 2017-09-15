#include <cstring>
#include <cstdint>
#include <iostream>
#include <unistd.h>

extern "C" {
#include "pt6312.h"
}
#include "display_def.hpp"

using std::cout;
using std::endl;

static void setCharacter( DisplayDef& display, const char c ) {
   display.clearDigits();
   std::string g1( 2, c );
   std::string g2( 5, c );
   display.setDigits(0, g1);
   display.setDigits(1, g2);
   updateDisplay( display.getData(),0,12);
}

// Test
int main( int argc, char* argv[] ) {

   init_pt6312();

   DisplayDef display( "vfd1.def" );
   if( display.isLoaded() == true ) {

      for( int i=1; i<=13; i++ ) {

         cout << "Setting symbol and round sector " << i << endl;

         display.setRoundSectorLevel(i);
         display.setSymbol(i);
         updateDisplay( display.getData(),0,12 );
         sleep(1);
      }
      std::string g1;
      std::string g2;
      for( int i=1; i<=13; i++ ) {

         char dig = (i>9 ? 0:i) + '0';
         cout << "Erasing symbol " << i << " and setting all digits to " << dig << endl;

         display.resetSymbol(i);
         g1.assign( 2, dig );
         g2.assign( 5, dig );
         display.resetDigit(0, 0, 2);
         display.resetDigit(1, 0, 5);
         display.setDigits(0, g1);
         display.setDigits(1, g2);
         updateDisplay( display.getData(),0,12 );

         sleep(1);
      }

      cout << "Removing digits and setting to new values plus dots" << endl;

      display.resetDigit(0, 0, 2);
      display.resetDigit(1, 0, 5);
      display.setDigits(0, "33");
      display.setDigits(1, "01234");
      display.setDots(1,0);
      display.setDots(1,2);
      display.setRoundSectorLevel(11);
      updateDisplay( display.getData(),0,12 );

      sleep(2);

      cout << "Removing dots and round sector" << endl;

      display.removeDots(1,0);
      display.removeDots(1,2);
      display.clearRoundSector();
      updateDisplay( display.getData(),0,12 );
 
      sleep(2);

      cout << "Setting sectors from a list and LED test" << endl;

      int sectors[] = { 0, 1, 3, 5, 7, 9, 11, 13, -1 };
      display.setRoundSectorSectors( sectors );
      updateDisplay( display.getData(),0,12 );

      for( int i=0; i<=4; i++ ) {
         setLEDs((1<<i)&0xF);
	 updateLeds();
         sleep(1);
      }

      cout << "Characters test" << endl;

      for( char c='A'; c<='z'; c++ ) {
         if( c == 'Z'+1 ) c = 'a';
         cout << "Showing all digits set to '" << c << "'" << endl;
	 setCharacter( display, c);
         sleep(1);
      }
   }
   else {
      cout << "Cannot load display definition file!" << endl;
   }

   close_pt6312();

   return 0;
}

