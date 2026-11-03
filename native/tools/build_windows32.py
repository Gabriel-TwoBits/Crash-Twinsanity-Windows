#!/usr/bin/env python3
"""The native build for 32 bit Windows, cross-compiled on Linux (native/WINDOWS32.md): clang (21 or later) for
i686-w64-mingw32 with the distribution's MinGW-w64 headers, runtime and libstdc++ (Debian: g++-mingw-w64-i686-posix) and lld.
Builds SDL3 from its source release and an LGPL FFmpeg (native/tools/build_ffmpeg.sh x86 windows) when they aren't there, then
configures and builds build/native/cmake-windows-x86/CrashTwinsanity.exe, with SDL's and FFmpeg's DLLs and the UI's fonts
beside it.

    .venv/bin/python native/tools/build_windows32.py [--configure] [--clean] [ninja arguments...]

The split (tools/split.py) has to have been made first: the build converts its data. Its generated files (the data as C++,
laid out for 32 bit pointers) go in build/native/windows-x86, apart from the 64 bit builds' build/native.
"""
import argparse
import os
import platform
import shutil
import subprocess
import sys
from pathlib import Path

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import build_linux  # noqa: E402

HERE = build_linux.HERE
DEPS = build_linux.DEPS
TRIPLE = "i686-w64-mingw32"
BUILD = HERE / "build" / "native" / "cmake-windows-x86"
GENERATED = HERE / "build" / "native" / "windows-x86"
FFMPEG = HERE / "build" / "native" / "ffmpeg" / "windows-x86"
SDL_PREFIX = DEPS / f"SDL3-{build_linux.SDL_VERSION}-windows-x86"
TOOLCHAIN = HERE / "native" / "toolchain-mingw-i686.cmake"
# What the MinGW-w64 cross toolchain and lld are installed by on Debian
PACKAGES = "g++-mingw-w64-i686-posix lld"


def check_cross_toolchain():
    """The MinGW-w64 cross toolchain's resource compiler and libstdc++ (its posix flavour: std::thread), and lld"""
    gcc = Path("/usr/lib/gcc") / TRIPLE
    posix = sorted(gcc.glob("*-posix")) if gcc.exists() else []
    if shutil.which(f"{TRIPLE}-windres") is None or not posix:
        sys.exit(f"No MinGW-w64 cross toolchain for {TRIPLE} (posix threads): sudo apt install {PACKAGES}")
    if shutil.which("ld.lld") is None:
        sys.exit(f"No lld: sudo apt install {PACKAGES}")


def own_sdl(cmake, ninja, clang, clangxx):
    """SDL3 cross-compiled from its source release into SDL_PREFIX when it isn't there"""
    if (SDL_PREFIX / "lib" / "cmake" / "SDL3" / "SDL3Config.cmake").exists():
        return SDL_PREFIX
    source = build_linux.sdl_source()
    build = DEPS / f"SDL3-{build_linux.SDL_VERSION}-build-windows-x86"
    subprocess.run([cmake, "-S", str(source), "-B", str(build), "-G", "Ninja", f"-DCMAKE_MAKE_PROGRAM={ninja}",
                    f"-DCMAKE_TOOLCHAIN_FILE={TOOLCHAIN}", f"-DCMAKE_C_COMPILER={clang}", f"-DCMAKE_CXX_COMPILER={clangxx}",
                    "-DCMAKE_BUILD_TYPE=Release", f"-DCMAKE_INSTALL_PREFIX={SDL_PREFIX}", "-DCMAKE_INSTALL_LIBDIR=lib",
                    "-DSDL_TESTS=OFF", "-DSDL_EXAMPLES=OFF"], check=True)
    subprocess.run([cmake, "--build", str(build), "--target", "install"], check=True)
    return SDL_PREFIX


def own_ffmpeg(clang):
    """Our decode-only LGPL 2.1 FFmpeg's DLLs for 32 bit Windows (native/tools/build_ffmpeg.sh), built when they aren't there"""
    if (FFMPEG / "include" / "libavcodec" / "avcodec.h").exists():
        return
    for tool, package in (("make", "make"), ("curl", "curl")):
        build_linux.find_tool(tool, package)
    if shutil.which("nasm") is None:
        print("no nasm: FFmpeg's x86 assembly will be off (sudo apt install nasm)", flush=True)
    env = dict(os.environ)
    env["CC"] = clang
    print("building FFmpeg (native/tools/build_ffmpeg.sh x86 windows)", flush=True)
    subprocess.run(["sh", str(HERE / "native" / "tools" / "build_ffmpeg.sh"), "x86", "windows"], cwd=HERE, env=env,
                   check=True)


def main():
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--configure", action="store_true", help="configure again")
    parser.add_argument("--clean", action="store_true", help="delete the build first")
    parser.add_argument("ninja", nargs=argparse.REMAINDER)
    args = parser.parse_args()

    if platform.system() != "Linux":
        sys.exit("The 32 bit Windows build is cross-compiled on Linux (native/WINDOWS32.md)")
    if not (HERE / "asm" / "data").exists():
        sys.exit("No split: run tools/split.py first (README.md)")

    found = build_linux.find_clang()
    if found is None:
        sys.exit(f"No clang {build_linux.MIN_CLANG_MAJOR} or later on the PATH: install it (Debian 13: sudo apt install -t "
                 f"trixie-backports clang-{build_linux.MIN_CLANG_MAJOR}), native/WINDOWS32.md")
    clang, clangxx = found
    check_cross_toolchain()
    cmake = build_linux.find_tool("cmake", "cmake")
    ninja = build_linux.find_tool("ninja", "ninja-build")
    sdl = own_sdl(cmake, ninja, clang, clangxx)
    own_ffmpeg(clang)

    if args.clean and BUILD.exists():
        shutil.rmtree(BUILD)
    if args.configure or not (BUILD / "build.ninja").exists():
        subprocess.run([cmake, "-S", str(HERE / "native"), "-B", str(BUILD), "-G", "Ninja", f"-DCMAKE_MAKE_PROGRAM={ninja}",
                        f"-DCMAKE_TOOLCHAIN_FILE={TOOLCHAIN}", f"-DCMAKE_CXX_COMPILER={clangxx}",
                        f"-DCMAKE_PREFIX_PATH={sdl}", f"-DTWIN_FFMPEG_DIR={FFMPEG}",
                        f"-DTWIN_GENERATED_DIR={GENERATED}", f"-DPython3_EXECUTABLE={sys.executable}"], check=True)
    result = subprocess.run([cmake, "--build", str(BUILD), "--"] + args.ninja)
    if result.returncode == 0:
        print(f"built {BUILD / 'CrashTwinsanity.exe'}")
    sys.exit(result.returncode)


if __name__ == "__main__":
    main()
