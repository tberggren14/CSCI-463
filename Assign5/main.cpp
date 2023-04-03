//***************************************************************************
//
//  Trevor Berggren
//  Z1906236
//  CSCI 463 
//
//  I certify that this is my own work and where appropriate an extension 
//  of the starter code provided for the assignment.
//
//***************************************************************************

#include <sstream>
#include <iostream>
#include <getopt.h>
#include "memory.h"
#include "rv32i_hart.h"
#include "cpu_single_hart.h"

/**
 * Usage function used to print useful information
 *
 * @return void
 *
 * @note Shows how to use the program flags
 ********************************************************************************/
static void usage()
{
	std::cerr << "Usage : rv32i [-d ] [ -i] [-r] [- z] [-l exec - limit ] "
        <<  "[-m hex - mem - size ] infile" << std::endl;
	std::cerr << "    -d show disassembly before program execution" << std::endl;
    std::cerr << "    -i show instruction printing during execution" << std::endl;
    std::cerr << "    -l maximum number of instructions to exec" << std::endl;
    std::cerr << "    -m specify memory size (default = 0x100)" << std::endl;
    std::cerr << "    -r show register printing during execution" << std::endl;
    std::cerr << "    -z show a dump of the regs & memory after simulation" << std::endl;
	exit(1);
}

/**
 * disaaemble function used to print instruction information
 *
 * @return void
 *
 * @note loops through mem and calls decode to print the instructions
 ********************************************************************************/
static void disassemble(const memory &mem)
{
    uint32_t i = 0;
    while(i != mem.get_size())          // loop to go over every bit 
    {
        std::cout << hex::to_hex32(i) << ": " << hex::to_hex32(mem.get32(i))
        << " " << rv32i_decode::decode(i,mem.get32(i)) << std::endl;
        i += 4;                                                    // calls decode to decode an instruction

    }


}

/**
 * Main function used to call printBinFloat to decode a hex number to IEEE 754 number
 *
* @param argc an int for the argument count
* @param argv a char for the argument values
 * @return int
 *
 ********************************************************************************/

int main(int argc, char **argv)
{
    uint32_t memory_limit = 0x100;
    uint64_t exec_limit = 0;
    bool show_disassembly = false;
    bool show_instructions = false;
    bool show_registers = false;
    bool show_post_dump = false;
    int opt;
    while ((opt = getopt(argc, argv, "dirzl:m:")) != -1)
    {
        switch (opt)
        {
            case 'd':
            {
               show_disassembly = true;
            }
            break;
            case 'i':
            {
                show_instructions = true;
            }
            break;
            case 'r':
            {
              show_registers = true;   
            }
            break;
            case 'z':
            {
               show_post_dump = true;
            }
            break;
            case 'l':
            {
               std::istringstream iss(optarg);
            	iss >> std::dec >> exec_limit;
            }
            break;
            case 'm':
        	{
        		std::istringstream iss(optarg);
            	iss >> std::hex >> memory_limit;
			}
            break;
            default: /* ’?’ */
			usage(); 
		}
	}
    if (optind >= argc)
    	usage();    // missing filename

    memory mem(memory_limit);

    if (!mem.load_file(argv[optind]))
        usage();

    if(show_disassembly)
        disassemble(mem);

    cpu_single_hart cpu(mem);
    cpu.reset();

    cpu.set_show_instructions(show_instructions);
    cpu.set_show_registers(show_registers);

    cpu.run(exec_limit);

    if(show_post_dump)
    {
        cpu.dump();
        mem.dump();
    }

return 0; 

}
