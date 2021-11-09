#include <cstring>
#include <string>

#include "text_data.h"
#include "base_ani.h"

// Text rebound animation control: the text moves to the left and then to the right
// when reaching the end of string
class TextReboundAni : public BaseAni, public TextData {

   uint8_t window_size;
   uint8_t display_group; // Can be DisplayAni::ALL_GROUPS or a group number
   bool up_direction;

public:

   // text_size:   size of the string that will be written
   // window_size: size of the display group where the text will be written
   TextReboundAni( const std::string& text, const uint8_t window_size, const uint8_t group ) :
                                        window_size(window_size),
                                        display_group(group),
                                        up_direction(true),
                                        TextData(text),
                                        BaseAni( text_size-window_size, BaseAni::ANI_REBOUND ) {}

   uint8_t next();
   uint8_t getDisplayGroup();

   void updateDisplay( DisplayDef& display );
};


// Round sector rebound animation:
class RoundSectorReboundAni : public BaseAni {

   bool up_direction;

public:
   RoundSectorReboundAni( const uint8_t segments ) :
                                        up_direction(true),
                                        BaseAni( segments, BaseAni::ANI_REBOUND ) {}

   uint8_t next();
   void updateDisplay( DisplayDef& display );
};
