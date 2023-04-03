#include "rv32i_hart.h"

/**
* adds if illgeal instruction occured 
*
 * @param insn the instruction needed to exec 
* @param pos  ostream to be able to print   
 *
 * @note sets the halt tag and adds why the reason for the halt
 ********************************************************************************/
void rv32i_hart::exec_illegal_insn ( uint32_t insn, std::ostream* pos)
{
   if ( pos )
       *pos << render_illegal_insn (insn);

   halt = true ;
   halt_reason = " Illegal instruction ";

}

/**
 * Program ticker to exec an instruction
 * @return void
 *
* @param hdr is a header string   
 *
 * @note simulates the execution of instructions and adds to the instruction counter
 ********************************************************************************/
void rv32i_hart::tick(const std::string & hdr)
{
    if(halt)
    return;

    ++insn_counter;

    if(show_registers)
        dump(hdr);

    if(pc & 0x03)
    {
        halt = true;
        halt_reason = "PC alignment error";
        return;
    }

    uint32_t insn = mem.get32(pc);
    if(show_instructions)
    {
        std::cout << hdr << hex::to_hex32(pc) << ": " << hex::to_hex32(insn) << " ";
        exec(insn, &std::cout);
        std::cout << std::endl;
    }
    else 
        exec(insn,nullptr);

}

/**
 * calls the register dump
 * @return void
 *
* @param hdr is a header string  
 *
 * @note calls register files dump to print out the registers and then the pc
 ********************************************************************************/
void rv32i_hart::dump( const std::string & hdr) const
{
   
    regs.dump(hdr);

    std::cout << "pc " << hex::to_hex32(pc) << std::endl;
}

/**
 * Resets all the GP varibles  
 * @return void
 *
 *
 * @note Makes all the global varibles reset to be able to be used again
 ********************************************************************************/
void rv32i_hart::reset()
{
    pc = 0;
    insn_counter = 0;
    halt = false;
    halt_reason = "none";

}

/**
 * Main exec to find methods
 * @return void
 *
* @param insn the instruction needed to exec 
* @param pos  ostream to be able to print   
 *
 * @note Goes through a switch to find what function is need for the instruction 
 * to be able to execute it 
 ********************************************************************************/
void rv32i_hart::exec(uint32_t insn, std::ostream* pos)
{
     uint32_t funct3 = get_funct3(insn);
    uint32_t funct7 = get_funct7(insn);
    
// switch goes those all possible opcodes and then has sub switchs to break down by the funct3 and then used
// funct7 as needed to break down more
switch(get_opcode(insn))
{
    default:  exec_illegal_insn(insn,pos); return;
    case opcode_lui:  exec_lui(insn,pos); return;
    case opcode_auipc:  exec_auipc(insn,pos); return;
    case opcode_jal:  exec_jal(insn,pos); return;
    case opcode_jalr:  exec_jalr(insn,pos); return;


    case opcode_btype:
        switch (funct3)
        {
            default:  exec_illegal_insn(insn,pos);return;
            case funct3_beq:  exec_beq(insn, pos); return;
            case funct3_bne:  exec_bne(insn, pos); return;
            case funct3_blt:  exec_blt(insn, pos); return;
            case funct3_bge:  exec_bge(insn, pos); return;
            case funct3_bltu:  exec_bltu(insn, pos); return;
            case funct3_bgeu:  exec_bgeu(insn, pos); return;
        }

        assert(0 && "unrecognized funct3");  //impossible

    case opcode_load_imm:
        switch(funct3)
        {
            default:  exec_illegal_insn(insn,pos);return;
            case funct3_lb:  exec_lb( insn,pos); return;
            case funct3_lh:  exec_lh( insn, pos); return;
            case funct3_lw:  exec_lw( insn, pos); return;
            case funct3_lbu:  exec_lbu( insn, pos); return;
            case funct3_lhu:  exec_lhu( insn, pos); return;

        }
     assert(0 && "unrecognized funct3");  //impossible

      case opcode_stype:
        switch(funct3)
        {
            default:  exec_illegal_insn(insn,pos);return;
           case funct3_sb:  exec_sb( insn,pos); return;
           case funct3_sh:  exec_sh( insn, pos); return;
           case funct3_sw:  exec_sw( insn, pos); return;

        }
    assert(0 && "unrecognized funct3");  //impossible

    case opcode_alu_imm:
        switch (funct3)
        {
            default:  exec_illegal_insn(insn,pos);return;
            case funct3_add:  exec_addi(insn, pos, get_imm_i(insn)); return;
            case funct3_sll:  exec_slli(insn, pos, get_imm_i(insn)%XLEN); return;
            case funct3_slt:  exec_slti(insn, pos, get_imm_i(insn)); return;
            case funct3_sltu:  exec_sltiu(insn, pos, get_imm_i(insn)); return;
            case funct3_xor:  exec_xori(insn, pos, get_imm_i(insn)); return;
        
            case funct3_or:  exec_ori(insn, pos, get_imm_i(insn)); return;
            case funct3_and:  exec_andi(insn, pos, get_imm_i(insn)); return;

            case funct3_srx:
                switch(funct7)
                {
                    default:  exec_illegal_insn(insn,pos);return;
                    case funct7_sra:  exec_srai(insn, pos, get_imm_i(insn)%XLEN); return;
                    case funct7_srl:  exec_srli(insn, pos, get_imm_i(insn)%XLEN); return;
                }

            assert(0 && "unrecognized funct7");  //impossible
        }

        assert(0 && "unrecognized funct3");  //impossible

        case opcode_rtype:
            switch(funct3)
            {
                default:  exec_illegal_insn(insn,pos);return;
                case funct3_add: 
                    switch(funct7)
                    {
                        default:  exec_illegal_insn(insn,pos);return;
                        case funct7_add:  exec_add(insn,pos); return;
                        case funct7_sub:  exec_sub(insn,pos); return;
                    }
                case funct3_sll:  exec_sll(insn,pos); return;
                case funct3_slt:   exec_slt(insn,pos); return;
                case funct3_sltu:  exec_sltu(insn,pos); return;
                case funct3_xor:   exec_xor(insn,pos); return;
                case funct3_or:   exec_or(insn,pos); return;
                case funct3_and:  exec_and(insn,pos); return;
                case funct3_srx:
                switch(funct7)
                {
                    default: return exec_illegal_insn(insn,pos);return;
                    case funct7_sra:   exec_sra(insn,pos); return;
                    case funct7_srl:   exec_srl(insn,pos); return;
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
                        case 0: exec_ecall(insn,pos); return;
                        case 1: exec_ebreak(insn,pos); return;
                    }
               
                case funct3_csrrw:  exec_csrrw(insn, pos); return;
                case funct3_csrrs:  exec_csrrs(insn, pos); return;
                case funct3_csrrc:  exec_csrrc (insn, pos); return;
                case funct3_csrrwi:  exec_csrrwi(insn, pos); return;
                case funct3_csrrsi:  exec_csrrsi(insn, pos); return;
                case funct3_csrrci:  exec_csrrci(insn, pos); return;
                assert(0 && "unrecognized funct3");  //impossible
            }

assert(0 && "unrecognized get_opcode(insn)"); // It should be //impossible to ever get here!

}

}

