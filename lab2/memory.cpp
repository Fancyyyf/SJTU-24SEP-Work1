#include "memory.h"

#include <format>
#include <fstream>
#include <iostream>
#include <stdexcept>

Memory::Memory() : data() {
}

void Memory::load_binary(const char *filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Can't open binary file");
    }
    file.read(reinterpret_cast<char *>(data.data()), data.size());
    if (file.bad()) {
        //检查是否发生了严重错误（比如硬盘损坏、系统I/O错误等）。
        //和 fail() 不同，它不是逻辑失败（如类型转换失败），而是“真出错”
        throw std::runtime_error("Failed to load binary file");
    }
    if (!file.eof()) {
        //检查文件是否读到了末尾。
        throw std::runtime_error("too large memory footprint");
    }
}

std::optional<uint8_t> Memory::get_byte(const uint64_t addr) const {
    //读取某个地址上的操作，大小仅一个字节
    if (addr >= data.size()) {
        return std::nullopt;
    }
    return data[addr];
}

bool Memory::set_byte(const uint64_t addr, const uint8_t value) {
    //某个地址上写入某个量，大小为一字节
    if (addr >= data.size()) {
        return false;
    }
    data[addr] = value;
    return true;
}

std::optional<uint64_t> Memory::get_long(const uint64_t addr) const {
    //按照 Y86-64 的小端序，读取 8 字节。
    if (addr >= data.size() || addr + 8 >= data.size()) {
        return std::nullopt;
    }//检查越界，八个字节都在内
    uint64_t result = 0;//一个字节为8位整数，读八个字节，故类型为64位整数
    for (int i = 0; i < 8; i++) {
        result |= static_cast<uint64_t>(data[addr + i]) << (8 * i);
        //|=表示按位或运算，
        //<< (8 * i)表示位移操作符
    }
    return result;
}

bool Memory::set_long(const uint64_t addr, uint64_t value) {
    //按照 Y86-64 的小端序，写入 8 字节。
    if (addr >= data.size() || addr + 8 >= data.size()) {
        return false;
    }
    for (int i = 0; i < 8; i++) {
        data[addr + i] = value & 0xFF;
        //value & 0xFF 的意思是：取出 value 的最低 8 位（也就是最低字节）。
        value >>= 8;
        //value >>= 8;
        //把 value 右移 8 位，也就是把刚刚提取的最低字节“丢掉”。
    }
    return true;
}

void Memory::diff(const Memory &m0, const Memory &m1) {
    //逐个比较两个 Memory 对象中每8字节的内容，如果不同，就打印出地址和这两个值的差异。
    for (uint64_t addr = 0; addr < m0.data.size(); addr += 8) {
        if (uint64_t v0 = *m0.get_long(addr), v1 = *m1.get_long(addr); v0 != v1) {
            std::cout << std::format("0x{:016x}:\t0x{:016x}\t0x{:016x}\n",
                                     addr, v0, v1);
        }
    }
}
