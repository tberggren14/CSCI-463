#include "rv32i_decode.h"

/**
 * decode function used to call different render types
 *
* @param addr is the location of the insturction in memory
* @param insn is the instruction
 * @return string
 *
 * @note goes through a switch to decode an insturction then asign to os
 ********************************************************************************/
std::string rv32i_decode::decode(uint32_t addr, uint32_t insn)
{
    uint32_t funct3 = get_funct3(insn);
    uint32_t funct7 = get_funct7(insn);

// switch goes those all possible opcodes and then has sub switchs to break down by the funct3 and then used
// funct7 as needed to break down more
switch(get_opcode(insn))
{
    default: return render_illegal_insn(insn);
    case opcode_lui: return render_lui(insn);
    case opcode_auipc: return render_auipc(insn);
    case opcode_jal: return render_jal(addr, insn);
    case opcode_jalr: return render_jalr(insn);


    case opcode_btype:
        switch (funct3)
        {
            default: return render_illegal_insn(insn);
            case funct3_beq: return render_btype(addr, insn, "beq");
            case funct3_bne: return render_btype(addr, insn, "bne");
            case funct3_blt: return render_btype(addr, insn, "blt");
            case funct3_bge: return render_btype(addr, insn, "bge");
            case funct3_bltu: return render_btype(addr, insn, "bltu");
            case funct3_bgeu: return render_btype(addr, insn, "bgeu");
        }

        assert(0 && "unrecognized funct3");  //impossible

    case opcode_load_imm:
        switch(funct3)
        {
            default: return render_illegal_insn(insn);
            case funct3_lb: return render_itype_load( insn,"lb");
            case funct3_lh: return render_itype_load( insn, "lh");
            case funct3_lw: return render_itype_load( insn, "lw");
            case funct3_lbu: return render_itype_load( insn, "lbu");
            case funct3_lhu: return render_itype_load( insn, "lhu");

        }
     assert(0 && "unrecognized funct3");  //impossible

      case opcode_stype:
        switch(funct3)
        {
            default: return render_illegal_insn(insn);
           case funct3_sb: return render_stype( insn,"sb");
           case funct3_sh: return render_stype( insn, "sh");
           case funct3_sw: return render_stype( insn, "sw");

        }
    assert(0 && "unrecognized funct3");  //impossible

    case opcode_alu_imm:
        switch (funct3)
        {
            default: return render_illegal_insn(insn);
            case funct3_add: return render_itype_alu(insn, "addi", get_imm_i(insn));
            case funct3_sll: return render_itype_alu(insn, "slli", get_imm_i(insn)%XLEN);
            case funct3_slt: return render_itype_alu(insn, "slti", get_imm_i(insn));
            case funct3_sltu: return render_itype_alu(insn, "sltiu", get_imm_i(insn));
            case funct3_xor: return render_itype_alu(insn, "xori", get_imm_i(insn));
        
            case funct3_or: return render_itype_alu(insn, "ori", get_imm_i(insn));
            case funct3_and: return render_itype_alu(insn, "andi", get_imm_i(insn));

            case funct3_srx:
                switch(funct7)
                {
                    default: return render_illegal_insn(insn);
                    case funct7_sra: return render_itype_alu(insn, "srai", get_imm_i(insn)%XLEN);
                    case funct7_srl: return render_itype_alu(insn, "srli", get_imm_i(insn)%XLEN);
                }

            assert(0 && "unrecognized funct7");  //impossible
        }

        assert(0 && "unrecognized funct3");  //impossible

        case opcode_rtype:
            switch(funct3)
            {
                default: return render_illegal_insn(insn);
                case funct3_add: 
                    switch(funct7)
                    {
                        default: return render_illegal_insn(insn);
                        case funct7_add: return render_rtype(insn,"add");
                        case funct7_sub: return render_rtype(insn,"sub");
                    }
                case funct3_sll: return render_rtype(insn,"sll");
                case funct3_slt: return  render_rtype(insn,"slt");
                case funct3_sltu: return render_rtype(insn,"sltu");
                case funct3_xor: return  render_rtype(insn,"xor");
                case funct3_or: return  render_rtype(insn,"or");
                case funct3_and: return render_rtype(insn,"and");
                case funct3_srx:
                switch(funct7)
                {
                    default: return render_illegal_insn(insn);
                    case funct7_sra: return  render_rtype(insn,"sra");
                    case funct7_srl: return  render_rtype(insn,"srl");
                }
               

                assert(0 && "unrecognized funct7");  //impossible
            }
            assert(0 && "unrecognized funct7");  //impossible

        case opcode_system:
            switch(funct3)
           {
                case 0b000:
                    switch(get_imm_i(insn))
                    {
                        case 0: return render_ecall(insn);
                        case 1: return render_ebreak(insn);
                    }
               
                case funct3_csrrw: return render_csrrx(insn, "csrrw");
                case funct3_csrrs: return render_csrrx(insn, "csrrs");
                case funct3_csrrc: return render_csrrx (insn, "csrrc");
                case funct3_csrrwi: return render_csrrxi(insn, "csrrwi");
                case funct3_csrrsi: return render_csrrxi(insn, "csrrsi");
                case funct3_csrrci: return render_csrrxi(insn, "csrrci");
                assert(0 && "unrecognized funct3");  //impossible
            }

assert(0 && "unrecognized opcode"); // It should be //impossible to ever get here!

}

}

