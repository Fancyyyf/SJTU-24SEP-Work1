#include <iostream>
#include <string>
#include <string_view>

#include "simulator.h"

static constexpr int MAX_STEP = 10000;//最大步骤数？

static void usage(const char *program) {
    //打印程序的使用方法，提示用户需要提供文件名和可选的最大步数。
    std::cout << "Usage: " << program << " file.bin [max_steps]\n";
}

int main(const int argc, char *argv[]) {
    if (argc < 2 || argc > 3) {
        usage(argv[0]);
        return 0;
        //程序需要 1~2 个参数（除了 argv[0]）：
        //file.bin：必须
        //max_steps：可选
        //如果参数不对，就打印用法并退出。
    }

    const int max_steps = argc > 2 ? std::stoi(argv[2]) : MAX_STEP;//std::stoi 是 C++ 标准库中的一个函数，用于将字符串转换为整数。它属于 <string> 头文件。
    //如果用户提供了第三个参数（argv[2]），用 std::stoi 把它转成整数，表示最大运行步数；
    //否则用默认的 MAX_STEP = 10000。

    const char *filename = argv[1];
    //argv[1] 是文件名，比如 "prog.bin"。
    if (not std::string_view(filename).ends_with(".bin")) {
        //检查文件名是否以 .bin 结尾；
        //如果不是，视为无效输入，提示用法。
        usage(argv[0]);
        return 0;
    }

    Simulator sim;
    sim.load_binary(filename);
    //创建一个 Simulator 对象。
    //加载 .bin 文件到模拟器内部（比如把程序、内存、寄存器等初始化）。

    const Registers save_registers = sim.get_registers();
    //保存运行前的状态，便于后面对比运行后的结果；
    const Memory save_memory = sim.get_memory();

    sim.run(max_steps);//开始运行

//输出状态差异
    std::cout << "Changes to registers:\n";
    Registers::diff(save_registers, sim.get_registers());

    std::cout << "\nChanges to memory:\n";
    Memory::diff(save_memory, sim.get_memory());

    return 0;
}
