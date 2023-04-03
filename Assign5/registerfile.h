#ifndef REGISTERFILE_H
#define REGISTERFILE_H
#include <vector>
#include <iostream>
#include "hex.h"

class registerfile
{
    public:
        registerfile();
        void reset();
        void set(uint32_t r, int32_t val);
        int32_t get(uint32_t r) const;
        void dump(const std::string &hdr) const;

    private: 
        std::vector<int32_t> registers;
};

#endif