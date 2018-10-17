#pragma once

#include "display_def.hpp"
#include "base_ani.hpp"

// This object contains a set of animations on the VFD display.
// It's only possible to set up a text animation per group and one for a round sector.
class DisplayAni {

public:
   DisplayAni( const DisplayDef& d ) : display(d) {}

   // Create a text animation on a group or on all characters on the display
   // Returns an unique id of the animation
   uint8_t create_text_animation( const std::string& str,
       const BaseAni::AnimationType t = BaseAni::ANI_REVEAL, const uint8_t group=ALL_GROUPS );

   // Create a round animation
   // Returns an unique id of the animation
   uint8_t create_round_sector_animation();

   // Animation tick: fps
   void tick();

   static const uint8_t ALL_GROUPS = 0xFF;

private:
   const DisplayDef& display;
   std::vector<BaseAni> animations;

};
