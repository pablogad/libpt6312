#include <cstring>
#include <cstdint>
#include <iostream>
#include <unistd.h>

#include "vfd_interface.h"
#include "display_def.h"
#include "display_ani.h"

using std::cout;
using std::endl;

static void setCharacter(DisplayDef& display, VfdInterface& vfd, const char c ) {
   display.clearDigits();
   for( uint8_t g=0; g<display.getNumberOfGroups(); g++ ) {
      std::string gs( display.getNumberOfDigitsOnGroup( g ), c );
      display.setDigits( g, gs );
   }
   vfd.updateDisplay( display.getData(),0,12);
}


// Test
int main( int argc, char* argv[] ) {

   std::string display_def( "display_defs/vfd1.def" );

   // A definition file can be specified
   if( argc >= 2 ) {
      display_def.assign( argv[1] );
   }

   cout << "Display definition file: " << display_def << endl;

   DisplayDef display( display_def.c_str() );
   VfdInterface vfd(&display);

   bool proceed = display.isLoaded();
   if( proceed ) {

      vfd.Init();

      // All lit?
      if (argc == 3 && strcmp(argv[2], "-O") == 0) {
         memset (display.getData(), 0xFF, 12);
	 vfd.updateDisplay(display.getData(), 0, 12);
	 proceed = false;
      }
      // All off?
      if (argc == 3 && strcmp(argv[2], "-F") == 0) {
         memset (display.getData(), 0, 12);
	 vfd.updateDisplay(display.getData(), 0, 12);
	 proceed = false;
      }
   }
   if( proceed ) {
      for( int i=1; i<=13; i++ ) {

         cout << "Setting symbol and round sector " << i << endl;

         display.setRoundSectorLevel(i);
         display.setSymbol(i);

         vfd.updateDisplay( display.getData(),0,12 );
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

         vfd.updateDisplay( display.getData(),0,12 );

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
      vfd.updateDisplay( display.getData(),0,12 );

      sleep(2);

      cout << "Removing dots and round sector" << endl;

//
      display.removeDots(1,0);
      display.removeDots(1,2);
      display.clearRoundSector();
      vfd.updateDisplay( display.getData(),0,12 );
 
      sleep(2);

      cout << "Setting sectors from a list and LED test" << endl;

      int sectors[] = { 0, 1, 3, 5, 7, 9, 11, 13, -1 };
      display.setRoundSectorSectors( sectors );
      vfd.updateDisplay( display.getData(),0,12 );

      for( int i=0; i<=4; i++ ) {
         vfd.writeLeds((1<<i)&0xF);
         sleep(1);
      }

      cout << "Characters test" << endl;

      for( char c='A'; c<='z'; c++ ) {
         if( c == 'Z'+1 ) c = 'a';
         cout << "Showing all digits set to '" << c << "'" << endl;
	 setCharacter(display, vfd, c);
         sleep(1);
      }
      display.resetDigit(0,1);
      const uint8_t collection[] = { 5 };
      vfd.updateDisplayPartial( display.getData(),collection,1);

      cout << "Showing test crossing group boundaries" << endl;

      display.clearDigits();
      display.setDigits( "APABLOIIII", 1 );
      vfd.updateDisplay();

      // Test animation
      DisplayAni testAnimations( display );
      testAnimations.create_text_animation( "0123456789 HELLO ALL", BaseAni::ANI_REBOUND, 0 );

      int cnt=0;
      while(cnt++ < 100) {
         testAnimations.tick();
         sleep(1);
         // Draw!
         vfd.updateDisplay();
      }

   }
   else if( argc != 3 ) {
      cout << "Cannot load display definition file!" << endl;
   }

   return 0;
}
