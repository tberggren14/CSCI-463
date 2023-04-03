#include "rv32i_decode.h"

/**
 * decode function used to call different render types
 *
* @param addr 
* @param insn 
 * @return string
 *
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
* @param insn 
 * @return uint32_t
 *
 ********************************************************************************/
uint32_t rv32i_decode::get_opcode(uint32_t insn)
{
    return (insn & 0x0000007f);         // extracts the opcode
}

/**
 * get the rd 
 *
* @param insn 
 * @return uint32_t
 *
 ********************************************************************************/
uint32_t rv32i_decode::get_rd(uint32_t insn)
{
    return ((insn & 0x00000f00) >> 7);          // extracts the rd and extend

}


/**
 * get the funct3 code
 *
* @param insn 
 * @return uint32_t
 *
 ********************************************************************************/
uint32_t rv32i_decode::get_funct3(uint32_t insn)
{
    return ((insn & 0x00007000) >> 12);             // extracts the funct3 and extend

}

/**
 * get the rs1 code
 *
* @param insn 
 * @return uint32_t
 *
 ********************************************************************************/

uint32_t rv32i_decode::get_rs1(uint32_t insn)
{
    return ((insn & 0x000f8000) >> 15);         // extracts the rs1 and extend

}

/**
 * get the rs2 code
 *
* @param insn 
 * @return uint32_t
 *
 ********************************************************************************/
uint32_t rv32i_decode::get_rs2(uint32_t insn)
{
    return ((insn & 0x01f00000) >> 20);            // extracts rs2 and extend

}

/**
 * get the funct code
 *
* @param insn 
 * @return uint32_t
 *
 ********************************************************************************/
uint32_t rv32i_decode::get_funct7(uint32_t insn)
{

    return ((insn & 0xfe000000) >> 25);         // extracts the funct7 from the insn and shifts it 
}

/**
 * get the imm_i code
 *
* @param insn 
 * @return int32_t
 *
 ********************************************************************************/
int32_t rv32i_decode::get_imm_i(uint32_t insn)
{
    int32_t imm_i = (insn & 0xfff00000) >> 20; // extracts the imm_i from the insn and shifts it 

    if(imm_i & 0x800)               // if neg
    imm_i |= 0xfffff000;

        return imm_i;

}

/**
 * get the imm_u code
 *
* @param insn 
 * @return int32_t
 *
 ********************************************************************************/
int32_t rv32i_decode::get_imm_u(uint32_t insn)
{
    int32_t imm_u = insn & 0xfffff000;          // extracts the imm_u from the insn 

    return imm_u;

}

/**
 * get the imm_b code
 *
* @param insn 
 * @return int32_t
 *
 ********************************************************************************/
int32_t rv32i_decode::get_imm_b(uint32_t insn)
{
    int32_t imm_b = (insn & 0x80000000) >> (31-12);      //extracts the signed bit 
    imm_b |= (insn & 0x7e000000) >> (25-5);                 // extracts the imm[12]
    imm_b |= (insn & 0x00000f00) >> (8-1);                  // extracts the imm[4;1]
    imm_b |= (insn & 0x00000080) >> (11-7);                  // extracts the imm[11]

    if(insn & 0x80000000)                               // if neg 
    imm_b |= 0xffffe000;
    
    return imm_b;


}

/**
 * get the imm_s code
 *
* @param insn 
 * @return int32_t
 *
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
 * get the imm_j code
 *
* @param insn 
 * @return int32_t
 *
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
 * prints a ERROR
 *
* @param insn 
 * @return string
 *
 ********************************************************************************/
std::string rv32i_decode::render_illegal_insn(uint32_t insn)
{
    return "ERROR: UNIMPLEMENTED INSTRUCTION";
}

/**
 * prints a render lui 
 *
* @param insn 
 * @return string
 *
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
 * prints a render auipc  
 *
* @param insn 
 * @return string
 *
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
 * prints a render jal 
 *
* @param addr 
* @param insn 
 * @return string
 *
 ********************************************************************************/
