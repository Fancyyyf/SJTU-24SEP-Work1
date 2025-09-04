#include "instructions.h"

#include <stdexcept>

bool need_regids(const InstructionCode icode) {
    // TODO 返回指令是否含有寄存器 ID
    switch (icode) {
        case InstructionCode::HALT:
        case InstructionCode::NOP : return false;
        case InstructionCode::RRMOVQ :
        case InstructionCode::IRMOVQ :
        case InstructionCode::RMMOVQ :
        case InstructionCode::MRMOVQ :
        case InstructionCode::ALU :return true;
        case InstructionCode::JMP :
        case InstructionCode::CALL :
        case InstructionCode::RET :return false;
        case InstructionCode::PUSHQ :
        case InstructionCode::POPQ :return true;
        default:
            throw std::logic_error("instructions.cpp: need_regids() not implemented");
    }
}

bool need_imm(const InstructionCode icode) {
    // TODO 返回指令是否含有立即数
    switch (icode) {
        case InstructionCode::HALT:
        case InstructionCode::NOP :
        case InstructionCode::RRMOVQ :return false;
        case InstructionCode::IRMOVQ :
        case InstructionCode::RMMOVQ :
        case InstructionCode::MRMOVQ :return true;
        case InstructionCode::ALU :return false;
        case InstructionCode::JMP :
        case InstructionCode::CALL :return true;
        case InstructionCode::RET :
        case InstructionCode::PUSHQ :
        case InstructionCode::POPQ :return false;
        default:
            throw std::logic_error("instructions.cpp: need_imm() not implemented");
    }
}

ConditionCodes::ConditionCodes()
    : ConditionCodes(true, false, false) {
}

ConditionCodes::ConditionCodes(const bool zf, const bool sf, const bool of)
    : value((zf << 2) | (sf << 1) | (of << 0)) {
}

static bool is_overflow(const AluOp op, const int64_t a, const int64_t b, const int64_t val) {
    switch (op) {
        case AluOp::ADD:
            return ((a < 0) == (b < 0)) && ((val < 0) != (a < 0));//第一个判断同号，第二个判断结果与原数不同号，代表溢出
        case AluOp::SUB:
            return ((a > 0) == (b < 0)) && ((val < 0) != (b < 0));
        default:
            return false;
    }
}

ConditionCodes ConditionCodes::compute(const AluOp op, const uint64_t a, const uint64_t b, const uint64_t val) {
    bool zero = (val == 0);
    bool sign = (static_cast<int64_t>(val) < 0);
    bool overflow = is_overflow(op, static_cast<int64_t>(a), static_cast<int64_t>(b), static_cast<int64_t>(val));

    return {zero, sign, overflow};
}

bool ConditionCodes::satisfy(const Condition cond) const {
    const bool zf = (value >> 2) & 1;
    const bool sf = (value >> 1) & 1;
    const bool of = (value >> 0) & 1;

    switch (cond) {
        //^代表异或，|代表按位或，~为取反
        case Condition::YES://无条件跳转
            return true;
        case Condition::LE://负或0，为正溢出不可能
            return (sf ^ of) | zf;
        case Condition::L://负
            return sf ^ of;
        case Condition::E://0
            return zf;
        case Condition::NE://不为0
            return zf ^ 1;
        case Condition::GE://大于等于0
            return sf ^ of ^ 1;
        case Condition::G://大于0
            return (sf ^ of ^ 1) & (zf ^ 1);
    }
    return false;
}

const char *ConditionCodes::name() const {
    switch (value) {
        case 0: return "Z=0 S=0 O=0";
        case 1: return "Z=0 S=0 O=1";
        case 2: return "Z=0 S=1 O=0";
        case 3: return "Z=0 S=1 O=1";
        case 4: return "Z=1 S=0 O=0";
        case 5: return "Z=1 S=0 O=1";
        case 6: return "Z=1 S=1 O=0";
        case 7: return "Z=1 S=1 O=1";
        default: return "???????????";
    }
}
