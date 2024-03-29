#include "rebound_ani.h"
#include "display_ani.h"

// TMP!
#include <iostream>

//
//  TEXT REBOUND ANIMATION
//
uint8_t TextReboundAni::next() {
   // window_size : visible portion
   if( up_direction ) {
      if( step < text_size - window_size )
         step++;
      else {
         up_direction = false;
      }
   }
   else {
      if( step > 0 )
         step--;
      else {
         up_direction = true;
      }
   }
}

uint8_t TextReboundAni::getDisplayGroup() {
   return display_group;
}

void TextReboundAni::updateDisplay( DisplayDef& display ) {
   // Write currently visible portion of the text
   if( display_group == DisplayAni::ALL_GROUPS ) {
      display.setDigits(text, step);
   }
   else {
      display.setDigits( display_group, text, step );
   }
}


//
// ROUND SECTOR REBOUND ANIMATION
//
uint8_t RoundSectorReboundAni::next() {
   if( up_direction ) {
      if( step < max_steps ) step++;
      else up_direction = false;
   }
   else {
      if( step > 0 ) step--;
      else up_direction = true;
   }
}

void RoundSectorReboundAni::updateDisplay( DisplayDef& display ) {
   // Write currently visible sectors
   display.setRoundSectorLevel( step );
}
