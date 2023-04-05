# Self-Modifying C++ Program Using asmjit

This project demonstrates a self-modifying C++ program that generates and executes new code at runtime without relying on external compilers. The program uses the [asmjit](https://asmjit.com/) library for Just-In-Time (JIT) compilation.

## Important Disclaimer

**Warning:** Generating and executing code at runtime can pose security risks and may cause system instability. This code is provided for educational purposes only. Use it responsibly and at your own risk!

## Requirements

- Unix-like operating system (Linux, macOS).
- C++17 or higher.
- [asmjit](https://asmjit.com/) library.
- CMake for building the project.

## Building the Project

### Clone the Repository

```bash
git clone https://github.com/michaldrozd/self-morphing.git
cd self-morphing

