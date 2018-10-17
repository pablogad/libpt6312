#include "base_ani.hpp"

// Text rebound animation control: the text moves to the left and then to the right
// when reaching the end of string
class TextReboundAni : public BaseAni {

   uint8_t text_size;
   uint8_t window_size;
   bool up_direction;

public:

   // text_size:   size of the string that will be written
   // window_size: size of the display group where the text will be written
   TextReboundAni( const uint8_t text_size, const uint8_t window_size ) :
                                        text_size(text_size),
                                        window_size(window_size),
                                        up_direction(true),
                                        BaseAni( text_size-window_size, BaseAni::ANI_REBOUND ) {}

   uint8_t next();
};
