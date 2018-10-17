#include "display_ani.hpp"
#include "rebound_ani.hpp"

void DisplayAni::tick() {
   // Advance a frame on the current animations
   for( BaseAni& ani : animations ) {
      ani.next();
      uint8_t offset = ani.get();
      
   }
}

// Create a text animation effect of certain type
uint8_t DisplayAni::create_text_animation(const std::string& str,
               const BaseAni::AnimationType t, const uint8_t group) {

   uint8_t display_size;
   if( group == ALL_GROUPS ) {
      // Add size of all the groups
      display_size=0;
      for( int g=0; g<display.getNumberOfGroups(); g++ ) {
         display_size += display.getNumberOfDigitsOnGroup(g);
      }
   }
   else {
      // Get size of the specified group
      display_size = display.getNumberOfDigitsOnGroup( group );
   }
   if( t == BaseAni::ANI_REBOUND ) {
      TextReboundAni newAni( str, display_size, group );
      animations.push_back( newAni );
   }
   // else if ANI_SCROLL
   // else if ANI_REVEAL
}

uint8_t DisplayAni::create_round_sector_animation() {
}
