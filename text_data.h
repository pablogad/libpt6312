#include <cstdint>
#include <cstring>
#include <string>

class TextData {

protected:
   char text[22];
   uint8_t text_size;

public:
   TextData( const std::string& str ) : text_size( str.size() ) {
      strncpy( text, str.c_str(), str.size() > 22 ? 22 : str.size() );
   }
};

