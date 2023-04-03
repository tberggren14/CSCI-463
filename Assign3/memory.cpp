#include "memory.h"
#include "hex.h"
#include <string>
#include <fstream>

/**
 * Allocate siz bytes in the mem vector and initialize every byte/element to 0xa5.
 *
 *
 * @param siz a 32 bit unsigned hex number used for the mem vector
 * 
 *
 * @note constructor
 ********************************************************************************/
    memory::memory(uint32_t siz)
    {
        siz = (siz+15)&0xfffffff0;  // round up 

        mem.resize(siz,0xa5);       // resize and store a default value 
    }

   /**
 * Destructor 
 *
 * 
 *
 * @note cleans up mem vector 
 ********************************************************************************/
    memory::~memory()
    {
        mem.erase(mem.begin(),mem.end());   // calls erase to clear vector

    }

/**
* Return true if the given address i does not represent an element that is present in the mem
 *   vector. 
 *
 * @param i a 32 bit unsigned hex number used for address in vector
 * 
 * @return bool 
 *
 * @note returns true if it is outside of vector 
 ********************************************************************************/
    bool memory::check_illegal(uint32_t i) const
    {

            if(i >= mem.size())             // checks if addy is bigger than the size
            {
            std::cout << "WARNING: Address out of range: " << hex::to_hex0x32(i) << std::endl;
            return true;
            }
            
        return false;

    }

/**
 * Returns the size of the vector 
 *
 * @return int as the size of the vector
 *
 ********************************************************************************/
    uint32_t memory::get_size() const
    {
        return mem.size();

    }


/**
 * Check to see if the given addr is in your mem by calling check_illegal(). 
 *   If addr is in the valid range then return the value of the byte from your simulated memory at the given address. 
 *
 * @param addra 32 bit unsigned hex number used for address in vector
 * @return uint8_t 
 *
 * @note assigns in the mem vector  
 ********************************************************************************/
    uint8_t memory::get8(uint32_t addr) const
    {

        if(check_illegal(addr))
        {
            return 0;

        }
         return mem[addr];
    }


/**
 * This function must call your get8() function twice to get two bytes and then combine them in little-endian1 order 
    to create a 16-bit return value.
*  
 * @param addra 32 bit unsigned hex number used for address in vector
 * @return uint16_t 
 *
 * @note stores a 16 bit value
 ********************************************************************************/
    uint16_t memory::get16(uint32_t addr) const
    {
        uint16_t temp = ((uint16_t)get8(addr)) | ((uint16_t)get8(addr+1) << 8); // casts to a uint16_t and shift 8  bits

         return temp;
    }

    /**
 * This function must call your get16() function twice to get two bytes and then combine them in little-endian1 order 
    to create a 32-bit return value.
*  
 * @param addra 32 bit unsigned hex number used for address in vector
 * @return uint32_t 
 *
 * @note stores a 32 bit value 
 ********************************************************************************/
    uint32_t memory::get32(uint32_t addr) const
    {                 
        uint32_t temp = ((uint32_t)get16(addr)) | ((uint32_t)get16(addr + 2) << 16) ; // casts to a uint16_t and shift 16 bits
        return temp;
    }


    /**
 * This function will call get8() and then return the sign-extended value of the byte as a 32-bit
    signed integer.
 *  
 * @param addra 32 bit unsigned hex number used for address in vector
 * @return int32_t 
 *
 * @note returns a signed extended value
 ********************************************************************************/
    int32_t memory::get8_sx(uint32_t addr) const
    {
        int32_t temp = get8(addr) ;     // cast to sign extended
       // temp >>= 31;
         return temp;
    }

   /**
 * This function will call get16() and then return the sign-extended value of the byte as a 32-bit
    signed integer.
 *  
 * @param addra 32 bit unsigned hex number used for address in vector
 * @return int32_t 
 *
 * @note returns a signed extended value
 ********************************************************************************/
    int32_t memory::get16_sx(uint32_t addr) const
    {
         int32_t temp = get16(addr);        // cast to sign extended
         //temp >>= 24;
         return temp;
    }

    /**
 * This function will call get32() and then return the sign-extended value of the byte as a 32-bit
    signed integer.
 *  
 * @param addra 32 bit unsigned hex number used for address in vector
 * @return int32_t 
 *
 * @note returns a signed extended value
 ********************************************************************************/
    int32_t memory::get32_sx(uint32_t addr) const
    {
         return get32(addr);        
    }

/**
 * This function will call check_illegal() to verify the addr argument is valid. If addr is valid then 
    set the byte in the simulated memory at that address to the given val.
 *  
 * @param addra 32 bit unsigned hex number used for address in vector
 * @param val 32 bit unsigned hex number used for values
 * @return void  
 *
 * @note stores value in memory at addr
 ********************************************************************************/
    void memory::set8(uint32_t addr, uint8_t val)
    {
        if(check_illegal(addr))
        {
            return;
        }

        mem[addr] = val;
    }

  /**
 * This function will call set8() twice to store the given val in little-endian 
 *  
 * @param addra 32 bit unsigned hex number used for address in vector
 * @param val 32 bit unsigned hex number used for values
 * @return int32_t 
 *
 * @note stores 16 bits
 ********************************************************************************/
    void memory::set16(uint32_t addr, uint16_t val)
    {
        
        set8(addr,(uint8_t)(val));
        set8(addr+1,((val)>>8));
    }

    /**
 * This function will call set16() twice to store the given val in little-endian 
 *  
 * @param addra 32 bit unsigned hex number used for address in vector
 * @param val 32 bit unsigned hex number used for values
 * @return int32_t 
 *
 * @note stores 32 bits
 ********************************************************************************/
    void memory::set32(uint32_t addr, uint32_t val)
    {
      
        set16(addr,(uint16_t)(val));
        set16(addr+2,((val) >> 16));
    }

    /**
 * Dump the entire contents of your simulated memory in hex with the corresponding ASCII2 characters
 * @return void  
 *
 * @note prints the memory out
 ********************************************************************************/
    void memory::dump() const
    {
        auto it = mem.begin();
        int addressLine = 0;

        while(it != mem.end())
        {
            if(addressLine % 16 == 0)
            {
            std::cout << hex::to_hex32(addressLine) << ": ";

            size_t t = std::distance(mem.begin(),it); 
            for(size_t x = 0; x < 16; x++)
            {
                if(x == 8)
                    std::cout << " ";

                std::cout << hex::to_hex8(get8(t)) << " ";
                t++;
            }

            std::cout << "*";
            size_t i = std::distance(mem.begin(),it); 
            for(size_t x = 0; x < 16; x++)
            {
                uint8_t ch = get8(i);
                ch = isprint(ch) ? ch : '.';
                std::cout << ch;
                i++;
            }
            std::cout << "*";
            std::cout << std::endl;
            }

            addressLine ++;
            it++;

        }
        
       

    }

/**
 * Open the file named fname in binary mode and read its contents into your simulated memory.
 *  
 * @param string a file name
 * @return bool
 *
 * @note laods the file and stores it in memory
 ********************************************************************************/
    bool memory::load_file(const std::string &fname)
    {
        std::ifstream infile(fname, std::ios::in|std::ios::binary);
        
        if(!infile.is_open())
        {
            std::cerr << "Can't open file " <<  fname << " for reading.";
            return false;

        }


        uint8_t i;
         infile >> std::noskipws;
         
         for (uint32_t addr = 0; infile >> i; ++addr)
        {
            if(check_illegal(addr))
            {
                std::cerr << "Program too big." << std::endl;
                infile.close();
                return false;

            }

            set8(addr,i);

        }
         infile.close();
         return true;
        
    }
