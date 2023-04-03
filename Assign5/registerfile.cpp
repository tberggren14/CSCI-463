#include "registerfile.h"

/**
 * Constructor 
 * @return void  
 *
 * @note resets registerfile with default values 
 ********************************************************************************/
registerfile::registerfile()
{
    reset(); 
}

/**
 * reset function to reset registerfile 
 * @return void  
 *
 * @note resets all 32 regs with default values besides reg 0 -> 0x0
 ********************************************************************************/
void registerfile::reset()
{
    registers.resize(32,0xf0f0f0f0);
    registers[0] = 0x0;

}

/**
 * set function 
 * @return void  
 * @param r index of the vector
 * @param val the value to be added to the reg
 * @note sets the index r in the registers vector with the value pass in (val)
 ********************************************************************************/
void registerfile::set(uint32_t r, int32_t val)
{
    if(r == 0)
        return;

    registers[r] = val;

}

/**
 * get function 
 * @return int32_t  
 * @param r index of the vector
 * @note gets the index r in the registers vector and returns it 
 ********************************************************************************/
int32_t registerfile::get(uint32_t r) const
{
    if(r == 0)
        return 0;

    return registers[r];

}

/**
 * Dump the register vector
 * @return void  
 * @param hdr prefix string 
 * @note dumps the register vecotr and prints out a header string if there is one
 * 
 ********************************************************************************/
void registerfile::dump(const std::string &hdr) const
{
     auto it = registers.begin();               // start of vector
        int addressLine = 0;                    // address line for registers

        while(it != registers.end())            
        {
            if(addressLine % 8 == 0)               // if 0 4 8 16 or 24
            {
                if(hdr != " ")                      // checks if hdr is null or not
                    std::cout << hdr << "  x" << addressLine << " ";
                else                                // prints either the prefix header or just the reg number
                    std::cout << " x" << addressLine << " ";

            size_t t = std::distance(registers.begin(),it);             // distance from the start of vector to it
            for(size_t x = 0; x < 8; x++)                               // loops through the 8  registers
            {
                if(x == 4)
                    std::cout << " ";

                std::cout << hex::to_hex32(get(t)) << " ";
                t++;
            }
            std::cout << std::endl;
            }

            addressLine ++;                                 // increments 
            it++;

        }
        std::cout << std::endl;

}