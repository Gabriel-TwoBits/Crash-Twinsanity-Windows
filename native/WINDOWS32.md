# The native build for 32 bit Windows

The same game C++ and platform layer as the 64 bit Windows build (native/WINDOWS.md), built for 32 bit x86
(`i686-w64-mingw32`): for 32 bit Windows, Windows 7 SP1 included, on a processor with SSE4.2 (Intel's Core i since 2008,
AMD's since 2011). It runs on 64 bit Windows too, where the 64 bit build is the better choice: x86-64's twice as many
registers make it faster.

It is cross-compiled on Linux with clang and the distribution's MinGW-w64, whose C runtime is the system's `msvcrt.dll`
(Windows 7 has it; the 64 bit build's UCRT needs an update there).

**Tested:** in Wine 10 (Debian 13), on the 64 bit host with the 32 bit Windows program: the self-tests (`--selftest-abi`,
`--selftest-lowmemory`) and levels played with the level sweep's scripted input (native/LEVEL_SWEEP.md).
**Not tested yet:** a real Windows 7, and Intel's OpenGL driver.

## Building

You need about 3 GB of disk space, your PAL disc image (SLES-52568) and an internet connection for the first build.

1. **Install the tools** (Debian 13; clang 21 is in trixie-backports):

   ```sh
   sudo apt install -t trixie-backports clang-21
   sudo apt install g++-mingw-w64-i686-posix lld cmake ninja-build make curl nasm python3-venv
   ```

   `g++-mingw-w64-i686-posix` brings the MinGW-w64 headers, its runtime, libstdc++ with `std::thread` (the posix flavour) and
   `i686-w64-mingw32-windres`. GCC itself isn't used: the layout tools need clang.

2. **The PS2 toolchain, the Python environment, the game's executable and the split**: as on Linux (native/LINUX.md, steps 2
   and 3).

3. **Build** (the first time it also builds SDL3 and FFmpeg for 32 bit Windows):

   ```sh
   .venv/bin/python native/tools/build_windows32.py
   ```

   It builds SDL3 from its source release into `build/native/deps/SDL3-<version>-windows-x86`, FFmpeg's DLLs into
   `build/native/ffmpeg/windows-x86` (`native/tools/build_ffmpeg.sh x86 windows`), and the program into
   `build/native/cmake-windows-x86` with the DLLs and fonts beside it. The generated files (the retail data laid out for 32 bit
   pointers) go in `build/native/windows-x86`, apart from the 64 bit builds' `build/native`, so both can be built from one tree.

4. **Package** (the zip the player unpacks, `build/native/dist/Crash-Twinsanity-Windows-32bit.zip`):

   ```sh
   .venv/bin/python native/tools/package_windows.py --build build/native/cmake-windows-x86 --zip Crash-Twinsanity-Windows-32bit.zip
   ```

5. **Try it in Wine** (`sudo apt install wine wine32:i386`):

   ```sh
   wine build/native/cmake-windows-x86/CrashTwinsanity.exe --iso "Z:\path\to\Crash Twinsanity (Europe).iso"
   ```

## What 32 bit Windows needed

| What | Why | How |
| --- | --- | --- |
| Methods' convention (common.h's `RETAIL_CC`) | 32 bit MinGW's methods are thiscall (the object in ecx), but a retail vtable's or function pointer's function is called as a plain function, the object first (`CallVirtual`, `Abi::CallEabi`) | Every retail function is cdecl: `RETAIL` gives it the attribute, and a template's members that explicit specializations give retail names (pools.h) and pointers to retail methods (instancesection.cpp's `AddElement`) declare it |
| Floats among integers (abi.h's `TWIN_EABI_CC`) | x86's own conventions pass every argument on the stack in its position: the EABI's way of filling the integers' and floats' registers each in their order, which the retail vtables' callers count on (native/WINDOWS.md) | `RETAIL_N32`'s functions take clang's regcall: integers in eax, ecx, edx, edi, esi and floats in xmm0-7, each in their order. `Abi::CallEabi` and the tables of native/tools/sysv_functions.py work as on x64 |
| Symbols | 32 bit Windows' C symbols start with an underscore, and a name starting with L is the assembler's local label (`LoadTexture` wouldn't link) | `RETAIL` puts the underscore in front, as on macOS; the tools (data_types.py, make_stubs.py) and the generated C++ (`SYMBOL`) take it off and put it back |
| OpenGL's convention (hw/gl.h's `TWIN_GL_API`) | 32 bit Windows' OpenGL is stdcall: the callee takes its arguments off the stack, and a cdecl call through the loaded pointers left the stack wrong at every call | The loaded entry points' types take stdcall there |
| Pointers of 4 bytes (native/tools) | The data converter laid out pointers, pointer-sized integers and vtable entries at 64 bits' sizes | `host.POINTER_BYTES` from the compiler's target: pointers and `uiptr` of 4 bytes, copied as the PS2 has them (nothing widened), vtable entries of 8 bytes |
| The tools' target (native/tools/host.py) | The tools ran clang for the machine they ran on, and wrote into build/native | CMake gives them the compiler's target (`TWIN_TARGET`, passed to clang as `--target`) and a folder of their own (`TWIN_GENERATED`) |
| 128 bit integers (tamtypes.h) | 32 bit x86 has no `__int128` | `u128` and `s128` are 16 bytes' storage there, aligned to 16 (nothing does arithmetic on them) |
| Floats | The x87's 80 bit intermediates round differently from the PS2's floats | `-msse4.2 -mfpmath=sse`: SSE's single precision, as the 64 bit build's |
| The retail code's null pointers (lowmemory_win32.cpp) | x86's decoding differs: no REX (40-4F are INC and DEC), ModRM's mod 0 r/m 5 an absolute address (not RIP-relative), 4 byte moffs, the address size prefix 16 bit addressing | The copy of the faulting instruction decoded the 32 bit way, its displacement moved modulo 2^32, then a `jmp rel32` back |
| The crash report and the hang watch | No unwinding tables, and the build omits frame pointers | The registers are x86's; the backtrace scans the stack for return addresses (a value in a module's code just after a call), which may list a stale one |
| The profiler (`$TWIN_PROFILE`) | No unwinding tables | The innermost frame only: a flat profile |
| `printf` (libc.cpp) | MinGW's C99 stdio (on with msvcrt) defines `printf` inline in its header | The PS2 C library's `printf` is left out there: the native `kputs` writes to standard output, as MinGW's does |
| Thread-local variables (toolchain-mingw-i686.cmake) | The distribution's libstdc++ was built by GCC, which emulates them on MinGW | `-femulated-tls` |

The toolchain file (`native/toolchain-mingw-i686.cmake`) links with lld, the C++ runtime and winpthreads statically: the
program needs only Windows' own DLLs, SDL's and FFmpeg's (which need only Windows' own: libgcc linked in).