/**
 * prints a exec lui  
 * @return void
 *
* @param insn the instruction needed to exec 
* @param pos  ostream to be able to print   
 *
 * @note simulates the execution of a lui instruction and adds 4 to PC and sets the
 * value in the register
 ********************************************************************************/
void rv32i_hart::exec_lui(uint32_t insn,std::ostream* pos)
{
    uint32_t rd = get_rd(insn);
    int32_t imm_u = get_imm_u(insn);

   if ( pos )
  {
     std::string s = render_lui(insn);
    *pos << std :: setw (instruction_width) << std :: setfill (' ') << std :: left << s;
    *pos << "// " << render_reg(rd) << " = " << hex::to_hex32(imm_u);

  }
  regs.set(rd, imm_u);
  pc += 4;
    
}

/**
 * prints a exec auipc  
 * @return void
 *
* @param insn the instruction needed to exec 
* @param pos  ostream to be able to print   
 *
 * @note simulates the execution of a auipc instruction and adds 4 to PC and sets the
 * value in the register
 ********************************************************************************/
void rv32i_hart::exec_auipc(uint32_t insn,std::ostream* pos)
{
    uint32_t rd = get_rd(insn);
    int32_t imm_u = get_imm_u(insn);

    // simulates the instruction
    int32_t val = pc + imm_u;
    
                
    if ( pos )
  {
    std::string s = render_auipc(insn);
    *pos << std :: setw (instruction_width) << std :: setfill (' ') << std :: left << s;
    *pos << "// " << render_reg(rd) << " = " << hex::to_hex32(pc) << " + " << hex::to_hex32(imm_u) << 
    " = " << hex::to_hex32(val);

  }
  // sets the register rd to the value
    regs.set(rd,val);
  pc += 4;
 
}


/**
 * exec a jal instruction 
 ** @return void
* @param insn the instruction needed to exec 
* @param pos  ostream to be able to print   
 *
 * @note simulates the execution of a jal instruction and adds 4 to PC and sets the
 * value in the register
 ********************************************************************************/
///@parm pc The memory pcess where the insn is stored.
void rv32i_hart::exec_jal(uint32_t insn,std::ostream* pos)
{
    
    uint32_t rd = get_rd(insn);
    int32_t imm_j = get_imm_j(insn);

    // simulates the instruction
    int32_t val = pc + 4;
 
                 

   // adds the rendered values plus a commented version of it to be printed
    if ( pos )
    {
       std::string s = render_jal(rd,insn);
   *pos << std :: setw (instruction_width) << std :: setfill (' ') << std :: left << s;
    *pos << "// " << render_reg(rd) << " = " << hex::to_hex32(pc) << ", + " << hex::to_hex32(imm_j) << 
    " pc = " << hex::to_hex32(val) << " + " << hex::to_hex32(pc) << " = " << hex::to_hex32(val);
    }

    // sets the register rd to the value
    regs.set(rd,val);
     pc += imm_j;
    
}

/**
 * exec a jalr instruction 
 * @return void
 *
* @param insn the instruction needed to exec 
* @param pos  ostream to be able to print   
 *
 * @note simulates the execution of a jalr instruction and adds (regs.get(rs1)+imm_i) & ~1 to PC and sets the
 * value in the register
 ********************************************************************************/
void rv32i_hart::exec_jalr(uint32_t insn,std::ostream* pos)
{
    uint32_t rd = get_rd(insn);
    int32_t imm_i = get_imm_i(insn);
    uint32_t rs1 = get_rs1(insn);

   // simulates the instruction
    int32_t val = pc + 4;

                

   // adds the rendered values plus a commented version of it to be printed
    if ( pos )
    {
   std::string s = render_jalr(insn);
    *pos << std :: setw (instruction_width) << std :: setfill (' ') << std :: left << s;
    *pos << "// " << render_reg(rd) << " = " << hex::to_hex32(pc) << ", + " << hex::to_hex32(imm_i) << 
    " pc = (" << hex::to_hex32(val) << " + " << hex::to_hex32(pc) << ") & 0xfffffffe = " << hex::to_hex32(val);

    }
    // sets the register rd to the value
    regs.set(rd,val);
    pc = (regs.get(rs1)+imm_i) & ~1;
   
}


/**
 * exec a beq instruction 
 * @return void
 *
* @param insn the instruction needed to exec 
* @param pos  ostream to be able to print   
 *
 * @note simulates the execution of a beq instruction and adds(regs.get(rs1) == regs.get(rs2)) ? imm_b : 4);to PC
 ********************************************************************************/
void rv32i_hart::exec_beq(uint32_t insn, std::ostream* pos)
{
    int32_t imm_b = get_imm_b(insn);
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn); 

                

   // adds the rendered values plus a commented version of it to be printed
    if ( pos )
    {
    std::string s = render_btype(rd,insn,"beq");
     *pos << std :: setw (instruction_width) << std :: setfill (' ') << std :: left << s;
    *pos << "// pc += " << " pc += (" << hex::to_hex32(rs1) << " == " << hex::to_hex32(rs2) << 
    " ? " << hex::to_hex32(imm_b) << " : 4 " << " = "  
    << hex::to_hex32(pc );

    }

    pc += (int32_t)((regs.get(rs1) == regs.get(rs2)) ? imm_b : 4);

  
}

