#include "simulator.h"
Simulator::Simulator() : pc(0) {
}

//开始运行
void Simulator::run(const int max_steps) {
    int step = 0;
    Status status = Status::AOK;
    for (step = 0; step < max_steps && status == Status::AOK; step++) {
        status = next_instruction();
    }
    report_stopped(step, status);
}

static uint8_t get_hi4(const uint8_t pack) { return (pack >> 4) & 0xF; }//获得指令码
static uint8_t get_lo4(const uint8_t pack) { return pack & 0xF; }//获得指令功能

Status Simulator::next_instruction() {
    uint64_t next_pc = pc;//地址

    // get code and function (1 byte)
    const std::optional<uint8_t> codefun = memory.get_byte(next_pc);
    if (!codefun) {
        report_bad_inst_addr();
        return Status::ADR;
    }
    const auto icode = static_cast<InstructionCode>(get_hi4(codefun.value()));
    uint8_t ifun = get_lo4(codefun.value());
    next_pc++;
    uint8_t regA = 0,regB = 0;
    std::optional<uint64_t> imm = 0;

    // TODO: get registers if needed (1 byte)
     if (need_regids(icode)) {
         const std::optional<uint8_t> iregister = memory.get_byte(next_pc);
         next_pc++;
         regA = get_hi4(iregister.value());
         regB = get_lo4(iregister.value());
         if (!iregister) {
             report_bad_inst_addr();
             return Status::ADR;
         }
     }

    // TODO: get immediate if needed (8 bytes)
     if (need_imm(icode)) {
         imm = memory.get_long(next_pc);
         next_pc+=8;
         if(!imm){
             report_bad_inst_addr();
             return Status::ADR;
         }
     }

    // execute the instruction
    switch (icode) {
        case InstructionCode::HALT: // 0:0
        {
            return Status::HLT;
        }
        case InstructionCode::NOP: // 1:0
        {
            pc = next_pc;
            return Status::AOK;
        }

        case InstructionCode::RRMOVQ: // 2:x regA:regB
        {
            if(error_invalid_reg(regB)|| error_invalid_reg(regA)){
                return Status::INS;}
            if(cc.satisfy(static_cast<Condition>(ifun))){
                registers[regB]=registers[regA];
            }
            pc=next_pc;

            return Status::AOK;
        }

        case InstructionCode::IRMOVQ: // 3:0 F:regB imm
        {
            if(error_invalid_reg(regB)|| error_valid_reg(regA)){
                return Status::INS;}
            registers[regB] = *imm;
            pc = next_pc;
            return Status::AOK;
        }

        case InstructionCode::RMMOVQ: // 4:0 regA:regB imm
        {
            if(error_invalid_reg(regB)|| error_invalid_reg(regA)){
                return Status::INS;}
            uint64_t tmp=*imm+registers[regB];
            if(!memory.set_long(tmp,registers[regA]))
            {
                report_bad_data_addr(tmp);
                return Status::ADR;
            }
            pc = next_pc;
            return Status::AOK;
        }

        case InstructionCode::MRMOVQ: // 5:0 regA:regB imm
        {
            if(error_invalid_reg(regB)|| error_invalid_reg(regA))
            {
                return Status::INS;
            }
            uint64_t tmp=*imm+registers[regB];
            if(!memory.get_long(tmp) ){
                report_bad_data_addr(tmp);
                return Status::ADR;
            }else{
                registers[regA]=*memory.get_long(tmp);
                pc = next_pc;
                return Status::AOK;
            }
        }

        case InstructionCode::ALU: // 6:x regA:regB
        {
            if(error_invalid_reg(regB)|| error_invalid_reg(regA)){
                return Status::INS;}
            switch(static_cast<AluOp>(ifun)){
                case AluOp::ADD:
                {
                    uint64_t tmp=registers[regB]+registers[regA];
                    cc=ConditionCodes::compute(AluOp::ADD,registers[regA],registers[regB],tmp);
                    registers[regB]=tmp;
                    break;
                }
                case AluOp::SUB:
                {
                    uint64_t tmp=registers[regB]-registers[regA];
                    cc=ConditionCodes::compute(AluOp::SUB,registers[regA],registers[regB],tmp);
                    registers[regB]=tmp;
                    break;
                }
                case AluOp::AND:
                {
                    uint64_t  tmp = registers[regB] & registers[regA];
                    cc=ConditionCodes::compute(AluOp::AND,registers[regA],registers[regB],tmp);
                    registers[regB]=tmp;
                    break;
                }
                case AluOp::XOR:
                {
                    uint64_t  tmp = registers[regB] ^ registers[regA];
                    cc=ConditionCodes::compute(AluOp::XOR,registers[regA],registers[regB],tmp);
                    registers[regB]=tmp;
                    break;
                }
                default:
                    report_bad_inst(codefun.value());
                    return Status::INS;
            }
            pc=next_pc;
            return Status::AOK;
        }

        case InstructionCode::JMP: // 7:x imm
        {
            if(cc.satisfy(static_cast<Condition>(ifun))){
                if(!memory.get_long(pc+1)){
                    report_bad_inst_addr();
                    return Status::ADR;
                }
                pc=*memory.get_long(pc+1);
            }else{
                pc=next_pc;
            }
            return Status::AOK;
        }

        case InstructionCode::CALL: // 8:0 imm
        {
            if(!memory.set_long(registers[4]-8,next_pc))
            {
                report_bad_stack_addr(registers[4]-8);
                return Status::ADR;
            }
            registers[4]-=8;
            if(!memory.get_long(pc+1)){
                report_bad_inst_addr();
                return Status::ADR;
            }
            pc=*memory.get_long(pc+1);
            return Status::AOK;
        }

        case InstructionCode::RET: // 9:0
        {
            if(!memory.get_long(registers[4])){
                report_bad_stack_addr(registers[4]);
                return Status::ADR;
            }
            pc=*memory.get_long(registers[4]);
            registers[4]+=8;
            return Status::AOK;
        }

        case InstructionCode::PUSHQ: // A:0 regA:F
        {
            if(error_invalid_reg(regA)|| error_valid_reg(regB)){
                return Status::INS;}

            if(!memory.set_long(registers[4]-8,registers[regA]))
            {
                report_bad_stack_addr(registers[4]-8);
                return Status::ADR;
            }
            registers[4]-=8;
            pc=next_pc;
            return Status::AOK;
        }

        case InstructionCode::POPQ: // B:0 regA:F
        {
            if(error_invalid_reg(regA)|| error_valid_reg(regB)){
                return Status::INS;}
            if(!memory.get_long(registers[4])){
                report_bad_stack_addr(registers[4]);
                return Status::ADR;
            }
            registers[regA] =*memory.get_long(registers[4]);
            if(regA!=4){
                registers[4]+=8;
            }
            pc=next_pc;
            return Status::AOK;
        }
        default:
            report_bad_inst(codefun.value());
            return Status::INS;// TODO: unsupported now, replace with your implementation
    }
}
