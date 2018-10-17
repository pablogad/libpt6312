#include "rebound_ani.hpp"

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
