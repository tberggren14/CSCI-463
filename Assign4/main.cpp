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
#include "rv32i_decode.h"

using std::cout;
using std::cin;
using std::endl;
using std::cerr;

/**
 * Usage function used to print useful information
 *
 * @return void
 *
 ********************************************************************************/
static void usage()
{
	cerr << "Usage: rv32i [-m hex-mem-size] infile" << endl;
	cerr << "    -m specify memory size (default = 0x100)" << endl;
	exit(1);
}

/**
 * Usage function used to print useful information
 *
 * @return void
 *
 ********************************************************************************/
static void disassemble(const memory &mem)
{
    uint32_t pc = 0;
    while(!(pc == mem.get_size()))          // loop to go over every bit 
    {
        cout << hex::to_hex32(pc) << ": " << hex::to_hex32(mem.get32(pc))
        << " " << rv32i_decode::decode(pc,mem.get32(pc)) << endl;
        pc += 4;                                                    // calls decode to decode an instruction

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
    int opt;
    while ((opt = getopt(argc, argv, "m:")) != -1)
    {
        switch (opt)
        {
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

    disassemble(mem);
    mem.dump();

return 0; 

}
