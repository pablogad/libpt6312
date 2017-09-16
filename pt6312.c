#include <string.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <sys/types.h>

#include <bcm2835.h>

#include "pt6312_commands.h"

static const uint8_t PT6312_MEM_SIZE = 22;

#define _DBG 0
#if _DBG
#include <stdio.h>
#endif


// GPIO pins
#define IR_PIN  RPI_GPIO_P1_15
#define CLK_PIN RPI_GPIO_P1_23
#define STB_PIN RPI_GPIO_P1_24
#define DIO_PIN RPI_GPIO_P1_19

#define TWAIT_DLY 2              // A delay period (strobe time) (us)
#define HWAIT_DLY TWAIT_DLY/2    // Half a delay period (clk transition) (us)


// Private functions

// Nanoseconds delay
static inline const void delayClock( const long micros ) {
   struct timespec request = { 0, micros*1000 };
   struct timespec remain;
   while( clock_nanosleep( CLOCK_MONOTONIC, 0, &request, &remain ) != 0 ) {
       if( errno != EINTR ) break;
       request.tv_nsec = remain.tv_nsec;
   }
}

// Handle STB line
static inline const void STB_UP() {
   #if _DBG 
   printf( "STB HIGH\n" );
   #endif
   bcm2835_gpio_set( STB_PIN );
}
static inline const void STB_DN() {
   #if _DBG 
   printf( "STB LOW\n" );
   #endif
   bcm2835_gpio_clr( STB_PIN );
}

// Handle CLK line
static inline const void CLK_UP() {
   #if _DBG 
   printf( "CLK HIGH\n" );
   #endif
   delayClock( HWAIT_DLY );   // Needed for it to work - shouldn't! - investigate further
   bcm2835_gpio_set( CLK_PIN );
}
static inline const void CLK_DN() {
   #if _DBG 
   printf( "CLK LOW\n" );
   #endif
   delayClock( HWAIT_DLY );   // Needed for it to work - shouldn't!
   bcm2835_gpio_clr( CLK_PIN );
}

// 1 us min delay - period with STB high, CLK is pulled up
static inline const void twait() {
   #if _DBG 
   printf( "TWAIT\n" );
   #endif
   //bcm2835_delayMicroseconds( TWAIT_DLY );
   delayClock( TWAIT_DLY );
}
// .5 us min delay - period between CLK transitions
static inline const void hwait() {
   #if _DBG 
   printf( "HWAIT\n" );
   #endif
   //bcm2835_delayMicroseconds( HWAIT_DLY );
   delayClock( HWAIT_DLY );
}

// Set STROBE HIGH for more than 1us
static inline void strobeDelay() {
   CLK_UP();
   twait();
   STB_UP();
   twait();
}

