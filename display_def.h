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
                                SYM_MP3=13,
                                SYM_VCD=14,
                                SYM_TOTAL=15,
                                SYM_PROG=16,
                                SYM_PAIR=17,
                                SYM_LOCK=18,
                                SYM_ANGLE=19,
                                SYM_RAND=20,
                                SYM_A=21,
                                SYM_MB=22,
                                SYM_1L=23,
                                SYM_2R=24 };

enum class KeyId : uint32_t { KEY_PLAY=1, KEY_PAUSE=2, KEY_STOP=3,
                              KEY_REW=4, KEY_FWD=5, KEY_PREV=6, KEY_NEXT=7,
                              KEY_EJECT=8, KEY_NP=9,
                              KEY_NONE=0xFF };

// Possible colors
enum class ColorId : uint8_t { White=0, Red=1, Yellow=2, Orange=3 };


// Definition of a digit
typedef struct _Digit {

   _Digit() : hasAdd( false ), hasDots( false ) {}

   uint8_t grid;          // Grid 0..n
   uint16_t segments[8];  // Order: Up,UpLeft,UpRight,Med,DnLeft,DnRight,Dn,Add
                          //     ---Up---
                          //    |        |
                          // UpLeft   UpRight
                          //    |        |
                          //    |---Med--|
                          //    |        |
                          // DnLeft   DnRight
                          //    |        |
                          //     ---Dn---
                          // Add: vertical or slash bar if any (hasAdd true)

   bool hasAdd;
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

// Definition of a key
typedef struct {
   KeyId keyCode;  // Id of the key
   uint32_t scanCode; // Scan code sent by PT6312
} Key;


// Display definition
class DisplayDef {

public:

   DisplayDef( const std::string& fileName );

   // Clear all the contents of the display
   void clearData();

   // Symbols
   void setSymbol( const uint8_t usercode );
   void setSymbol( const SymbolId code );
   void resetSymbol( const uint8_t usercode );
   void resetSymbol( const SymbolId code );

   // Digits / "chars"
   void setDigits( const uint8_t group, const std::string& str, const uint8_t offset=0 );
   void setDigits( const std::string& str, const uint8_t offset=0 );
   void setDigitCustomData( const uint8_t group, const uint8_t offset, const uint8_t segment_bits );
   void resetDigit( const uint8_t group, const uint8_t offset, const uint8_t len=1 );
   void resetDigit( const Digit& digit );
   void clearDigits();
   bool hasDots( const uint8_t group, const uint8_t index );
   void setDots( const uint8_t group, const uint8_t index );
   void removeDots( const uint8_t group, const uint8_t index );
   void removeDots( const uint8_t group );

   // Round signal
   void clearRoundSector();
   uint8_t getRoundSectorLevelCount() const;
   void setRoundSectorSegment( const uint8_t level );
   void setRoundSectorLevel( const uint8_t level );
   void resetRoundSectorLevel( const uint8_t level );
   void setRoundSectorSectors( const int* index_list );

   // Digit groups
   uint8_t getNumberOfGroups();
   uint8_t getNumberOfDigitsOnGroup( const uint8_t group );
   void resetGroup( const uint8_t group );
   bool canShowTime( const uint8_t group, uint8_t& posIni );
   bool canShowHMS( const uint8_t group, uint8_t& posIni );

   // Key state
   void getKeys( std::vector<KeyId> &keys, uint32_t keys_buffer );

   uint8_t* getData();  // Get buffer pointer. Length of buffer is 22 bytes.
   std::vector<uint8_t> getDifferences( const uint8_t* other_buffer, const uint8_t len=22 );

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
   std::vector<Key> keys;
   RoundSector roundSec;

   bool loaded;

   uint8_t data[22];  // Memory space to dump to PT6312: 11 digits x 16 grids
};

