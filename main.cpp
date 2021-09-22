// Warning: Generating and executing code at runtime can pose security risks and may cause system instability. This code is provided for educational purposes only. Use it responsibly and at your own risk!
#include <iostream>
#include <chrono>
#include <thread>
#include <random>
#include <asmjit/asmjit.h>
#include <sys/mman.h>
#include <unistd.h>

using namespace asmjit;

typedef void (*Func)();

void shuffle_memory(uint8_t* start, size_t size) {
    std::random_device rd;
    std::mt19937 gen(rd());
    for (size_t i = size - 1; i > 0; i--) {
        std::uniform_int_distribution<> dis(0, i);
        size_t j = dis(gen);
        std::swap(start[i], start[j]);
    }
}

class StealthCodeHolder : public CodeHolder {
public:
    void obfuscate() {
        uint8_t* code_ptr = (uint8_t*)_allocator.data();
        size_t code_size = _allocator.codeSize();
        shuffle_memory(code_ptr, code_size);
    }
};

int main() {
    // Randomize stack location
    alloca(rand() % 1024);

    uint32_t version = time(nullptr) % 10000;

    while (true) {
        // Random sleep to avoid patterns
        std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 70000 + 10000));

        JitRuntime rt;
        StealthCodeHolder code;
        code.init(rt.environment());

        x86::Assembler a(&code);

        // Add random NOPs for timing variation
        for(int i = 0; i < rand() % 10; i++) {
            a.nop();
        }

        Label messageLabel = a.newLabel();

        // Randomize register usage
        x86::Gp regs[] = {x86::rax, x86::rbx, x86::rcx, x86::rdx};
        std::random_shuffle(std::begin(regs), std::end(regs));

        #ifndef _WIN32
        // Add some junk code
        if(rand() % 2) {
            a.push(regs[0]);
            a.xor_(regs[0], regs[0]);
            a.pop(regs[0]);
        }

        // Actual functionality hidden among junk
        a.mov(regs[0], 1);
        a.mov(x86::rdi, 1);
        a.lea(x86::rsi, messageLabel);

        // More obfuscation
        if(rand() % 2) {
            a.push(regs[1]);
            a.mov(regs[1], 25);
            a.mov(x86::rdx, regs[1]);
            a.pop(regs[1]);
        } else {
            a.mov(x86::rdx, 25);
        }

        a.mov(x86::rax, 1);

        // Insert random jumps
        Label l1 = a.newLabel();
        Label l2 = a.newLabel();

        if(rand() % 2) {
            a.jmp(l1);
            a.bind(l2);
            a.nop();
            a.bind(l1);
        }

        a.syscall();

        // Randomize exit
        if(rand() % 2) {
            a.mov(x86::rax, 60);
            a.xor_(x86::rdi, x86::rdi);
        } else {
            a.mov(regs[0], 60);
            a.mov(x86::rax, regs[0]);
            a.mov(x86::rdi, 0);
        }

        a.syscall();
        #endif

        a.bind(messageLabel);
        std::string msg = "Version " + std::to_string(version) + "\n";
        a.embed(msg.c_str(), msg.size());

        // Obfuscate the code
        code.obfuscate();

        Func fn;
        Error err = rt.add(&fn, &code);

        if (!err) {
            // Protect against debugging
            if(ptrace(PTRACE_TRACEME, 0, 1, 0) == -1) {
                _exit(1);
            }

            // Execute with random timing
            std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 100));
            fn();
        }

        rt.release(fn);
        version = (version * 1664525 + 1013904223) % 10000; // LCG for version

        // Clean up traces
        memset(&code, 0, sizeof(code));
    }

    return 0;
}
