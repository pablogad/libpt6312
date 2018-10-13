#include <cstring>
#include <string>
#include <iostream>
#include <fstream>
#include <regex>

#include "display_def.hpp"
#include "char_table.hpp"

// Translate color string to enumeration
static inline uint8_t getColor( const std::string& color ) {
   if( color == "Red" )    return static_cast<uint8_t>( ColorId::Red );
   if( color == "Yellow" ) return static_cast<uint8_t>( ColorId::Yellow );
   if( color == "Orange" ) return static_cast<uint8_t>( ColorId::Orange );
   if( color == "White" )  return static_cast<uint8_t>( ColorId::White );
   throw std::runtime_error( "Color " + color + " not recognized" );
}

// Get the grid and segment values from a g.ssss string
static inline std::pair<uint8_t,uint16_t> getGridSegment( const std::string& grid_seg_data ) {
   if( grid_seg_data.length() != 6 || grid_seg_data[1] != '.' )
      throw std::runtime_error( "Invalid grid specification " + grid_seg_data );
   std::pair<uint8_t,uint16_t> rv;
   try {
      rv.first  = std::stoi( grid_seg_data.substr(0,1) );
      rv.second = std::stoi( grid_seg_data.substr(2), nullptr, 16 );  // Hex number
   } catch( const std::invalid_argument& e ) {
      throw std::runtime_error( "Invalid grid or segment values in " + grid_seg_data );
   } catch( const std::out_of_range& e ) {
      throw std::runtime_error( "Out of range value in " + grid_seg_data );
   }

   return rv;
}


