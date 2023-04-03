#include "cpu_single_hart.h"

/**
 * The one cpu hart that run the program on the cpu
 * @return void  
 *
 * @note this is where the program starts. calls tick to run everything and prints halt
 ********************************************************************************/
void cpu_single_hart::run(uint64_t exec_limit)
{

   regs.set(2,mem.get_size());      // sets the size of the program

   uint64_t i = 0;            // counter
   //while not halted and exec_limit is 0 or i is less than limit
     while(!is_halted() && ( exec_limit == 0 || ++i <= exec_limit))
         if(!is_halted())
            tick();

   if(is_halted())            // prints if halted 
      std::cout << get_halt_reason() << " ";
                                                   // prints the instruction counter
   std::cout << get_insn_counter() << " instructions executed" << std::endl;
   

}