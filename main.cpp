#include <iostream>
#include <chrono>
#include <thread>
#include <asmjit/asmjit.h>

using namespace asmjit;

// Function signature typedef
typedef void (*Func)();

int main() {
    int version = 1;

    while (true) {
        std::cout << "Running version " << version << std::endl;

        // JIT runtime
        JitRuntime rt;

        // CodeHolder to hold the generated code
        CodeHolder code;
        code.init(rt.environment());

        // Create an assembler
        x86::Assembler a(&code);

        // Label for the message
        Label messageLabel = a.newLabel();

        // Generate code to print a message using syscall (Linux x86_64)
#ifdef _WIN32
        // Windows specific code (requires MessageBox or similar)
        std::cerr << "This example is for Unix-like systems." << std::endl;
        return 1;
#else
        // Linux syscall write to stdout
        // ssize_t write(int fd, const void *buf, size_t count);
        a.mov(x86::rax, 1);                      // syscall number for write
        a.mov(x86::rdi, 1);                      // fd (stdout)
        a.lea(x86::rsi, messageLabel);           // buffer
        a.mov(x86::rdx, 25);                     // count
        a.mov(x86::rax, 1);                      // syscall: sys_write
        a.syscall();                             // call kernel

        // Exit syscall
        a.mov(x86::rax, 60);                     // syscall: sys_exit
        a.xor_(x86::rdi, x86::rdi);              // status = 0
        a.syscall();                             // call kernel
#endif

        // Data section
        a.bind(messageLabel);
        std::string message = "Hello from version " + std::to_string(version) + "!\n";
        a.embed(message.c_str(), message.size());

        // Compile the code
        Func fn;
        Error err = rt.add(&fn, &code);

        if (err) {
            std::cerr << "Error: " << DebugUtils::errorAsString(err) << std::endl;
            return 1;
        }

        // Execute the generated function
        fn();

        // Release the code
        rt.release(fn);

        version++;

        // Wait for a minute
        std::this_thread::sleep_for(std::chrono::minutes(1));
    }

    return 0;
}