/**
 * exec a bge instruction 
 * @return void
 *
* @param insn the instruction needed to exec 
* @param pos  ostream to be able to print   
 *
 * @note simulates the execution of a bge instruction and adds(regs.get(rs1) >= regs.get(rs2)) ? imm_b : 4);to PC
 ********************************************************************************/
void rv32i_hart::exec_bge(uint32_t insn, std::ostream* pos)
{    
    int32_t imm_b = get_imm_b(insn);
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);  


   // adds the rendered values plus a commented version of it to be printed
    if ( pos )
    {
   std::string s = render_btype(rd,insn,"bge");
    *pos << std :: setw (instruction_width) << std :: setfill (' ') << std :: left << s;
    *pos << "// pc += " << " pc += (" << hex::to_hex32(rs1) << " >= " << hex::to_hex32(rs2) << 
    " ? " << hex::to_hex32(imm_b) << " : 4 " << " = "  
    << hex::to_hex32(pc );

    }

    pc += (int32_t)((regs.get(rs1) >= regs.get(rs2)) ? imm_b : 4);

}

/**
 * exec a bgeu instruction 
 * @return void
 *
* @param insn the instruction needed to exec 
* @param pos  ostream to be able to print   
 *
 * @note simulates the execution of a bgeu instruction and adds(regs.get(rs1) >= regs.get(rs2)) ? imm_b : 4);to PC for
 * an unsigned 
 ********************************************************************************/
void rv32i_hart::exec_bgeu(uint32_t insn, std::ostream* pos)
{
    int32_t imm_b = get_imm_b(insn);
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);  


   // adds the rendered values plus a commented version of it to be printed
    if ( pos )
    {
   std::string s = render_btype(rd,insn,"bgeu");
     *pos << std :: setw (instruction_width) << std :: setfill (' ') << std :: left << s;
    *pos << "// pc += " << " pc += (" << hex::to_hex32(rs1) << " >=U " << hex::to_hex32(rs2) << 
    " ? " << hex::to_hex32(imm_b) << " : 4 " << " = "  
    << hex::to_hex32(pc );

    }

     pc += (uint32_t)((regs.get(rs1) >= regs.get(rs2)) ? imm_b : 4);

}

/**
 * exec a blt instruction 
 * @return void
 *
* @param insn the instruction needed to exec 
* @param pos  ostream to be able to print   
 *
 * @note simulates the execution of a blt instruction and adds(regs.get(rs1) < regs.get(rs2)) ? imm_b : 4);to PC
 ********************************************************************************/
void rv32i_hart::exec_blt(uint32_t insn, std::ostream* pos)
{
    int32_t imm_b = get_imm_b(insn);
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);  


   // adds the rendered values plus a commented version of it to be printed
    if ( pos )
    {
   std::string s = render_btype(rd,insn,"blt");
    *pos << std :: setw (instruction_width) << std :: setfill (' ') << std :: left << s;
    *pos << "// pc += " << " pc += (" << hex::to_hex32(rs1) << " < " << hex::to_hex32(rs2) << 
    " ? " << hex::to_hex32(imm_b) << " : 4 " << " = "  
    << hex::to_hex32(pc );

    }

     pc += (int32_t)((regs.get(rs1) < regs.get(rs2)) ? imm_b : 4);

}

/**
 * exec a bltu instruction 
 * @return void
 *
* @param insn the instruction needed to exec 
* @param pos  ostream to be able to print   
 *
 * @note simulates the execution of a beq instruction and adds(regs.get(rs1) < regs.get(rs2)) ? imm_b : 4);to PC for 
 * unsigned
 ********************************************************************************/
void rv32i_hart::exec_bltu(uint32_t insn, std::ostream* pos)
{
    int32_t imm_b = get_imm_b(insn);
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn); 


   // adds the rendered values plus a commented version of it to be printed
    if ( pos )
    {
   std::string s = render_btype(rd,insn,"bltu");
     *pos << std :: setw (instruction_width) << std :: setfill (' ') << std :: left << s;
    *pos << "// pc += " << " pc += (" << hex::to_hex32(rs1) << " <U " << hex::to_hex32(rs2) << 
    " ? " << hex::to_hex32(imm_b) << " : 4 " << " = "  
    << hex::to_hex32(pc );

    }

    pc += (uint32_t)(regs.get(rs1) < regs.get(rs2)) ? imm_b : 4;

}

/**
 * exec a bne instruction 
 * @return void
 *
* @param insn the instruction needed to exec 
* @param pos  ostream to be able to print   
 *
 * @note simulates the execution of a bne instruction and adds(regs.get(rs1) != regs.get(rs2)) ? imm_b : 4);to PC
 ********************************************************************************/
void rv32i_hart::exec_bne(uint32_t insn, std::ostream* pos)
{
    int32_t imm_b = get_imm_b(insn);
    uint32_t rd = get_rd(insn);
    int32_t rs1 = get_rs1(insn);
    int32_t rs2 = get_rs2(insn);  


   // adds the rendered values plus a commented version of it to be printed
    if ( pos )
    {
        std::string s = render_btype(rd,insn,"bne");
     *pos << std :: setw (instruction_width) << std :: setfill (' ') << std :: left << s;
    *pos << "// pc += " << " pc += (" << hex::to_hex32(rs1) << " != " << hex::to_hex32(rs2) << 
    " ? " << hex::to_hex32(imm_b) << " : 4 " << " = "  
    << hex::to_hex32(pc);

    }

     pc += (int32_t)((regs.get(rs1) != regs.get(rs2)) ? imm_b : 4);

}



/**
 * exec a load instruction
 * @return void
 *
* @param insn the instruction needed to exec 
* @param pos  ostream to be able to print   
 *
 * @note simulates the execution of a lb instruction and adds 4 to PC
 ********************************************************************************/