// Display definition
DisplayDef::DisplayDef( const char* fileName ) : loaded(false) {
   clearData();
   std::ifstream ifs( fileName );

   if( ifs.is_open() ) {

      std::regex line_regex( "([ ]*)([A-Za-z0-9]+) *([,\\.A-Za-z0-9]*) *" ); 
      std::regex data_regex( "([0-8]\\.[0-9A-Fa-f]{4})" );
      std::regex symbol_regex( " *Symbol([A-Za-z0-9]+) +([0-9]{1,2}),([0-8])\\.([0-9A-Fa-f]{4})" );

      std::string line;

      bool inDef = false;
      bool inDigitGroup = false;
      bool inDigit = false;
      bool inRoundSector = false;

      uint8_t numSector = 0;

      while( ifs.good() ) {
         std::getline( ifs, line );
         // Delete comments
         size_t pos = line.find('#');
         if( pos != std::string::npos ) {
            line.erase( pos );
         }
         if( line.length() == 0 )
            continue; // Ignore empty lines

         // Decompose fields of line
         std::smatch line_match;
         if( std::regex_match( line, line_match, line_regex ) ) {
            if( line_match.size() != 4 ) {
               // Invalid line
               throw std::runtime_error( "Invalid line " + line );
            }
         }

         if( inDef == false && line_match[2] == "Begin" ) { 
            // Ignore everything except begin of definition
            inDef = true;
            
            continue;
         }
         else {
            // Read data
            std::string command = line_match[2].str();
            try {
            if( command == "End" ) {
               // End of data on file
               break;
            }
            // Main functions - sort from less to more lengths!!
            else if( command.substr(0,3) == "Sec" ) {
               if( inRoundSector == false )
                 throw std::runtime_error( "Invalid token " + command + " not inside a RoundSector" );
               // Must be consecutive
               uint8_t sector_number = std::stoi( command.substr(3) );
               std::pair<uint8_t,uint16_t> gs = getGridSegment( line_match[3] );
               if( sector_number != roundSec.sectors.size()+1 )
                  throw std::runtime_error( "Sector numbers must be consecutive and starting at 1" );
               if( roundSec.grid == 0xFF )
                  roundSec.grid = gs.first;
               if( roundSec.grid != gs.first )
                  throw std::runtime_error( "All sectors must belong to the same grid" );
               roundSec.sectors.push_back( gs.second );
            }
            else if( command.substr(0,3) == "Seg" ) {
               if( inDigit == false )
                 throw std::runtime_error( "Invalid token " + command + " not inside a Digit" );

               DigitGroup& currentDigitGroup = groupList.back();
               Digit& currentDigit = currentDigitGroup.digits.back();

               std::string grid_seg_data = line_match[3].str();
               uint8_t grid = std::stoi( grid_seg_data.substr(0,1) );
               uint16_t segcode = std::stoi( grid_seg_data.substr(2), nullptr, 16 );
               if( grid_seg_data.length() != 6 || grid_seg_data[1] != '.' )
                  throw std::runtime_error( "Invalid grid specification " + grid_seg_data );

               if( currentDigit.grid == 0xFF )
                  currentDigit.grid = grid;
               if( currentDigit.grid != grid )
                  throw std::runtime_error( "All the segments in a digit must belong to the same grid" );

               if( command == "SegHorUp" ) {
                  currentDigit.segments[0] = segcode;
               }
               else if( command == "SegVerUpL" ) {
                  currentDigit.segments[1] = segcode;
               }
               else if( command == "SegVerUpR" ) {
                  currentDigit.segments[2] = segcode;
               }
               else if( command == "SegHorMed" ) {
                  currentDigit.segments[3] = segcode;
               }
               else if( command == "SegVerDnL" ) {
                  currentDigit.segments[4] = segcode;
               }
               else if( command == "SegVerDnR" ) {
                  currentDigit.segments[5] = segcode;
               }
               else if( command == "SegHorDn" ) {
                  currentDigit.segments[6] = segcode;
               }
               else if( command == "SegAdd" ) {
                  currentDigit.segments[7] = segcode;
                  currentDigit.hasAdd = true;
               }
               else throw std::runtime_error( "Invalid token " + command );
            }
            else if( command.substr(0,4) == "Dots" ) {
               if( inDigit == false )
                 throw std::runtime_error( "Invalid token Dots not inside a Digit" );
               Digit& currentDigit = groupList.back().digits.back();
               std::pair<uint8_t,uint16_t> gs = getGridSegment( line_match[3] );
               currentDigit.hasDots = true;
               currentDigit.dotsGrid = gs.first;
               currentDigit.dotsCode = gs.second;
            }
            else if( command.substr(0,5) == "Color" ) {
               if( inDigit != false || inDigitGroup == false )
                 throw std::runtime_error( "Invalid token Color not in a DigitGroup" );
               groupList.back().color = getColor( line_match[3] );
            }
            else if( command.substr(0,5) == "Digit" && command[5] != 's' ) {
               if( inDigitGroup == false )
                  throw std::runtime_error( "Invalid token Digit out of a DigitGroup" );
               inDigit = true;
               Digit newDigit;
               newDigit.grid = 0xFF;
               groupList.back().digits.push_back( newDigit );
            }
            else if( command.substr(0,6) == "Symbol" ) {
               inDigitGroup = false;
               inDigit = false;
               inRoundSector = false;

               std::smatch symbol_match;

               if( std::regex_match( line, symbol_match, symbol_regex ) ) {
                  if( symbol_match.size() != 5 )
                     throw std::runtime_error( "Invalid symbol line " + line );

                  Symbol newSym;

                  try {
                    const std::string& symStr = symbol_match[1].str();
                    newSym.usercode = std::stoi( symbol_match[2].str() );
                    newSym.grid = std::stoi( symbol_match[3].str() );
                    newSym.code = std::stoi( symbol_match[4].str(), nullptr, 16 );

                    if( symStr == "Dvd" ) { newSym.symbolCode = SymbolId::SYM_DVD; }
                    else if( symStr == "Dts" ) { newSym.symbolCode = SymbolId::SYM_DTS; }
                    else if( symStr == "S" ) { newSym.symbolCode = SymbolId::SYM_S; }
                    else if( symStr == "V" ) { newSym.symbolCode = SymbolId::SYM_V; }
                    else if( symStr == "CD" ) { newSym.symbolCode = SymbolId::SYM_CD; }
                    else if( symStr == "Play" ) { newSym.symbolCode = SymbolId::SYM_Play; }
                    else if( symStr == "Pause" ) { newSym.symbolCode = SymbolId::SYM_Pause; }
                    else if( symStr == "ALL" ) { newSym.symbolCode = SymbolId::SYM_ALL; }
                    else if( symStr == "Loop" ) { newSym.symbolCode = SymbolId::SYM_Loop; }
                    else if( symStr == "DolbyD" ) { newSym.symbolCode = SymbolId::SYM_DolbyD; }
                    else if( symStr == "Camera" ) { newSym.symbolCode = SymbolId::SYM_Camera; }
                    else if( symStr == "PBC" ) { newSym.symbolCode = SymbolId::SYM_PBC; }
                    else if( symStr == "MP3" ) { newSym.symbolCode = SymbolId::SYM_MP3; }
                    else if( symStr == "VCD" ) { newSym.symbolCode = SymbolId::SYM_VCD; }
                    else if( symStr == "TOTAL" ) { newSym.symbolCode = SymbolId::SYM_TOTAL; }
                    else if( symStr == "PROG" ) { newSym.symbolCode = SymbolId::SYM_PROG; }
                    else if( symStr == "Pair" ) { newSym.symbolCode = SymbolId::SYM_PAIR; }
                    else if( symStr == "Lock" ) { newSym.symbolCode = SymbolId::SYM_LOCK; }
                    else if( symStr == "ANGLE" ) { newSym.symbolCode = SymbolId::SYM_ANGLE; }
                    else if( symStr == "PROG" ) { newSym.symbolCode = SymbolId::SYM_PROG; }
                    else if( symStr == "RAND" ) { newSym.symbolCode = SymbolId::SYM_RAND; }
                    else if( symStr == "A" ) { newSym.symbolCode = SymbolId::SYM_A; }
                    else if( symStr == "MB" ) { newSym.symbolCode = SymbolId::SYM_MB; }
                    else if( symStr == "1L" ) { newSym.symbolCode = SymbolId::SYM_1L; }
                    else if( symStr == "2R" ) { newSym.symbolCode = SymbolId::SYM_2R; }

                    else throw std::runtime_error( "Undefined symbol " + command + ", " + symStr + ". Add new symbols to source code as needed" );
                  } catch( std::invalid_argument& e ) {
                      throw std::runtime_error( "Invalid number in symbol " + command );
                  }

		  // Add symbol to list
		  symbols.push_back( newSym );
               }
               else throw std::runtime_error( "Symbol not recognized " + command + ". Add new symbols to source code as needed" );
            }
            else if( command.substr(0,11) == "DigitsGroup" ) {
               inDigitGroup = true;
               DigitGroup newDigitGroup;
               groupList.push_back( newDigitGroup );
            }
            else if( command.substr(0,11) == "RoundSector" ) {
               inRoundSector = true;
               inDigitGroup = false;
               inDigit = false;

               roundSec.sectors.clear();
               roundSec.grid = 0xFF;
            }
            } catch( std::out_of_range& e ) {
               break;
            }
         }
      }
      loaded = true;
      ifs.close();
   }
}