///@parm addr The memory address where the insn is stored.
std::string rv32i_decode::render_jal(uint32_t addr, uint32_t insn)
{
    uint32_t pc = get_rd(insn);
    int32_t imm_j = get_imm_j(insn);
    std::ostringstream os;
                                    // formats output
    os << render_mnemonic("jal") << render_reg(0x1) << ","
       << to_hex0x32((imm_j + addr + pc ));

    return os.str();


}

/**
 * prints a render jalr 
 *
* @param insn 
 * @return string
 *
 ********************************************************************************/
std::string rv32i_decode::render_jalr(uint32_t insn)
{
    uint32_t rd = get_rd(insn);
    uint32_t pc = rd;
    int32_t imm_i = get_imm_i(insn);
    uint32_t rs1 = get_rs1(insn);
    pc = ((rs1+imm_i)&~1);
    std::ostringstream os;
                                    // formats output
    os << render_mnemonic("jalr") << render_reg(rd) << ","
       << render_base_disp(0x1,pc); // reg defaults to 0x1

    return os.str();

}


/**
 * prints a render b types 
 *
* @param addr 
* @param insn 
* @param mnemonic 
 * @return string
 *
 ********************************************************************************/
///@parm addr The memory address where the insn is stored.
std::string rv32i_decode::render_btype(uint32_t addr, uint32_t insn, const char *mnemonic)
{
    std::ostringstream os;
    uint32_t rd = get_rd(insn);
    uint32_t funct3 = get_funct3(insn);
    uint32_t pc = rd;
    int32_t imm_b = get_imm_b(insn);
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);

    switch(funct3)
    {
        case funct3_beq:        // equal
        pc = addr + ((rs1==rs2) ? imm_b : 4);
        break; 

        case funct3_bge:    //greater than or equal
        pc = addr + ((rs1>=rs2) ? imm_b : 4);
        break;

        case funct3_bgeu:       // unsigned greater than or equal
        pc = addr + ((rs1>=rs2) ? imm_b : 4);
        break;

        case funct3_blt:        // less than
        pc = addr + ((rs1<rs2) ? imm_b : 4);
        break;

        case funct3_bltu:   // less than unsigned
        pc = addr + ((rs1<rs2) ? imm_b : 4);
        break;

        case funct3_bne:    // not equal 
        pc = addr + ((rs1!=rs2) ? imm_b : 4);
        break;
    }
        // formats output
    os << render_mnemonic(mnemonic) << render_reg(rs1) << ","
     << render_reg(rs2) << "," << to_hex0x32(pc);


    return os.str();
}

/**
 * prints a render i types 
 *
* @param insn 
* @param mnemonic 
 * @return string
 *
 ********************************************************************************/
std::string rv32i_decode::render_itype_load(uint32_t insn, const char *mnemonic)
{
    std::ostringstream os;
    uint32_t rd = get_rd(insn);
    uint32_t funct3 = get_funct3(insn);
    uint32_t pc = rd;
    int32_t imm_i = get_imm_i(insn);
    uint32_t rs1 = get_rs1(insn);

    switch(funct3)
    {
        case funct3_lb:// cast to int32_t
        rd = (int32_t) rs1 + imm_i;
        break; 

        case funct3_lh: // cast to int32_t
         rd = (int32_t) rs1 + imm_i;
        break;

        case funct3_lw: // cast to int32_t
         rd = (int32_t) rs1 + imm_i;
        break;

        case funct3_lbu: // cast to int32_t
         rd = (int32_t) rs1 + imm_i;
        break;

        case funct3_lhu: // cast to int32_t
        rd = (int32_t) rs1 + imm_i;
        break;
    }
        // formatting output
     os << render_mnemonic(mnemonic) << render_reg(pc) << ","
     << render_base_disp(0x0,rd);

     return os.str();
}

/**
 * prints a render s types 
 *
* @param insn 
* @param mnemonic 
 * @return string
 *
 ********************************************************************************/
