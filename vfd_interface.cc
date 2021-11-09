#include <cassert>
#include <cstdint>
#include <iomanip>
#include <iostream>

#include "vfd_interface.h"
#include "pt6312_commands.h"
#include "gpio.h"

GPIO gpio;

#define IR_READ (1<<22)  // PIN 15 - infrared read input
#define CLK     (1<<23)  // PIN 16 - CLK input for PT6312
#define STB     (1<<24)  // PIN 18 - STROBE input for PT6312
#define DIO     (1<<25)  // PIN 22 - DATA IN/OUT input/output for PT6312

#define TWAIT_DLY 2              // A delay period (strobe time) (us)
#define HWAIT_DLY TWAIT_DLY/2    // Half a delay period (clk transition) (us)

// Display control base command
static uint8_t display_ctrl = DISPLAY_ON | MAX_BRIGHTNESS;

static const uint8_t PT6312_MEM_SIZE = 22;

// Handle STB line
static inline void STB_UP() {
   gpio.SetBits(STB);
}
static inline void STB_DN() {
   gpio.ClearBits(STB);
}

// 1 us min delay - period with STB high, CLK is pulled up
static inline void twait() {
   gpio.busy_nano_sleep( TWAIT_DLY * 1000 );
}

// .5 us min delay - period between CLK transitions
static inline void hwait() {
   gpio.busy_nano_sleep( HWAIT_DLY * 1000 );
}

// Handle CLK line
static inline void CLK_UP() {
   hwait();
   gpio.SetBits(CLK);
}
static inline void CLK_DN() {
   hwait();
   gpio.ClearBits(CLK);
}

// Set STROBE HIGH for more than 1us
static inline void strobeDelay() {
   CLK_UP();
   twait();
   STB_UP();
   twait();
}

// Send bit 0 of b. STB must be LOW.
static inline void sendBit( uint8_t b ) {
   CLK_DN();
   hwait();
   if( b&1 )
      gpio.SetBits(DIO);
   else
      gpio.ClearBits(DIO);
   CLK_UP();  // Load data on rising edge
   hwait();
}

// Send byte without signaling STB_DN
static inline void sendByteNoStb( uint8_t data ) {
   sendBit( data ); data /= 2;
   sendBit( data ); data /= 2;
   sendBit( data ); data /= 2;
   sendBit( data ); data /= 2;
   sendBit( data ); data /= 2;
   sendBit( data ); data /= 2;
   sendBit( data ); data /= 2;
   sendBit( data );
}

// Send a byte (command or data).
// STB remains LOW at the end! CLK always HIGH at the end.
static inline void sendByte( uint8_t data ) {
   STB_DN();
   twait();
   sendByteNoStb( data );
}
// Read data pin on bit 0 of ret value. The pin must be set in INPT mode!
static inline uint8_t readBit() {
   uint8_t rv = 0;
   CLK_DN();
   hwait();
   CLK_UP();
   if( gpio.GetBits( DIO ) ) rv |= 1;
   hwait();
   return rv;
}

// Read a single byte bit by bit
static inline uint8_t readByte() {

   uint8_t rv = 0;

   rv |= readBit(); rv *= 2;
   rv |= readBit(); rv *= 2;
   rv |= readBit(); rv *= 2;
   rv |= readBit(); rv *= 2;
   rv |= readBit(); rv *= 2;
   rv |= readBit(); rv *= 2;
   rv |= readBit(); rv *= 2;
   rv |= readBit();

   return rv;
}

VfdInterface::VfdInterface() :
          display(nullptr), defLoaded(false) {}
VfdInterface::VfdInterface( DisplayDef* displayDef ) :
	  display(displayDef), defLoaded(true) {}

uint32_t VfdInterface::readKeys() {

   sendByte( DATA_SET_CMD | RD_KEY_DATA );
   twait();

   // Set DIO pin in input mode
   gpio.InitInputs( DIO );

   raw_keys = readByte();
   raw_keys = (raw_keys << 8) + readByte();
   raw_keys = (raw_keys << 8) + readByte();

   // Set DIO pin back in output mode
   gpio.InitOutputs( DIO );

   strobeDelay();

   return raw_keys;
}

// Write leds
void VfdInterface::writeLeds( uint8_t leds ) {

   leds_state = leds & 0x0F;

   sendByte( DATA_SET_CMD | WR_LEDS | INC_ADDRESS );
   twait();

   sendByte( leds_state );
   strobeDelay();
}

// Get last written leds value
uint8_t VfdInterface::getLeds() { return leds_state; }

// Send a block of data to the display
void VfdInterface::updateDisplayPartial( const uint8_t* data, const uint8_t* indices, uint8_t len ) {
   // First send CMD2: DATA_SET[WR_TO_DISPLAY]
   sendByte( DATA_SET_CMD | WR_TO_DISPLAY | INC_ADDRESS );
   strobeDelay();

   int cnt=0;
   while( cnt != len ) {
      // Send CMD3 (ADDRESS_SET) and a single data
      uint8_t idx = indices[cnt];
      if( idx < 22 ) {  // Ignore data if invalid
         sendByte( ADDR_SET_CMD | idx );
         do {
            twait();

            // Send data. If the next index is consecutive, no need to
            // send a new ADDR_SET as the address is already correct.
            sendByteNoStb( data[idx] );
            if( cnt+1 != len && indices[cnt+1] == idx+1 ) {
               cnt++;
                  idx = indices[cnt];
               continue;
            }
            else {
               strobeDelay();
               break;
            }
         } while( 1 );
      }

      cnt++;
   }
}
void VfdInterface::updateDisplay() {
   if( defLoaded && display->isLoaded() )
      updateDisplay( display->getData(), 0, 22 );
}
// Send a block of data to the display
void VfdInterface::updateDisplay( const uint8_t* data, const uint8_t offset, uint8_t len ) {

   if( offset+len > PT6312_MEM_SIZE ) return;

   sendByte( DATA_SET_CMD | WR_TO_DISPLAY | INC_ADDRESS );
   strobeDelay();

   sendByte( ADDR_SET_CMD | (offset & 0x1F ) );
   twait();

   uint8_t idx=0;
   while( idx != len ) {
      sendByteNoStb( data[idx+offset] );
      idx++;
      // After the last byte we set STB up again
      if( idx == len ) strobeDelay();
      else             twait();
   }

   // Read keys
   readKeys();

   // Update display status (on/off and brightness)
   sendByte( MODE_SET_CMD | DISPLAY_6GRID_16SEGMENT_MODE );
   strobeDelay();

   sendByte( DISPLAY_CTRL_CMD | display_ctrl );
   strobeDelay();
}

bool VfdInterface::Init() {

   // Does nothing if already initialized
   initialized_ = gpio.Init();

   // Fail if definition file not loaded
   if (defLoaded && initialized_)
      initialized_ = display->isLoaded();

   if (initialized_) {
      gpio.InitOutputs( CLK | STB | DIO );
      // not used right now - gpio.InitInputs( IR );
   }

   return initialized_;
}

