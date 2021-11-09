#include "display_def.h"

class VfdInterface  {

public:

   VfdInterface();
   VfdInterface(DisplayDef* display);

   bool Init();

   uint32_t readKeys();
   void writeLeds( uint8_t leds );
   uint8_t getLeds();

   void updateDisplay();
   void updateDisplay( const uint8_t* data, const uint8_t offset, uint8_t len );
   void updateDisplayPartial( const uint8_t* data, const uint8_t* indices, uint8_t len );


private:
   bool defLoaded;
   bool initialized_;

   DisplayDef* display;

   uint32_t raw_keys;
   uint8_t leds_state;

};

