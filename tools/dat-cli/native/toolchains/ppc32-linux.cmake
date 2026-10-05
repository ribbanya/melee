# 32-bit big-endian: the archives' own byte order and pointer size, as
# PowerPC Linux binaries run under qemu's user mode (the native dev shell
# has both). Linked statically, so qemu needs no sysroot.
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR ppc)
set(CMAKE_C_COMPILER powerpc-unknown-linux-gnu-gcc)
set(CMAKE_EXE_LINKER_FLAGS_INIT "-static")
find_program(QEMU_PPC qemu-ppc REQUIRED)
set(CMAKE_CROSSCOMPILING_EMULATOR "${QEMU_PPC}")
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