// Send bit 0 of b. STB must be LOW.
static inline const void sendBit( uint8_t b ) {
   CLK_DN();
   hwait();
   #if _DBG 
   printf( "  SEND BIT(%c)\n", b&1 ? '1':'0');
   #endif
   if( b&1 )  bcm2835_gpio_set( DIO_PIN ); 
   else       bcm2835_gpio_clr( DIO_PIN );
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
   #if _DBG 
   printf( "DATA(%02X)\n", data);
   #endif
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
   if( bcm2835_gpio_lev( DIO_PIN ) == HIGH ) rv |= 1;
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

// The buffer must be 3 bytes long at least
void readKeys( uint8_t* buffer ) {

   sendByte( DATA_SET_CMD | RD_KEY_DATA );
   twait();

   // Set DIO pin in input mode
   bcm2835_gpio_fsel( DIO_PIN, BCM2835_GPIO_FSEL_INPT );

   buffer[0] = readByte();
   buffer[1] = readByte();
   buffer[2] = readByte();

   // Set DIO pin back in output mode
   bcm2835_gpio_fsel( DIO_PIN, BCM2835_GPIO_FSEL_OUTP );

   strobeDelay();
}

// Write leds
static void writeLeds( uint8_t data ) {

   sendByte( DATA_SET_CMD | WR_LEDS | INC_ADDRESS );
   twait();

   sendByte( data & 0x0F );
   strobeDelay();
}

// Current state variables
static uint8_t display_ctrl = DISPLAY_ON | MAX_BRIGHTNESS;
static uint8_t keys_buffer[3] = {0};
static uint8_t leds_status = 0;


// Public functions
int init_pt6312() {

   int rc = -1;

   // Initialize bcm2835 library
   if( bcm2835_init() )
   {
      rc = 0;

      // Set up pins
      bcm2835_gpio_fsel( IR_PIN,  BCM2835_GPIO_FSEL_INPT );
      bcm2835_gpio_fsel( CLK_PIN, BCM2835_GPIO_FSEL_OUTP );
      bcm2835_gpio_fsel( STB_PIN, BCM2835_GPIO_FSEL_OUTP );
      bcm2835_gpio_fsel( DIO_PIN, BCM2835_GPIO_FSEL_OUTP );

      strobeDelay();

#if _DBG
      printf( "-- CMD2: DATA_SET[WR_TO_DISPLAY]\n" );
#endif
      // Clear memory: CMD2
      sendByte( DATA_SET_CMD | WR_TO_DISPLAY | INC_ADDRESS );
      strobeDelay();

      // Send zeros to fill all memory
#if _DBG
      printf( "----------- BEGIN RESET MEMORY\n" ); 
      printf( "-- CMD3: ADDR_SET[0]\n" );
#endif
      sendByte( ADDR_SET_CMD | 0x00 );
      twait();

      for( uint8_t cnt=0; cnt < PT6312_MEM_SIZE; cnt++ ) {
#if _DBG
         printf( "   DATA[0]\n" );
#endif
         sendByteNoStb( 0x00 );
         if( cnt != PT6312_MEM_SIZE-1 )
            twait();
         else
            strobeDelay();
      }
#if _DBG
      printf( "----------- END RESET MEMORY\n" ); 
#endif

#if _DBG
      printf( "----------- BEGIN SET LEDS OFF\n" ); 
      printf( "-- CMD2: DATA_SET[LEDS]\n" );
#endif
      // Clear memory: CMD2
      writeLeds( 0x00 );
#if _DBG
      printf( "----------- END SET LEDS OFF\n" ); 
#endif

      // Send display configuration (mode set command)
#if _DBG
      printf( "-- CMD1: MODE_SET[16 GRID 16 SEGMENT]\n" );
#endif
      sendByte( MODE_SET_CMD | DISPLAY_6GRID_16SEGMENT_MODE );
      strobeDelay();

      // Send display on command
#if _DBG
      printf( "-- CMD4: DISPLAY_CTRL[MAX_BRT + DISPLAY_ON]\n" );
#endif
      sendByte( DISPLAY_CTRL_CMD | DISPLAY_ON | MAX_BRIGHTNESS );
      strobeDelay();
   }

   return rc;
}

void close_pt6312() {
   bcm2835_close();
}

// Set brightness value 0..7
void setBrightness( uint8_t brt ) {
   display_ctrl = ( display_ctrl & 0xF8 ) | ( brt & 0x7 );
}

// Set display on or off
void setDisplayOn() { display_ctrl &= 0x08; }
void setDisplayOff() { display_ctrl &= 0xF7; }
// Get display control data
uint8_t getDisplayStatus() { return display_ctrl; }

// Set LEDs status
void setLEDs( uint8_t leds ) {
   leds_status = leds & 0x0F;
}
// Get LEDs status
uint8_t getLEDs() { return leds_status; }

// Update LEDs with current value
void updateLeds() {

   sendByte( DATA_SET_CMD | WR_LEDS | INC_ADDRESS );
   twait();

   sendByte( leds_status & 0x07 );
   strobeDelay();
}

// Send a non-contiguous set of data. The offsets of the bytes to send comes in the array.
void updateDisplayPartial( const uint8_t* data, const uint8_t* indices, const int len ) {
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

// Send data to display and read keys
void updateDisplay( const uint8_t* data, const uint8_t offset, uint8_t len ) {

   if( offset+len > PT6312_MEM_SIZE ) return;

#if _DBG
      printf( "-- CMD2: DATA_SET[WR_TO_DISPLAY]\n" );
#endif
   sendByte( DATA_SET_CMD | WR_TO_DISPLAY | INC_ADDRESS );
   strobeDelay();

#if _DBG
   printf( "----------- BEGIN WRITE MEMORY\n" ); 
   printf( "-- CMD3: ADDR_SET[0]\n" );
#endif

   sendByte( ADDR_SET_CMD | (offset & 0x1F ) );
   twait();

   uint8_t idx=0;
   while( idx != len ) {
#if _DBG
      printf( "   DATA[%d]=%d\n", idx, data[idx+offset] );
#endif
      sendByteNoStb( data[idx+offset] );
      idx++;
      // After the last byte we set STB up again
      if( idx == len ) strobeDelay();
      else             twait();
   }

#if _DBG
      printf( "----------- END WRITE MEMORY\n" ); 
#endif

   // Read keys
#if _DBG
   printf( "----------- BEGIN READ KEYS\n" ); 
   printf( "-- CMD2: DATA_SET[READ_KEYS]\n" );
#endif
   readKeys( keys_buffer );
#if _DBG
   printf( "  READ[%02X%02X%02X]\n",
           keys_buffer[0], keys_buffer[1], keys_buffer[2] ); 
   printf( "----------- END READ KEYS\n" ); 
#endif

   // Write leds
#if _DBG
   printf( "----------- BEGIN WRITE LEDs\n" ); 
   printf( "-- CMD2: DATA_SET[WRITE_LEDS]\n" );
#endif
   writeLeds( leds_status );
#if _DBG
   printf( "----------- END WRITE LEDS\n" ); 
#endif

   // Update display status (on/off and brightness)
#if _DBG
   printf( "-- CMD1: MODE_SET[16 GRID 16 SEGMENT]\n" );
#endif
   sendByte( MODE_SET_CMD | DISPLAY_6GRID_16SEGMENT_MODE );
   strobeDelay();

#if _DBG
   printf( "-- CMD4: DISPLAY_CTRL[%0X]\n", display_ctrl );
#endif

   sendByte( DISPLAY_CTRL_CMD | display_ctrl );
   strobeDelay();
}

