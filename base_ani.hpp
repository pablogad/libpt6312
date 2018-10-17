#pragma once

#include <cstdint>

// Animation control class
class BaseAni {
public:
   // Definitions
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
   // Next animation step
   virtual uint8_t next();

   // Restart animation, it always starts at step 0
   void reset();

   // Get current step
   uint8_t get();
};
