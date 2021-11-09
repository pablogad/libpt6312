## libpt6312

This is a library to control a PT6312 (or compatible, like AD16312) VFD controller chip using the GPIO ports of a Raspberry pi.

The configuration of the VFD display in terms of grids and segments is specified with an external file with .def extension.

## Files

The target static library libpt6312.a is composed of the following modules:

   vfd_interface.cc : low level GPIO interface using memory mapped access.
                      By default, the GPIO pins used are the same as the SPI interface
                      of the raspberry pi. You can use any GPIOs modifying the constants
                      inside this file (CLK_PIN, STB_PIN, DIO_PIN and IR_PIN).
   
   display_def.cpp :  high level interface to manage the display data in user
                      friendly terms (symbols, characters, etc). This module
                      only controls the data that will be sent to the display,
                      but doesn't update the display by itself.

  test.cpp        : example showing the capabilities of the display. 

A sample is provided for several displays that I have, from some old DVDs. The grids
and segments definition for those particular displays are contained in the \*.def files
found inside the display_defs directory. As each VFD has its own distribution of segments
and grids, its quite probable that the ones I have are different from yours.

A tool to find out the definition for new displays is provided. It does a continuous loop
requesting for a grid and a value. The program will activate on the display only the
segments specified by the value on the entered grid. The segment value is a 16 bit integer
and each bit will lit a single segment on the grid when its value is 1:

  create_display.cc

When a grid value of 666 is entered, the program ends. The value can be a decimal number or
an hex number, when 0xHHHH syntax is used.

It is also possible to turn on of off all the segments at once with the command line options,
this is useful to confirm that the connection is working initially:

   --all-grids: show all grids activated
   --clear: deactivate all grids

It the displays known to me, there are three kinds of graphics:

  - Digits: they have 7 segments and all the segments of a digit are on the same grid.
            Sometimes there is a ':' symbol by the digit. In the .def file this is
            associated to the digit to its left.

  - Symbols: those are individual graphics using a single segment.

  - Round group: a segmented circle that can be animated when playing to give the impression
            of a rotating CD.

Each of them are defined on the .def file with an associated label.
Once you have the values for all the segments, you can create your own .def file for the
display.  The format for every segment on the .def file is G.HHHH where G=grid, HHHH=hex
value of the segment. In the def file there are structures to contain the segments of
each known element, for example, a Digit contains seven segments.

The Digit elements are set up inside Groups, which are collections of digits shown
together on the display. Each digit can also have a ':' symbol associated to it, to
make easy to show time formats.

The def file can also contain a list of key codes to translate the 24 bits codes obtained
by pressing each key during the execution of the utility. Refer to the vfdblusky.def file
to see an example of configuration.

Refer to the existing files to see the self-explanatory structure. In display_def.h
there are several enums with the label definitions. If your display has a new symbol,
you can add it to the enum.


## Library usage

The PT6312 chip keeps an internal memory of 11 grids with 16 segments each. A bit set to 1
on this memory causes the corresponding segment of the display to be lit.

This library uses a buffer that contains all the possible segments. To show data on the
display, you set values on this buffer using the methods in DisplayDef, or you can even fill
the buffer on your own if you wish.

There are two classes to use:

   - DisplayDef : contains the definition of the display read from the .def file. This class
                  has methods to help writing to the display (digits, symbols and group). It
                  uses a private buffer.

   - VfdInterface : used to control the PT6312 hardware at the lowest level. It can:

         * update the display.
         * read the keys.
         * turn on/off the LEDs.

     There are two options, build the interface passing an instance of DisplayDef or without
     it. In the first case the display can be updated with the buffer kept by the DisplayDef
     instance, in the second case the user has to pass a buffer kept in his/her program
     (the size is 22 bytes as mentioned before).

     After building the VfdInterface instance, we have to call Init() to set up GPIOs.

     Example:

        DisplayDef displayDef("/home/test/VFD/vfd.def");
        VfdInterface vfd( &displayDef );
        vfd.Init();

        vfd.updateDisplay();

     Refer to the test program to see how the data is sent to the display and updated.


The class DisplayDef keeps its copy of the data, you can get a pointer to it with the
getData() method.

When the buffer is filled with the segments to be lit, you call the methods updateDisplay
of VfdInterface:

   - updateDisplay() : uses the buffer kept by DisplayDef. Only works if the VfdInterface
                      instance was created with a DisplayDef and the definition file was
                      open successfully.
   - updateDisplay( const uint8_t\* data, offset, len ) : uses a buffer passed by the user
                      and only updates 'len' bytes on the display


## Keys

The PT6312 chip in addition to manage the VFD display can also read keys (for example,
play, stop, pause, next, prev...).
The keys are read when the display is updated (method updateDisplay on VfdInterface class).
To read the raw 32 bit value representing the pressed key(s), use the method readKeys().


## Compilation (library, tools and example)

mkdir build

cd build

cmake .. && make ; cd -

Result of the compilation (these files are left in build directory):

   - libpt6312.a : static library needed to access the VFD display.

   - create_disp : tool to aid the creation of new display definition files. Needed because
                   every display has a different distribution of grids and segments.

   - test_pt6312 : test program that shows symbols and digits on the display.

The test program looks by default for a file called vfd1.def on the current directory, so it
should be run from the directory where the file is located, but it is also possible to pass
the complete path of another definition file as a command line parameter.


## Physical connection

The PT6312 chip has 4 pins for its control interface:

  CLK  Clock pulse, sent with every bit to be transferred.
  STB  Strobe: pulse sent at the end of a byte transmitted.
  DIN  
  DOUT Data IO. The IN and OUT pins are normally connected together, as the transmission is
       either read or write.

Note that the PT6312 works at 5v logic levels while the Raspi works at 3v3, so you'll have
to set up some kind of interface to connect those lines to GPIOs of the Raspberry pi.
In practice, the CLK and STB lines can be connected directly as the 3v3 level is interpreted
as a logic 1 in the 5v logic side. The DIO line OTH needs bidirectional level conversion to
avoid damaging the Raspberry pi.
