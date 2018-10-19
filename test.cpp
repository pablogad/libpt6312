#include <cstring>
#include <cstdint>
#include <iostream>
#include <unistd.h>

extern "C" {
#include "pt6312.h"
}

#include "display_def.hpp"
#include "display_ani.hpp"

using std::cout;
using std::endl;

static void setCharacter( DisplayDef& display, const char c ) {
   display.clearDigits();
   for( uint8_t g=0; g<display.getNumberOfGroups(); g++ ) {
      std::string gs( display.getNumberOfDigitsOnGroup( g ), c );
      display.setDigits( g, gs );
   }
   updateDisplay( display.getData(),0,12);
}


// Test
int main( int argc, char* argv[] ) {

   init_pt6312();
   std::string display_def( "display_defs/vfd1.def" );

   // A definition file can be specified
   if( argc == 2 ) {
      display_def.assign( argv[1] );
   }

   cout << "Display definition file: " << display_def << endl;

   DisplayDef display( display_def.c_str() );
   if( display.isLoaded() == true ) {

      for( int i=1; i<=13; i++ ) {

         cout << "Setting symbol and round sector " << i << endl;

         display.setRoundSectorLevel(i);
         display.setSymbol(i);

         updateDisplay( display.getData(),0,12 );
         sleep(1);
      }

      for( int i=1; i<=13; i++ ) {

         char dig = (i>9 ? 0:i) + '0';
         cout << "Erasing symbol " << i << " and setting all digits to " << dig << endl;

         display.resetSymbol(i);
         for( uint8_t g=0; g<display.getNumberOfGroups(); g++ ) {
            std::string gs( display.getNumberOfDigitsOnGroup( g ), dig );
            display.resetDigit( g, 0, display.getNumberOfDigitsOnGroup( g ) );
            display.setDigits( g, gs );
         }

         updateDisplay( display.getData(),0,12 );

         sleep(1);
      }

      cout << "Removing digits and setting to new values plus dots" << endl;

//
      display.resetDigit(0, 0, 2);
      display.resetDigit(1, 0, 5);
      display.setDigits(0, "33");
      display.setDigits(1, "01234");
//
      display.setDots(1,0);
      display.setDots(1,2);
      display.setRoundSectorLevel(11);
      updateDisplay( display.getData(),0,12 );

      sleep(2);

      cout << "Removing dots and round sector" << endl;

//
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
      display.resetDigit(0,1);
      const uint8_t collection[] = { 5 };
      updateDisplayPartial( display.getData(),collection,1);

      cout << "Showing test crossing group boundaries" << endl;

      display.clearDigits();
      display.setDigits( "APABLOIIII", 1 );
      updateDisplay( display.getData(),0,12 );

      // Test animation
      DisplayAni testAnimations( display );
      testAnimations.create_text_animation( "0123456789 HELLO ALL", BaseAni::ANI_REBOUND, 0 );

      int cnt=0;
      while(cnt++ < 100) {
         testAnimations.tick();
         sleep(1);
         updateDisplay( display.getData(),0,12 );
      }
   }
   else {
      cout << "Cannot load display definition file!" << endl;
   }

   close_pt6312();

   return 0;
}
