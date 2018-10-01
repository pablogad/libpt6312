## libpt6312

This is a library to control a PT6312 (or compatible, like AD16312) VFD controller chip using the GPIO ports of a Raspberry pi.

The configuration of the VFD display in terms of grids and segments is specified with an external file with .def extension.

## Files

The target static library libpt6312.a is composed of the following modules:

   pt6312.c        : low level GPIO interface using the libbcm2835 library.
   
   display_def.cpp : high level interface to manage the display data in user
                     friendly terms (symbols, characters, etc). This module
                     only controls the data that will be sent to the display,
                     but doesn't update the display by itself.

A sample is provided for one of the displays that I have, from an old DVD. The grids
and segments definition for this particular display is contained in the vfd1.def file.

  test.cpp        : example showing the capabilities of the display.

Also a tool to find out the data for new displays is provided. It has a continuous loop
requesting for a grid and a value. The program will activate only the segments specified
by the value on the entered grid. The value is a 16 bit integer and each bit will lit a
single segment on the grid when its value is 1.

  create_display.c

When a grid value of 0 is entered, the program ends. The value can be a decimal number or
an hex number, when 0xHHHH syntax is used.

Once you have the values for all the segments, you can create your own .def file for the
display. In this file, the grid numbers starts at 0, while in create_display the first
grid is 1, you will have to substract one from the grid values. The format for every
segment is G.HHHH where G=grid, HHHH=hex value of the segment.

When executed with parameters,
   --all-grids: turns on all the segments in all the grids on the display.
   --clear    : turns off all the grids/segments on the display.
   --help     : prints a help message showing the options.

## Compilation (library, tools and example)

mkdir build

cd build

cmake .. && make ; cd -


The example looks for the vfd1.def file on the current directory, so it should be run
from the directory where the file is located. Alternatively, you can pass a display
definition file name (.def) with any valid path.

## External dependencies

  http://www.airspayce.com/mikem/bcm2835/ - libbcm2835

