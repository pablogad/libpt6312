#ifndef __PT6312_H__
#define __PT6312_H__

int init_pt6312();
void close_pt6312();

void readKeys( uint8_t* buffer );
void writeLeds( uint8_t data );
void updateDisplay( const uint8_t* data, const uint8_t offset, uint8_t len );
void updateLeds();

void setBrightness( uint8_t brt );
void setDisplayOn();
void setDisplayOff();
void setLEDs( uint8_t leds );

uint8_t getDisplayStatus();
uint8_t getLEDs();

#endif

