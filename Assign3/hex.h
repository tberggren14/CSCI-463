#ifndef HEX_H
#define HEX_H
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <stdio.h>

using std::string;

class hex 
{
public:
    static string to_hex8(uint8_t );
    static string to_hex32(uint32_t );
    static string to_hex0x32(uint32_t );
};

#endif