/**
 * get the opt code
 *
* @param insn is the instruction
 * @return uint32_t
 *
 * @note returns the extracted opcode
 ********************************************************************************/
uint32_t rv32i_decode::get_opcode(uint32_t insn)
{
    return (insn & 0x0000007f);         // extracts the opcode
}

/**
 * get the register value
 *
* @param insn is the instruction
 * @return uint32_t
 *
 * @note returns the register value by masking and shifting
 ********************************************************************************/
uint32_t rv32i_decode::get_rd(uint32_t insn)
{
    return ((insn & 0x00000f00) >> 7);          // extracts the rd and extend

}


/**
 * get the funct3 value
 *
* @param insn is the instruction
 * @return uint32_t
 *
 * @note returns the funct3 value by masking and shifting
 ********************************************************************************/
uint32_t rv32i_decode::get_funct3(uint32_t insn)
{
    return ((insn & 0x00007000) >> 12);             // extracts the funct3 and extend

}

/**
 * get the register 1 value
 *
* @param insn is the instruction
 * @return uint32_t
 *
 * @note returns the register 1 value by masking and shifting
 ********************************************************************************/

uint32_t rv32i_decode::get_rs1(uint32_t insn)
{
    return ((insn & 0x000f8000) >> 15);         // extracts the rs1 and extend

}

/**
 * get the register 2 value
 *
* @param insn is the instruction
 * @return uint32_t
 *
 * @note returns the register 2 value by masking and shifting
 ********************************************************************************/
uint32_t rv32i_decode::get_rs2(uint32_t insn)
{
    return ((insn & 0x01f00000) >> 20);            // extracts rs2 and extend

}

/**
 * get the funct 7 value
 *
* @param insn is the instruction
 * @return uint32_t
 *
 * @note returns the funct7 value by masking and shifting
 ********************************************************************************/
uint32_t rv32i_decode::get_funct7(uint32_t insn)
{

    return ((insn & 0xfe000000) >> 25);         // extracts the funct7 from the insn and shifts it 
}

/**
 * get the imm i value
 *
* @param insn is the instruction
 * @return uint32_t
 *
 * @note returns the imm i value by masking and shifting
 ********************************************************************************/
int32_t rv32i_decode::get_imm_i(uint32_t insn)
{
    int32_t imm_i = (insn & 0xfff00000) >> 20; // extracts the imm_i from the insn and shifts it 

    if(imm_i & 0x800)               // if neg
    imm_i |= 0xfffff000;

        return imm_i;

}