// True if a definition has been read successfully from a file
bool DisplayDef::isLoaded() { return loaded; }

// Sets display buffer to 0s
void DisplayDef::clearData() {
   memset( data, 0, 22 );
}

inline void DisplayDef::setDataBits( const uint8_t grid, const uint16_t code ) {
   int address = grid * 2;
   data[address] |= static_cast<uint8_t>(code&0x00FF);
   data[address+1] |= static_cast<uint8_t>((code&0xFF00) >> 8);
}

// Code has 1s on the segments to clear
inline void DisplayDef::resetDataBits( const uint8_t grid, const uint16_t code ) {
   int address = grid * 2;
   uint16_t segmask = code ^ 0xFFFF;  // Invert code, 1s on the segments not to clear
   data[address] &= static_cast<uint8_t>(segmask & 0xFF);
   data[address+1] &= static_cast<uint8_t>((segmask & 0xFF00) >> 8);
}

// Write a symbol to the display memory if it exists
// (if the Symbol code, among the list defined on header file, is
// declared on the definition file)
void DisplayDef::setSymbol( const SymbolId code ) {
   for( Symbol sym : symbols ) {
      if( sym.symbolCode == code ) {
         setDataBits( sym.grid, sym.code );
         break;
      }
   }
}
// Write a symbol to the display memory if it exists, using
// the user code declared on the definition file
void DisplayDef::setSymbol( const uint8_t usercode ) {
   for( Symbol sym : symbols ) {
      if( sym.usercode == usercode ) {
         setDataBits( sym.grid, sym.code );
         break;
      }
   }
}