void rv32i_hart::exec_lb(uint32_t insn, std::ostream* pos)
{
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    uint32_t imm_i = get_imm_i(insn);

    mem.set8(regs.get(rs1),imm_i);
    // simulates the instruction
    int32_t val = (mem.get8_sx(regs.get(rs1)+imm_i));


   // adds the rendered values plus a commented version of it to be printed
    if ( pos )
    {
   std::string s = render_itype_load(insn,"lb");
    *pos << std :: setw (instruction_width) << std :: setfill (' ') << std :: left << s;
    *pos << "// " << render_reg(rd) << " = sx(m8(" << hex::to_hex32(rs1) << " + " << hex::to_hex32(imm_i)
    << ")) = " << hex::to_hex32(val);

    }
    regs.set(rd, val);
    pc+=4;
    
}

/**
 * exec a load instruction
 * @return void
 *
* @param insn the instruction needed to exec 
* @param pos  ostream to be able to print   
 *
 * @note simulates the execution of a lh instruction and adds 4 to PC
 ********************************************************************************/
void rv32i_hart::exec_lh(uint32_t insn, std::ostream* pos)
{
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    uint32_t imm_i = get_imm_i(insn);
    
    mem.set16(regs.get(rs1),imm_i);
    // simulates the instruction
    int32_t val = (mem.get16_sx(regs.get(rs1)+imm_i));


   // adds the rendered values plus a commented version of it to be printed
    if ( pos )
    {
    std::string s = render_itype_load(insn,"lh");
    *pos << std :: setw (instruction_width) << std :: setfill (' ') << std :: left << s;
    *pos << "// " << render_reg(rd) << " = sx(m16(" << hex::to_hex32(rs1) << " + " << hex::to_hex32(imm_i)
    << ")) = " << hex::to_hex32(val);

    }
    regs.set(rd, val);
    pc+=4;
        
}

/**
 * exec a load instruction
 * @return void
 *
* @param insn the instruction needed to exec 
* @param pos  ostream to be able to print   
 *
 * @note simulates the execution of a lw instruction and adds 4 to PC
 ********************************************************************************/
void rv32i_hart::exec_lw(uint32_t insn, std::ostream* pos)
{
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    uint32_t imm_i = get_imm_i(insn);
    
    mem.set32(regs.get(rs1),imm_i);
    // simulates the instruction
    int32_t val = (mem.get32(regs.get(rs1)+imm_i));


   // adds the rendered values plus a commented version of it to be printed
    if ( pos )
    {
    std::string s = render_itype_load(insn,"lw");
   *pos << std :: setw (instruction_width) << std :: setfill (' ') << std :: left << s;
    *pos << "// " << render_reg(rd) << " = sx(m32(" << hex::to_hex32(rs1) << " + " << hex::to_hex32(imm_i)
    << ")) = " << hex::to_hex32(val);

    }
    regs.set(rd, val);
    pc+=4;
}

/**
 * exec a load instruction
 * @return void
 *
* @param insn the instruction needed to exec 
* @param pos  ostream to be able to print   
 *
 * @note simulates the execution of a lbu instruction and adds 4 to PC
 ********************************************************************************/
void rv32i_hart::exec_lbu(uint32_t insn, std::ostream* pos)
{
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    uint32_t imm_i = get_imm_i(insn);

    mem.set8(regs.get(rs1),imm_i);
    // simulates the instruction
    int32_t val = (mem.get8_sx(regs.get(rs1)+imm_i));


   // adds the rendered values plus a commented version of it to be printed
    if ( pos )
    {
    std::string s = render_itype_load(insn,"lbu");
   *pos << std :: setw (instruction_width) << std :: setfill (' ') << std :: left << s;
    *pos << "// " << render_reg(rd) << " = zx(m8(" << hex::to_hex32(rs1) << " + " << hex::to_hex32(imm_i)
    << ")) = " << hex::to_hex32(val);

    }
    regs.set(rd, val);
    pc+=4;
}

/**
 * exec a load instruction
 * @return void
 *
* @param insn the instruction needed to exec 
* @param pos  ostream to be able to print   
 *
 * @note simulates the execution of a lhu instruction and adds 4 to PC
 ********************************************************************************/
void rv32i_hart::exec_lhu(uint32_t insn, std::ostream* pos)
{
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    uint32_t imm_i = get_imm_i(insn);

    mem.set16(regs.get(rs1),imm_i);
    // simulates the instruction
    int32_t val = (mem.get16_sx(regs.get(rs1)+imm_i));

   // adds the rendered values plus a commented version of it to be printed
    if ( pos )
    {
    std::string s = render_itype_load(insn,"lhu");
   *pos << std :: setw (instruction_width) << std :: setfill (' ') << std :: left << s;
    *pos << "// " << render_reg(rd) << " = zx(m16(" << hex::to_hex32(rs1) << " + " << hex::to_hex32(imm_i)
    << ")) = " << hex::to_hex32(val);

    }
    regs.set(rd, val);
    pc+=4;

}
 

/**
 * exec a s type instruction
 * @return void
 *
* @param insn the instruction needed to exec 
* @param pos  ostream to be able to print   
 *
 * @note simulates the execution of a sb instruction and adds 4 to PC and sets the value in the register
 ********************************************************************************/
void rv32i_hart::exec_sb(uint32_t insn, std::ostream* pos)
{
    uint32_t rs1 = get_rs1(insn);
    uint32_t imm_s = get_imm_s(insn);
    uint32_t rd = get_rd(insn);

    mem.set8(regs.get(rs1),imm_s);
   int32_t rs2 = (regs.get(rs1)+imm_s);


   // adds the rendered values plus a commented version of it to be printed
    if ( pos )
    {
    std::string s = render_stype(insn, "sb ");
    *pos << std :: setw (instruction_width) << std :: setfill (' ') << std :: left << s;
    *pos << "// m8(" << hex::to_hex32(rs1) << " + " << hex::to_hex32(imm_s)
    << ") = " << hex::to_hex32(rs2);

    }
    regs.set(rd, rs2);
    pc+=4;


}
/**
 * exec a s type instruction
 * @return void
 *
* @param insn the instruction needed to exec 
* @param pos  ostream to be able to print   
 *
 * @note simulates the execution of a sh instruction and adds 4 to PC and sets the value in the register
 ********************************************************************************/
