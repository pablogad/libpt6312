#ifndef __PT6312_CMD__
#define __PT6312_CMD__

/* CMD1 definitions */
#define CMD1_CMD         0x00
#define MODE_SET_CMD     CMD1_CMD
#define DISPLAY_4GRID_16SEGMENT_MODE 0x00
#define DISPLAY_5GRID_16SEGMENT_MODE 0x01
#define DISPLAY_6GRID_16SEGMENT_MODE 0x02
#define DISPLAY_7GRID_15SEGMENT_MODE 0x03
#define DISPLAY_8GRID_14SEGMENT_MODE 0x04
#define DISPLAY_9GRID_13SEGMENT_MODE 0x05
#define DISPLAY_10GRID_12SEGMENT_MODE 0x06
#define DISPLAY_11GRID_11SEGMENT_MODE 0x07

/* CMD2 definitions */
#define CMD2_CMD          0x40
#define DATA_SET_CMD      CMD2_CMD
#define WR_TO_DISPLAY     0x00
#define WR_LEDS           0x01
#define RD_KEY_DATA       0x02
#define RD_SW_DATA        0x03
#define INC_ADDRESS       0x00
#define FIXED_ADDRESS     0x04

/* CMD3 definitions */
#define CMD3_CMD          0xC0
#define ADDR_SET_CMD      CMD3_CMD

/* CMD4 definitions */
#define CMD4_CMD          0x80
#define DISPLAY_CTRL_CMD  CMD4_CMD
#define DISPLAY_ON        0x08
#define DISPLAY_OFF       0x00
#define MIN_BRIGHTNESS    0x00
#define V2_16_BRIGHTNESS  0x01
#define V4_16_BRIGHTNESS  0x02
#define V10_16_BRIGHTNESS 0x03
#define V11_16_BRIGHTNESS 0x04
#define V12_16_BRIGHTNESS 0x05
#define V13_16_BRIGHTNESS 0x06
#define MAX_BRIGHTNESS    0x07

#endif