// Delete a symbol from the display memory by symbol code
// If this code is declared on the definition file, it gets cleared
// from the display
void DisplayDef::resetSymbol( const SymbolId code ) {
   for( Symbol sym : symbols ) {
      if( sym.symbolCode == code ) {
         resetDataBits( sym.grid, sym.code );
         break;
      }
   }
}
// Delete a symbol from the display memory using the user code associated
// to the symbol on the definition file
void DisplayDef::resetSymbol( const uint8_t usercode ) {
   for( Symbol sym : symbols ) {
      if( sym.usercode == usercode ) {
         resetDataBits( sym.grid, sym.code );
         break;
      }
   }
}

// Write a single character to the display memory
void DisplayDef::setChar( const Digit& d, const uint8_t v ) {
   int address = d.grid * 2;
   uint16_t segments = 0;  // The two bytes to combine with the grid
   if( v&1 )   segments += d.segments[0];
   if( v&2 )   segments += d.segments[1];
   if( v&4 )   segments += d.segments[2];
   if( v&8 )   segments += d.segments[3];
   if( v&16 )  segments += d.segments[4];
   if( v&32 )  segments += d.segments[5];
   if( v&64 )  segments += d.segments[6];
   if( ( v&128 ) && d.hasAdd ) segments += d.segments[7];
   setDataBits( d.grid, segments );
}

// Clear a digit on the display memory (no segments lit for the digit)
//    group : group of digits where the digit is
//    index : the 0..n index of the digit inside the group
void DisplayDef::resetDigit( const uint8_t group, uint8_t index, uint8_t len ) {
   if( group < groupList.size() ) {

      const DigitGroup& g = groupList[ group ];

      if( g.digits.size() <= index+len )
      while( len-- ) {
         const Digit& d = g.digits[ index++ ];

         // Create a mask with 0s on the segments to clear
         uint16_t segments = d.segments[0] + d.segments[1] + d.segments[2] + \
                             d.segments[3] + d.segments[4] + d.segments[5] + \
                             d.segments[6];
         if( d.hasAdd ) segments += d.segments[7];
         resetDataBits( d.grid, segments );
      }
   }
}
// Clear all the digits on the display memory
void DisplayDef::clearDigits() {
   for( const DigitGroup& g : groupList ) {
       for( const Digit& d : g.digits ) {
          // Create a mask with 0s on the segments to clear
          uint16_t segments = d.segments[0] + d.segments[1] + d.segments[2] + \
                              d.segments[3] + d.segments[4] + d.segments[5] + \
                              d.segments[6];
          if( d.hasAdd ) segments += d.segments[7];
          resetDataBits( d.grid, segments );
       }
   }
}

// Method to translate a character to a 8 digit resembling it more or less.
// The returned value has the following format:
//    bit 0: segment Up
//    bit 1: segment UpLeft
//    bit 2: segment UpRight
//    bit 3: segment Med
//    bit 4: segment DnLeft
//    bit 5: segment DnRight
//    bit 6: segment Dn
inline uint8_t DisplayDef::translateChar( const char c ) {

   uint8_t v = 0x7F; // By default all lit

   // Convert with table
   std::map<char,uint8_t>::const_iterator it = charToDisplayMap.find(c);
   if( it != charToDisplayMap.end() )
      v = charToDisplayMap.find(c)->second;

   return v;
}

// Write digits of a digits group. Tries to translate characters
// to something meaningful if possible.
// Excess characters on str are ignored. Does nothing if group does not exist.
//    group : digits group
//    offset: initial digit on the digit group (0 by default)
void DisplayDef::setDigits( const uint8_t group, const std::string& str, const uint8_t offset ) {
   if( group < groupList.size() ) {
      const DigitGroup& g = groupList[ group ];
      int idx=static_cast<int>(offset);
      for( char c : str ) {
         if( idx > g.digits.size() ) break;
         const Digit& d = g.digits[idx++];
         setChar( d, translateChar(c) );
      }
   }
}

