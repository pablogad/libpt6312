#include "base_ani.hpp"

uint8_t BaseAni::next() {
   step++;
   if( step == max_steps ) step=0;
   return step;
}

void BaseAni::reset() {
   step = 0;
}

uint8_t BaseAni::get() {
   return step;
}

// Default update: do nothing
void BaseAni::updateDisplay( DisplayDef& display ) {
}

