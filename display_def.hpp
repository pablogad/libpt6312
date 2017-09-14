#pragma once

#include <string>
#include <vector>
#include <cstdint>

// Possible symbol codes
enum class SymbolId : uint8_t { SYM_DVD=1,
                                SYM_DTS=2,
                                SYM_S=3,
                                SYM_V=4,
                                SYM_CD=5,
                                SYM_Play=6,
                                SYM_Pause=7,
                                SYM_ALL=8,
                                SYM_Loop=9,
                                SYM_DolbyD=10,
                                SYM_Camera=11,
                                SYM_PBC=12,
                                SYM_MP3=13 };

// Possible colors
enum class ColorId : uint8_t { White=0, Red=1, Yellow=2, Orange=3 };


// Definition of a digit
typedef struct {
   uint8_t grid;          // Grid 0..n
   uint16_t segments[7];  // Order: Up,UpLeft,UpRight,Med,DnLeft,DnRight,Dn
                          //     ---Up---
                          //    |        |
                          // UpRight  UpLeft
                          //    |        |
                          //    |---Med--|
                          //    |        |
                          // DnRight  DnLeft
                          //    |        |
                          //     ---Dn---

   bool hasDots;
   uint8_t dotsGrid;
   uint16_t dotsCode;  // Only if its got dots
} Digit;

// Definition of a group of digits
typedef struct {
   std::vector<Digit> digits;
   uint8_t color;
} DigitGroup;

// Definition of the round sector: may only exist one
typedef struct {
   std::vector<uint16_t> sectors;
   uint8_t grid;
} RoundSector;


// Definition of a symbol
typedef struct {
   SymbolId symbolCode;  // Id of the symbol
   uint8_t grid;   // Grid 0..n
   uint8_t usercode; // Reference for the user
   uint16_t code;  // Code of the bit for the symbol inside the grid
   uint8_t color;  // Color of this symbol
} Symbol;


// Display definition
class DisplayDef {

public:
   DisplayDef( const char* fileName );

   // Clear all the contents of the display
   void clearData();
   // Symbols
   void setSymbol( const uint8_t usercode );
   void setSymbol( const SymbolId code );
   void resetSymbol( const uint8_t usercode );
   void resetSymbol( const SymbolId code );
   // Digits / "chars"
   void setDigits( const uint8_t group, const std::string& str, const uint8_t offset=0 );
   void resetDigit( const uint8_t group, const uint8_t offset, const uint8_t len=1 );
   void clearDigits();
   void setDots( const uint8_t group, const uint8_t index );
   void removeDots( const uint8_t group, const uint8_t index );
   // Round signal
   void clearRoundSector();
   void setRoundSectorLevel( const uint8_t level );
   void setRoundSectorSectors( const int* index_list );

   uint8_t* getData();  // Get buffer pointer

   bool isLoaded();

private:

   // Convert symbols to display format
   uint8_t translateChar( const char c );
   void setDataBits( const uint8_t grid, const uint16_t code );
   void resetDataBits( const uint8_t group, const uint16_t segments );
   void setChar( const Digit& d, const uint8_t c );

   // Members
   std::vector<DigitGroup> groupList;
   std::vector<Symbol> symbols;
   RoundSector roundSec;

   bool loaded;

   uint8_t data[22];  // Memory space to dump to PT6312
};

