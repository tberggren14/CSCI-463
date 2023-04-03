#include "hex.h"


/*
This function must return a std::string with exactly 2 hex digits representing the 8 bits of
the i argument.
The following code snipit is one way to format an 8-bit integer into a 2-character hex string with a leading zero:
*/

/**
This function must return a std::string with exactly 2 hex digits representing the 8 bits of
the i argument.
 *
 * @param i a 8 bit unsigned hex number used for address in vector
 * @return string
 * prints the int as a hex  
 ********************************************************************************/
string hex::to_hex8(uint8_t i)
{
     
    std::ostringstream os;
    os << std::hex << std::setfill('0') << std::setw(2) << static_cast<uint16_t>(i);
    return os.str();


}

/**
This function must return a std::string with 8 hex digits representing the 32 bits of the i
argument.
 *
 * @param i a 8 bit unsigned hex number used for address in vector
 * @return string
 * prints the int as a hex  
 ********************************************************************************/
string hex::to_hex32(uint32_t i)
{

    std::ostringstream os;
    os << std::hex << std::setfill('0') << std::setw(8) << i;
    return os.str();


}

/**
This function must return a std::string beginning with 0x, followed by the 8 hex digits representing 
 *
 * @param i a 32 bit unsigned hex number used for address in vector
 * @return string
 * prints the int as a hex  
 ********************************************************************************/
string hex::to_hex0x32(uint32_t i)
{

    return std::string("0x")+to_hex32(i);

}