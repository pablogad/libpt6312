#pragma once

#include <cstdint>

#include "display_def.h"

// Animation control class
class BaseAni {
public:
   // Animation type definitions
   // For text animation:
   //    ANI_REVEAL: show one character more on each frame
   //    ANI_SCROLL: draw the text and displace it to the left on every frame
   //    ANI_REBOUND: draw the text and displace to the left and then to the right
   //                 when all the text was presented, then again to the left when
   //                 the first character is reached
   // For round sector animation:
   //    ANI_REVEAL: show a new segment of the round sector in every frame
   //    ANI_SCROLL: show a single segment and move to the next on every frame
   //    ANI_REBOUND: show a new segment on every frame and start hiding one segment
   //                 on every frame when all are shown; when all are turned off,
   //                 start again.
   typedef enum { ANI_REVEAL,  // Reveal characters one by one
                  ANI_SCROLL,  // Scroll to left 
                  ANI_REBOUND  // Scroll side to side
                } AnimationType;

protected:
   uint8_t step;
   uint8_t max_steps;

   BaseAni::AnimationType type;

public:
   BaseAni( const uint8_t num_steps, const BaseAni::AnimationType t ) : max_steps(num_steps),
                                                         step(0),
                                                         type(t) {}
   // Needed for vector<ptr_to_BaseAni>
   virtual ~BaseAni() {}

   // Next animation step
   virtual uint8_t next();

   // Restart animation, it always starts at step 0
   void reset();

   // Get current step
   uint8_t get();

   // Get animation type
   const BaseAni::AnimationType getAniType() const;

   // Update data on the display. Does not refresh the display!
   virtual void updateDisplay( DisplayDef& display );
};