void rv32i_hart::exec_sh(uint32_t insn, std::ostream* pos)
{
    uint32_t rs1 = get_rs1(insn);
    uint32_t imm_s = get_imm_s(insn);
    uint32_t rd = get_rd(insn);

    mem.set16(regs.get(rs1),imm_s);
   int32_t rs2 = (regs.get(rs1)+imm_s);


   // adds the rendered values plus a commented version of it to be printed
    if ( pos )
    {
    std::string s = render_stype(insn, "sh ");
   *pos << std :: setw (instruction_width) << std :: setfill (' ') << std :: left << s;
    *pos << "// m16(" << hex::to_hex32(rs1) << " + " << hex::to_hex32(imm_s)
    << ") = " << hex::to_hex32(rs2);

    }
    regs.set(rd, rs2);
    pc+=4;

}

/**
 * exec a s type instruction
 * @return void
 *
* @param insn the instruction needed to exec 
* @param pos  ostream to be able to print   
 *
 * @note simulates the execution of a sw instruction and adds 4 to PC and sets the value in the register
 ********************************************************************************/
void rv32i_hart::exec_sw(uint32_t insn, std::ostream* pos)
{
   uint32_t rs1 = get_rs1(insn);
    uint32_t imm_s = get_imm_s(insn);
    uint32_t rd = get_rd(insn);

    mem.set32(regs.get(rs1),imm_s);
   int32_t rs2 = (regs.get(rs1)+imm_s);



   // adds the rendered values plus a commented version of it to be printed
    if ( pos )
    {
    std::string s = render_stype(insn, "sw ");
    *pos << std :: setw (instruction_width) << std :: setfill (' ') << std :: left << s;
    *pos << "// m32(" << hex::to_hex32(rs1) << " + " << hex::to_hex32(imm_s)
    << ") = " << hex::to_hex32(rs2);

    }
    regs.set(rd, rs2);
    pc+=4;


}

/**
 * exec a i type instruction
 * @return void
 *
* @param insn the instruction needed to exec 
* @param pos  ostream to be able to print   
 *
 * @note simulates the execution of a addi instruction and adds 4 to PC and sets the value in the register
 ********************************************************************************/
void rv32i_hart::exec_addi(uint32_t insn, std::ostream* pos, int32_t imm_i)
{
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);

    // simulates the instruction
    int32_t val = (regs.get(rs1) + imm_i);


   // adds the rendered values plus a commented version of it to be printed
    if ( pos )
    {
    std::string s = render_rtype(insn, "slt ");
    *pos << std :: setw (instruction_width) << std :: setfill (' ') << std :: left << s;
    *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32 (regs.get (rs1) ) << " + "
    << hex::to_hex0x32 (imm_i) << " = " << hex::to_hex0x32 (val);

    }
    // sets the register rd to the value
    regs.set(rd,val);
    pc+=4;
}

/**
 * exec a i type instruction
 * @return void
 *
* @param insn the instruction needed to exec 
* @param pos  ostream to be able to print   
 *
 * @note simulates the execution of a slti instruction and adds 4 to PC and sets the value in the register
 ********************************************************************************/
void rv32i_hart::exec_slti(uint32_t insn, std::ostream* pos, int32_t imm_i)
{
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);

    // simulates the instruction
    int32_t val =  ((regs.get(rs1) < imm_i) ? 1 : 0);       // ifregs.get(rs1) is less than rs2 print 1 or 0


   // adds the rendered values plus a commented version of it to be printed
    if ( pos )
    {
   std::string s = render_rtype(insn, "slt ");
      *pos << std :: setw (instruction_width) << std :: setfill (' ') << std :: left << s;
    *pos << "// " << render_reg(rd) << " = (" << hex::to_hex0x32 (regs.get (rs1) ) << " < " << 
    hex::to_hex32(imm_i) << ") ? 1 : 0 = " << hex::to_hex0x32 (val);

    }
    // sets the register rd to the value
    regs.set(rd,val);
    pc+=4;
}

/**
 * exec a i type instruction
 * @return void
 *
* @param insn the instruction needed to exec 
* @param pos  ostream to be able to print   
 *
 * @note simulates the execution of a sltiu instruction and adds 4 to PC and sets the value in the register
 ********************************************************************************/
void rv32i_hart::exec_sltiu(uint32_t insn, std::ostream* pos, int32_t imm_i)
{
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);

    // simulates the instruction
    int32_t val =  ((regs.get(rs1) < imm_i) ? 1 : 0);       // ifregs.get(rs1) is less than rs2 print 1 or 0


   // adds the rendered values plus a commented version of it to be printed
    if ( pos )
    {
    std::string s = render_rtype(insn, "slt ");
    *pos << std :: setw (instruction_width) << std :: setfill (' ') << std :: left << s;
    *pos << "// " << render_reg(rd) << " = (" << hex::to_hex0x32 (regs.get (rs1) ) << " <U " << 
    hex::to_hex32(imm_i) << ") ? 1 : 0 = " << hex::to_hex0x32 (val);

    }
    // sets the register rd to the value
    regs.set(rd,val);
    pc+=4;
}

/**
 * exec a i type instruction
 * @return void
 *
* @param insn the instruction needed to exec 
* @param pos  ostream to be able to print   
 *
 * @note simulates the execution of a xori instruction and adds 4 to PC and sets the value in the register
 ********************************************************************************/
void rv32i_hart::exec_xori(uint32_t insn, std::ostream* pos, int32_t imm_i)
{
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);

    // simulates the instruction
    int32_t val =  (regs.get(rs1) ^ imm_i );            //xor


   // adds the rendered values plus a commented version of it to be printed
    if ( pos )
    {
    std::string s = render_rtype(insn, "slt ");
    *pos << std :: setw (instruction_width) << std :: setfill (' ') << std :: left << s;
    *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32 (regs.get (rs1) ) << " ^ " << 
    hex::to_hex32(imm_i) << " = " << hex::to_hex0x32 (val);

    }
    // sets the register rd to the value
    regs.set(rd,val);
    pc+=4;
}

/**
 * exec a i type instruction
 * @return void
 *
* @param insn the instruction needed to exec 
* @param pos  ostream to be able to print   
 *
 * @note simulates the execution of a ori instruction and adds 4 to PC and sets the value in the register
 ********************************************************************************/
