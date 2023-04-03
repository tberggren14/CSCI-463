#ifndef CPU_SINGLE_HART_H
#define CPU_SINGLE_HART_H
#include "rv32i_hart.h"

class cpu_single_hart : public rv32i_hart
{
    public:
        cpu_single_hart(memory &mem) : rv32i_hart(mem){}
        void run(uint64_t);

};

#endif