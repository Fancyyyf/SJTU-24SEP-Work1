#pragma once

#include <cstdint>
#include <optional>
#include <array>
//保存了程序的指令和数据。可以想象成一个巨大的 byte 数组。
//用于读取文件进行操作
class Memory {
public:
    Memory();

    /** 加载二进制文件。 */
    void load_binary(const char *filename);

    /**
     * 读取 1 字节。
     * @return 空值表示地址越界。
     */
    [[nodiscard]]
    std::optional<uint8_t> get_byte(uint64_t addr) const;

    /**
     * 写入 1 字节。
     * @return true 表示写入成功，false 表示地址越界。
     */
    [[nodiscard]]
    bool set_byte(uint64_t addr, uint8_t value);

    /**
     * 按照 Y86-64 的小端序，读取 8 字节。
     * @return 空值表示地址越界。
     */
     [[nodiscard]]
    std::optional<uint64_t> get_long(uint64_t addr) const;

    /**
     * 按照 Y86-64 的小端序，写入 8 字节。
     * @return true 表示写入成功，false 表示地址越界。
     */
    [[nodiscard]]
    bool set_long(uint64_t addr, uint64_t value);

    /** 输出内存的变动。 */
    static void diff(const Memory &m0, const Memory &m1);

private:
    static constexpr uint64_t MEM_SIZE = 8192;
    //本次lab默认内存的大小定为 8 KiB，只有在这个范围内的内存地址才是有效的。
    //constexpr意思是默认编译时即可确定的常数

    std::array<uint8_t, MEM_SIZE> data;
};