void rv32i_hart::exec_ori(uint32_t insn, std::ostream* pos, int32_t imm_i)
{
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);

    // simulates the instruction
    int32_t val =  (regs.get(rs1) | imm_i);         // reg =regs.get(rs1) or imm_i


   // adds the rendered values plus a commented version of it to be printed
    if ( pos )
    {
   std::string s = render_rtype(insn, "slt ");
   *pos << std :: setw (instruction_width) << std :: setfill (' ') << std :: left << s;
    *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32 (regs.get (rs1) ) << " | " << 
    hex::to_hex32(imm_i) << " = " << hex::to_hex0x32 (val);

    }
    // sets the register rd to the value
    regs.set(rd,val);
    pc+=4;
}

/**
 * exec a i type instruction
 * @return void
 *
* @param insn the instruction needed to exec 
* @param pos  ostream to be able to print   
 *
 * @note simulates the execution of a andi instruction and adds 4 to PC and sets the value in the register
 ********************************************************************************/
void rv32i_hart::exec_andi(uint32_t insn, std::ostream* pos, int32_t imm_i)
{
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);

    // simulates the instruction
    int32_t val =  (regs.get(rs1) & imm_i);         // reg =regs.get(rs1) & imm_i


   // adds the rendered values plus a commented version of it to be printed
    if ( pos )
    {
    std::string s = render_rtype(insn, "slt ");
    *pos << std :: setw (instruction_width) << std :: setfill (' ') << std :: left << s;
    *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32 (regs.get (rs1) ) << " & " << 
    hex::to_hex32(imm_i) << " = " << hex::to_hex0x32 (val);

    }
    // sets the register rd to the value
    regs.set(rd,val);
    pc+=4;
}

/**
 * exec a i type instruction
 * @return void
 *
* @param insn the instruction needed to exec 
* @param pos  ostream to be able to print   
 *
 * @note simulates the execution of a slli instruction and adds 4 to PC and sets the value in the register
 ********************************************************************************/
void rv32i_hart::exec_slli(uint32_t insn, std::ostream* pos, int32_t imm_i)
{
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);

    // simulates the instruction
    int32_t val =  (regs.get(rs1) << imm_i);        // shif


   // adds the rendered values plus a commented version of it to be printed
    if ( pos )
    {
    std::string s = render_rtype(insn, "slt ");
   *pos << std :: setw (instruction_width) << std :: setfill (' ') << std :: left << s;
    *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32 (regs.get (rs1) ) << " << " << 
    hex::to_hex32(imm_i) << " = " << hex::to_hex0x32 (val);

    }
    // sets the register rd to the value
    regs.set(rd,val);
    pc+=4;
}

/**
 * exec a i type instruction
 * @return void
 *
* @param insn the instruction needed to exec 
* @param pos  ostream to be able to print   
 *
 * @note simulates the execution of a srli instruction and adds 4 to PC and sets the value in the register
 ********************************************************************************/
void rv32i_hart::exec_srli(uint32_t insn, std::ostream* pos, int32_t imm_i)
{
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);

    // simulates the instruction
    int32_t val =  (regs.get(rs1) << imm_i);


   // adds the rendered values plus a commented version of it to be printed
    if ( pos )
    {
    std::string s = render_rtype(insn, "slt ");
   *pos << std :: setw (instruction_width) << std :: setfill (' ') << std :: left << s;
    *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32 (regs.get (rs1) ) << " >> " << 
    hex::to_hex32(imm_i) << " = " << hex::to_hex0x32 (val);
    }
    // sets the register rd to the value
    regs.set(rd,val);
    pc+=4;
}

/**
 * exec a i type instruction
 * @return void
 *
* @param insn the instruction needed to exec 
* @param pos  ostream to be able to print   
 *
 * @note simulates the execution of a srai instruction and adds 4 to PC and sets the value in the register
 ********************************************************************************/
void rv32i_hart::exec_srai(uint32_t insn, std::ostream* pos, int32_t imm_i)
{
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);

    // simulates the instruction
    int32_t val =  (regs.get(rs1) >> imm_i);


   // adds the rendered values plus a commented version of it to be printed
    if ( pos )
    {
   std::string s = render_rtype(insn, "slt ");
   *pos << std :: setw (instruction_width) << std :: setfill (' ') << std :: left << s;
    *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32 (regs.get (rs1) ) << " >> " << 
    hex::to_hex32(imm_i) << " = " << hex::to_hex0x32 (val); 

    }
    // sets the register rd to the value
    regs.set(rd,val);
    pc+=4;
}
 
/**
 * exec a r type instruction
 * @return void
 *
* @param insn the instruction needed to exec 
* @param pos  ostream to be able to print   
 *
 * @note simulates the execution of a add instruction and adds 4 to PC and sets the value in the register
 ********************************************************************************/
   
void rv32i_hart::exec_add(uint32_t insn, std::ostream* pos)
{
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);

    // simulates the instruction
    int32_t val =  regs.get(rs1) + regs.get(rs2);


   // adds the rendered values plus a commented version of it to be printed
    if ( pos )
    {
    std::string s = render_rtype(insn, "slt ");
   *pos << std :: setw (instruction_width) << std :: setfill (' ') << std :: left << s;
    *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32 (regs.get (rs1) ) << " + "
    << hex::to_hex0x32 (rs2) << " = " << hex::to_hex0x32 (val);

    }
    // sets the register rd to the value
    regs.set(rd,val);
    pc+=4;
}

/**
 * exec a r type instruction
 * @return void
 *
* @param insn the instruction needed to exec 
* @param pos  ostream to be able to print   
 *
 * @note simulates the execution of a sub instruction and adds 4 to PC and sets the value in the register
 ********************************************************************************/
void rv32i_hart::exec_sub(uint32_t insn, std::ostream* pos)
{
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);

    // simulates the instruction
    int32_t val = regs.get(rs1) - regs.get(rs2);

   // adds the rendered values plus a commented version of it to be printed
    if ( pos )
    {
    std::string s = render_rtype(insn, "slt ");
    *pos << std :: setw (instruction_width) << std :: setfill (' ') << std :: left << s;
    *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32 (regs.get (rs1) ) << " - "
    << hex::to_hex0x32 (rs2) << " = " << hex::to_hex0x32 (val);

    }

    // sets the register rd to the value
    regs.set(rd,val);
    pc+=4;

}