std::string rv32i_decode::render_stype(uint32_t insn, const char *mnemonic)
{
    std::ostringstream os;
    uint32_t rd = get_rd(insn);
    uint32_t funct3 = get_funct3(insn);
    int32_t pc = rd;
    int32_t imm_s = get_imm_s(insn);
    uint32_t rs1 = get_rs1(insn);
   // uint32_t rs2 = get_rs2(insn);

    switch(funct3)
    {
        case funct3_sb:
        pc = (int32_t) rs1 + imm_s;     // cast to int32_t
        break; 

        case funct3_sh: 
         pc = (int32_t) rs1 + imm_s;    // cast to int32_t
        break;

        case funct3_sw: 
        pc =  (int32_t) rs1 + imm_s;    // cast to int32_t
        break;
    }
    // formatting output
    os << render_mnemonic(mnemonic) << render_reg(0x4) 
     << "," << render_base_disp(0x0,pc);    


    return os.str();

}

/**
 * prints a render i type alu
 *
* @param insn 
* @param mnemonic 
* @param imm_i 
 * @return string
 *
 ********************************************************************************/
std::string rv32i_decode::render_itype_alu(uint32_t insn, const char *mnemonic, int32_t imm_i)
{
     std::ostringstream os;
    uint32_t rd = get_rd(insn);
    uint32_t funct3 = get_funct3(insn);
    uint32_t funct7 = get_funct7(insn);
    int32_t rs1 = get_rs1(insn);
    uint32_t pc = rd;
    switch(funct3)
    {
        case funct3_add:                    //add
        rd = (rs1 + imm_i);
        break; 

        case funct3_slt: 
        rd = ((rs1 < imm_i) ? 1 : 0);       // if rs1 is less than rs2 print 1 or 0
        break;

        case funct3_sltu: 
        rd = ((rs1 < imm_i) ? 1 : 0);       // if rs1 is less than rs2 print 1 or 0
        break;

        case funct3_xor:
        rd = (rs1 ^ imm_i );            //xor
        break; 

        case funct3_or: 
         rd = (rs1 |imm_i);         // reg = rs1 or imm_i
        break;

        case funct3_and: 
        rd = (rs1 & imm_i);         // reg = rs1 & imm_i
        break;

        case funct3_sll:
        rd = (rs1 << imm_i);        // shift
        break; 

        case funct3_srx: 
         switch(funct7)
         {
            case funct7_srl:        // shifting
            rd = (rs1 << imm_i);
            break;

            case funct7_sub:        // shift
            rd = (rs1 >> imm_i);
            break;
         }
    }
        // formatting output
    os << render_mnemonic(mnemonic) << render_reg(pc) << ","
     << render_reg(pc+1) << "," << std::dec << (imm_i);


    return os.str();

}

/**
 * prints a render r type 
 *
* @param insn 
* @param mnemonic 
 * @return string
 *
 ********************************************************************************/
std::string rv32i_decode::render_rtype(uint32_t insn, const char *mnemonic)
{
    uint32_t rd = get_rd(insn);
    uint32_t pc = rd;
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);
    uint32_t funct3 = get_funct3(insn);
    uint32_t funct7 = get_funct7(insn);

    std::ostringstream os;

    switch(funct3)
    {
        case funct3_add: 
            switch(funct7)
            {
                case funct7_add:        //add
                rd = rs1 + rs2;
                
                break;
                case funct7_sub:    //sub
                rd = rs1 - rs2;
                
                break;
            }
        break;

        case funct3_and:            //and 
            rd = rs1 & rs2;
            
        break;

        case funct3_or :            // or
            rd = rs1 | rs2;
            
        
        break;

        case funct3_sll:                // shift
            rd = (rs1 << (rs2 % XLEN));
             
        
        break;

        case funct3_slt:                // if rs1 is less than rs2 print 1 or 0
            rd = ((rs1<rs2)? 1: 0);
            
        
        break;

        case funct3_sltu:               // if rs1 is less than rs2 print 1 or 0 unsigned
            rd = ((rs1<rs2)? 1: 0);
            
        
        break;

        case funct3_srx: 
            switch(funct7)
            {
                case funct7_srl:            // shifting
                rd = (rs1 >> (rs2 % XLEN));
                break;

                case funct7_sra:            // shifting rs2 mod xlen bits 
                rd = (rs1 >> (rs2 % XLEN));
                break;
            }
       
        break;

        case funct3_xor: // xor
            rd = rs1 ^ rs2;
            
        
        break;

    }
        // formatting output
     os << render_mnemonic(mnemonic) << render_reg(pc) << ","
     << render_reg(rs1) << "," << render_reg(rs2);

    return os.str();


}