// Write digits by position. The position is counted starting in the first group (G0) and
// increases until the number of digits of the group are filled, then continues on to
// the next group, and so on.
// Tries to translate characters to something meaningful if possible.
// Excess characters on str are ignored.
//    offset: initial digit counting from digit 0 on group 0
void DisplayDef::setDigits( const std::string& str, const uint8_t offset ) {

   uint8_t group = 0;
   int current_digit = static_cast<int>( offset );
   DigitGroup& g = groupList[ group ];

   // Look for the initial group and digit at offset
   while( 1 ) {
      if( current_digit < g.digits.size() ) break;  // Found!
      current_digit -= g.digits.size();
      group++;
      if( group >= groupList.size() ) break;
      g = groupList[ group ];
   }

   // Transfer chars starting at group/current_digit
   for( char c : str ) {
      if( current_digit >= g.digits.size() ) {

         // All chars of the group transferred, go onto the next group
         group++;

       	 // Exit if no more groups
         if( group >= groupList.size() ) break;

         g = groupList[ group ];
         current_digit = 0;  // First digit in the next group
      }
      const Digit& d = g.digits[ current_digit++ ];
      setChar( d, translateChar(c) );
   }
}

// Write a digit of a group specifying the segments of the digit to lit.
// See display_def.hpp to see the bit field values needed to set every segment.
void DisplayDef::setDigitCustomData( const uint8_t group, const uint8_t offset, const uint8_t segment_bits ) {
   if( group < groupList.size() ) {
      const DigitGroup& g = groupList[ group ];
      if( g.digits.size() > offset ) {
         const Digit& d = g.digits[ offset ];
         setChar( d, segment_bits );
      }
   }
}

// Write dots if defined for the group. If not, the call is ignored.
//    group : digits group containing the : character
//    index : the index of the digit having the : in the group
void DisplayDef::setDots( const uint8_t group, const uint8_t index ) {
   if( group < groupList.size() ) {
      const DigitGroup& g = groupList[ group ];
      const Digit& d = g.digits[index];
      if( d.hasDots ) {
         setDataBits( d.dotsGrid, d.dotsCode );
      }
   }
}
void DisplayDef::removeDots( const uint8_t group, const uint8_t index ) {
   if( group < groupList.size() ) {
      const DigitGroup& g = groupList[ group ];
      const Digit& d = g.digits[index];
      if( d.hasDots ) {
         resetDataBits( d.dotsGrid, d.dotsCode );
      }
   }
}

// Clear round sector: remove all segments of the RS from display memory
void DisplayDef::clearRoundSector() {
   uint16_t segments = 0;
   for( uint16_t s : roundSec.sectors ) {
      segments += s;
   }
   resetDataBits( roundSec.grid, segments );
}
// Get number of levels of the RS
uint8_t DisplayDef::getRoundSectorLevelCount() {
   return roundSec.sectors.size();
}
// Set the number of sectors requested starting by 12 o'clock position
void DisplayDef::setRoundSectorLevel( const uint8_t level ) {
   if( level <= roundSec.sectors.size() ) {
      int index = 0;
      clearRoundSector();
      uint16_t segments = 0;
      while( index != level ) segments += roundSec.sectors[ index++ ];
      setDataBits( roundSec.grid, segments );
   }
}
// Set the sectors having the indexes contained on the list.
// The end of the list is signaled by a -1 value.
void DisplayDef::setRoundSectorSectors( const int* idx_list ) {
   int cnt=0;
   uint16_t segments = 0;
   clearRoundSector();
   while( cnt<roundSec.sectors.size() && idx_list[cnt] != -1 && idx_list[cnt] < roundSec.sectors.size() ) {
      segments += roundSec.sectors[ idx_list[cnt] ];
      cnt++;
   }
   setDataBits( roundSec.grid, segments );
}

// Returns number of digit groups
uint8_t DisplayDef::getNumberOfGroups() {
   return groupList.size();
}

// Returns number of digits on a group. 0 if the group does not exist.
uint8_t DisplayDef::getNumberOfDigitsOnGroup( const uint8_t group ) {

   uint8_t val = 0;

   if( group < groupList.size() ) {

      const DigitGroup& g = groupList[ group ];

      val = static_cast<uint8_t>( g.digits.size() );
   }

   return val;
}

// Return internal data memory representation of the display memory.
uint8_t* DisplayDef::getData() { return data; }

// Return a list of grids that have changed in relation to the data array
std::vector<uint8_t> DisplayDef::getDifferences( const uint8_t* other_data, const uint8_t len ) {
   std::vector<uint8_t> vec;
   if( len <= 22 ) {
      uint8_t idx = 0;
      while( idx != len ) {
         if( other_data[idx] != data[idx] )
            vec.push_back( idx );
	 idx++;
      }
   }
   return vec;
}

