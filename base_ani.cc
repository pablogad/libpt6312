#include "base_ani.h"

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

const BaseAni::AnimationType BaseAni::getAniType() const {
   return type;
}

// Default update: do nothing
void BaseAni::updateDisplay( DisplayDef& display ) {
}