/**
 * exec a r type instruction
 * @return void
 *
* @param insn the instruction needed to exec 
* @param pos  ostream to be able to print   
 *
 * @note simulates the execution of a and instruction and adds 4 to PC and sets the value in the register
 ********************************************************************************/
void rv32i_hart::exec_and(uint32_t insn, std::ostream* pos)
{
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);

    // simulates the instruction
    int32_t val = regs.get(rs1) & regs.get(rs2);

   // adds the rendered values plus a commented version of it to be printed
    if ( pos )
    {
    std::string s = render_rtype(insn, "slt ");
    *pos << std :: setw (instruction_width) << std :: setfill (' ') << std :: left << s;
    *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32 (regs.get (rs1) ) << " & "
    << hex::to_hex0x32 (rs2) << " = " << hex::to_hex0x32 (val);

    }
    // sets the register rd to the value
    regs.set(rd,val);
    pc+=4;

}

/**
* exec a r type instruction
 * @return void
 *
* @param insn the instruction needed to exec 
* @param pos  ostream to be able to print   
 *
 * @note simulates the execution of a addi instruction and adds 4 to PC and sets the value in the register
 ********************************************************************************/
void rv32i_hart::exec_or(uint32_t insn, std::ostream* pos)
{
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);

    // simulates the instruction
    int32_t val = regs.get(rs1) | regs.get(rs2);

   // adds the rendered values plus a commented version of it to be printed
    if ( pos )
    {
   std::string s = render_rtype(insn, "slt ");
    *pos << std :: setw (instruction_width) << std :: setfill (' ') << std :: left << s;
    *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32 (regs.get (rs1) ) << " | "
    << hex::to_hex0x32 (rs2) << " = " << hex::to_hex0x32 (val);

    }
    // sets the register rd to the value
    regs.set(rd,val);
    pc+=4;
}

/**
* exec a r type instruction
 * @return void
 *
* @param insn the instruction needed to exec 
* @param pos  ostream to be able to print   
 *
 * @note simulates the execution of a sll instruction and adds 4 to PC and sets the value in the register
 ********************************************************************************/
void rv32i_hart::exec_sll(uint32_t insn, std::ostream* pos)
{
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);

    // simulates the instruction
    int32_t val = (regs.get(rs1) << (regs.get(rs2) % XLEN));

   // adds the rendered values plus a commented version of it to be printed
    if ( pos )
    {
    std::string s = render_rtype(insn, "slt ");
    *pos << std :: setw (instruction_width) << std :: setfill (' ') << std :: left << s;
    *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32 (regs.get (rs1) ) << " << " << 
    XLEN << " = " << hex::to_hex0x32 (val);

    }
    // sets the register rd to the value
    regs.set(rd,val);
    pc+=4;
}
/**
* exec a r type instruction
 * @return void
 *
* @param insn the instruction needed to exec 
* @param pos  ostream to be able to print   
 *
 * @note simulates the execution of a slt instruction and adds 4 to PC and sets the value in the register
 ********************************************************************************/
void rv32i_hart::exec_slt(uint32_t insn, std::ostream* pos)
{
   uint32_t rd = get_rd(insn);
   uint32_t rs1 = get_rs1(insn);
   uint32_t rs2 = get_rs2(insn);

   // simulates the instruction
    int32_t val = (regs.get(rs1) < regs.get(rs2)) ? 1 : 0;

   if ( pos )
  {
    std::string s = render_rtype(insn, "slt ");
    *pos << std :: setw (instruction_width) << std :: setfill (' ') << std :: left << s;
    *pos << "// " << render_reg(rd) << " = (" << hex::to_hex0x32 (regs.get (rs1) ) << " < " << 
    hex::to_hex0x32 (regs.get(rs2)) << ") ? 1 : 0 = " << hex::to_hex0x32 (val);
  }
  regs.set(rd, val);
  pc += 4;

}

/**
* exec a r type instruction
 * @return void
 *
* @param insn the instruction needed to exec 
* @param pos  ostream to be able to print   
 *
 * @note simulates the execution of a sltu instruction and adds 4 to PC and sets the value in the register
 ********************************************************************************/
void rv32i_hart::exec_sltu(uint32_t insn, std::ostream* pos)
{
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);

    // simulates the instruction
    int32_t val = ((regs.get(rs1)< regs.get(rs2))? 1: 0);

   // adds the rendered values plus a commented version of it to be printed
    if ( pos )
    {
    std::string s = render_rtype(insn, "slt ");
    *pos << std :: setw (instruction_width) << std :: setfill (' ') << std :: left << s;
    *pos << "// " << render_reg(rd) << " = (" << hex::to_hex0x32 (regs.get (rs1) ) << " <U " << 
    hex::to_hex0x32 (regs.get(rs2)) << ") ? 1 : 0 = " << hex::to_hex0x32 (val);

    }
    // sets the register rd to the value
    regs.set(rd,val);
    pc+=4;
}

/**
* exec a r type instruction
 * @return void
 *
* @param insn the instruction needed to exec 
* @param pos  ostream to be able to print   
 *
 * @note simulates the execution of a srl instruction and adds 4 to PC and sets the value in the register
 ********************************************************************************/
void rv32i_hart::exec_srl(uint32_t insn, std::ostream* pos)
{
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);

    // simulates the instruction
    int32_t val = (regs.get(rs1) >> (regs.get(rs2) % XLEN));

   // adds the rendered values plus a commented version of it to be printed
    if ( pos )
    {
    std::string s = render_rtype(insn, "slt ");
    *pos << std :: setw (instruction_width) << std :: setfill (' ') << std :: left << s;
    *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32 (regs.get (rs1) ) << " >> " << 
    XLEN << " = " << hex::to_hex0x32 (val);

    }
    // sets the register rd to the value
    regs.set(rd,val);
    pc+=4;
}