/**
 * get the imm u value
 *
* @param insn is the instruction
 * @return uint32_t
 *
 * @note returns the imm u value by masking 
 ********************************************************************************/
int32_t rv32i_decode::get_imm_u(uint32_t insn)
{
    int32_t imm_u = insn & 0xfffff000;          // extracts the imm_u from the insn 

    return imm_u;

}

/**
 * get the imm b value
 *
* @param insn is the instruction
 * @return uint32_t
 *
 * @note returns the imm b value by masking and shifting
 ********************************************************************************/
int32_t rv32i_decode::get_imm_b(uint32_t insn)
{
    int32_t imm_b = (insn & 0x80000000) >> (31-12);      //extracts the signed bit 
    imm_b |= (insn & 0x7e000000) >> (25-5);                 // extracts the imm[12]
    imm_b |= (insn & 0x00000f00) >> (8-1);                  // extracts the imm[4;1]
    imm_b |= (insn & 0x00000080) >> (11-7);                  // extracts the imm[11]

    if(insn & 0x80000000)                               // if neg 
    imm_b |= 0xffffe000;
    
    return (imm_b & ~1);


}

/**
 * get the imm i value
 *
* @param insn is the instruction
 * @return uint32_t
 *
 * @note returns the imm i value by masking and shifting
 ********************************************************************************/
int32_t rv32i_decode::get_imm_s(uint32_t insn)
{
    int32_t imm_s = (insn & 0xfe000000) >> (25-5);

    imm_s |= (insn & 0x00000f80) >> (7-0);          // extracts the imm_s by oring insn with a mask and shifting if over 

    if(insn & 0x80000000)                           // if neg 
    imm_s |= 0xfffff000;   // sign-extend the left

    return imm_s;

}

/**
 * get the imm j value
 *
* @param insn is the instruction
 * @return uint32_t
 *
 * @note returns the imm j value by masking and shifting
 ********************************************************************************/
int32_t rv32i_decode::get_imm_j(uint32_t insn)
{
    int32_t imm_j = (insn & 0x80000000) >> (31-20); // get the first half of the imm_j
    imm_j |= (insn & 0x7fe00000) >> (25-5);
    imm_j |= (insn & 0x00100000) >> (8-1);          // get the second half of the imm_j
    imm_j |= (insn & 0x000ff000);

    if(insn & 0x80000000)                           // if neg 
    imm_j |= 0xffe00000;

    return imm_j;


}


/**
 * prints a error
 *
* @param insn is the instruction
 * @return uint32_t
 *
 * @note returns the error reason for illgeal instruction
 ********************************************************************************/
std::string rv32i_decode::render_illegal_insn(uint32_t insn)
{
    return "ERROR: UNIMPLEMENTED INSTRUCTION";
}

/**
 * renders a lui print format
 *
* @param insn is the instruction
 * @return string value to print to os
 *
 * @note renders a lui instruction to print by shifting and masking
 ********************************************************************************/
std::string rv32i_decode::render_lui(uint32_t insn)
{
    uint32_t rd = get_rd(insn);
    int32_t imm_u = get_imm_u(insn);
    std::ostringstream os;
                                // formats output with a mask and shift
    os << render_mnemonic("lui") << render_reg(rd) << ","
       << to_hex0x20((imm_u >> 12)&0x0fffff);

    return os.str();

}

/**
 * renders a auipc print format
 *
* @param insn is the instruction
 * @return string value to print to os
 *
 * @note renders a auipc instruction to print by shifting and masking
 ********************************************************************************/
std::string rv32i_decode::render_auipc(uint32_t insn)
{
    uint32_t rd = get_rd(insn);
    int32_t imm_u = get_imm_u(insn);
    std::ostringstream os;
                                // formats output with a mask and shift
    os << render_mnemonic("auipc") << render_reg(rd) << ","
       << to_hex0x20((imm_u >> 12)&0x0fffff);

    return os.str();

}


/**
 * renders a jal print format
 *
* @param insn is the instruction
* @param addr is the location of the instuction in memory
 * @return string value to print to os
 *
 * @note renders a jal instruction to print to standard output
 ********************************************************************************/
