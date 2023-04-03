#ifndef MEMORY_H
#define MEMORY_H

#include <vector>
#include <iostream>
#include "hex.h"

class memory : public hex
{
public:
    memory(uint32_t siz);
    ~memory();

    bool check_illegal(uint32_t ) const;
    uint32_t get_size() const;
    uint8_t get8(uint32_t ) const;
    uint16_t get16(uint32_t ) const;
    uint32_t get32(uint32_t ) const;

    int32_t get8_sx(uint32_t ) const;
    int32_t get16_sx(uint32_t ) const;
    int32_t get32_sx(uint32_t ) const;

    void set8(uint32_t , uint8_t ); 
    void set16(uint32_t , uint16_t ); 
    void set32(uint32_t , uint32_t );

    void dump() const;
    bool load_file(const std::string &);

private:
        std::vector<uint8_t> mem;
 };

#endif