/**
* exec a r type instruction
 * @return void
 *
* @param insn the instruction needed to exec 
* @param pos  ostream to be able to print   
 *
 * @note simulates the execution of a sra instruction and adds 4 to PC and sets the value in the register
 ********************************************************************************/
void rv32i_hart::exec_sra(uint32_t insn, std::ostream* pos)
{
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);

    // simulates the instruction
    int32_t val = (regs.get(rs1) >> (regs.get(rs2) % XLEN));

   // adds the rendered values plus a commented version of it to be printed
    if ( pos )
    {
    std::string s = render_rtype(insn, "slt ");
    *pos << std :: setw (instruction_width) << std :: setfill (' ') << std :: left << s;
    *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32 (regs.get (rs1) ) << " >> " << 
    XLEN << " = " << hex::to_hex0x32 (val);

    }
    // sets the register rd to the value
    regs.set(rd,val);
    pc+=4;
}

/**
* exec a r type instruction
 * @return void
 *
* @param insn the instruction needed to exec 
* @param pos  ostream to be able to print   
 *
 * @note simulates the execution of a xor instruction and adds 4 to PC and sets the value in the register
 ********************************************************************************/
void rv32i_hart::exec_xor(uint32_t insn, std::ostream* pos)
{
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);

    // simulates the instruction
    int32_t val = regs.get(rs1) ^ regs.get(rs2);

   // adds the rendered values plus a commented version of it to be printed
    if ( pos )
    {
    std::string s = render_rtype(insn, "slt ");
    *pos << std :: setw (instruction_width) << std :: setfill (' ') << std :: left << s;
    *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32 (regs.get (rs1) ) << " ^ " << 
    hex::to_hex32(rs2) << " = " << hex::to_hex0x32 (val);

    }
    // sets the register rd to the value
    regs.set(rd,val);
    pc+=4;
}

/**
* exec a ecall instruction
 * @return void
 *
* @param insn the instruction needed to exec 
* @param pos  ostream to be able to print   
 *
 * @note simulates the execution of a ecall instruction and halts the program 
 ********************************************************************************/
void rv32i_hart::exec_ecall(uint32_t insn , std::ostream* pos)
{
    if(pos)
    {
        std::string s = render_ecall(insn);
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// HALT ";
    }
    halt = true;
    halt_reason = " ECALL instruction ";

}

/**
* exec a ebreak instruction
 * @return void
 *
* @param insn the instruction needed to exec 
* @param pos  ostream to be able to print   
 *
 * @note simulates the execution of a ebreak instruction and halts the program 
 ********************************************************************************/
void rv32i_hart::exec_ebreak(uint32_t insn , std::ostream* pos)
{
    if(pos)
    {
        std::string s = render_ebreak(insn);
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// HALT ";
    }
    halt = true;
    halt_reason = " EBREAK instruction ";
}

/**
* exec a csrrw instruction
 * @return void
 *
* @param insn the instruction needed to exec 
* @param pos  ostream to be able to print   
 *
 * @note simulates the execution of a csrrw instruction and adds 4 to PC and sets the value in the register
 ********************************************************************************/

void rv32i_hart::exec_csrrw(uint32_t insn, std::ostream* pos)
{
    //uint32_t rd = get_rd(insn);
    
    
    int32_t imm_i = (insn & 0xfff00000);

    pc >>= (imm_i % XLEN);     /// shifting mod bits

}

/**
* exec a csrrw instruction
 * @return void
 *
* @param insn the instruction needed to exec 
* @param pos  ostream to be able to print   
 *
 * @note simulates the execution of a csrrw instruction and adds 4 to PC and sets the value in the register
 ********************************************************************************/
void rv32i_hart::exec_csrrs(uint32_t insn, std::ostream* pos)
{
   // uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    
    int32_t imm_i = (insn & 0xfff00000);

    pc = regs.get(rs1) - imm_i;      //sub

}
 

/**
* exec a csrrw instruction
 * @return void
 *
* @param insn the instruction needed to exec 
* @param pos  ostream to be able to print   
 *
 * @note simulates the execution of a csrrw instruction and adds 4 to PC and sets the value in the register
 ********************************************************************************/
void rv32i_hart::exec_csrrc(uint32_t insn, std::ostream* pos)
{
   // uint32_t rd = get_rd(insn);
    
    
    int32_t imm_i = (insn & 0xfff00000);
    uint32_t rs1 = get_rs1(insn);

    pc = regs.get(rs1) + imm_i;      // add 

}
   

/**
* exec a csrrw instruction
 * @return void
 *
* @param insn the instruction needed to exec 
* @param pos  ostream to be able to print   
 *
 * @note simulates the execution of a csrrw instruction and adds 4 to PC and sets the value in the register
 ********************************************************************************/
void rv32i_hart::exec_csrrwi(uint32_t insn, std::ostream* pos)
{
   // uint32_t rd = get_rd(insn);
    
    
    int32_t imm_i = (insn & 0xfff00000);        // get imm_i with out shifting
    uint32_t rs1 = get_rs1(insn);

    pc = regs.get(rs1) + imm_i;              // add and mask the last 12 bits
}

/**
* exec a csrrw instruction
 * @return void
 *
* @param insn the instruction needed to exec 
* @param pos  ostream to be able to print   
 *
 * @note simulates the execution of a csrrw instruction and adds 4 to PC and sets the value in the register
 ********************************************************************************/
void rv32i_hart::exec_csrrsi(uint32_t insn, std::ostream* pos)
{
    //uint32_t rd = get_rd(insn);
    
    
    int32_t imm_i = (insn & 0xfff00000);        // get imm_i with out shifting
    uint32_t rs1 = get_rs1(insn);

    pc = regs.get(rs1) + imm_i;              // add 

}

/**
* exec a csrrw instruction
 * @return void
 *
* @param insn the instruction needed to exec 
* @param pos  ostream to be able to print   
 *
 * @note simulates the execution of a csrrw instruction and adds 4 to PC and sets the value in the register
 ********************************************************************************/
void rv32i_hart::exec_csrrci(uint32_t insn, std::ostream* pos)
{
   // uint32_t rd = get_rd(insn);
    
    
    int32_t imm_i = (insn & 0xfff00000);        // get imm_i with out shifting

    pc >>= (imm_i % XLEN);        // shift 

}