std::string rv32i_decode::render_jal(uint32_t addr, uint32_t insn)
{
    uint32_t rd = get_rd(insn);
    int32_t imm_j = get_imm_j(insn);
    std::ostringstream os;
                                    // formats output
    os << render_mnemonic("jal") << render_reg(rd) << ","
       << to_hex0x32((imm_j));

    return os.str();


}

/**
 * renders a jalr print format
 *
* @param insn is the instruction
 * @return string value to print to os
 *
 * @note renders a jalr instruction to print to standard output
 ********************************************************************************/
std::string rv32i_decode::render_jalr(uint32_t insn)
{
    uint32_t rd = get_rd(insn);
    int32_t imm_i = get_imm_i(insn);
    uint32_t rs1 = get_rs1(insn);
    std::ostringstream os;
                                    // formats output
    os << render_mnemonic("jalr") << render_reg(rd) << ","
       << render_base_disp(rs1,imm_i); // reg defaults to 0x1

    return os.str();

}


/**
 * renders a BYTPE instruction to  print format
 *
* @param insn is the instruction
* @param addr is the location of the instuction in memory
* @param mnemonic is the mnemonic of the instruction
 * @return string value to print to os
 *
 * @note renders a btype instruction to print to standard output by if it is signed or unsigned
 ********************************************************************************/
std::string rv32i_decode::render_btype(uint32_t addr, uint32_t insn, const char *mnemonic)
{
    std::ostringstream os;
    uint32_t funct3 = get_funct3(insn);
    int32_t imm_b = get_imm_b(insn);
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);

   if(funct3 == funct3_bltu ||funct3 == funct3_bgeu )
    {
        uint32_t uImm_b = (uint32_t) imm_b;
        os << render_mnemonic(mnemonic) << render_reg(rs1) << ","
     << render_reg(rs2) << "," << to_hex0x32(uImm_b);
    return os.str();

    }

        // formats output
    os << render_mnemonic(mnemonic) << render_reg(rs1) << ","
     << render_reg(rs2) << "," << to_hex0x32(imm_b);


    return os.str();
}

/**
 * renders a itype load print format
 *
* @param insn is the instruction
* @param mnemonic is the mnemonic of the instruction
 * @return string value to print to os
 *
 * @note renders a itype load instruction to print to standard output
 ********************************************************************************/
std::string rv32i_decode::render_itype_load(uint32_t insn, const char *mnemonic)
{
    std::ostringstream os;
    uint32_t rd = get_rd(insn);
    int32_t imm_i = get_imm_i(insn);
    uint32_t rs1 = get_rs1(insn);

        // formatting output
     os << render_mnemonic(mnemonic) << render_reg(rd) << ","
     << render_base_disp(imm_i,rs1);

     return os.str();
}

/**
 * renders a stype print format
 *
* @param insn is the instruction
* @param mnemonic is the mnemonic of the instruction
 * @return string value to print to os
 *
 * @note renders a stype instruction to print to standard output
 ********************************************************************************/
std::string rv32i_decode::render_stype(uint32_t insn, const char *mnemonic)
{
    std::ostringstream os;
    int32_t imm_s = get_imm_s(insn);
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);



    // formatting output
    os << render_mnemonic(mnemonic) << render_reg(rs2) 
     << "," << render_base_disp(imm_s,rs1);    


    return os.str();

}

/**
 * renders a itype alu print format
 *
* @param insn is the instruction
* @param mnemonic is the mnemonic of the instruction
* @param addr is the location of the instuction in memory
 * @return string value to print to os
 *
 * @note renders a itype alu instruction to print to standard output
 ********************************************************************************/
std::string rv32i_decode::render_itype_alu(uint32_t insn, const char *mnemonic, int32_t imm_i)
{
     std::ostringstream os;
    uint32_t rd = get_rd(insn);
    int32_t rs1 = get_rs1(insn);
    
        // formatting output
    os << render_mnemonic(mnemonic) << render_reg(rd) << ","
     << render_reg(rs1) << "," << std::dec << (imm_i);


    return os.str();

}

