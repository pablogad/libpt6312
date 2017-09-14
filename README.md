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

## Compilation (library and example)

mkdir build
cmake .. && make

The example looks for the vfd1.def file on the current directory, so it should be run
from the directory where the file is located.

## External dependencies

  http://www.airspayce.com/mikem/bcm2835/ - libbcm2835