/**
 * prints a render ecall 
 *
* @param insn 
 * @return string
 *
 ********************************************************************************/
std::string rv32i_decode::render_ecall(uint32_t insn)
{
   return "ecall";

}

/**
 * prints a render ebreak 
 *
* @param insn 
 * @return string
 *
 ********************************************************************************/
std::string rv32i_decode::render_ebreak(uint32_t insn)
{
    return "ebreak";

}

/**
 * prints a render csr 
 *
* @param insn 
* @param mnemonic 
 * @return string
 *
 ********************************************************************************/
std::string rv32i_decode::render_csrrx(uint32_t insn, const char *mnemonic)
{
    std::ostringstream os;
    uint32_t rd = get_rd(insn);
    uint32_t funct3 = get_funct3(insn);
    uint32_t pc = rd;
    int32_t imm_i = (insn & 0xfff00000);
    uint32_t rs1 = get_rs1(insn);
   // imm_i <<= 20;

    

    switch(funct3)
    {
        case funct3_csrrw:
         pc >>= (imm_i % XLEN);     /// shifting mod bits
        break; 

        case funct3_csrrs: 
         pc = rs1 - imm_i;      //sub
         //pc >>= 20;
        break;

        case funct3_csrrc: 
         pc = rs1 + imm_i;      // add 
       // pc >>= 20;
        break;
    }
            // formatting the output 
   os << render_mnemonic(mnemonic) << render_reg(rd) << ","
     << hex::to_hex0x12(imm_i >> 20) << "," << render_reg(rs1);


    return os.str();

}

/**
 * prints a render csr 
 *
* @param insn 
* @param mnemonic 
 * @return string
 *
 ********************************************************************************/
std::string rv32i_decode::render_csrrxi(uint32_t insn, const char *mnemonic)
{
    std::ostringstream os;
    uint32_t rd = get_rd(insn);
    uint32_t funct3 = get_funct3(insn);
    uint32_t pc = rd;
    int32_t imm_i = (insn & 0xfff00000);        // get imm_i with out shifting
    uint32_t rs1 = get_rs1(insn);
   // uint32_t rs2 = get_rs2(insn);

    switch(funct3)
    {
        case funct3_csrrwi:
         pc = rs1 + imm_i;              // add and mask the last 12 bits
         imm_i &= 0xfffff000;
        break; 

        case funct3_csrrsi: 
         pc = rs1 + imm_i;              // add 
         //pc >>= 20;
        break;

        case funct3_csrrci: 
          pc >>= (imm_i % XLEN);        // shift 
       // pc >>= 20;
        break;
    }
    // formats output
   os << render_mnemonic(mnemonic) << render_reg(rd) << ","
     << hex::to_hex0x12(imm_i >> 20) << "," << std::dec << rs1;


    return os.str();


}

/**
 * prints a render reg number in decicaml 
 *
* @param r 
 * @return string
 *
 ********************************************************************************/
std::string rv32i_decode::render_reg(int r)
{
    std::ostringstream os;      // prints the reg r in decicaml with a x
    os << "x" << std::dec << r;
    return os.str();

}

/**
 * prints a render base display 
 *
* @param reg 
* @param imm 
 * @return string
 *
 ********************************************************************************/
std::string rv32i_decode::render_base_disp(uint32_t reg, int32_t imm)
{
    std::ostringstream os;          // gives a base disp with the imm first than the reg in ()
    os << std::dec << imm <<"(" << render_reg(reg) << ")";
    return os.str();

}

/**
 * prints a render mnemonic 
 *
* @param memory 
 * @return string
 *
 ********************************************************************************/
std::string rv32i_decode::render_mnemonic(const std::string &m)
{
    std::ostringstream os;              // sets the mnemonic width to all be the same
    os << std::left << std::setw(mnemonic_width) << m;
    return os.str();

}