/**
 * renders a itype load print format
 *
* @param insn is the instruction
* @param mnemonic is the mnemonic of the instruction
 * @return string value to print to os
 *
 * @note renders a itype load instruction to print to standard output
 ********************************************************************************/
std::string rv32i_decode::render_rtype(uint32_t insn, const char *mnemonic)
{
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);

    std::ostringstream os;

    
        // formatting output
     os << render_mnemonic(mnemonic) << render_reg(rd) << ","
     << render_reg(rs1) << "," << render_reg(rs2);

    return os.str();


}

/**
 * renders a ecall to print format
 *
* @param insn is the instruction
 * @return string value to print to os
 *
 * @note renders ecall to return to print to os 
 ********************************************************************************/
std::string rv32i_decode::render_ecall(uint32_t insn)
{
   return "ecall";

}

/**
 * renders a ebreak to print format
 *
* @param insn is the instruction
 * @return string value to print to os
 *
 * @note renders ebreak to return to print to os 
 ********************************************************************************/
std::string rv32i_decode::render_ebreak(uint32_t insn)
{
    return "ebreak";

}

/**
 * renders a cssx type print format
 *
* @param insn is the instruction
* @param mnemonic is the mnemonic of the instruction
 * @return string value to print to os
 *
 * @note renders a cssrx instruction to print to standard output
 ********************************************************************************/
std::string rv32i_decode::render_csrrx(uint32_t insn, const char *mnemonic)
{
    std::ostringstream os;
    uint32_t rd = get_rd(insn);
    int32_t imm_i = (insn & 0xfff00000);
    uint32_t rs1 = get_rs1(insn);

            // formatting the output 
   os << render_mnemonic(mnemonic) << render_reg(rd) << ","
     << hex::to_hex0x12(imm_i >> 20) << "," << render_reg(rs1);


    return os.str();

}

/**
 * renders a cssxi type print format
 *
* @param insn is the instruction
* @param mnemonic is the mnemonic of the instruction
 * @return string value to print to os
 *
 * @note renders a cssrxi instruction to print to standard output
 ********************************************************************************/
std::string rv32i_decode::render_csrrxi(uint32_t insn, const char *mnemonic)
{
    std::ostringstream os;
    uint32_t rd = get_rd(insn);
    int32_t imm_i = (insn & 0xfff00000);        // get imm_i with out shifting
    uint32_t rs1 = get_rs1(insn);

    
    // formats output
   os << render_mnemonic(mnemonic) << render_reg(rd) << ","
     << hex::to_hex0x12(imm_i >> 20) << "," << std::dec << rs1;


    return os.str();


}

/**
 * renders a register print format
 *
* @param r is the register
 * @return string value to print to os
 *
 * @note renders a register value to print to standard output
 ********************************************************************************/
std::string rv32i_decode::render_reg(int r)
{
    std::ostringstream os;      // prints the reg r in decicaml with a x
    os << "x" << std::dec << r;
    return os.str();

}

/**
 * renders a register print format
 *
* @param reg is the register
* @param imm is the displacement
 * @return string value to print to os
 *
 * @note renders a register value as a base and dispalcement to print to standard output
 ********************************************************************************/
std::string rv32i_decode::render_base_disp(uint32_t reg, int32_t imm)
{
    std::ostringstream os;          // gives a base disp with the imm first than the reg in ()
    os << std::dec << imm <<"(" << render_reg(reg) << ")";
    return os.str();

}

/**
 * renders a mnemonic print format
 *
* @param r is the register
 * @return string value to print to os
 *
 * @note renders a mnemonic value to print to standard output with a spacing
 ********************************************************************************/
std::string rv32i_decode::render_mnemonic(const std::string &m)
{
    std::ostringstream os;              // sets the mnemonic width to all be the same
    os << std::left << std::setw(mnemonic_width) << m;
    return os.str();

}
